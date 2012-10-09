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
#ifndef INCLUDED_Metafunctions_h_GUID_3c58ffca_c330_403d_8ab1_1ce2f8ab3eac
#define INCLUDED_Metafunctions_h_GUID_3c58ffca_c330_403d_8ab1_1ce2f8ab3eac

// Internal Includes
#include "Types.h"

// Library/third-party includes
#include <boost/mpl/map.hpp>
#include <boost/shared_ptr.hpp>
#include <tuple-transmission/Receiver_fwd.h>

// Standard includes
// - none

namespace FlexReceive {
	namespace detail {

		typedef boost::mpl::map<> EmptyMPLMap;

		template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
		struct ComputeRegData {
			typedef Registration::RegData<transmission::Receiver<MessageCollection>, HandlerOwnerAdditionFunctor > type;
		};

		template<typename TypeMap, typename Message, typename Handler>
		struct ComputeNextTypeMap {
			typedef typename boost::mpl::insert<TypeMap, boost::mpl::pair<Message, Handler> >::type type;
		};

		template<typename TypeMap, typename RegData>
		struct ComputeRegProxy {
			typedef Registration::RegProxy<TypeMap, RegData> type;
		};

		template<typename TypeMap, typename Message, typename Handler, typename RegData>
		struct ComputeNextRegProxy {
			typedef typename ComputeNextTypeMap<TypeMap, Message, Handler>::type NextMapType;
			typedef typename ComputeRegProxy<NextMapType, RegData>::type type;
		};

		template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
		struct ComputeInitialRegProxy {
			typedef typename ComputeRegData<MessageCollection, HandlerOwnerAdditionFunctor>::type RegDataType;
			typedef typename ComputeRegProxy< EmptyMPLMap , RegDataType>::type type;
		};

		template<typename TypeMap>
		struct ImplPtr {
			typedef Impl::ReceiveHandlerImpl<TypeMap> object_type;
			typedef boost::shared_ptr< object_type > type;
		};


	} // end of namespace detail
} // end of namespace FlexReceive

#endif // INCLUDED_Metafunctions_h_GUID_3c58ffca_c330_403d_8ab1_1ce2f8ab3eac
