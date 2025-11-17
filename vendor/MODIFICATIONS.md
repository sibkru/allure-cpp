# Modifications to Vendored Code

This document lists all modifications made to the vendored Systelab libraries.

## Why Modifications Were Necessary

GTestAllureUtilities only needs basic JSON serialization capabilities from the Systelab adapters. We don't use JSON schema validation functionality. Additionally, the schema validation code has compatibility issues with the RapidJSON version we're using (v1.1.0).

## Libraries Modified

### cpp-rapidjson-json-adapter

**Source**: https://github.com/systelab/cpp-rapidjson-json-adapter
**License**: MIT (see THIRD_PARTY_LICENSES.md)

#### Files Modified

##### 1. RapidJSONAdapter/JSONAdapter.h

**Lines Changed**: 19-20

**Modifications**:
- Kept `buildSchemaValidator()` method declarations (required by interface)
- Changed comment to reflect they return nullptr

**Reason**:
- Must implement all pure virtual methods from IJSONAdapter to avoid abstract class
- Methods exist but return nullptr since functionality not needed

##### 2. RapidJSONAdapter/JSONAdapter.cpp

**Lines Changed**: 4, 36-45

**Modifications**:
- Commented out `#include "JSONSchemaValidator.h"` (line 4)
- Changed `buildSchemaValidator()` implementations to return `nullptr` (lines 36-45)

**Reason**:
- JSONSchemaValidator.cpp has API incompatibilities with RapidJSON v1.1.0
- Schema validation is not used by GTestAllureUtilities
- Returning nullptr satisfies the interface contract without the broken dependency
- Avoids compilation errors without losing needed functionality

**Diff**:
```cpp
// Original:
#include "JSONSchemaValidator.h"

std::unique_ptr<IJSONSchemaValidator> JSONAdapter::buildSchemaValidator(const IJSONDocument& jsonDocument) const
{
    return std::make_unique<JSONSchemaValidator>(jsonDocument);
}

// Modified:
// MODIFICATION: Commented out - see vendor/MODIFICATIONS.md
// #include "JSONSchemaValidator.h"

// MODIFICATION: Schema validation methods commented out - see vendor/MODIFICATIONS.md
// std::unique_ptr<IJSONSchemaValidator> JSONAdapter::buildSchemaValidator(const IJSONDocument& jsonDocument) const
// {
//     return std::make_unique<JSONSchemaValidator>(jsonDocument);
// }
```

#### Files Excluded from Build

##### JSONSchemaValidator.cpp

**Reason**: Has RapidJSON v1.1.0 API incompatibilities and is not needed

**Exclusion Method**: CMake `list(REMOVE_ITEM)` in src/GTestAllureUtilities/CMakeLists.txt

---

### cpp-json-adapter

**Source**: https://github.com/systelab/cpp-json-adapter
**License**: MIT (see THIRD_PARTY_LICENSES.md)

#### Files Modified

**None** - Interface headers used as-is

---

## Future Updates

If updating to newer versions of these libraries:

1. Check if JSONSchemaValidator has been fixed for newer RapidJSON versions
2. If fixed, remove the modifications and include full functionality
3. If not fixed, reapply these modifications
4. Update this document with any new changes
