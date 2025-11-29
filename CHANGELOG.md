# Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/)
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

### Added
- (placeholder)

### Changed
- (placeholder)

### Removed
- (placeholder)



## [0.1.1] - 2025-11-29
### Changed
- fixed CMakeLists.txt file to correctly use googletest, when allure-cpp is included as library in another project and googletest is fetched via Cmake FetchContent
- fixed docs snippets to ensure includes are part of the snippets
- fixed getting starting cmakelists.txt to contain project and minimum cmake version
- fixed includes of public library headers to angle brackets in examples and docs



## [0.1.0] - 2025-11-28
### Added
- Initial public release of `allure-cpp`, a library for generating Allure 2 reports from C++ tests.
- GoogleTest adapter and example report.
- CppUTest adapter and example report.
- CMake integration with minimum C++17.
- Documentation site and quick-start guides.
