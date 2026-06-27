"""
Regression: instance binary/unary operator with a constant operand

Two related bugs in the Python port that surfaced when running
tests/bytes.be under ``python3 -m berry_port -s -g``:

1. ins_binop dropped the constant table
   -----------------------------------
   For ``instance OP something`` expressions, the VM dispatches to
   ``ins_binop`` so the instance's overload method (e.g. ``+``) can be
   invoked. In the Python port the arithmetic helpers forwarded the
   call like this:

       elif var_isinstance(a):
           ins_binop(vm, "+", reg_idx, 0, ins)   # ktab → 0 (wrong)

   The C version passes the current frame's constant table pointer so
   that ``ins_binop`` can resolve a K-operand (e.g. the literal ``'01'``
   in ``b + '01'``) against the constants. Passing ``0`` meant the K
   lookup dereferenced the wrong memory and the other operand arrived
   at the overload as **nil**, producing ``type_error: operand must be
   bytes``.

   The fix threads the real ``ktab`` through every ``ins_binop`` call
   and updates ``ins_binop`` itself to use ``_resolve_rkb/_resolve_rkc``
   (which already know how to index a ktab list vs. the stack).

2. bytes fixed-size init skipped writing .size back
   ------------------------------------------------
   ``m_init`` initialises a local ``buf_impl`` whose ``prev_*`` fields
   act as "invalid" sentinels, so that every computed field is written
   back by ``m_write_attributes`` when the constructor finishes. The
   Python port used ``prev_size=-1`` — but ``-1`` is also
   ``BYTES_SIZE_FIXED``. For ``bytes(-1)`` (and any other fixed-size
   constructor) ``new_size`` ended up equal to ``prev_size``, so the
   ``.size`` member on the instance stayed at ``nil``. Later calls
   through ``m_read_attributes`` then hit
   ``TypeError: '<' not supported between instances of 'NoneType'
   and 'int'``.

   The fix sets ``prev_size=0`` to match the C initializer
   ``{ 0, 0, NULL, 0, -1, NULL, bfalse, bfalse, bfalse }`` so the
   invalid sentinel is ``prev_len=-1`` as originally intended.

These tests exercise the two fixes end-to-end by compiling and running
small Berry snippets on the Python port.
"""

import io
import os
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.berry import be_vm_new, be_dostring


def _run(source):
    """Execute a Berry snippet on the Python port, capturing stdout."""
    vm = be_vm_new()
    old_stdout = sys.stdout
    sys.stdout = captured = io.StringIO()
    try:
        rc = be_dostring(vm, source)
    finally:
        sys.stdout = old_stdout
    return rc, captured.getvalue()


# ---------------------------------------------------------------------------
# Bug 1: ins_binop must forward ktab so instance OP K-constant works
# ---------------------------------------------------------------------------

class TestInsBinopKConstant:
    """Instance operator overloads receive the constant operand, not nil."""

    def test_bytes_plus_string_literal(self):
        """bytes + literal string: uses m_merge with a K string operand."""
        rc, out = _run(
            "var b = bytes('AA') + '01'\n"
            "print(b)\n"
        )
        assert rc == 0
        assert out.strip() == "bytes('AA3031')"

    def test_bytes_plus_empty_string_literal(self):
        rc, out = _run(
            "var b = bytes('AA') + ''\n"
            "print(b)\n"
        )
        assert rc == 0
        assert out.strip() == "bytes('AA')"

    def test_bytes_connect_string_literal(self):
        """`..` mirrors the same dispatch path through object_binop."""
        rc, out = _run(
            "var b = bytes('AA')\n"
            "b .. '01'\n"
            "print(b)\n"
        )
        assert rc == 0
        assert out.strip() == "bytes('AA3031')"

    def test_bytes_equal_with_literal(self):
        """== on an instance and a literal must see the literal, not nil."""
        rc, out = _run(
            "print(bytes('1122') == bytes('1122'))\n"
            "print(bytes('1122') == bytes('3344'))\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == ["true", "false"]

    def test_list_equality_with_literal(self):
        """list overloads ``==`` and is compared against a K-constant list."""
        rc, out = _run(
            "print([1, 2, 3] == [1, 2, 3])\n"
            "print([1, 2] == [1, 2, 3])\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == ["true", "false"]


# ---------------------------------------------------------------------------
# Bug 2: bytes(-1) / fixed-size bytes must initialise .size
# ---------------------------------------------------------------------------

class TestBytesFixedSizeInit:
    """Fixed-size bytes constructors must write .size back to the instance."""

    def test_bytes_minus_one(self):
        """bytes(-1) → single zero byte, size 1, len 1."""
        rc, out = _run(
            "var b = bytes(-1)\n"
            "print(b)\n"
            "print(b.size())\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == ["bytes('00')", "1"]

    def test_bytes_minus_four(self):
        """bytes(-4) → four zero bytes, fixed size."""
        rc, out = _run(
            "var b = bytes(-4)\n"
            "print(b)\n"
            "print(b.size())\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == ["bytes('00000000')", "4"]

    def test_bytes_default_then_tostring(self):
        """bytes() and bytes(0) must not leave .size at nil either."""
        rc, out = _run(
            "print(bytes())\n"
            "print(bytes(0))\n"
            "print(bytes(1))\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == [
            "bytes('')",
            "bytes('')",
            "bytes('')",
        ]

    def test_fixed_bytes_resize_rejected(self):
        """A fixed-size bytes still reports its size correctly after use."""
        rc, out = _run(
            "var b = bytes(-3)\n"
            "b[0] = 0xAA\n"
            "b[1] = 0xBB\n"
            "b[2] = 0xCC\n"
            "print(b)\n"
            "print(b.size())\n"
        )
        assert rc == 0
        assert out.strip().splitlines() == ["bytes('AABBCC')", "3"]


# ---------------------------------------------------------------------------
# End-to-end: the whole tests/bytes.be suite must run clean under -s -g
# ---------------------------------------------------------------------------

_BYTES_TEST_FILE = os.path.join(
    os.path.dirname(__file__), '..', 'bytes.be'
)


@pytest.mark.skipif(
    not os.path.isfile(_BYTES_TEST_FILE),
    reason="tests/bytes.be not present",
)
def test_tests_bytes_be_runs_clean():
    """`python3 -m berry_port -s -g tests/bytes.be` must succeed.

    This is the exact invocation that originally surfaced both bugs.
    """
    import subprocess
    root = os.path.join(os.path.dirname(__file__), '..', '..')
    result = subprocess.run(
        [sys.executable, '-m', 'berry_port', '-s', '-g', _BYTES_TEST_FILE],
        capture_output=True, text=True, timeout=60, cwd=root,
    )
    assert result.returncode == 0, (
        f"bytes.be failed: rc={result.returncode}\n"
        f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
    )
    # No assertion errors or Python tracebacks should appear.
    combined = result.stdout + result.stderr
    assert "Traceback" not in combined, combined
    assert "assert_failed" not in combined, combined
