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
#ifndef INCLUDED_HandlerManager_h_GUID_aa190576_a439_4077_b64e_44e621eee586
#define INCLUDED_HandlerManager_h_GUID_aa190576_a439_4077_b64e_44e621eee586

// Internal Includes
#include "Types.h"
#include "PushToPointerVecFunctor.h"
#include "Registration.h"
#include "Metafunctions.h"

// Library/third-party includes
#include <tuple-transmission/Receiver.h>

// Standard includes
// - none


namespace FlexReceive {
	class HandlerManager {
		public:

			template<typename MessageCollection>
			typename detail::ComputeInitialRegProxy<MessageCollection, detail::PushToPtrVecFunctor>::type
			registerHandlerSet(transmission::Receiver<MessageCollection> & recv) {
				prepareToRegisterHandlerSet(recv);
				_handlerOwner.clear();
				return registerHandlerSetImpl(recv, detail::PushToPtrVecFunctor(_handlerOwner));
			}
		protected:

			template<typename MessageCollection>
			void prepareToRegisterHandlerSet(transmission::Receiver<MessageCollection> & recv) {
				/// Remove possible references to handler implementation
				recv.clearHandler();
				/// Delete implementation
				_implPtr.reset();
				/// Clear references to individual handlers
				_handlerMap.clear();
			}

			template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
			typename detail::ComputeInitialRegProxy<MessageCollection, HandlerOwnerAdditionFunctor>::type
			registerHandlerSetImpl(transmission::Receiver<MessageCollection> & recv, HandlerOwnerAdditionFunctor const& ownerAddition) {
				/// Must call prepareToRegisterHandlerSet then delete all old handlers before calling this!
				/// Populate the registration data struct with references as needed.
				typedef typename detail::ComputeRegData<MessageCollection, HandlerOwnerAdditionFunctor>::type RegDataType;
				typedef boost::shared_ptr<RegDataType> RegDataPtr;
				RegDataPtr data(new RegDataType(_handlerMap, _implPtr, ownerAddition, recv));

				/// Now, we are ready to get a new implementation.
				return Registration::createInitialProxy(data);
			}
		private:
			FlexReceive::Types::TypeHandlerMap _handlerMap;
			FlexReceive::Types::FlexRecvBasePtr _implPtr;
			FlexReceive::Types::GenericHandlerPtrVec _handlerOwner;
	};
} // end of namespace FlexReceive

#endif // INCLUDED_HandlerManager_h_GUID_aa190576_a439_4077_b64e_44e621eee586
