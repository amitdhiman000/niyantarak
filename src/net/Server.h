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
* Description : Server
*/

#ifndef __SERVER_AMU__
#define __SERVER_AMU__

#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <mutex>

#include "Config.h"
#include "Utils.h"

#include "base/Thread.hpp"

namespace NAMESPACE_NET {

struct ClientInfoT
{
	std::int32_t clientFd = -1;
	std::string name;
	std::string address;
	std::string hostName;
	std::string screenId;
};


struct ServerDelegate
{
	virtual ~ServerDelegate() = default;

	virtual void onClientConnected(const ClientInfoT &info) = 0;

	virtual void onClientDisconnected(const ClientInfoT &info) = 0;

	virtual void onDataReceived(const DataT& message) = 0;
};

class Server
{
public:
	using ClientsInfoMap = std::map<std::int32_t, ClientInfoT>;

	explicit Server(ServerDelegate* delegate);

	virtual ~Server();

	bool start(const std::uint16_t inPort);

	void stop();

	void watch();

	void disconnect(const std::int32_t inClientFd);

	std::int32_t send(const std::int32_t inClientSock, const DataT& inData);

	std::int32_t send(const std::int32_t inClientSock, const void* inData, const std::size_t inLength);

	DataT receive(const std::int32_t inSock, const std::size_t inLength);

	bool receive(const std::int32_t inSock, DataT* const outData, const std::size_t inLength);

	bool receive(const std::int32_t inSock, const std::size_t inLength, void* const outData, std::int32_t* const outLength);

private:

	void addClient(const ClientInfoT& inClientInfo);

	void removeClient(const ClientInfoT& inClientInfo);

	void notifyClientData(const DataT& inData);

	std::int32_t mServerSock = -1;
	ServerDelegate* mDelegate = nullptr;
	ClientsInfoMap mClientsInfo;
	std::mutex mClientsMutex;

	Thread mThread;
	bool mWatching = false;
};

} // end namespace NAMESPACE_CORE


#endif //__SERVER_AMU__
