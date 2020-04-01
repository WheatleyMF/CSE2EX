#include "../Platform.h"

#include <chrono>
#include <stddef.h>
#include <stdlib.h>
#include <thread>

#include <GLFW/glfw3.h>

#include "../Rendering.h"

#include "../../WindowsWrapper.h"

#include "../../Bitmap.h"
#include "../../KeyControl.h"
#include "../../Main.h"
#include "../../Organya.h"
#include "../../Profile.h"
#include "../../Resource.h"

BOOL bActive = TRUE;

extern GLFWwindow *window;

static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	(void)window;
	(void)scancode;
	(void)mods;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				gKey |= KEY_ESCAPE;
				break;

			case GLFW_KEY_W:
				gKey |= KEY_MAP;
				break;

			case GLFW_KEY_LEFT:
				gKey |= KEY_LEFT;
				break;

			case GLFW_KEY_RIGHT:
				gKey |= KEY_RIGHT;
				break;

			case GLFW_KEY_UP:
				gKey |= KEY_UP;
				break;

			case GLFW_KEY_DOWN:
				gKey |= KEY_DOWN;
				break;

			case GLFW_KEY_X:
				gKey |= KEY_X;
				break;

			case GLFW_KEY_Z:
				gKey |= KEY_Z;
				break;

			case GLFW_KEY_S:
				gKey |= KEY_ARMS;
				break;

			case GLFW_KEY_A:
				gKey |= KEY_ARMSREV;
				break;

			case GLFW_KEY_LEFT_SHIFT:
			case GLFW_KEY_RIGHT_SHIFT:
				gKey |= KEY_SHIFT;
				break;

			case GLFW_KEY_F1:
				gKey |= KEY_F1;
				break;

			case GLFW_KEY_F2:
				gKey |= KEY_F2;
				break;

			case GLFW_KEY_Q:
				gKey |= KEY_ITEM;
				break;

			case GLFW_KEY_COMMA:
				gKey |= KEY_ALT_LEFT;
				break;

			case GLFW_KEY_PERIOD:
				gKey |= KEY_ALT_DOWN;
				break;

			case GLFW_KEY_SLASH:
				gKey |= KEY_ALT_RIGHT;
				break;

			case GLFW_KEY_L:
				gKey |= KEY_L;
				break;

			case GLFW_KEY_EQUAL:
				gKey |= KEY_PLUS;
				break;

			case GLFW_KEY_F5:
				gbUseJoystick = FALSE;
				break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
			case GLFW_KEY_ESCAPE:
				gKey &= ~KEY_ESCAPE;
				break;

			case GLFW_KEY_W:
				gKey &= ~KEY_MAP;
				break;

			case GLFW_KEY_LEFT:
				gKey &= ~KEY_LEFT;
				break;

			case GLFW_KEY_RIGHT:
				gKey &= ~KEY_RIGHT;
				break;

			case GLFW_KEY_UP:
				gKey &= ~KEY_UP;
				break;

			case GLFW_KEY_DOWN:
				gKey &= ~KEY_DOWN;
				break;

			case GLFW_KEY_X:
				gKey &= ~KEY_X;
				break;

			case GLFW_KEY_Z:
				gKey &= ~KEY_Z;
				break;

			case GLFW_KEY_S:
				gKey &= ~KEY_ARMS;
				break;

			case GLFW_KEY_A:
				gKey &= ~KEY_ARMSREV;
				break;

			case GLFW_KEY_LEFT_SHIFT:
			case GLFW_KEY_RIGHT_SHIFT:
				gKey &= ~KEY_SHIFT;
				break;

			case GLFW_KEY_F1:
				gKey &= ~KEY_F1;
				break;

			case GLFW_KEY_F2:
				gKey &= ~KEY_F2;
				break;

			case GLFW_KEY_Q:
				gKey &= ~KEY_ITEM;
				break;

			case GLFW_KEY_COMMA:
				gKey &= ~KEY_ALT_LEFT;
				break;

			case GLFW_KEY_PERIOD:
				gKey &= ~KEY_ALT_DOWN;
				break;

			case GLFW_KEY_SLASH:
				gKey &= ~KEY_ALT_RIGHT;
				break;

			case GLFW_KEY_L:
				gKey &= ~KEY_L;
				break;

			case GLFW_KEY_EQUAL:
				gKey &= ~KEY_PLUS;
				break;
		}
	}
}

static void WindowFocusCallback(GLFWwindow *window, int focused)
{
	(void)window;

	if (focused)
		ActiveWindow();
	else
		InactiveWindow();
}

static void WindowSizeCallback(GLFWwindow *window, int width, int height)
{
	(void)window;

	Backend_HandleWindowResize(width, height);
}

void PlatformBackend_Init(void)
{
	glfwInit();
}

void PlatformBackend_Deinit(void)
{
	glfwTerminate();
}

void PlatformBackend_PostWindowCreation(void)
{
	// Hook callbacks
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetWindowFocusCallback(window, WindowFocusCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);

	// Set up window icon

	// TODO - GLFW_ICON
#ifndef _WIN32	// On Windows, we use native icons instead (so we can give the taskbar and window separate icons, like the original EXE does)
	size_t resource_size;
	const unsigned char *resource_data = FindResource("ICON_MINI", "ICON", &resource_size);

	unsigned int width, height;
	unsigned char *rgb_pixels = DecodeBitmap(resource_data, resource_size, &width, &height);

	if (rgb_pixels != NULL)
	{
		unsigned char *rgba_pixels = (unsigned char*)malloc(width * height * 4);

		unsigned char *rgb_pointer = rgb_pixels;
		unsigned char *rgba_pointer = rgba_pixels;

		if (rgba_pixels != NULL)
		{
			for (unsigned int y = 0; y < height; ++y)
			{
				for (unsigned int x = 0; x < width; ++x)
				{
					*rgba_pointer++ = *rgb_pointer++;
					*rgba_pointer++ = *rgb_pointer++;
					*rgba_pointer++ = *rgb_pointer++;
					*rgba_pointer++ = 0xFF;
				}
			}

			GLFWimage glfw_image = {(int)width, (int)height, rgba_pixels};
			glfwSetWindowIcon(window, 1, &glfw_image);

			free(rgba_pixels);
		}

		FreeBitmap(rgb_pixels);
	}
#endif
}

BOOL PlatformBackend_GetBasePath(char *string_buffer)
{
	return FALSE;
}

BOOL PlatformBackend_SystemTask(void)
{
	if (glfwWindowShouldClose(window))
	{
		StopOrganyaMusic();
		return FALSE;
	}

	while (!bActive)
		glfwWaitEvents();

	glfwPollEvents();
/*
	while (SDL_PollEvent(NULL) || !bActive)
	{
		SDL_Event event;

		if (!SDL_WaitEvent(&event))
			return FALSE;

		switch (event.type)
		{
			case SDL_DROPFILE:
				LoadProfile(event.drop.file);
				SDL_free(event.drop.file);
				break;


			case SDL_RENDER_TARGETS_RESET:
				Backend_HandleRenderTargetLoss();
				break;

		}
	}
*/
	return TRUE;
}

void PlatformBackend_ShowMessageBox(const char *title, const char *message)
{
	//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, message, NULL);
}

unsigned long PlatformBackend_GetTicks(void)
{
	return (unsigned long)(glfwGetTime() * 1000.0);
}

void PlatformBackend_Delay(unsigned int ticks)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ticks));
}
