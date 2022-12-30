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
* Dated : 26-04-2020
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : ThreadGuard
*/

#ifndef __THREAD_GUARD_AMU__
#define __THREAD_GUARD_AMU__

#include "Thread.hpp"

template<typename FunctionType>
class ThreadGuard;

template<typename ReturnType, typename ...ParameterTypes>
class ThreadGuard<ReturnType(ParameterTypes...)>
{
public:
	explicit ThreadGuard(Thread<ReturnType(ParameterTypes...)>&& t_)
	: mThread(std::move(t_))
	{
	}

	~ThreadGuard()
	{
		mThread.stop();
		if(mThread.joinable())
		{
			mThread.join();
		}
	}

	ThreadGuard(ThreadGuard const&)=delete;
	ThreadGuard(ThreadGuard&&)=delete;
	ThreadGuard& operator=(ThreadGuard const&)=delete;
	ThreadGuard& operator=(ThreadGuard&&)=delete;

private:
	Thread<ReturnType(ParameterTypes...)> mThread;
};

#endif // __THREAD_GUARD_AMU__
