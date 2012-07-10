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
#ifndef INCLUDED_vrpn_Connection_Callbacks_h_GUID_7905e7d4_0941_4a26_a3bc_db47c2056e19
#define INCLUDED_vrpn_Connection_Callbacks_h_GUID_7905e7d4_0941_4a26_a3bc_db47c2056e19

// Internal Includes
#include "vrpn_Callback_Wrappers_Base.h"

// Library/third-party includes
#include <vrpn_Connection.h>

// Standard includes
// - none

typedef vrpn_Boost_Callback_Wrapper<vrpn_MESSAGEHANDLER, vrpn_HANDLERPARAM, int> vrpn_Analog_Output_Change_Handler;


#endif // INCLUDED_vrpn_Connection_Callbacks_h_GUID_7905e7d4_0941_4a26_a3bc_db47c2056e19
