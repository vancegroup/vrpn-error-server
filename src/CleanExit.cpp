/** @file
	@brief Implementation

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program

*/

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "CleanExit.h"
#include "vrpn_SemaphoreGuard.h"

// Library/third-party includes
#if defined (_WIN32) && !defined (__CYGWIN__)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
#endif

// Standard includes
#include <stdexcept>


void cleanexit_setExitFlag() {
	vrpn_SemaphoreGuard guard(CleanExit::instance()._sem);
	if (guard.acquire()) {
		CleanExit::instance()._done = true;
	}
}
namespace {


// install a signal handler to shut down the devices
// On Windows, the signal handler is run in a different thread from
// the main application.  We don't want to go destroying things in
// here while they are being used there, so we set a flag telling the
// main program it is time to exit.
#if defined (_WIN32) && !defined (__CYGWIN__)
	/**
	 * Handle exiting cleanly when we get ^C or other signals.
	 */
	BOOL WINAPI handleConsoleSignalsWin(DWORD signaltype) {
		switch (signaltype) {
			case CTRL_C_EVENT:
			case CTRL_BREAK_EVENT:
			case CTRL_CLOSE_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				cleanexit_setExitFlag();
				return TRUE;
				// Don't exit, but return FALSE so default handler
				// gets called. The default handler, ExitProcess, will exit.
			default:
				return FALSE;
		}
	}

#else

#include <signal.h>
	void sighandler(int) {
		cleanexit_setExitFlag();
	}
#endif
}

CleanExit::CleanExit()
	: _done(false)
	, _registered(false) {
}

CleanExit & CleanExit::instance() {
	static CleanExit inst;
	return inst;
}

void CleanExit::registerHandlers() {
	if (_registered) {
		throw std::logic_error("CleanExit handlers were already registered elsewhere!");
	}
	_registered = true;
#ifdef _WIN32
	// This handles all kinds of signals.
	SetConsoleCtrlHandler(handleConsoleSignalsWin, TRUE);
#else
#ifdef sgi
	sigset(SIGINT, sighandler);
	sigset(SIGKILL, sighandler);
	sigset(SIGTERM, sighandler);
	sigset(SIGPIPE, sighandler);
#else
	signal(SIGINT, sighandler);
	signal(SIGKILL, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGPIPE, sighandler);
#endif // not sgi
#endif // not WIN32
}

CleanExit::~CleanExit() {
}

bool CleanExit::exitRequested() {
	vrpn_SemaphoreGuard guard(_sem);
	if (guard.acquire()) {
		return _done;
	}
	return false; // should never happen.
}


