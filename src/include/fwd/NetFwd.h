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
* Description : Network forward declarations.
*/

#ifndef __NET_FWD_AMU__
#define __NET_FWD_AMU__

#include "Config.h"
#include <memory>

namespace NAMESPACE_NET {
	class Server;
	class Client;
}

using ServerSPtr = std::shared_ptr<NAMESPACE_NET::Server>;
using ClientSPtr = std::shared_ptr<NAMESPACE_NET::Client>;

#endif // __NET_FWD_AMU__
