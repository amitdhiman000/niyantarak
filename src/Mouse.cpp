/*
 * Mouse.cpp
 *
 *  Created on: 16-Oct-2016
 *      Author: amu
 */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "Mouse.h"
#include "Debug.h"

namespace core {

Mouse::Mouse(void)
: mX(0)
, mY(0)
{
}

Mouse::~Mouse(void)
{
}

void Mouse::move(int x, int y)
{
	moveMouse(x, y);
	mX = x;
	mY = y;
}

void Mouse::moveMouse(int x, int y)
{
	Display *displayMain = XOpenDisplay(NULL);

	if(nullptr == displayMain)
	{
		LOGE("Error Opening the Display !!!");
		return;
	}

	XWarpPointer(displayMain, None, None, 0, 0, 0, 0, x, y);

	//XCloseDisplay(displayMain);
}

} /* end namespace core */
