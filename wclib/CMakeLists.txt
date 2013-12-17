cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wc )

add_subdirectory( test )

set( wc_sources
	src/WCPacket.c
	src/WCConnection.c
)

set( wc_include_dirs
	./include/
)

set( wc_library_dirs
)

set( wc_libraries
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic" )
link_directories( ${wc_library_dirs} )
include_directories( ${wc_include_dirs} )

add_library( wc SHARED ${wc_sources} )
set_target_properties( wc PROPERTIES LIBRARY_OUTPUT_DIRECTORY lib )
target_link_libraries( wc ${wc_libraries} )
