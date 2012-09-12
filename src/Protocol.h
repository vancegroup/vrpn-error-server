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
#ifndef INCLUDED_Protocol_h_GUID_ae62d52d_aaac_4675_9296_d18b79f719fc
#define INCLUDED_Protocol_h_GUID_ae62d52d_aaac_4675_9296_d18b79f719fc

// Internal Includes
// - none

// Library/third-party includes
#include <tuple-transmission/MessageCollection.h>
#include <tuple-transmission/MessageType.h>
#include <tuple-transmission/envelopes/BasicChecksum.h>
#include <boost/mpl/vector.hpp>

// Standard includes
// - none

namespace Protocol {
	struct XYFloatError : transmission::MessageTypeBase< boost::mpl::vector<float, float> > {};
	struct XYRotationFloatError : transmission::MessageTypeBase< boost::mpl::vector<float, float, float> > {};

	typedef transmission::MessageCollection <
	boost::mpl::vector
	< XYFloatError
	, XYRotationFloatError > ,
	transmission::envelopes::BasicChecksum > ComputerToRobot;
} // end of namespace Protocol

#endif // INCLUDED_Protocol_h_GUID_ae62d52d_aaac_4675_9296_d18b79f719fc
