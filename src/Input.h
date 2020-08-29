#pragma once

#include "WindowsWrapper.h"

struct DIRECTINPUTSTATUS
{
	BOOL bLeft;
	BOOL bRight;
	BOOL bUp;
	BOOL bDown;
	BOOL bButton[32];	// The original `Input.cpp` assumed there were 32 buttons (because of DirectInput's `DIJOYSTATE` struct)
};

void ReleaseDirectInput(void);
BOOL InitDirectInput(void);
BOOL GetJoystickStatus(DIRECTINPUTSTATUS *status);
BOOL ResetJoystickStatus(void);
