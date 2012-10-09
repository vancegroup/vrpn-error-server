/**
	@file
	@brief Implementation

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

// Internal Includes
#include "ErrorComputer.h"

// Library/third-party includes
#include <vrpn_Analog_Output.h>
#include <vrpn_Tracker.h>

// Standard includes
#include <iostream>

ErrorComputer::ErrorComputer(vrpn_Tracker_Remote * tracker, vrpn_Analog_Output_Remote * output, int robot_sensor, int goal_sensor, size_t worldx, size_t worldz)
	: _tracker(tracker)
	, _output(output)
	, _robotSensor(robot_sensor)
	, _robotHandler(&ErrorComputer::_handleRobot, this)
	, _gotRobot(false)
	, _positionRobot(3)
	, _goalSensor(goal_sensor)
	, _goalHandler(&ErrorComputer::_handleGoal, this)
	, _gotGoal(false)
	, _positionGoal(3)
	, _worldx(worldx)
	, _worldz(worldz) {
	vrpn_Callbacks::register_change_handler(_tracker, _robotHandler, _robotSensor);
	vrpn_Callbacks::register_change_handler(_tracker, _goalHandler, _goalSensor);
	_lastOutput[0] = 0;
	_lastOutput[1] = 0;
}

ErrorComputer::~ErrorComputer() {
	vrpn_Callbacks::unregister_change_handler(_tracker, _robotHandler, _robotSensor);
	vrpn_Callbacks::unregister_change_handler(_tracker, _goalHandler, _goalSensor);
}

void ErrorComputer::_handleRobot(const vrpn_TRACKERCB info) {
	_gotRobot = true;
	_positionRobot = std::valarray<double>(info.pos, 3);
	//std::cout << "Robot: " << _positionRobot[0] << ", " << _positionRobot[1] << ", " << _positionRobot[2] << std::endl;
}

void ErrorComputer::_handleGoal(const vrpn_TRACKERCB info) {
	_gotGoal = true;
	_positionGoal = std::valarray<double>(info.pos, 3);
	//std::cout << "Goal: " << _positionGoal[0] << ", " << _positionGoal[1] << ", " << _positionGoal[2] << std::endl;
}

void ErrorComputer::mainloop() {
	if (_gotGoal && _gotRobot) {
		std::valarray<double> error = _positionGoal - _positionRobot;

		ChannelArray desiredOutput = {{error[_worldx], error[_worldz]}};
		if (desiredOutput != _lastOutput) {
			//std::cout << "Requesting output " << desiredOutput[0] << ", " << desiredOutput[1] << std::endl;
			_output->request_change_channels(CHANNELS, desiredOutput.data());
			_lastOutput = desiredOutput;
			_gotGoal = false;
			_gotRobot = false;
		}
	}
}
