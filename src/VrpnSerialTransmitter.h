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
#ifndef INCLUDED_VrpnSerialTransmitter_h_GUID_b6a7fab1_4156_4ef2_a375_64c76f2e2d85
#define INCLUDED_VrpnSerialTransmitter_h_GUID_b6a7fab1_4156_4ef2_a375_64c76f2e2d85

// Internal Includes
// - none

// Library/third-party includes
#include <tuple-transmission/detail/bases/TransmitterBase.h>
#include <vrpn_SerialPort.h>

// Standard includes
// - none

class VrpnSerialTransmitter : public ::transmission::transmitters::TransmitterBase<VrpnSerialTransmitter> {
	public:
		VrpnSerialTransmitter(vrpn_SerialPort & port) : _port(port) {}

		void writeByte(uint8_t v) {
			write(&v, 1);
		}

		void write(uint8_t * data, std::size_t len) {
			_port.write(data, len);
		}


	private:
		vrpn_SerialPort & _port;
};
#endif // INCLUDED_VrpnSerialTransmitter_h_GUID_b6a7fab1_4156_4ef2_a375_64c76f2e2d85
