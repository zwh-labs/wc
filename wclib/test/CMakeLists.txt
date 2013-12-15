cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wctest )

set( wctest_sources
	main.c
)

set( wctest_include_dirs
	../include/
)

set( wctest_library_dirs
	../lib/
)

set( wctest_libraries
	wc
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic" )
link_directories( ${wctest_library_dirs} )
include_directories( ${wctest_include_dirs} )

add_executable( wctest ${wctest_sources} )
set_target_properties( wctest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin )
target_link_libraries( wctest ${wctest_libraries} )
