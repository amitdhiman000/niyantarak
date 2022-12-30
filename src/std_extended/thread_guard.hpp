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
* Description : thread_guard
*/

#ifndef __STD_THREAD_GUARD_AMU__
#define __STD_THREAD_GUARD_AMU__

#include <thread>

namespace std_extended {

class thread_guard
{
public:
	explicit thread_guard(std::thread&& t_)
	: m_thread(std::move(t_))
	{
	}

	~thread_guard()
	{
		if(m_thread.joinable())
		{
			m_thread.join();
		}
	}

	thread_guard(thread_guard const&)=delete;
	thread_guard(thread_guard&&)=delete;
	thread_guard& operator=(thread_guard const&)=delete;
	thread_guard& operator=(thread_guard&&)=delete;

private:
	std::thread m_thread;
};

} // ends namespace std_extended

#endif // __STD_THREAD_GUARD_AMU__
