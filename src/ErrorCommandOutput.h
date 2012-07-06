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
#ifndef INCLUDED_ErrorCommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4
#define INCLUDED_ErrorCommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4

// Internal Includes
#include <vrpn_Analog_Output.h>
#include <vrpn_SerialPort.h>

// Library/third-party includes
#include <boost/scoped_ptr.hpp>
#include <boost/function.hpp>

#include <util/Stride.h>

// Standard includes
#include <string>
#include <iosfwd>

class ErrorCommandOutput {
	public:
		static const int NumChannels = 2;
		static const char CommandPrefix = 'E';

		typedef boost::function<void(std::string const&)> DataHandlerCallback;
		ErrorCommandOutput(const char * deviceName, const char * portName, long baud, vrpn_Connection * c = NULL, DataHandlerCallback callback = DataHandlerCallback());
		~ErrorCommandOutput();

		void setDataCallback(DataHandlerCallback callback) {
			_callback = callback;
		}

		std::ostream & log() const;
		void mainloop();
	private:
		void _changeHandler(const vrpn_float64 * channels);
		static VRPN_CALLBACK void _changeHandlerTrampoline(void * userdata, const vrpn_ANALOGOUTPUTCB info);

		vrpn_SerialPort _port;
		boost::scoped_ptr<vrpn_Analog_Output_Callback_Server> _out_server;
		std::string _last_cmd;
		DataHandlerCallback _callback;
		std::string _recv;
		util::Stride _showCommandStride;

};


#endif // INCLUDED_ErrorCommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4
