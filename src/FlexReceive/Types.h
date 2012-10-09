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
#ifndef INCLUDED_Types_h_GUID_6b3c31ed_b2c9_4f07_8aef_4dc2ed551c71
#define INCLUDED_Types_h_GUID_6b3c31ed_b2c9_4f07_8aef_4dc2ed551c71

// Internal Includes
// - none

// Library/third-party includes
#include <loki/AssocVector.h>
#include <util/TypeId.h>
#include <boost/shared_ptr.hpp>

// Standard includes
#include <vector>

namespace FlexReceive {
	class HandlerManager;
	class VRPNHandlerManager;

	namespace Impl {
		class ReceiveHandlerBase;

		template<typename MessageHandlerMap>
		class ReceiveHandlerImpl;
	} // end of namespace Impl

	namespace Registration {
		template<typename, typename>
		struct RegData;

		template<typename>
		class RegProxyBase;

		template<typename, typename>
		class RegProxy;

		class RegProxyFactory;
	} // end of namespace Registration

	namespace Types {
		typedef boost::shared_ptr<Impl::ReceiveHandlerBase> FlexRecvBasePtr;
		typedef boost::shared_ptr<void> GenericHandlerPtr;
		typedef std::vector<GenericHandlerPtr> GenericHandlerPtrVec;
		typedef Loki::AssocVector<util::TypeId, void *> TypeHandlerMap;
	} // end of namespace Types
} // end of namespace FlexReceive

#endif // INCLUDED_Types_h_GUID_6b3c31ed_b2c9_4f07_8aef_4dc2ed551c71
