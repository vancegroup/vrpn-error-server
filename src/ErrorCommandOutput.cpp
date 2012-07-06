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


ErrorCommandOutput::ErrorCommandOutput(const char * deviceName, const char * portName, long baud, vrpn_Connection * c, DataHandlerCallback callback)
	: _port(portName, baud)
	, _callback(callback) {
	_out_server.reset(new vrpn_Analog_Output_Callback_Server(deviceName, c, NumChannels));
	_out_server->register_change_handler(this, &ErrorCommandOutput::_changeHandlerTrampoline);
	std::fill(_last_values.begin(), _last_values.end(), 0);
}

ErrorCommandOutput::~ErrorCommandOutput() {
	_out_server->unregister_change_handler(this, &ErrorCommandOutput::_changeHandlerTrampoline);
}

void ErrorCommandOutput::mainloop() {
	_out_server->mainloop();

	_recv = _port.read_available_characters();
	if (!_recv.empty()) {
		if (_callback) {
			_callback(_recv);
		} else {
			log() << "Received data from serial: " << _recv << std::endl;
		}
	}
}

void ErrorCommandOutput::_changeHandler(const vrpn_float64 * channels) {
	std::ostringstream s;
	s << CommandPrefix;
	for (const vrpn_float64 * i = channels, * e = channels + NumChannels; i != e; ++i) {
		s << " " << static_cast<int>(*i);
	}
	log() << "Sending command '" << s.str() << "'" << std::endl;
	/// Add a newline just to cap it off.
	s << '\n';
	_port.write(s.str());
}
void ErrorCommandOutput::_changeHandlerTrampoline(void * userdata, const vrpn_ANALOGOUTPUTCB info) {
	static_cast<ErrorCommandOutput*>(userdata)->_changeHandler(info.channel);
}

std::ostream & ErrorCommandOutput::log() const {
	return (std::cout << "ErrorCommandOutput: ");
}
