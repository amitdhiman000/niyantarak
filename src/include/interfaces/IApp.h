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
* Description : IApp
*/

#ifndef __IAPP_AMU__
#define __IAPP_AMU__

#include <string>

class IApp {
public:
	IApp() = default;

	virtual ~IApp() = default;

	virtual void init(std::int32_t argc, char* argv[]) = 0;

	virtual void run(void) = 0;

	virtual void terminate(void) = 0;

	virtual bool willTerminate(void) = 0;

	virtual void willShow(void) = 0;

	virtual void willClose(void) = 0;
};

#endif // __IAPP_AMU__
