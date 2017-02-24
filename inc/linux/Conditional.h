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
* Description : Condition utility class
*/

#ifndef __AMU_LINUX_CONDITIONAL_AMU__
#define __AMU_LINUX_CONDITIONAL_AMU__

#include "Mutex.h"

class Condition {
public:
	Condition(void)
	{
		pthread_cond_init(&mCondition, nullptr);
	}

	~Condition(void)
	{
		pthread_cond_destroy(&mCondition);
	}

	bool wait(Mutex &mutex)
	{
		return (0 == pthread_cond_wait(&mCondition, &mutex.mMutex));
	}

	bool wait(Mutex &mutex, unsigned time)
	{
		return (0 == pthread_cond_timedwait(&mCondition, &mutex.mMutex, nullptr));
	}

	void notifyone(void)
	{
		pthread_cond_signal(&mCondition);
	}

	void notifyall(void)
	{
		pthread_cond_broadcast(&mCondition);
	}

private:
	pthread_cond_t mCondition;
};


#endif /* __AMU_LINUX_CONDITIONAL_AMU__ */
