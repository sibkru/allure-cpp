cmake_minimum_required(VERSION 3.14)

# Add project folder into includes
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Configure AllureCpp static library
set(ALLURE_CPP AllureCpp)

# Collect core source files (non-framework-specific)
file(GLOB ALLURE_CORE_SRC_FILES
    "Model/*.cpp"
    "Framework/TestLifecycleListenerBase.cpp"
    "AllureAPI.cpp"
    "Services/ServicesFactory.cpp"
    "Services/EventHandlers/*.cpp"
    "Services/Property/*.cpp"
    "Services/Report/*.cpp"
    "Services/System/*.cpp"
)
file(GLOB ALLURE_CORE_HDR_FILES
    "Model/*.h"
    "Framework/*.h"
    "AllureAPI.h"
    "Services/ServicesFactory.h"
    "Services/IServicesFactory.h"
    "Services/EventHandlers/*.h"
    "Services/Property/*.h"
    "Services/Report/*.h"
    "Services/System/*.h"
)

set(ALLURE_CORE_SRC ${ALLURE_CORE_SRC_FILES})
set(ALLURE_CORE_HDR ${ALLURE_CORE_HDR_FILES})


# Conditionally add GoogleTest adapter sources
if(ALLURE_ENABLE_GOOGLETEST)
    file(GLOB GTEST_ADAPTER_SRC "Framework/Adapters/GoogleTest/*.cpp")
    file(GLOB GTEST_ADAPTER_HDR "Framework/Adapters/GoogleTest/*.h")
    list(APPEND ALLURE_CORE_SRC ${GTEST_ADAPTER_SRC})
    list(APPEND ALLURE_CORE_HDR ${GTEST_ADAPTER_HDR})
    file(GLOB GTEST_SERVICE_SRC "Services/GoogleTest/*.cpp")
    file(GLOB GTEST_SERVICE_HDR "Services/GoogleTest/*.h")
    list(APPEND ALLURE_CORE_SRC ${GTEST_SERVICE_SRC})
    list(APPEND ALLURE_CORE_HDR ${GTEST_SERVICE_HDR})
endif()

# Conditionally add CppUTest adapter sources
if(ALLURE_ENABLE_CPPUTEST)
    file(GLOB CPPUTEST_ADAPTER_SRC "Framework/Adapters/CppUTest/*.cpp")
    file(GLOB CPPUTEST_ADAPTER_HDR "Framework/Adapters/CppUTest/*.h")
    list(APPEND ALLURE_CORE_SRC ${CPPUTEST_ADAPTER_SRC})
    list(APPEND ALLURE_CORE_HDR ${CPPUTEST_ADAPTER_HDR})
endif()

# Create the library
add_library(${ALLURE_CPP} STATIC ${ALLURE_CORE_SRC} ${ALLURE_CORE_HDR})

target_include_directories(${ALLURE_CPP} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Link dependencies
target_link_libraries(${ALLURE_CPP} PUBLIC nlohmann_json::nlohmann_json)

if(ALLURE_ENABLE_GOOGLETEST)
    target_link_libraries(${ALLURE_CPP} PUBLIC gtest)
    target_compile_definitions(${ALLURE_CPP} PUBLIC ALLURE_GOOGLETEST_ENABLED)
endif()

if(ALLURE_ENABLE_CPPUTEST)
    target_link_libraries(${ALLURE_CPP} PUBLIC CppUTest)
    target_compile_definitions(${ALLURE_CPP} PUBLIC ALLURE_CPPUTEST_ENABLED)
endif()

#Configure source groups
foreach(FILE ${ALLURE_CORE_SRC} ${ALLURE_CORE_HDR})
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
