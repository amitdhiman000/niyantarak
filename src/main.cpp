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
* Description : main file
*/

#include "linux/EventLoop.h"
#include "linux/Thread.h"
#include "Mouse.h"
#include "Debug.h"

#include <sys/signal.h>


using namespace core;
using namespace std;

static EventLoop sEventLoop;


void signalHandler (int signum)
{
	APP_LOGD("signal %d received", signum);
	sEventLoop.stop();
}

#if 1
int main(void)
{
	APP_LOGI("");

	struct sigaction newAction;
	/* Set up the structure to specify the new action. */
	newAction.sa_handler = signalHandler;
	sigemptyset(&newAction.sa_mask);
	newAction.sa_flags = 0;
	//struct sigaction oldAction;
	//sigaction (SIGINT, nullptr, &oldAction);
	//if (oldAction.sa_handler != SIG_IGN)
	sigaction(SIGINT, &newAction, nullptr);
	sigaction(SIGABRT, &newAction, nullptr);
	sigaction(SIGHUP, &newAction, nullptr);
	sigaction(SIGTERM, &newAction, nullptr);

	Mouse mouse;
	mouse.move(1000, 20);

	APP_LOGD("mouse moved");
	Thread th([]() { APP_LOGD("Hello world"); });
	th.start();
	th.detach();
	sEventLoop.start();

	APP_LOGD("Done");
	return 0;
}
#else

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

/* Send Fake Key Event */
static void SendKey(Display * disp, KeySym keysym, KeySym modsym)
{
	KeyCode keycode = 0, modcode = 0;
	keycode = XKeysymToKeycode (disp, keysym);
	if (keycode == 0) return;
	XTestGrabControl (disp, True);
	/* Generate modkey press */
	if (modsym != 0) {
		modcode = XKeysymToKeycode(disp, modsym);
		XTestFakeKeyEvent (disp, modcode, True, 0);
	}
	/* Generate regular key press and release */
	XTestFakeKeyEvent (disp, keycode, True, 0);
	XTestFakeKeyEvent (disp, keycode, False, 0);

	/* Generate modkey release */
	if (modsym != 0)
	XTestFakeKeyEvent (disp, modcode, False, 0);

	XSync (disp, False);
	XTestGrabControl (disp, False);
}

/* Main Function */
int main(void)
{
	Display *disp = XOpenDisplay (NULL);
	sleep (5);
	/* Send ASCII A & B */
	SendKey (disp, XK_A, 0);
	SendKey (disp, XK_B, 0);
	/* Send ALT+Tab */
	sleep (3);
	SendKey (disp, XK_Tab, XK_Alt_L);
	sleep (3);
	SendKey (disp, XK_Tab, XK_Alt_L);
}
#endif
