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
#ifndef INCLUDED_FlexibleReceiveHandlerManager_h_GUID_2fadebac_7348_4959_b0d0_6ce7a6daa185
#define INCLUDED_FlexibleReceiveHandlerManager_h_GUID_2fadebac_7348_4959_b0d0_6ce7a6daa185

// Internal Includes
// - none

// Library/third-party includes
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/pair.hpp>
#include <loki/AssocVector.h>
#include <util/TypeId.h>

// Standard includes
#include <vector>


class FlexibleReceiveHandlerManager;

namespace FlexReceive {
	namespace Impl {
		class FlexibleReceiveHandlerBase;
	} // end of namespace Impl

	namespace Types {
		typedef boost::shared_ptr<Impl::FlexibleReceiveHandlerBase> FlexRecvBasePtr;
		typedef boost::shared_ptr<void> GenericHandlerPtr;
		typedef std::vector<GenericHandlerPtr> GenericHandlerPtrVec;
		typedef Loki::AssocVector<util::TypeId, void *> TypeHandlerMap;
	} // end of namespace Types

	namespace Impl {
		class FlexibleReceiveHandlerBase {
			public:
				virtual ~FlexibleReceiveHandlerBase() {}
		};

		template<typename MessageHandlerMap>
		class FlexibleReceiveHandlerImpl {
			public:
				template<typename M, typename Sequence>
				void operator()(M const&,
				                Sequence const& s,
				                typename M::message_type_tag * = NULL,
				                typename boost::enable_if<boost::mpl::has_key<MessageHandlerMap, M> >::type * = NULL) {

					typedef typename boost::mpl::at<MessageHandlerMap, M>::type HandlerType;
					HandlerType * h = static_cast<HandlerType *>(_handlers[util::TypeId(typeid(M))]);
					(*h)(s);
				}

				template<typename M, typename Sequence>
				void operator()(M const&,
				                Sequence const&,
				                typename M::message_type_tag * = NULL,
				                typename boost::disable_if<boost::mpl::has_key<MessageHandlerMap, M> >::type * = NULL) {
					/* unhandled message */
				}

			private:
				friend class FlexibleReceiveHandlerManager;
				FlexibleReceiveHandlerImpl(Types::TypeHandlerMap & handlers) : _handlers(handlers) {}
				Types::TypeHandlerMap & _handlers;
		};

	} // end of namespace Impl

	namespace Registration {

		template<typename ReceiverType, typename HandlerOwnerAdditionFunctor>
		struct RegData {
			RegData(Types::TypeHandlerMap & handlers, Types::FlexRecvBasePtr & ptr, HandlerOwnerAdditionFunctor ownHandler, ReceiverType & recv)
				: _handlers(handlers)
				, _ptr(ptr)
				, _ownHandler(ownHandler)
				, _recv(recv) {}

			Types::TypeHandlerMap & _handlers;
			Types::FlexRecvBasePtr & _ptr;
			HandlerOwnerAdditionFunctor _ownHandler;
			ReceiverType & _recv;
		};

		template<typename, typename>
		class RegProxy;

		template<typename RegistrationData>
		class RegProxyBase {
			protected:
				RegProxyBase(RegistrationData & data) : _isLast(true), _data(data) {}

				template<typename TypeMap, typename Message, typename Handler>
				struct ReturnType {
					typedef RegProxy<typename boost::mpl::insert<TypeMap, boost::mpl::pair<Message, Handler> >::type, RegistrationData> type;
				};

				template<typename TypeMap, typename Message, typename Handler>
				typename ReturnType<TypeMap, Message, Handler>::type
				createNextProxy() {
					_isLast = false;
					return typename ReturnType<TypeMap, Message, Handler>::type(_data);
				}

				template<typename TypeMap>
				void instantiateImplementationAndSetHandler() {
					/// Create new overall handler implementation managed by a specific shared_ptr
					boost::shared_ptr<Impl::FlexibleReceiveHandlerImpl<TypeMap> > handlerImpl(new Impl::FlexibleReceiveHandlerImpl<TypeMap>(_data._handlers));
					/// Register this overall handler with the receiver.
					_data._recv.setHandler(*handlerImpl);

					/// Change the handler manager's generic implementation pointer
					/// to own the handler we just created.
					_data._ptr = handlerImpl;
				}

				bool _isLast;
				RegistrationData & _data;

		};

		template<typename TypeMap, typename RegistrationData>
		class RegProxy : public RegProxyBase<RegistrationData> {
			public:
				typedef RegProxyBase<RegistrationData> base;

