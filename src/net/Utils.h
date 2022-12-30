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
* Description : DataT
*/

#ifndef __NET_UTILS_AMU__
#define __NET_UTILS_AMU__

#include "Config.h"
#include <cstring>

namespace NAMESPACE_NET {

struct DataT final
{
	DataT()
	: mData()
	, mSize()
	, mCapacity()
	{
	}

	DataT(std::size_t capacity)
	: mCapacity(capacity)
	{
		mData = new unsigned char[capacity];
	}

	DataT(const void *data, std::size_t size)
	{
		assign(data, size);
	}

	~DataT()
	{
		release();
	}

	void assign(const void *data, std::size_t size)
	{
		release();
		mData = new unsigned char[size];
		memcpy(mData, data, size);
		mCapacity = size;
		mSize = size;
	}

	void release(void)
	{
		if (mData)
		{
			delete []mData;
			mData = nullptr;
			mCapacity = 0;
			mSize = 0;
		}
	}

	void adopt(void* data, size_t size)
	{
		release();
		mData = static_cast<unsigned char*>(data);
		mCapacity = size;
		mSize = size;
	}

	void resize(std::size_t size)
	{
		if (size <= mCapacity)
		{
			mSize = size;
		}
	}

	void* data(void) const
	{
		return mData;
	}

	std::size_t size(void) const
	{
		return mSize;
	}

	std::size_t capacity(void) const
	{
		return mCapacity;
	}

private:
	unsigned char* mData = nullptr;
	std::size_t mSize = 0;
	std::size_t mCapacity = 0;
};

} // ends namespace NAMESPACE_NET

#endif // __NET_UTILS_AMU__
