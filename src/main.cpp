//============================================================================
// Name        : main.cpp
// Author      : Amit Dhiman
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sys/signal.h>
#include <glib.h>
#include <iostream>

#include "Mouse.h"
#include "Debug.h"

using namespace core;
using namespace std;

static GMainLoop *sGMainLoop;


void signalHandler (int signum)
{
	LOGD("signal %d received", signum);
	if (sGMainLoop) {
		g_main_loop_quit(sGMainLoop);
	}
}

int main(void)
{
	LOGI("");

	sGMainLoop = g_main_loop_new(nullptr, FALSE);

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

	LOGD("mouse moved");
	g_main_loop_run(sGMainLoop);

	LOGD("Done");
	return 0;
}
