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
* Description : Thread utility class
*/


#ifndef __AMU_LINUX_THREAD_AMU__
#define __AMU_LINUX_THREAD_AMU__

#include <Debug.h>
#include <pthread.h>
#include <functional>

class Thread {
public:
	template<typename FuncT, typename... ArgsT>
	Thread(FuncT func, ArgsT... args)
	: mThreadHandle(0)
	{
		mLamdaFunc = [func, args...](){ func(args...); };
	}

	~Thread(void)
	{
		//mThreadHandle = 0;
	}

	bool start(void)
	{
		if (0 == pthread_create(&mThreadHandle, nullptr,
				[](void *data) -> void *{
					static_cast<Thread *>(data)->mLamdaFunc();
					return nullptr;
				}, this)) {

			APP_LOGD("thread handle [%lu]", mThreadHandle);
			return true;
		}
		return false;
	}

	/* Asynchronous behavior in POSIX */
	bool cancel(void)
	{
		if (0 == pthread_cancel(mThreadHandle)) {
			mThreadHandle = 0;
			return true;
		}
		return false;
	}

	bool join(void)
	{
		return (0 == pthread_join(mThreadHandle, nullptr));
	}

	bool detach(void)
	{
		return (0 == pthread_detach(mThreadHandle));
	}

	inline bool alive(void) const { return (mThreadHandle != 0); }

private:
	std::function<void(void)> mLamdaFunc;
	pthread_t mThreadHandle;
};

#endif /* __AMU_LINUX_THREAD_AMU__ */
