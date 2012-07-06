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
#include "ErrorCommandOutput.h"

// Library/third-party includes
#include <tclap/CmdLine.h>

// Standard includes
#include <string>
#include <iostream>

extern const char * vrpn_MAGIC;

int main(int argc, char * argv[]) {
	std::string port;
	std::string devName;
	long baud;
	try {
		// Define the command line object.
		TCLAP::CmdLine cmd("Send appropriate error commands to a serial-connected controller", ' ',
		                   "1.0 (using " + std::string(vrpn_MAGIC) + ")");

		TCLAP::ValueArg<std::string> portname("p", "port", "serial port name", true, "", "serial port", cmd);
		TCLAP::ValueArg<std::string> outdevname("d", "devname", "vrpn_Analog_Output device to create", false, "ErrorCommand", "device name", cmd);
		TCLAP::ValueArg<long> baudrate("b", "baud", "baud rate", false, 115200, "baud rate", cmd);
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		port = portname.getValue();
		devName = outdevname.getValue();
		baud = baudrate.getValue();
	} catch (TCLAP::ArgException & e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

	ErrorCommandOutput output(devName.c_str(), port.c_str(), baud);
	while (1) {
		output.mainloop();
		vrpn_SleepMsecs(1);
	}
	return 0;
}
