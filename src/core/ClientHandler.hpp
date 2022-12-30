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
* Description : ClientHandler class
*/

#ifndef __CLIENT_HANDLER_AMU__
#define __CLIENT_HANDLER_AMU__

#include <core/InputManger.hpp>
#include <net/Client.h>
#include "Config.h"
#include "third_party/json.hpp"

class ClientHandler : public NAMESPACE_NET::ClientDelegate
{
public:

	ClientHandler()
	{
	}

	~ClientHandler() override = default;

	void onConnected(void)
	{
		APP_LOGD("Connected");
		mClient->watch();
	}

	void onDisconnected(void)
	{
		APP_LOGD("Disconnected");
	}

	void onDataReceived(const NAMESPACE_NET::DataT& inData)
	{
		APP_LOGD("received : %s", (const char*)inData.data());
		const auto data = static_cast<const char*>(inData.data());
		try {
			nlohmann::json jsonObj = nlohmann::json::parse(data);
			const std::int32_t x = jsonObj["x"];
			const std::int32_t y = jsonObj["y"];
			APP_LOGD("(%d, %d)", x, y);
			mInputMgr.setMousePos(x, y);
		} catch(...)
		{
			APP_LOGD("can't parse : %s", (const char*)inData.data());
		}
	}

	void setClient(std::shared_ptr<NAMESPACE_NET::Client> inClient)
	{
		mClient = inClient;
	}

private:
	std::shared_ptr<NAMESPACE_NET::Client> mClient;
	NAMESPACE_CORE::InputManager mInputMgr;
};

#endif // __CLIENT_HANDLER_AMU__
