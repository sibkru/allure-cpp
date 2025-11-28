---
title: StepGuard
description: API reference for the StepGuard class
---

RAII guard for automatic step lifecycle management.

## Public Methods

### StepGuard

Copy constructor

```cpp
void StepGuard( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to copy construct from |


### StepGuard

Move constructor

```cpp
void StepGuard( other)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `other` | `` | The object to move construct from |


### StepGuard

Construct a step guard and start the step.

```cpp
void StepGuard(std::string_view name)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the step. |


### ~StepGuard

Destructor automatically ends the step. This is noexcept to prevent exceptions during stack unwinding.

```cpp
void ~StepGuard()
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


