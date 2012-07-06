
set(SOURCES
	ErrorCommandOutput.cpp
	ErrorCommandOutput.h)

add_library(error_server ${SOURCES})
target_link_libraries(error_server ${VRPN_LIBRARIES})
