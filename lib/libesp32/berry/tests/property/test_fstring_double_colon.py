"""
Tests for f-string double-colon (::) syntax support.

F-strings now accept '::' inside expressions as an escaped colon, enabling
ternary expressions like:  f"{cond ? val_true :: val_false}"

Previously, a single ':' inside an f-string expression was always treated
as the separator between the expression and the format specifier.  With this
change, '::' is treated as a literal ':' within the expression, while a
single ':' still introduces the format specifier.

Tests cover:
  - Basic ternary with :: (true/false branches)
  - :: combined with format specifiers (single :)
  - :: with string values in ternary
  - :: with self-documenting = syntax
  - Existing single-colon format specifiers still work
  - Regular f-string features remain unaffected
"""

import os
import sys
import subprocess
import tempfile

import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry(script_text, timeout=15):
    """Write script to temp file, run with berry_port, return CompletedProcess."""
    with tempfile.NamedTemporaryFile(
        mode="w", suffix=".be", delete=False
    ) as f:
        f.write(script_text)
        fname = f.name
    try:
        return subprocess.run(
            [*BERRY_CMD, fname],
            capture_output=True, text=True, timeout=timeout,
        )
    finally:
        os.unlink(fname)


class TestFstringDoubleColon:
    """Double-colon :: in f-string expressions acts as literal colon."""

    def test_ternary_true_branch(self):
        r = run_berry('assert(f"{true ? 1 :: 0}" == "1")\nprint("OK")\n')
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_ternary_false_branch(self):
        r = run_berry('assert(f"{false ? 1 :: 0}" == "0")\nprint("OK")\n')
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_ternary_with_format_spec(self):
        """:: in expression + single : for format specifier."""
        r = run_berry(
            'assert(f"{true ? 100 :: 0:%05i}" == "00100")\n'
            'assert(f"{false ? 100 :: 0:%05i}" == "00000")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_ternary_with_string_values(self):
        r = run_berry(
            "assert(f\"{true ? 'yes' :: 'no'}\" == \"yes\")\n"
            "assert(f\"{false ? 'yes' :: 'no'}\" == \"no\")\n"
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_ternary_with_int_format(self):
        r = run_berry(
            'assert(f"{true ? 1 :: 0:%i}" == "1")\n'
            'assert(f"{false ? 1 :: 0:%i}" == "0")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_self_documenting_with_double_colon(self):
        """The = syntax should preserve :: in the label text."""
        r = run_berry(
            'var s = f"{true ? 1 :: 0=}"\n'
            'assert(s == "true ? 1 :: 0=1")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout


class TestFstringSingleColonStillWorks:
    """Existing single-colon format specifier behavior is preserved."""

    def test_format_spec_integer(self):
        r = run_berry('assert(f"{42:%05i}" == "00042")\nprint("OK")\n')
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_format_spec_string(self):
        r = run_berry('assert(f"{42:%s}" == "42")\nprint("OK")\n')
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_self_documenting_equals(self):
        r = run_berry(
            'var x = 42\n'
            'assert(f"{x=}" == "x=42")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_simple_expression(self):
        r = run_berry(
            'var a = 10\n'
            'assert(f"val={a}" == "val=10")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_multiple_expressions(self):
        r = run_berry(
            'var a = 1 var b = 2\n'
            'assert(f"{a}+{b}={a+b}" == "1+2=3")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout

    def test_escaped_braces(self):
        r = run_berry(
            'assert(f"{{hello}}" == "{hello}")\n'
            'print("OK")\n'
        )
        assert r.returncode == 0, r.stderr
        assert "OK" in r.stdout
