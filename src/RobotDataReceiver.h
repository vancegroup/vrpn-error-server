/** @file
	@brief Header

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

#pragma once
#ifndef INCLUDED_RobotDataReceiver_h_GUID_dc50a193_9d2b_47b0_a1fe_8224495b27a3
#define INCLUDED_RobotDataReceiver_h_GUID_dc50a193_9d2b_47b0_a1fe_8224495b27a3

// Internal Includes
#include <NexusRobotProtocol.h>
#include "FlexReceive/Types.h"

// Library/third-party includes
#include <tuple-transmission/receiveadapters/VrpnSerial.h>
#include <tuple-transmission/Receiver.h>
#include <vrpn_Connection.h>
#include <boost/shared_ptr.hpp>

// Standard includes
// - none

class RobotDataReceiver {
	public:
		RobotDataReceiver(const char * basename, vrpn_Connection * c, vrpn_SerialPort & port);

		void mainloop();
	private:
		typedef FlexReceive::VRPNHandlerManager HandlerManagerType;
		transmission::VrpnSerialReceiveAdapter _recvAdapter;
		transmission::Receiver<Protocol::RobotToComputer> _recv;
		boost::shared_ptr<HandlerManagerType> _handler;
};

#endif // INCLUDED_RobotDataReceiver_h_GUID_dc50a193_9d2b_47b0_a1fe_8224495b27a3
