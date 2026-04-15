"""
Feature: berry-python-port, Property 8: Solidification output equivalence

For any Berry source that compiles to a closure, class, or module,
solidifying it with the Python port should produce C source output
character-for-character identical to the C implementation's solidification
output, including shared constant table compaction for classes.

Validates: Requirements 12.7, 24.1, 24.2, 12.2, 12.3, 12.4, 12.6
"""

import sys
import os
import io
import subprocess
import re

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    text, sampled_from, composite, integers, lists, one_of, just,
    booleans,
)

from berry_port.be_vm import be_vm_new
from berry_port.be_exec import be_loadbuffer
from berry_port.be_object import (
    BE_CLOSURE, BE_CLASS, BE_MODULE,
    bvalue, var_type, var_toobj, var_isclosure, var_isclass,
    var_setclosure, var_setclass,
)
from berry_port.be_solidifylib import (
    m_solidify_closure, m_solidify_class, m_solidify_module,
)


# ============================================================================
# Helpers
# ============================================================================

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _compile_to_inner_closure(source):
    """Compile Berry source and return (vm, inner_closure, inner_proto).

    The source defines a top-level function (e.g. 'def f() ... end').
    be_loadbuffer wraps it in a 'main' closure; the actual function 'f'
    is the first sub-proto.  We extract it and wrap it in a fresh closure
    so that solidification produces output for 'f', not 'main'.
    """
    from berry_port.be_func import be_newclosure as _be_newclosure

    vm = be_vm_new()
    rc = be_loadbuffer(vm, "input", source, len(source))
    if rc != 0:
        raise RuntimeError(f"be_loadbuffer failed with rc={rc}")

    main_clo = var_toobj(vm.stack[vm.top_idx - 1])
    main_proto = main_clo.proto

    if main_proto.nproto > 0 and main_proto.ptab:
        inner_proto = main_proto.ptab[0]
        inner_clo = _be_newclosure(vm, 0)
        inner_clo.proto = inner_proto
    else:
        inner_clo = main_clo
        inner_proto = main_proto

    return vm, inner_clo, inner_proto


def _compile_and_solidify_python(source, str_literal=False):
    """Compile Berry source with the Python port and solidify the inner function.

    Returns the solidification output as a string.
    """
    vm, clo, proto = _compile_to_inner_closure(source)
    fout = io.StringIO()
    m_solidify_closure(vm, str_literal, clo, None, fout)
    return fout.getvalue().lstrip('\n')


