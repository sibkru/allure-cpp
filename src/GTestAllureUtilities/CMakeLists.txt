cmake_minimum_required(VERSION 3.14)

# Configure preprocessor definitions
add_compile_options(-D RAPIDJSON_HAS_STDSTRING=1)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure GTestAllureUtilities static library
set(GTEST_ALLURE_UTILITIES GTestAllureUtilities)
file(GLOB_RECURSE GTEST_ALLURE_UTILITIES_SRC "*.cpp")
file(GLOB_RECURSE GTEST_ALLURE_UTILITIES_HDR "*.h")

# Add vendored Systelab RapidJSONAdapter sources (see vendor/MODIFICATIONS.md)
file(GLOB RAPIDJSON_ADAPTER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/RapidJSONAdapter/*.cpp")
# Exclude JSONSchemaValidator - not needed and has RapidJSON version incompatibility
list(REMOVE_ITEM RAPIDJSON_ADAPTER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/RapidJSONAdapter/JSONSchemaValidator.cpp")
list(APPEND GTEST_ALLURE_UTILITIES_SRC ${RAPIDJSON_ADAPTER_SRC})

add_library(${GTEST_ALLURE_UTILITIES} STATIC ${GTEST_ALLURE_UTILITIES_SRC} ${GTEST_ALLURE_UTILITIES_HDR})
target_include_directories(${GTEST_ALLURE_UTILITIES} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor
)
target_link_libraries(${GTEST_ALLURE_UTILITIES} PUBLIC gtest)

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
