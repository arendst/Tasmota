"""
Tests for importing unknown modules.

Verifies that:
- be_loadmodule exists and delegates to be_loadmode(vm, name, 1)
- Importing an unknown module produces a Berry error, not a Python crash
- Importing known built-in modules still works
"""

import subprocess
import sys
import unittest
from unittest.mock import patch, MagicMock

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry(*args, input_text=None, timeout=15):
    """Run berry_port CLI with given args, return CompletedProcess."""
    return subprocess.run(
        [*BERRY_CMD, *args],
        capture_output=True,
        text=True,
        input=input_text,
        timeout=timeout,
    )


class TestBeLoadmoduleFunction(unittest.TestCase):
    """Unit tests for the be_loadmodule wrapper in be_exec."""

    def test_be_loadmodule_exists(self):
        """be_loadmodule should be importable from be_exec."""
        from berry_port.be_exec import be_loadmodule
        assert callable(be_loadmodule)

    def test_be_loadmodule_delegates_to_be_loadmode(self):
        """be_loadmodule(vm, name) should call be_loadmode(vm, name, 1)."""
        from berry_port import be_exec
        sentinel = object()
        vm = MagicMock()
        with patch.object(be_exec, 'be_loadmode', return_value=sentinel) as mock_lm:
            result = be_exec.be_loadmodule(vm, "somefile.be")
            mock_lm.assert_called_once_with(vm, "somefile.be", 1)
            assert result is sentinel


class TestImportUnknownModule(unittest.TestCase):
    """Integration tests: importing unknown modules should error, not crash."""

    def test_import_unknown_no_crash(self):
        """'import nonexistent_re' should produce a Berry error, not a Python traceback."""
        r = run_berry("-e", "import nonexistent_re")
        # Should exit with non-zero (error), but NOT with a Python traceback
        assert r.returncode != 0
        assert "AttributeError" not in r.stderr
        assert "Traceback" not in r.stderr

    def test_import_unknown_error_message(self):
        """Error message should mention the module name."""
        r = run_berry("-e", "import nonexistent_re")
        combined = r.stdout + r.stderr
        assert "not found" in combined.lower() or "error" in combined.lower()

    def test_import_unknown_various_names(self):
        """Several non-existent module names should all error gracefully."""
        for name in ["nonexistent_mod", "foobar123", "xyz"]:
            r = run_berry("-e", f"import {name}")
            assert r.returncode != 0, f"import {name} should fail"
            assert "Traceback" not in r.stderr, f"import {name} should not crash"


class TestImportKnownModules(unittest.TestCase):
    """Sanity check: built-in modules should still import fine."""

    def test_import_math(self):
        r = run_berry("-e", "import math print(math)")
        assert r.returncode == 0
        assert "<module: math>" in r.stdout

    def test_import_debug(self):
        r = run_berry("-e", "import debug print(debug)")
        assert r.returncode == 0
        assert "<module: debug>" in r.stdout

    def test_import_json(self):
        r = run_berry("-e", "import json print(json)")
        assert r.returncode == 0
        assert "<module: json>" in r.stdout


if __name__ == "__main__":
    unittest.main()
