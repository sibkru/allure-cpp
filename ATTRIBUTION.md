# Attribution

## Origin

This project originated as a fork of [cpp-gtest-allure-utilities](https://github.com/systelab/cpp-gtest-allure-utilities) by Systelab Technologies, licensed under the MIT License.

## Why Start an Independent Repository

The original project was designed specifically for GoogleTest and generated reports in the Allure 1.x format. This project updated the json-generation to the Allure 2 schema, refactored the code to not depend on GoogleTest, but to enable the addition of more test frameworks, and simplified the build, to simply work using CMake Fetch Content.

### Vendored Dependencies

This project includes vendored copies of three Systelab libraries under the `vendor/` directory, properly attributed in `vendor/THIRD_PARTY_LICENSES.md`:
- JSONAdapterInterface
- JSONAdapterTestUtilities
- RapidJSONAdapter

All modifications to these vendored libraries are documented in `vendor/MODIFICATIONS.md`.

## Acknowledgments

We gratefully acknowledge Systelab Technologies for creating the original cpp-gtest-allure-utilities project, which served as the foundation for this work.

## License

This project maintains the original MIT License. See [LICENSE](LICENSE) file for details.
