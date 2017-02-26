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
* Description : Mutex utility class
*/

#ifndef __AMU_LINUX_MUTEX_AMU__
#define __AMU_LINUX_MUTEX_AMU__

class Mutex {
	Mutex(void)
	{
		pthread_mutex_init(&mMutex, nullptr);
	}

	~Mutex(void)
	{
		pthread_mutex_destroy(&mMutex);
	}

	Mutex(const Mutex &) = delete;
	Mutex(Mutex &&) = delete;
	Mutex &operator=(const Mutex &) = delete;
	Mutex &operator=(Mutex &&) = delete;

	bool lock(void)
	{
		return (0 == pthread_mutex_lock(&mMutex));
	}

	bool unlock(void)
	{
		return (0 == pthread_mutex_unlock(&mMutex));
	}

	bool trylock(void)
	{
		return (0 == pthread_mutex_trylock(&mMutex));
	}

	friend class Condition;

private:
	pthread_mutex_t mMutex;
};

#endif /* __AMU_LINUX_MUTEX_AMU__ */
