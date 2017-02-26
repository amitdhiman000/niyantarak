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
* Description : Mouse class
*/

#ifndef __MOUSE_AMU__
#define __MOUSE_AMU__

#include "Config.h"
#include "NDisplay.hpp"

namespace CORE_NAMESPACE {

class Mouse {
public:
	Mouse(void)
	: mX(0)
	, mY(0)
	{
		mNDisplay = std::make_shared<NDisplay>();
	}

	Mouse(NDisplaySPtr pDisplay)
	: mNDisplay(pDisplay)
	, mX(0)
	, mY(0)
	{

	}

	~Mouse(void)
	{
	}

	void move(int pX, int pY)
	{
		moveMouse(pX, pY);
		mX = pX;
		mY = pY;
	}

	inline int getX(void) const { return mX; }
	inline int getY(void) const { return mY; }

private:
	void moveMouse(int pX, int pY)
	{
		APP_LOGV("");
		if (mNDisplay) {
			auto display = mNDisplay->getDisplay();
			XSelectInput(display, mNDisplay->getRootWindow(), KeyReleaseMask);
			XWarpPointer(display, None, mNDisplay->getRootWindow(), 0, 0, 0, 0, pX, pY);
			XFlush(display);
		} else {
			APP_LOGE("no display object");
		}
	}

private:
	NDisplaySPtr mNDisplay;
	int mX;
	int mY;
};

typedef std::shared_ptr<Mouse> MouseSPtr;
} /* end CORE_NAMESPACE */

#endif /* __MOUSE_AMU__ */
