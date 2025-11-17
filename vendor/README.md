# Vendored Dependencies

This directory contains third-party libraries that have been vendored (copied into this repository) for ease of use with CMake FetchContent workflow.

## Why vendor these libraries?

These libraries were originally distributed via Conan package manager. To simplify the build process and remove the Conan dependency, we've vendored them directly with proper attribution.

## Libraries Included

| Library | Version | Source | Purpose |
|---------|---------|--------|---------|
| JSONAdapterInterface | Latest | [systelab/cpp-json-adapter](https://github.com/systelab/cpp-json-adapter) | Interface definitions for JSON operations |
| JSONAdapterTestUtilities | Latest | [systelab/cpp-json-adapter](https://github.com/systelab/cpp-json-adapter) | Test utilities and mocks for JSON adapter |
| RapidJSONAdapter | Latest | [systelab/cpp-rapidjson-json-adapter](https://github.com/systelab/cpp-rapidjson-json-adapter) | RapidJSON implementation of JSON adapter interfaces |

## License

All vendored libraries are MIT licensed. See [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md) for full license text and attribution.

## Updates

To update these vendored libraries to newer versions:

```bash
# Update cpp-json-adapter
git clone https://github.com/systelab/cpp-json-adapter.git /tmp/cpp-json-adapter
cp -r /tmp/cpp-json-adapter/JSONAdapterInterface ./vendor/
cp -r /tmp/cpp-json-adapter/JSONAdapterTestUtilities ./vendor/

# Update cpp-rapidjson-json-adapter
git clone https://github.com/systelab/cpp-rapidjson-json-adapter.git /tmp/cpp-rapidjson-json-adapter
cp -r /tmp/cpp-rapidjson-json-adapter/src/RapidJSONAdapter ./vendor/
```
