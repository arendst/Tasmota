#!/usr/bin/env python3
"""Reject forbidden C++ standard-library string/container types in library sources."""

import argparse
import re
import sys
from pathlib import Path

FORBIDDEN_TYPES = ("string", "vector", "array", "map")
EXPECTED_CALLBACK = re.compile(
    r"^\s*#\s*define\s+MQTT_CALLBACK_SIGNATURE\s+"
    r"std\s*::\s*function\s*<\s*void\s*\(\s*char\s*\*\s*,\s*"
    r"uint8_t\s*\*\s*,\s*unsigned\s+int\s*\)\s*>\s*callback\s*$"
)
FUNCTION_INCLUDE = re.compile(r"^\s*#\s*include\s*<functional>\s*$")


def strip_comments_and_literals(source: str) -> str:
    """Replace C++ comments and literals with spaces while preserving line breaks."""
    result = []
    index = 0
    state = "code"

    while index < len(source):
        current = source[index]
        following = source[index + 1] if index + 1 < len(source) else ""

        if state == "code":
            if current == "/" and following == "/":
                result.extend("  ")
                index += 2
                state = "line_comment"
            elif current == "/" and following == "*":
                result.extend("  ")
                index += 2
                state = "block_comment"
            elif current == '"':
                result.append(" ")
                index += 1
                state = "string"
            elif current == "'":
                result.append(" ")
                index += 1
                state = "character"
            else:
                result.append(current)
                index += 1
        elif state == "line_comment":
            result.append("\n" if current == "\n" else " ")
            index += 1
            if current == "\n":
                state = "code"
        elif state == "block_comment":
            if current == "*" and following == "/":
                result.extend("  ")
                index += 2
                state = "code"
            else:
                result.append("\n" if current == "\n" else " ")
                index += 1
        else:
            if current == "\\" and following:
                result.extend("  " if following != "\n" else " \n")
                index += 2
            elif (state == "string" and current == '"') or (
                state == "character" and current == "'"
            ):
                result.append(" ")
                index += 1
                state = "code"
            else:
                result.append("\n" if current == "\n" else " ")
                index += 1

    return "".join(result)


def fail(message: str) -> None:
    print(f"stdlib-scan: FAIL - {message}", file=sys.stderr)
    raise SystemExit(1)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--selection", required=True)
    parser.add_argument("sources", nargs="+", type=Path)
    args = parser.parse_args()

    source_text = {}
    code_text = {}
    for path in args.sources:
        source_text[path] = path.read_text(encoding="utf-8")
        code_text[path] = strip_comments_and_literals(source_text[path])

    forbidden_pattern = re.compile(
        r"\bstd\s*::\s*(" + "|".join(FORBIDDEN_TYPES) + r")\b"
    )
    for path, code in code_text.items():
        match = forbidden_pattern.search(code)
        if match:
            line = code[: match.start()].count("\n") + 1
            fail(f"{path}:{line} uses std::{match.group(1)}")

    function_locations = []
    include_locations = []
    for path, code in code_text.items():
        for match in re.finditer(r"\bstd\s*::\s*function\b", code):
            function_locations.append((path, code[: match.start()].count("\n") + 1))
        for line_number, line in enumerate(code.splitlines(), start=1):
            if FUNCTION_INCLUDE.fullmatch(line):
                include_locations.append((path, line_number))

    if len(function_locations) != 1:
        fail(f"expected exactly one std::function use, found {len(function_locations)}")
    if len(include_locations) != 1:
        fail(f"expected exactly one #include <functional>, found {len(include_locations)}")

    function_path, function_line = function_locations[0]
    source_line = source_text[function_path].splitlines()[function_line - 1]
    if not EXPECTED_CALLBACK.fullmatch(source_line):
        fail(
            f"{function_path}:{function_line} is not the allowed "
            "MQTT_CALLBACK_SIGNATURE declaration"
        )

    print(
        f"stdlib-scan: PASS [{args.selection}] - library sources contain no "
        "forbidden std string/container types; the sole std::function is "
        "MQTT_CALLBACK_SIGNATURE"
    )


if __name__ == "__main__":
    main()
