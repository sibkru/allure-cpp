---
title: Add a Testing Framework
description: Learn how to integrate your testing framework with Allure C++
---

Allure C++ was designed with framework-agnosticism in mind, providing a generic adapter interface that makes integration possible for frameworks that follow common testing patterns.

## Overview

The allure-cpp architecture separates framework-specific code from the core reporting logic through a set of interfaces. To add support for a new framework, you implement these interfaces to translate framework events into Allure's generic event model.

The effort required to integrate a framework depends on how closely it aligns with the expected patterns. GoogleTest and CppUTest integrate cleanly because they provide explicit lifecycle hooks and test metadata. Frameworks with significantly different architectures may require substantial adaptation or may not fit the model without major changes.

## Core Interfaces

You need to implement three primary interfaces to integrate a testing framework:

### ITestFrameworkAdapter

The main entry point for framework integration. This interface handles:

- **Framework initialization** - Registering with the test framework's event system
- **Status provider creation** - Creating instances that can query test status at runtime
- **Capability reporting** - Describing which features the framework supports

The adapter connects your framework to the Allure reporting system by implementing the `initialize()`, `createStatusProvider()`, and `getCapabilities()` methods.

**Location:** `src/Framework/ITestFrameworkAdapter.h`

### ITestStatusProvider

Provides runtime status queries during test execution. The interface defines two methods:

- `isCurrentTestFailed()` - Check if the current test has failed
- `isCurrentTestSkipped()` - Check if the current test is skipped

Some frameworks provide APIs to query test status during execution (GoogleTest's `HasFailure()`), while others only expose status after completion. If your framework doesn't support runtime queries, you can return cached values or always return false.

**Location:** `src/Framework/ITestStatusProvider.h`

### ITestMetadata

Exposes test information in a framework-agnostic way. Required methods include:

- `getTestName()` - Individual test name
- `getSuiteName()` - Test suite/group name
- `getFullName()` - Fully qualified test name
- `getFileName()` - Source file location
- `getLineNumber()` - Line number in source

Optional methods support parametric tests, type parameters, and value parameters if your framework provides these features.

**Location:** `src/Framework/ITestMetadata.h`

## Event Flow

Your adapter must translate framework-specific events into lifecycle callbacks defined by `ITestLifecycleListener`. The expected event sequence is:

1. `onTestProgramStart()` - Program initialization
2. For each test suite:
   - `onTestSuiteStart(metadata)` - Suite begins
   - For each test in the suite:
     - `onTestStart(metadata)` - Test begins
     - Test executes
     - `onTestEnd(metadata, status)` - Test completes with status
   - `onTestSuiteEnd(status)` - Suite completes
3. `onTestProgramEnd()` - Program finalization

Frameworks without explicit suite concepts can synthesize suite events or call suite start/end around individual tests.

**Location:** `src/Framework/ITestLifecycleListener.h`

## Framework Capabilities

The `FrameworkCapabilities` structure describes which features your framework supports:

- `supportsTestSuites` - Does the framework have test suites/groups?
- `supportsRuntimeStatus` - Can you query test status during execution?
- `supportsSkippedTests` - Does the framework support skipped/disabled tests?
- `supportsParametricTests` - Does the framework support data-driven tests?
- `frameworkName` - Name of the framework (e.g., "Catch2")
- `frameworkVersion` - Framework version string

These capabilities allow the reporting system to adapt its behavior based on framework limitations.

**Location:** `src/Framework/FrameworkCapabilities.h`

## Implementation Steps

1. **Create adapter directory** - Add a new directory under `src/Framework/Adapters/YourFramework/`

2. **Implement metadata class** - Create a class implementing `ITestMetadata` that wraps your framework's test information

3. **Implement status provider** - Create a class implementing `ITestStatusProvider` using your framework's status query APIs (if available)

4. **Implement framework adapter** - Create a class implementing `ITestFrameworkAdapter` that:
   - Registers event listeners with your framework
   - Translates framework events to `ITestLifecycleListener` calls
   - Creates status providers
   - Reports framework capabilities

5. **Create public API** - Provide a simple initialization class (like `AllureGTest` or `AllureCppUTest`) that users instantiate in their main function

6. **Add CMake configuration** - Create an option to enable/disable your adapter and link against your framework

## Reference Implementations

Study the existing adapters for examples:

**GoogleTest adapter:**
- Entry point: [`AllureGTest.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/GoogleTest/AllureGTest.h)
- Adapter: [`GTestAdapter.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/GoogleTest/GTestAdapter.h)
- Event listener: [`GTestEventListener.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/GoogleTest/GTestEventListener.h)
- Status provider: [`GTestStatusProvider.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/GoogleTest/GTestStatusProvider.h)
- Metadata: [`GTestMetadata.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/GoogleTest/GTestMetadata.h)

**CppUTest adapter:**
- Entry point: [`AllureCppUTest.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/CppUTest/AllureCppUTest.h)
- Adapter: [`CppUTestAdapter.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/CppUTest/CppUTestAdapter.h)
- Plugin: [`CppUTestPlugin.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/CppUTest/CppUTestPlugin.h)
- Status provider: [`CppUTestStatusProvider.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/CppUTest/CppUTestStatusProvider.h)
- Metadata: [`CppUTestMetadata.h`](https://github.com/sibkru/allure-cpp/tree/main/src/Framework/Adapters/CppUTest/CppUTestMetadata.h)

## Limitations

The generic framework assumes certain patterns common to xUnit-style testing frameworks:

- Hierarchical test organization (program → suites → tests)
- Discrete test lifecycle events (start/end boundaries)
- Ability to access test metadata (name, file, line)
- Status reporting (pass/fail/skip)

Frameworks with fundamentally different architectures (e.g., pure BDD frameworks, property-based testing, or frameworks without clear test boundaries) may not map cleanly to this model. In such cases, significant architectural changes might be required, or integration might not be practical.

The design prioritizes common C++ testing frameworks like GoogleTest, CppUTest, Catch2, and Boost.Test, which all follow similar patterns. If your framework differs substantially, evaluate whether the integration effort is justified or whether adapting your test structure to fit the model is feasible.
