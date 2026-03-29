"""
Feature: berry-python-port, Property 7: VM execution equivalence

For any valid Berry program, executing it on the Python VM should produce
the same observable output (printed strings, return values, exceptions) as
the C VM.

We validate this by:
  a) Running curated Berry programs on the Python port and checking output
  b) Cross-validating against the C binary for the same programs
  c) Using Hypothesis to generate Berry expressions and compare outputs

Validates: Requirements 6.4, 24.3
"""

import sys
import os
import io
import subprocess
import tempfile

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    sampled_from, composite, one_of, integers, lists, booleans,
)

from berry_port.berry import be_vm_new, be_dostring


# ============================================================================
# Helpers
# ============================================================================

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
        try:
            os.unlink(tmppath)
        except OSError:
            pass


# ============================================================================
# Test vectors: (description, berry_source, expected_output)
# ============================================================================

_ARITHMETIC_VECTORS = [
    ("add ints", "print(1 + 2)", "3\n"),
    ("multiply ints", "print(3 * 4)", "12\n"),
    ("subtract ints", "print(10 - 3)", "7\n"),
    ("negate int", "print(-5)", "-5\n"),
    ("add reals", "print(1.5 + 2.5)", "4\n"),
    ("multiply real", "print(2.0 * 3.0)", "6\n"),
    ("int zero", "print(0)", "0\n"),
    ("large int", "print(1000000)", "1000000\n"),
]

_BOOLEAN_VECTORS = [
    ("true", "print(true)", "true\n"),
    ("false", "print(false)", "false\n"),
    ("and true", "print(true && true)", "true\n"),
    ("and false", "print(true && false)", "false\n"),
    ("or true", "print(false || true)", "true\n"),
    ("or false", "print(false || false)", "false\n"),
    ("not false", "var x = !false\nprint(x)", "true\n"),
    ("not true", "var x = !true\nprint(x)", "false\n"),
]

_COMPARISON_VECTORS = [
    ("lt true", "print(1 < 2)", "true\n"),
    ("lt false", "print(2 < 1)", "false\n"),
    ("le true", "print(1 <= 1)", "true\n"),
    ("gt true", "print(2 > 1)", "true\n"),
    ("ge true", "print(1 >= 1)", "true\n"),
    ("eq true", "print(1 == 1)", "true\n"),
    ("eq false", "print(1 == 2)", "false\n"),
    ("neq true", "print(1 != 2)", "true\n"),
    ("neq false", "print(1 != 1)", "false\n"),
]

_STRING_VECTORS = [
    ("simple string", "print('hello')", "hello\n"),
    ("string concat", "print('foo' + 'bar')", "foobar\n"),
    ("empty string", "print('')", "\n"),
    ("string with spaces", "print('hello world')", "hello world\n"),
    ("str function", "print(str(42))", "42\n"),
    ("str bool", "print(str(true))", "true\n"),
    ("str nil", "print(str(nil))", "nil\n"),
]

_VARIABLE_VECTORS = [
    ("local var", "var x = 42\nprint(x)", "42\n"),
    ("var reassign", "var x = 1\nx = 2\nprint(x)", "2\n"),
    ("multiple vars", "var a = 1\nvar b = 2\nprint(a + b)", "3\n"),
    ("var string", "var s = 'hi'\nprint(s)", "hi\n"),
]

_CONTROL_FLOW_VECTORS = [
    ("if true", "if true print('yes') end", "yes\n"),
    ("if false else", "if false print('no') else print('else') end", "else\n"),
    ("if elif", "var x = 2\nif x == 1\nprint('one')\nelif x == 2\nprint('two')\nend", "two\n"),
    ("while loop", "var i = 0\nwhile i < 3\nprint(i)\ni = i + 1\nend", "0\n1\n2\n"),
    ("nested if", "var x = 10\nif x > 5\nif x > 8\nprint('big')\nend\nend", "big\n"),
    ("while break", "var i = 0\nwhile true\nif i >= 3 break end\nprint(i)\ni = i + 1\nend", "0\n1\n2\n"),
]

