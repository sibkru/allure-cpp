# Documentation Snippets

This directory contains code snippets that are extracted and injected into the documentation. The snippets are actual compilable code that is built and tested as part of the build process, ensuring that documentation examples always reflect working code.

## Overview

The snippet system ensures documentation stays synchronized with the actual library API by:

1. **Writing snippets as compilable tests** - All code examples are real C++ files that compile
2. **Extracting snippets** - A Python script parses snippet markers and extracts the code
3. **Injecting into docs** - Another Python script injects extracted snippets into markdown templates
4. **Building snippets** - CMake builds all snippet files to verify they compile

## Directory Structure

```
test/docs-snippets/
├── CMakeLists.txt              # Build configuration for snippet executables
├── README.md                   # This file
├── GoogleTestExamples.cpp      # GoogleTest API usage examples
├── GoogleTestSetup.cpp         # GoogleTest main() with custom setup
├── GoogleTestEnvironment.cpp   # GoogleTest main() using gtest_main
├── CppUTestExamples.cpp        # CppUTest API usage examples
├── CppUTestSetup.cpp           # CppUTest main() with standard setup
└── CppUTestCustomOutput.cpp    # CppUTest main() with custom output
```

## Snippet Marker Format

Snippets are marked in C++ files using special comments:

```cpp
// [snippet:snippet-name]
... code to extract ...
// [/snippet:snippet-name]
```

Example:

```cpp
// [snippet:googletest-main]
int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    allure::adapters::googletest::AllureGTest allureHelper;
    return RUN_ALL_TESTS();
}
// [/snippet:googletest-main]
```

## Documentation Template Format

In markdown template files (under `docs/src/content/docs-templates/`), use snippet placeholders:

```markdown
<!-- snippet:snippet-name:language -->
```

You can optionally add attributes after the language. For example, to show a file name on the code fence:

```markdown
<!-- snippet:my-snippet:cpp:title=tests.cpp -->
```

Example:

```markdown
## Main File Setup

Create your test runner's main file:

<!-- snippet:googletest-main:cpp -->
```

## Workflow

### 1. Build the Snippet Tests

First, ensure all snippets compile correctly:

```bash
# Configure with desired frameworks enabled
cmake -B build -DALLURE_ENABLE_GOOGLETEST=ON -DALLURE_ENABLE_CPPUTEST=ON

# Build snippet tests
cmake --build build --target DocsSnippet_GoogleTestSetup
cmake --build build --target DocsSnippet_GoogleTestEnvironment
cmake --build build --target DocsSnippet_CppUTestSetup
cmake --build build --target DocsSnippet_CppUTestCustomOutput
```

Or build all snippet targets at once (if your CMake generator supports it).

### 2. Extract Snippets

Run the extraction script to parse snippet markers and generate the JSON file:

```bash
python3 scripts/extract_snippets.py
```

This creates `docs/generated/snippets.json` containing all extracted code snippets.

### 3. Inject Snippets into Documentation

Run the injection script to replace placeholders in templates with actual code:

```bash
python3 scripts/inject_snippets.py
```

This reads templates from `docs/src/content/docs-templates/` and writes complete markdown files to `docs/src/content/docs/` with injected snippets.

### 4. Build Documentation

Now you can build your documentation site (e.g., with Starlight/Astro):

```bash
cd docs
npm install
npm run build
```

## Complete Workflow Script

The complete workflow is automated in `scripts/update_docs.sh`:

```bash
# Update documentation (snippets only)
./scripts/update_docs.sh

# Update documentation and build the site
./scripts/update_docs.sh --build-docs
```

The script performs these steps:
1. Builds and runs snippet tests to verify code examples
2. Generates API reference docs with MrDocs (if available)
3. Extracts snippets from test source files
4. Injects snippets into documentation templates
5. Optionally builds the documentation site

## Adding New Snippets

To add a new snippet to the documentation:

1. **Add snippet to source file**: Create or edit a `.cpp` file in `test/docs-snippets/` and wrap your code with snippet markers:

   ```cpp
   // [snippet:my-new-example]
   void myFunction() {
       // Your code here
   }
   // [/snippet:my-new-example]
   ```

2. **Update CMakeLists.txt** (if needed): If you created a new file with a `main()` function, add a new executable target to `test/docs-snippets/CMakeLists.txt`.

3. **Add placeholder to template**: In the appropriate template file under `docs/src/content/docs-templates/`, add a snippet placeholder:

   ```markdown
   <!-- snippet:my-new-example:cpp -->
   ```

4. **Run the workflow**: Execute the scripts to extract and inject the snippet.

## Best Practices

### Keep Snippets Focused

Each snippet should demonstrate one specific concept. Don't combine multiple unrelated features in a single snippet.

### Use Descriptive Names

Snippet names should clearly indicate what they demonstrate:
- ✅ `googletest-suite-metadata`
- ✅ `cpputest-steps`
- ❌ `example1`
- ❌ `test`

### Ensure Snippets Compile

All snippets must be part of a compilable executable. The CMakeLists.txt should build targets that include each snippet.

### Separate Incompatible Snippets

If snippets cannot coexist in the same file (e.g., multiple `main()` functions), create separate files and separate CMake targets.

### Template Files are Source

The template files in `docs/src/content/docs-templates/` are the source of truth. Never edit the generated files in `docs/src/content/docs/` directly - they will be overwritten.

## Troubleshooting

### Snippet Not Found Error

If you get "Snippet 'xxx' not found", ensure:
1. The snippet markers are correctly formatted in the source file
2. The snippet name matches exactly (case-sensitive)
3. You ran `extract_snippets.py` after adding the snippet

### Duplicate Snippet Name

Each snippet name must be unique across all files. If you get a duplicate error, rename one of the snippets.

### Compilation Errors

If snippet tests fail to compile:
1. Check that all necessary headers are included
2. Verify that the snippet is valid C++ code
3. Ensure the CMake target links all required libraries

### Unclosed Snippet Marker

Ensure every `[snippet:name]` has a matching `[/snippet:name]` with the exact same name.
