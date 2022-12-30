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
* Dated : 25-04-2020
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Stoppable
*/

#ifndef __REPEATE_TASK_AMU__
#define __REPEATE_TASK_AMU__

#include "Stoppable.hpp"

template <typename FunctionType>
class RepeateTask;

template<typename ReturnType, typename... ParameterTypes>
class RepeateTask<ReturnType(ParameterTypes...)> : public Stoppable<ReturnType(ParameterTypes...)>
	{
	public:
	RepeateTask(std::function<void(void)> inFxn)
		: mFunc(inFxn)
		{
		}

		~RepeateTask()
		{
			stop();
		}

		ReturnType run(ParameterTypes... args) override
		{
			ReturnType ret;
			for (; stopRequested() ;)
			{
				ret = mFunc(args...);
			}
			return ret;
		}

	private:
		std::function<ReturnType(ParameterTypes...)> mFunc;
	};

#endif //__REPEATE_TASK_AMU__
