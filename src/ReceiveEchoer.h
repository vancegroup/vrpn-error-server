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
#ifndef INCLUDED_ReceiveEchoer_h_GUID_cedd6f79_a060_4039_9adb_74da92d753fd
#define INCLUDED_ReceiveEchoer_h_GUID_cedd6f79_a060_4039_9adb_74da92d753fd

// Internal Includes
#include <vrpn_SerialPort.h>

// Library/third-party includes
// - none

// Standard includes
#include <string>
#include <iostream>

class ReceiveEchoer {
	public:

		/// @brief Constructor
		ReceiveEchoer(vrpn_SerialPort & port, bool showData = true)
			: _port(port)
			, _showData(showData) {
		}

		/// @brief Mainloop function: must be called frequently to allow VRPN to
		/// service requests
		void mainloop() {
			std::string recv = _port.read_available_characters();
			if (!recv.empty()) {
				log() << "RECV: ";
				if (_showData) {
					log() << recv;
				} else {
					log() << recv.size() << " bytes";
				}
				log() << std::endl;
			}
		}
	private:
		/// @brief internal member function returning the stream for status messages.
		std::ostream & log() const {
			return std::cout;
		}

		vrpn_SerialPort & _port;
		bool _showData;
};

#endif // INCLUDED_ReceiveEchoer_h_GUID_cedd6f79_a060_4039_9adb_74da92d753fd
