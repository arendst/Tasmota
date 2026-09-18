"""
Feature: berry-python-port — be_str2real single-precision overflow handling

When BE_USE_SINGLE_FLOAT is enabled, be_str2real must saturate overflowing
values to +/- infinity rather than raising, matching the silent-overflow
semantics of C `float` arithmetic.

The original Python port crashed with an OverflowError on inputs like
"123e65" because it relied on `struct.pack('<f', x)` to round through
float32 — and that function raises when x exceeds FLT_MAX.

C reference (src/be_strlib.c):

    while (e--) {
        if (e > 0 && sum > BREAL_MAX / ratio) goto overflow;
        sum *= ratio;
    }
    ...
overflow:
    #if BE_USE_SINGLE_FLOAT == 0
        return sign == '-' ? -HUGE_VAL : HUGE_VAL;
    #else
        return sign == '-' ? -HUGE_VALF : HUGE_VALF;
    #endif

This file covers:
  - _f32 saturation at FLT_MAX (no OverflowError)
  - be_str2real returning +/-inf for oversized exponents in both the
    integer-times-exponent and fraction-times-exponent paths
  - Normal in-range parses still produce exact float32 values
  - End-to-end: json.load on the offending case "[123e65]" parses without
    raising
"""

import math
import os
import subprocess
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.be_strlib import BREAL_MAX, _f32, be_str2real
from berry_port.berry_conf import BE_USE_SINGLE_FLOAT


_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


# ============================================================================
# 1. _f32 saturation
# ============================================================================

class TestF32Saturation:
    """_f32 must never raise; values beyond FLT_MAX clamp to +/-inf."""

    def test_zero(self):
        assert _f32(0.0) == 0.0

    def test_small_positive(self):
        # 1.5 is exactly representable in float32
        assert _f32(1.5) == 1.5

    def test_small_negative(self):
        assert _f32(-1.5) == -1.5

    def test_flt_max_stays_finite(self):
        # Value just at FLT_MAX must still round to a finite float32
        result = _f32(BREAL_MAX)
        assert math.isfinite(result)

    def test_positive_overflow_saturates_to_inf(self):
        # 1e40 is well above FLT_MAX (3.4e38); must not raise
        result = _f32(1e40)
        assert result == math.inf

    def test_negative_overflow_saturates_to_neg_inf(self):
        result = _f32(-1e40)
        assert result == -math.inf

    def test_python_inf_passes_through(self):
        assert _f32(math.inf) == math.inf
        assert _f32(-math.inf) == -math.inf

    def test_nan_passes_through(self):
        assert math.isnan(_f32(float('nan')))

    def test_does_not_raise_for_huge_value(self):
        # Regression: struct.pack('<f', 1e60) used to raise OverflowError
        try:
            _f32(1e60)
        except OverflowError:
            pytest.fail("_f32 must not raise OverflowError")


# ============================================================================
# 2. be_str2real in-range parses
# ============================================================================

class TestBeStr2RealNormal:
    """Values comfortably within float32 range parse exactly."""

    def test_zero(self):
        assert be_str2real("0") == 0.0

    def test_integer(self):
        assert be_str2real("123") == 123.0

    def test_negative(self):
        assert be_str2real("-42") == -42.0

    def test_decimal(self):
        # 0.5 is exactly representable in float32
        assert be_str2real("0.5") == 0.5

    def test_exponent_positive(self):
        assert be_str2real("1e2") == 100.0

    def test_exponent_negative(self):
        # 0.01 isn't exact in binary float, but the round-trip through
        # float32 must stay finite and close
        result = be_str2real("1e-2")
        assert math.isfinite(result)
        assert abs(result - 0.01) < 1e-6

    def test_capital_e(self):
        assert be_str2real("1E2") == 100.0

    def test_positive_sign(self):
        assert be_str2real("+5") == 5.0

    def test_fraction_with_exponent(self):
        # 1.5e2 = 150, exactly representable
        assert be_str2real("1.5e2") == 150.0


# ============================================================================
# 3. be_str2real overflow — regression for json_advanced.be
# ============================================================================

# The concrete values triggering the original crash, plus related forms.
_OVERFLOW_INPUTS_POSITIVE = [
    "123e65",       # from JSONTestSuite "number" case — the original crasher
    "1e40",         # just above FLT_MAX
    "1e100",        # very large
    "123.456e78",   # fraction + large exponent (from json_test_cases)
    "9e38",         # marginally above FLT_MAX
]

_OVERFLOW_INPUTS_NEGATIVE = [
    "-123e65",
    "-1e40",
    "-1.5e38",  # within range
    "-1.5e39",  # just above
    "-123.456e78",
]


