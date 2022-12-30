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
* Description : Atomic
*/

#ifndef __ATOMIC_AMU__
#define __ATOMIC_AMU__

#include <mutex>

template<typename T>
class Atomic
{
	Atomic(T data)
	: mData(data)
	{
	}

	Atomic<T>(const Atomic<T>& other)
	{
		copy(*this, other);
	}

	Atomic<T>(Atomic<T>&& other)
	{
		move(*this, other);
	}

	~Atomic() = default;

	Atomic<T>& operator=(const Atomic<T>& other)
	{
		return copy(*this, other);
	}

	Atomic<T>& operator=(Atomic<T>&& other)
	{
		return move(*this, other);
	}

	operator T()
	{
		std::unique_lock lock(mMutex);
		T data = mData;
		return data;
	}

	bool operator==(const Atomic<T>& other)
	{
		return equals(*this, other) == 0;
	}

	bool operator!=(const Atomic<T>& other)
	{
		return equals(*this, other) != 0;
	}

private:

	int equals(const Atomic<T>& val1, Atomic<T>& val2)
	{
		std::unique_lock lock(val1.mMutex);
		std::unique_lock lock(val2.mMutex);

		if (val1.mData == val2.mData)
		{
			return 0;
		} else if (val1.mData < val2.mData)
		{
			return -1;
		}
		return 1;
	}

	Atomic<T>& copy(Atomic<T>& dest, const Atomic<T>& src)
	{
		if (&dest != &src)
		{
			std::unique_lock lock(dest.mMutex);
			std::unique_lock lock(src.mMutex);
			dest.mData = src.mData;
		}
		return dest;
	}

	Atomic<T>& move(Atomic<T>& dest, Atomic<T>&& src)
	{
		if (&dest != &src)
		{
			std::unique_lock lock(dest.mMutex);
			std::unique_lock lock(src.mMutex);
			dest.mData = std::move(src.mData);
		}
		return dest;
	}

private:
	std::mutex mMutex;
	T mData;
};
