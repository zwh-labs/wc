cmake_minimum_required( VERSION 2.6 )
project( wc )

add_subdirectory( wclib )

set( BUILD_FIRMWARE FALSE CACHE BOOL "Build firmware" )
if( BUILD_FIRMWARE )
	add_subdirectory( wcfw )
else()
	message( STATUS "NOT building firmware" )
endif()

set( BUILD_WCSHARP FALSE CACHE BOOL "Build C# bindings" )
if( BUILD_WCSHARP )
	add_subdirectory( wc-sharp )
else()
	message( STATUS "NOT building C# bindings" )
endif()

find_package( Doxygen )
if( DOXYGEN_FOUND )
	if( NOT TARGET doc )
		add_custom_target( doc )
	endif()
else( DOXYGEN_FOUND )
	message( STATUS "Doxygen not found - Documentation can't be generated." )
endif( DOXYGEN_FOUND )
