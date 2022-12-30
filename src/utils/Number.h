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
* Description : NumberUtlis
*/

#include <iomanip>
#include <sstream>

namespace utils {
namespace number {

	template<typename T>
	std::string toString(const T value, const std::size_t digitsCount)
	{
		std::ostringstream os;
		os<<std::setfill('0')<<std::setw(digitsCount)<<value;
		return os.str();
	}

}
}
