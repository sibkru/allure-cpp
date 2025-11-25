# Build from sources

## Prerequisites
  - [Git](https://git-scm.com/)
  - [CMake](https://cmake.org/) (version 3.14 or higher)
  - A C++14 compiler or newer (GCC, Clang, MSVC)
  - Internet connection (for downloading dependencies)

## Build steps

This project uses CMake with FetchContent to automatically download and build dependencies. No package manager required!

By default, no test framework adapters are enabled. You must explicitly enable the adapters you need via CMake options.

### All Platforms

```bash
git clone https://github.com/sibkru/allure-cpp.git
cd allure-cpp
mkdir build && cd build
cmake ..
cmake --build .
```

### Running Tests

After building, you can run the tests:

```bash
# From the build directory
ctest

# Or run individual test executables
./bin/UnitTest
./bin/IntegrationTest
./bin/SampleTestProject
```

### Build Options

#### Framework Selection

```bash
# Build with GoogleTest support
cmake -DALLURE_ENABLE_GOOGLETEST=ON ..

# Build with CppUTest support
cmake -DALLURE_ENABLE_CPPUTEST=ON -DALLURE_ENABLE_GOOGLETEST=OFF ..

# Build with both frameworks
cmake -DALLURE_ENABLE_GOOGLETEST=ON -DALLURE_ENABLE_CPPUTEST=ON ..
```

#### Build Type

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Using a specific generator (e.g., Ninja)
cmake -G Ninja ..
```

### Running Examples

Examples are only built if you enable the corresponding adapter.

```bash
# GoogleTest example (requires ALLURE_ENABLE_GOOGLETEST=ON)
./bin/GoogleTestAllureExample

# CppUTest example (requires ALLURE_ENABLE_CPPUTEST=ON)
./bin/CppUTestAllureExample
```

## Dependencies

Dependencies are automatically fetched based on enabled frameworks:

**Core (always required):**
- **nlohmann/json** (v3.11.3) - JSON serialization

**Optional (controlled by CMake options):**
- **GoogleTest** (v1.14.0) - when `ALLURE_ENABLE_GOOGLETEST=ON`
- **CppUTest** (v4.0) - when `ALLURE_ENABLE_CPPUTEST=ON`

No manual installation or package managers (Conan, vcpkg, etc.) required!
