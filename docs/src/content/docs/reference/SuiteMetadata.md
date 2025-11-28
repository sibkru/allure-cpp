---
title: SuiteMetadata
description: API reference for the SuiteMetadata class
---

Fluent builder for test suite metadata. Metadata is automatically applied when the builder is destroyed (RAII). Each setter queues work to be executed once the suite builder leaves scope.

## Public Methods

### SuiteMetadata

Default constructor

```cpp
void SuiteMetadata()
```


### SuiteMetadata

Copy constructor

```cpp
void SuiteMetadata( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to copy construct from |


### SuiteMetadata

Move constructor

```cpp
void SuiteMetadata( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to move construct from |


### ~SuiteMetadata

Destructor

```cpp
void ~SuiteMetadata()
```


### operator=

Copy assignment operator

```cpp
void operator=( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to copy assign from |


### operator=

Move assignment operator

```cpp
void operator=( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to move assign from |


### description

Set the suite description.

```cpp
void description(std::string_view desc)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `desc` | `std::string_view` | Description text. |


### epic

Add an epic label to the suite.

```cpp
void epic(std::string_view epic)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `epic` | `std::string_view` | Epic identifier. |


### label

Add a custom label to the suite.

```cpp
void label(std::string_view name, std::string_view value)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Label name. |
| `value` | `std::string_view` | Label value. |


### name

Set the suite name.

```cpp
void name(std::string_view name)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Human-readable suite name. |


### severity

Add a severity label to the suite.

```cpp
void severity(std::string_view severity)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `severity` | `std::string_view` | Severity value. |


