"""
Tests for the 'preproc' module (berry_port/be_preproclib.py).

The preproc module exposes the VM's preprocessor macro map so Berry
scripts can read and mutate the macros that the lexer consumes via
#define / #undef / #if directives.

Coverage beyond the in-tree tests/preproc_mod.be:
  - Direct Hypothesis property tests on define/undef/defined/value
  - Isolation of the preprocessor map across VM instances
  - Cross-validation against the compiled C binary when available
  - Interaction between the lexer (#define) and the runtime module

Validates: preproc module parity with C implementation.
"""

import os
import sys
import subprocess
import tempfile
from string import ascii_letters, digits

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from hypothesis import given, settings, HealthCheck
from hypothesis import strategies as st

from berry_port.berry import (
    be_vm_new,
    be_vm_delete,
    be_loadstring,
    be_dostring,
    BE_OK,
    BE_SYNTAX_ERROR,
)
from berry_port.be_api import be_getexcept


# ---------------------------------------------------------------------------
# Fixtures and helpers
# ---------------------------------------------------------------------------

BERRY_CMD = [sys.executable, "-m", "berry_port"]
_C_BERRY = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_C_BERRY) and os.access(_C_BERRY, os.X_OK)


def _run_python(source, timeout=15):
    """Run Berry source through the Python port via -e, return CompletedProcess."""
    return subprocess.run(
        [*BERRY_CMD, "-e", source],
        capture_output=True, text=True, timeout=timeout,
    )


def _run_c(source, timeout=15):
    """Run Berry source through the C binary via -e; return CompletedProcess or None."""
    if not _c_binary_available():
        return None
    try:
        return subprocess.run(
            [_C_BERRY, "-e", source],
            capture_output=True, text=True, timeout=timeout,
        )
    except (subprocess.TimeoutExpired, OSError):
        return None


@pytest.fixture
def vm():
    v = be_vm_new()
    yield v
    be_vm_delete(v)


# Hypothesis strategy: valid Berry identifier (lexer rule for macro names)
_ident_start = ascii_letters + "_"
_ident_rest = ascii_letters + digits + "_"

identifier_strategy = st.builds(
    lambda head, tail: head + tail,
    st.sampled_from(_ident_start),
    st.text(alphabet=_ident_rest, min_size=0, max_size=20),
)

# Integer values that fit in bint and are representable as a Berry literal.
# Upper bound is 2**31-1 = 2147483647 (INT_MAX for a 32-bit signed int).
# Lower bound is -(2**31-1) rather than -2**31, because Berry parses the
# literal `-2147483648` as unary-minus applied to `2147483648`, which
# overflows the integer parser (same in the C and Python implementations).
int_value_strategy = st.integers(min_value=-(2**31 - 1), max_value=2**31 - 1)


# ============================================================================
# Direct API tests (in-process via be_dostring)
# ============================================================================

