"""
Feature: berry-python-port, Property 6: Compilation equivalence

For any valid Berry source string, compiling it with the Python port's
be_loadbuffer should produce a bproto with the same bytecode instruction
sequence, constants table values, upvalue descriptors, sub-proto hierarchy,
and function metadata (argc, nstack, varg) as the C implementation.

We validate this by:
  a) Checking internal consistency of compiled protos (determinism, metadata)
  b) Cross-validating against the C binary via solidification output, which
     encodes all proto fields (bytecode hex, constants, upvals, sub-protos,
     nstack, argc, varg) in a canonical text format.

Validates: Requirements 3.3, 4.4
"""

import sys
import os
import io
import re
import subprocess

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    sampled_from, composite, one_of, booleans, integers, lists,
)

from berry_port.be_vm import be_vm_new
from berry_port.be_exec import be_loadbuffer
from berry_port.be_object import (
    BE_INT, BE_REAL, BE_STRING, BE_NIL,
    var_type, var_toint, var_toreal, var_tostr, var_toobj,
)
from berry_port.be_string import be_str2cstr
from berry_port.be_solidifylib import m_solidify_closure
from berry_port.be_func import be_newclosure as _be_newclosure


# ============================================================================
# Helpers
# ============================================================================

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _compile_source(source):
    """Compile Berry source with the Python port.

    Returns (vm, main_closure, main_proto).
    """
    vm = be_vm_new()
    rc = be_loadbuffer(vm, "input", source, len(source))
    if rc != 0:
        raise RuntimeError(f"be_loadbuffer failed with rc={rc}")
    main_clo = var_toobj(vm.stack[vm.top_idx - 1])
    return vm, main_clo, main_clo.proto


def _extract_inner_closure(vm, main_proto):
    """Extract the first sub-proto from a main proto and wrap it in a closure."""
    if main_proto.nproto > 0 and main_proto.ptab:
        inner_proto = main_proto.ptab[0]
        inner_clo = _be_newclosure(vm, 0)
        inner_clo.proto = inner_proto
        return inner_clo, inner_proto
    return None, None


def _solidify_python(source):
    """Compile and solidify the inner function 'f' using the Python port."""
    vm, main_clo, main_proto = _compile_source(source)
    inner_clo, inner_proto = _extract_inner_closure(vm, main_proto)
    if inner_clo is None:
        raise RuntimeError("No inner function found in source")
    fout = io.StringIO()
    m_solidify_closure(vm, True, inner_clo, None, fout)
    return fout.getvalue().lstrip('\n')


def _solidify_c(source):
    """Compile and solidify the inner function 'f' using the C binary.

    Returns the solidification output string, or None on failure.
    """
    if not _c_binary_available():
        return None
    berry_script = source + "\nimport solidify\nsolidify.dump(f, true)\n"
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
            return None
    return output[idx + 1:]


def _strip_trailing_whitespace(s):
    """Normalize trailing whitespace for comparison."""
    lines = s.split('\n')
    lines = [line.rstrip() for line in lines]
    while lines and lines[-1] == '':
        lines.pop()
    return '\n'.join(lines) + '\n'


def _extract_instructions_from_solidify(output):
    """Extract instruction hex values from solidification output."""
    hex_pattern = re.compile(r'0x([0-9A-Fa-f]{8}),\s+//')
    return [int(h, 16) for h in hex_pattern.findall(output)]


def _extract_nstack(output):
    """Extract nstack value from solidification output."""
    m = re.search(r'(\d+),\s+/\* nstack \*/', output)
    return int(m.group(1)) if m else None


def _extract_argc(output):
    """Extract argc value from solidification output."""
    m = re.search(r'(\d+),\s+/\* argc \*/', output)
    return int(m.group(1)) if m else None


def _extract_varg(output):
    """Extract varg value from solidification output."""
    m = re.search(r'(\d+),\s+/\* varg \*/', output)
    return int(m.group(1)) if m else None


def _extract_int_constants(output):
    """Extract integer constants from solidification output."""
    return [int(m) for m in re.findall(r'be_const_int\((-?\d+)\)', output)]


def _extract_real_constants(output):
    """Extract real constants from solidification output."""
    return [float(m) for m in re.findall(r'be_const_real_hex\(([^)]+)\)', output)]


def _extract_string_constants(output):
    """Extract string constant identifiers from solidification output."""
    return re.findall(r'be_(?:nested_str|str_weak)\(([^)]+)\)', output)


