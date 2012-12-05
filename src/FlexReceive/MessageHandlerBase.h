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
#ifndef INCLUDED_MessageHandlerBase_h_GUID_8f9d13ea_2bc9_43dd_9095_87fd793decdd
#define INCLUDED_MessageHandlerBase_h_GUID_8f9d13ea_2bc9_43dd_9095_87fd793decdd

// Internal Includes
#include "Types.h"

// Library/third-party includes
// - none

// Standard includes
// - none

namespace FlexReceive {
	class MessageHandlerBase {
		public:
			virtual ~MessageHandlerBase();
	};

} // end of namespace FlexReceive

#endif // INCLUDED_MessageHandlerBase_h_GUID_8f9d13ea_2bc9_43dd_9095_87fd793decdd
