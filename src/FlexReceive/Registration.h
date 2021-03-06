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
#ifndef INCLUDED_Registration_h_GUID_b3da0824_dbea_45c3_b51c_e5f8f0913072
#define INCLUDED_Registration_h_GUID_b3da0824_dbea_45c3_b51c_e5f8f0913072

// Internal Includes
#include "Types.h"
#include "Metafunctions.h"
#include "Impl.h"

// Library/third-party includes
// - none

// Standard includes
#include <typeinfo>

namespace FlexReceive {
	namespace Registration {
		/// @brief A class containing data/references that are shared by
		/// all registration proxies.
		///
		/// An instance is dynamically allocated by the HandlerManager automatically,
		/// and with lifetime managed by a smart pointer, is passed along chained
		/// registration calls.
		///
		/// Only needed to be used by RegProxyBase, RegProxy, and HandlerManager -
		/// not for "end-user" use.
		template<typename ReceiverType, typename HandlerOwnerAdditionFunctor>
		struct RegData {
			public:
				RegData(Types::TypeHandlerMap & handlers, Types::FlexRecvBasePtr & ptr, HandlerOwnerAdditionFunctor ownHandler, ReceiverType & recv)
					: _handlers(handlers)
					, _ptr(ptr)
					, _ownHandler(ownHandler)
					, _recv(recv) {}

				template<typename MessageType, typename HandlerType>
				void registerHandler(MessageType const&, HandlerType handler) {
					_ownHandler(handler);
					_handlers.insert(std::make_pair(util::TypeId(typeid(MessageType)), boost::any(handler)));
				}

				template<typename TypeMap>
				void createAndRegisterImplementation(TypeMap const &) {
					typedef typename detail::ImplPtr<TypeMap>::type ImplPtrType;

					/// Create new overall handler implementation managed by a specific shared_ptr
					ImplPtrType handlerImpl(Impl::ImplementationFactory::create<TypeMap>(_handlers));

					/// Register this overall handler with the receiver.
					_recv.setHandler(*handlerImpl);

					/// Change the handler manager's generic implementation pointer
					/// to own the handler we just created, so its lifetime is managed.
					_ptr = handlerImpl;
				}

			private:

				Types::TypeHandlerMap & _handlers;
				Types::FlexRecvBasePtr & _ptr;
				HandlerOwnerAdditionFunctor _ownHandler;
				ReceiverType & _recv;
		};

		/// @brief Class used to control constructor access by different
		/// template specializations of RegProxy, and by HandlerManagers.
		class RegProxyFactory {
			public:
				template<typename RegistrationData>
				static inline RegProxy<detail::EmptyMPLMap, RegistrationData> createInitialProxy(boost::shared_ptr<RegistrationData> data) {
					return RegProxyFactory::create < detail::EmptyMPLMap, RegistrationData>(data);
				}
			private:
				template<typename> friend class RegProxyBase;
				template<typename TypeMap, typename RegistrationData>
				static inline RegProxy<TypeMap, RegistrationData> create(boost::shared_ptr<RegistrationData> data) {
					return RegProxy<TypeMap, RegistrationData>(data);
				}
		};

		template<typename RegistrationData>
		class RegProxyBase {
			protected:
				typedef boost::shared_ptr<RegistrationData> DataPtr;

				/// @brief Constructor taking a smart pointer to the persistent
				/// proxy data.
				RegProxyBase(DataPtr data) : _isLast(true), _data(data) {}

				/// @brief Function called in "return" of derived classes
				/// that performs the message type handler registration
				/// and returns a new registration proxy with the additional
				/// type knowledge.
				template<typename TypeMap, typename Message, typename Handler, typename HandlerParam>
				typename detail::ComputeNextRegProxy<TypeMap, Message, Handler, RegistrationData>::type
				registerAndReturnNextProxy(HandlerParam h) {
					/// Insert this handler into the map
					_data->registerHandler(Message(), h);

					/// Indicate that we've been used to register a new handler,
					/// so this object has only partial type knowledge.
					_isLast = false;

					typedef typename detail::ComputeNextTypeMap<TypeMap, Message, Handler>::type NextMapType;

					return RegProxyFactory::create<NextMapType>(_data);
				}

				/// @brief This templated method must be called from derived
				/// class destructors, passing the TypeMap template argument.
				///
				/// This allows the base class to check if this is the last
				/// proxy in a "string" of chained proxy calls, and if so,
				/// use that complete type knowledge to generate a full
				/// message handler implementation.
				template<typename TypeMap>
				void derivedDisposalAction() {
					if (_isLast) {

						/// We are the return value from the last proxy call,
						/// and were unused (didn't get called) - so we have
						/// accumulated full type knowledge. Use it, before we
						/// lose it.
						_data->createAndRegisterImplementation(TypeMap());
					}
				}
			private:
				bool _isLast;
				DataPtr _data;
		};

		template<typename TypeMap, typename RegistrationData>
		class RegProxy : public RegProxyBase<RegistrationData> {
			public:
				typedef RegProxyBase<RegistrationData> base;

				template<typename Handler>
				typename detail::ComputeNextRegProxy<TypeMap, typename Handler::message_type, Handler, RegistrationData>::type
				registerHandler(Handler * h) {
					typedef typename Handler::message_type Message;
					/// Call actual work-doer and return next proxy for additional registration
					return base::template registerAndReturnNextProxy<TypeMap, Message, Handler>(h);
				}

				~RegProxy() {
					base::template derivedDisposalAction<TypeMap>();
				}
			private:
				friend class RegProxyFactory;
				RegProxy(boost::shared_ptr<RegistrationData> data) : RegProxyBase<RegistrationData>(data) {}
		};



	} // end of namespace Registration
} // end of namespace FlexReceive

#endif // INCLUDED_Registration_h_GUID_b3da0824_dbea_45c3_b51c_e5f8f0913072
