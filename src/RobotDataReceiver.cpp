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
#include "RobotDataReceiver.h"
#include "BinaryAnalogMessage.h"
#include "BinaryMessageHandler.h"
#include "Protocol.h"

// Library/third-party includes

// Standard includes
// - none


RobotDataReceiver::RobotDataReceiver(const char * basename, vrpn_Connection * c, vrpn_SerialPort & port) : _recvAdapter(port) {
	std::string base(basename);

	BinaryAnalogMessage<Protocol::CurrentWheelVelocities> * rpm = new BinaryCommandAnalog<Protocol::CurrentWheelVelocities>((base + "RPM").c_str(), c);
	_container.add(rpm);

	BinaryAnalogMessage<Protocol::CurrentPWMOutput> * pwm = new BinaryCommandAnalog<Protocol::CurrentPWMOutput>((base + "PWM").c_str(), c);
	_container.add(pwm);

	_handler.reset(BinaryMessageHandler::createNew()(rpm)(pwm).handleCollection<Protocol::RobotToComputer>());
}

void RobotDataReceiver::mainloop() {
	unsigned int bytes = _handler->getReceiver().receiveFrom(_recvAdapter);
	if (bytes > 0) {
		_handler->getReceiver().processMessages();
	}
	_handler.mainloop();
	_container.mainloop();
}
