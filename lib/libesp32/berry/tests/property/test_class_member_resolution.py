"""
Feature: berry-python-port, Property 12: Class instance member resolution

For any class hierarchy (class with 0+ superclasses) and any member name,
looking up the member via be_instance_member_simple should find the same member
at the same level in the hierarchy and return the same type as the C
implementation. Instance variable layout (nvar fields) should match.

Specifically:
- Instance variables (MT_VARIABLE) resolve to the correct value from the
  owning instance's members array.
- Methods (MT_PRIMMETHOD, MT_METHOD) resolve through the class chain.
- Members defined on a subclass shadow same-named members on a superclass.
- The nvar count on each class matches the number of instance variables bound.
- The super/sub chain of instances mirrors the class hierarchy.

Validates: Requirements 10.4, 10.5
"""

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, lists, text, sampled_from, booleans,
    composite, just, one_of,
)

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_BOOL, BE_CLASS, BE_INSTANCE, BE_NONE,
    BE_FUNCTION, BE_NTVFUNC,
    MT_VARIABLE, MT_METHOD, MT_PRIMMETHOD,
    bgc, bstringtable, bvalue, bclass, binstance,
    var_setint, var_setnil, var_type, var_toint, var_toobj,
    var_isnil, var_tontvfunc,
)
from berry_port.be_class import (
    be_newclass, be_class_member_bind, be_class_native_method_bind,
    be_class_member, be_instance_member_simple, be_class_newobj,
    _newobject, _instance_member,
)
from berry_port.be_string import be_string_init, be_newstrn


# ---------------------------------------------------------------------------
# Minimal mock VM
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


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------
def _make_class(vm, name, super_=None, var_names=None, method_names=None):
    """Create a class with the given instance variables and native methods."""
    name_str = be_newstrn(vm, name, len(name))
    c = be_newclass(vm, name_str, super_)
    if var_names:
        for vn in var_names:
            vn_str = be_newstrn(vm, vn, len(vn))
            be_class_member_bind(vm, c, vn_str, True)
    if method_names:
        for mn in method_names:
            mn_str = be_newstrn(vm, mn, len(mn))
            be_class_native_method_bind(vm, c, mn_str, lambda vm: 0)
    return c


def _make_instance(vm, c):
    """Create an instance chain for class c (mirrors _newobject)."""
    return _newobject(vm, c)


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------
# Simple identifier-like names for members
member_names = text(
    alphabet="abcdefghijklmnopqrstuvwxyz_",
    min_size=1, max_size=8,
)

# Lists of unique member names
unique_var_names = lists(member_names, min_size=0, max_size=6, unique=True)
unique_method_names = lists(member_names, min_size=0, max_size=6, unique=True)



# ---------------------------------------------------------------------------
# Property 12a: nvar matches the number of instance variables bound
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(var_names=unique_var_names)
def test_nvar_matches_bound_variables(var_names):
    """A class's nvar field must equal the number of instance variables bound."""
    vm = _fresh_vm()
    c = _make_class(vm, "TestClass", var_names=var_names)
    assert c.nvar == len(var_names), (
        f"nvar={c.nvar}, expected {len(var_names)}"
    )


# ---------------------------------------------------------------------------
# Property 12b: Instance variable lookup resolves to correct slot
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(var_names=lists(member_names, min_size=1, max_size=6, unique=True))
def test_instance_variable_lookup(var_names):
    """Each instance variable should be resolvable and initially nil."""
    vm = _fresh_vm()
    c = _make_class(vm, "TestClass", var_names=var_names)
    inst = _make_instance(vm, c)

    for name in var_names:
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_instance_member_simple(vm, inst, name_str, dst)
        assert type_ == BE_NIL, (
            f"Variable '{name}' should resolve to nil (type={type_})"
        )


# ---------------------------------------------------------------------------
# Property 12c: Instance variable set and retrieve round-trip
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    var_names=lists(member_names, min_size=1, max_size=6, unique=True),
    values=lists(integers(min_value=-10000, max_value=10000), min_size=6, max_size=6),
)
def test_instance_variable_set_retrieve(var_names, values):
    """Setting an instance variable and reading it back should return the same value."""
    vm = _fresh_vm()
    c = _make_class(vm, "TestClass", var_names=var_names)
    inst = _make_instance(vm, c)

    # Set each variable to a distinct integer
    for i, name in enumerate(var_names):
        var_setint(inst.members[i], values[i])

    # Read back via be_instance_member_simple
    for i, name in enumerate(var_names):
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_instance_member_simple(vm, inst, name_str, dst)
        assert type_ == BE_INT, (
            f"Variable '{name}' type should be BE_INT, got {type_}"
        )
        assert var_toint(dst) == values[i], (
            f"Variable '{name}': got {var_toint(dst)}, expected {values[i]}"
        )


