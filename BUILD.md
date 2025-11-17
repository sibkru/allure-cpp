# Build from sources

## Prerequisites
  - [Git](https://git-scm.com/)
  - [CMake](https://cmake.org/) (version 3.14 or higher)
  - A C++14 compiler or newer (GCC, Clang, MSVC)
  - Internet connection (for downloading dependencies)

## Build steps

This project uses CMake with FetchContent to automatically download and build dependencies (GoogleTest and RapidJSON). No package manager required!

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

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Using a specific generator (e.g., Ninja)
cmake -G Ninja ..
```

## Dependencies

All dependencies are automatically fetched and built by CMake:
- **GoogleTest** (v1.14.0) - for unit testing and mocking
- **RapidJSON** (v1.1.0) - for JSON serialization

No manual installation or package managers (Conan, vcpkg, etc.) required!