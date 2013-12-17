cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wctest )

#set( wctest_sources
#	main.c
#)

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

add_executable( wctestSimple testSimple.c )
add_executable( wctestThread testThread.c )
set_target_properties( wctestSimple PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin )
set_target_properties( wctestThread PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin )
target_link_libraries( wctestSimple ${wctest_libraries} )
target_link_libraries( wctestThread ${wctest_libraries} )
