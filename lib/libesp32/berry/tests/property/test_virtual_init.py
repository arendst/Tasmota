"""
Test: virtual 'init' on classes without an explicit constructor.

When a class does not define an `init` method, accessing `.init` on an
instance must resolve to a synthetic empty constructor (a native function)
rather than raising `attribute_error`.

Two related bugs caused `tests/super_auto.be` to fail on the Python port
while passing on the C build:

1. `be_instance_member` extracted the attribute-name string via
   `name.s if hasattr(name, 's') else ""`, which always yielded `""` for
   `bstring` objects (bstring stores the data in `_s`, only `bcstring` has
   `s`). The `"init"` synthetic-constructor branch therefore never fired.
2. `_be_default_init_native_function` was a placeholder that returned 0
   instead of delegating to the real implementation in be_vm.

The fix: look up the C string via `be_str2cstr(name)`, and delegate the
placeholder to `be_vm.be_default_init_native_function`.

Regression test for that two-part fix.
"""

import sys
import os
import io

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

import pytest

from berry_port.berry import be_vm_new, be_dostring
from berry_port.be_object import (
    BE_NIL, BE_NONE, BE_NTVFUNC,
    bgc, bstringtable, bvalue,
    var_type, var_tontvfunc, var_primetype,
)
from berry_port.be_class import (
    be_newclass, be_class_member_bind,
    be_instance_member, _newobject,
)
from berry_port.be_string import be_string_init, be_newstrn, be_str2cstr
from berry_port.be_vm import be_default_init_native_function


# ---------------------------------------------------------------------------
# Minimal mock VM for direct be_instance_member calls
# ---------------------------------------------------------------------------
class MockVM:
    def __init__(self):
        self.gc = bgc()
        self.gc.usage = 0
        self.strtab = bstringtable()
        self.compopt = 0


def _fresh_vm():
    vm = MockVM()
    be_string_init(vm)
    return vm


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
# Unit tests: be_instance_member synthesizes a native init()
# ============================================================================

def test_virtual_init_on_empty_class():
    """Accessing 'init' on an instance of a class without an explicit init
    must return a native function, not BE_NONE."""
    vm = _fresh_vm()
    cname = be_newstrn(vm, "Z", 1)
    c = be_newclass(vm, cname)
    inst = _newobject(vm, c)

    dst = bvalue()
    name = be_newstrn(vm, "init", 4)
    type_ = be_instance_member(vm, inst, name, dst)

    # Must not be BE_NONE — that would trigger attribute_error in the VM
    assert type_ != BE_NONE, (
        "be_instance_member returned BE_NONE for 'init' on class without init"
    )
    # Must be a native function
    assert var_type(dst) == BE_NTVFUNC, (
        f"expected BE_NTVFUNC ({BE_NTVFUNC}), got {var_type(dst)}"
    )
    # The stored callable must be callable and, when invoked, must delegate
    # to be_vm.be_default_init_native_function (not the placeholder that
    # previously returned 0 unconditionally).
    fn = var_tontvfunc(dst)
    assert callable(fn), "synthesized 'init' is not callable"

    # Verify the delegation by spying on be_vm.be_default_init_native_function.
    import berry_port.be_vm as _be_vm
    calls = []
    real = _be_vm.be_default_init_native_function

    def spy(vm_arg):
        calls.append(vm_arg)
        return real(vm_arg)

    _be_vm.be_default_init_native_function = spy
    try:
        # Need a minimal call frame so the real function can use vm.cf.
        # Build a spy VM that carries the attributes the real function touches.
        from berry_port.be_object import bvalue as _bvalue, bcallframe
        call_vm = _fresh_vm()
        call_vm.stack = [_bvalue() for _ in range(4)]
        call_vm.reg_idx = 0
        call_vm.top_idx = 0
        call_vm.cf = bcallframe()
        call_vm.cf.func = 0
        fn(call_vm)
    finally:
        _be_vm.be_default_init_native_function = real
    assert len(calls) == 1, (
        "synthesized 'init' did not delegate to be_vm.be_default_init_native_function"
    )


