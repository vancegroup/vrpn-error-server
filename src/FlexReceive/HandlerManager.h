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
	/** @brief A class that manages a flexible receive handler.

		This HandlerManager class contains data structures used by a
		template-driven receive handler implementation suitable to be specified
		to a tuple-transmission transmission::Receiver object. This link
		as well as the implementation object are constructed by a
		"registration sequence", started with a call to
		HandlerManager::registerHandlerSet passing the receiver, then chained
		method calls to .registerHandler.then repeated
		method calls of .registerHandler on the return value,
	*/
	class HandlerManager {
		public:

			/** @brief The method to begin the message-handler registration
				process.

				Pass your tuple-transmission receiver object. Any existing
				handler will be cleared.

				On the return value of this object, call .registerHandler,
				passing objects capable of handling and responding to a
				single message type. You can chain additional calls to .registerHandler,
				as it returns a proxy object with that method.

				After all registerHandler calls complete and the expression
				is fully evaluated, the final temporary proxy object will generate
				a receive handler implementation with full, appropriate type
				knowledge.
			*/
			template<typename MessageCollection>
			typename detail::ComputeInitialRegProxy<MessageCollection, detail::PushToPtrVecFunctor>::type
			registerHandlerSet(transmission::Receiver<MessageCollection> & recv) {
				prepareToRegisterHandlerSet(recv);
				_handlerOwner.clear();
				return registerHandlerSetImpl(recv, detail::PushToPtrVecFunctor(_handlerOwner));
			}
		protected:
			/**	@brief Clears data structures (in a suitable order for controlled
				destruction) in preparation for starting a new registration
				process.

				After calling this method, the derived class should delete
				the handlers using its implementation-specific ownership methods,
				then return the value of a call to registerHandlerSetImpl.
			*/
			template<typename MessageCollection>
			void prepareToRegisterHandlerSet(transmission::Receiver<MessageCollection> & recv) {
				/// Remove possible references to handler implementation
				recv.clearHandler();
				/// Delete implementation
				_implPtr.reset();
				/// Clear references to individual handlers
				_handlerMap.clear();
			}

			/** @brief Returns the initial registration proxy for a new
				registration process - should be returned by derived classes'
				registerHandlerSet.

				After this call and chained method calls to registerHandler,
				the registration proxy with the fullest type information will
				instantiate a receive handler implementation, set it as the
				handler in the Receiver, and transfer ownership of that implementation
				to this object.
			*/
			template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
			typename detail::ComputeInitialRegProxy<MessageCollection, HandlerOwnerAdditionFunctor>::type
			registerHandlerSetImpl(transmission::Receiver<MessageCollection> & recv, HandlerOwnerAdditionFunctor const& ownerAddition) {
				/// Must call prepareToRegisterHandlerSet then delete all old handlers before calling this!
				/// Populate the registration data struct with references as needed.
				typedef typename detail::ComputeRegData<MessageCollection, HandlerOwnerAdditionFunctor>::type RegDataType;
				typedef boost::shared_ptr<RegDataType> RegDataPtr;
				RegDataPtr data(new RegDataType(_handlerMap, _implPtr, ownerAddition, recv));

				/// Now, we are ready to get a new implementation.
				return Registration::RegProxyFactory::createInitialProxy(data);
			}
		private:
			/// @brief Map from a message's typeid to a handler stored in
			/// a generic container without type knowledge. The type knowledge
			/// in the generated ReceiveHandlerImpl is used to extract and
			/// invoke its contents.
			FlexReceive::Types::TypeHandlerMap _handlerMap;

			/// @brief Pointer to the generated ReceiveHandlerImpl, which
			/// provides for this object to own the generated handler.
			FlexReceive::Types::FlexRecvBasePtr _implPtr;

			/// @brief A vector of generic smart pointers for use in the
			/// general case to manage ownership of individual handler objects.
			FlexReceive::Types::GenericHandlerPtrVec _handlerOwner;
	};
} // end of namespace FlexReceive

#endif // INCLUDED_HandlerManager_h_GUID_aa190576_a439_4077_b64e_44e621eee586
