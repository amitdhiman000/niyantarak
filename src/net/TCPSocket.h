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
* Description : TCPSocket class
*/

#ifndef __TCP_SOCKET_AMU__
#define __TCP_SOCKET_AMU__

#include <net/ISocket.h>

namespace net {

class TCPSocket : public ISocket {
public:
	TCPSocket();

	~TCPSocket() override;

	bool bind(const char* inAddr, const char* inPort) override;

	bool bind(const std::string& inAddr, const std::string& inPort) override;

	bool listen(std::int32_t inMaxQueued) override;

	bool accept(ISocket& outSocket) override;

	bool close() override;

	std::int32_t read(void* const outData, const std::size_t inSize) const override;

	DataT read(const std::size_t inSize) const override;

	std::int32_t write(const void* const inData, const std::size_t inSize) override;

	std::string error() const override;

	std::int32_t fd() const override;

	bool isValid() const override;

	bool isReadyForRead() const override;

	bool isReadyForWrite() const override;

	bool isDisconnected() const override;
private:
	std::int32_t mHandle = -1;
};

}
#endif // __TCP_SOCKET_AMU__