class TestBeStr2RealOverflow:
    """Oversized exponents must saturate to +/- infinity without raising."""

    @pytest.mark.parametrize("text", _OVERFLOW_INPUTS_POSITIVE)
    def test_positive_overflow_returns_inf(self, text):
        if not BE_USE_SINGLE_FLOAT:
            pytest.skip("overflow threshold only meaningful under single-float")
        result = be_str2real(text)
        assert result == math.inf, f"for {text!r} expected inf, got {result!r}"

    @pytest.mark.parametrize("text", [
        "-123e65",
        "-1e40",
        "-1.5e39",
        "-123.456e78",
    ])
    def test_negative_overflow_returns_neg_inf(self, text):
        if not BE_USE_SINGLE_FLOAT:
            pytest.skip("overflow threshold only meaningful under single-float")
        result = be_str2real(text)
        assert result == -math.inf, f"for {text!r} expected -inf, got {result!r}"

    def test_no_overflow_error_raised(self):
        # Regression guard: the original bug surfaced as OverflowError from
        # struct.pack deep inside _f32. Make sure none of these inputs raise.
        for text in _OVERFLOW_INPUTS_POSITIVE + _OVERFLOW_INPUTS_NEGATIVE:
            try:
                be_str2real(text)
            except OverflowError as exc:
                pytest.fail(f"be_str2real({text!r}) raised OverflowError: {exc}")

    def test_huge_exponent_digits_still_returns_inf(self):
        # Exponent value > 300 hits the e > 300 early-out in both C and Python.
        if not BE_USE_SINGLE_FLOAT:
            pytest.skip("overflow threshold only meaningful under single-float")
        assert be_str2real("1e9999") == math.inf
        assert be_str2real("-1e9999") == -math.inf

    def test_integer_mantissa_overflow(self):
        # Integer part alone can overflow before reaching exponent.
        # A 50-digit integer blows past BREAL_MAX in the integer loop.
        if not BE_USE_SINGLE_FLOAT:
            pytest.skip("overflow threshold only meaningful under single-float")
        text = "1" + "0" * 50  # 1e50 written out as integer digits
        assert be_str2real(text) == math.inf


# ============================================================================
# 4. return_end still reports a sane end position on overflow
# ============================================================================

class TestBeStr2RealReturnEnd:
    """The (value, end_pos) form must still return a consumed position,
    even when overflow short-circuits the parse. This keeps callers like
    be_str2num from hanging on the input."""

    def test_overflow_returns_tuple(self):
        if not BE_USE_SINGLE_FLOAT:
            pytest.skip()
        val, end = be_str2real("123e65", return_end=True)
        assert val == math.inf
        # Some portion of the input must have been consumed
        assert end > 0

    def test_normal_returns_tuple(self):
        val, end = be_str2real("1.5 ", return_end=True)
        assert val == 1.5
        assert end > 0


# ============================================================================
# 5. End-to-end: json.load on the offending case
# ============================================================================

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def _run_berry(source, timeout=15):
    return subprocess.run(
        [*BERRY_CMD, "-e", source],
        capture_output=True,
        text=True,
        timeout=timeout,
    )


class TestJsonLoadOverflowDoesNotCrash:
    """The original failure mode was an uncaught OverflowError bubbling up
    from deep inside json.load(). Ensure these inputs now parse cleanly."""

    def test_json_load_123e65(self):
        # This is the exact string from tests/json_test_cases.json
        # under "positive" -> "number"
        r = _run_berry("import json\nvar v = json.load('[123e65]')\nprint(v != nil)")
        assert r.returncode == 0, f"stderr: {r.stderr}"
        assert r.stdout.strip() == "true"

    def test_json_load_huge_fraction_exponent(self):
        r = _run_berry("import json\nvar v = json.load('[123.456e78]')\nprint(v != nil)")
        assert r.returncode == 0, f"stderr: {r.stderr}"
        assert r.stdout.strip() == "true"

    def test_json_load_any_huge_cases_do_not_crash(self):
        # The "any" section of json_test_cases.json is loaded only to check
        # that no crash occurs — value semantics aren't asserted.
        for case in [
            "[123.456e-789]",
            "[-1e+9999]",
            "[1.5e+9999]",
            "[-123123e100000]",
            "[123123e100000]",
            "[123e-10000000]",
        ]:
            src = f"import json\njson.load({case!r})\nprint('ok')"
            r = _run_berry(src)
            assert r.returncode == 0, (
                f"crashed on {case!r}\nstderr: {r.stderr}"
            )
            assert r.stdout.strip() == "ok"


# ============================================================================
# 6. Cross-validation with C binary (if available)
# ============================================================================

_CROSS_VECTORS = [
    "123e65",
    "1e40",
    "1e100",
    "-1e40",
    "1.5e38",  # just below FLT_MAX
    "-1.5e38",
    "0.5",
    "1",
    "0",
]


def _parse_via_c(text):
    """Parse a real via the C binary and return its printed form."""
    if not _c_binary_available():
        return None
    # Use real() which goes through be_str2real under the hood
    script = f'print(real("{text}"))'
    result = subprocess.run(
        [_BERRY_BIN, "-e", script],
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return None
    return result.stdout.strip()


def _py_printed(value):
    """Format a float the way Berry prints reals: inf -> 'inf', etc."""
    if value == math.inf:
        return "inf"
    if value == -math.inf:
        return "-inf"
    if math.isnan(value):
        return "nan"
    # Berry prints reals via %g style — approximate by trimming trailing zeros
    return repr(value)


if _c_binary_available():
    @pytest.mark.parametrize("text", _CROSS_VECTORS)
    def test_overflow_classification_matches_c(text):
        """For each input, the Python port and C must agree on whether the
        result is finite, +inf, or -inf."""
        py_val = be_str2real(text)
        c_out = _parse_via_c(text)
        if c_out is None:
            pytest.skip("C binary failed to evaluate")

        if py_val == math.inf:
            assert "inf" in c_out and "-" not in c_out, (
                f"{text!r}: Python got inf, C got {c_out!r}"
            )
        elif py_val == -math.inf:
            assert "-inf" in c_out, (
                f"{text!r}: Python got -inf, C got {c_out!r}"
            )
        else:
            # Finite on both sides
            assert "inf" not in c_out, (
                f"{text!r}: Python got finite {py_val}, C got {c_out!r}"
            )
