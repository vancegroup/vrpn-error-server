# Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com/
# Iowa State University HCI Graduate Program/VRAC

cmake_minimum_required(VERSION 2.6)

# Set package properties
project(vrpn-error-server)

###
# Options
###



###
# Perform build configuration of dependencies
###


# Locally-developed modules dist'ed with this app - always have this first.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(UseBackportedModules)


# Build VRPN as subproject
set(VRPN_BUILD_SERVER_LIBRARY TRUE CACHE INTERNAL "" FORCE)
set(VRPN_USE_HID TRUE CACHE INTERNAL "" FORCE)
set(VRPN_USE_LOCAL_HIDAPI TRUE CACHE INTERNAL "" FORCE)
add_subdirectory(vrpn)

set(VRPN_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/quat")
set(VRPN_LIBRARIES vrpnserver)
set(VRPN_FOUND TRUE)
add_definitions(-DVRPN_USING_CMAKE)

find_package(Boost REQUIRED)

set(TCLAP_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/tclap-1.2.1/include")

set(TUPLE_TRANSMISSION_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/tuple-transmission")

set(LOKILIB_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/loki-lib/include")

set(PROTOCOL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/NexusRobotProtocol")

###
# Build the project
###

# The app is in the "src" subdirectory
add_subdirectory(src)

include(InstallRequiredSystemLibraries)

