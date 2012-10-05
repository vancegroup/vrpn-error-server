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
#ifndef INCLUDED_BinaryCommandOutput_h_GUID_4aa4e41d_6625_47d7_9066_8c8c4518db35
#define INCLUDED_BinaryCommandOutput_h_GUID_4aa4e41d_6625_47d7_9066_8c8c4518db35

// Internal Includes
#include "vrpn_Callbacks/vrpn_Analog_Output_Callbacks.h"
#include <vrpn_Analog_Output.h>
#include <vrpn_SerialPort.h>

// Library/third-party includes
#include <boost/mpl/size.hpp>
#include <boost/fusion/include/array.hpp>
#include <boost/fusion/include/transform.hpp>
#include <tuple-transmission/Send.h>
#include <tuple-transmission/BoundMessageType.h>
#include <tuple-transmission/transmitters/AutosizedBoostArrayBuffer.h>
#include <tuple-transmission/transmitters/VrpnSerial.h>
#include <boost/scoped_ptr.hpp>

// Standard includes
#include <iostream>
#include <algorithm>

struct TransformFunctorBase {
	typedef float result_type;
};

struct CastToFloatWrapper : TransformFunctorBase {
	template<typename T>
	float operator()(T val) const {
		return static_cast<float>(val);
	}
};

template < typename MessageCollection, typename MessageType, typename TransformFunctor = CastToFloatWrapper>
class BinaryCommandOutput {
	public:
		typedef BinaryCommandOutput<MessageCollection, MessageType> type;
		typedef MessageType message_type;
		typedef MessageCollection message_collection;
		typedef boost::mpl::size<MessageType> message_channels;
		typedef transmission::transmitters::VrpnSerial transmitter_type;

		/// @brief Constructor
		/// @param deviceName - a name to use for the vrpn_Analog_Output device it creates.
		/// @param c - a vrpn_Connection to use if you have already created one (optional)
		BinaryCommandOutput(const char * deviceName, vrpn_SerialPort & port, vrpn_Connection * c = NULL, double interval = 0, bool displaySendMessage = true)
			: _interval(interval)
			, _port(port)
			, _handler(&type::_changeHandler, this)
			, _transform()
			, _tx(port)
			, _displayMessage(displaySendMessage) {
			_out_server.reset(new vrpn_Analog_Output_Callback_Server(deviceName, c, message_channels()));
			vrpn_gettimeofday(&_nextMessage, NULL);
			vrpn_Callbacks::register_change_handler(_out_server.get(), _handler);
		}

		/// @brief Destructor - remove change handler from contained Analog_Output server.
		~BinaryCommandOutput() {
			vrpn_Callbacks::unregister_change_handler(_out_server.get(), _handler);
		}

		void setCommandInterval(double milliseconds) {
			_interval = milliseconds;
		}

		void setTransform(TransformFunctor const & newTransform) {
			_transform = newTransform;
		}

		/// @brief Mainloop function: must be called frequently to allow VRPN to
		/// service requests
		void mainloop() {
			_out_server->mainloop();
		}
	private:
		/// @brief internal member function returning the stream for status messages.
		std::ostream & log() const {
			return std::cout;
		}

		/// @brief Internal member function called when vrpn receives new values to output.
		void _changeHandler(const vrpn_ANALOGOUTPUTCB info);

		double _interval;
		vrpn_Analog_Output_Change_Handler _handler;
		vrpn_SerialPort & _port;
		boost::scoped_ptr<vrpn_Analog_Output_Callback_Server> _out_server;

		TransformFunctor _transform;

		transmitter_type _tx;

		bool _displayMessage;

		struct timeval _lastMessage;
		struct timeval _nextMessage;
};

template < typename MessageCollection, typename MessageType, typename TransformFunctor >
inline void BinaryCommandOutput<MessageCollection, MessageType, TransformFunctor>::_changeHandler(const vrpn_ANALOGOUTPUTCB info) {

	/// Find out if it's time to report again.
	struct timeval now;
	vrpn_gettimeofday(&now, NULL);
	if (_interval != 0) {
		if (vrpn_TimevalGreater(now, _nextMessage)) {
			_nextMessage = vrpn_TimevalNormalize(vrpn_TimevalSum(now, vrpn_MsecsTimeval(_interval)));
		} else {
			return;
		}
	}

	_lastMessage = now;

	if (_displayMessage) {
		log() << "Send!" << std::endl;
	}

	/// Copy data into a boost array.
	boost::array<vrpn_float64, message_channels::value> data;
	std::copy(info.channel, info.channel + message_channels(), data.begin());
	transmission::send<MessageCollection, MessageType>(_tx, boost::fusion::transform(data, _transform));
}

#endif // INCLUDED_BinaryCommandOutput_h_GUID_4aa4e41d_6625_47d7_9066_8c8c4518db35
