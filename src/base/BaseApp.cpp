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

#include "BaseApp.h"

BaseApp::BaseApp()
{
}

BaseApp::~BaseApp()
{
}

void BaseApp::init(std::int32_t argc, char* argv[])
{
	// init base app
}

void BaseApp::run(void)
{
#ifdef LINUX
	mEventLoop.start();
#endif
}

void BaseApp::terminate(void)
{
#ifdef LINUX
	mEventLoop.stop();
#endif
}

bool BaseApp::willTerminate(void)
{
	return true;
}

void BaseApp::willShow(void)
{

}

void BaseApp::willClose(void)
{

}
