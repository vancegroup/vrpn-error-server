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
	, _gotRobot(false)
	, _positionRobot(3)
	, _goalSensor(goal_sensor)
	, _gotGoal(false)
	, _positionGoal(3)
	, _worldx(worldx)
	, _worldz(worldz) {
	_tracker->register_change_handler(this, &ErrorComputer::_handleRobot, _robotSensor);
	_tracker->register_change_handler(this, &ErrorComputer::_handleGoal, _goalSensor);
	_lastOutput[0] = 0;
	_lastOutput[1] = 0;
}

ErrorComputer::~ErrorComputer() {
	_tracker->unregister_change_handler(this, &ErrorComputer::_handleRobot, _robotSensor);
	_tracker->unregister_change_handler(this, &ErrorComputer::_handleGoal, _goalSensor);
}

void VRPN_CALLBACK ErrorComputer::_handleRobot(void * userdata, const vrpn_TRACKERCB info) {
	ErrorComputer * self = static_cast<ErrorComputer*>(userdata);
	self->_gotRobot = true;
	self->_positionRobot = std::valarray<double>(info.pos, 3);
	//std::cout << "Robot: " << self->_positionRobot[0] << ", " << self->_positionRobot[1] << ", " << self->_positionRobot[2] << std::endl;
}

void VRPN_CALLBACK ErrorComputer::_handleGoal(void * userdata, const vrpn_TRACKERCB info) {
	ErrorComputer * self = static_cast<ErrorComputer*>(userdata);
	self->_gotGoal = true;
	self->_positionGoal = std::valarray<double>(info.pos, 3);
	//std::cout << "Goal: " << self->_positionGoal[0] << ", " << self->_positionGoal[1] << ", " << self->_positionGoal[2] << std::endl;
}

void ErrorComputer::operator()() {
	if (_gotGoal && _gotRobot) {
		std::valarray<double> error = _positionGoal - _positionRobot;

		ChannelArray desiredOutput = {error[_worldx], error[_worldz]};
		if (desiredOutput != _lastOutput) {
			//std::cout << "Requesting output " << desiredOutput[0] << ", " << desiredOutput[1] << std::endl;
			_output->request_change_channels(CHANNELS, desiredOutput.data());
			_lastOutput = desiredOutput;
			_gotGoal = false;
			_gotRobot = false;
		}
	}
}
