cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wc )

include( GenerateExportHeader )

find_package( Threads )

set( wc_sources
	src/Packet.c
	src/Connection.c
	src/Connection_Configuration.c
	src/Thread.c
	src/Thread_Mutex.c
	src/Thread_Thread.c
	src/Configuration.c
	src/Configuration_stdio.c
	src/Configuration_ArgumentParser.c
	src/WheelMovement_Configuration.c
)

include_directories( include )

add_compiler_export_flags()

add_library( wc SHARED ${wc_sources} )
set_target_properties( wc PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set_target_properties( wc PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
set_target_properties( wc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )
target_link_libraries( wc ${CMAKE_THREAD_LIBS_INIT} )

generate_export_header( wc )
if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden" )
endif()
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

if( CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )
endif()

set( BUILD_EXAMPLES TRUE CACHE BOOL "Compile examples" )
if( BUILD_EXAMPLES )
	add_subdirectory( examples )
else( BUILD_EXAMPLES )
	message( STATUS "NOT building examples" )
endif( BUILD_EXAMPLES )
