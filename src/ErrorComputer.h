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
#ifndef INCLUDED_ErrorComputer_h_GUID_fa857175_0ca1_4b88_820a_f7d0b1233c46
#define INCLUDED_ErrorComputer_h_GUID_fa857175_0ca1_4b88_820a_f7d0b1233c46

// Internal Includes
// - none

// Library/third-party includes
#include <boost/noncopyable.hpp>
#include <boost/array.hpp>
#include <vrpn_Tracker.h>

// Standard includes
#include <valarray>

class vrpn_Analog_Output_Remote;

class ErrorComputer : boost::noncopyable {
	public:
		enum {
			CHANNELS = 2
		};
		typedef boost::array<vrpn_float64, CHANNELS> ChannelArray;
		ErrorComputer(vrpn_Tracker_Remote * tracker, vrpn_Analog_Output_Remote * output, int robot_sensor = 0, int goal_sensor = 1, size_t worldx = 0, size_t worldz = 1);
		~ErrorComputer();

		/// Compute and send updated error, if applicable.
		void operator()();
	private:
		static void VRPN_CALLBACK _handleRobot(void * userdata, const vrpn_TRACKERCB info);
		static void VRPN_CALLBACK _handleGoal(void * userdata, const vrpn_TRACKERCB info);

		vrpn_Tracker_Remote * _tracker;
		vrpn_Analog_Output_Remote * _output;

		int _robotSensor;
		bool _gotRobot;
		std::valarray<double> _positionRobot;

		int _goalSensor;
		bool _gotGoal;
		std::valarray<double> _positionGoal;

		size_t _worldx;
		size_t _worldz;

		ChannelArray _lastOutput;
};

#endif // INCLUDED_ErrorComputer_h_GUID_fa857175_0ca1_4b88_820a_f7d0b1233c46
