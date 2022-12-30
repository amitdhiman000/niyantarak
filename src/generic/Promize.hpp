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
* Description : Promise
*/

#ifndef __PROMISE_AMU__
#define __PROMISE_AMU__

#include <future>

template<typename T>
class Promise
{
public:

	using ResolveFunction = std::function<void(T)>;
	using ExceptionFunction = std::function<void(std::exception exc)>;

	Promise()
	: mFuture(mPromise.get_future())
	{}

	bool valid() const
	{
		return mFuture.valid();
	}

 	bool resolved() const
	{
		return (valid() && mFuture.wait_for(std::chrono::milliseconds(0)) != std::future_status::timeout);
	}

	void resolve(const T inVal)
	{
		if (!resolved())
		{
			mPromise.set_value(inVal);
			mValue = inVal;
			if (mResolveFunc)
			{
				mResolveFunc(inVal);
			}
		}
	}

	void reject(std::exception& exc)
	{
		if (!resolved())
		{
			mPromise.set_exception(exc);
			if (mExceptionFunc)
			{
				mExceptionFunc(exc);
			}
		}
	}

	T getValue()
	{
		return mValue;
	}

	Promise& then(ResolveFunction inFunc)
	{
		mResolveFunc = inFunc;
		return *this;
	}

	Promise& except(ExceptionFunction inFunc)
	{
		mExceptionFunc = inFunc;
		return *this;
	}

private:
	T mValue;
	ResolveFunction mResolveFunc;
	ExceptionFunction mExceptionFunc;
	std::promise<T> mPromise;
	std::future<T> mFuture;
};

#endif // __PROMISE_AMU__
