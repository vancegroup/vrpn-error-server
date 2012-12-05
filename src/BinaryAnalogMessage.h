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
#ifndef INCLUDED_BinaryAnalogMessage_h_GUID_8fa5114b_939d_476b_b3b0_a1ff7fdc7f32
#define INCLUDED_BinaryAnalogMessage_h_GUID_8fa5114b_939d_476b_b3b0_a1ff7fdc7f32

// Internal Includes
#include "FlexReceive/MessageHandlerBase.h"
#include "CopySequenceToArray.h"

// Library/third-party includes
#include <vrpn_Analog.h>
#include <boost/mpl/size.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/fusion/include/at.hpp>
#include <boost/fusion/include/size.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/utility/enable_if.hpp>

// Standard includes
#include <iostream>
#include <string>

class CastToFloat64 {
	public:
		/*
		template<typename Sig>
		struct result;

		template<typename Self, typename T>
		struct result<Self(T)> {
		    typedef vrpn_float64 type;
		};
		*/
		typedef vrpn_float64 result_type;
		template<typename T>
		vrpn_float64 operator()(T v) const {
			return static_cast<vrpn_float64>(v);
		}
};




template<typename MessageType, typename TransformFunctor = CastToFloat64>
class BinaryAnalogMessage : public FlexReceive::MessageHandlerBase {
		typedef typename boost::mpl::size<MessageType>::type message_size;
	public:
		virtual ~BinaryAnalogMessage() {}
		typedef boost::array<vrpn_float64, message_size::value> array_type;
		typedef MessageType message_type;
		BinaryAnalogMessage(const char * name, vrpn_Connection *c, TransformFunctor const& func = TransformFunctor())
			: _f(func)
			, _server(new vrpn_Analog_Server(name, c, message_size()))
			, _name(name) {
			_server->setNumChannels(message_size());
		}

		template<typename SequenceType>
		void operator()(SequenceType const& s) {
			std::cout << _name << ": Got sequence data to report" << std::endl;

			copySequenceToArray(boost::fusion::transform(s, _f), _server->channels());
			_server->report();
		}

		void mainloop() {
			_server->mainloop();
		}

	private:
		TransformFunctor _f;
		boost::scoped_ptr<vrpn_Analog_Server> _server;
		std::string _name;

};

#endif // INCLUDED_BinaryAnalogMessage_h_GUID_8fa5114b_939d_476b_b3b0_a1ff7fdc7f32
