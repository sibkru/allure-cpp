# GoogleTest Allure example
add_executable(GoogleTestAllureExample
	SampleTests.cpp
	CalculatorTests.cpp
	../shared/Calculator.cpp
	main.cpp
)

target_link_libraries(GoogleTestAllureExample
	AllureCpp
	gtest
	gtest_main
)

set_target_properties(GoogleTestAllureExample PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if(TARGET examples)
	add_dependencies(examples GoogleTestAllureExample)
endif()
