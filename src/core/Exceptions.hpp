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
* Dated : 24-02-2017
* Author : Amit Dhiman <amitdhiman000@gmail.com>
* Description : Exception classes
*/

#include <string>
#include <exception>

class DisplayNotFoundException: public std::exception {
public:
	explicit DisplayNotFoundException(const char* msg)
	: mErrorMsg(msg)
	{
	}

	explicit DisplayNotFoundException(const std::string& msg)
	: mErrorMsg(msg)
	{
	}

	virtual ~DisplayNotFoundException(void) throw()
	{
	}

	virtual const char* what(void) const throw()
	{
		return mErrorMsg.c_str();
	}

protected:
	std::string mErrorMsg;
};
