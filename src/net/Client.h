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
* Description : Debug macros
*/

#ifndef __CLIENT_AMU__
#define __CLIENT_AMU__

#include <cstdint>
#include <string>

#include "Config.h"
#include "Utils.h"

namespace NAMESPACE_NET {

class ClientDelegate
{
public:
	virtual ~ClientDelegate() = default;

	virtual void onConnected(void) = 0;

	virtual void onDisconnected(void) = 0;

	virtual void onDataReceived(const DataT& inData) = 0;
};

class Client
{
public:
	Client(ClientDelegate *inDelegate);

	virtual ~Client();

	void connect(const std::string& inServerIP, const std::uint16_t inServerPort);

	void disconnect(void);

	void watch(void);

private:

	std::int32_t send(const std::int32_t inClientSock, const DataT& inData);

	std::int32_t send(const std::int32_t inClientSock, const void* inData, const std::size_t inLength);

	bool receive(const std::int32_t inSock, const std::size_t inLength, void* const outData, std::int32_t* const outReadLength);

	void onDataReceived(const void* inData, std::size_t inLength);

private:
	ClientDelegate* mDelegate = nullptr;
	std::int32_t mClientSock = -1;

};

} // end namespace NAMESPACE_CORE

#endif //__CLIENT_AMU__
