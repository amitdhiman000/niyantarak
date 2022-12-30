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
* Description : App interface
*/

#include <core/InputManger.hpp>
#include "include/App.h"

#include "net/Server.h"
#include "net/Client.h"

#include "core/ServerHandler.hpp"
#include "core/ClientHandler.hpp"

#include <thread>
#include "base/Thread.hpp"

#include "Defines.h"

using namespace NAMESPACE_CORE;
using namespace NAMESPACE_NET;

App::App()
: mMode("client")
{
}

App::~App()
{

}

void App::init(std::int32_t argc, char* argv[])
{
	if (argc > 1 && strncmp(argv[1], "server", strlen("server")) == 0)
	{
		mMode = "server";
	}
}

void App::run()
{
	if (mMode == "server")
	{
		auto serverHandler = new ServerHandler();
		mServer = std::make_shared<Server>(serverHandler);
		serverHandler->setServer(mServer);
		std::thread th([this](){
			mServer->start(SERVER_PORT);
		});
		th.detach();
	} else {
		auto clientHandler = new ClientHandler();
		mClient = std::make_shared<Client>(clientHandler);
		clientHandler->setClient(mClient);
		std::thread th([this](){
			APP_LOGD("server address: %s", SERVER_ADDR);
			mClient->connect(SERVER_ADDR, SERVER_PORT);
		});
		th.detach();
	}
	super::run();
}

