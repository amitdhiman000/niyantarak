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
* Description : TCPSocket class
*/

#include "TCPSocket.h"

#include "Config.h"
#include "Debug.h"

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

namespace net {

TCPSocket::TCPSocket()
{
}

TCPSocket::~TCPSocket()
{
	close();
}

bool TCPSocket::bind(const char* inAddr, const char* inPort)
{
	int sock = 0;
	struct sockaddr_in serverAddress = {0, };

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(inAddr);
	serverAddress.sin_port = ::htons(::atoi(inPort));

	if ((sock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		APP_LOGE("Socket creation error: %s(%d)", strerror(errno), errno);
		return false;
	}

#if 1
	std::int32_t reuseAddress = 1;
	if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) == -1)
	{
		APP_LOGE("Socket setsockopt error: %s(%d)", strerror(errno), errno);
		return false;
	}
#endif

	if (::bind(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
	{
		APP_LOGE("Socket bind error: %s(%d)", strerror(errno), errno);
		return false;
	}
	return true;
}

bool TCPSocket::bind(const std::string& inAddr, const std::string& inPort)
{
	return bind(inAddr.c_str(), inPort.c_str());
}

bool TCPSocket::listen(std::int32_t inMaxQueued)
{
	if (!isValid())
		return false;

	return ::listen(mHandle, inMaxQueued) == 0;
}

bool TCPSocket::accept(ISocket& outSocket)
{
	auto sock = dynamic_cast<TCPSocket&>(outSocket);
	struct sockaddr_in clientAddress = {0, };
	socklen_t length = sizeof(clientAddress);
	const auto clientFd = ::accept(mHandle, reinterpret_cast<sockaddr*>(&clientAddress), &length);
	if (clientFd > 0)
	{
		sock.mHandle = clientFd;
		return true;
	}
	return false;
}

bool TCPSocket::close()
{
	::close(mHandle);
	mHandle = -1;
	return true;
}

std::int32_t TCPSocket::read(void* const outData, const std::size_t inSize) const
{
	const auto bytesRead = ::read(mHandle, outData, inSize);
	return bytesRead;
}

DataT TCPSocket::read(const std::size_t inSize) const
{
	DataT data(inSize);
	auto bytesRead = ::read(mHandle, data.data(), inSize);
	if (bytesRead >= 0)
	{
		data.resize(bytesRead);
	}
	return data;
}

std::int32_t TCPSocket::write(const void* const inData, const std::size_t inSize)
{
	const auto bytesSent = ::write(mHandle, inData, inSize);
	return bytesSent;
}

std::string TCPSocket::error() const
{
	return strerror(errno);
}

std::int32_t TCPSocket::fd() const
{
	return mHandle;
}

bool TCPSocket::isValid() const
{
	return mHandle >= 0;
}


bool TCPSocket::isReadyForRead() const
{
	return false;
}

bool TCPSocket::isReadyForWrite() const
{
	return false;
}

bool TCPSocket::isDisconnected() const
{
	return false;
}

} // ends namespace net
