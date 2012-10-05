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
#ifndef INCLUDED_AppObject_h_GUID_cf6e4611_d60e_40ec_893c_5d57c79a1779
#define INCLUDED_AppObject_h_GUID_cf6e4611_d60e_40ec_893c_5d57c79a1779

// Internal Includes
// - none

// Library/third-party includes
#include <tclap/CmdLine.h>
#include <vrpn_MainloopContainer.h>
#include <vrpn_SerialPort.h>

// Standard includes
#include <iostream>
#include <string>


#define VERBOSE_START(X) std::cout << X << "..." << std::flush
#define VERBOSE_MSG(X) std::cout << X << std::endl
#define VERBOSE_DONE() std::cout << " done." << std::endl

class vrpn_Connection;

/// @brief An object to use for shared code between the various executables
class AppObject {
	public:
		AppObject();
		~AppObject();

		void parseAndBeginSetup(int argc, const char * const * argv);

		template<typename T>
		void addToMainloop(T obj) {
			_container.add(obj);
		}

		vrpn_Connection * getConnection() const {
			return _c;
		}

		vrpn_SerialPort & getSerialPort() {
			return _port;
		}

		template<typename Collection, typename MessageType>
		void addBinaryCommandOutput(std::string const & devName) {
			_container.add(new BinaryCommandOutput<Collection, MessageType>(devName.c_str(), _port, _c, _interval));
		}

		template<int NumChannels, char CommandPrefix>
		void addCommandOutput(std::string const & devName) {
			_container.add(new CommandOutput<NumChannels, CommandPrefix>(devName.c_str(), _port, _c, _interval));
		}

		void addReceiveEchoer();

		void enterMainloop();
	private:
		TCLAP::CmdLine _cmd;
		vrpn_Connection * _c;
		vrpn_SerialPort _port;
		vrpn_MainloopContainer _container;
		double _interval;


};

#endif // INCLUDED_AppObject_h_GUID_cf6e4611_d60e_40ec_893c_5d57c79a1779
