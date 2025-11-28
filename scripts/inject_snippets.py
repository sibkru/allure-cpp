#!/usr/bin/env python3
"""
Inject code snippets into documentation markdown files.

This script reads snippet placeholders in markdown files and replaces them with
actual code from the snippets.json file. It preserves template files and generates
output files with injected snippets.

Snippet placeholder format:
    <!-- snippet:snippet-name:language -->

The script will replace this with:
    ```language
    ... code from snippet ...
    ```
"""

import re
import json
import sys
from pathlib import Path
from typing import Dict, List


class SnippetInjector:
    """Injects code snippets into markdown files."""

    # Regex pattern for snippet placeholders
    PLACEHOLDER_PATTERN = re.compile(
        r'^(\s*)<!--\s*snippet:([^:]+):([^:]+)(?::([^:]+))?\s*-->\s*$'
    )

    def __init__(self, snippets: Dict[str, str]):
        self.snippets = snippets
        self.errors: List[str] = []
        self.warnings: List[str] = []

    def inject_into_file(self, input_path: Path, output_path: Path) -> bool:
        """
        Inject snippets into a single markdown file.

        Args:
            input_path: Source template file with snippet placeholders
            output_path: Destination file with injected code

        Returns:
            True if successful, False if errors occurred
        """
        try:
            with open(input_path, 'r', encoding='utf-8') as f:
                lines = f.readlines()

            output_lines: List[str] = []
            snippets_injected = 0

            for line_num, line in enumerate(lines, start=1):
                match = self.PLACEHOLDER_PATTERN.match(line)

                if match:
                    indent = match.group(1)
                    snippet_name = match.group(2)
                    language = match.group(3)
                    extra = match.group(4)  # Optional attributes (e.g., title=tests.cpp)

                    if snippet_name not in self.snippets:
                        self.errors.append(
                            f"{input_path}:{line_num}: Snippet '{snippet_name}' not found"
                        )
                        # Keep the placeholder in output
                        output_lines.append(line)
                        continue

                    # Inject the snippet
                    snippet_code = self.snippets[snippet_name]

                    # Add opening code fence
                    info_string = language
                    if extra:
                        meta = extra.strip()
                        # Normalize title meta to include quotes if omitted: title=foo.cpp -> title="foo.cpp"
                        if meta.startswith("title=") and '"' not in meta and "'" not in meta:
                            meta = f'title="{meta[len("title="):]}"'
                        info_string = f'{language} {meta}'

                    output_lines.append(f"{indent}```{info_string}\n")

                    # Add snippet code (with proper indentation)
                    for code_line in snippet_code.split('\n'):
                        if code_line.strip():  # Non-empty line
                            output_lines.append(f"{indent}{code_line}\n")
                        else:  # Empty line
                            output_lines.append("\n")

                    # Add closing code fence
                    output_lines.append(f"{indent}```\n")

                    snippets_injected += 1
                else:
                    # Keep the line as-is
                    output_lines.append(line)

            # Write output file
            output_path.parent.mkdir(parents=True, exist_ok=True)
            with open(output_path, 'w', encoding='utf-8') as f:
                f.writelines(output_lines)

            if snippets_injected > 0:
                print(f"  {input_path.name}: injected {snippets_injected} snippet(s)")

            return True

        except Exception as e:
            self.errors.append(f"{input_path}: Error processing file: {e}")
            return False

    def inject_into_directory(
        self,
        input_dir: Path,
        output_dir: Path,
        pattern: str = "**/*.md"
    ) -> int:
        """
        Inject snippets into all matching markdown files in a directory.

        Args:
            input_dir: Source directory with template files
            output_dir: Destination directory for files with injected snippets
            pattern: Glob pattern for matching files (default: **/*.md for .md files)

        Returns:
            Number of files processed successfully
        """
        processed_count = 0

        # Process .md files
        for input_path in sorted(input_dir.glob(pattern)):
            if input_path.is_file():
                # Compute relative path and output path
                relative_path = input_path.relative_to(input_dir)
                output_path = output_dir / relative_path

                if self.inject_into_file(input_path, output_path):
                    processed_count += 1

        # Process .mdx files separately if pattern is for .md
        if pattern == "**/*.md":
            mdx_pattern = "**/*.mdx"
            for input_path in sorted(input_dir.glob(mdx_pattern)):
                if input_path.is_file():
                    relative_path = input_path.relative_to(input_dir)
                    output_path = output_dir / relative_path

                    if self.inject_into_file(input_path, output_path):
                        processed_count += 1

        return processed_count

    def has_errors(self) -> bool:
        """Check if any errors were encountered."""
        return len(self.errors) > 0

    def print_errors(self) -> None:
        """Print all errors to stderr."""
        for error in self.errors:
            print(f"ERROR: {error}", file=sys.stderr)

    def print_warnings(self) -> None:
        """Print all warnings to stderr."""
        for warning in self.warnings:
            print(f"WARNING: {warning}", file=sys.stderr)


def load_snippets(snippets_file: Path) -> Dict[str, str]:
    """Load snippets from JSON file."""
    with open(snippets_file, 'r', encoding='utf-8') as f:
        return json.load(f)


def main():
    """Main entry point."""
    # Determine project root (script is in scripts/)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent

    # Paths
    snippets_file = project_root / "docs" / "generated" / "snippets.json"
    templates_dir = project_root / "docs" / "src" / "content" / "docs-templates"
    output_dir = project_root / "docs" / "src" / "content" / "docs"

    print(f"Loading snippets from: {snippets_file}")

    # Load snippets
    try:
        snippets = load_snippets(snippets_file)
        print(f"Loaded {len(snippets)} snippet(s)")
    except FileNotFoundError:
        print(f"ERROR: Snippets file not found: {snippets_file}", file=sys.stderr)
        print("Please run extract_snippets.py first", file=sys.stderr)
        return 1
    except json.JSONDecodeError as e:
        print(f"ERROR: Invalid JSON in snippets file: {e}", file=sys.stderr)
        return 1

    print(f"\nInjecting snippets from templates in: {templates_dir}")
    print(f"Output directory: {output_dir}")

    # Inject snippets
    injector = SnippetInjector(snippets)
    processed_count = injector.inject_into_directory(templates_dir, output_dir)

    # Report results
    print(f"\nProcessed {processed_count} file(s)")

    if injector.has_errors():
        print("\nErrors encountered during injection:", file=sys.stderr)
        injector.print_errors()
        return 1

    if injector.warnings:
        print("\nWarnings:", file=sys.stderr)
        injector.print_warnings()

    return 0


if __name__ == "__main__":
    sys.exit(main())
