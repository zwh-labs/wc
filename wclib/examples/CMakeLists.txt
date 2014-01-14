cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wcExamples )

set( wcExamples_include_dirs
	../include/
)

set( wcExamples_library_dirs
	../lib/
)

set( wcExamples_libraries
	wc
)

if( NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic" )
	set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic" )
endif()

link_directories( ${wcExamples_library_dirs} )
include_directories( ${wcExamples_include_dirs} )

add_executable( wcExampleRaw raw.c )
add_executable( wcExampleThread thread.c )
set_target_properties( wcExampleRaw PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin )
set_target_properties( wcExampleThread PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin )
target_link_libraries( wcExampleRaw ${wcExamples_libraries} )
target_link_libraries( wcExampleThread ${wcExamples_libraries} )

set( BUILD_SDL2_EXAMPLES FALSE CACHE BOOL "Compile SDL2 examples" )
if( BUILD_SDL2_EXAMPLES )
	find_package( OpenGL REQUIRED )
	add_executable( wcExampleSDL2OpenGL sdl_opengl.c )
	set_target_properties( wcExampleSDL2OpenGL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin )
	target_link_libraries( wcExampleSDL2OpenGL ${wcExamples_libraries} SDL2 ${OPENGL_LIBRARIES} )
else( BUILD_SDL2_EXAMPLES )
	message( STATUS "NOT building SDL2 examples" )
endif( BUILD_SDL2_EXAMPLES )
