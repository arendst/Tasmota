"""
Test: variadic function arguments (*args).

When a function is defined with `def f(a, *l)`, the `*l` parameter should
capture all extra arguments as a proper list instance.  The fix in
prep_closure() uses be_newobject(vm, "list") to create a real list instance
instead of a raw blist, so the vararg is printable, indexable, and supports
list methods like .size(), .item(), iteration, etc.
"""

import sys
import os
import io

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_dostring


def _run(source):
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


# ============================================================================
# Basic vararg capture
# ============================================================================

_BASIC_VECTORS = [
    (
        "vararg captures extra args",
        "def f(a, *l) print(l) end f(1,2,3,4)",
        "[2, 3, 4]\n",
    ),
    (
        "vararg with no extra args gives empty list",
        "def f(a, *l) print(l) end f(1)",
        "[]\n",
    ),
    (
        "all params are vararg",
        "def f(*l) print(l) end f(1,2,3)",
        "[1, 2, 3]\n",
    ),
    (
        "vararg with no args at all",
        "def f(*l) print(l) end f()",
        "[]\n",
    ),
    (
        "single extra arg",
        "def f(a, *l) print(l) end f(10, 20)",
        "[20]\n",
    ),
    (
        "multiple fixed args before vararg",
        "def f(a, b, c, *l) print(l) end f(1,2,3,4,5,6)",
        "[4, 5, 6]\n",
    ),
    (
        "multiple fixed args, no extras",
        "def f(a, b, c, *l) print(l) end f(1,2,3)",
        "[]\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _BASIC_VECTORS,
                         ids=[v[0] for v in _BASIC_VECTORS])
def test_vararg_basic(desc, source, expected):
    rc, output = _run(source)
    assert rc == 0, f"[{desc}] rc={rc}"
    assert output == expected, f"[{desc}] expected {expected!r}, got {output!r}"


# ============================================================================
# Vararg is a proper list instance (supports methods)
# ============================================================================

_INSTANCE_VECTORS = [
    (
        "vararg .size()",
        "def f(a, *l) print(l.size()) end f(1,2,3,4)",
        "3\n",
    ),
    (
        "vararg type is instance",
        "def f(*l) print(type(l)) end f(1)",
        "instance\n",
    ),
    (
        "vararg classname is list",
        "def f(*l) print(classname(l)) end f(1)",
        "list\n",
    ),
    (
        "vararg indexing with .item()",
        "def f(a, *l) print(l.item(0)) print(l.item(1)) end f(10,20,30)",
        "20\n30\n",
    ),
    (
        "vararg indexing with []",
        "def f(a, *l) print(l[0]) print(l[1]) end f(10,20,30)",
        "20\n30\n",
    ),
    (
        "vararg .push() works",
        "def f(*l) l.push(99) print(l) end f(1,2)",
        "[1, 2, 99]\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _INSTANCE_VECTORS,
                         ids=[v[0] for v in _INSTANCE_VECTORS])
def test_vararg_instance(desc, source, expected):
    rc, output = _run(source)
    assert rc == 0, f"[{desc}] rc={rc}"
    assert output == expected, f"[{desc}] expected {expected!r}, got {output!r}"


# ============================================================================
# Vararg iteration
# ============================================================================

def test_vararg_for_loop():
    source = (
        "def f(*l)\n"
        "  for x : l\n"
        "    print(x)\n"
        "  end\n"
        "end\n"
        "f(10, 20, 30)"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "10\n20\n30\n"


# ============================================================================
# Vararg with mixed types
# ============================================================================

def test_vararg_mixed_types():
    source = (
        "def f(a, *l) print(l) end\n"
        "f(1, 'hello', true, nil, 3.14)"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "['hello', true, nil, 3.14]\n"


# ============================================================================
# Vararg in methods
# ============================================================================

def test_vararg_in_method():
    source = (
        "class A\n"
        "  def f(*l)\n"
        "    print(l)\n"
        "  end\n"
        "end\n"
        "var a = A()\n"
        "a.f(1, 2, 3)"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "[1, 2, 3]\n"


def test_vararg_method_no_extras():
    source = (
        "class A\n"
        "  def f(*l)\n"
        "    print(l.size())\n"
        "  end\n"
        "end\n"
        "var a = A()\n"
        "a.f()"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "0\n"


# ============================================================================
# Vararg with closures
# ============================================================================

def test_vararg_captured_by_closure():
    source = (
        "def f(*l)\n"
        "  return def () return l end\n"
        "end\n"
        "var g = f(5, 6, 7)\n"
        "print(g())"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "[5, 6, 7]\n"


# ============================================================================
# Vararg passed to another function
# ============================================================================

def test_vararg_passed_as_argument():
    source = (
        "def show(x) print(x) end\n"
        "def f(*l) show(l) end\n"
        "f(10, 20)"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "[10, 20]\n"


# ============================================================================
# Nested vararg calls
# ============================================================================

def test_nested_vararg_calls():
    source = (
        "def inner(*l) return l.size() end\n"
        "def outer(*l) return inner(l[0], l[1]) end\n"
        "print(outer(1, 2, 3))"
    )
    rc, output = _run(source)
    assert rc == 0
    assert output == "2\n"
