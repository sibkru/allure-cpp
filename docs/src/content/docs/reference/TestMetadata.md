---
title: TestMetadata
description: API reference for the TestMetadata class
---

Fluent builder for test case metadata. Metadata is automatically applied when the builder is destroyed (RAII). Each setter records an operation that is executed once the builder leaves scope, so chains stay cheap and side-effect free until destruction. Builders are intended for use on the thread executing the current test case.

## Public Methods

### TestMetadata

Default constructor

```cpp
void TestMetadata()
```


### TestMetadata

Copy constructor

```cpp
void TestMetadata( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to copy construct from |


### TestMetadata

Move constructor

```cpp
void TestMetadata( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to move construct from |


### ~TestMetadata

Destructor

```cpp
void ~TestMetadata()
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

Set a plain-text description of the test.

```cpp
void description(std::string_view desc)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `desc` | `std::string_view` | Description text. |


### descriptionHtml

Set an HTML-formatted description of the test.

```cpp
void descriptionHtml(std::string_view html)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `html` | `std::string_view` | HTML description; caller is responsible for valid markup. |


### epic

Add an epic label.

```cpp
void epic(std::string_view epic)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `epic` | `std::string_view` | Epic identifier. |


### feature

Add a feature label.

```cpp
void feature(std::string_view feature)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `feature` | `std::string_view` | Feature identifier. |


### flaky

Mark the test as flaky.

```cpp
void flaky()
```


### hiddenParameter

Add a parameter whose value is hidden in the report.

```cpp
void hiddenParameter(std::string_view name, std::string_view value)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Parameter name. |
| `value` | `std::string_view` | Parameter value. |


### issue

Shortcut for adding an issue link.

```cpp
void issue(std::string_view name, std::string_view url)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Issue identifier. |
| `url` | `std::string_view` | Issue URL. |


### known

Mark the test as known (expected) failure.

```cpp
void known()
```


### label

Add a custom label.

```cpp
void label(std::string_view name, std::string_view value)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Label name. |
| `value` | `std::string_view` | Label value. |


### link

Add a link to an external system.

```cpp
void link(std::string_view name, std::string_view url, std::string_view type)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Display name of the link. |
| `url` | `std::string_view` | Target URL. |
| `type` | `std::string_view` | Link type (e.g., "issue", "tms", or custom). Defaults to "custom". |


### maskedParameter

Add a parameter whose value is masked in the report.

```cpp
void maskedParameter(std::string_view name, std::string_view value)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Parameter name. |
| `value` | `std::string_view` | Parameter value. |


### muted

Mark the test as muted (do not affect build status).

```cpp
void muted()
```


### name

Set the test display name.

```cpp
void name(std::string_view name)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Human-readable test name. |


### owner

Add an owner label.

```cpp
void owner(std::string_view owner)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `owner` | `std::string_view` | Responsible person or team. |


### parameter

Add a visible parameter.

```cpp
void parameter(std::string_view name, std::string_view value)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | Parameter name. |
| `value` | `std::string_view` | Parameter value. |


### severity

Add a severity label.

```cpp
void severity(std::string_view severity)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `severity` | `std::string_view` | Severity value (e.g., "critical", "minor"). |


### story

Add a story label.

```cpp
void story(std::string_view story)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `story` | `std::string_view` | Story identifier. |


### tag

Add a tag label.

```cpp
void tag(std::string_view tag)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `tag` | `std::string_view` | Arbitrary tag value. |


### tms

Shortcut for adding a TMS link.

```cpp
void tms(std::string_view name, std::string_view url)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | TMS case identifier. |
| `url` | `std::string_view` | TMS URL. |


