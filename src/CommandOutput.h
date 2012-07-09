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
#ifndef INCLUDED_CommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4
#define INCLUDED_CommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4

// Internal Includes
#include <vrpn_Analog_Output.h>
#include <vrpn_SerialPort.h>

// Library/third-party includes
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <util/Stride.h>

// Standard includes
#include <string>
#include <iostream>
#include <sstream>

/// @brief Template class to serve NumChannels of vrpn_Analog_Output
/// and send changes over serial in ASCII, prefixed by a letter and
/// space-delimited.
template < int NumChannels = 2, char CommandPrefix = 'E' >
class CommandOutput {
	public:
		typedef CommandOutput<NumChannels, CommandPrefix> type;
		typedef boost::function<void(std::string const&)> DataHandlerCallback;

		/// @brief Constructor
		/// @param deviceName - a name to use for the vrpn_Analog_Output device it creates.
		/// @param serialPortName - the name of the serial port (something like COM3 or /dev/ttyUSB0)
		/// @param baud - serial port data rate
		/// @param c - a vrpn_Connection to use if you have already created one (optional)
		/// @param callback - a function/functor to call when serial data is received (optional)
		/// @param commandStride - print the command only once in this many times (optional - default 20)
		CommandOutput(const char * deviceName, const char * serialPortName, long baud, vrpn_Connection * c = NULL, DataHandlerCallback callback = DataHandlerCallback(), int commandStride = 20)
			: _port(serialPortName, baud)
			, _callback(callback)
			, _showCommandStride(commandStride) {
			_out_server.reset(new vrpn_Analog_Output_Callback_Server(deviceName, c, NumChannels));
			_out_server->register_change_handler(this, &type::_changeHandlerTrampoline);
		}

		/// @brief Destructor - remove change handler from contained Analog_Output server.
		~CommandOutput() {
			_out_server->unregister_change_handler(this, &type::_changeHandlerTrampoline);
		}

		/// @brief Set a function/functor to call when data is received over serial,
		/// instead of printing it out.
		void setDataCallback(DataHandlerCallback callback) {
			_callback = callback;
		}

		/// @brief Mainloop function: must be called frequently to allow VRPN to
		/// service requests
		void mainloop();
	private:
		/// @brief internal member function returning the stream for status messages.
		std::ostream & log() const;

		/// @brief Internal member function called when vrpn receives new values to output.
		void _changeHandler(const vrpn_float64 * channels);

		/// @brief Static internal member used to handle vrpn callbacks and forward
		/// them to the object instance that created them.
		static void VRPN_CALLBACK _changeHandlerTrampoline(void * userdata, const vrpn_ANALOGOUTPUTCB info) {
			static_cast<type*>(userdata)->_changeHandler(info.channel);
		}


		vrpn_SerialPort _port;
		boost::scoped_ptr<vrpn_Analog_Output_Callback_Server> _out_server;

		/// @brief String contents of last command sent, excluding trailing \n -
		/// used to determine if an update should trigger sending a new command.
		std::string _last_cmd;

		/// @brief Callback to handle received serial data.
		DataHandlerCallback _callback;

		/// @brief Stride object to show only every n-th command
		util::Stride _showCommandStride;

};


template<int NumChannels, char CommandPrefix>
inline void CommandOutput<NumChannels, CommandPrefix>::mainloop() {
	_out_server->mainloop();

	std::string recv = _port.read_available_characters();
	if (!recv.empty()) {
		if (_callback) {
			_callback(recv);
		} else {
			log() << "RECV: " << recv << std::endl;
		}
	}
}

template<int NumChannels, char CommandPrefix>
inline void CommandOutput<NumChannels, CommandPrefix>::_changeHandler(const vrpn_float64 * channels) {
	std::ostringstream cmd;
	cmd << CommandPrefix;
	for (const vrpn_float64 * i = channels, * e = channels + NumChannels; i != e; ++i) {
		cmd << " " << *i;
	}

	/// If the new command would be different than the last thing we sent
	if (cmd.str() != _last_cmd) {
		_last_cmd = cmd.str();
		if (_showCommandStride) {
			log() << "SEND: " << cmd.str() << std::endl;
		}
		_showCommandStride++;
		/// Add a newline just to cap it off.
		cmd << '\n';
		_port.write(cmd.str());
	}
}

template<int NumChannels, char CommandPrefix>
inline std::ostream & CommandOutput<NumChannels, CommandPrefix>::log() const {
	return std::cout;
}

#endif // INCLUDED_CommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4
