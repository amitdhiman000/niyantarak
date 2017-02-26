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
* Description : DisplayController class
*/


#ifndef __DISPLAY_CONTROLLER_AMU__
#define __DISPLAY_CONTROLLER_AMU__

#include <unistd.h>
#include "Mouse.hpp"
#include "NDisplay.hpp"
#include "linux/Thread.hpp"

namespace CORE_NAMESPACE {

const char *keyName[] = {
		    "first",
		    "second (or middle)",
		    "third",
		    "fourth",  // :D
		    "fifth"    // :|
		};

class DisplayController {
public:
	DisplayController(void)
	: mMouse(nullptr)
	, mEventListenerThread(nullptr)
	{

	}

	~DisplayController(void)
	{
	}

	void start(void)
	{
		mNDisplay = std::make_shared<NDisplay>();
		mEventListenerThread = new Thread([this](){
			displayEventListenerFunc();
		});
		mEventListenerThread->start();
		mEventListenerThread->detach();

		mMouse = std::make_shared<Mouse>(mNDisplay);
	}

	void stop(void)
	{
		mEventListenerThread->cancel();
	}

private:
#if 0
	void displayEventListenerFunc(void)
	{
		XEvent xevent;
		auto display = mNDisplay->getDisplay();
		auto window = mNDisplay->getRootWindow();

		XAllowEvents(display, AsyncBoth, CurrentTime);

		XGrabPointer(display, window, 1,
					 PointerMotionMask | ButtonPressMask | ButtonReleaseMask ,
					 GrabModeAsync, GrabModeAsync,
					 None,
					 None,
					 CurrentTime);

		while(true) {
			XNextEvent(display, &xevent);
			switch (xevent.type) {
			case MotionNotify:
				APP_LOGD("Mouse move : [%d, %d]", xevent.xmotion.x_root, xevent.xmotion.y_root);
				break;
			case ButtonPress:
				APP_LOGD("Button pressed  : %s", keyName[xevent.xbutton.button - 1]);
				break;
			case ButtonRelease:
				APP_LOGD("Button released : %s", keyName[xevent.xbutton.button - 1]);
				break;
			}
		}
	}
#else
	void displayEventListenerFunc(void)
	{
		APP_LOGD("");
		auto display = mNDisplay->getDisplay();
		auto window = mNDisplay->getRootWindow();
		Window root;
		Window child;
		int rx;
		int ry;
		int wx;
		int wy;
		unsigned mask;
		while(true) {
			auto ret = XQueryPointer(display, window, &root, &child, &rx, &ry, &wx, &wy, &mask);
			APP_LOGD("ret : %d", ret);
			APP_LOGI("rx %d, ry %d, wx %d, wy %d", rx, ry, wx, wy);
			usleep(1000*1000);
		}

	}
#endif
private:
	MouseSPtr mMouse;
	NDisplaySPtr mNDisplay;
	Thread *mEventListenerThread;
};

} /* end CORE_NAMESPACE  */

#endif /* __DISPLAY_CONTROLLER_AMU__ */
