#include "Back.h"

#include <stddef.h>
#include <stdio.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Main.h"

BACK gBack;
int gWaterY;
static unsigned long color_black;

// TODO - Another function that has an incorrect stack frame
BOOL InitBack(const char *fName, int type)
{
	// Unused
	color_black = GetCortBoxColor(RGB(0, 0, 0x10));

	// Get width and height
	char path[MAX_PATH];
	sprintf(path, "%s\\%s.pbm", gDataPath, fName);

	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return FALSE;

	// This is ridiculously platform-dependant:
	// It should break on big-endian CPUs, and platforms where short isn't 16-bit and long isn't 32-bit.
	unsigned short bmp_header_buffer[7];	// These names aren't the original. This ruins the stack frame layout.
	unsigned long bmp_header_buffer2[10];

	fread(bmp_header_buffer, 14, 1, fp);

	// Check if this is a valid bitmap file
	if (bmp_header_buffer[0] != 0x4D42)	// 'MB' (we use hex to prevent a compiler warning)
	{
#ifdef FIX_BUGS
		// The original game forgets to close fp
		fclose(fp);
#endif
		return FALSE;
	}

	fread(bmp_header_buffer2, 40, 1, fp);
	fclose(fp);

	gBack.partsW = bmp_header_buffer2[1];
	gBack.partsH = bmp_header_buffer2[2];

	// Set background stuff and load texture
	gBack.flag = TRUE;
	if (!ReloadBitmap_File(fName, SURFACE_ID_LEVEL_BACKGROUND))
		return FALSE;

	gBack.type = type;
	gWaterY = 240 * 0x10 * 0x200;
	return TRUE;
}

void ActBack(void)
{
	switch (gBack.type)
	{
		case 5:
			gBack.fx += 6 * 0x200;
			break;

		case 6:
		case 7:
			++gBack.fx;
			gBack.fx %= 640;
			break;
	}
}

void PutBack(int fx, int fy)
{
	int x, y;
	RECT rect = {0, 0, gBack.partsW, gBack.partsH};

	switch (gBack.type)
	{
		case 0:
			for (y = 0; y < WINDOW_HEIGHT; y += gBack.partsH)
				for (x = 0; x < WINDOW_WIDTH; x += gBack.partsW)
					PutBitmap4(&grcGame, x, y, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			break;

		case 1:
			for (y = -((fy / 2 / 0x200) % gBack.partsH); y < WINDOW_HEIGHT; y += gBack.partsH)
				for (x = -((fx / 2 / 0x200) % gBack.partsW); x < WINDOW_WIDTH; x += gBack.partsW)
					PutBitmap4(&grcGame, x, y, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			break;

		case 2:
			for (y = -((fy / 0x200) % gBack.partsH); y < WINDOW_HEIGHT; y += gBack.partsH)
				for (x = -((fx / 0x200) % gBack.partsW); x < WINDOW_WIDTH; x += gBack.partsW)
					PutBitmap4(&grcGame, x, y, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			break;

		case 5:
			for (y = -gBack.partsH; y < WINDOW_HEIGHT; y += gBack.partsH)
				for (x = -((gBack.fx / 0x200) % gBack.partsW); x < WINDOW_WIDTH; x += gBack.partsW)
					PutBitmap4(&grcGame, x, y, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			break;

		case 6:
		case 7:
			rect.top = 0;
			rect.bottom = 88;
			rect.left = 0;
			rect.right = 320;
			PutBitmap4(&grcGame, 0, 0, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.top = 88;
			rect.bottom = 123;
			rect.left = gBack.fx / 2;
			rect.right = 320;
			PutBitmap4(&grcGame, 0, 88, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.left = 0;
			PutBitmap4(&grcGame, 320 - ((gBack.fx / 2) % 320), 88, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.top = 123;
			rect.bottom = 146;
			rect.left = gBack.fx % 320;
			rect.right = 320;
			PutBitmap4(&grcGame, 0, 123, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.left = 0;
			PutBitmap4(&grcGame, 320 - (gBack.fx % 320), 123, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.top = 146;
			rect.bottom = 176;
			rect.left = 2 * gBack.fx % 320;
			rect.right = 320;
			PutBitmap4(&grcGame, 0, 146, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.left = 0;
			PutBitmap4(&grcGame, 320 - ((gBack.fx * 2) % 320), 146, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.top = 176;
			rect.bottom = 240;
			rect.left = 4 * gBack.fx % 320;
			rect.right = 320;
			PutBitmap4(&grcGame, 0, 176, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			rect.left = 0;
			PutBitmap4(&grcGame, 320 - ((gBack.fx * 4) % 320), 176, &rect, SURFACE_ID_LEVEL_BACKGROUND);

			break;
	}
}

void PutFront(int fx, int fy)
{
	int xpos, ypos;

	RECT rcWater[2] = {{0, 0, 32, 16}, {0, 16, 32, 48}};

	int x, y;
	int x_1, x_2;
	int y_1, y_2;

	switch (gBack.type)
	{
		case 3:
			x_1 = fx / (32 * 0x200);
			x_2 = x_1 + (((WINDOW_WIDTH + (32 - 1)) / 32) + 1);
			y_1 = 0;
			y_2 = y_1 + 32;

			for (y = y_1; y < y_2; ++y)
			{
				ypos = ((y * 32 * 0x200) / 0x200) - (fy / 0x200) + (gWaterY / 0x200);

				if (ypos < -32)
					continue;

				if (ypos > WINDOW_HEIGHT)
					break;

				for (x = x_1; x < x_2; ++x)
				{
					xpos = ((x * 32 * 0x200) / 0x200) - (fx / 0x200);
					PutBitmap3(&grcGame, xpos, ypos, &rcWater[1], SURFACE_ID_LEVEL_BACKGROUND);
					if (y == 0)
						PutBitmap3(&grcGame, xpos, ypos, &rcWater[0], SURFACE_ID_LEVEL_BACKGROUND);
				}
			}

			break;
	}
}
