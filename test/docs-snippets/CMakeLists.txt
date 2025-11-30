# Documentation snippet tests
# These executables exist solely to verify that code snippets in the documentation compile and work correctly.
# They are also executed to ensure they don't have runtime errors.

# GoogleTest snippet examples
if(ALLURE_ENABLE_GOOGLETEST)
    # Basic GoogleTest setup with custom main
    add_executable(DocsSnippet_GoogleTestSetup
        GoogleTestSetup.cpp
        GoogleTestExamples.cpp
    )
    target_link_libraries(DocsSnippet_GoogleTestSetup PRIVATE
        AllureCpp
        gtest
    )

    # GoogleTest setup using gtest_main with environment
    add_executable(DocsSnippet_GoogleTestEnvironment
        GoogleTestEnvironment.cpp
        GoogleTestExamples.cpp
    )
    target_link_libraries(DocsSnippet_GoogleTestEnvironment PRIVATE
        AllureCpp
        gtest
        gtest_main
    )

    # Landing page example showcased on the homepage
    add_executable(DocsSnippet_GoogleTestLanding
        GoogleTestLanding.cpp
        GoogleTestSetup.cpp
    )
    target_link_libraries(DocsSnippet_GoogleTestLanding PRIVATE
        AllureCpp
        gtest
    )

    # Mark these as documentation tests
    set_target_properties(DocsSnippet_GoogleTestSetup DocsSnippet_GoogleTestEnvironment DocsSnippet_GoogleTestLanding
        PROPERTIES
        FOLDER "Documentation/Snippets"
    )

    # Add CTest targets to run and verify the snippets
    add_test(NAME DocsSnippet_GoogleTestSetup_Run
             COMMAND DocsSnippet_GoogleTestSetup
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_test(NAME DocsSnippet_GoogleTestEnvironment_Run
             COMMAND DocsSnippet_GoogleTestEnvironment
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_test(NAME DocsSnippet_GoogleTestLanding_Run
             COMMAND DocsSnippet_GoogleTestLanding
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # Clean up allure results after each test
    set_tests_properties(DocsSnippet_GoogleTestSetup_Run PROPERTIES
        FIXTURES_CLEANUP DocsSnippet_GoogleTestSetup_Cleanup)
    set_tests_properties(DocsSnippet_GoogleTestEnvironment_Run PROPERTIES
        FIXTURES_CLEANUP DocsSnippet_GoogleTestEnvironment_Cleanup)
    set_tests_properties(DocsSnippet_GoogleTestLanding_Run PROPERTIES
        FIXTURES_CLEANUP DocsSnippet_GoogleTestLanding_Cleanup)
endif()

# CppUTest snippet examples
if(ALLURE_ENABLE_CPPUTEST)
    # Basic CppUTest setup
    add_executable(DocsSnippet_CppUTestSetup
        CppUTestSetup.cpp
        CppUTestExamples.cpp
    )
    target_link_libraries(DocsSnippet_CppUTestSetup PRIVATE
        AllureCpp
        CppUTest
        CppUTestExt
    )

    # CppUTest with custom output
    add_executable(DocsSnippet_CppUTestCustomOutput
        CppUTestCustomOutput.cpp
        CppUTestExamples.cpp
    )
    target_link_libraries(DocsSnippet_CppUTestCustomOutput PRIVATE
        AllureCpp
        CppUTest
        CppUTestExt
    )

    # Mark these as documentation tests
    set_target_properties(DocsSnippet_CppUTestSetup DocsSnippet_CppUTestCustomOutput
        PROPERTIES
        FOLDER "Documentation/Snippets"
    )

    # Add CTest targets to run and verify the snippets
    add_test(NAME DocsSnippet_CppUTestSetup_Run
             COMMAND DocsSnippet_CppUTestSetup
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_test(NAME DocsSnippet_CppUTestCustomOutput_Run
             COMMAND DocsSnippet_CppUTestCustomOutput
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    # Clean up allure results after each test
    set_tests_properties(DocsSnippet_CppUTestSetup_Run PROPERTIES
        FIXTURES_CLEANUP DocsSnippet_CppUTestSetup_Cleanup)
    set_tests_properties(DocsSnippet_CppUTestCustomOutput_Run PROPERTIES
        FIXTURES_CLEANUP DocsSnippet_CppUTestCustomOutput_Cleanup)
endif()
