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
* Description : Socket class
*/

#ifndef __ISOCKET_AMU__
#define __ISOCKET_AMU__

#include <cstdint>
#include <string>

#include "Utils.h"

namespace net {

class ISocket {
public:
	ISocket() = default;

	virtual ~ISocket() = default;

	virtual bool bind(const char* inAddr, const char* inPort) = 0;

	virtual bool bind(const std::string& inAddr, const std::string& inPort) = 0;

	virtual bool listen(std::int32_t inMaxQueued) = 0;

	virtual bool accept(ISocket& outSocket) = 0;

	virtual bool close() = 0;

	virtual std::int32_t read(void* const outData, const std::size_t inSize) const = 0;

	virtual DataT read(const std::size_t inSize) const = 0;

	virtual std::int32_t write(const void* const inData, const std::size_t inSize) = 0;

	virtual std::string error() const = 0;

	virtual std::int32_t fd() const = 0;

	virtual bool isValid() const = 0;

	virtual bool isReadyForRead() const = 0;

	virtual bool isReadyForWrite() const = 0;

	virtual bool isDisconnected() const = 0;
};

}
#endif // __ISOCKET_AMU__