_FUNCTION_VECTORS = [
    ("simple call", "def f(x) return x + 1 end\nprint(f(41))", "42\n"),
    ("two args", "def f(x, y) return x + y end\nprint(f(3, 4))", "7\n"),
    ("no args", "def f() return 99 end\nprint(f())", "99\n"),
    ("nested call", "def f(x) return x * 2 end\ndef g(x) return f(x) + 1 end\nprint(g(5))", "11\n"),
    ("recursive", "def fib(n) if n <= 1 return n end return fib(n-1) + fib(n-2) end\nprint(fib(10))", "55\n"),
    ("lambda", "var f = /x-> x * 2\nprint(f(21))", "42\n"),
    ("lambda no arg", "var f = /-> 7\nprint(f())", "7\n"),
]

_CONTAINER_VECTORS = [
    ("list literal", "var l = [1, 2, 3]\nprint(l)", "[1, 2, 3]\n"),
    ("empty list", "var l = []\nprint(l)", "[]\n"),
    ("list index", "var l = [10, 20, 30]\nprint(l[1])", "20\n"),
    ("list size", "var l = [1, 2, 3]\nprint(size(l))", "3\n"),
    ("map literal", "var m = {'a': 1}\nprint(m['a'])", "1\n"),
]

_CLASS_VECTORS = [
    ("class init+method",
     "class C\nvar x\ndef init(x) self.x = x end\ndef get() return self.x end\nend\n"
     "var c = C(42)\nprint(c.get())", "42\n"),
    ("class no args",
     "class C\ndef hello() return 'hi' end\nend\nvar c = C()\nprint(c.hello())", "hi\n"),
    ("isinstance",
     "class C end\nvar c = C()\nprint(isinstance(c, C))", "true\n"),
    ("classname",
     "class C end\nvar c = C()\nprint(classname(c))", "C\n"),
    ("class static method",
     "class C\nstatic def greet() return 'hello' end\nend\nprint(C.greet())", "hello\n"),
    ("class static var",
     "class C\nstatic var count = 10\nend\nprint(C.count)", "10\n"),
    ("class static method with args",
     "class C\nstatic def add(a, b) return a + b end\nend\nprint(C.add(3, 4))", "7\n"),
    ("class static via subclass",
     "class Base\nstatic def who() return 'base' end\nend\n"
     "class Sub : Base\nend\nprint(Sub.who())", "base\n"),
]

_MISC_VECTORS = [
    ("nil print", "print(nil)", "nil\n"),
    ("type int", "print(type(42))", "int\n"),
    ("type string", "print(type('hi'))", "string\n"),
    ("type bool", "print(type(true))", "bool\n"),
    ("type nil", "print(type(nil))", "nil\n"),
    ("multi print", "print(1, 2, 3)", "1 2 3\n"),
    ("print real int", "print(4.0)", "4\n"),
    ("assert pass", "assert(true)", ""),
    ("assert expr", "assert(1 + 1 == 2)", ""),
]

_ALL_VECTORS = (
    _ARITHMETIC_VECTORS
    + _BOOLEAN_VECTORS
    + _COMPARISON_VECTORS
    + _STRING_VECTORS
    + _VARIABLE_VECTORS
    + _CONTROL_FLOW_VECTORS
    + _FUNCTION_VECTORS
    + _CONTAINER_VECTORS
    + _CLASS_VECTORS
    + _MISC_VECTORS
)


# ============================================================================
# Property 7a: Python port produces correct output for curated programs
# ============================================================================

import pytest


@pytest.mark.parametrize("desc,source,expected", _ALL_VECTORS,
                         ids=[v[0] for v in _ALL_VECTORS])
