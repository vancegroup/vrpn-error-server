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
#include "BinaryCommandOutput.h"
#include "Protocol.h"
#include "CommandOutput.h"
#include "ErrorComputer.h"
#include "CleanExit.h"
#include "ReceiveEchoer.h"

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_MainloopContainer.h>
#include <vrpn_Tracker_RazerHydra.h>
#include <util/Stride.h>
#include <boost/scoped_ptr.hpp>

// Standard includes
#include <string>
#include <iostream>

extern const char * vrpn_MAGIC;

#define VERBOSE_START(X) std::cout << X << "..." << std::flush
#define VERBOSE_MSG(X) std::cout << X << std::endl
#define VERBOSE_DONE() std::cout << " done." << std::endl

typedef BinaryCommandOutput<Protocol::ComputerToRobot, Protocol::XYFloatError> BinaryXYFloatOutput;

int main(int argc, char * argv[]) {
	std::string port;
	std::string devName;
	long baud;
	int strideNum;
	int portNum;
	bool externalSource;
	double interval;
	bool useBinary;
	try {
		// Define the command line object.
		TCLAP::CmdLine cmd("Send appropriate error commands to a serial-connected controller", ' ',
		                   "1.0 (using " + std::string(vrpn_MAGIC) + ")");

		TCLAP::ValueArg<std::string> portname("p", "port", "serial port name", true, "", "serial port", cmd);
		TCLAP::ValueArg<std::string> outdevname("d", "devname", "vrpn_Analog_Output device to create", false, "ErrorCommand", "device name", cmd);
		TCLAP::ValueArg<long> baudrate("b", "baud", "baud rate", false, 115200, "baud rate", cmd);
		TCLAP::ValueArg<int> strideval("s", "stride", "stride between messages (number skipped per 1 sent)", false, 1, "stride", cmd);
		TCLAP::ValueArg<int> portnumval("n", "netport", "network port for VRPN to listen on (defaults to standard VRPN port)", false, vrpn_DEFAULT_LISTEN_PORT_NO, "port", cmd);
		TCLAP::SwitchArg externalData("e", "external", "use external source of error rather than built-in tracker", cmd);
		TCLAP::SwitchArg binaryData("x", "binary", "use the binary command", cmd);
		TCLAP::ValueArg<double> msginterval("i", "interval", "milliseconds of interval between messages", false, 0, "ms", cmd);
		cmd.parse(argc, argv);

		// Get the value parsed by each arg.
		port = portname.getValue();
		devName = outdevname.getValue();
		baud = baudrate.getValue();
		strideNum = strideval.getValue();
		portNum = portnumval.getValue();
		externalSource = externalData.getValue();
		useBinary = binaryData.getValue();
		interval = msginterval.getValue();
	} catch (TCLAP::ArgException & e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		return 1;
	}

	CleanExit::instance().registerHandlers();

#ifdef _WIN32
	WSADATA wsaData;
	int status;
	if ((status = WSAStartup(MAKEWORD(1, 1), &wsaData)) != 0) {
		std::cerr << "WSAStartup failed with " << status << std::endl;
		return 1;
	}
	VERBOSE_MSG("WSAStartup completed");
#endif // WIN32

	util::Stride s(strideNum);
	/// MainloopContainer will hold and own (and thus appropriately delete)
	/// anything we can give it that has a "mainloop" method.
	vrpn_MainloopContainer container;
	VERBOSE_START("Creating server connection on port " << portNum);
	vrpn_Connection * c = vrpn_create_server_connection(portNum);
	container.add(c);
	VERBOSE_DONE();

	VERBOSE_START("Opening serial port " << port);
	vrpn_SerialPort serialPort(port.c_str(), baud);
	VERBOSE_DONE();

	if (useBinary) {
		VERBOSE_START("Creating binary command output server");
		container.add(new BinaryXYFloatOutput(devName.c_str(), serialPort, c, interval));
		VERBOSE_DONE();
	} else {
		VERBOSE_START("Creating command output server");
		container.add(new CommandOutput < 2, 'E' > (devName.c_str(), serialPort, c, interval));
		VERBOSE_DONE();
	}


	VERBOSE_START("Creating receive echoer");
	container.add(new ReceiveEchoer(serialPort));
	VERBOSE_DONE();


	{
		boost::scoped_ptr<ErrorComputer> error_computations;
		if (!externalSource) {
			container.add(new vrpn_Tracker_RazerHydra("Tracker0", c));

			vrpn_Tracker_Remote * tkr_remote = new vrpn_Tracker_Remote("Tracker0@localhost", c);
			container.add(tkr_remote);

			vrpn_Analog_Output_Remote * outRemote = new vrpn_Analog_Output_Remote(devName.c_str(), c);
			container.add(outRemote);

			error_computations.reset(new ErrorComputer(tkr_remote, outRemote));
		}

		/// Error computer must be created after and destroyed before the mainloop container
		/// and its contents.
		VERBOSE_MSG("Entering mainloop.");
		while (!CleanExit::instance().exitRequested()) {
			container.mainloop();
			if (s && error_computations) {
				(*error_computations)();
			}
			s++;
			vrpn_SleepMsecs(1);
		}
		VERBOSE_MSG("Exiting...");
	}
	return 0;
}
