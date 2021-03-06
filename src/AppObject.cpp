/**
	@file
	@brief Implementation

	@date 2012

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "AppObject.h"
#include "CleanExit.h"

// Library/third-party includes
#include <vrpn_Configure.h>

// Standard includes
#include <cstdlib>

extern const char * vrpn_MAGIC;

AppObject::AppObject(const char * usageMessage)
	: _cmd(usageMessage, ' ', "1.0 (using " + std::string(vrpn_MAGIC) + ")")
	, _adderProxy(_cmd)
	, _c(NULL)
	, _port()
	, _container()
	, _interval(0.0) {

	/// Let _cmd deal with its own mess.
	_cmd.setExceptionHandling(true);
	CleanExit::instance().registerHandlers();

#ifdef _WIN32
	WSADATA wsaData;
	int status;
	if ((status = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		std::cerr << "WSAStartup failed with " << status << std::endl;
		std::abort();
	}
	VERBOSE_MSG("WSAStartup completed");
#endif // WIN32

}

AppObject::~AppObject() {
#ifdef _WIN32
	WSACleanup();
#endif
}

void AppObject::parseAndBeginSetup(int argc, const char * const * argv) {
	TCLAP::ValueArg<std::string> portname("p", "port", "serial port name", true, "", "serial port", _cmd);
	TCLAP::ValueArg<long> baudrate("b", "baud", "baud rate", false, 115200, "baud rate", _cmd);
	TCLAP::ValueArg<int> portnumval("n", "netport", "network port for VRPN to listen on (defaults to standard VRPN port)", false, vrpn_DEFAULT_LISTEN_PORT_NO, "port", _cmd);
	TCLAP::ValueArg<double> msginterval("i", "interval", "milliseconds of interval between messages", false, 0, "ms", _cmd);
	TCLAP::ValueArg<std::string> inFilename("", "incominglog", "in log file", false, "", "filename", _cmd);
	TCLAP::ValueArg<std::string> outFilename("", "outgoinglog", "out log file", false, "", "filename", _cmd);

	_cmd.parse(argc, argv);

	bool logIn = false;
	bool logOut = false;

	if (!inFilename.getValue().empty()) {
		VERBOSE_MSG("Logging incoming to " << inFilename.getValue());
		logIn = true;
	}

	if (!outFilename.getValue().empty()) {
		VERBOSE_MSG("Logging outgoing to " << outFilename.getValue());
		logOut = true;
	}

	VERBOSE_START("Creating server connection on port " << portnumval.getValue());
	_c = vrpn_create_server_connection(portnumval.getValue(),
	                                   logIn ? inFilename.getValue().c_str() : NULL,
	                                   logOut ? outFilename.getValue().c_str() : NULL);
	_container.add(_c);
	VERBOSE_DONE();


	VERBOSE_START("Opening serial port " << portname.getValue() << " at a baudrate of " << baudrate.getValue());
	_port.open(portname.getValue().c_str(), baudrate.getValue());
	VERBOSE_DONE();

	_interval = msginterval.getValue();
}

void AppObject::enterMainloop() {

	VERBOSE_MSG("Mainloop starting.");
	while (!CleanExit::instance().exitRequested()) {
		_container.mainloop();
		vrpn_SleepMsecs(1);
	}
	VERBOSE_MSG(std::endl << "Mainloop stopping.");
}
