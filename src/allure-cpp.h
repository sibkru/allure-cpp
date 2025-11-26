#pragma once

/**
 * Allure C++ - Modern API for Allure test reporting
 *
 * This is the primary header for allure-cpp. Include this in your test files.
 *
 * Quick Start:
 *
 *   #include <allure-cpp.h>
 *   using namespace allure;
 *
 *   TEST_F(MyTest, Example) {
 *       test()
 *           .epic("Authentication")
 *           .feature("Login")
 *           .severity("critical");
 *
 *       step("Login as user", [&]() {
 *           // test code
 *       });
 *
 *       attachText("log", "test output");
 *   }
 *
 * All API is in the allure:: namespace.
 */

// Core infrastructure (internal)
#include "API/Core.h"

// Step API (RAII guards and functions)
#include "API/StepGuard.h"
#include "API/StepFunctions.h"

// Metadata builders (fluent API)
#include "API/TestMetadata.h"

// Attachments
#include "API/Attachment.h"

// Utilities
#include "API/Utils.h"

// Framework adapters (conditionally included based on CMake options)
#ifdef ALLURE_GOOGLETEST_ENABLED
    #include "Framework/Adapters/GoogleTest/AllureGTest.h"
#endif

// Note: CppUTest adapter headers are NOT auto-included here due to macro conflicts.
// Include your own/project headers first, and keep CppUTest headers last so their
// new/delete overrides don't leak into other includes. We only expose the lightweight
// AllureCppUTest facade here (it does not include any CppUTest headers); you still
// include CppUTest yourself in your main/tests.
#ifdef ALLURE_CPPUTEST_ENABLED
    // Only include the main class, not the full adapter headers
    #include "Framework/Adapters/CppUTest/AllureCppUTest.h"
#endif

/**
 * Main namespace for Allure C++ API.
 *
 * Contains all public API functions and classes.
 */
namespace allure {
    // All public API is declared in the included headers
}