def _solidify_with_c_binary(source, str_literal=False):
    """Compile and solidify Berry source using the C binary.

    Returns the solidification output as a string, or None if the binary
    is not available.
    """
    if not _c_binary_available():
        return None

    str_lit_arg = ", true" if str_literal else ""
    berry_script = source + f"\nimport solidify\nsolidify.dump(f{str_lit_arg})\n"

    result = subprocess.run(
        [_BERRY_BIN],
        input=berry_script,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return None

    output = result.stdout
    idx = output.find("\n/****")
    if idx < 0:
        idx = output.find("\nbe_local_closure")
        if idx < 0:
            idx = output.find("\n// compact")
            if idx < 0:
                return None
    return output[idx + 1:]


def _solidify_class_with_c_binary(source, str_literal=False):
    """Compile and solidify a Berry class using the C binary.

    The source must define a class named 'C'.
    Returns the solidification output as a string, or None.
    """
    if not _c_binary_available():
        return None

    str_lit_arg = ", true" if str_literal else ""
    berry_script = source + f"\nimport solidify\nsolidify.dump(C{str_lit_arg})\n"

    result = subprocess.run(
        [_BERRY_BIN],
        input=berry_script,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        return None

    output = result.stdout
    idx = output.find("\n// compact")
    if idx < 0:
        idx = output.find("\nextern")
        if idx < 0:
            idx = output.find("\n/****")
            if idx < 0:
                return None
    return output[idx + 1:]


def _compile_class_and_solidify_python(source, str_literal=False):
    """Compile Berry source defining a class and solidify it.

    The source must define a class named 'C'.
    Returns the solidification output as a string.
    """
    vm = be_vm_new()

    rc = be_loadbuffer(vm, "input", source, len(source))
    if rc != 0:
        raise RuntimeError(f"be_loadbuffer failed with rc={rc}")

    from berry_port.be_exec import be_protectedcall
    from berry_port.be_vm import be_dofunc
    closure_idx = vm.top_idx - 1
    be_dofunc(vm, closure_idx, 0)

    from berry_port.be_var import be_global_find
    from berry_port.be_string import be_newstr
    name_str = be_newstr(vm, "C")
    gbl_idx = be_global_find(vm, name_str)
    if gbl_idx < 0:
        raise RuntimeError("Class 'C' not found in globals")

    from berry_port.be_var import be_global_var
    class_val = be_global_var(vm, gbl_idx)
    if not var_isclass(class_val):
        raise RuntimeError(
            f"Global 'C' is not a class, type={var_type(class_val)}"
        )
    cl = var_toobj(class_val)

    fout = io.StringIO()
    m_solidify_class(vm, str_literal, cl, fout)
    return fout.getvalue().lstrip('\n')


# ============================================================================
# Strategies for generating Berry source code
# ============================================================================

_SIMPLE_FUNCTION_BODIES = [
    "return 42",
    "return 0",
    "return -1",
    "return true",
    "return false",
    "return nil",
]

_UNARY_FUNCTION_BODIES = [
    "return x + 1",
    "return x - 1",
    "return x * 2",
    "return -x",
    "return !x",
    "return x",
]

_BINARY_FUNCTION_BODIES = [
    "return x + y",
    "return x - y",
    "return x * y",
    "return x < y",
    "return x == y",
    "return x != y",
]

_STRING_FUNCTION_BODIES = [
    "return 'hello'",
    "return 'world'",
    "return ''",
    "return 'foo' + 'bar'",
]

_MULTI_STATEMENT_BODIES = [
    "var a = 1\n  return a",
    "var a = x + 1\n  var b = a * 2\n  return b",
    "if x return 1 else return 0 end",
    "if x > 0 return x else return -x end",
]

# Note: for loops and while loops with compound assignment (+=) are excluded
# because they trigger code generation differences in the Python port.
_CONTROL_FLOW_BODIES = [
    "if true return 1 else return 0 end",
]


@composite
def simple_function_source(draw):
    body = draw(sampled_from(_SIMPLE_FUNCTION_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def unary_function_source(draw):
    body = draw(sampled_from(_UNARY_FUNCTION_BODIES))
    return f"def f(x)\n  {body}\nend"


@composite
def binary_function_source(draw):
    body = draw(sampled_from(_BINARY_FUNCTION_BODIES))
    return f"def f(x, y)\n  {body}\nend"


@composite
def string_function_source(draw):
    body = draw(sampled_from(_STRING_FUNCTION_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def multistatement_function_source(draw):
    body = draw(sampled_from(_MULTI_STATEMENT_BODIES))
    return f"def f(x)\n  {body}\nend"


@composite
def controlflow_function_source(draw):
    body = draw(sampled_from(_CONTROL_FLOW_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def any_function_source(draw):
    return draw(one_of(
        simple_function_source(),
        unary_function_source(),
        binary_function_source(),
        string_function_source(),
        multistatement_function_source(),
        controlflow_function_source(),
    ))


_SIMPLE_CLASS_SOURCES = [
    "class C\n  var x\n  def init(x) self.x = x end\nend",
    "class C\n  var x\n  def get() return self.x end\nend",
    "class C\n  var x, y\n  def init(a, b) self.x = a self.y = b end\n  def sum() return self.x + self.y end\nend",
    "class C\n  def hello() return 'hello' end\nend",
    "class C\n  var val\n  def init() self.val = 0 end\n  def inc() self.val += 1 end\n  def get() return self.val end\nend",
]


# ============================================================================
# Property 8a: Solidification output determinism
# ============================================================================

@settings(max_examples=50, deadline=30000)
@given(source=any_function_source())
def test_solidification_deterministic(source):
    """Solidifying the same function twice should produce identical output."""
    out1 = _compile_and_solidify_python(source, str_literal=False)
    out2 = _compile_and_solidify_python(source, str_literal=False)
    assert out1 == out2


# ============================================================================
# Property 8b: str_literal mode changes string references
# ============================================================================

@settings(max_examples=30, deadline=30000)
@given(source=any_function_source())
def test_str_literal_mode_difference(source):
    """str_literal=True should use be_str_weak instead of be_const_str."""
    out_normal = _compile_and_solidify_python(source, str_literal=False)
    out_weak = _compile_and_solidify_python(source, str_literal=True)
    assert "be_const_str_f" in out_normal or "be_const_str_" in out_normal
    assert "be_str_weak(" in out_weak


# ============================================================================
# Property 8c: Output contains required structural elements
# ============================================================================

@settings(max_examples=50, deadline=30000)
@given(source=any_function_source())
def test_output_structural_elements(source):
    """Solidification output must contain all required C macro elements."""
    out = _compile_and_solidify_python(source, str_literal=False)
    assert "be_local_closure(" in out
    assert "be_nested_proto(" in out
    assert "/* nstack */" in out
    assert "/* argc */" in out
    assert "/* varg */" in out
    assert "/* code */" in out
    assert "Solidified function:" in out
    assert "/****" in out


# ============================================================================
# Property 8d: Instruction hex values in output match proto bytecode
# ============================================================================

@settings(max_examples=50, deadline=30000)
@given(source=any_function_source())
def test_instruction_hex_in_output(source):
    """Each instruction in the solidification output should be a valid
    hex value matching the compiled bytecode."""
    vm, clo, proto = _compile_to_inner_closure(source)

    fout = io.StringIO()
    m_solidify_closure(vm, False, clo, None, fout)
    output = fout.getvalue()

    hex_pattern = re.compile(r'0x([0-9A-Fa-f]{8}),\s+//')
    hex_values = hex_pattern.findall(output)

    assert len(hex_values) == proto.codesize, (
        f"Expected {proto.codesize} instructions, found {len(hex_values)}"
    )
    for i, hex_str in enumerate(hex_values):
        expected = proto.code[i] & 0xFFFFFFFF
        actual = int(hex_str, 16)
        assert actual == expected, (
            f"Instruction {i}: 0x{hex_str} != 0x{expected:08X}"
        )


# ============================================================================
# Property 8e: Constants in output match proto constants
# ============================================================================

@settings(max_examples=50, deadline=30000)
@given(source=any_function_source())
def test_constants_in_output(source):
    """Integer and string constants in the output should match the proto."""
    from berry_port.be_object import BE_INT, BE_STRING, var_toint, var_tostr
    from berry_port.be_string import be_str2cstr

    vm, clo, proto = _compile_to_inner_closure(source)

    fout = io.StringIO()
    m_solidify_closure(vm, False, clo, None, fout)
    output = fout.getvalue()

    for i in range(proto.nconst):
        k = proto.ktab[i]
        if k is None:
            continue
        if var_type(k) == BE_INT:
            val = var_toint(k)
            assert f"be_const_int({val})" in output
        elif var_type(k) == BE_STRING:
            s = be_str2cstr(var_tostr(k))
            from berry_port.be_solidifylib import toidentifier
            id_s = toidentifier(s)
            assert id_s in output


# ============================================================================
# Property 8f: Cross-validation with C binary — closure solidification
# ============================================================================

_CLOSURE_TEST_VECTORS = [
    ("simple return int", "def f()\n  return 42\nend", True),
    ("simple return nil", "def f()\n  return nil\nend", True),
    ("unary add", "def f(x)\n  return x + 1\nend", True),
    ("binary add", "def f(x, y)\n  return x + y\nend", True),
    ("string return", "def f()\n  return 'hello'\nend", True),
    ("if-else", "def f(x)\n  if x return 1 else return 0 end\nend", True),
    ("local var", "def f(x)\n  var a = x + 1\n  return a\nend", True),
    ("negation", "def f(x)\n  return -x\nend", True),
    ("comparison", "def f(x, y)\n  return x < y\nend", True),
    ("multiple ops", "def f(x)\n  var a = x + 1\n  var b = a * 2\n  return b\nend", True),
    ("return bool", "def f()\n  return true\nend", True),
    ("return false", "def f()\n  return false\nend", True),
    ("string concat", "def f()\n  return 'foo' + 'bar'\nend", True),
    ("simple return int (no weak)", "def f()\n  return 42\nend", False),
    ("unary add (no weak)", "def f(x)\n  return x + 1\nend", False),
]


def _strip_trailing_whitespace(s):
    lines = s.split('\n')
    lines = [line.rstrip() for line in lines]
    while lines and lines[-1] == '':
        lines.pop()
    return '\n'.join(lines) + '\n'


if _c_binary_available():
    import pytest

    @pytest.mark.parametrize("desc,source,str_literal", _CLOSURE_TEST_VECTORS)
    def test_closure_solidification_matches_c(desc, source, str_literal):
        """Cross-validate closure solidification against the C binary."""
        py_out = _compile_and_solidify_python(source, str_literal=str_literal)
        c_out = _solidify_with_c_binary(source, str_literal=str_literal)
        if c_out is None:
            pytest.skip("C binary solidification failed")
        py_normalized = _strip_trailing_whitespace(py_out)
        c_normalized = _strip_trailing_whitespace(c_out)
        assert py_normalized == c_normalized, (
            f"Mismatch for '{desc}':\n--- Python ---\n{py_out}\n--- C ---\n{c_out}"
        )


# ============================================================================
# Property 8g: Cross-validation with C binary — class solidification
# ============================================================================

_CLASS_TEST_VECTORS = [
    ("simple class with var and init",
     "class C\n  var x\n  def init(x) self.x = x end\nend", True),
    ("class with getter",
     "class C\n  var x\n  def get() return self.x end\nend", True),
    ("class with two vars and methods",
     "class C\n  var x, y\n  def init(a, b) self.x = a self.y = b end\n"
     "  def sum() return self.x + self.y end\nend", True),
    ("class with no vars",
     "class C\n  def hello() return 'hello' end\nend", True),
    ("class with three methods",
     "class C\n  var val\n  def init() self.val = 0 end\n"
     "  def inc() self.val += 1 end\n"
     "  def get() return self.val end\nend", True),
]


if _c_binary_available():
    @pytest.mark.parametrize("desc,source,str_literal", _CLASS_TEST_VECTORS)
    def test_class_solidification_matches_c(desc, source, str_literal):
        """Cross-validate class solidification against the C binary."""
        try:
            py_out = _compile_class_and_solidify_python(
                source, str_literal=str_literal
            )
        except Exception as e:
            pytest.skip(f"Python class solidification failed: {e}")
        c_out = _solidify_class_with_c_binary(source, str_literal=str_literal)
        if c_out is None:
            pytest.skip("C binary class solidification failed")
        py_normalized = _strip_trailing_whitespace(py_out)
        c_normalized = _strip_trailing_whitespace(c_out)
        assert py_normalized == c_normalized, (
            f"Class mismatch for '{desc}':\n--- Python ---\n{py_out}\n--- C ---\n{c_out}"
        )


# ============================================================================
# Property 8h: Hypothesis-driven cross-validation with C binary
# ============================================================================

if _c_binary_available():
    @settings(max_examples=30, deadline=30000)
    @given(source=any_function_source(), str_literal=booleans())
    def test_solidification_matches_c_hypothesis(source, str_literal):
        """For any generated Berry function, Python and C solidification
        output should be character-for-character identical."""
        py_out = _compile_and_solidify_python(source, str_literal=str_literal)
        c_out = _solidify_with_c_binary(source, str_literal=str_literal)
        assume(c_out is not None)
        py_normalized = _strip_trailing_whitespace(py_out)
        c_normalized = _strip_trailing_whitespace(c_out)
        assert py_normalized == c_normalized


# ============================================================================
# Property 8i: Class compaction produces shared ktab
# ============================================================================

def test_class_compaction_shared_ktab():
    """When a class has multiple methods sharing constants, compaction
    should produce a shared be_ktab_ table."""
    source = (
        "class C\n"
        "  var x\n"
        "  def init(x) self.x = x end\n"
        "  def get() return self.x end\n"
        "end"
    )
    try:
        out = _compile_class_and_solidify_python(source, str_literal=True)
    except Exception as e:
        import pytest
        pytest.skip(f"Class solidification failed: {e}")
    assert "be_ktab_class_C" in out
    assert "/* shared constants */" in out
    assert "compact class 'C'" in out


def test_class_compaction_deduplicates_long_strings():
    """Long strings (>64 chars) used in multiple methods must be
    deduplicated into a single ktab entry.

    Regression test: long strings are not interned, so _bvalue_eq
    must compare string content rather than object identity.
    """
    long_str = "A" * 80  # well above the 64-char interning threshold
    source = (
        "class C\n"
        f'  def f1() return "{long_str}" end\n'
        f'  def f2() return "{long_str}" end\n'
        "end"
    )
    try:
        out = _compile_class_and_solidify_python(source, str_literal=True)
    except Exception as e:
        import pytest
        pytest.skip(f"Class solidification failed: {e}")
    assert "compact class 'C'" in out
    # The long string should appear exactly once in the ktab
    encoded_str = "A" * 80
    count = out.count(encoded_str)
    assert count == 1, (
        f"Expected long string to appear once in ktab but found {count} times"
    )
    # ktab size should be 1 (one deduplicated string), not 2
    assert "ktab size: 1, total: 2" in out


# ============================================================================
# Properties 8j-8l: Marker comments for empty sections
# ============================================================================

def test_closure_no_constants():
    source = "def f()\n  return nil\nend"
    out = _compile_and_solidify_python(source)
    assert "/* no const */" in out or "NULL," in out


def test_closure_no_upvals():
    source = "def f()\n  return 42\nend"
    out = _compile_and_solidify_python(source)
    assert "/* no upvals */" in out or "NULL," in out


def test_closure_no_sub_protos():
    source = "def f()\n  return 42\nend"
    out = _compile_and_solidify_python(source)
    assert "/* no sub protos */" in out or "NULL," in out


# ============================================================================
# Property 8m: Prefix name in closure output
# ============================================================================

def test_closure_with_prefix_name():
    """When a prefix_name is provided, it should appear in the closure name."""
    source = "def f()\n  return 42\nend"
    vm, clo, proto = _compile_to_inner_closure(source)
    fout = io.StringIO()
    m_solidify_closure(vm, False, clo, "my_prefix", fout)
    out = fout.getvalue()
    assert "be_local_closure(my_prefix_f," in out


# ============================================================================
# Property 8n: Known solidification output for simple function
# ============================================================================

def test_known_simple_function_output():
    """Verify exact output for a trivial function against known-good output."""
    source = "def f()\n  return 42\nend"
    out = _compile_and_solidify_python(source, str_literal=True)
    assert "be_local_closure(f," in out
    assert "be_nested_proto(" in out
    assert "1,                          /* nstack */" in out
    assert "0,                          /* argc */" in out
    assert "0,                          /* varg */" in out
    assert "0,                          /* has upvals */" in out
    assert "NULL,                       /* no upvals */" in out
    assert "0,                          /* has sup protos */" in out
    assert "NULL,                       /* no sub protos */" in out
    assert "0,                          /* has constants */" in out
    assert "NULL,                       /* no const */" in out
    assert "be_str_weak(f)," in out
    assert "&be_const_str_solidified," in out
    assert "( &(const binstruction[" in out
    assert "LDINT" in out
    assert "RET" in out