class TestPreprocAPI:
    """In-process sanity checks that don't need a subprocess."""

    def test_import_preproc_succeeds(self, vm):
        code = "import preproc\nassert(preproc != nil)\n"
        assert be_dostring(vm, code) == BE_OK

    def test_empty_define_stores_one(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('EMPTY')\n"
            "assert(preproc.defined('EMPTY'))\n"
            "assert(preproc.value('EMPTY') == 1)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_numeric_value_parsed_as_int(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('N', '42')\n"
            "assert(preproc.value('N') == 42)\n"
            "assert(type(preproc.value('N')) == 'int')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_negative_numeric_value(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('N', '-7')\n"
            "assert(preproc.value('N') == -7)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_quoted_string_strips_quotes(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            'preproc.define("Q", \'"hello"\')\n'
            "assert(preproc.value('Q') == 'hello')\n"
            "assert(type(preproc.value('Q')) == 'string')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_single_quoted_string_strips_quotes(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            'preproc.define("Q", "\'world\'")\n'
            "assert(preproc.value('Q') == 'world')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_raw_string_kept_verbatim(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('R', 'not_a_number')\n"
            "assert(preproc.value('R') == 'not_a_number')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_undef_removes_entry(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('X', '1')\n"
            "assert(preproc.defined('X'))\n"
            "preproc.undef('X')\n"
            "assert(!preproc.defined('X'))\n"
            "assert(preproc.value('X') == nil)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_clear_empties_map(self, vm):
        code = (
            "import preproc\n"
            "preproc.define('A', '1')\n"
            "preproc.define('B', '2')\n"
            "preproc.clear()\n"
            "assert(!preproc.defined('A'))\n"
            "assert(!preproc.defined('B'))\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_undef_on_missing_is_nop(self, vm):
        """undef of a never-defined macro must not raise."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.undef('NEVER_DEFINED')\n"
            "assert(!preproc.defined('NEVER_DEFINED'))\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_value_on_missing_returns_nil(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "assert(preproc.value('MISSING') == nil)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_defined_on_missing_returns_false(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "assert(preproc.defined('MISSING') == false)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_clear_before_any_define_is_safe(self, vm):
        """clear() must work even if the preprocessor map was never touched."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "assert(preproc.defined('X') == false)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_redefine_overrides_previous(self, vm):
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('V', '1')\n"
            "preproc.define('V', '2')\n"
            "assert(preproc.value('V') == 2)\n"
            "preproc.define('V', 'str')\n"
            "assert(preproc.value('V') == 'str')\n"
        )
        assert be_dostring(vm, code) == BE_OK


# ============================================================================
# Type-error contract tests
# ============================================================================

class TestPreprocTypeErrors:
    """Non-string name arguments must raise type_error, matching C."""

    @pytest.mark.parametrize("fn", ["define", "undef", "defined"])
    def test_non_string_name_raises(self, vm, fn):
        code = (
            "import preproc\n"
            "var raised = false\n"
            "try\n"
            f"  preproc.{fn}(123)\n"
            "except 'type_error'\n"
            "  raised = true\n"
            "end\n"
            "assert(raised)\n"
        )
        assert be_dostring(vm, code) == BE_OK


# ============================================================================
# VM isolation
# ============================================================================

class TestPreprocVMIsolation:
    """Each VM has its own preprocessor map; they must not leak between VMs."""

    def test_define_in_vm_a_invisible_to_vm_b(self):
        va = be_vm_new()
        vb = be_vm_new()
        try:
            code_a = "import preproc\npreproc.clear()\npreproc.define('SHARED', '1')\n"
            code_b = (
                "import preproc\n"
                "assert(!preproc.defined('SHARED'))\n"
                "assert(preproc.value('SHARED') == nil)\n"
            )
            assert be_dostring(va, code_a) == BE_OK
            assert be_dostring(vb, code_b) == BE_OK
        finally:
            be_vm_delete(va)
            be_vm_delete(vb)

    def test_clear_in_vm_a_does_not_wipe_vm_b(self):
        va = be_vm_new()
        vb = be_vm_new()
        try:
            assert be_dostring(
                va, "import preproc\npreproc.define('KEEP_A', '1')\n"
            ) == BE_OK
            assert be_dostring(
                vb, "import preproc\npreproc.define('KEEP_B', '2')\n"
            ) == BE_OK
            # Wipe VM A
            assert be_dostring(va, "import preproc\npreproc.clear()\n") == BE_OK
            # VM B is untouched
            assert be_dostring(
                vb, "import preproc\nassert(preproc.value('KEEP_B') == 2)\n"
            ) == BE_OK
        finally:
            be_vm_delete(va)
            be_vm_delete(vb)


# ============================================================================
# Lexer <-> module interaction
# ============================================================================

class TestPreprocLexerIntegration:
    """#define in source code and preproc.define share the same map."""

    def test_hash_define_visible_via_module(self, vm):
        """A lexer #define in the same script is observable after compilation.

        Note: preproc.clear() must come before any #define, because the
        lexer runs during compilation, i.e. before any script code executes.
        Placing clear() after #define wipes the macro that was just parsed.
        """
        # First, wipe any pre-existing macros from earlier parametrized runs.
        assert be_dostring(vm, "import preproc\npreproc.clear()\n") == BE_OK
        code = (
            "import preproc\n"
            "#define FROM_LEXER 77\n"
            "assert(preproc.defined('FROM_LEXER'))\n"
            "assert(preproc.value('FROM_LEXER') == 77)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_module_define_controls_hash_if(self, vm):
        """preproc.define() must be visible to a later compile() using #if."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('FEATURE_ON')\n"
            "var f = compile("
            "  \"#if FEATURE_ON\\n\""
            "  \"return 'on'\\n\""
            "  \"#else\\n\""
            "  \"return 'off'\\n\""
            "  \"#endif\\n\""
            ")\n"
            "assert(f() == 'on')\n"
            "preproc.undef('FEATURE_ON')\n"
            "var g = compile("
            "  \"#if FEATURE_ON\\n\""
            "  \"return 'on'\\n\""
            "  \"#else\\n\""
            "  \"return 'off'\\n\""
            "  \"#endif\\n\""
            ")\n"
            "assert(g() == 'off')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_string_replacement(self, vm):
        """Quoted-string value via preproc.define substitutes in $MACRO"..." prefix form."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            'preproc.define(\'TR\', \'"Translated"\')\n'
            "var f = compile('return $TR\"Default\"')\n"
            "assert(f() == 'Translated')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_bare_macro_emits_value(self, vm):
        """`$IDENT` with no trailing quote emits the macro's string value."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('AAA', 'hello')\n"
            "var f = compile('return $AAA')\n"
            "assert(f() == 'hello')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_bare_macro_undefined_is_syntax_error(self, vm):
        """`$IDENT` with no trailing quote errors when the macro is undefined."""
        # Must run via be_loadstring so the syntax error is surfaced as a
        # return code rather than an uncaught exception.
        code = 'return $BARE_UNDEFINED'
        rc = be_loadstring(vm, code)
        translated = be_getexcept(vm, rc)
        assert translated == BE_SYNTAX_ERROR

    def test_translatable_bare_macro_non_string_is_syntax_error(self, vm):
        """`$IDENT` with no trailing quote errors when the macro is an int."""
        # Prime the macro with an integer value, then try to use $NUMERIC.
        prime = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('NUMERIC', '42')\n"
        )
        assert be_dostring(vm, prime) == BE_OK
        rc = be_loadstring(vm, 'return $NUMERIC')
        translated = be_getexcept(vm, rc)
        assert translated == BE_SYNTAX_ERROR

    def test_translatable_replacement_with_adjacent_strings(self, vm):
        """`$IDENT"default" " tail"` concatenates the replacement + adjacent literals."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('AAA', 'hello')\n"
            "var f = compile('return $AAA\"hi\" \" folks\"')\n"
            "assert(f() == 'hello folks')\n"
            "var g = compile('return $AAA\"hi\" \" \" \"there\"')\n"
            "assert(g() == 'hello there')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_default_with_adjacent_strings(self, vm):
        """`$IDENT"default" " tail"` with IDENT undefined concatenates default + tail."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "var f = compile('return $UNSET\"hi\" \" folks\"')\n"
            "assert(f() == 'hi folks')\n"
            "var g = compile('return $UNSET\"a\" \"b\" \"c\"')\n"
            "assert(g() == 'abc')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_mixed_quote_styles_in_concatenation(self, vm):
        """Concatenation works across single and double quoted segments."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('AAA', 'hello')\n"
            "var f = compile(\"return $AAA'hi' ' folks'\")\n"
            "assert(f() == 'hello folks')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_escape_processing_per_segment(self, vm):
        """Escape sequences in each adjacent literal are processed independently."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "var f = compile('return $UNSET\"a\\\\tb\" \"c\\\\nd\"')\n"
            "assert(f() == \"a\\tbc\\nd\")\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_translatable_int_macro_falls_back_to_default(self, vm):
        """`$IDENT"default"` with IDENT defined as integer emits the default text."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('N', '7')\n"
            "var f = compile('return $N\"default\"')\n"
            "assert(f() == 'default')\n"
        )
        assert be_dostring(vm, code) == BE_OK

    def test_stray_dollar_raises_syntax_error(self, vm):
        """Stray `$` tokens that are not a valid $IDENT"..." prefix are syntax errors.

        Covers:
          - bare `$`
          - `$` followed by a digit
          - `$` followed by whitespace
          - `$IDENT` not followed by a quote
        """
        stray_sources = [
            'return $',            # bare $
            'return $1',           # $ followed by a digit
            'return $ ',           # $ followed by whitespace
            'return $FOO',         # $IDENT not followed by a quote
        ]
        for src in stray_sources:
            rc = be_loadstring(vm, src)
            # be_loadbuffer returns BE_EXCEPTION; translate via be_getexcept
            # which maps the "syntax_error" exception name to BE_SYNTAX_ERROR,
            # matching the C public API contract.
            translated = be_getexcept(vm, rc)
            assert translated == BE_SYNTAX_ERROR, (
                f"expected BE_SYNTAX_ERROR for {src!r}, got rc={rc} "
                f"translated={translated}"
            )

    def test_zero_defined_is_falsy_in_hash_if(self, vm):
        """#if treats a macro defined to 0 as false (matches C semantics)."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            "preproc.define('Z', '0')\n"
            "assert(preproc.defined('Z'))\n"
            "var f = compile("
            "  \"#if Z\\n\""
            "  \"return 'yes'\\n\""
            "  \"#else\\n\""
            "  \"return 'no'\\n\""
            "  \"#endif\\n\""
            ")\n"
            "assert(f() == 'no')\n"
        )
        assert be_dostring(vm, code) == BE_OK


# ============================================================================
# Hypothesis property tests (in-process, fast)
# ============================================================================

class TestPreprocProperties:
    """Randomized properties exercised via be_dostring."""

    @settings(
        max_examples=60,
        deadline=None,
        suppress_health_check=[HealthCheck.function_scoped_fixture],
    )
    @given(name=identifier_strategy, value=int_value_strategy)
    def test_int_roundtrip(self, vm, name, value):
        """For any identifier, define(name, str(int)) then value(name) == int."""
        # Reset between examples: Hypothesis reuses the vm fixture.
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            f"preproc.define('{name}', '{value}')\n"
            f"assert(preproc.value('{name}') == {value})\n"
            f"assert(preproc.defined('{name}'))\n"
        )
        assert be_dostring(vm, code) == BE_OK

    @settings(
        max_examples=60,
        deadline=None,
        suppress_health_check=[HealthCheck.function_scoped_fixture],
    )
    @given(name=identifier_strategy)
    def test_define_then_undef_invariant(self, vm, name):
        """After define+undef, defined() is false and value() is nil."""
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            f"preproc.define('{name}')\n"
            f"assert(preproc.defined('{name}'))\n"
            f"preproc.undef('{name}')\n"
            f"assert(!preproc.defined('{name}'))\n"
            f"assert(preproc.value('{name}') == nil)\n"
        )
        assert be_dostring(vm, code) == BE_OK

    @settings(
        max_examples=40,
        deadline=None,
        suppress_health_check=[HealthCheck.function_scoped_fixture],
    )
    @given(
        name=identifier_strategy,
        payload=st.text(
            alphabet=st.characters(
                blacklist_categories=("Cs",),
                blacklist_characters=('\'', '"', '\\', '\x00', '\n', '\r'),
            ),
            min_size=1, max_size=20,
        ),
    )
    def test_raw_string_roundtrip(self, vm, name, payload):
        """Non-numeric, non-quoted values come back as-is."""
        # Reject payloads that look like integers or begin/end with a quote
        stripped = payload.strip()
        if stripped and stripped.lstrip('+-').isdigit():
            return
        code = (
            "import preproc\n"
            "preproc.clear()\n"
            f"preproc.define('{name}', '{payload}')\n"
            f"assert(preproc.value('{name}') == '{payload}')\n"
        )
        assert be_dostring(vm, code) == BE_OK


# ============================================================================
# Cross-validation with the C binary
# ============================================================================

@pytest.mark.skipif(
    not _c_binary_available(),
    reason="C berry binary not available for cross-validation",
)
class TestPreprocCrossValidation:
    """Same script run through C and Python must produce identical output."""

    @pytest.mark.parametrize("script", [
        # define + value + type
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('X', '42')\n"
         "print(preproc.value('X'), type(preproc.value('X')))"),
        # empty define stores 1
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('E')\n"
         "print(preproc.value('E'))"),
        # quoted string loses quotes
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('Q', '\"hi\"')\n"
         "print(preproc.value('Q'), type(preproc.value('Q')))"),
        # negative int
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('N', '-123')\n"
         "print(preproc.value('N'))"),
        # defined / undef / clear cycle
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('A', '1')\n"
         "print(preproc.defined('A'))\n"
         "preproc.undef('A')\n"
         "print(preproc.defined('A'))\n"
         "preproc.clear()\n"
         "print(preproc.defined('A'))"),
        # lexer #define runs at compile time — preproc.clear() at runtime
        # wipes it; both implementations must agree that the value is nil.
        ("import preproc\n"
         "preproc.clear()\n"
         "#define LEX 99\n"
         "print(preproc.value('LEX'))"),
        # Lexer #define without a later clear() is visible to the module.
        ("import preproc\n"
         "#define LEX2 55\n"
         "print(preproc.value('LEX2'))"),
        # raw string (non-numeric, non-quoted)
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('R', 'plainword')\n"
         "print(preproc.value('R'), type(preproc.value('R')))"),
        # $AAA bare form — emits the macro's string value verbatim.
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('AAA', 'hello')\n"
         "var f = compile('return $AAA')\n"
         "print(f())"),
        # $AAA\"default\" — macro defined, replacement wins, default
        # discarded; adjacent literal concatenated.
        ("import preproc\n"
         "preproc.clear()\n"
         "preproc.define('AAA', 'hello')\n"
         "var f = compile('return $AAA\"hi\" \" folks\"')\n"
         "print(f())"),
        # $BBB\"default\" — macro undefined, default wins; adjacent
        # literal concatenated.
        ("import preproc\n"
         "preproc.clear()\n"
         "var f = compile('return $BBB\"hi\" \" folks\"')\n"
         "print(f())"),
    ])
    def test_c_python_equivalence(self, script):
        r_py = _run_python(script)
        r_c = _run_c(script)
        assert r_c is not None
        assert r_py.returncode == 0, f"python stderr: {r_py.stderr}"
        assert r_c.returncode == 0, f"C stderr: {r_c.stderr}"
        assert r_py.stdout == r_c.stdout, (
            f"Output diverged for script:\n{script}\n"
            f"--- python ---\n{r_py.stdout}\n"
            f"--- C ---\n{r_c.stdout}\n"
        )

    def test_c_python_type_error_names_match(self):
        """Both implementations raise 'type_error' for non-string names."""
        script = (
            "import preproc\n"
            "try\n"
            "  preproc.define(123)\n"
            "except .. as e\n"
            "  print(e)\n"
            "end\n"
            "try\n"
            "  preproc.undef(123)\n"
            "except .. as e\n"
            "  print(e)\n"
            "end\n"
            "try\n"
            "  preproc.defined(123)\n"
            "except .. as e\n"
            "  print(e)\n"
            "end\n"
        )
        r_py = _run_python(script)
        r_c = _run_c(script)
        assert r_c is not None
        assert r_py.stdout == r_c.stdout == "type_error\ntype_error\ntype_error\n"
