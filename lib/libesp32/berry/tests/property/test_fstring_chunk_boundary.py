"""
Tests for f-string parsing across file chunk boundaries and string.format().

Bug 1 (be_lexer.py — _scan_f_string reader restoration):
  The lexer reads files in 256-byte chunks (FILE_BUFFER_SIZE).  When an
  f-string is parsed, _scan_f_string transpiles it into a format() call
  and temporarily replaces the reader.  On restoration, the saved cursor
  character was served as a separate 1-char chunk.  _next() then overwrote
  lr.s / lr.len with that 1-char return, losing the remaining file buffer.
  Identifiers immediately after the f-string were truncated (e.g.
  'prefix_dir' became 'efix_dir').

Bug 2 (be_strlib.py — _pushstr / be_pushvfstr):
  be_incrtop() returns a stack *index* (int), not a bvalue.  _pushstr and
  two branches in be_pushvfstr used the return value directly as a bvalue,
  causing 'int object has no attribute v' when string.format() was called
  at runtime.
"""

import os
import sys
import subprocess
import tempfile

import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry_file(script_text, extra_args=None, timeout=15):
    """Write script_text to a temp .be file, run it, return CompletedProcess."""
    with tempfile.NamedTemporaryFile(
        mode="w", suffix=".be", delete=False
    ) as f:
        f.write(script_text)
        fname = f.name
    try:
        args = [*BERRY_CMD, "-s", "-g"]
        if extra_args:
            args.extend(extra_args)
        args.append(fname)
        return subprocess.run(
            args, capture_output=True, text=True, timeout=timeout
        )
    finally:
        os.unlink(fname)


def _pad(n):
    """Return exactly *n* bytes of Berry comment padding."""
    lines = []
    remaining = n
    while remaining > 0:
        if remaining >= 4:
            line_len = min(remaining, 76)
            lines.append("# " + "A" * (line_len - 3) + "\n")
            remaining -= line_len
        elif remaining == 3:
            lines.append("# \n"); remaining -= 3
        elif remaining == 2:
            lines.append("#\n"); remaining -= 2
        else:
            lines.append("\n"); remaining -= 1
    return "".join(lines)


# ── Bug 1: f-string reader restoration across chunk boundaries ──────────

class TestFstringChunkBoundary:
    """An f-string followed by an identifier must parse correctly
    regardless of where the f-string falls relative to 256-byte
    chunk boundaries."""

    # The code under test: declare a variable, use it inside an f-string,
    # then reference it again on the very next statement.  Before the fix
    # the second reference was truncated.
    CODE_TEMPLATE = (
        "var prefix_dir = 'src/'\n"
        "var fname = 'test.be'\n"
        "var msg = f'Skipping: {{fname}}'\n"
        "var full = prefix_dir + fname\n"
        "assert(full == 'src/test.be')\n"
        "print('OK')\n"
    )

    @pytest.mark.parametrize(
        "pad_size",
        # Sweep across several 256-byte boundaries so the f-string
        # lands at every interesting alignment.
        list(range(200, 1400, 50)),
        ids=lambda n: f"pad{n}",
    )
    def test_identifier_after_fstring_at_boundary(self, pad_size):
        script = _pad(pad_size) + self.CODE_TEMPLATE
        r = run_berry_file(script)
        assert r.returncode == 0, (
            f"pad={pad_size} total={len(script)}: {r.stderr or r.stdout}"
        )
        assert "OK" in r.stdout

    def test_exact_boundary_1280(self):
        """Regression: the original bug manifested at byte 1280 (=256*5)
        where 'prefix_dir' started at offset 1278 and 'pr' straddled
        the chunk boundary."""
        # Place the f-string so the identifier after it starts near 1278
        code = self.CODE_TEMPLATE
        # We want total padding ~ 1278 - offset_of_prefix_dir_in_code
        # The 4th line "var full = prefix_dir" starts ~70 bytes into code
        target = 1278 - 70
        script = _pad(target) + code
        r = run_berry_file(script)
        assert r.returncode == 0, r.stderr or r.stdout

    def test_multiple_fstrings_across_boundaries(self):
        """Multiple f-strings in the same file should all parse correctly."""
        parts = []
        for i in range(6):
            parts.append(_pad(200))
            parts.append(
                f"var v{i} = {i}\n"
                f"var s{i} = f'value={{v{i}}}'\n"
            )
        parts.append("assert(v5 == 5)\nprint('OK')\n")
        script = "".join(parts)
        r = run_berry_file(script)
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_fstring_with_empty_remaining_buffer(self):
        """Edge case: f-string consumes the entire remaining chunk,
        leaving zero bytes in the saved reader buffer."""
        # Use a long f-string to exhaust the buffer
        long_expr = "a" * 50
        code = (
            f"var {'a' * 50} = 42\n"
            f"var s = f'result={{{'a' * 50}}}'\n"
            "print('OK')\n"
        )
        # Pad to push near a boundary
        script = _pad(256 * 4) + code
        r = run_berry_file(script)
        assert r.returncode == 0, r.stderr or r.stdout


# ── Bug 2: string.format() _pushstr / be_pushvfstr stack fix ────────────

class TestStringFormatPushstr:
    """string.format() must not crash with 'int has no attribute v'.
    This was caused by _pushstr treating be_incrtop()'s return (an index)
    as a bvalue."""

    def test_format_simple_string(self):
        r = run_berry_file(
            "import string\n"
            "var s = string.format('%s', 'hello')\n"
            "assert(s == 'hello')\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_format_integer(self):
        r = run_berry_file(
            "import string\n"
            "var s = string.format('%d items', 42)\n"
            "assert(s == '42 items')\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_format_real(self):
        r = run_berry_file(
            "import string\n"
            "var s = string.format('pi=%.2f', 3.14)\n"
            "assert(s == 'pi=3.14')\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_format_multiple_args(self):
        r = run_berry_file(
            "import string\n"
            "var s = string.format('%s=%d (%.1f)', 'x', 10, 2.5)\n"
            "assert(s == 'x=10 (2.5)')\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_format_percent_literal(self):
        r = run_berry_file(
            "import string\n"
            "var s = string.format('100%%')\n"
            "assert(s == '100%')\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout

    def test_fstring_with_quotes_in_format(self):
        """The original failing line: f-string with single quotes inside."""
        r = run_berry_file(
            "var prefix_out = 'build'\n"
            "print(f\"# Output directory '{prefix_out}' cleaned\")\n"
            "print('OK')\n"
        )
        assert r.returncode == 0, r.stderr or r.stdout
        assert "OK" in r.stdout
        assert "# Output directory 'build' cleaned" in r.stdout
