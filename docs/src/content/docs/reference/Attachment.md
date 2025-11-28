---
title: Attachment
description: API reference for the Attachment class
---

Attachment builder for adding files, screenshots, and logs to test reports.

## Public Methods

### attach

Attaches the built attachment to the current test or step.

```cpp
void attach()
```


### fromBinary

Creates an attachment from a binary data buffer.

```cpp
Attachment fromBinary(std::string_view name, std::string_view mimeType, const void* data, size_t size)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the attachment. |
| `mimeType` | `std::string_view` | The MIME type of the attachment (e.g., "image/png"). |
| `data` | `const void*` | A pointer to the binary data. |
| `size` | `size_t` | The size of the data in bytes. |

**Returns:**

An Attachment object.


### fromFile

Creates an attachment from a file.

```cpp
Attachment fromFile(std::string_view name, std::string_view filePath)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the attachment. |
| `filePath` | `std::string_view` | The path to the file. The MIME type will be detected automatically. |

**Returns:**

An Attachment object.


### fromText

Creates a plain text attachment.

```cpp
Attachment fromText(std::string_view name, std::string_view content)
```

**Parameters:**

| Name | Type | Description |
|------|------|-------------|
| `name` | `std::string_view` | The name of the attachment. |
| `content` | `std::string_view` | The text content. |

**Returns:**

An Attachment object.