def test_non_init_attribute_still_returns_none():
    """The synthetic-constructor shortcut must only fire for the name 'init',
    not for other missing attributes."""
    vm = _fresh_vm()
    cname = be_newstrn(vm, "Z", 1)
    c = be_newclass(vm, cname)
    inst = _newobject(vm, c)

    dst = bvalue()
    name = be_newstrn(vm, "bogus_attr_xyz", 14)
    type_ = be_instance_member(vm, inst, name, dst)
    assert type_ == BE_NONE, (
        f"missing attribute should return BE_NONE, got {type_}"
    )


def test_be_str2cstr_on_short_string_matches_content():
    """Guard against regression of the bstring._s vs bcstring.s mixup:
    be_str2cstr on a short bstring must return the original content."""
    vm = _fresh_vm()
    name = be_newstrn(vm, "init", 4)
    assert be_str2cstr(name) == "init"


def test_explicit_init_not_shadowed_by_virtual():
    """When a class defines its own `init`, the lookup must return that
    member (as an instance variable slot in this direct-binding case),
    not the synthetic native function."""
    vm = _fresh_vm()
    cname = be_newstrn(vm, "HasInit", 7)
    c = be_newclass(vm, cname)
    # Bind 'init' as an instance variable so the lookup has something to
    # find before it would fall through to the synthetic constructor.
    init_name = be_newstrn(vm, "init", 4)
    be_class_member_bind(vm, c, init_name, True)
    inst = _newobject(vm, c)

    dst = bvalue()
    type_ = be_instance_member(vm, inst, init_name, dst)
    # The bound instance variable is initialized to nil, so we get BE_NIL,
    # not a native function.
    assert type_ == BE_NIL, (
        f"explicit 'init' binding should shadow the synthetic one, got type={type_}"
    )
    assert var_type(dst) != BE_NTVFUNC, (
        "explicit 'init' must not be replaced by the synthetic native function"
    )


# ============================================================================
# End-to-end tests mirroring tests/super_auto.be
# ============================================================================

_VECTORS = [
    (
        "z.init resolves to a function",
        "class Z end\n"
        "z = Z()\n"
        "print(type(z.init))",
        "function\n",
    ),
    (
        "z.init is not nil",
        "class Z end\n"
        "z = Z()\n"
        "print(z.init != nil)",
        "true\n",
    ),
    (
        "z.init() is a no-op that does not raise",
        "class Z end\n"
        "z = Z()\n"
        "z.init()\n"
        "print('ok')",
        "ok\n",
    ),
    (
        "super(self).init() chain works when superclass has no init",
        "class A2 static a = 1 end\n"
        "class B2 : A2\n"
        "  var b\n"
        "  def init(a, b) super(self).init(a) self.b = b end\n"
        "end\n"
        "var o = B2(10, 20)\n"
        "print(o.b)",
        "20\n",
    ),
    (
        "three-level super chain with missing init on root",
        "class A3 end\n"
        "class B3 : A3\n"
        "  var b\n"
        "  def init(a, b) super(self).init() self.b = b end\n"
        "end\n"
        "class C3 : B3\n"
        "  var c\n"
        "  def init(a, b, c) super(self).init(a, b) self.c = c end\n"
        "end\n"
        "var o = C3(1, 2, 3)\n"
        "print(o.b, o.c)",
        "2 3\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _VECTORS,
                         ids=[v[0] for v in _VECTORS])
def test_virtual_init_end_to_end(desc, source, expected):
    """End-to-end scenarios exercising the virtual `init` path from Berry."""
    rc, output = _run_python(source)
    assert rc == 0, f"[{desc}] be_dostring returned rc={rc}, output={output!r}"
    assert output == expected, (
        f"[{desc}] output mismatch:\n"
        f"  expected: {expected!r}\n"
        f"  got:      {output!r}"
    )


# ============================================================================
# Full super_auto.be regression
# ============================================================================

def test_super_auto_script_runs_clean():
    """The full tests/super_auto.be script must run to completion with all
    assertions passing. This is the original failing scenario."""
    here = os.path.dirname(__file__)
    script = os.path.join(here, '..', 'super_auto.be')
    with open(script, 'r') as f:
        source = f.read()
    rc, output = _run_python(source)
    assert rc == 0, (
        f"tests/super_auto.be failed with rc={rc}\noutput:\n{output}"
    )
