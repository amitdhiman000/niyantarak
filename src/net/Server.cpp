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
* Description : Server
*/

#include "Server.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <vector>


#include "Constants.h"
#include "Defines.h"
#include "Debug.h"
#include "utils/Number.h"


using namespace std;

namespace NAMESPACE_NET {

	Server::Server(ServerDelegate* inDelegate)
	: mDelegate(inDelegate)
	{
	}

	Server::~Server()
	{
	}

	bool Server::start(const std::uint16_t inPort)
	{
		APP_LOGD("");
		int serverSock = 0;

		fd_set readSet;

		struct sockaddr_in clientAddress = {0, };
		struct sockaddr_in serverAddress = {0, };

		clientAddress.sin_family = AF_INET;
		clientAddress.sin_addr.s_addr = INADDR_ANY;
		//clientAddress.sin_port = htons(10000);

		serverAddress.sin_family = AF_INET;
		serverAddress.sin_addr.s_addr = inet_addr(SERVER_ADDR);
		serverAddress.sin_port = htons(inPort);

		if ((serverSock = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			APP_LOGE("Socket creation error: %s(%d)", strerror(errno), errno);
			return false;
		}

#if 1
		std::int32_t reuseAddress = 1;
		if (::setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &reuseAddress, sizeof(reuseAddress)) == -1)
		{
			APP_LOGE("Socket setsockopt error: %s(%d)", strerror(errno), errno);
			return false;
		}
#endif
		// binding server addr structure to serverSock
		if (::bind(serverSock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
		{
			APP_LOGE("Socket bind error: %s(%d)", strerror(errno), errno);
			return false;
		}
		::listen(serverSock, 10);

		APP_LOGD("Listening for connections...");
		// clear the descriptor set
		FD_ZERO(&readSet);

		for (;;)
		{
			FD_SET(serverSock, &readSet);

			// select the ready descriptor
			auto nReady = select(serverSock + 1, &readSet, nullptr, nullptr, nullptr);
			if (nReady < 0)
			{
				APP_LOGE("Error from select %s(%d)", strerror(errno), errno);
				return false;
			}
			// if TCP socket is readable then handle
			if (FD_ISSET(serverSock, &readSet))
			{
				APP_LOGD("Got a new connection");
				socklen_t length = sizeof(clientAddress);
				auto clientFd = ::accept(serverSock, reinterpret_cast<sockaddr*>(&clientAddress), &length);
				if (clientFd > 0)
				{
					ClientInfoT clientInfo;
					clientInfo.clientFd = clientFd;
					clientInfo.address = inet_ntoa(clientAddress.sin_addr);

					std::int32_t size = 0;
					char buffer[32] = {0, };
					if (receive(clientFd, sizeof(buffer) - 1, (void*)buffer, &size))
					{
						// extract extra info from first packet
						addClient(clientInfo);
						if (send(clientFd , "conn/ack" , strlen("conn/ack")) < 0)
						{
							APP_LOGI("Error sending conn/ack packet to (%s)", clientInfo.address.c_str());
							break;
						}
						watch();
					}
				}
			}
	    }
		return false;
	}

	void Server::watch()
	{
		if (mWatching) {
			return;
		}

		mWatching = true;
		auto watcherFunc = [this](ShouldStopFxnT shouldStop) -> void
		{
			fd_set readSet;
			fd_set errorSet;

			for (; !shouldStop() ;)
			{
				vector<int32_t> listenFds = {};
				{
					std::unique_lock lock(mClientsMutex);
					if (mClientsInfo.empty())
					{
						APP_LOGI("No any clients connected [stop watching]");
						break;
					}
					for (const auto& client : mClientsInfo)
					{
						listenFds.push_back(client.first);
					}
				}

				int32_t maxFd = 0;
				// clear the descriptor set
				FD_ZERO(&readSet);
				FD_ZERO(&errorSet);
				for (const auto& fd : listenFds)
				{
					FD_SET(fd, &readSet);
					FD_SET(fd, &errorSet);
					if (maxFd < fd)
					{
						maxFd = fd;
					}
				}
				APP_LOGD("maxFd %d", maxFd);
				//struct timeval timeoutVal = {5, 0};
				// select the ready descriptor
				auto nReady = ::select(maxFd + 1, &readSet, nullptr, &errorSet, nullptr);
				if (nReady < 0)
				{
					APP_LOGE("Error from select %s(%d)", strerror(errno), errno);
					return;
				} else if (nReady == 0)
				{
					APP_LOGD("timeout, read again");
					// timeout, just go and listen again.
				} else
				{
					APP_LOGD("something happened nReady %d", nReady);
					for (const auto& fd : listenFds)
					{
						if (FD_ISSET(fd, &errorSet))
						{
							APP_LOGE("Error on fd : %d", fd);
							{
								std::unique_lock lock(mClientsMutex);
								auto itr = mClientsInfo.find(fd);
								if (itr != mClientsInfo.end())
								{
									removeClient(mClientsInfo[fd]);
								}
							}
						} else if (FD_ISSET(fd, &readSet))
						{
							APP_LOGD("Ready for read fd : %d", fd);
							{
								std::unique_lock lock(mClientsMutex);
								auto itr = mClientsInfo.find(fd);
								if (itr != mClientsInfo.end())
								{
									DataT data = receive(fd, 32);
									if (data.size() > 0)
									{
										notifyClientData(data);
									} else {
										APP_LOGD("Client is disconnected... [%d]", fd);
										disconnect(fd);
									}
								}
							}
						}
					}
				}
			}
			APP_LOGD("Thread stopped");
			mWatching = false;
		};

		mThread.execute(std::function<void(ShouldStopFxnT)>(watcherFunc));

		if (!mThread)
		{
			//mThread = std::move(Thread<void(void)>(watcherFunc));
			//mThread.run();
		}
	}

	void Server::stop()
	{
		close(mServerSock);
		mServerSock = -1;
		// close all active client connections
		for (auto clientInfo : mClientsInfo)
		{
			disconnect(clientInfo.first);
		}
	}

	void Server::disconnect(const std::int32_t inClientFd)
	{
		send(inClientFd, "terminate", strlen("terminate"));
		::close(inClientFd);
		const auto clientItr = mClientsInfo.find(inClientFd);
		if (clientItr != mClientsInfo.end())
		{
			removeClient(clientItr->second);
		}
	}

	std::int32_t Server::send(const std::int32_t inClientSock, const DataT& inData)
	{
		return send(inClientSock, inData.data(), inData.size());
	}

	std::int32_t Server::send(const std::int32_t inClientSock, const void* inData, const std::size_t inLength)
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

	DataT Server::receive(const std::int32_t inSock, const std::size_t inLength)
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

	bool Server::receive(const std::int32_t inSock, std::size_t inLength, void* const outData, std::int32_t* const outReadLength)
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

	void Server::addClient(const ClientInfoT& inClientInfo)
	{
		// add client
		{
			std::unique_lock lock(mClientsMutex);
			mClientsInfo.insert({inClientInfo.clientFd, inClientInfo});
		}

		if (mDelegate)
		{
			mDelegate->onClientConnected(inClientInfo);
		}
	}

	void Server::removeClient(const ClientInfoT& inClientInfo)
	{
		// remove client
		{
			std::unique_lock lock(mClientsMutex);
			mClientsInfo.erase(inClientInfo.clientFd);
		}

		if (mDelegate)
		{
			mDelegate->onClientDisconnected(inClientInfo);
		}
	}

	void Server::notifyClientData(const DataT& inData)
	{
		if (mDelegate)
		{
			mDelegate->onDataReceived(inData);
		}
	}
}
