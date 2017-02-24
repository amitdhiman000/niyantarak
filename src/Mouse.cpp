/* Copyright 2016 Amit Dhiman
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Dated : 24-02-2017
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Mouse class implementation
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
	APP_LOGV("");
	Display *displayMain = XOpenDisplay(nullptr);

	if(nullptr == displayMain)
	{
		APP_LOGE("Error Opening the Display !!!");
		return;
	}

	XWarpPointer(displayMain, None, None, 0, 0, 0, 0, x, y);

	//XCloseDisplay(displayMain);
}

} /* end namespace core */
