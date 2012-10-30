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
#ifndef INCLUDED_MainloopContainerAddFunctor_h_GUID_0de22fbb_0f8f_495d_b652_c3daa795c355
#define INCLUDED_MainloopContainerAddFunctor_h_GUID_0de22fbb_0f8f_495d_b652_c3daa795c355

// Internal Includes
// - none

// Library/third-party includes
#include <vrpn_MainloopContainer.h>

// Standard includes
// - none

namespace FlexReceive {
	namespace detail {
		class MainloopContainerAddFunctor {
			public:
				MainloopContainerAddFunctor(vrpn_MainloopContainer & container) : _container(&container) {}

				template<typename T>
				T operator()(T o) {
					return _container->add(o);
				}
			private:
				vrpn_MainloopContainer * _container;
		};

	} // end of namespace detail
} // end of namespace FlexReceive


#endif // INCLUDED_MainloopContainerAddFunctor_h_GUID_0de22fbb_0f8f_495d_b652_c3daa795c355
