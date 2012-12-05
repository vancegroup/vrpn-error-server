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
#ifndef INCLUDED_Impl_h_GUID_a4e77a74_d68a_41c3_a04e_de0710c4303b
#define INCLUDED_Impl_h_GUID_a4e77a74_d68a_41c3_a04e_de0710c4303b

// Internal Includes
#include "Types.h"
#include "Metafunctions.h"

// Library/third-party includes
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/at.hpp>

// Standard includes
// - none


namespace FlexReceive {
	namespace Impl {
		/// @brief Base class for ReceiveHandlerImpl allowing for shared_ptr
		/// ownership of derived classes without requiring full type knowledge.
		class ReceiveHandlerBase {
			public:
				virtual ~ReceiveHandlerBase() {}
		};

		/// @brief Templated receive handler, registered with the Receiver,
		/// that uses its full type knowledge to extract and make use of
		/// the individual message handlers stored generically in the
		/// HandlerManager.
		template<typename MessageHandlerMap>
		class ReceiveHandlerImpl : public ReceiveHandlerBase {
			public:
				virtual ~ReceiveHandlerImpl() {}

				/// @brief Function call operator for messages we have a handler for.
				///
				/// Using typeid on the message type, the handler is requested
				/// from the HandlerManager. It is then extracted from its
				/// generic container, and invoked on the received message.
				template<typename M, typename Sequence>
				void operator()(M const&,
				                Sequence const& s,
				                typename M::message_type_tag * = NULL,
				                typename boost::enable_if<boost::mpl::has_key<MessageHandlerMap, M> >::type * = NULL) {
					using boost::any_cast;
					typedef typename boost::mpl::at<MessageHandlerMap, M>::type HandlerType;
					HandlerType * h = any_cast<HandlerType *>(_handlers[util::TypeId(typeid(M))]);
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
				friend class ImplementationFactory;
				ReceiveHandlerImpl(Types::TypeHandlerMap & handlers) : _handlers(handlers) {}
				Types::TypeHandlerMap & _handlers;
		};

		class ImplementationFactory {
			public:
				template<typename MessageHandlerMap>
				static inline typename detail::ImplPtr<MessageHandlerMap>::type create(Types::TypeHandlerMap & handlers) {
					typename detail::ImplPtr<MessageHandlerMap>::type p(new ReceiveHandlerImpl<MessageHandlerMap>(handlers));
					return p;
				}
		};
	} // end of namespace Impl
} // end of namespace FlexReceive

#endif // INCLUDED_Impl_h_GUID_a4e77a74_d68a_41c3_a04e_de0710c4303b
