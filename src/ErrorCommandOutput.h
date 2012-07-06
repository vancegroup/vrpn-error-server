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
#include <boost/scoped_ptr.h>
#include <boost/array.h>
#include <boost/function.h>

// Standard includes
#include <string>
#include <iosfwd>

class ErrorCommandOutput {
	public:
		static const int NumChannels = 2;
		static const char CommandPrefix = 'E';

		typedef boost::function<void(std::string const&)> DataHandlerCallback;
		ErrorCommandOutput(const char * portName, long baud, DataHandlerCallback callback = DataHandlerCallback(), vrpn_Connection * c = NULL);
		
		void setDataCallback(DataHandlerCallback callback) {
			_callback = callback;
		}

		std::ostream & log() const;
		void mainloop();
	private:
	
		typedef boost::array<vrpn_float64, NumChannels> StateType;
		
		vrpn_SerialPort _port;
		boost::scoped_ptr<vrpn_Analog_Output_Server> _out_server;
		StateType _last_values;
		DataHandlerCallback _callback;
		std::string _recv;

};


#endif // INCLUDED_ErrorCommandOutput_h_GUID_c09e1d6f_b4a2_48ff_936a_42b3216d4bf4
