# 2009-2011 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryandnet>
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
add_subdirectory(vrpn)

set(VRPN_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn" "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/quat")
set(VRPN_LIBRARIES vrpnserver)
set(VRPN_FOUND TRUE)
add_definitions(-DVRPN_USING_CMAKE)


###
# Build the project
###

# The app is in the "src" subdirectory
add_subdirectory(src)

include(InstallRequiredSystemLibraries)

