cmake_minimum_required( VERSION 2.6 )
project( wc )

add_subdirectory( wclib )

set( BUILD_FIRMWARE FALSE CACHE BOOL "Build firmware" )
if( BUILD_FIRMWARE )
	add_subdirectory( wcfw )
else()
	message( STATUS "NOT building firmware" )
endif()

set( BUILD_WCSHARP_EXAMPLES FALSE CACHE BOOL "Build C# examples" )
if( BUILD_WCSHARP_EXAMPLES )
	add_subdirectory( wc-sharp/examples )
else()
	message( STATUS "NOT building C# examples" )
endif()
