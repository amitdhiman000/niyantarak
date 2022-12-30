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
* Description : InputManager class
*/


#ifndef __DISPLAY_CONTROLLER_AMU__
#define __DISPLAY_CONTROLLER_AMU__

#include <fwd/CoreFwd.h>
#include <unistd.h>

#include "core/Mouse.hpp"
#include "core/PlatformDisplay.hpp"

namespace NAMESPACE_CORE
{

	enum class MouseButtonT {
		kNone,
		kLeft,
		kMiddle,
		kRight,
		kScrollUp,
		kScrollDown
	};

	const char *mouseButtonsMap[] = {
								"None",
								"left",
								"middle (center)",
								"right",
								"scroll up",
								"scroll down"
							};

	unsigned long mapMouseButtonToX11(MouseButtonT type)
	{
		switch(type)
		{
		default:
		case MouseButtonT::kNone:
			return 0;
		case MouseButtonT::kLeft:
			return 1;
		case MouseButtonT::kMiddle:
			return 2;
		case MouseButtonT::kRight:
			return 3;
		case MouseButtonT::kScrollUp:
			return 4;
		case MouseButtonT::kScrollDown:
			return 5;
		}
	}

	MouseButtonT mapX11toMouseButton(unsigned long x11Button)
	{
		switch(x11Button)
		{
		default:
		case 0:
			return MouseButtonT::kNone;
		case 1:
			return MouseButtonT::kLeft;
		case 2:
			return MouseButtonT::kMiddle;
		case 3:
			return MouseButtonT::kRight;
		case 4:
			return MouseButtonT::kScrollUp;
		case 5:
			return MouseButtonT::kScrollDown;
		}
	}

	struct MouseDataT {
		std::int32_t x;
		std::int32_t y;
	};

	struct KeyboardDataT {
		std::int32_t key;
	};

	struct InputDataT {
		MouseDataT mouse;
		KeyboardDataT keyboard;
	};

	class InputListener {
	public:
		virtual ~InputListener() = default;
		virtual void onInput(const InputDataT& inData) = 0;
	};


	class InputManager {
	public:
		InputManager(void)
		{
			mDisplay = std::make_shared<PlatformDisplay>();
			mMouse = std::make_shared<Mouse>(mDisplay);
		}

		~InputManager(void)
		{
		}

		void start(void)
		{
			mStop = false;
			capture();
		}

		void stop(void)
		{
			mStop = true;
		}

		bool stopped() const
		{
			return mStop;
		}

		void subscribe(InputListener* inListener)
		{
			mInputListener = inListener;
		}

		void unsubscribe(InputListener* inListener)
		{
			mInputListener = nullptr;
		}

		MouseDataT getMousePos() const
		{
			auto display = mDisplay->getDisplay();
			auto window = mDisplay->getRootWindow();
			Window root;
			Window child;
			std::int32_t rx;
			std::int32_t ry;
			std::int32_t wx;
			std::int32_t wy;
			unsigned mask;
			auto ret = XQueryPointer(display, window, &root, &child, &rx, &ry, &wx, &wy, &mask);
			if (ret < 0)
			{
				APP_LOGE("Error reading mouse %d", ret);
			}
			return MouseDataT{rx, ry};
		}

		void setMousePos(std::int32_t x, std::int32_t y)
		{
			XWarpPointer(mDisplay->getDisplay(), None, mDisplay->getRootWindow(), 0, 0, 0, 0, x, y);
			XFlush(mDisplay->getDisplay());
		}

		void moveMousePos(std::int32_t dx, std::int32_t dy)
		{
			XWarpPointer(mDisplay->getDisplay(), None, None, 0, 0, 0, 0, dx, dy);
			XFlush(mDisplay->getDisplay());
		}

