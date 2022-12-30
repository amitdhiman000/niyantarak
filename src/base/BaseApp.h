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
* Description : BaseApp
*/

#ifndef __BASEAPP_AMU__
#define __BASEAPP_AMU__

#include "../include/Config.h"
#include "../include/interfaces/IApp.h"

#ifdef LINUX
#include "../include/linux/EventLoop.hpp"
#endif

class BaseApp : public IApp {
public:
	BaseApp();

	virtual ~BaseApp();

	void init(std::int32_t argc, char* argv[]) override;

	void run(void) override;

	void terminate(void) override;

	bool willTerminate(void) override;

	void willShow(void) override;

	void willClose(void) override;

private:
#ifdef LINUX
	EventLoop mEventLoop;
#endif
};

#endif // __BASEAPP_AMU__
