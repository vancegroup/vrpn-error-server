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
#include <tuple-transmission/envelopes/Basic.h>
#include <boost/mpl/vector.hpp>
#include <boost/cstdint.hpp>

// Standard includes
// - none

namespace Protocol {
	using namespace transmission;
	namespace mpl = boost::mpl;
	using boost::int16_t;
	struct XYFloatError : MessageTypeBase< mpl::vector<float, float> > {};
	struct XYRotationFloatError : MessageTypeBase< mpl::vector<float, float, float> > {};

	struct XYIntError : MessageTypeBase< mpl::vector<int16_t, int16_t> > {};
	struct XYIntFloatError : MessageTypeBase< mpl::vector<int16_t, int16_t, int16_t> > {};

	struct XYIntVelocities : MessageTypeBase< mpl::vector<int16_t, int16_t> > {};
	struct XYRotationIntVelocities : MessageTypeBase< mpl::vector<int16_t, int16_t, int16_t> > {};

	struct CurrentWheelSetpoints : MessageTypeBase< mpl::vector<int16_t, int16_t, int16_t, int16_t> > {};
	struct CurrentWheelVelocities : MessageTypeBase< mpl::vector<int16_t, int16_t, int16_t, int16_t> > {};
	struct CurrentPWMOutput : MessageTypeBase< mpl::vector<int16_t, int16_t, int16_t, int16_t> > {};

	struct StartControl : MessageTypeBase< mpl::vector<> > {};
	struct EndControl : MessageTypeBase< mpl::vector<> > {};

	typedef MessageCollection <
	mpl::vector
	< XYFloatError
	, XYRotationFloatError
	, XYIntError
	, XYIntFloatError
	, XYIntVelocities
	, XYRotationIntVelocities
	, StartControl
	, EndControl
	> ,
	envelopes::BasicChecksum > ComputerToRobot;

	typedef MessageCollection<mpl::vector<XYRotationIntVelocities, CurrentWheelSetpoints>, envelopes::Basic> BlueToGreen;
	typedef MessageCollection<mpl::vector<CurrentWheelSetpoints, CurrentWheelVelocities>, envelopes::Basic> GreenToBlue;
	typedef MessageCollection<mpl::vector<CurrentWheelSetpoints, CurrentWheelVelocities, CurrentPWMOutput>, envelopes::BasicChecksum> RobotToComputer;
} // end of namespace Protocol

#endif // INCLUDED_Protocol_h_GUID_ae62d52d_aaac_4675_9296_d18b79f719fc
