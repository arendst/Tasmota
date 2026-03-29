"""
Test: class constructor called via module attribute (NOT_METHOD + BE_CLASS path).

When a class with a constructor is stored as a module attribute and then
instantiated via `module.attr()`, the VM recall loop must account for the
`mode` offset when computing `var_idx` after `be_class_newobj`.  Without
the fix, `var_idx` pointed at the instance (BE_INSTANCE) instead of the
constructor closure, causing a spurious '() method not found' error.
"""

import sys
import os
import io

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest
from berry_port.berry import be_vm_new, be_dostring


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


# ============================================================================
# Regression tests for module-class constructor recall bug
# ============================================================================

_VECTORS = [
    (
        "basic constructor via module",
        "m = module('m')\n"
        "def f(x)\n"
        "  class A\n"
        "    var v\n"
        "    def init() self.v = 42 end\n"
        "  end\n"
        "  x.a = A\n"
        "  return m.a()\n"
        "end\n"
        "var o = f(m)\n"
        "assert(o.v == 42)\n"
        "print(o.v)",
        "42\n",
    ),
    (
        "constructor with arguments via module",
        "m = module('m')\n"
        "def f(x)\n"
        "  class B\n"
        "    var v\n"
        "    def init(val) self.v = val end\n"
        "  end\n"
        "  x.b = B\n"
        "  return m.b(99)\n"
        "end\n"
        "var o = f(m)\n"
        "print(o.v)",
        "99\n",
    ),
    (
        "no-constructor class via module",
        "m = module('m')\n"
        "def f(x)\n"
        "  class C var v end\n"
        "  x.c = C\n"
        "  return m.c()\n"
        "end\n"
        "var o = f(m)\n"
        "print(type(o))",
        "instance\n",
    ),
    (
        "inherited class with constructor via module",
        "class Base\n"
        "  var a\n"
        "  def init(x) self.a = x end\n"
        "end\n"
        "m = module('m')\n"
        "def f(x)\n"
        "  class D : Base\n"
        "    var b\n"
        "    def init(x, y) super(self).init(x) self.b = y end\n"
        "  end\n"
        "  x.d = D\n"
        "  return m.d(10, 20)\n"
        "end\n"
        "var o = f(m)\n"
        "print(o.a)\nprint(o.b)",
        "10\n20\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _VECTORS,
                         ids=[v[0] for v in _VECTORS])
def test_module_class_constructor(desc, source, expected):
    """Instantiating a class via a module attribute must invoke the constructor
    correctly, regardless of the NOT_METHOD mode offset."""
    rc, output = _run_python(source)
    assert rc == 0, f"[{desc}] be_dostring returned rc={rc}"
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )
