cmake_minimum_required( VERSION 2.6 )
project( wc_sharp_examples )

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

execute_process( COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_PATH}" )

add_custom_target( wc_sharp_examples ALL
	COMMAND xbuild examples.csproj /p:BaseOutputPath="${EXECUTABLE_OUTPUT_PATH}"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Building C#-Examples project"
)