# ---------------------------------------------------------------------------
# Property 12d: Native method lookup returns MT_PRIMMETHOD type
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(method_names=lists(member_names, min_size=1, max_size=6, unique=True))
def test_native_method_lookup(method_names):
    """Native methods bound to a class should be resolvable on instances."""
    vm = _fresh_vm()
    c = _make_class(vm, "TestClass", method_names=method_names)
    inst = _make_instance(vm, c)

    for name in method_names:
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_instance_member_simple(vm, inst, name_str, dst)
        assert type_ == MT_PRIMMETHOD, (
            f"Method '{name}' should resolve to MT_PRIMMETHOD ({MT_PRIMMETHOD}), got {type_}"
        )


# ---------------------------------------------------------------------------
# Property 12e: Subclass members shadow superclass members
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    shared_names=lists(member_names, min_size=1, max_size=4, unique=True),
)
def test_subclass_shadows_superclass(shared_names):
    """When both a subclass and superclass define the same member name,
    the subclass version should be found first."""
    vm = _fresh_vm()

    # Superclass: bind shared names as native methods
    super_c = _make_class(vm, "Super", method_names=shared_names)

    # Subclass: bind same names as instance variables (shadows the methods)
    sub_c = _make_class(vm, "Sub", super_=super_c, var_names=shared_names)

    inst = _make_instance(vm, sub_c)

    # Set subclass instance vars to known values
    for i, name in enumerate(shared_names):
        var_setint(inst.members[i], 42 + i)

    # Lookup should find the subclass instance variable, not the superclass method
    for i, name in enumerate(shared_names):
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_instance_member_simple(vm, inst, name_str, dst)
        assert type_ == BE_INT, (
            f"'{name}' should resolve to subclass var (BE_INT), got {type_}"
        )
        assert var_toint(dst) == 42 + i


# ---------------------------------------------------------------------------
# Property 12f: Superclass-only members are reachable from subclass instances
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    super_methods=lists(member_names, min_size=1, max_size=4, unique=True),
    sub_vars=lists(member_names, min_size=0, max_size=4, unique=True),
)
def test_superclass_members_reachable(super_methods, sub_vars):
    """Members defined only on the superclass should be reachable from
    a subclass instance, as long as they aren't shadowed."""
    vm = _fresh_vm()

    # Filter out names that collide
    non_shadowed = [m for m in super_methods if m not in sub_vars]
    assume(len(non_shadowed) > 0)

    super_c = _make_class(vm, "Super", method_names=super_methods)
    sub_c = _make_class(vm, "Sub", super_=super_c, var_names=sub_vars)
    inst = _make_instance(vm, sub_c)

    for name in non_shadowed:
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_instance_member_simple(vm, inst, name_str, dst)
        assert type_ == MT_PRIMMETHOD, (
            f"Superclass method '{name}' should be reachable, got type={type_}"
        )


# ---------------------------------------------------------------------------
# Property 12g: Instance super/sub chain mirrors class hierarchy
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(depth=integers(min_value=1, max_value=5))
def test_instance_chain_mirrors_class_hierarchy(depth):
    """The instance super chain should have one instance per class in the
    hierarchy, and each instance's _class should match the corresponding class."""
    vm = _fresh_vm()

    # Build a chain of classes: C0 <- C1 <- ... <- C_{depth-1}
    classes = []
    prev = None
    for i in range(depth):
        name = f"C{i}"
        c = _make_class(vm, name, super_=prev, var_names=[f"v{i}"])
        classes.append(c)
        prev = c

    # The leaf class is the last one
    leaf = classes[-1]
    inst = _make_instance(vm, leaf)

    # Walk the instance super chain and verify it matches the class chain
    current = inst
    for i in range(depth - 1, -1, -1):
        assert current is not None, (
            f"Instance chain ended early at depth {depth - 1 - i}"
        )
        assert current._class is classes[i], (
            f"Instance at level {i} has wrong class"
        )
        assert current._class.nvar == 1, (
            f"Class C{i} should have nvar=1, got {current._class.nvar}"
        )
        current = current.super

    assert current is None, "Instance chain should end with None"


