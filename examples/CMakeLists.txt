# Sample Tests Example
#
# This example demonstrates the usage of GTestAllureUtilities with basic test cases.

add_executable(SampleTests
	SampleTests.cpp
	main.cpp
)

target_link_libraries(SampleTests
	GTestAllureUtilities
	gtest
	gtest_main
)

set_target_properties(SampleTests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# Add to examples target if it exists
if(TARGET examples)
	add_dependencies(examples SampleTests)
endif()
