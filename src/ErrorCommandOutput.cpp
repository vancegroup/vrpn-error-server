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
// - none

// Standard includes
#include <algorithm>
#include <iostream>
#include <sstream>

ErrorCommandOutput::ErrorCommandOutput(const char * deviceName, const char * portName, long baud, DataHandlerCallback callback, vrpn_Connection * c)
	: _port(portName, baud)
	, _callback(callback) {
	_out_server.reset(new vrpn_Analog_Output_Server(deviceName, c, NumChannels));
	std::fill(_last_values.begin(), _last_values.end(), 0);
}

void ErrorCommandOutput::mainloop() {
	_out_server->mainloop();
	StateType newValues;
	std::copy(_out_server->o_channels(), _out_server->o_channels() + NumChannels, newValues.begin());
	
	if (newValues != _last_values) {
		std::ostringstream s;
		s << CommandPrefix;
		for (StateType::const_iterator i = newValues.begin(), e = newValues.end(); i != e; ++i) {
			s << " " << static_cast<int>(*i);
		}
		log() << "Sending command '" << s.str() << "'" << std::endl;
		/// Add a newline just to cap it off.
		s << '\n';
		_port.write(s.str());
		_last_values = newValues;
	}
	
	_recv = _port.read_available_characters();
	if (!_recv.empty()) {
		if (_callback) {
			_callback(_recv);
		} else {
			log() << "Received data from serial: " << _recv << std::endl;
		}
	}
}

std::ostream & ErrorCommandOutput::log() const {
	return (std::cout << "ErrorCommandOutput: ");
}
