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
#ifndef INCLUDED_vrpn_Boost_Callback_Wrappers_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2
#define INCLUDED_vrpn_Boost_Callback_Wrappers_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2

// Internal Includes
// - none

// Library/third-party includes
#include <boost/function.hpp>
#include <boost/static_assert.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/int.hpp>
#include <boost/bind.hpp>
#include <vrpn_Configure.h>

// Standard includes
// - none

template<typename CallbackFunctionPtr, typename ArgType>
class vrpn_Boost_Callback_Wrapper {
	public:
		/*
			/// Check that our assumptions are correct.
			BOOST_STATIC_ASSERT( (boost::function_types::function_arity<CallbackFunctionPtr>::value == 2) );
			BOOST_STATIC_ASSERT( (boost::is_same<typename boost::function_types::result_type<CallbackFunctionType>::type, void>::value) );
			typedef boost::function_types::parameter_types<CallbackFunctionType> ParameterTypes;
			BOOST_STATIC_ASSERT( (boost::is_same<boost::mpl::at<ParameterTypes, boost::mpl::int_<0> >, void *>::value) );

			/// Extract the actual data argument type.
			typedef boost::mpl::at<ParameterTypes, boost::mpl::int_<1> > ArgType;
			*/
		typedef boost::function<void (ArgType)> HandlerFunctorType;
		typedef vrpn_Boost_Callback_Wrapper<CallbackFunctionPtr, ArgType> type;

		vrpn_Boost_Callback_Wrapper(HandlerFunctorType handler)
			: _handler(handler) {}
		template<typename T, typename U>
		vrpn_Boost_Callback_Wrapper(T memfcn, U self)
			: _handler(boost::bind(memfcn, self, _1)) {}

		void * getUserdata() {
			return &_handler;
		}
		CallbackFunctionPtr getCallback() {
			return &type::_handlerTrampoline;
		}


	private:
		static void VRPN_CALLBACK _handlerTrampoline(void * userdata, ArgType arg) {
			(*static_cast<HandlerFunctorType*>(userdata))(arg);
		}
		HandlerFunctorType _handler;
};

#include <vrpn_Analog_Output.h>
typedef vrpn_Boost_Callback_Wrapper<vrpn_ANALOGOUTPUTCHANGEHANDLER, const vrpn_ANALOGOUTPUTCB> vrpn_Analog_Output_Change_Handler;

#endif // INCLUDED_vrpn_Boost_Callback_Wrappers_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2
