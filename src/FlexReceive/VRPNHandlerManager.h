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
#ifndef INCLUDED_VRPNHandlerManager_h_GUID_dfd1dce1_594b_443b_9135_aeff1312a424
#define INCLUDED_VRPNHandlerManager_h_GUID_dfd1dce1_594b_443b_9135_aeff1312a424

// Internal Includes
#include "HandlerManager.h"
#include "Metafunctions.h"
#include "MainloopContainerAddFunctor.h"

// Library/third-party includes
#include <vrpn_MainloopContainer.h>

// Standard includes
// - none

namespace FlexReceive {
	class VRPNHandlerManager : public HandlerManager {
		public:

			template<typename MessageCollection>
			typename detail::ComputeInitialRegProxy<MessageCollection, detail::MainloopContainerAddFunctor>::type
			registerHandlerSet(transmission::Receiver<MessageCollection> & recv) {
				prepareToRegisterHandlerSet(recv);
				_container.clear();
				return registerHandlerSetImpl(recv, detail::MainloopContainerAddFunctor(_container));
			}

			void mainloop() {
				_container.mainloop();
			}
		private:
			vrpn_MainloopContainer _container;

	};

} // end of namespace FlexReceive

#endif // INCLUDED_VRPNHandlerManager_h_GUID_dfd1dce1_594b_443b_9135_aeff1312a424