def test_python_port_output(desc, source, expected):
    """The Python port must produce the expected output for each program."""
    rc, output = _run_python(source)
    assert rc == 0, (
        f"[{desc}] be_dostring returned rc={rc}, expected 0"
    )
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )


# ============================================================================
# Property 7b: Python port output matches C binary for curated programs
# ============================================================================

if _c_binary_available():
    @pytest.mark.parametrize("desc,source,expected", _ALL_VECTORS,
                             ids=[v[0] for v in _ALL_VECTORS])
    def test_output_matches_c_binary(desc, source, expected):
        """Python port output must match C binary output."""
        py_rc, py_out = _run_python(source)
        c_result = _run_c(source)
        if c_result is None:
            pytest.skip("C binary not available or timed out")
        c_rc, c_out = c_result
        # Both should succeed
        assert py_rc == 0, f"[{desc}] Python port failed with rc={py_rc}"
        assert c_rc == 0, f"[{desc}] C binary failed with rc={c_rc}"
        # Output must match
        assert py_out == c_out, (
            f"[{desc}] output mismatch:\n"
            f"  Python: {py_out!r}\n"
            f"  C:      {c_out!r}"
        )


# ============================================================================
# Property 7c: be_dostring returns 0 for valid programs
# ============================================================================

_VALID_PROGRAMS = [
    "var x = 1",
    "var x = 'hello'",
    "def f() end",
    "class C end",
    "if true end",
    "while false end",
    "var l = []",
    "var m = {}",
    "assert(true)",
    "var x = 1 + 2 + 3",
    "def f(x, y) return x end\nf(1, 2)",
]


@pytest.mark.parametrize("source", _VALID_PROGRAMS)
def test_valid_programs_return_zero(source):
    """be_dostring must return 0 for all valid Berry programs."""
    rc, _ = _run_python(source)
    assert rc == 0, f"Expected rc=0 for {source!r}, got {rc}"


# ============================================================================
# Property 7d: Syntax errors return non-zero
# ============================================================================

_INVALID_PROGRAMS = [
    "def",
    "if",
    "class",
    "var = 1",
    "1 +",
    "def f( end",
    "var x = [",
]


@pytest.mark.parametrize("source", _INVALID_PROGRAMS)
def test_syntax_errors_return_nonzero(source):
    """be_dostring must return non-zero for syntax errors."""
    rc, _ = _run_python(source)
    assert rc != 0, f"Expected non-zero rc for invalid source {source!r}"


# ============================================================================
# Property 7e: Deterministic execution — same program, same output
# ============================================================================

@pytest.mark.parametrize("desc,source,expected", _ALL_VECTORS[:15],
                         ids=[v[0] for v in _ALL_VECTORS[:15]])
def test_execution_deterministic(desc, source, expected):
    """Running the same program twice must produce identical output."""
    rc1, out1 = _run_python(source)
    rc2, out2 = _run_python(source)
    assert rc1 == rc2, f"[{desc}] rc mismatch: {rc1} vs {rc2}"
    assert out1 == out2, f"[{desc}] output mismatch: {out1!r} vs {out2!r}"


# ============================================================================
# Hypothesis strategies for generating Berry expressions
# ============================================================================

_INT_LITERALS = [str(i) for i in [-100, -1, 0, 1, 2, 42, 100, 999]]
_REAL_LITERALS = ["0.0", "1.0", "1.5", "2.5", "3.14", "-1.0", "0.5"]
_BOOL_LITERALS = ["true", "false"]
_STRING_LITERALS = ["'hello'", "'world'", "''", "'foo'", "'bar'", "'test'"]
_NIL_LITERAL = ["nil"]

_UNARY_OPS = ["-"]
_BINARY_INT_OPS = ["+", "-", "*"]
_COMPARISON_OPS = ["<", "<=", ">", ">=", "==", "!="]
_LOGICAL_OPS = ["&&", "||"]