				template<typename Handler>
				typename base::template ReturnType<TypeMap, typename Handler::message_type, Handler>::type
				operator()(Handler * h) {
					/// Transfer ownership of this handler to the handler manager
					base::_data._ownHandler(h);

					/// Insert this handler into the map
					typedef typename Handler::message_type Message;
					base::_data._handlers.insert(std::pair<util::TypeId, void *>(typeid(Message), h));

					/// Return next proxy for additional
					return this->createNextProxy<TypeMap, Message, Handler>();
				}

				~RegProxy() {
					if (base::_isLast) {
						/// We are the return value from the last functor call,
						/// and were unused (didn't get called) - so we have
						/// accumulated full type knowledge. Use it, before we
						/// lose it.
						base::instantiateImplementationAndSetHandler<TypeMap>();
					}
				}
			private:
				friend class RegProxyBase<RegistrationData>;
				friend class FlexibleReceiveHandlerManager;
				RegProxy(RegistrationData & data) : RegProxyBase<RegistrationData>(data) {}

		};

	} // end of namespace Registration

} // end of namespace FlexReceive

class FlexibleReceiveHandlerManager {
	public:


		template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
		struct RegData {
			typedef FlexReceive::Registration::RegData<transmission::Receiver<MessageCollection>, HandlerOwnerAdditionFunctor > type;
		};

		template<typename MessageCollection, typename HandlerOwnerAdditionFunctor>
		struct EmptyRegProxy {
			typedef typename RegData<MessageCollection, HandlerOwnerAdditionFunctor>::type RegDataType;
			typedef boost::mpl::map<> EmptyMPLMap;
			typedef FlexReceive::Registration::RegProxy< EmptyMPLMap , RegDataType> type;
		};

		typedef boost::function<void(FlexReceive::Types::GenericHandlerPtr)> GenericAddFunctorType;

		template<typename MessageCollection>
		typename EmptyRegProxy<MessageCollection, GenericAddFunctorType>::type
		registerHandlerSet(transmission::Receiver<MessageCollection> & recv) {
			using namespace boost;
			prepareToRegisterHandlerSet(recv);
			_handlerOwner.clear();
			return registerHandlerSetImpl<MessageCollection, GenericAddFunctorType>(recv, bind(&FlexReceive::Types::GenericHandlerPtrVec::push_back, ref(_handlerOwner), _1));
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
		typename EmptyRegProxy<MessageCollection, HandlerOwnerAdditionFunctor>::type
		registerHandlerSetImpl(transmission::Receiver<MessageCollection> & recv, HandlerOwnerAdditionFunctor const& ownerAddition) {
			/// Must call prepareToRegisterHandlerSet then delete all old handlers before calling this!
			/// Populate the registration data struct with references as needed.
			typedef typename RegData<MessageCollection, HandlerOwnerAdditionFunctor>::type RegDataType;
			RegDataType data(_handlerMap, _implPtr, ownerAddition, recv);

			/// Now, we are ready to get a new implementation.
			return typename EmptyRegProxy<MessageCollection, HandlerOwnerAdditionFunctor>::type(data);
		}
	private:
		FlexReceive::Types::TypeHandlerMap _handlerMap;
		FlexReceive::Types::FlexRecvBasePtr _implPtr;
		FlexReceive::Types::GenericHandlerPtrVec _handlerOwner;
};

class VRPNReceiveHandlerManager : public FlexibleReceiveHandlerManager {
	private:
		class MainloopContainerAddFunctor {
			public:
				MainloopContainerAddFunctor(vrpn_MainloopContainer & container) : _container(container) {}

				template<typename T>
				T operator()(T o) {
					return _container.add(o);
				}
			private:
				vrpn_MainloopContainer & _container;
		};
	public:

		template<typename MessageCollection>
		typename EmptyRegProxy<MessageCollection, MainloopContainerAddFunctor>::type
		registerHandlerSet(transmission::Receiver<MessageCollection> & recv) {
			prepareToRegisterHandlerSet(recv);
			_container.clear();
			return registerHandlerSetImpl(recv, MainloopContainerAddFunctor(_container));
		}

		void mainloop() {
			_container.mainloop();
		}
	private:
		vrpn_MainloopContainer _container;

};
#endif // INCLUDED_FlexibleReceiveHandlerManager_h_GUID_2fadebac_7348_4959_b0d0_6ce7a6daa185
