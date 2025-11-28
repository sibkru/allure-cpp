#!/usr/bin/env python3
"""List DocsSnippet* CMake targets from the build tree.

This script runs `cmake --build build --target help` and extracts target names
starting with the given prefix (default: DocsSnippet_). It prints one target per
line to stdout and exits non‑zero if none are found.

Works on macOS and Linux; requires Python 3.6+ and CMake configured in ./build.
"""
import re
import subprocess
import sys
from typing import Iterable, Set

PREFIX_DEFAULT = "DocsSnippet_"


def extract_targets(lines: Iterable[str], prefix: str) -> Set[str]:
    pattern = re.compile(rf"\b({re.escape(prefix)}[A-Za-z0-9_+.\-]+)")
    targets = set()
    for line in lines:
        for match in pattern.finditer(line):
            targets.add(match.group(1))
    return targets


def main() -> int:
    prefix = sys.argv[1] if len(sys.argv) > 1 else PREFIX_DEFAULT

    try:
        proc = subprocess.run(
            ["cmake", "--build", "build", "--target", "help"],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=True,
        )
    except (FileNotFoundError, subprocess.CalledProcessError) as exc:
        sys.stderr.write(f"Failed to list targets via CMake: {exc}\n")
        if isinstance(exc, subprocess.CalledProcessError) and exc.stderr:
            sys.stderr.write(exc.stderr)
        return 1

    targets = extract_targets(proc.stdout.splitlines(), prefix)
    if not targets:
        sys.stderr.write(f"No targets found with prefix '{prefix}'.\n")
        return 2

    for target in sorted(targets):
        print(target)
    return 0


if __name__ == "__main__":
    sys.exit(main())
