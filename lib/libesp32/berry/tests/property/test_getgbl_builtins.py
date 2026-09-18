"""
Tests for GETGBL vs GETNGBL instruction selection with -g (named globals).

Bug: When solidifying with -g, builtins like `print` and `module` were
incorrectly emitted as GETNGBL (named global lookup by string constant)
instead of GETGBL (indexed builtin lookup).

Root cause: be_regfunc / be_regclass created "shadow" entries in the
global table for every builtin.  _global_find then returned an index
>= be_builtin_count for builtin names, causing the compiler's
singlevaraux to classify them as ETNGLOBAL instead of ETGLOBAL.

Fix: be_regfunc / be_regclass no longer create shadow global slots
for builtins.  Non-builtins (e.g. re_pattern) still get a global slot.
"""

import os
import sys
import subprocess
import tempfile
import re

import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def _run_berry_file(script, extra_args=None):
    with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
        f.write(script)
        fname = f.name
    try:
        args = [*BERRY_CMD, "-s", "-g"]
        if extra_args:
            args.extend(extra_args)
        args.append(fname)
        return subprocess.run(args, capture_output=True, text=True, timeout=15)
    finally:
        os.unlink(fname)


def _extract_instructions(stdout):
    """Extract instruction comment lines from solidify output."""
    return [
        line.strip()
        for line in stdout.splitlines()
        if "GETGBL" in line or "GETNGBL" in line
    ]


class TestBuiltinsUseGETGBL:
    """Builtins must use GETGBL (not GETNGBL) when solidified with -g."""

    def test_print_uses_getgbl(self):
        r = _run_berry_file(
            "def f() print('hi') end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        insts = _extract_instructions(r.stdout)
        assert any("GETGBL" in i and "G1" in i for i in insts), (
            f"Expected GETGBL G1 for print, got: {insts}"
        )

    def test_module_uses_getgbl(self):
        r = _run_berry_file(
            "def f() print(module) end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        insts = _extract_instructions(r.stdout)
        assert any("GETGBL" in i and "G11" in i for i in insts), (
            f"Expected GETGBL G11 for module, got: {insts}"
        )

    def test_all_builtins_use_getgbl(self):
        """Every builtin referenced in a function should emit GETGBL."""
        # Use a few representative builtins
        r = _run_berry_file(
            "def f()\n"
            "  print(str(42))\n"
            "  assert(true)\n"
            "  var x = int('1')\n"
            "  var y = real('1.5')\n"
            "  var z = size([1])\n"
            "  var t = type(x)\n"
            "end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        insts = _extract_instructions(r.stdout)
        # All should be GETGBL, none should be GETNGBL
        for inst in insts:
            assert "GETGBL" in inst and "GETNGBL" not in inst, (
                f"Builtin should use GETGBL, got: {inst}"
            )

    def test_no_constants_for_pure_builtin_function(self):
        """A function using only builtins should have 0 constants."""
        r = _run_berry_file(
            "def f() print(module) end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        assert "has constants */\n    0," in r.stdout or \
               "/* no const */" in r.stdout or \
               "0,                          /* has constants */" in r.stdout


class TestNonBuiltinsUseGETNGBL:
    """Non-builtin globals must use GETNGBL when solidified with -g."""

    def test_user_global_uses_getngbl(self):
        r = _run_berry_file(
            "animation = 42\n"
            "def f() print(animation) end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        insts = _extract_instructions(r.stdout)
        # print → GETGBL, animation → GETNGBL
        assert any("GETGBL" in i for i in insts), "print should use GETGBL"
        assert any("GETNGBL" in i for i in insts), "animation should use GETNGBL"

    def test_mixed_builtin_and_user_global(self):
        """Output should match C reference: GETGBL for builtins, GETNGBL for user globals."""
        r = _run_berry_file(
            "animation = 42\n"
            "def f() print(animation) end\n"
            "import solidify\nsolidify.dump(f, true)\n"
        )
        assert r.returncode == 0
        insts = _extract_instructions(r.stdout)
        assert len(insts) == 2
        assert "GETGBL" in insts[0] and "GETNGBL" not in insts[0]
        assert "GETNGBL" in insts[1]


class TestNonBuiltinClassRegistration:
    """Non-builtin classes (e.g. re_pattern) must still work."""

    def test_re_pattern_accessible(self):
        r = _run_berry_file(
            "import re\n"
            "var m = re.compile('abc')\n"
            "print(type(m))\n"
        )
        assert r.returncode == 0
        assert r.stdout.strip() == "instance"

    def test_re_pattern_search(self):
        r = _run_berry_file(
            "import re\n"
            "var m = re.compile('(\\\\d+)')\n"
            "print(m.search('abc123def'))\n"
        )
        assert r.returncode == 0
        assert "123" in r.stdout
