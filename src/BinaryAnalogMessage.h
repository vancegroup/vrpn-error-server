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
// - none

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
// - none

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


template<typename Index, typename Sequence, typename Iter>
inline void copySequenceToIterImpl(Sequence const& s,
                                   Iter i,
                                   typename boost::enable_if<boost::mpl::less<Index, typename boost::fusion::result_of::size<Sequence>::type> >::type * = NULL) {
	*i = boost::fusion::at<Index>(s);
	copySequenceToIterImpl<typename boost::mpl::plus<Index, boost::mpl::int_<1> >::type>(s, i + 1);
}

template<typename Index, typename Sequence, typename Iter>
inline void copySequenceToIterImpl(Sequence const&,
                                   Iter,
                                   typename boost::disable_if<boost::mpl::less<Index, typename boost::fusion::result_of::size<Sequence>::type> >::type * = NULL) {
}



template<typename Sequence, typename Iter>
inline void copySequenceToIter(Sequence const& s, Iter i) {
	copySequenceToIterImpl<boost::mpl::int_<0> >(s, i);
}

template<typename MessageType, typename TransformFunctor = CastToFloat64>
class BinaryAnalogMessage {
		typedef typename boost::mpl::size<MessageType>::type message_size;
	public:
		typedef boost::array<vrpn_float64, message_size::value> array_type;
		typedef MessageType message_type;
		BinaryAnalogMessage(const char * name, vrpn_Connection *c, TransformFunctor const& func = TransformFunctor())
			: _f(func)
			, _server(new vrpn_Analog_Server(name, c, message_size())) {
			_server->setNumChannels(message_size());
		}

		template<typename SequenceType>
		void operator()(SequenceType const& s) {
			copySequenceToIter(boost::fusion::transform(s, _f), _server->channels());
			_server->report();
		}

		void mainloop() {
			_server->mainloop();
		}

	private:
		TransformFunctor _f;
		boost::scoped_ptr<vrpn_Analog_Server> _server;

};

#endif // INCLUDED_BinaryAnalogMessage_h_GUID_8fa5114b_939d_476b_b3b0_a1ff7fdc7f32
