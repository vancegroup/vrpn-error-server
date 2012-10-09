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
#include "BinaryCommandOutput.h"
#include "CommandOutput.h"

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

class CmdAdderProxy {
	public:
		CmdAdderProxy(TCLAP::CmdLine & commandLine) : _cmd(commandLine) {}
		CmdAdderProxy & operator()(TCLAP::Arg & a) {
			_cmd.add(a);
			return *this;
		}
	private:
		TCLAP::CmdLine & _cmd;
};

/// @brief An object to use for shared code between the various executables
class AppObject {
	public:
		AppObject(const char * usageMessage);
		~AppObject();

		void parseAndBeginSetup(int argc, const char * const * argv);

		CmdAdderProxy & addArgs(TCLAP::Arg & a) {
			return _adderProxy(a);
		}

		template<typename T>
		T addToMainloop(T obj) {
			return _container.add(obj);
		}

		vrpn_Connection * getConnection() const {
			return _c;
		}

		vrpn_SerialPort & getSerialPort() {
			return _port;
		}

		template<typename Collection, typename MessageType, typename TransformFunctor>
		BinaryCommandOutput<Collection, MessageType, TransformFunctor> * addCustomBinaryCommandOutput(std::string const & devName, bool showSendMessages = true, TransformFunctor const& func = TransformFunctor()) {
			typedef BinaryCommandOutput<Collection, MessageType, TransformFunctor> ObjType;
			VERBOSE_START("Creating custom transform binary command output server: " << devName);
			ObjType * p = _container.add(new ObjType(devName.c_str(), _port, _c, _interval, showSendMessages));
			p->setTransform(func);
			VERBOSE_DONE();
			return p;
		}

		template<typename Collection, typename MessageType>
		BinaryCommandOutput<Collection, MessageType> * addBinaryCommandOutput(std::string const & devName, bool showSendMessages = true) {
			typedef BinaryCommandOutput<Collection, MessageType> ObjType;
			VERBOSE_START("Creating binary command output server: " << devName);
			ObjType * p = _container.add(new ObjType(devName.c_str(), _port, _c, _interval, showSendMessages));
			VERBOSE_DONE();
			return p;
		}

		template<int NumChannels, char CommandPrefix>
		CommandOutput<NumChannels, CommandPrefix> * addCommandOutput(std::string const & devName) {
			typedef CommandOutput<NumChannels, CommandPrefix> ObjType;
			VERBOSE_START("Creating ASCII command output server: " << devName);
			ObjType * p = _container.add(new ObjType(devName.c_str(), _port, _c, _interval));
			VERBOSE_DONE();
			return p;
		}

		void enterMainloop();

		void runMainloopOnce() {
			_container.mainloop();
		}
	private:
		TCLAP::CmdLine _cmd;
		CmdAdderProxy _adderProxy;
		vrpn_Connection * _c;
		vrpn_SerialPort _port;
		vrpn_MainloopContainer _container;
		double _interval;


};

#endif // INCLUDED_AppObject_h_GUID_cf6e4611_d60e_40ec_893c_5d57c79a1779
