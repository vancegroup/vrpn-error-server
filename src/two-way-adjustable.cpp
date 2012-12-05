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
#include <NexusRobotProtocol.h>
#include "ErrorComputer.h"
#include "RobotDataReceiver.h"
#include "ReceiveEchoer.h"

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_Tracker_RazerHydra.h>
#include <tuple-transmission/Send.h>
#include <tuple-transmission/transmitters/VrpnSerial.h>
#include <util/booststdint.h>

// Standard includes
#include <string>
#include <iostream>

struct ScaleAndRoundTransform {
	ScaleAndRoundTransform() : Kp(1) {}
	ScaleAndRoundTransform(vrpn_float64 gain) : Kp(gain) {}
	typedef stdint::int16_t result_type;
	template<typename T>
	stdint::int16_t operator()(T input) const {
		return static_cast<stdint::int16_t>(input * Kp + 0.5);
	}

	vrpn_float64 Kp;
};

int main(int argc, char * argv[]) {

	AppObject app("Send appropriate integer velocity/error commands to a serial-connected controller, and serving status messages as a vrpn_Analog");

	TCLAP::ValueArg<std::string> outdevname("", "devname", "base name of devices to create", false, "Robot", "device name");
	TCLAP::ValueArg<int> gain("g", "gain", "gain to apply to error before sending to robot", false, 5000, "Kp gain value");
	TCLAP::SwitchArg computeErr("c", "computeerror", "internally compute error and send output", true);
	TCLAP::SwitchArg useFloat("", "float", "use the float protocol with hard-coded Kp gain of 5000", false);
	TCLAP::SwitchArg razerhydra("r", "razer", "create built-in Razer Hydra server", true);
	TCLAP::ValueArg<std::string> trackerName("t", "trackername", "tracker device to access if computing error internally", false, "Tracker0@localhost", "VRPN device name");
	TCLAP::ValueArg<int> actual("a", "actual", "channel number for the 'actual' tracking sensor", false, 0, "channel number");
	TCLAP::ValueArg<int> desired("d", "desired", "channel number for the 'desired' tracking sensor", false, 1, "channel number");
	TCLAP::ValueArg<int> worldX("x", "worldX", "index of world x axis from tracker", false, 0, "axis index 0-2");
	TCLAP::ValueArg<int> worldZ("z", "worldZ", "index of world z axis from tracker", false, 1, "axis index 0-2");
	TCLAP::SwitchArg recv("", "receive", "enable receiving messages from the robot", false);
	TCLAP::SwitchArg verbose("", "verbose", "verbosely output what we send to the robot", false);

	app.addArgs(outdevname)(gain)(computeErr)(useFloat)(razerhydra)(trackerName)(actual)(desired)(worldX)(worldZ)(recv)(verbose);

	app.parseAndBeginSetup(argc, argv);

	std::string devName = outdevname.getValue();

	if (useFloat.getValue()) {
		app.addBinaryCommandOutput<Protocol::ComputerToRobot, Protocol::XYFloatError>(devName, verbose.getValue());
	} else {
		app.addCustomBinaryCommandOutput<Protocol::ComputerToRobot, Protocol::XYIntVelocities>(devName, verbose.getValue(), ScaleAndRoundTransform(gain.getValue()));
	}


	if (computeErr.getValue()) {
		if (razerhydra.getValue()) {
			VERBOSE_START("Creating Razer Hydra device server");
			app.addToMainloop(new vrpn_Tracker_RazerHydra("Tracker0", app.getConnection()));
			VERBOSE_DONE();
		}

		VERBOSE_START("Creating tracker remote: " << trackerName.getValue());
		vrpn_Tracker_Remote * tkr_remote = app.createVRPNRemote<vrpn_Tracker_Remote>(trackerName.getValue());
		VERBOSE_DONE();

		VERBOSE_START("Creating analog output remote: " << devName);
		vrpn_Analog_Output_Remote * outRemote = app.createVRPNRemote<vrpn_Analog_Output_Remote>(devName);
		VERBOSE_DONE();

		VERBOSE_START("Creating error computer, with actual on sensor " << actual.getValue() << ", desired on sensor " << desired.getValue());
		app.addToMainloop(new ErrorComputer(tkr_remote, outRemote, actual.getValue(), desired.getValue(), worldX.getValue(), worldZ.getValue()));
		VERBOSE_DONE();
	}

	if (recv.getValue()) {
		VERBOSE_START("Creating robot data receiver devices");
		app.addToMainloop(new RobotDataReceiver(devName.c_str(), app.getConnection(), app.getSerialPort()));
		VERBOSE_DONE();
	} else {
		VERBOSE_START("Creating receive echoer");
		app.addToMainloop(new ReceiveEchoer(app.getSerialPort(), true));
		VERBOSE_DONE();
	}

	VERBOSE_START("Sending 'StartControl' command");
	transmission::transmitters::VrpnSerial tx(app.getSerialPort());
	transmission::send<Protocol::ComputerToRobot, Protocol::StartControl>(tx);
	VERBOSE_DONE();

	app.enterMainloop();

	VERBOSE_START("Sending 'EndControl' command");
	transmission::send<Protocol::ComputerToRobot, Protocol::EndControl>(tx);
	VERBOSE_DONE();

	VERBOSE_MSG("Exiting...");
	return 0;
}