@composite
def int_expression(draw):
    """Generate a Berry integer expression and its expected print output."""
    a = draw(sampled_from([-100, -10, -1, 0, 1, 2, 5, 10, 42, 100]))
    b = draw(sampled_from([-10, -1, 1, 2, 3, 5, 7, 10]))
    op = draw(sampled_from(_BINARY_INT_OPS))
    if op == '+':
        expected = a + b
    elif op == '-':
        expected = a - b
    elif op == '*':
        expected = a * b
    else:
        return None  # shouldn't happen
    source = f"print({a} {op} {b})"
    return source, str(expected) + "\n"


@composite
def comparison_expression(draw):
    """Generate a Berry comparison expression and its expected output."""
    a = draw(sampled_from([-10, -1, 0, 1, 2, 5, 10, 42]))
    b = draw(sampled_from([-10, -1, 0, 1, 2, 5, 10, 42]))
    op = draw(sampled_from(_COMPARISON_OPS))
    if op == '<':
        result = a < b
    elif op == '<=':
        result = a <= b
    elif op == '>':
        result = a > b
    elif op == '>=':
        result = a >= b
    elif op == '==':
        result = a == b
    elif op == '!=':
        result = a != b
    else:
        return None
    expected = "true" if result else "false"
    source = f"print({a} {op} {b})"
    return source, expected + "\n"


@composite
def logical_expression(draw):
    """Generate a Berry logical expression and its expected output."""
    a = draw(sampled_from(_BOOL_LITERALS))
    b = draw(sampled_from(_BOOL_LITERALS))
    op = draw(sampled_from(_LOGICAL_OPS))
    a_val = a == "true"
    b_val = b == "true"
    if op == '&&':
        result = a_val and b_val
    else:
        result = a_val or b_val
    expected = "true" if result else "false"
    source = f"print({a} {op} {b})"
    return source, expected + "\n"


@composite
def string_concat_expression(draw):
    """Generate a Berry string concatenation and its expected output."""
    # Use simple known strings to avoid escaping issues
    strings = draw(lists(
        sampled_from(["hello", "world", "foo", "bar", "", "test", "abc"]),
        min_size=1, max_size=4,
    ))
    berry_parts = [f"'{s}'" for s in strings]
    source = "print(" + " + ".join(berry_parts) + ")"
    expected = "".join(strings) + "\n"
    return source, expected


@composite
def negate_expression(draw):
    """Generate a Berry negation expression and its expected output."""
    a = draw(sampled_from([-100, -1, 0, 1, 42, 100]))
    source = f"print(-({a}))"
    expected = str(-a) + "\n"
    return source, expected


@composite
def any_simple_expression(draw):
    """Generate any simple Berry expression with expected output."""
    return draw(one_of(
        int_expression(),
        comparison_expression(),
        logical_expression(),
        string_concat_expression(),
        negate_expression(),
    ))


# ============================================================================
# Property 7f: Hypothesis-driven — Python port produces correct output
# ============================================================================

