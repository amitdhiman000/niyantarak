/* Copyright 2020 Amit Dhiman
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
* Dated : 04-07-2020
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : TCPNode class
*/

#ifndef __TCP_NODE_AMU__
#define __TCP_NODE_AMU__

#include <cstdint>

#include "Utils.h"

namespace net {

class TCPNode {
public:
	TCPNode() = default;

	virtual ~TCPNode() = default;

	std::int32_t send(const std::int32_t inClientSock, const DataT& inData);

	std::int32_t send(const std::int32_t inClientSock, const void* inData, const std::size_t inLength);

	DataT receive(const std::int32_t inSock, const std::size_t inLength);

	bool receive(const std::int32_t inSock, std::size_t inLength, void* const outData, std::int32_t* const outReadLength);
};


} // ends namespace net



#endif // __TCP_NODE_AMU__