# ---------------------------------------------------------------------------
# Property 12h: Nonexistent member returns BE_NONE
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(var_names=unique_var_names)
def test_nonexistent_member_returns_none(var_names):
    """Looking up a member that doesn't exist should return BE_NONE (via nil)."""
    vm = _fresh_vm()
    c = _make_class(vm, "TestClass", var_names=var_names)
    inst = _make_instance(vm, c)

    # Use a name guaranteed not to be in var_names
    bogus = "zzz_nonexistent_zzz"
    assume(bogus not in var_names)

    dst = bvalue()
    name_str = be_newstrn(vm, bogus, len(bogus))
    type_ = be_instance_member_simple(vm, inst, name_str, dst)
    assert type_ == BE_NIL, (
        f"Nonexistent member should resolve to nil, got type={type_}"
    )


# ---------------------------------------------------------------------------
# Property 12i: be_class_member walks the class chain correctly
# ---------------------------------------------------------------------------
@settings(max_examples=200)
@given(
    super_methods=lists(member_names, min_size=1, max_size=4, unique=True),
    sub_methods=lists(member_names, min_size=1, max_size=4, unique=True),
)
def test_class_member_walks_chain(super_methods, sub_methods):
    """be_class_member should find members defined on the class or its super."""
    vm = _fresh_vm()

    non_shadowed = [m for m in super_methods if m not in sub_methods]
    assume(len(non_shadowed) > 0)

    super_c = _make_class(vm, "Super", method_names=super_methods)
    sub_c = _make_class(vm, "Sub", super_=super_c, method_names=sub_methods)

    # Subclass methods should be found
    for name in sub_methods:
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_class_member(vm, sub_c, name_str, dst)
        assert type_ == MT_PRIMMETHOD, (
            f"Sub method '{name}' not found via be_class_member"
        )

    # Non-shadowed superclass methods should also be found
    for name in non_shadowed:
        dst = bvalue()
        name_str = be_newstrn(vm, name, len(name))
        type_ = be_class_member(vm, sub_c, name_str, dst)
        assert type_ == MT_PRIMMETHOD, (
            f"Super method '{name}' not found via be_class_member on sub"
        )


# ---------------------------------------------------------------------------
# Edge case: empty class (no members at all)
# ---------------------------------------------------------------------------
def test_empty_class_instance():
    """An instance of a class with no members should have nvar=0 and
    all lookups should return nil."""
    vm = _fresh_vm()
    c = _make_class(vm, "Empty")
    assert c.nvar == 0

    inst = _make_instance(vm, c)
    assert inst._class is c
    assert inst.super is None
    assert len(inst.members) == 0

    dst = bvalue()
    name_str = be_newstrn(vm, "anything", 8)
    type_ = be_instance_member_simple(vm, inst, name_str, dst)
    assert type_ == BE_NIL


# ---------------------------------------------------------------------------
# Edge case: deep inheritance with variable at each level
# ---------------------------------------------------------------------------
def test_deep_inheritance_variable_isolation():
    """Variables at different levels of the hierarchy should be independent."""
    vm = _fresh_vm()

    # Grandparent with var 'x'
    gp = _make_class(vm, "GP", var_names=["x"])
    # Parent with var 'x' (shadows grandparent)
    p = _make_class(vm, "P", super_=gp, var_names=["x"])
    # Child with var 'x' (shadows parent)
    ch = _make_class(vm, "Ch", super_=p, var_names=["x"])

    inst = _make_instance(vm, ch)

    # Set child's x=100, parent's x=200, grandparent's x=300
    var_setint(inst.members[0], 100)
    var_setint(inst.super.members[0], 200)
    var_setint(inst.super.super.members[0], 300)

    # Lookup should find child's x (100) since it shadows the rest
    dst = bvalue()
    name_str = be_newstrn(vm, "x", 1)
    type_ = be_instance_member_simple(vm, inst, name_str, dst)
    assert type_ == BE_INT
    assert var_toint(dst) == 100