def _assert_proto_fields(proto, name_str, expected_argc, expected_nstack):
    """Assert basic proto metadata fields."""
    assert proto.argc == expected_argc, \
        f"argc: {proto.argc} != {expected_argc}"
    assert proto.nstack >= expected_argc, \
        f"nstack ({proto.nstack}) < argc ({expected_argc})"
    if expected_nstack is not None:
        assert proto.nstack == expected_nstack, \
            f"nstack: {proto.nstack} != {expected_nstack}"
    if name_str is not None:
        assert be_str2cstr(proto.name) == name_str, \
            f"name: {be_str2cstr(proto.name)!r} != {name_str!r}"


# ============================================================================
# Berry source strategies
# ============================================================================

_SIMPLE_BODIES = [
    "return 42",
    "return 0",
    "return -1",
    "return true",
    "return false",
    "return nil",
]

_UNARY_BODIES = [
    "return x + 1",
    "return x - 1",
    "return x * 2",
    "return -x",
    "return !x",
    "return x",
]

_BINARY_BODIES = [
    "return x + y",
    "return x - y",
    "return x * y",
    "return x < y",
    "return x == y",
    "return x != y",
    "return x >= y",
    "return x <= y",
]

_STRING_BODIES = [
    "return 'hello'",
    "return 'world'",
    "return ''",
    "return 'foo' + 'bar'",
]

_LOCAL_VAR_BODIES = [
    "var a = 1\n  return a",
    "var a = x + 1\n  var b = a * 2\n  return b",
    "var a = x\n  var b = y\n  return a + b",
]

_CONDITIONAL_BODIES = [
    "if x return 1 else return 0 end",
    "if x > 0 return x else return -x end",
    "if true return 1 else return 0 end",
]

_NESTED_FUNC_SOURCES = [
    # Closure capturing upvalue
    "def f(x)\n  def g()\n    return x\n  end\n  return g\nend",
    # Simple nested function (no capture)
    "def f()\n  def g()\n    return 1\n  end\n  return g\nend",
]

_LOOP_BODIES = [
    "var s = 0\n  for i : 1 .. x\n    s = s + i\n  end\n  return s",
    "var i = 0\n  while i < x\n    i = i + 1\n  end\n  return i",
]

_EXCEPTION_BODIES = [
    "try\n    return 1\n  except ..\n    return 0\n  end",
]


