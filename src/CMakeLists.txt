cmake_minimum_required(VERSION 3.14)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure GTestAllureUtilities static library
set(GTEST_ALLURE_UTILITIES GTestAllureUtilities)
file(GLOB_RECURSE GTEST_ALLURE_UTILITIES_SRC "*.cpp")
file(GLOB_RECURSE GTEST_ALLURE_UTILITIES_HDR "*.h")

add_library(${GTEST_ALLURE_UTILITIES} STATIC ${GTEST_ALLURE_UTILITIES_SRC} ${GTEST_ALLURE_UTILITIES_HDR})
target_include_directories(${GTEST_ALLURE_UTILITIES} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)
target_link_libraries(${GTEST_ALLURE_UTILITIES} PUBLIC gtest nlohmann_json::nlohmann_json)

#Configure source groups
foreach(FILE ${GTEST_ALLURE_UTILITIES_SRC} ${GTEST_ALLURE_UTILITIES_HDR}) 
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()
