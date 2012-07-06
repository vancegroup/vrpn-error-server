
include_directories(${Boost_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS} ${TCLAP_INCLUDE_DIRS} "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCES
	ErrorCommandOutput.cpp
	ErrorCommandOutput.h
	ErrorComputer.cpp
	ErrorComputer.h
	main.cpp)

add_executable(error_server ${SOURCES})
target_link_libraries(error_server ${VRPN_LIBRARIES})