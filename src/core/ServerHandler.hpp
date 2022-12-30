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
* Description : ServerHandler class
*/

#ifndef __SERVER_HANDLER_AMU__
#define __SERVER_HANDLER_AMU__

#include <core/InputManger.hpp>
#include <net/Server.h>

#include "Config.h"
#include "Defines.h"


class ServerHandler : public NAMESPACE_NET::ServerDelegate, public NAMESPACE_CORE::InputListener
{
public:

	ServerHandler()
	{
	}

	~ServerHandler() override = default;

	void onClientConnected(const NAMESPACE_NET::ClientInfoT &info)
	{
		APP_LOGI("Client Connected..");
		mActiveClientSock = info.clientFd;
		if (++mConnectedCount)
		{
			snoopInputs();
		}
	}

	void onClientDisconnected(const NAMESPACE_NET::ClientInfoT &info)
	{
		APP_LOGI("Client Disconnected..");
		mConnectedCount--;
		mActiveClientSock = -1;
	}

	void onDataReceived(const NAMESPACE_NET::DataT& inData)
	{
		APP_LOGI("Data received %s", static_cast<const char*>(inData.data()));
	}

	void snoopInputs()
	{
		if (mInputMgr.stopped())
		{
			std::thread th([this]() {
				mInputMgr.start();
			});
			th.detach();
		}
		mInputMgr.subscribe(this);
	}

	void unsnoopInputs()
	{
		mInputMgr.unsubscribe(this);
		mInputMgr.stop();
	}

	void sendInputs(const NAMESPACE_NET::DataT& data)
	{
		if (mServer && mActiveClientSock > 0)
		{
			mServer->send(mActiveClientSock, data);
		}
	}

	void onInput(const NAMESPACE_CORE::InputDataT& inData) override
	{
		static char buffer[50] = {0};
		const auto count = snprintf(buffer, sizeof(buffer), R"({"x": %d, "y": %d })", inData.mouse.x, inData.mouse.y);
		NAMESPACE_NET::DataT data(buffer, count);
		sendInputs(data);
	}

	void setServer(std::shared_ptr<NAMESPACE_NET::Server> server)
	{
		mServer = server;
	}

private:
	std::shared_ptr<net::Server> mServer;
	Thread mThread;
	NAMESPACE_CORE::InputManager mInputMgr;
	std::int32_t mActiveClientSock = -1;
	std::int32_t mConnectedCount = 0;
};

#endif // __SERVER_HANDLER_AMU__
