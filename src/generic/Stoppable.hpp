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

#ifndef __STOPPABLE_AMU__
#define __STOPPABLE_AMU__

#include <future>

template <typename FunctionType>
class Stoppable;

template<typename ReturnType, typename... ParameterTypes>
class Stoppable<ReturnType(ParameterTypes...)>
{
public:
	Stoppable()
	: mFuture(mPromise.get_future())
	{}

	virtual ~Stoppable() = default;

	Stoppable(Stoppable&& other)
	: mPromise(std::move(other.mPromise))
	, mFuture(std::move(other.mFuture))
	{}

	Stoppable& operator=(Stoppable && other)
	{
		if (this != &other)
		{
			mPromise = std::move(other.mPromise);
			mFuture = std::move(other.mFuture);
		}
		return *this;
	}

	virtual ReturnType run(ParameterTypes...) = 0;

	ReturnType operator()(ParameterTypes... args)
	{
		return run(args...);
	}

	void stop()
	{
		mPromise.set_value();
	}

	bool stopRequested() const
	{
		return (mFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout);
	}

private:
	std::promise<void> mPromise;
	std::future<void> mFuture;
};

#endif // __STOPPABLE_AMU__

