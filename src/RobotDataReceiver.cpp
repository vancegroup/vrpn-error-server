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
#include <NexusRobotProtocol.h>
#include "FlexReceive/HandlerManager.h"
#include "FlexReceive/VRPNHandlerManager.h"

// Library/third-party includes

// Standard includes
// - none


RobotDataReceiver::RobotDataReceiver(const char * basename, vrpn_Connection * c, vrpn_SerialPort & port)
	: _recvAdapter(port)
	, _handler(new HandlerManagerType) {
	std::string base(basename);
	_handler->registerHandlerSet(_recv)
	.registerHandler(new BinaryAnalogMessage<Protocol::CurrentWheelVelocities>((base + "RPM").c_str(), c))
	.registerHandler(new BinaryAnalogMessage<Protocol::CurrentPWMOutput>((base + "PWM").c_str(), c));
}

void RobotDataReceiver::mainloop() {
	unsigned int bytes = _recv.receiveFrom(_recvAdapter);
	if (bytes > 0) {
		_recv.processMessages();
	}
	_handler->mainloop();
}
