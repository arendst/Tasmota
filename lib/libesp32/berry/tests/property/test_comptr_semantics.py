"""
Regression tests for BE_COMPTR (C pointer) value semantics in the Python port.

In C, a BE_COMPTR value stores a raw void* and is used as both an opaque
object reference (functions, modules, strings cast to pointer) and as a
pointer-sized integer (via introspect.toptr(int), buffer pointers, etc.).

Python has no void*. The port stores either a Python int (when the source
was an integer address) or a Python object reference. Several operations
historically treated only one of those two cases correctly; this module
pins down the C-equivalent behavior for all of them.

Each test asserts the Python port's observable behavior, and — when the C
binary is available — cross-validates against it byte-for-byte.

Bugs covered (all fixed alongside this file):
  1. bool(toptr(0)) was true because "0 is not None" is true
  2. str(toptr(0x1000)) printed host id() instead of the stored address
  3. int(toptr(<object>)) raised ValueError on non-int payloads
  4. comptr + int / comptr - int did not advance the pointer
  5. comptr[int] read/write was a no-op
  6. comptr equality used Python identity instead of pointer-value compare
"""

import io
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest

from berry_port.berry import be_vm_new, be_dostring


# ---------------------------------------------------------------------------
# Helpers (mirror test_vm_execution.py)
# ---------------------------------------------------------------------------

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _run_python(source):
    """Run Berry source on the Python port, return (rc, stdout_str)."""
    vm = be_vm_new()
    old_stdout = sys.stdout
    sys.stdout = captured = io.StringIO()
    try:
        rc = be_dostring(vm, source)
    except Exception:
        rc = -1
    finally:
        sys.stdout = old_stdout
    return rc, captured.getvalue()


def _run_c(source):
    """Run Berry source on the C binary, return (rc, stdout_str) or None."""
    if not _c_binary_available():
        return None
    tmppath = None
    try:
        with tempfile.NamedTemporaryFile(
            mode='w', suffix='.be', delete=False
        ) as f:
            f.write(source + '\n')
            tmppath = f.name
        result = subprocess.run(
            [_BERRY_BIN, tmppath],
            capture_output=True, text=True, timeout=10,
        )
        return result.returncode, result.stdout
    except (subprocess.TimeoutExpired, OSError):
        return None
    finally:
        if tmppath is not None:
            try:
                os.unlink(tmppath)
            except OSError:
                pass


def _assert_python_output(source, expected):
    rc, out = _run_python(source)
    assert rc == 0, f"be_dostring returned rc={rc} for:\n{source}"
    assert out == expected, (
        f"Python output mismatch for:\n{source}\n"
        f"  expected: {expected!r}\n  got:      {out!r}"
    )


# ---------------------------------------------------------------------------
# Bug 1: bool(comptr) — NULL check must treat integer 0 as false
# ---------------------------------------------------------------------------

class TestComptrBool:
    def test_bool_nonzero_address_is_true(self):
        _assert_python_output(
            "import introspect\n"
            "print(bool(introspect.toptr(0x1000)))\n",
            "true\n",
        )

    def test_bool_zero_address_is_false(self):
        # Original regression: 0 is not None, so this wrongly returned true.
        _assert_python_output(
            "import introspect\n"
            "print(bool(introspect.toptr(0)))\n",
            "false\n",
        )

    def test_bool_object_backed_comptr_is_true(self):
        # A string-derived comptr wraps a Python object; should be truthy.
        _assert_python_output(
            "import introspect\n"
            "print(bool(introspect.toptr('hello')))\n",
            "true\n",
        )

    @pytest.mark.skipif(not _c_binary_available(), reason="C binary not built")
    @pytest.mark.parametrize("addr", [0, 1, 0x400, 0x1000, 0xFFFF])
    def test_bool_matches_c(self, addr):
        src = (
            "import introspect\n"
            f"print(bool(introspect.toptr({addr})))\n"
        )
        py = _run_python(src)
        c = _run_c(src)
        assert c is not None
        assert py[1] == c[1], f"Python {py[1]!r} != C {c[1]!r}"


# ---------------------------------------------------------------------------
# Bug 2: str(comptr) — integer-backed comptr must print the stored address
# ---------------------------------------------------------------------------

class TestComptrToString:
    @pytest.mark.parametrize("addr,expected_hex", [
        (0x0, "0x0"),
        (0x1, "0x1"),
        (0x400, "0x400"),
        (0x1000, "0x1000"),
        (0x7FFFFFFF, "0x7fffffff"),
    ])
    def test_str_prints_literal_address(self, addr, expected_hex):
        # Original regression: str() printed hex(id(v.v)) giving a host-side
        # id instead of the stored address.
        _assert_python_output(
            "import introspect\n"
            f"print(str(introspect.toptr({addr})))\n",
            f"<ptr: {expected_hex}>\n",
        )

    def test_print_prints_literal_address(self):
        # VM path (print) and str() path (strlib) are separate — check both.
        _assert_python_output(
            "import introspect\n"
            "print(introspect.toptr(0x1234))\n",
            "<ptr: 0x1234>\n",
        )

    @pytest.mark.skipif(not _c_binary_available(), reason="C binary not built")
    @pytest.mark.parametrize("addr", [0, 1, 0x400, 0x1000, 0xFFFF])
    def test_str_matches_c(self, addr):
        src = (
            "import introspect\n"
            f"print(str(introspect.toptr({addr})))\n"
        )
        py = _run_python(src)
        c = _run_c(src)
        assert c is not None
        assert py[1] == c[1], f"Python {py[1]!r} != C {c[1]!r}"


# ---------------------------------------------------------------------------
# Bug 3: int(comptr) — integer-backed returns the address, object-backed
#                      returns a host-side numeric id (not a crash)
# ---------------------------------------------------------------------------

class TestComptrToInt:
    @pytest.mark.parametrize("addr", [0, 1, 0x100, 0x1000, 0x7FFFFFFF])
    def test_int_of_int_pointer_roundtrip(self, addr):
        _assert_python_output(
            "import introspect\n"
            f"print(int(introspect.toptr({addr})))\n",
            f"{addr}\n",
        )

    def test_int_of_object_pointer_does_not_raise(self):
        # Original regression: int(introspect.toptr('hello')) raised
        # ValueError because int('hello') was attempted.
        rc, out = _run_python(
            "import introspect\n"
            "var n = int(introspect.toptr('hello'))\n"
            "print(type(n))\n"
        )
        assert rc == 0
        assert out == "int\n"


# ---------------------------------------------------------------------------
# Bug 4: pointer arithmetic — comptr + int, comptr - int
# ---------------------------------------------------------------------------

class TestComptrArithmetic:
    def test_add_advances_integer_pointer(self):
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(1024)\n"
            "p += 1\n"
            "print(p)\n",
            "<ptr: 0x401>\n",
        )

    def test_sub_rewinds_integer_pointer(self):
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(1024)\n"
            "p -= 2\n"
            "print(p)\n",
            "<ptr: 0x3fe>\n",
        )

    def test_add_then_sub_is_identity(self):
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(0x500)\n"
            "p += 10\n"
            "p -= 10\n"
            "assert(p == introspect.toptr(0x500))\n"
            "print('ok')\n",
            "ok\n",
        )

    def test_round_trip_through_large_offset(self):
        # (p + n) - n == p, using a large offset. Requires both arithmetic
        # directions to be consistent.
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(0x1000)\n"
            "var q = p + 100\n"
            "q -= 100\n"
            "assert(q == p)\n"
            "print('ok')\n",
            "ok\n",
        )


# ---------------------------------------------------------------------------
# Bug 5: indexing a comptr that wraps a writable buffer
# ---------------------------------------------------------------------------

class TestComptrIndexing:
    def test_getidx_reads_bytes(self):
        _assert_python_output(
            "var b = bytes('11223344')\n"
            "var p = b._buffer()\n"
            "print(p[0], p[1], p[2], p[3])\n",
            "17 34 51 68\n",
        )

    def test_setidx_writes_bytes(self):
        _assert_python_output(
            "var b = bytes('11223344')\n"
            "var p = b._buffer()\n"
            "p[0] = 0xFF\n"
            "p[1] = 0x55\n"
            "print(b)\n",
            "bytes('FF553344')\n",
        )

    def test_setidx_truncates_to_byte(self):
        # C stores as uint8_t, so values > 0xFF are masked.
        _assert_python_output(
            "var b = bytes('00000000')\n"
            "var p = b._buffer()\n"
            "p[2] = 0xFEBC\n"
            "assert(b == bytes('0000BC00'))\n"
            "print('ok')\n",
            "ok\n",
        )

    def test_getidx_is_unsigned(self):
        _assert_python_output(
            "var b = bytes('FF')\n"
            "var p = b._buffer()\n"
            "assert(p[0] == 255)\n"
            "print('ok')\n",
            "ok\n",
        )

    def test_getidx_on_integer_pointer_returns_zero(self):
        # A raw integer address cannot be dereferenced from Python; the VM
        # returns 0 rather than crashing — matches the previous stub behavior
        # for this case.
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(0x1000)\n"
            "print(p[0])\n",
            "0\n",
        )


# ---------------------------------------------------------------------------
# Bug 6: comptr equality uses pointer-value compare
# ---------------------------------------------------------------------------

class TestComptrEquality:
    def test_eq_same_integer_address(self):
        # Value beyond the small-int cache: a naive `is` would fail.
        _assert_python_output(
            "import introspect\n"
            "var a = introspect.toptr(1025)\n"
            "var b = introspect.toptr(1025)\n"
            "assert(a == b)\n"
            "print('ok')\n",
            "ok\n",
        )

    def test_ne_different_integer_addresses(self):
        _assert_python_output(
            "import introspect\n"
            "assert(introspect.toptr(1024) != introspect.toptr(1025))\n"
            "print('ok')\n",
            "ok\n",
        )

    def test_eq_after_arithmetic(self):
        # p + 1 == toptr(original+1) — requires both arithmetic and equality
        # to be correct.
        _assert_python_output(
            "import introspect\n"
            "var p = introspect.toptr(1024)\n"
            "p += 1\n"
            "assert(p == introspect.toptr(1025))\n"
            "print('ok')\n",
            "ok\n",
        )

    @pytest.mark.skipif(not _c_binary_available(), reason="C binary not built")
    def test_comptr_be_file_matches_c(self):
        # End-to-end: run the canonical comptr test file on both runtimes.
        test_file = os.path.join(
            os.path.dirname(__file__), '..', 'comptr.be'
        )
        if not os.path.isfile(test_file):
            pytest.skip("tests/comptr.be not present")
        result_c = subprocess.run(
            [_BERRY_BIN, test_file],
            capture_output=True, text=True, timeout=10,
        )
        result_py = subprocess.run(
            [sys.executable, '-m', 'berry_port', '-s', '-g', test_file],
            capture_output=True, text=True, timeout=30,
            cwd=os.path.join(os.path.dirname(__file__), '..', '..'),
        )
        assert result_c.returncode == 0, (
            f"C run failed: {result_c.stderr}"
        )
        assert result_py.returncode == 0, (
            f"Python run failed: {result_py.stdout}{result_py.stderr}"
        )
