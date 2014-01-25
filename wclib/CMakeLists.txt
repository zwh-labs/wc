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
	src/WheelPIDController.c
)

include_directories( include )

add_compiler_export_flags()

add_library( wc SHARED ${wc_sources} )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
set( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )

if( UNIX )
	set( MATH_LIB m )
else()
	set( MATH_LIB )
endif()
target_link_libraries( wc ${CMAKE_THREAD_LIBS_INIT} ${MATH_LIB} )

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

set( BUILD_EXAMPLES TRUE CACHE BOOL "Build native C examples" )
if( BUILD_EXAMPLES )
	add_subdirectory( examples )
else()
	message( STATUS "NOT building native C examples" )
endif()

set( BUILD_BINDINGS_CS_EXAMPLES FALSE CACHE BOOL "Build C# examples" )
if( BUILD_BINDINGS_CS_EXAMPLES )
	add_subdirectory( bindings/cs/examples )
else()
	message( STATUS "NOT building C# examples" )
endif()
