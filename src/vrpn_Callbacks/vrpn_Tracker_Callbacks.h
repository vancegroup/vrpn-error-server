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
#ifndef INCLUDED_vrpn_Tracker_Callbacks_h_GUID_87ed72a9_64a6_4909_9a83_089f507b75aa
#define INCLUDED_vrpn_Tracker_Callbacks_h_GUID_87ed72a9_64a6_4909_9a83_089f507b75aa

// Internal Includes
#include "vrpn_Callback_Wrappers_Base.h"

// Library/third-party includes
#include <vrpn_Tracker.h>

// Standard includes
// - none

typedef vrpn_Boost_Callback_Wrapper<vrpn_TRACKERCHANGEHANDLER, const vrpn_TRACKERCB> vrpn_Tracker_Change_Handler;
typedef vrpn_Boost_Callback_Wrapper<vrpn_TRACKERVELCHANGEHANDLER, const vrpn_TRACKERVELCB> vrpn_Tracker_Velocity_Change_Handler;
typedef vrpn_Boost_Callback_Wrapper<vrpn_TRACKERACCCHANGEHANDLER, const vrpn_TRACKERACCCB> vrpn_Tracker_Acceleration_Change_Handler;


#endif // INCLUDED_vrpn_Tracker_Callbacks_h_GUID_87ed72a9_64a6_4909_9a83_089f507b75aa