@settings(max_examples=200, deadline=30000)
@given(expr_and_expected=any_simple_expression())
def test_generated_expression_output(expr_and_expected):
    """For any generated Berry expression, the Python port must produce
    the mathematically correct output."""
    source, expected = expr_and_expected
    rc, output = _run_python(source)
    assert rc == 0, f"be_dostring failed for {source!r} with rc={rc}"
    assert output == expected, (
        f"Output mismatch for {source!r}:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )


# ============================================================================
# Property 7g: Hypothesis-driven cross-validation with C binary
# ============================================================================

if _c_binary_available():
    @settings(max_examples=100, deadline=30000)
    @given(expr_and_expected=any_simple_expression())
    def test_generated_expression_matches_c(expr_and_expected):
        """For any generated Berry expression, Python and C output must match."""
        source, expected = expr_and_expected
        py_rc, py_out = _run_python(source)
        assume(py_rc == 0)
        c_result = _run_c(source)
        assume(c_result is not None)
        c_rc, c_out = c_result
        assume(c_rc == 0)
        assert py_out == c_out, (
            f"Output mismatch for {source!r}:\n"
            f"  Python: {py_out!r}\n"
            f"  C:      {c_out!r}"
        )


# ============================================================================
# Property 7h: Multi-statement programs produce correct output
# ============================================================================

_MULTI_STATEMENT_VECTORS = [
    ("var and print",
     "var x = 10\nvar y = 20\nprint(x + y)",
     "30\n"),
    ("function composition",
     "def double(x) return x * 2 end\n"
     "def inc(x) return x + 1 end\n"
     "print(double(inc(5)))",
     "12\n"),
    ("while accumulator",
     "var sum = 0\nvar i = 1\n"
     "while i <= 5\nsum = sum + i\ni = i + 1\nend\n"
     "print(sum)",
     "15\n"),
    ("conditional chain",
     "def classify(x)\n"
     "if x > 0 return 'positive'\n"
     "elif x < 0 return 'negative'\n"
     "else return 'zero'\n"
     "end\nend\n"
     "print(classify(5))\nprint(classify(-3))\nprint(classify(0))",
     "positive\nnegative\nzero\n"),
    ("list operations",
     "var l = []\nl.push(1)\nl.push(2)\nl.push(3)\nprint(l)\nprint(l.size())",
     "[1, 2, 3]\n3\n"),
    ("class with methods",
     "class Counter\nvar n\n"
     "def init() self.n = 0 end\n"
     "def inc() self.n = self.n + 1 end\n"
     "def value() return self.n end\n"
     "end\n"
     "var c = Counter()\nc.inc()\nc.inc()\nc.inc()\nprint(c.value())",
     "3\n"),
    ("string type check",
     "print(type('hello'))\nprint(type(42))\nprint(type(true))\nprint(type(nil))",
     "string\nint\nbool\nnil\n"),
    ("multiple returns",
     "def max(a, b) if a > b return a else return b end end\n"
     "print(max(3, 7))\nprint(max(10, 2))",
     "7\n10\n"),
    ("nested function calls",
     "def f(x) return x + 1 end\n"
     "print(f(f(f(0))))",
     "3\n"),
    ("boolean short circuit and",
     "var x = 0\n"
     "def side() x = x + 1 return true end\n"
     "var r = false && side()\n"
     "print(x)",
     "0\n"),
    ("boolean short circuit or",
     "var x = 0\n"
     "def side() x = x + 1 return false end\n"
     "var r = true || side()\n"
     "print(x)",
     "0\n"),
]


@pytest.mark.parametrize("desc,source,expected", _MULTI_STATEMENT_VECTORS,
                         ids=[v[0] for v in _MULTI_STATEMENT_VECTORS])
def test_multi_statement_output(desc, source, expected):
    """Multi-statement Berry programs must produce correct output."""
    rc, output = _run_python(source)
    assert rc == 0, f"[{desc}] be_dostring returned rc={rc}"
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )


if _c_binary_available():
    @pytest.mark.parametrize("desc,source,expected", _MULTI_STATEMENT_VECTORS,
                             ids=[v[0] for v in _MULTI_STATEMENT_VECTORS])
    def test_multi_statement_matches_c(desc, source, expected):
        """Multi-statement program output must match C binary."""
        py_rc, py_out = _run_python(source)
        c_result = _run_c(source)
        if c_result is None:
            pytest.skip("C binary not available")
        c_rc, c_out = c_result
        assert py_rc == 0, f"[{desc}] Python port failed rc={py_rc}"
        assert c_rc == 0, f"[{desc}] C binary failed rc={c_rc}"
        assert py_out == c_out, (
            f"[{desc}] output mismatch:\n"
            f"  Python: {py_out!r}\n"
            f"  C:      {c_out!r}"
        )


# ============================================================================
# Property 7i: Hypothesis-driven multi-statement programs
# ============================================================================

