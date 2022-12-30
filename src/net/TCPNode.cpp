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
* Dated : 04-07-2020
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : TCPNode class
*/

#include "TCPNode.h"

#ifdef LINUX
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

#ifdef WINDOWS
// pending
#endif

#ifdef APPLE
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#endif

#include "utils/Number.h"
#include "Constants.h"
#include "Debug.h"

namespace net {

std::int32_t TCPNode::send(const std::int32_t inClientSock, const DataT& inData)
{
	return send(inClientSock, inData.data(), inData.size());
}

std::int32_t TCPNode::send(const std::int32_t inClientSock, const void* inData, const std::size_t inLength)
{
	if (inClientSock > 0)
	{
		const auto str = utils::number::toString(inLength, constants::kPayloadLengthChars);
		auto bytesSent = ::write(inClientSock, str.data(), str.length());
		if (bytesSent < 0) {
			APP_LOGE("Failed to send data length %s(%d)", strerror(errno), errno);
		}
		bytesSent = ::write(inClientSock, inData, inLength);
		if (bytesSent < 0)
		{
			APP_LOGE("Failed to send data %s(%d)", strerror(errno), errno);
		} else if (std::size_t(bytesSent) != inLength) {
			APP_LOGE("Failed to send full data %s(%d)", strerror(errno), errno);
		}
		return bytesSent;
	}
	return 0;
}

DataT TCPNode::receive(const std::int32_t inSock, const std::size_t inLength)
{
	DataT data;

	std::int32_t size = 0;
	auto buffer = new unsigned char[inLength];

	const bool ret = receive(inSock, inLength, data.data(), &size);
	if (ret)
	{
		data.adopt(buffer, size);
		return data;
	}
	return {};
}

bool TCPNode::receive(const std::int32_t inSock, std::size_t inLength, void* const outData, std::int32_t* const outReadLength)
{
	fd_set readSet;

	FD_SET(inSock, &readSet);

	// select the ready descriptor
	auto nReady = ::select(inSock + 1, &readSet, nullptr, nullptr, nullptr);

	if (nReady > 0)
	{
		char buffer[constants::kPayloadLengthChars + 1] = {0,};
		auto bytesRead = ::read(inSock, buffer, constants::kPayloadLengthChars);
		if (bytesRead > 0)
		{
			const std::size_t size = ::atoi(buffer);
			bytesRead = ::read(inSock, outData, size);
		}
		if (bytesRead > 0)
		{
			static_cast<volatile char*>(outData)[bytesRead] = 0;
			*outReadLength = bytesRead;
			return true;
		}
	} else {
		APP_LOGE("Failed to read %s(%d)", strerror(errno), errno);
	}

	return false;
}

} // ends namespace net

