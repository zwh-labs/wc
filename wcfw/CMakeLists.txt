cmake_minimum_required( VERSION 2.6 )
project( wcfw )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

set( wcfw_objdir "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/objdir" )

execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory "${wcfw_objdir}" )

add_custom_target( wcfw ALL
	COMMAND make OBJDIR="${wcfw_objdir}"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Building firmware"
)
