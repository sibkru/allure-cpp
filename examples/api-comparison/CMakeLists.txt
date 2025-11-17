# API Comparison Example
#
# This example demonstrates that both the old and new APIs produce identical
# Allure reports. Two executables are built using the same test file but
# different main functions.

# Old API example - uses buildListener()
add_executable(ApiComparisonOldAPI
	SampleTests.cpp
	main_old_api.cpp
)

target_link_libraries(ApiComparisonOldAPI
	GTestAllureUtilities
	gtest
	gtest_main
)

set_target_properties(ApiComparisonOldAPI PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# New API example - uses initializeGoogleTest()
add_executable(ApiComparisonNewAPI
	SampleTests.cpp
	main_new_api.cpp
)

target_link_libraries(ApiComparisonNewAPI
	GTestAllureUtilities
	gtest
	gtest_main
)

set_target_properties(ApiComparisonNewAPI PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# Allure Features Demo - comprehensive demonstration of all Allure 2 features
add_executable(AllureFeaturesDemo
	AllureFeaturesDemo.cpp
)

target_link_libraries(AllureFeaturesDemo
	GTestAllureUtilities
	gtest
	gtest_main
)

set_target_properties(AllureFeaturesDemo PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
)

# Add to examples target if it exists
if(TARGET examples)
	add_dependencies(examples ApiComparisonOldAPI ApiComparisonNewAPI AllureFeaturesDemo)
endif()
