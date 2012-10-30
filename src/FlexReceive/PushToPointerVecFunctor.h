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
#ifndef INCLUDED_PushToPointerVecFunctor_h_GUID_a1d6f9c4_e05f_4674_9021_7f38580f12f7
#define INCLUDED_PushToPointerVecFunctor_h_GUID_a1d6f9c4_e05f_4674_9021_7f38580f12f7

// Internal Includes
#include "Types.h"

// Library/third-party includes
#include <boost/shared_ptr.hpp>

// Standard includes
// - none


namespace FlexReceive {
	namespace detail {
		class PushToPtrVecFunctor {
			public:
				PushToPtrVecFunctor(Types::GenericHandlerPtrVec & handlerOwner) : _handlerOwner(handlerOwner) {}

				template<typename T>
				void operator()(T * handler) {
					boost::shared_ptr<T> p(handler);
					_handlerOwner.push_back(p);
				}
			private:
				Types::GenericHandlerPtrVec & _handlerOwner;
		};
	} // end of namespace detail
} // end of namespace FlexReceive

#endif // INCLUDED_PushToPointerVecFunctor_h_GUID_a1d6f9c4_e05f_4674_9021_7f38580f12f7
