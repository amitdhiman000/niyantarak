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
* Description : Client
*/

#include "Client.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include "Constants.h"
#include "Defines.h"
#include "Debug.h"
#include "utils/Number.h"

namespace NAMESPACE_NET {

	Client::Client(ClientDelegate* inDelegate)
	: mDelegate(inDelegate)
	{
	}

	Client::~Client()
	{
		disconnect();
	}

	void Client::connect(const std::string& inServerIP, const std::uint16_t inServerPort)
	{
		APP_LOGD("");

		int clientSock = 0;
		struct sockaddr_in serverAddress = {0, };

		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(inServerIP.c_str());
		serverAddress.sin_port = htons(inServerPort);

		if ((clientSock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			APP_LOGE("Socket creation error: %s(%d)", strerror(errno), errno);
			return;
		}

		do {
			/*
			struct sockaddr_in clientAddress = {0, };
			clientAddress.sin_family = AF_INET;
			clientAddress.sin_addr.s_addr = INADDR_ANY;
			clientAddress.sin_port = htons(clientPort);
			if (bind(clientSock, (struct sockaddr*) &clientAddress, sizeof(struct sockaddr_in)) < 0)
			{
				APP_LOGE("Can't bind client error %s(%d)", strerror(errno), errno);
				break;
			}
			*/

			if (::connect(clientSock, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0)
			{
				APP_LOGE("Connection Failed %s(%d)", strerror(errno), errno);
				break;
			}

			if (send(clientSock , (void*)"conn" , strlen("conn")) < 0)
			{
				APP_LOGI("Error sending conn packet to (%s)", inServerIP.c_str());
				break;
			}

			char buffer[512] = {0};
			auto len = 0;
			if (receive(clientSock, 8, buffer, &len))
			{
				APP_LOGE("Failed to read from server [%s]", inServerIP.c_str());
			}
			if (len > 0 && strncmp(buffer, "conn/ack", strlen("conn/ack")) == 0)
			{
				APP_LOGI("Client connected to server [%s]", inServerIP.c_str());
			} else
			{
				APP_LOGE("Failed to connect to server [%s]", inServerIP.c_str());
				return;
			}
			mClientSock = clientSock;
			if (mDelegate)
			{
				mDelegate->onConnected();
			}
			return;
		} while(false);

		// some error occurred.
		::close(clientSock);
	}

	void Client::disconnect()
	{
		::close(mClientSock);
		mClientSock = -1;
	}

	void Client::watch()
	{
		APP_LOGD("");

		if (mClientSock < 0)
		{
			APP_LOGE("client not connected");
			return;
		}

		char buffer[64] ={0, };
		std::int32_t length = 0;
		for (;;)
		{
			if (receive(mClientSock, sizeof(buffer), buffer, &length))
			{
				buffer[length] = 0;
				onDataReceived(buffer, std::size_t(length));
			} else {
				// do nothing for now.
			}
		}
	}

	std::int32_t Client::send(const std::int32_t inSock, const DataT& inData)
	{
		return send(inSock, inData.data(), inData.size());
	}

	std::int32_t Client::send(const std::int32_t inSock, const void* inData, const std::size_t inLength)
	{
		if (inSock > 0)
		{
			const auto str = utils::number::toString(inLength, constants::kPayloadLengthChars);
			auto bytesSent = ::write(inSock, str.data(), str.length());
			if (bytesSent < 0) {
				APP_LOGE("Failed to send data length %s(%d)", strerror(errno), errno);
			}
			bytesSent = ::write(inSock, inData, inLength);
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

	bool Client::receive(const std::int32_t inSock, const std::size_t inLength, void* const outData, std::int32_t* const outReadLength)
	{
		fd_set readSet;
		fd_set errorSet;

		// set listenfd and udpfd in readset
		FD_SET(inSock, &readSet);
		FD_SET(inSock, &errorSet);

		// select the ready descriptor
		auto nReady = ::select(inSock + 1, &readSet, nullptr, &errorSet, nullptr);

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
				if (FD_ISSET(inSock, &readSet))
				{
					static_cast<volatile char*>(outData)[bytesRead] = 0;
					*outReadLength = bytesRead;
					return true;
				} else if (FD_ISSET(inSock, &errorSet))
				{
					APP_LOGE("Looks like client disconnected");
					return false;
				}
			}
		} else {
			APP_LOGE("Failed to read %s(%d)", strerror(errno), errno);
		}

		return false;
	}

	void Client::onDataReceived(const void *inData, std::size_t inLength)
	{
		APP_LOGD("message : %s", (const char*)inData);
		if (mDelegate)
		{
			DataT data(inData, inLength + 1);
			mDelegate->onDataReceived(data);
		}
	}
}
