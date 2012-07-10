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
#ifndef INCLUDED_vrpn_Analog_Output_Callbacks_h_GUID_a4cf5466_8393_4176_9ea7_18f8f7256328
#define INCLUDED_vrpn_Analog_Output_Callbacks_h_GUID_a4cf5466_8393_4176_9ea7_18f8f7256328

// Internal Includes
#include "vrpn_Callback_Wrappers_Base.h"

// Library/third-party includes
#include <vrpn_Analog_Output.h>

// Standard includes
// - none

typedef vrpn_Boost_Callback_Wrapper<vrpn_ANALOGOUTPUTCHANGEHANDLER, const vrpn_ANALOGOUTPUTCB> vrpn_Analog_Output_Change_Handler;


#endif // INCLUDED_vrpn_Analog_Output_Callbacks_h_GUID_a4cf5466_8393_4176_9ea7_18f8f7256328
