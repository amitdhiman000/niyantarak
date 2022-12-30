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
* Dated : 18-04-2020
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Platform Display interface
*/

#ifndef __IPLATFORM_DISPLAY_AMU__
#define __IPLATFORM_DISPLAY_AMU__

#include <cstdint>

#if __linux__
#include <X11/Xlib.h>
#endif

#include "Config.h"

namespace NAMESPACE_CORE {

	class IPlatformDisplay
	{
	public:
		virtual ~IPlatformDisplay() = default;

		virtual Display *getDisplay(void) const = 0;

		virtual std::int32_t getScreen(void) const = 0;

		virtual Window getRootWindow(void) const = 0;
	};
}

#endif // __IPLATFORM_DISPLAY_AMU__
