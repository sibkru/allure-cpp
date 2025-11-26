# Build from sources

## Prerequisites
  - [Git](https://git-scm.com/)
  - [CMake](https://cmake.org/) (version 3.14 or higher)
  - A C++17 compiler or newer (GCC, Clang, MSVC)
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

After building, run the test executables from the build directory:

```bash
./bin/UnitTest
./bin/IntegrationTest
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

### Running Examples

Examples are built only if you enable the corresponding adapter and set `-DALLURE_BUILD_EXAMPLES=ON`.

```bash
# GoogleTest example (requires ALLURE_ENABLE_GOOGLETEST=ON and ALLURE_BUILD_EXAMPLES=ON)
./bin/GoogleTestAllureExample

# CppUTest example (requires ALLURE_ENABLE_CPPUTEST=ON and ALLURE_BUILD_EXAMPLES=ON)
./bin/CppUTestAllureExample
```

## Dependencies

Dependencies are automatically fetched based on enabled frameworks:

**Core (always required):**
- **nlohmann/json** (v3.11.3) - JSON serialization
- **fmt** (v10.2.1) - string formatting

**Optional (controlled by CMake options):**
- **GoogleTest** (v1.14.0) - when `ALLURE_ENABLE_GOOGLETEST=ON`
- **CppUTest** (v4.0) - when `ALLURE_ENABLE_CPPUTEST=ON`
