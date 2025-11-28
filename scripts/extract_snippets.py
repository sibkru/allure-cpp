#!/usr/bin/env python3
"""
Extract code snippets from test files.

This script scans test files for snippet markers and extracts the code between them.
Snippet markers should be in the format:
    // [snippet:snippet-name]
    ... code ...
    // [/snippet:snippet-name]

The extracted snippets are stored in a JSON file for later injection into documentation.
"""

import re
import json
import sys
from pathlib import Path
from typing import Dict, List, Tuple


class SnippetExtractor:
    """Extracts code snippets from source files."""

    # Regex patterns for snippet markers
    START_PATTERN = re.compile(r'^\s*//\s*\[snippet:([^\]]+)\]\s*$')
    END_PATTERN = re.compile(r'^\s*//\s*\[/snippet:([^\]]+)\]\s*$')

    def __init__(self):
        self.snippets: Dict[str, str] = {}
        self.errors: List[str] = []

    def extract_from_file(self, file_path: Path) -> None:
        """Extract all snippets from a single file."""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                lines = f.readlines()

            current_snippet: str | None = None
            snippet_lines: List[str] = []
            snippet_start_line: int = 0

            for line_num, line in enumerate(lines, start=1):
                # Check for start marker
                start_match = self.START_PATTERN.match(line)
                if start_match:
                    if current_snippet is not None:
                        self.errors.append(
                            f"{file_path}:{line_num}: Found nested snippet start "
                            f"'{start_match.group(1)}' inside snippet '{current_snippet}'"
                        )
                        continue

                    current_snippet = start_match.group(1)
                    snippet_start_line = line_num
                    snippet_lines = []
                    continue

                # Check for end marker
                end_match = self.END_PATTERN.match(line)
                if end_match:
                    snippet_name = end_match.group(1)

                    if current_snippet is None:
                        self.errors.append(
                            f"{file_path}:{line_num}: Found end marker for '{snippet_name}' "
                            "without matching start"
                        )
                        continue

                    if snippet_name != current_snippet:
                        self.errors.append(
                            f"{file_path}:{line_num}: End marker '{snippet_name}' doesn't match "
                            f"start marker '{current_snippet}' at line {snippet_start_line}"
                        )
                        current_snippet = None
                        snippet_lines = []
                        continue

                    # Store the snippet
                    snippet_code = self._process_snippet_lines(snippet_lines)

                    if current_snippet in self.snippets:
                        self.errors.append(
                            f"{file_path}:{line_num}: Duplicate snippet name '{current_snippet}'"
                        )
                    else:
                        self.snippets[current_snippet] = snippet_code

                    current_snippet = None
                    snippet_lines = []
                    continue

                # If we're inside a snippet, collect the line
                if current_snippet is not None:
                    snippet_lines.append(line)

            # Check for unclosed snippets
            if current_snippet is not None:
                self.errors.append(
                    f"{file_path}:{snippet_start_line}: Unclosed snippet '{current_snippet}'"
                )

        except Exception as e:
            self.errors.append(f"{file_path}: Error reading file: {e}")

    def _process_snippet_lines(self, lines: List[str]) -> str:
        """Process snippet lines by removing common leading whitespace."""
        if not lines:
            return ""

        # Remove trailing newline from last line if present
        processed_lines = [line.rstrip('\n') for line in lines]

        # Find minimum indentation (excluding empty lines)
        non_empty_lines = [line for line in processed_lines if line.strip()]
        if not non_empty_lines:
            return ""

        min_indent = min(len(line) - len(line.lstrip()) for line in non_empty_lines)

        # Remove common indentation
        dedented_lines = []
        for line in processed_lines:
            if line.strip():  # Non-empty line
                dedented_lines.append(line[min_indent:])
            else:  # Empty line
                dedented_lines.append("")

        return '\n'.join(dedented_lines)

    def extract_from_directory(self, directory: Path, pattern: str = "**/*.cpp") -> None:
        """Extract snippets from all matching files in a directory."""
        for file_path in sorted(directory.glob(pattern)):
            if file_path.is_file():
                self.extract_from_file(file_path)

    def save_to_json(self, output_file: Path) -> None:
        """Save extracted snippets to a JSON file."""
        output_file.parent.mkdir(parents=True, exist_ok=True)

        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(self.snippets, f, indent=2, ensure_ascii=False)

    def has_errors(self) -> bool:
        """Check if any errors were encountered."""
        return len(self.errors) > 0

    def print_errors(self) -> None:
        """Print all errors to stderr."""
        for error in self.errors:
            print(f"ERROR: {error}", file=sys.stderr)


def main():
    """Main entry point."""
    # Determine project root (script is in scripts/)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent

    # Source and output paths
    snippets_dir = project_root / "test" / "docs-snippets"
    output_file = project_root / "docs" / "generated" / "snippets.json"

    print(f"Extracting snippets from: {snippets_dir}")

    # Extract snippets
    extractor = SnippetExtractor()
    extractor.extract_from_directory(snippets_dir, "**/*.cpp")

    # Report results
    if extractor.has_errors():
        print("\nErrors encountered during extraction:", file=sys.stderr)
        extractor.print_errors()
        return 1

    print(f"Extracted {len(extractor.snippets)} snippets:")
    for name in sorted(extractor.snippets.keys()):
        line_count = extractor.snippets[name].count('\n') + 1
        print(f"  - {name} ({line_count} lines)")

    # Save to JSON
    extractor.save_to_json(output_file)
    print(f"\nSnippets saved to: {output_file}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
