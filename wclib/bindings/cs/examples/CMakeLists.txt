cmake_minimum_required( VERSION 2.6 )
cmake_policy( SET CMP0015 NEW )
project( wcCSExamples )

add_custom_target( wcCSExamples ALL
	COMMAND xbuild examples.csproj /p:BaseOutputPath="${EXECUTABLE_OUTPUT_PATH}"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	DEPENDS wc
	COMMENT "Building C#-Examples project"
)
