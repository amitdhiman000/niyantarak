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

#ifndef __PLATFORM_DISPLAY_AMU__
#define __PLATFORM_DISPLAY_AMU__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#include <stdint.h>
#include <string>
#include <memory>

#include "core/Exceptions.hpp"
#include "core/IPlatformDisplay.h"

#include "Config.h"
#include "Debug.h"

namespace NAMESPACE_CORE
{
	class PlatformDisplay : public IPlatformDisplay {
	public:
		PlatformDisplay(void) throw()
		{
			mDisplayName = getenv("DISPLAY") ? getenv("DISPLAY") : ":0.0";
			APP_LOGD("display %s", mDisplayName.c_str());
			if (!init()) {
				throw new DisplayNotFoundException("display name not found");
			}
		}

		PlatformDisplay(const char *name) throw()
		{
			mDisplayName = name? name: "";
			if (!init()) {
				throw new DisplayNotFoundException("display name not found");
			}
		}

		~PlatformDisplay(void)
		{
			deinit();
		}

		operator Display*(void) { return mDisplay; }

		inline Display* getDisplay(void) const override { return mDisplay; }

		inline int getScreen(void) const override { return mScreen; }

		inline Window getRootWindow(void) const override { return mRootWindow; }

	private:
		bool init(void)
		{
			mDisplay = XOpenDisplay(0);
			//mDisplay = XOpenDisplay(mDisplayName.c_str());
			RETV_MSG_IF(!mDisplay, false, "Failed to open display");

			mScreen = XDefaultScreen(mDisplay);

			mRootWindow = DefaultRootWindow(mDisplay);
			//mRootWindow = RootWindow(mDisplay, mScreen);
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
		std::string mDisplayName;
		int mScreen;
		Display *mDisplay;
		Window mRootWindow;
	};

} // namespace NAMESPACE_CORE

typedef std::shared_ptr<NAMESPACE_CORE::PlatformDisplay> PlatformDisplaySPtr;

#endif /* __PLATFORM_DISPLAY_AMU__ */