@composite
def var_print_program(draw):
    """Generate: var x = <expr>; print(x)"""
    val = draw(sampled_from([-99, -1, 0, 1, 42, 100, 999]))
    source = f"var x = {val}\nprint(x)"
    return source, str(val) + "\n"


@composite
def function_call_program(draw):
    """Generate: def f(x) return x <op> <val> end; print(f(<arg>))"""
    op = draw(sampled_from(["+", "-", "*"]))
    val = draw(sampled_from([1, 2, 3, 5, 10]))
    arg = draw(sampled_from([0, 1, 2, 5, 10, 20]))
    if op == '+':
        expected = arg + val
    elif op == '-':
        expected = arg - val
    else:
        expected = arg * val
    source = f"def f(x) return x {op} {val} end\nprint(f({arg}))"
    return source, str(expected) + "\n"


@composite
def if_else_program(draw):
    """Generate: if <cond> print(<a>) else print(<b>) end"""
    cond = draw(booleans())
    a = draw(sampled_from([1, 42, 99]))
    b = draw(sampled_from([0, -1, -99]))
    cond_str = "true" if cond else "false"
    expected = a if cond else b
    source = f"if {cond_str} print({a}) else print({b}) end"
    return source, str(expected) + "\n"


@composite
def while_sum_program(draw):
    """Generate: sum 1..n using while loop"""
    n = draw(integers(min_value=0, max_value=20))
    expected = n * (n + 1) // 2
    source = (
        f"var sum = 0\nvar i = 1\n"
        f"while i <= {n}\nsum = sum + i\ni = i + 1\nend\n"
        f"print(sum)"
    )
    return source, str(expected) + "\n"


@composite
def any_program(draw):
    return draw(one_of(
        var_print_program(),
        function_call_program(),
        if_else_program(),
        while_sum_program(),
    ))


@settings(max_examples=200, deadline=30000)
@given(prog_and_expected=any_program())
def test_generated_program_output(prog_and_expected):
    """For any generated Berry program, the Python port must produce
    the correct output."""
    source, expected = prog_and_expected
    rc, output = _run_python(source)
    assert rc == 0, f"be_dostring failed for generated program with rc={rc}"
    assert output == expected, (
        f"Output mismatch for generated program:\n"
        f"  source:   {source!r}\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )


if _c_binary_available():
    @settings(max_examples=100, deadline=30000)
    @given(prog_and_expected=any_program())
    def test_generated_program_matches_c(prog_and_expected):
        """For any generated Berry program, Python and C output must match."""
        source, expected = prog_and_expected
        py_rc, py_out = _run_python(source)
        assume(py_rc == 0)
        c_result = _run_c(source)
        assume(c_result is not None)
        c_rc, c_out = c_result
        assume(c_rc == 0)
        assert py_out == c_out, (
            f"Output mismatch:\n"
            f"  source: {source!r}\n"
            f"  Python: {py_out!r}\n"
            f"  C:      {c_out!r}"
        )


# ============================================================================
# Known failing tests — xfail until underlying VM bugs are fixed
# ============================================================================

_KNOWN_FAILING_VECTORS = [
    (
        "for loop",
        "for i : 0 .. 2\nprint(i)\nend",
        "0\n1\n2\n",
    ),
    (
        "closure",
        "def make(x) return def () return x end end\nvar f = make(99)\nprint(f())",
        "99\n",
    ),
    (
        "try-except",
        "try\nraise 'oops', 'msg'\nexcept .. as e, m\nprint(e)\nprint(m)\nend",
        "oops\nmsg\n",
    ),
    (
        "range sum",
        "var s = 0\nfor i : 1 .. 5\ns = s + i\nend\nprint(s)",
        "15\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _KNOWN_FAILING_VECTORS)
def test_known_failing_programs(desc, source, expected):
    """Programs that previously failed due to VM bugs, now fixed."""
    rc, output = _run_python(source)
    assert rc == 0, f"[{desc}] be_dostring returned rc={rc}"
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )
