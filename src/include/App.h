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
* Description : App
*/

#ifndef __APP_AMU__
#define __APP_AMU__


#include <string>

#include "base/BaseApp.h"
#include "fwd/NetFwd.h"

class App : public BaseApp
{
public:

	using super = BaseApp;

	App();

	~App() override;

	void init(std::int32_t argc, char* argv[]) override;

	void run(void) override;

private:
	std::string mMode;
	std::shared_ptr<NAMESPACE_NET::Server> mServer;
	std::shared_ptr<NAMESPACE_NET::Client> mClient;

};

#endif // __APP_AMU__
