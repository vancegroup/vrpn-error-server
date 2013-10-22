find_package(Qt4)
if(NOT QT_FOUND)
    return()
endif()
include(${QT_USE_FILE})



qt4_wrap_ui(GENERATED Configuration.ui)

set(SOURCES RobotServerGui.cpp RobotServerGui.h)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(robot_server ${SOURCES} ${GENERATED})
target_link_libraries(robot_server server-common ${QT_LIBRARIES})
install(TARGETS robot_server
	RUNTIME DESTINATION bin)
