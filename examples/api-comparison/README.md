# API Comparison Example

This example demonstrates that the **old API** (`buildListener()`) and the **new API** (`initializeGoogleTest()`) produce **identical Allure reports**.

## Overview

Two executables are built from the same test file (`SampleTests.cpp`) but with different `main()` functions:

- **ApiComparisonOldAPI** - Uses the traditional `buildListener()` approach
- **ApiComparisonNewAPI** - Uses the new `initializeGoogleTest()` approach

Both generate Allure JSON reports that can be compared to verify they are functionally equivalent.

## Building

From the project root:

```bash
cmake --build build --target ApiComparisonOldAPI ApiComparisonNewAPI
```

## Running

```bash
# Run with old API
./build/examples/ApiComparisonOldAPI

# Run with new API
./build/examples/ApiComparisonNewAPI
```

## Generated Reports

The executables generate Allure JSON files in:
- `allure-results-old-api/` - JSON files from old API
- `allure-results-new-api/` - JSON files from new API

Each run creates 3 JSON files (one per test suite):
- BasicTestSuite
- ParametricTestSuite
- ComplexTestSuite

## Verifying Equivalence

### Option 1: Visual Inspection with Allure

Generate and view Allure HTML reports:

```bash
# Generate report for old API
allure generate allure-results-old-api -o allure-report-old --clean

# Generate report for new API
allure generate allure-results-new-api -o allure-report-new --clean

# Open in browser to compare
allure open allure-report-old
allure open allure-report-new
```

The reports should be visually identical except for:
- UUIDs (different per run)
- Timestamps (different per run)

### Option 2: JSON Comparison

Compare the structure of generated JSON files (ignoring timestamps and UUIDs):

```bash
# Compare one test suite from each
cat "allure-results-old-api"/*BasicTestSuite*.json | jq 'del(.uuid, .start, .stop, .steps[].start, .steps[].stop)' > old.json
cat "allure-results-new-api"/*BasicTestSuite*.json | jq 'del(.uuid, .start, .stop, .steps[].start, .steps[].stop)' > new.json
diff old.json new.json
```

The diff should be empty, showing the reports are structurally identical.

## Test Coverage

The sample tests demonstrate:

### BasicTestSuite
- **testSimplePass** - Simple assertions
- **testWithSteps** - Multiple action/expected result steps
- **testWithStatusQuery** - Using the new `getStatusProvider()` method

### ParametricTestSuite
- **testWithParameter** - Parametric tests (3 instances)

### ComplexTestSuite
- **testNestedSteps** - Nested operations with multiple steps
- **testMultipleAssertions** - Multiple expectation steps

## Key Observations

1. **Both APIs work identically** - All tests pass with both approaches
2. **Same JSON structure** - Generated Allure reports are structurally identical
3. **Backward compatibility** - Old API continues to work perfectly
4. **New features** - `getStatusProvider()` works with both APIs

## Conclusion

This example proves that:
- The new API (`initializeGoogleTest()`) is a drop-in replacement for the old API
- Existing code using `buildListener()` will continue to work unchanged
- Both approaches produce identical Allure reports
- The refactoring maintains 100% backward compatibility

## Installing Allure (Optional)

To generate HTML reports, install Allure:

```bash
# macOS
brew install allure

# Linux
sudo apt-get install allure

# Or download from https://docs.qameta.io/allure/#_installing_a_commandline
```
