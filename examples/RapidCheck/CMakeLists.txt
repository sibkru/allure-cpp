# RapidCheck Allure example
add_executable(RapidCheckAllureExample
	PropertyTests.cpp
	main.cpp
)

target_link_libraries(RapidCheckAllureExample
	AllureCpp
	gtest
	gtest_main
	rapidcheck
)

set_target_properties(RapidCheckAllureExample PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(TARGET examples)
	add_dependencies(examples RapidCheckAllureExample)
endif()
