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
* Description : Thread
*/

#ifndef __THREAD_AMU__
#define __THREAD_AMU__

#include "Debug.h"
#include <thread>
#include <functional>

using ShouldStopFxnT = std::function<bool(void)>;

class Thread
{
public:
	Thread() = default;

	template<typename RetT, typename... ParamsT>
	Thread(const std::function<RetT(ShouldStopFxnT, ParamsT...)>& inFunc, ParamsT... params)
	{
		APP_LOGD("this : %p", this);
		mFunctionHolder = [this, func = inFunc, params...]() {
			func(std::bind(&Thread::stopRequested, this), std::forward<ParamsT...>(params)...);
		};
	}

	virtual ~Thread()
	{
		APP_LOGD("this : %p", this);
		stop();
		if (mThread.joinable())
		{
			mThread.join();
		} else {
			//std::terminate();
		}
	}

	Thread(Thread&& other)
	{
		APP_LOGD("this : %p", this);
		move(*this, std::forward<Thread&&>(other));
	}

	Thread& operator=(Thread&& other)
	{
		APP_LOGD("this : %p", this);
		stopRequested();
		move(*this, std::forward<Thread&&>(other));
		return *this;
	}

	template<typename RetT, typename... ParamsT>
	void start(std::function<RetT(ShouldStopFxnT, ParamsT...)>& inFunc, ParamsT... params)
	{
		mFunctionHolder = [this, func = std::move(inFunc), params...]() {
							func(std::bind(&Thread::stopRequested, this), std::forward<ParamsT...>(params)...);
						};
		run(std::forward<ParamsT>(params)...);
	}

	template<typename RetT, typename... ParamsT>
	void run(ParamsT... params)
	{
		if (mFunctionHolder)
		{
			auto lamda = [this, func = std::move(mFunctionHolder)](ShouldStopFxnT shouldStop, ParamsT ...params) {
				mActive = true;
				func(std::forward<ParamsT>(params)...);
				mActive = false;
			};
			mThread = std::move(
						std::thread(std::move(lamda),
						[this]() -> bool { return stopRequested(); },
						std::forward<ParamsT>(params)...)
					);
		}
	}

	template<typename RetT, typename... ParamsT>
	void execute(std::function<RetT(ShouldStopFxnT, ParamsT...)> inFunc, ParamsT ...params)
	{
		APP_LOGD("");
		auto lamda = [this, func = std::move(inFunc)](ShouldStopFxnT shouldStop, ParamsT ...params) {
						APP_LOGD("");
						mActive = true;
						func(shouldStop, std::forward<ParamsT...>(params)...);
						mActive = false;
					};
		mThread = std::move(
							std::thread(std::move(lamda),
							std::bind(&Thread::stopRequested, this),
							std::forward<ParamsT...>(params)...)
						);
	}

	template<typename RetT, typename... ParamsT>
	void operator()(ParamsT... params)
	{
		run(std::forward<ParamsT>(params)...);
	}

	bool joinable() const
	{
		return mThread.joinable();
	}

	void join()
	{
		mThread.join();
	}

	void detach()
	{
		mThread.detach();
	}

	void stop()
	{
		APP_LOGD("this : %p", this);
		mStop = true;
	}

	bool stopRequested() const
	{
		//APP_LOGD("this : %p", this);
		return mStop;
	}

	bool active() const
	{
		return mActive;
	}

	operator bool()
	{
		return active();
	}

private:
	Thread& move(Thread& dest, Thread&& src)
	{
		if (&dest != &src)
		{
			dest.mThread = std::move(src.mThread);
			dest.mFunctionHolder = std::move(src.mFunctionHolder);
		}
		return dest;
	}

private:
	bool mActive = false;
	bool mStop = false;
	std::thread mThread;
	std::function<void()> mFunctionHolder;
};

#endif // __THREAD_AMU__

