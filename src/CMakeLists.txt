
include_directories(${Boost_INCLUDE_DIR}
	${VRPN_INCLUDE_DIRS}
	${TCLAP_INCLUDE_DIRS}
	${TUPLE_TRANSMISSION_INCLUDE_DIRS}
	${LOKILIB_INCLUDE_DIRS}
	${PROTOCOL_INCLUDE_DIRS}
	"${CMAKE_CURRENT_SOURCE_DIR}/..")

set(LIB_SOURCES
	vrpn_Callbacks/vrpn_Analog_Output_Callbacks.h
	vrpn_Callbacks/vrpn_Callback_Wrappers_Base.h
	vrpn_Callbacks/vrpn_Connection_Callbacks.h
	vrpn_Callbacks/vrpn_Tracker_Callbacks.h

	FlexReceive/HandlerManager.h
	FlexReceive/Impl.h
	FlexReceive/MainloopContainerAddFunctor.h
	FlexReceive/MessageHandlerBase.cpp
	FlexReceive/MessageHandlerBase.h
	FlexReceive/Metafunctions.h
	FlexReceive/PushToPointerVecFunctor.h
	FlexReceive/Registration.h
	FlexReceive/Types.h
	FlexReceive/VRPNHandlerManager.h

	AppObject.cpp
	AppObject.h
	BinaryAnalogMessage.h
	BinaryCommandOutput.h
	CleanExit.cpp
	CleanExit.h
	CommandOutput.h
	ErrorComputer.cpp
	ErrorComputer.h
	ReceiveEchoer.h
	RobotDataReceiver.cpp
	RobotDataReceiver.h
	vrpn_SemaphoreGuard.h)

add_library(server-common STATIC ${LIB_SOURCES})
target_link_libraries(server-common ${VRPN_LIBRARIES})

add_executable(error_server main.cpp)
target_link_libraries(error_server server-common)

install(TARGETS error_server
	RUNTIME DESTINATION bin)

if(MSVC)
	message(STATUS "Too much metaprogramming for Visual C++ to handle in two-way-adjustable, so skipping it!")
else()
	add_executable(two_way_adjustable_server two-way-adjustable.cpp)
	target_link_libraries(two_way_adjustable_server server-common)
	install(TARGETS two_way_adjustable_server
		RUNTIME DESTINATION bin)
endif()

find_package(Qt4)
if(QT_FOUND)
	include(${QT_USE_FILE})
	include_directories(${CMAKE_CURRENT_BINARY_DIR})
	qt4_wrap_ui(GENERATED Configuration.ui)
	add_executable(robot_server RobotServerGui.cpp RobotServerGui.h ${GENERATED})
	target_link_libraries(robot_server server-common ${QT_LIBRARIES})
	install(TARGETS robot_server
		RUNTIME DESTINATION bin)
endif()
