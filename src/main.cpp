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
* Description : main file
*/

#include <sys/signal.h>

#include "linux/EventLoop.hpp"
#include "core/DisplayController.hpp"

#include "Config.h"
#include "Debug.h"

#define __CATCH_SIGNAL 0

using namespace CORE_NAMESPACE;

static EventLoop sEventLoop;

#if __CATCH_SIGNAL
void signalHandler(int signum)
{
	APP_LOGD("signal %d received", signum);
	sEventLoop.stop();
}
#endif

int main(void)
{
	APP_LOGI("");

#if __CATCH_SIGNAL
	struct sigaction newAction;
	/* Set up the structure to specify the new action. */
	newAction.sa_handler = signalHandler;
	sigemptyset(&newAction.sa_mask);
	newAction.sa_flags = 0;

	sigaction(SIGINT, &newAction, nullptr);
	sigaction(SIGABRT, &newAction, nullptr);
	sigaction(SIGHUP, &newAction, nullptr);
	sigaction(SIGTERM, &newAction, nullptr);
#endif

	DisplayController controller;
	controller.start();

	sEventLoop.start();
	APP_LOGD("Done");
	return 0;
}