@composite
def simple_function_source(draw):
    body = draw(sampled_from(_SIMPLE_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def unary_function_source(draw):
    body = draw(sampled_from(_UNARY_BODIES))
    return f"def f(x)\n  {body}\nend"


@composite
def binary_function_source(draw):
    body = draw(sampled_from(_BINARY_BODIES))
    return f"def f(x, y)\n  {body}\nend"


@composite
def string_function_source(draw):
    body = draw(sampled_from(_STRING_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def local_var_function_source(draw):
    body = draw(sampled_from(_LOCAL_VAR_BODIES))
    return f"def f(x, y)\n  {body}\nend"


@composite
def conditional_function_source(draw):
    body = draw(sampled_from(_CONDITIONAL_BODIES))
    return f"def f(x)\n  {body}\nend"


@composite
def loop_function_source(draw):
    body = draw(sampled_from(_LOOP_BODIES))
    return f"def f(x)\n  {body}\nend"


@composite
def exception_function_source(draw):
    body = draw(sampled_from(_EXCEPTION_BODIES))
    return f"def f()\n  {body}\nend"


@composite
def any_function_source(draw):
    return draw(one_of(
        simple_function_source(),
        unary_function_source(),
        binary_function_source(),
        string_function_source(),
        local_var_function_source(),
        conditional_function_source(),
        loop_function_source(),
        exception_function_source(),
    ))


# ============================================================================
# Property 6a: Compilation determinism — same source always produces same proto
# ============================================================================

@settings(max_examples=100, deadline=30000)
@given(source=any_function_source())
def test_compilation_deterministic(source):
    """Compiling the same source twice must produce identical bytecode."""
    vm1, _, proto1_main = _compile_source(source)
    vm2, _, proto2_main = _compile_source(source)

    # Compare the inner function proto (first sub-proto)
    assert proto1_main.nproto == proto2_main.nproto, \
        f"nproto mismatch: {proto1_main.nproto} != {proto2_main.nproto}"

    if proto1_main.nproto > 0:
        p1 = proto1_main.ptab[0]
        p2 = proto2_main.ptab[0]
        assert p1.argc == p2.argc
        assert p1.nstack == p2.nstack
        assert p1.varg == p2.varg
        assert p1.codesize == p2.codesize
        for i in range(p1.codesize):
            assert p1.code[i] == p2.code[i], \
                f"code[{i}]: 0x{p1.code[i]:08X} != 0x{p2.code[i]:08X}"
        assert p1.nconst == p2.nconst
        assert p1.nupvals == p2.nupvals
        assert p1.nproto == p2.nproto


# ============================================================================
# Property 6b: Proto metadata consistency
# ============================================================================

@settings(max_examples=100, deadline=30000)
@given(source=any_function_source())
def test_proto_metadata_consistent(source):
    """Compiled proto must have consistent metadata fields."""
    vm, _, main_proto = _compile_source(source)
    _, inner_proto = _extract_inner_closure(vm, main_proto)
    if inner_proto is None:
        return  # skip if no inner function

    # nstack must be >= argc (need room for arguments)
    assert inner_proto.nstack >= inner_proto.argc, \
        f"nstack ({inner_proto.nstack}) < argc ({inner_proto.argc})"

    # codesize must be > 0 (at least a RET instruction)
    assert inner_proto.codesize > 0, "codesize must be > 0"

    # code list length must match codesize
    assert len(inner_proto.code) == inner_proto.codesize, \
        f"code length ({len(inner_proto.code)}) != codesize ({inner_proto.codesize})"

    # nconst must match ktab length
    if inner_proto.ktab:
        assert inner_proto.nconst == len(inner_proto.ktab), \
            f"nconst ({inner_proto.nconst}) != ktab length ({len(inner_proto.ktab)})"
    else:
        assert inner_proto.nconst == 0

    # nupvals must match upvals length
    if inner_proto.upvals:
        assert inner_proto.nupvals == len(inner_proto.upvals)
    else:
        assert inner_proto.nupvals == 0

    # nproto must match ptab length
    if inner_proto.ptab:
        assert inner_proto.nproto == len(inner_proto.ptab)
    else:
        assert inner_proto.nproto == 0

    # Function name should be 'f'
    assert be_str2cstr(inner_proto.name) == 'f', \
        f"Expected name 'f', got {be_str2cstr(inner_proto.name)!r}"


# ============================================================================
# Property 6c: Constant table types are correct
# ============================================================================

@settings(max_examples=100, deadline=30000)
@given(source=any_function_source())
def test_constant_types_valid(source):
    """All constants in the compiled proto must have valid Berry types."""
    vm, _, main_proto = _compile_source(source)
    _, inner_proto = _extract_inner_closure(vm, main_proto)
    if inner_proto is None or inner_proto.nconst == 0:
        return

    valid_types = {BE_INT, BE_REAL, BE_STRING, BE_NIL}
    for i in range(inner_proto.nconst):
        k = inner_proto.ktab[i]
        t = var_type(k)
        assert t in valid_types, \
            f"ktab[{i}] has unexpected type {t}"


# ============================================================================
# Property 6d: Instructions are valid 32-bit values with valid opcodes
# ============================================================================

@settings(max_examples=100, deadline=30000)
@given(source=any_function_source())
def test_instructions_valid(source):
    """All instructions must be valid 32-bit values with opcodes 0-47."""
    from berry_port.be_decoder import IGET_OP

    vm, _, main_proto = _compile_source(source)
    _, inner_proto = _extract_inner_closure(vm, main_proto)
    if inner_proto is None:
        return

    for i in range(inner_proto.codesize):
        ins = inner_proto.code[i]
        assert 0 <= ins <= 0xFFFFFFFF, \
            f"code[{i}] out of 32-bit range: {ins}"
        op = IGET_OP(ins)
        assert 0 <= op <= 47, \
            f"code[{i}] invalid opcode {op}: 0x{ins:08X}"


# ============================================================================
# Property 6e: Argc matches function parameter count
# ============================================================================

def test_argc_zero_params():
    """A function with no parameters should have argc=0."""
    vm, _, main_proto = _compile_source("def f()\n  return 1\nend")
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.argc == 0


def test_argc_one_param():
    """A function with one parameter should have argc=1."""
    vm, _, main_proto = _compile_source("def f(x)\n  return x\nend")
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.argc == 1


def test_argc_two_params():
    """A function with two parameters should have argc=2."""
    vm, _, main_proto = _compile_source("def f(x, y)\n  return x + y\nend")
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.argc == 2


def test_argc_three_params():
    """A function with three parameters should have argc=3."""
    vm, _, main_proto = _compile_source(
        "def f(a, b, c)\n  return a + b + c\nend"
    )
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.argc == 3


# ============================================================================
# Property 6f: Upvalue descriptors for closures
# ============================================================================

def test_closure_upvalue_descriptors():
    """A closure capturing an outer variable should have correct upvalue descriptors."""
    source = "def f(x)\n  def g()\n    return x\n  end\n  return g\nend"
    vm, _, main_proto = _compile_source(source)
    _, f_proto = _extract_inner_closure(vm, main_proto)
    assert f_proto is not None

    # f should have one sub-proto (g)
    assert f_proto.nproto == 1, f"Expected 1 sub-proto, got {f_proto.nproto}"
    g_proto = f_proto.ptab[0]

    # g captures x from f's stack, so it should have 1 upvalue
    assert g_proto.nupvals == 1, f"Expected 1 upval, got {g_proto.nupvals}"
    uv = g_proto.upvals[0]
    # instack=1 means captured from enclosing function's stack
    assert uv.instack == 1, f"Expected instack=1, got {uv.instack}"
    # idx=0 means first parameter (x) of enclosing function
    assert uv.idx == 0, f"Expected idx=0, got {uv.idx}"


def test_no_upvalues_for_simple_function():
    """A function with no captures should have zero upvalues."""
    source = "def f(x)\n  return x + 1\nend"
    vm, _, main_proto = _compile_source(source)
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.nupvals == 0


# ============================================================================
# Property 6g: Sub-proto hierarchy
# ============================================================================

def test_nested_function_creates_sub_proto():
    """A function containing a nested def should have a sub-proto."""
    source = "def f()\n  def g()\n    return 1\n  end\n  return g\nend"
    vm, _, main_proto = _compile_source(source)
    _, f_proto = _extract_inner_closure(vm, main_proto)
    assert f_proto.nproto == 1
    g_proto = f_proto.ptab[0]
    assert be_str2cstr(g_proto.name) == 'g'
    assert g_proto.argc == 0


def test_no_sub_protos_for_flat_function():
    """A function with no nested defs should have zero sub-protos."""
    source = "def f(x)\n  return x * 2\nend"
    vm, _, main_proto = _compile_source(source)
    _, p = _extract_inner_closure(vm, main_proto)
    assert p.nproto == 0


# ============================================================================
# Property 6h: Cross-validation with C binary via solidification output
# ============================================================================
# Solidification output encodes all proto fields: bytecode (hex), constants,
# upvalue descriptors, sub-protos, nstack, argc, varg. If the solidification
# output matches character-for-character, the compiled protos are equivalent.

_CROSS_VALIDATION_VECTORS = [
    ("return int", "def f()\n  return 42\nend"),
    ("return nil", "def f()\n  return nil\nend"),
    ("return true", "def f()\n  return true\nend"),
    ("return false", "def f()\n  return false\nend"),
    ("return string", "def f()\n  return 'hello'\nend"),
    ("identity", "def f(x)\n  return x\nend"),
    ("add const", "def f(x)\n  return x + 1\nend"),
    ("sub const", "def f(x)\n  return x - 1\nend"),
    ("mul const", "def f(x)\n  return x * 2\nend"),
    ("negate", "def f(x)\n  return -x\nend"),
    ("logical not", "def f(x)\n  return !x\nend"),
    ("binary add", "def f(x, y)\n  return x + y\nend"),
    ("binary sub", "def f(x, y)\n  return x - y\nend"),
    ("binary mul", "def f(x, y)\n  return x * y\nend"),
    ("less than", "def f(x, y)\n  return x < y\nend"),
    ("equal", "def f(x, y)\n  return x == y\nend"),
    ("not equal", "def f(x, y)\n  return x != y\nend"),
    ("greater equal", "def f(x, y)\n  return x >= y\nend"),
    ("less equal", "def f(x, y)\n  return x <= y\nend"),
    ("string concat", "def f()\n  return 'foo' + 'bar'\nend"),
    ("empty string", "def f()\n  return ''\nend"),
    ("local var", "def f(x)\n  var a = x + 1\n  return a\nend"),
    ("two locals", "def f(x)\n  var a = x + 1\n  var b = a * 2\n  return b\nend"),
    ("if-else return",
     "def f(x)\n  if x return 1 else return 0 end\nend"),
    ("if-gt return",
     "def f(x)\n  if x > 0 return x else return -x end\nend"),
    ("for loop",
     "def f(x)\n  var s = 0\n  for i : 1 .. x\n    s = s + i\n  end\n  return s\nend"),
    ("while loop",
     "def f(x)\n  var i = 0\n  while i < x\n    i = i + 1\n  end\n  return i\nend"),
    ("while with continue",
     "def f(x)\n  var i = 0\n  var s = 0\n  while i < x\n    i = i + 1\n    if i == 3 continue end\n    s = s + i\n  end\n  return s\nend"),
    ("minimal continue",
     "def f()\n  var i = 0\n  while i < 10\n    i = i + 1\n    continue\n  end\n  return i\nend"),
    ("try-except",
     "def f()\n  try\n    return 1\n  except ..\n    return 0\n  end\nend"),
    ("closure with upvalue",
     "def f(x)\n  def g()\n    return x\n  end\n  return g\nend"),
    ("nested no capture",
     "def f()\n  def g()\n    return 1\n  end\n  return g\nend"),
    ("three params",
     "def f(a, b, c)\n  return a + b + c\nend"),
    ("return zero", "def f()\n  return 0\nend"),
    ("return neg", "def f()\n  return -1\nend"),
    ("walrus operator",
     "def f(x)\n  var a\n  if (a := x)\n    return a\n  end\nend"),
]


if _c_binary_available():
    import pytest

    @pytest.mark.parametrize("desc,source", _CROSS_VALIDATION_VECTORS)
    def test_compilation_matches_c_binary(desc, source):
        """Cross-validate: Python port compilation must produce the same
        bytecode as the C implementation, verified via solidification output."""
        py_out = _solidify_python(source)
        c_out = _solidify_c(source)
        if c_out is None:
            pytest.skip("C binary solidification failed")
        py_norm = _strip_trailing_whitespace(py_out)
        c_norm = _strip_trailing_whitespace(c_out)
        assert py_norm == c_norm, (
            f"Compilation mismatch for '{desc}':\n"
            f"--- Python solidification ---\n{py_out}\n"
            f"--- C solidification ---\n{c_out}"
        )


# ============================================================================
# Property 6i: Hypothesis-driven cross-validation with C binary
# ============================================================================

if _c_binary_available():
    @settings(max_examples=50, deadline=30000)
    @given(source=any_function_source())
    def test_compilation_matches_c_hypothesis(source):
        """For any generated Berry function, the Python port's compilation
        must produce the same proto fields as the C implementation,
        verified via solidification output comparison."""
        py_out = _solidify_python(source)
        c_out = _solidify_c(source)
        assume(c_out is not None)
        py_norm = _strip_trailing_whitespace(py_out)
        c_norm = _strip_trailing_whitespace(c_out)
        assert py_norm == c_norm


# ============================================================================
# Property 6j: Instruction-level cross-validation
# ============================================================================
# Even without full solidification comparison, we can extract and compare
# individual proto fields from the solidification output.

if _c_binary_available():
    @settings(max_examples=50, deadline=30000)
    @given(source=any_function_source())
    def test_bytecode_instructions_match_c(source):
        """The compiled bytecode instructions must match between Python and C."""
        py_out = _solidify_python(source)
        c_out = _solidify_c(source)
        assume(c_out is not None)

        py_insts = _extract_instructions_from_solidify(py_out)
        c_insts = _extract_instructions_from_solidify(c_out)

        assert len(py_insts) == len(c_insts), (
            f"Instruction count mismatch: Python={len(py_insts)}, C={len(c_insts)}"
        )
        for i, (pi, ci) in enumerate(zip(py_insts, c_insts)):
            assert pi == ci, (
                f"Instruction {i} mismatch: Python=0x{pi:08X}, C=0x{ci:08X}"
            )

    @settings(max_examples=50, deadline=30000)
    @given(source=any_function_source())
    def test_metadata_matches_c(source):
        """Proto metadata (nstack, argc, varg) must match between Python and C."""
        py_out = _solidify_python(source)
        c_out = _solidify_c(source)
        assume(c_out is not None)

        assert _extract_nstack(py_out) == _extract_nstack(c_out), \
            f"nstack mismatch: Py={_extract_nstack(py_out)}, C={_extract_nstack(c_out)}"
        assert _extract_argc(py_out) == _extract_argc(c_out), \
            f"argc mismatch: Py={_extract_argc(py_out)}, C={_extract_argc(c_out)}"
        assert _extract_varg(py_out) == _extract_varg(c_out), \
            f"varg mismatch: Py={_extract_varg(py_out)}, C={_extract_varg(c_out)}"

    @settings(max_examples=50, deadline=30000)
    @given(source=any_function_source())
    def test_constants_match_c(source):
        """Integer constants in the compiled proto must match between Python and C."""
        py_out = _solidify_python(source)
        c_out = _solidify_c(source)
        assume(c_out is not None)

        py_ints = _extract_int_constants(py_out)
        c_ints = _extract_int_constants(c_out)
        assert py_ints == c_ints, \
            f"Int constants mismatch: Py={py_ints}, C={c_ints}"

        py_strs = _extract_string_constants(py_out)
        c_strs = _extract_string_constants(c_out)
        assert py_strs == c_strs, \
            f"String constants mismatch: Py={py_strs}, C={c_strs}"
