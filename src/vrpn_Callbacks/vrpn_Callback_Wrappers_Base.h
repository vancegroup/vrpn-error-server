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
#ifndef INCLUDED_vrpn_Callback_Wrappers_Base_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2
#define INCLUDED_vrpn_Callback_Wrappers_Base_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2

// Internal Includes
// - none

// Library/third-party includes
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vrpn_Configure.h>

// Standard includes
// - none

namespace callback_detail {
	template<typename HandlerFunctorType, typename ArgType, typename ReturnType>
	struct Trampoline {
		static ReturnType VRPN_CALLBACK call(void * userdata, ArgType arg) {
			return (*static_cast<HandlerFunctorType*>(userdata))(arg);
		}
	};

	// Partial specialization for void return types
	template<typename HandlerFunctorType, typename ArgType>
	struct Trampoline<HandlerFunctorType, ArgType, void> {
		static void VRPN_CALLBACK call(void * userdata, ArgType arg) {
			(*static_cast<HandlerFunctorType*>(userdata))(arg);
		}

	};
} // end of namespace callback_detail

template<typename Derived>
class vrpn_Boost_Callback_Wrapper_Base {
	public:
		typedef Derived derived;
		Derived & getDerived() {
			return *static_cast<Derived*>(this);
		}
};

template<typename CallbackFunctionPtr, typename ArgType, typename ReturnType = void>
class vrpn_Boost_Callback_Wrapper : public vrpn_Boost_Callback_Wrapper_Base<vrpn_Boost_Callback_Wrapper<CallbackFunctionPtr, ArgType, ReturnType> > {
	public:
		typedef boost::function<ReturnType(ArgType)> HandlerFunctorType;
		typedef vrpn_Boost_Callback_Wrapper<CallbackFunctionPtr, ArgType, ReturnType> type;

		/// Constructor from a function object.
		vrpn_Boost_Callback_Wrapper(HandlerFunctorType handler)
			: _handler(handler) {}

		/// Construction from a pointer to member function and an object pointer
		/// (uses boost::bind to create a function object)
		template<typename T, typename U>
		vrpn_Boost_Callback_Wrapper(T memfcn, U self)
			: _handler(boost::bind(memfcn, self, _1)) {}

		void * getUserdata() {
			return &_handler;
		}
		CallbackFunctionPtr getCallback() {
			return &TrampolineType::call;
		}


	private:
		typedef callback_detail::Trampoline<HandlerFunctorType, ArgType, ReturnType> TrampolineType;
		HandlerFunctorType _handler;
};

namespace vrpn_Callbacks {
	template<typename T, typename Derived>
	void register_change_handler(T vrpnptr, vrpn_Boost_Callback_Wrapper_Base<Derived> & callback) {
		vrpnptr->register_change_handler(callback.getDerived().getUserdata(), callback.getDerived().getCallback());
	}

	template<typename T, typename Derived>
	void unregister_change_handler(T vrpnptr, vrpn_Boost_Callback_Wrapper_Base<Derived> & callback) {
		vrpnptr->unregister_change_handler(callback.getDerived().getUserdata(), callback.getDerived().getCallback());
	}
}

#endif // INCLUDED_vrpn_Callback_Wrappers_Base_h_GUID_34c8198e_9d65_42f8_8c96_d91bbd8133e2
