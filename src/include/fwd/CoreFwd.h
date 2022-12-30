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
* Description : core forward declarations
*/


#ifndef __CORE_FWD_AMU__
#define __CORE_FWD_AMU__

#include "Config.h"
#include <memory>

namespace NAMESPACE_CORE {
	class Mouse;
	class PlatformDisplay;
}

using MouseSPtr = std::shared_ptr<NAMESPACE_CORE::Mouse>;
using MouseWPtr = std::weak_ptr<NAMESPACE_CORE::Mouse>;

using PlatformDisplaySPtr = std::shared_ptr<NAMESPACE_CORE::PlatformDisplay>;
using PlatformDisplayWPtr = std::weak_ptr<NAMESPACE_CORE::PlatformDisplay>;


#endif // __CORE_FWD_AMU__
