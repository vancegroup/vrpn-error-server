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
#include "Protocol.h"
#include "ErrorComputer.h"
#include "ReceiveEchoer.h"

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_Tracker_RazerHydra.h>
#include <tuple-transmission/Send.h>
#include <tuple-transmission/transmitters/VrpnSerial.h>

// Standard includes
#include <string>
#include <iostream>

int main(int argc, char * argv[]) {
	std::string devName;
	bool externalSource;
	bool useBinary;

	AppObject app("Send appropriate error commands to a serial-connected controller");

	TCLAP::ValueArg<std::string> outdevname("d", "devname", "vrpn_Analog_Output device to create", false, "ErrorCommand", "device name");
	TCLAP::SwitchArg externalData("e", "external", "use external source of error rather than built-in tracker");
	TCLAP::SwitchArg binaryData("x", "binary", "use the binary command");

	app.addArgs(outdevname)(externalData)(binaryData);

	app.parseAndBeginSetup(argc, argv);

	// Get the value parsed by each arg.
	devName = outdevname.getValue();
	externalSource = externalData.getValue();
	useBinary = binaryData.getValue();

	/// MainloopContainer will hold and own (and thus appropriately delete)
	/// anything we can give it that has a "mainloop" method.

	if (useBinary) {
		VERBOSE_START("Creating binary command output server");
		app.addBinaryCommandOutput<Protocol::ComputerToRobot, Protocol::XYFloatError>(devName);
		VERBOSE_DONE();
	} else {
		VERBOSE_START("Creating command output server");
		app.addCommandOutput < 2, 'E' > (devName);
		VERBOSE_DONE();
	}


	VERBOSE_START("Creating receive echoer");
	app.addToMainloop(new ReceiveEchoer(app.getSerialPort()));
	VERBOSE_DONE();

	if (!externalSource) {
		app.addToMainloop(new vrpn_Tracker_RazerHydra("Tracker0", app.getConnection()));

		vrpn_Tracker_Remote * tkr_remote = new vrpn_Tracker_Remote("Tracker0@localhost", app.getConnection());
		app.addToMainloop(tkr_remote);

		vrpn_Analog_Output_Remote * outRemote = new vrpn_Analog_Output_Remote(devName.c_str(), app.getConnection());
		app.addToMainloop(outRemote);

		app.addToMainloop(new ErrorComputer(tkr_remote, outRemote));
	}

	if (useBinary) {
		VERBOSE_START("Sending 'StartControl' command");
		transmission::transmitters::VrpnSerial tx(app.getSerialPort());
		transmission::send<Protocol::ComputerToRobot, Protocol::StartControl>(tx);
		VERBOSE_DONE();
	}
	app.enterMainloop();
	if (useBinary) {
		VERBOSE_START("Sending 'EndControl' command");
		transmission::transmitters::VrpnSerial tx(app.getSerialPort());
		transmission::send<Protocol::ComputerToRobot, Protocol::EndControl>(tx);
		VERBOSE_DONE();
	}

	app.runMainloopOnce();
	VERBOSE_MSG("Exiting...");
	return 0;
}
