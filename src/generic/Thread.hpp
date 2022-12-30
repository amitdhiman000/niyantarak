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
#include <future>
#include <functional>

using ShouldStopFxnT = std::function<bool(void)>;

template <typename FunctionType>
class Thread;

template<typename ReturnType, typename... ParameterTypes>
class Thread<ReturnType(ParameterTypes...)>
{
public:
	using FunctionT = std::function<ReturnType(ShouldStopFxnT, ParameterTypes...)>;

	Thread() = default;

	Thread(const FunctionT& inFunc)
	: mFuture(mPromise.get_future())
	, mFunction(inFunc)
	{
		APP_LOGD("this : %p", this);
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

	void start(const std::function<void(ShouldStopFxnT, ParameterTypes...)>& inFunc, ParameterTypes... args)
	{
		mFunction = inFunc;
		run(std::forward<ParameterTypes>(args)...);
	}

	void run(ParameterTypes... args)
	{
		if (mFunction)
		{
#if 0
			auto lamda = [this, func = std::move(mFunction)](std::promise<void>& inPromise, ParameterTypes ...args) {
				mActive = true;
				func(std::forward<ParameterTypes>(args)...);
				mActive = false;
			};
			mThread = std::move(
						std::thread(std::move(lamda),
						[this]() -> bool { return stopRequested(); },
						std::forward<ParameterTypes>(args)...)
					);
#else
			mThread = std::move(
								std::thread(std::move(mFunction),
								[this]() -> bool { return stopRequested(); },
								std::forward<ParameterTypes>(args)...)
							);
#endif
		}
	}

	template<typename R, typename... P>
	void execute(std::function<R(ShouldStopFxnT, P...)> inFunc, P ...args)
	{
		APP_LOGD("");
		auto lamda = [this, func = std::move(inFunc)](ShouldStopFxnT shouldStop, ParameterTypes ...args) {
						APP_LOGD("");
						mActive = true;
						func(shouldStop, std::forward<ParameterTypes>(args)...);
						mActive = false;
					};
		mThread = std::move(
							std::thread(std::move(lamda),
							std::bind(&Thread::stopRequested, this),
							std::forward<ParameterTypes>(args)...)
						);
	}

	void operator()(ParameterTypes... args)
	{
		run(std::forward<ParameterTypes>(args)...);
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
		if (mFuture.valid())
		{
			mPromise.set_value();
		}
	}

	bool stopRequested() const
	{
		APP_LOGD("this : %p", this);
		if (mFuture.valid())
		{
			APP_LOGD("");
			auto val = (mFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
			return val;
		}
		return false;
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
			dest.mPromise = std::move(src.mPromise);
			dest.mFuture = std::move(src.mFuture);
			dest.mFunction = std::move(src.mFunction);
		}
		return dest;
	}

private:
	bool mActive = false;
	std::thread mThread;
	std::promise<void> mPromise;
	std::future<void> mFuture;

	FunctionT mFunction;
};

#endif // __THREAD_AMU__