		bool grabMouse()
		{
			auto display = mDisplay->getDisplay();
			auto window = mDisplay->getRootWindow();

			const auto eventsMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

			auto error = XGrabPointer(display, window, 1,
						 eventsMask ,
						 GrabModeAsync, GrabModeAsync,
						 None,
						 None,
						 CurrentTime);
			if (error != GrabSuccess)
			{
				XUngrabPointer(mDisplay->getDisplay(), CurrentTime);
			}
			return (error == GrabSuccess);
		}

		void ungrabMouse()
		{
			XUngrabPointer(mDisplay->getDisplay(), CurrentTime);
		}

		bool grabKeyboard()
		{
			auto error = XGrabKeyboard(mDisplay->getDisplay(), mDisplay->getRootWindow(), False, GrabModeAsync,
				                       GrabModeAsync, CurrentTime);
			if (error != GrabSuccess)
			{
				XUngrabKeyboard(mDisplay->getDisplay(), CurrentTime);
			}
			return (error == GrabSuccess);
		}

		void ungrabKeyboard()
		{
			XUngrabKeyboard(mDisplay->getDisplay(), CurrentTime);
		}

		void fakeMouseButton(MouseButtonT button, bool pressed)
		{
			auto display = mDisplay->getDisplay();
			const auto xButton = mapMouseButtonToX11(button);
			if (xButton > 0 && xButton < 11) {
				::XTestFakeButtonEvent(display, xButton,
						pressed ? True : False, CurrentTime);
				::XFlush(display);
			}
		}

		void fakeMouseMove(std::int32_t x, std::int32_t y)
		{
			auto display = mDisplay->getDisplay();
			if (false) {
				::XWarpPointer(display, None, mDisplay->getRootWindow(), 0, 0, 0, 0, x, y);
			}
			else {
				::XTestFakeMotionEvent(display, DefaultScreen(display),
									x, y, CurrentTime);
			}
			::XFlush(display);
		}

		void fakeMouseRelativeMove(std::int32_t dx, std::int32_t dy) const
		{
			auto display = mDisplay->getDisplay();
			// FIXME -- ignore xinerama for now
			if (false) {
				//XWarpPointer(m_display, None, m_root, 0, 0, 0, 0, x, y);
			}
			else {
				::XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime);
			}
			::XFlush(display);
		}

	private:
		void capture(void)
		{
			APP_LOGI("start capturing inputs");
			XEvent xevent;
			auto display = mDisplay->getDisplay();
			//auto window = mDisplay->getRootWindow();
			::XAllowEvents(display, AsyncBoth, CurrentTime);

			const auto eventsMask = PointerMotionMask | ButtonPressMask | ButtonReleaseMask;

			grabMouse();
			for(; !mStop; )
			{
#if 0
				int ret = XPending(display);
				if (ret == 0)
				{
					continue;
				}
				APP_LOGD("ret [%d]", ret);
#endif
				XNextEvent(display, &xevent);
				switch (xevent.type) {
				case MotionNotify:
					APP_LOGD("Mouse move : [%d, %d]", xevent.xmotion.x_root, xevent.xmotion.y_root);
					break;
				case ButtonPress:
					APP_LOGD("Button pressed  : %s", mouseButtonsMap[xevent.xbutton.button]);
					break;
				case ButtonRelease:
					APP_LOGD("Button released : %s", mouseButtonsMap[xevent.xbutton.button]);
					break;
				default:
					APP_LOGD("event %d", xevent.type);
					break;
				}

				if (mInputListener)
				{
					InputDataT data{ {xevent.xmotion.x_root, xevent.xmotion.y_root} };
					mInputListener->onInput(data);
				}
			}

			XSync(display, False);
			APP_LOGD("snooping ends");
			//ungrabMouse();
		}
	private:
		bool mStop = true;
		InputListener* mInputListener = nullptr;
		MouseSPtr mMouse;
		PlatformDisplaySPtr mDisplay;
	};

} /* end CORE_NAMESPACE  */

#endif /* __DISPLAY_CONTROLLER_AMU__ */
