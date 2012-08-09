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

//           Copyright Iowa State University 2012.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#pragma once
#ifndef INCLUDED_CleanExit_h_GUID_4C46A0BB_038B_4CA9_6B26_57E3C1AC22F8
#define INCLUDED_CleanExit_h_GUID_4C46A0BB_038B_4CA9_6B26_57E3C1AC22F8


// Internal Includes
// - none

// Library/third-party includes
#include <boost/noncopyable.hpp>
#include <vrpn_Shared.h>

// Standard includes
// - none

class CleanExit : boost::noncopyable {
	public:
		static CleanExit & instance();
		void registerHandlers();
		~CleanExit();

		bool exitRequested();
	private:
		CleanExit();
		volatile bool _done;
		bool _registered;
		vrpn_Semaphore _sem;
		friend void cleanexit_setExitFlag();
};

#endif // INCLUDED_CleanExit_h_GUID_4C46A0BB_038B_4CA9_6B26_57E3C1AC22F8

