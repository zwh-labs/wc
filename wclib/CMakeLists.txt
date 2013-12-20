cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wc )

find_package( Threads )

add_subdirectory( examples )

set( wc_sources
	src/WCPacket.c
	src/WCConnection.c
	src/WCThread.c
	src/WCThread_Mutex.c
	src/WCThread_Thread.c
	src/WCConfiguration.c
	src/WCConfiguration_ArgumentParser.c
	src/WCWheelMovement_Configuration.c
)

set( wc_include_dirs
	./include/
)

set( wc_library_dirs
)

set( wc_libraries
	${CMAKE_THREAD_LIBS_INIT}
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic" )
link_directories( ${wc_library_dirs} )
include_directories( ${wc_include_dirs} )

add_library( wc SHARED ${wc_sources} )
set_target_properties( wc PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set_target_properties( wc PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set_target_properties( wc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
target_link_libraries( wc ${wc_libraries} )
