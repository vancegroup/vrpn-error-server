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
#include "RobotDataReceiver.h"

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_Tracker_RazerHydra.h>
#include <tuple-transmission/Send.h>
#include <tuple-transmission/transmitters/VrpnSerial.h>
#include <util/booststdint.h>

// Standard includes
#include <string>
#include <iostream>

struct ScaleAndCastToIntTransform {
	ScaleAndCastToIntTransform() : Kp(1) {}
	ScaleAndCastToIntTransform(vrpn_float64 gain) : Kp(gain) {}
	typedef stdint::int16_t result_type;
	template<typename T>
	stdint::int16_t operator()(T input) const {
		return static_cast<stdint::int16_t>(input * Kp);
	}

	vrpn_float64 Kp;
};

int main(int argc, char * argv[]) {
	std::string devName;
	std::string tracker;
	bool externalSource;

	AppObject app("Send appropriate error commands to a serial-connected controller, and serving status messages as a vrpn_Analog");

	TCLAP::ValueArg<std::string> outdevname("d", "devname", "base name of devices to create", false, "Robot", "device name");
	TCLAP::ValueArg<int> gain("g", "gain", "gain to apply to error before sending to robot", false, 5000, "Kp gain value");
	TCLAP::SwitchArg computeErr("c", "computeerror", "internally compute error and send output", false);
	TCLAP::SwitchArg hydra("h", "hydra", "create built-in Hydra server", false);
	TCLAP::ValueArg<std::string> trackerName("t", "trackername", "tracker device to access if computing error internally", false, "Tracker0@localhost", "VRPN device name");
	TCLAP::ValueArg<int> actual("a", "actual", "channel number for the 'actual' tracking sensor", false, 0, "channel number");
	TCLAP::ValueArg<int> desired("d", "desired", "channel number for the 'desired' tracking sensor", false, 1, "channel number");
	TCLAP::ValueArg<int> worldX("x", "worldX", "index of world x axis from tracker", false, 0, "axis index 0-2");
	TCLAP::ValueArg<int> worldZ("z", "worldZ", "index of world z axis from tracker", false, 1, "axis index 0-2");

	app.addArgs(outdevname)(gain)(computeErr)(hydra)(trackerName)(actual)(desired)(worldX)(worldZ);

	app.parseAndBeginSetup(argc, argv);

	devName = outdevname.getValue();

	app.addCustomBinaryCommandOutput<Protocol::ComputerToRobot, Protocol::XYIntError>(devName, false, ScaleAndCastToIntTransform(gain.getValue()));

	if (computeErr.getValue()) {
		if (hydra.getValue()) {
			app.addToMainloop(new vrpn_Tracker_RazerHydra("Tracker0", app.getConnection()));
		}
		vrpn_Tracker_Remote * tkr_remote = new vrpn_Tracker_Remote(trackerName.getValue().c_str(), app.getConnection());
		app.addToMainloop(tkr_remote);

		vrpn_Analog_Output_Remote * outRemote = new vrpn_Analog_Output_Remote(devName.c_str(), app.getConnection());
		app.addToMainloop(outRemote);

		app.addToMainloop(new ErrorComputer(tkr_remote, outRemote, actual.getValue(), desired.getValue(), worldX.getValue(), worldZ.getValue()));
	}

	VERBOSE_START("Creating robot data receiver devices");
	app.addToMainloop(new RobotDataReceiver(trackerName.getValue().c_str(), app.getConnection(), app.getSerialPort()));
	VERBOSE_DONE();

	VERBOSE_START("Sending 'StartControl' command");
	transmission::transmitters::VrpnSerial tx(app.getSerialPort());
	transmission::send<Protocol::ComputerToRobot, Protocol::StartControl>(tx);
	VERBOSE_DONE();

	app.enterMainloop();

	VERBOSE_START("Sending 'EndControl' command");
	transmission::send<Protocol::ComputerToRobot, Protocol::EndControl>(tx);
	VERBOSE_DONE();

	app.runMainloopOnce();
	VERBOSE_MSG("Exiting...");
	return 0;
}
