---
title: Functions
description: Namespace-level utility functions
---

Utility functions available in the `allure` namespace.

### attachFile

Convenience function to attach a file.

```cpp
void attachFile(std::string_view name, std::string_view filePath)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the attachment. |
| `filePath` | `std::string_view` | The path to the file to attach. |


### attachText

Convenience function to attach plain text content.

```cpp
void attachText(std::string_view name, std::string_view content)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the attachment. |
| `content` | `std::string_view` | The text content to attach. |


### convertStatus

```cpp
model::Status convertStatus( provider)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `provider` | `` | No description |

**Returns:**

Returns a value of type `model::Status`


### step

Starts a test step and returns a guard object.

```cpp
StepGuard step(std::string_view name)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the step. Keep the guard alive for the full step scope. |

**Returns:**

A object that controls the lifetime of the step.


### suite

Factory function to create a SuiteMetadata builder.

```cpp
SuiteMetadata suite()
```

**Returns:**

Fluent builder for test suite metadata. Metadata is automatically applied when the builder is destroyed (RAII). Each setter queues work to be executed once the suite builder leaves scope.


### test

Factory function to create a TestMetadata builder.

```cpp
TestMetadata test()
```

**Returns:**

Fluent builder for test case metadata. Metadata is automatically applied when the builder is destroyed (RAII). Each setter records an operation that is executed once the builder leaves scope, so chains stay cheap and side-effect free until destruction. Builders are intended for use on the thread executing the current test case.


