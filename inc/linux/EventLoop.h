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
* Description : Event loop utility class
*/

#ifndef __AMU_LINUX_EVENT_LOOP_H__
#define __AMU_LINUX_EVENT_LOOP_H__

#include <glib.h>

class EventLoop {
public:
	EventLoop(void)
	: mLoop(nullptr)
	{}

	~EventLoop(void)
	{
		stop();
	}

	bool start(void)
	{
		if (!running() && init()) {
			g_main_loop_run(mLoop);
			return true;
		}
		return false;
	}

	void stop(void)
	{
		if (mLoop) {
			g_main_loop_quit(mLoop);
			g_main_loop_unref(mLoop);
			mLoop = nullptr;
		}
	}

	/* time in milliseconds */
	void runfor(unsigned time)
	{
		if (init()) {
			g_timeout_add(time, [](gpointer data)->gboolean {
				static_cast<EventLoop *>(data)->stop();
				return FALSE;
			}, this);
		}
	}

	bool running(void) const
	{
		if (nullptr == mLoop) {
			return false;
		}
		return g_main_loop_is_running(mLoop);
	}
private:

	bool init(void)
	{
		if (nullptr == mLoop) {
			mLoop = g_main_loop_new(nullptr, FALSE);
		}
		return (nullptr != mLoop);
	}

private:
	GMainLoop *mLoop;
};

#endif /* __AMU_LINUX_EVENT_LOOP_H__ */
