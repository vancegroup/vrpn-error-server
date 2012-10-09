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
#ifndef INCLUDED_BinaryMessageHandler_h_GUID_9cdca7f2_f9c9_4579_a97d_7ed75354198e
#define INCLUDED_BinaryMessageHandler_h_GUID_9cdca7f2_f9c9_4579_a97d_7ed75354198e

// Internal Includes
// - none

// Library/third-party includes
#include <loki/AssocVector.h>
#include <util/TypeId.h>
#include <tuple-transmission/Receiver.h>
#include <boost/scoped_ptr.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/map.hpp>

// Standard includes
// - none

template<typename MessageHandlerMap, typename MessageCollection>
class BinaryMessageHandlerImpl;

class BinaryMessageHandler;

namespace BinaryMessageHandlerInit {
	namespace mpl = boost::mpl;

	struct InitialData;

	template<typename TypeMap>
	class AdditionalProxy;

	class InitialProxyBase {

		protected:
			template<typename TypeMap, typename Message, typename Handler>
			struct ReturnType {
				typedef AdditionalProxy<typename boost::mpl::insert<TypeMap, mpl::pair<Message, Handler> >::type> type;
			};

			template<typename TypeMap, typename Message, typename Handler>
			typename ReturnType<TypeMap, Message, Handler>::type
			createNextProxy(InitialData & init) {
				return typename ReturnType<TypeMap, Message, Handler>::type(&init);
			}

	};

	template<typename Derived, typename TypeMap>
	class ProxyInterface : public InitialProxyBase {
		public:

			template<typename Handler>
			typename ReturnType<TypeMap, typename Handler::message_type, Handler>::type
			operator()(Handler * h) {
				typedef typename Handler::message_type Message;
				getDerivedInit().messageHandlers.insert(std::pair<util::TypeId, void *>(typeid(Message), h));
				return createNextProxy<TypeMap, Message, Handler>(getDerivedInit());
			}

			template<typename MessageCollection>
			BinaryMessageHandlerImpl<TypeMap, MessageCollection> * handleCollection() {
				return new BinaryMessageHandlerImpl<TypeMap, MessageCollection>(getDerivedInit().messageHandlers);
			}

		protected:
			InitialData & getInit() {
				return *init;
			}

			InitialData & getDerivedInit() {
				return (static_cast<Derived*>(this))->getInit();
			}

			ProxyInterface(InitialData * initial) : init(initial) {}
			InitialData * init;

	};
	template<typename TypeMap>
	class AdditionalProxy : public ProxyInterface<AdditionalProxy<TypeMap>, TypeMap> {
		public:

		private:
			AdditionalProxy(InitialData & initial) : ProxyInterface<AdditionalProxy<TypeMap>, TypeMap>(initial) {}
			friend class InitialProxyBase;
	};

	struct InitialData {
		typedef Loki::AssocVector<util::TypeId, void *> TypeMap;
		TypeMap messageHandlers;
	};

	class FirstProxy : public ProxyInterface<FirstProxy, mpl::map<> > {
		public:

			InitialData & getInit() {
				return init;
			}
		private:
			FirstProxy() : ProxyInterface<FirstProxy, mpl::map<> >(NULL) {}
			InitialData init;
			friend class BinaryMessageHandler;
	};
} // end of namespace BinaryMessageHandlerInit

class BinaryMessageHandler {
	public:
		typedef Loki::AssocVector<util::TypeId, void *> TypeMap;
		virtual ~BinaryMessageHandler() {}

	protected:

		BinaryMessageHandler(TypeMap const& handlers) : _handlers(handlers) {}

	public:
		static BinaryMessageHandlerInit::FirstProxy createNew() {
			return BinaryMessageHandlerInit::FirstProxy();
		}
	protected:
		TypeMap _handlers;
};

template<typename MessageHandlerMap, typename MessageCollection>
class BinaryMessageHandlerImpl : public BinaryMessageHandler  {
	public:
		BinaryMessageHandlerImpl(TypeMap const& handlers) : BinaryMessageHandler(handlers), _recv(*this) {}

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
		                Sequence const& s,
		                typename M::message_type_tag * = NULL,
		                typename boost::disable_if<boost::mpl::has_key<MessageHandlerMap, M> >::type * = NULL) {
			/* unhandled message */
		}

		transmission::Receiver<MessageCollection> & getReceiver() {
			return _recv;
		}
	private:

		transmission::Receiver<MessageCollection> _recv;
};

#endif // INCLUDED_BinaryMessageHandler_h_GUID_9cdca7f2_f9c9_4579_a97d_7ed75354198e
