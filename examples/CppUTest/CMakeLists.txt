cmake_minimum_required(VERSION 3.14)

project(CppUTestAllureExample)

# Create executable
add_executable(${PROJECT_NAME}
    SampleTests.cpp
    CalculatorTests.cpp
    ../shared/Calculator.cpp
    main.cpp
)

# Link against AllureCpp library and CppUTest
target_link_libraries(${PROJECT_NAME}
    AllureCpp
    CppUTest
    CppUTestExt
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
