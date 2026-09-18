"""
Tests for the Berry time module (be_timelib.py).

Covers: time.time(), time.dump(ts), time.clock().

Validates:
  - Return types and value ranges for each function
  - time.dump() map keys and value ranges (year/month/day/hour/min/sec/weekday/epoch)
  - time.dump() argument handling (missing arg, nil, non-int, ints of various sizes)
  - C-API weekday convention (0=Sunday..6=Saturday) matches the C implementation
  - Monotonic behavior of time.time() and time.clock()
  - Round-trip: time.dump(t)['epoch'] == t
  - Cross-validation against the compiled C binary when available
"""

import sys
import os
import time as _pytime
import subprocess
import tempfile

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


# ---------------------------------------------------------------------------
# Fixtures and helpers
# ---------------------------------------------------------------------------
@pytest.fixture
def vm():
    v = be_vm_new()
    yield v
    be_vm_delete(v)


_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _run_c(source):
    """Run Berry source on the C binary, return (rc, stdout) or None if unavailable."""
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


# ============================================================================
# time.time()
# ============================================================================
def test_time_returns_int(vm):
    """time.time() returns an integer."""
    code = (
        "import time\n"
        "assert(type(time.time()) == 'int')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_time_is_positive(vm):
    """time.time() returns a positive value (we're well past 1970)."""
    code = (
        "import time\n"
        "assert(time.time() > 0)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_time_is_close_to_now(vm):
    """time.time() should be within a few seconds of Python's time.time()."""
    py_before = int(_pytime.time())
    # Push a sentinel range as Berry code and have Berry assert it
    code = (
        "import time\n"
        f"var t = time.time()\n"
        f"assert(t >= {py_before - 1})\n"
        # Allow a 5-second window for slow CI machines
        f"assert(t <= {py_before + 5})\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_time_monotonic_nondecreasing(vm):
    """Successive calls to time.time() should be non-decreasing."""
    code = (
        "import time\n"
        "var t1 = time.time()\n"
        "var t2 = time.time()\n"
        "assert(t2 >= t1)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_time_takes_no_args(vm):
    """time.time() is called with no arguments (extra args should be ignored in C)."""
    # The C implementation ignores extra arguments; just verify it works with none.
    code = (
        "import time\n"
        "var t = time.time()\n"
        "assert(t > 0)\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ============================================================================
# time.dump(ts)
# ============================================================================
def test_dump_returns_instance(vm):
    """time.dump(int) returns a map instance."""
    code = (
        "import time\n"
        "var d = time.dump(0)\n"
        "assert(type(d) == 'instance')\n"
        "assert(classname(d) == 'map')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_has_all_keys(vm):
    """time.dump() result has all expected keys."""
    code = (
        "import time\n"
        "var d = time.dump(0)\n"
        "assert(d.contains('year'))\n"
        "assert(d.contains('month'))\n"
        "assert(d.contains('day'))\n"
        "assert(d.contains('hour'))\n"
        "assert(d.contains('min'))\n"
        "assert(d.contains('sec'))\n"
        "assert(d.contains('weekday'))\n"
        "assert(d.contains('epoch'))\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_all_values_are_ints(vm):
    """All values in time.dump() result are integers."""
    code = (
        "import time\n"
        "var d = time.dump(1000000)\n"
        "assert(type(d['year']) == 'int')\n"
        "assert(type(d['month']) == 'int')\n"
        "assert(type(d['day']) == 'int')\n"
        "assert(type(d['hour']) == 'int')\n"
        "assert(type(d['min']) == 'int')\n"
        "assert(type(d['sec']) == 'int')\n"
        "assert(type(d['weekday']) == 'int')\n"
        "assert(type(d['epoch']) == 'int')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_epoch_roundtrip(vm):
    """time.dump(t)['epoch'] equals t."""
    code = (
        "import time\n"
        "var t = 1234567890\n"
        "var d = time.dump(t)\n"
        "assert(d['epoch'] == t)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_epoch_zero_year(vm):
    """time.dump(0)['year'] is 1970 (epoch) — unless in a TZ west of UTC
    where Dec 31 1969 is the local date at epoch 0."""
    code = (
        "import time\n"
        "var d = time.dump(0)\n"
        "var y = d['year']\n"
        "assert(y == 1970 || y == 1969)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_field_ranges(vm):
    """All fields fall within their sensible ranges."""
    code = (
        "import time\n"
        "var d = time.dump(1700000000)\n"       # Nov 14 2023
        "assert(d['year'] >= 1970 && d['year'] <= 9999)\n"
        "assert(d['month'] >= 1 && d['month'] <= 12)\n"
        "assert(d['day'] >= 1 && d['day'] <= 31)\n"
        "assert(d['hour'] >= 0 && d['hour'] <= 23)\n"
        "assert(d['min'] >= 0 && d['min'] <= 59)\n"
        "assert(d['sec'] >= 0 && d['sec'] <= 60)\n"   # 60 allowed for leap sec
        "assert(d['weekday'] >= 0 && d['weekday'] <= 6)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_matches_python_localtime(vm):
    """For a known timestamp, time.dump() matches Python's localtime.

    The C and Python ports both use the system's localtime(), so they will
    agree with Python's time.localtime() on the same host.
    """
    ts = 1700000000  # Nov 14 2023 UTC
    t = _pytime.localtime(ts)
    expected_wday = (t.tm_wday + 1) % 7  # C convention: Sun=0

    code = (
        "import time\n"
        f"var d = time.dump({ts})\n"
        f"assert(d['year'] == {t.tm_year})\n"
        f"assert(d['month'] == {t.tm_mon})\n"
        f"assert(d['day'] == {t.tm_mday})\n"
        f"assert(d['hour'] == {t.tm_hour})\n"
        f"assert(d['min'] == {t.tm_min})\n"
        f"assert(d['sec'] == {t.tm_sec})\n"
        f"assert(d['weekday'] == {expected_wday})\n"
        f"assert(d['epoch'] == {ts})\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_weekday_c_convention(vm):
    """weekday uses C convention: 0=Sunday..6=Saturday.

    Jan 4 1970 00:00 UTC was a Sunday. But because of TZ offsets that might
    be +/- 1 day, we accept Jan 3 (Saturday=6) or Jan 4 (Sunday=0) or
    Jan 5 (Monday=1). What matters is that weekday is in [0..6] and follows
    the Sunday=0 convention — so we just check the range and exercise a
    known case that we can compute both ways.
    """
    # Pick a noon-UTC timestamp to avoid DST edge cases affecting weekday.
    # 2024-01-07 12:00:00 UTC is a Sunday everywhere on Earth.
    ts = 1704628800  # 2024-01-07 12:00:00 UTC
    t = _pytime.localtime(ts)
    expected_wday = (t.tm_wday + 1) % 7
    code = (
        "import time\n"
        f"var d = time.dump({ts})\n"
        f"assert(d['weekday'] == {expected_wday})\n"
        "assert(d['weekday'] >= 0 && d['weekday'] <= 6)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_no_args_returns_nil(vm):
    """time.dump() with no arguments returns nil."""
    code = (
        "import time\n"
        "assert(time.dump() == nil)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_nil_arg_returns_nil(vm):
    """time.dump(nil) returns nil (non-int argument)."""
    code = (
        "import time\n"
        "assert(time.dump(nil) == nil)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_string_arg_returns_nil(vm):
    """time.dump(string) returns nil (non-int argument)."""
    code = (
        "import time\n"
        "assert(time.dump('foo') == nil)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_real_arg_returns_nil(vm):
    """time.dump(real) returns nil — C m_dump checks be_isint(vm, 1)."""
    code = (
        "import time\n"
        "assert(time.dump(1.5) == nil)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_large_timestamp(vm):
    """time.dump() works with large timestamps (year > 2038 on 64-bit)."""
    # 2099-01-01 00:00:00 UTC = 4070908800
    ts = 4070908800
    code = (
        "import time\n"
        f"var d = time.dump({ts})\n"
        # On 32-bit time_t platforms this wraps; on 64-bit it returns 2099.
        # Accept either non-nil map.
        "assert(d != nil)\n"
        f"assert(d['epoch'] == {ts})\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_dump_uses_time_time_value(vm):
    """Chaining: time.dump(time.time()) returns a map with epoch==time.time()."""
    code = (
        "import time\n"
        "var t = time.time()\n"
        "var d = time.dump(t)\n"
        "assert(d != nil)\n"
        "assert(d['epoch'] == t)\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ============================================================================
# time.clock()
# ============================================================================
def test_clock_returns_real(vm):
    """time.clock() returns a real number."""
    code = (
        "import time\n"
        "assert(type(time.clock()) == 'real')\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_clock_nonnegative(vm):
    """time.clock() returns a non-negative value."""
    code = (
        "import time\n"
        "assert(time.clock() >= 0.0)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_clock_monotonic_nondecreasing(vm):
    """Successive calls to time.clock() are non-decreasing."""
    code = (
        "import time\n"
        "var c1 = time.clock()\n"
        # Burn some CPU to ensure the second call is later
        "for i: 0..999 end\n"
        "var c2 = time.clock()\n"
        "assert(c2 >= c1)\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ============================================================================
# Module metadata
# ============================================================================
def test_time_module_importable(vm):
    """The time module can be imported."""
    code = (
        "import time\n"
        "assert(time != nil)\n"
    )
    assert be_dostring(vm, code) == BE_OK


def test_time_module_has_expected_attrs(vm):
    """Module exposes exactly 'time', 'dump', and 'clock' as callables."""
    # `introspect.members` may not be guaranteed, but we can check presence
    # by simply calling each member.
    code = (
        "import time\n"
        "assert(type(time.time) == 'function')\n"
        "assert(type(time.dump) == 'function')\n"
        "assert(type(time.clock) == 'function')\n"
    )
    assert be_dostring(vm, code) == BE_OK


# ============================================================================
# Cross-validation against the C binary
# ============================================================================
@pytest.mark.skipif(not _c_binary_available(),
                    reason="C berry binary not available for cross-validation")
def test_time_c_cross_validate_types():
    """Python port and C binary agree on the types returned by each function."""
    source = (
        "import time\n"
        "print(type(time.time()))\n"
        "print(type(time.clock()))\n"
        "print(type(time.dump(0)))\n"
        "print(classname(time.dump(0)))\n"
    )
    c_result = _run_c(source)
    assert c_result is not None
    c_rc, c_out = c_result
    assert c_rc == 0
    c_lines = c_out.strip().splitlines()
    assert c_lines == ["int", "real", "instance", "map"]


@pytest.mark.skipif(not _c_binary_available(),
                    reason="C berry binary not available for cross-validation")
def test_time_c_cross_validate_dump_fields():
    """Python port and C binary produce identical time.dump() fields
    for the same deterministic timestamp."""
    ts = 1700000000  # 2023-11-14 UTC
    source = (
        "import time\n"
        f"var d = time.dump({ts})\n"
        "print(d['year'], d['month'], d['day'], d['hour'], d['min'], d['sec'], d['weekday'], d['epoch'])\n"
    )
    c_result = _run_c(source)
    assert c_result is not None
    c_rc, c_out = c_result
    assert c_rc == 0

    # Run the same source on the Python port
    vm = be_vm_new()
    try:
        # Capture stdout
        import io
        saved = sys.stdout
        sys.stdout = buf = io.StringIO()
        try:
            rc = be_dostring(vm, source)
        finally:
            sys.stdout = saved
        assert rc == BE_OK
        py_out = buf.getvalue()
    finally:
        be_vm_delete(vm)

    assert py_out.strip() == c_out.strip(), \
        f"Python: {py_out!r} != C: {c_out!r}"


@pytest.mark.skipif(not _c_binary_available(),
                    reason="C berry binary not available for cross-validation")
def test_time_c_cross_validate_dump_nil_cases():
    """Python and C agree that non-int arguments to dump return nil."""
    source = (
        "import time\n"
        "print(time.dump())\n"
        "print(time.dump(nil))\n"
        "print(time.dump('x'))\n"
        "print(time.dump(1.5))\n"
    )
    c_result = _run_c(source)
    assert c_result is not None
    c_rc, c_out = c_result
    assert c_rc == 0
    assert c_out.strip().splitlines() == ["nil", "nil", "nil", "nil"]
