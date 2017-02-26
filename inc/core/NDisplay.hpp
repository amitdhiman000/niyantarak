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
* Description : Native Display class
*/

#ifndef __NDISPLAY_AMU__
#define __NDISPLAY_AMU__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstring>
#include <memory>

#include "Debug.h"
#include "Exceptions.hpp"

class NDisplay {
public:
	NDisplay(void) throw()
	{
		mDisplayName = getenv("DISPLAY");
		if (!init()) {
			throw new DisplayNotFoundException("display name not found");
		}
	}

	NDisplay(const char *name) throw()
	{
		mDisplayName = strdup(name);
		if (!init()) {
			throw new DisplayNotFoundException("display name not found");
		}
	}
	~NDisplay(void)
	{
		deinit();
		free(mDisplayName);
	}

	operator Display*(void) { return mDisplay; }

	inline Display *getDisplay(void) const { return mDisplay; }
	inline int getScreen(void) const { return mScreen; }
	inline Window getRootWindow(void) const { return mRootWindow; }

private:
	bool init(void)
	{
		mDisplay = XOpenDisplay(mDisplayName);
		RETV_MSG_IF(!mDisplay, false, "Failed to open display");

		mScreen = XDefaultScreen(mDisplay);

		mRootWindow = RootWindow(mDisplay, mScreen);
		RETV_MSG_IF(!mRootWindow, false, "Failed to get root window");

		return true;
	}

	void deinit(void)
	{
		if (mDisplay) {
			XCloseDisplay(mDisplay);
			mDisplay = nullptr;
		}
	}

private:
	char *mDisplayName;
	Display *mDisplay;
	int mScreen;
	Window mRootWindow;
};

typedef std::shared_ptr<NDisplay> NDisplaySPtr;

#endif /* __NDISPLAY_AMU__ */
