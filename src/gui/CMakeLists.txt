find_package(Qt4)
if(NOT QT_FOUND)
    return()
endif()
include(${QT_USE_FILE})


set(SOURCES
    Configuration.cpp
    RobotServerGui.cpp
    RobotServerGui.h
    RobotSettings.cpp
    RobotSettings.h)

qt4_wrap_ui(GENERATED Configuration.ui)

set(MOC
    Configuration.h)
qt4_wrap_cpp(GENERATED ${MOC})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(robot_server ${SOURCES} ${MOC} ${GENERATED})
target_link_libraries(robot_server server-common ${QT_LIBRARIES})
install(TARGETS robot_server
	RUNTIME DESTINATION bin)
