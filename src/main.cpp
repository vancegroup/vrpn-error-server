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
#include "CommandOutput.h"
#include "ErrorComputer.h"

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_MainloopContainer.h>
#include <vrpn_Tracker_RazerHydra.h>
#include <util/Stride.h>

// Standard includes
#include <string>
#include <iostream>

extern const char * vrpn_MAGIC;

int main(int argc, char * argv[]) {
	std::string port;
	std::string devName;
	long baud;
	int strideNum;
	try {
		// Define the command line object.
		TCLAP::CmdLine cmd("Send appropriate error commands to a serial-connected controller", ' ',
		                   "1.0 (using " + std::string(vrpn_MAGIC) + ")");

		TCLAP::ValueArg<std::string> portname("p", "port", "serial port name", true, "", "serial port", cmd);
		TCLAP::ValueArg<std::string> outdevname("d", "devname", "vrpn_Analog_Output device to create", false, "ErrorCommand", "device name", cmd);
		TCLAP::ValueArg<long> baudrate("b", "baud", "baud rate", false, 115200, "baud rate", cmd);
		TCLAP::ValueArg<long> strideval("s", "stride", "stride between messages (number skipped per 1 sent)", false, 1, "stride", cmd);
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		port = portname.getValue();
		devName = outdevname.getValue();
		baud = baudrate.getValue();
		strideNum = strideval.getValue();
	} catch (TCLAP::ArgException & e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}
	util::Stride s(strideNum);
	/// MainloopContainer will hold and own (and thus appropriately delete)
	/// anything we can give it that has a "mainloop" method.
	vrpn_MainloopContainer container;
	vrpn_Connection * c = vrpn_create_server_connection();
	container.add(c);

	container.add(new CommandOutput < 2, 'E' > (devName.c_str(), port.c_str(), baud, c));

	container.add(new vrpn_Tracker_RazerHydra("Tracker0", c));

	vrpn_Tracker_Remote * tkr_remote = new vrpn_Tracker_Remote("Tracker0@localhost", c);
	container.add(tkr_remote);

	vrpn_Analog_Output_Remote * outRemote = new vrpn_Analog_Output_Remote(devName.c_str(), c);
	container.add(outRemote);

	{
		/// Error computer must be created after and destroyed before the mainloop container
		/// and its contents.
		ErrorComputer error_computations(tkr_remote, outRemote);
		while (1) {
			container.mainloop();
			if (s) {
				error_computations();
			}
			s++;
			vrpn_SleepMsecs(1);
		}
	}
	return 0;
}
