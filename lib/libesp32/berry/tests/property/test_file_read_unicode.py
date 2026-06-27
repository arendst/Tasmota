"""
Tests for file read/readline preserving raw UTF-8 byte sequences.

Berry strings store UTF-8 byte sequences as latin-1 Python strings
(one char per byte).  When reading from a file, the raw bytes must be
preserved so that multi-byte UTF-8 characters (e.g. U+00B0 '°' encoded
as \\xC2\\xB0) are not collapsed into single codepoints.

This was a regression where file.read() decoded as UTF-8 then re-stored
the result, losing leading bytes of multi-byte sequences for characters
in the latin-1 range (codepoints 128–255).
"""

import sys
import os
import tempfile
import shutil

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


# ---------------------------------------------------------------------------
# Fixtures
# ---------------------------------------------------------------------------
@pytest.fixture
def vm():
    v = be_vm_new()
    yield v
    be_vm_delete(v)


@pytest.fixture
def tmpdir():
    d = tempfile.mkdtemp()
    yield d
    shutil.rmtree(d, ignore_errors=True)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _write_utf8(tmpdir, name, content_bytes):
    """Write raw bytes to a file and return the path."""
    path = os.path.join(tmpdir, name)
    with open(path, 'wb') as f:
        f.write(content_bytes)
    return path


# ---------------------------------------------------------------------------
# file.read() — UTF-8 byte preservation
# ---------------------------------------------------------------------------
class TestFileReadUnicode:
    """file.read() must preserve raw UTF-8 byte sequences."""

    def test_degree_sign_read(self, vm, tmpdir):
        """'°C' (\\xC2\\xB0 C) must survive a file read + compile round-trip.

        The compiled function should return a string whose bytes match
        the original UTF-8 encoding (3 bytes: \\xC2\\xB0\\x43).
        """
        path = _write_utf8(tmpdir, "degree.be", "def f() return '°C' end\n".encode('utf-8'))
        # Use a global 'f' so compile(s)+c() can redefine it, then call it.
        code = f"""
f = open('{path}')
s = f.read()
f.close()
c = compile(s)
c()
assert(size(f()) == 3)
"""
        assert be_dostring(vm, code) == BE_OK

    def test_degree_sign_solidify_identifier(self, vm, tmpdir):
        """Solidified constant for '°C' must be _XC2_XB0C, not _XB0C."""
        path = _write_utf8(tmpdir, "degree.be", "def f() return '°C' end\n".encode('utf-8'))

        import io
        from berry_port.be_exec import be_loadbuffer
        from berry_port.be_vm import be_vm_new as _new, be_dofunc
        from berry_port.be_object import var_toobj

        vm2 = _new()
        # Compile and run the file-reading script
        src = f"var fh=open('{path}') var s=fh.read() fh.close() var c=compile(s) c()"
        rc = be_loadbuffer(vm2, "input", src, len(src))
        assert rc == 0
        be_dofunc(vm2, vm2.top_idx - 1, 0)

        # Now find 'f' in the global scope and solidify it
        from berry_port.be_solidifylib import m_solidify_closure
        from berry_port import be_api
        be_api.be_getglobal(vm2, "f")
        v = vm2.stack[vm2.top_idx - 1]
        clo = var_toobj(v)

        fout = io.StringIO()
        m_solidify_closure(vm2, False, clo, None, fout)
        output = fout.getvalue()

        assert '_XC2_XB0C' in output, (
            f"Expected _XC2_XB0C in solidified output but got: {output}"
        )
        # Ensure the truncated form (without leading \xC2) is NOT present
        # as a standalone identifier. _XB0C appears as substring of _XC2_XB0C
        # so we check that removing the correct form leaves no stray _XB0C.
        stripped = output.replace('_XC2_XB0C', '')
        assert '_XB0C' not in stripped, (
            "Found truncated _XB0C (missing leading \\xC2 byte)"
        )

    def test_multibyte_chars_in_latin1_range(self, vm, tmpdir):
        """Characters U+0080–U+00FF have 2-byte UTF-8; both bytes must survive."""
        # U+00E9 'é' = \xC3\xA9, U+00FC 'ü' = \xC3\xBC
        path = _write_utf8(tmpdir, "accents.txt", "éü".encode('utf-8'))
        code = f"""
var f = open('{path}')
var s = f.read()
f.close()
assert(s == 'éü')
"""
        assert be_dostring(vm, code) == BE_OK

    def test_ascii_unaffected(self, vm, tmpdir):
        """Pure ASCII content must still work correctly."""
        path = _write_utf8(tmpdir, "ascii.txt", b"hello world")
        code = f"""
var f = open('{path}')
var s = f.read()
f.close()
assert(s == 'hello world')
"""
        assert be_dostring(vm, code) == BE_OK

    def test_mixed_ascii_and_multibyte(self, vm, tmpdir):
        """Mixed ASCII and multi-byte UTF-8 must all be preserved."""
        # "Temp: 25°C" — the ° is \xC2\xB0
        path = _write_utf8(tmpdir, "mixed.txt", "Temp: 25°C".encode('utf-8'))
        code = f"""
var f = open('{path}')
var s = f.read()
f.close()
assert(s == 'Temp: 25°C')
"""
        assert be_dostring(vm, code) == BE_OK


# ---------------------------------------------------------------------------
# file.readline() — UTF-8 byte preservation
# ---------------------------------------------------------------------------
class TestFileReadlineUnicode:
    """file.readline() must also preserve raw UTF-8 byte sequences."""

    def test_readline_degree_sign(self, vm, tmpdir):
        """readline() must preserve multi-byte UTF-8 chars."""
        path = _write_utf8(tmpdir, "lines.txt", "25°C\nok\n".encode('utf-8'))
        code = f"""
var f = open('{path}')
var line = f.readline()
f.close()
assert(line == '25°C\\n')
"""
        assert be_dostring(vm, code) == BE_OK

    def test_readline_multiple_multibyte(self, vm, tmpdir):
        """Multiple lines with multi-byte chars must all be correct."""
        content = "café\nüber\n".encode('utf-8')
        path = _write_utf8(tmpdir, "multi.txt", content)
        code = f"""
var f = open('{path}')
var l1 = f.readline()
var l2 = f.readline()
f.close()
assert(l1 == 'café\\n')
assert(l2 == 'über\\n')
"""
        assert be_dostring(vm, code) == BE_OK
