"""
Feature: berry-python-port, Property 9: Bytecode serialization round-trip

For any bproto produced by the compiler, saving it to a bytecode file with
be_bytecode_save and loading it back with be_bytecode_load should reconstruct
an equivalent bproto (same bytecode, constants, upvals, sub-protos).

Validates: Requirements 11.2, 11.3
"""

import sys
import os
import io
import struct
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from hypothesis import given, settings, assume
from hypothesis.strategies import (
    integers, floats, text, lists, booleans, composite,
    sampled_from, just, one_of, none,
)

from berry_port.be_object import (
    BE_NIL, BE_INT, BE_REAL, BE_STRING, BE_PROTO, BE_CLOSURE,
    bvalue, bproto, bclosure, bupvaldesc,
    var_setint, var_setreal, var_setstr, var_setnil,
    var_type, var_toint, var_toreal, var_tostr,
)
from berry_port.be_string import be_newstr, be_str2cstr
from berry_port.be_func import be_newproto, be_newclosure
from berry_port.be_vm import be_vm_new
from berry_port.be_bytecode import (
    be_bytecode_save_to_fs, be_bytecode_load_from_fs,
)
from berry_port.be_decoder import ISET_OP, ISET_RA, ISET_RKB, ISET_RKC
from berry_port.berry_conf import BE_USE_SINGLE_FLOAT


# ---------------------------------------------------------------------------
# Strategies
# ---------------------------------------------------------------------------

# Valid instruction: random 32-bit value (any opcode is fine for serialization)
instructions = integers(min_value=0, max_value=0xFFFFFFFF)

# Safe identifier-like strings for proto names (non-empty ASCII, no null bytes)
safe_strings = text(
    alphabet='abcdefghijklmnopqrstuvwxyz_0123456789',
    min_size=1, max_size=20,
)

# Integer constants within 64-bit signed range
int_constants = integers(min_value=-(1 << 31), max_value=(1 << 31) - 1)

# Real constants — finite floats only (NaN/Inf don't round-trip reliably via struct)
# When BE_USE_SINGLE_FLOAT is enabled, values must survive float32 round-trip.
if BE_USE_SINGLE_FLOAT:
    _raw_reals = floats(
        min_value=-1e15, max_value=1e15,
        allow_nan=False, allow_infinity=False,
    ).map(lambda r: struct.unpack('<f', struct.pack('<f', r))[0])
    real_constants = _raw_reals
else:
    real_constants = floats(
        min_value=-1e15, max_value=1e15,
        allow_nan=False, allow_infinity=False,
    )

# Upvalue descriptors
upval_instack = integers(min_value=0, max_value=1)
upval_idx = integers(min_value=0, max_value=255)


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def create_vm():
    """Create a fresh VM for testing."""
    return be_vm_new()


def make_proto(vm, name, source, argc, nstack, varg,
               code_list, int_consts, real_consts, str_consts,
               upval_descs, sub_protos=None):
    """Build a bproto with the given fields."""
    proto = be_newproto(vm)
    proto.name = be_newstr(vm, name)
    proto.source = be_newstr(vm, source)
    proto.argc = argc
    proto.nstack = nstack
    proto.varg = varg

    # Bytecode
    proto.code = list(code_list)
    proto.codesize = len(code_list)

    # Constants: ints, then reals, then strings
    ktab = []
    for iv in int_consts:
        v = bvalue()
        var_setint(v, iv)
        ktab.append(v)
    for rv in real_consts:
        v = bvalue()
        var_setreal(v, rv)
        ktab.append(v)
    for sv in str_consts:
        v = bvalue()
        var_setstr(v, be_newstr(vm, sv))
        ktab.append(v)
    proto.ktab = ktab
    proto.nconst = len(ktab)

    # Upvalue descriptors
    uvs = []
    for instack, idx in upval_descs:
        uv = bupvaldesc()
        uv.instack = instack
        uv.idx = idx
        uvs.append(uv)
    proto.upvals = uvs
    proto.nupvals = len(uvs)

    # Sub-protos
    if sub_protos:
        proto.ptab = sub_protos
        proto.nproto = len(sub_protos)
    else:
        proto.ptab = None
        proto.nproto = 0

    return proto


def assert_proto_equal(original, loaded, path="root"):
    """Recursively compare two bproto objects for equivalence."""
    # Name
    assert be_str2cstr(loaded.name) == be_str2cstr(original.name), \
        f"{path}: name mismatch: {be_str2cstr(loaded.name)!r} != {be_str2cstr(original.name)!r}"

    # Metadata
    assert loaded.argc == original.argc, \
        f"{path}: argc mismatch: {loaded.argc} != {original.argc}"
    assert loaded.nstack == original.nstack, \
        f"{path}: nstack mismatch: {loaded.nstack} != {original.nstack}"
    assert loaded.varg == original.varg, \
        f"{path}: varg mismatch: {loaded.varg} != {original.varg}"

    # Bytecode
    assert loaded.codesize == original.codesize, \
        f"{path}: codesize mismatch: {loaded.codesize} != {original.codesize}"
    for i in range(original.codesize):
        assert loaded.code[i] == original.code[i], \
            f"{path}: code[{i}] mismatch: {loaded.code[i]:#010x} != {original.code[i]:#010x}"

    # Constants
    assert loaded.nconst == original.nconst, \
        f"{path}: nconst mismatch: {loaded.nconst} != {original.nconst}"
    for i in range(original.nconst):
        ov = original.ktab[i]
        lv = loaded.ktab[i]
        assert var_type(lv) == var_type(ov), \
            f"{path}: ktab[{i}] type mismatch: {var_type(lv)} != {var_type(ov)}"
        if var_type(ov) == BE_INT:
            assert var_toint(lv) == var_toint(ov), \
                f"{path}: ktab[{i}] int mismatch: {var_toint(lv)} != {var_toint(ov)}"
        elif var_type(ov) == BE_REAL:
            assert var_toreal(lv) == var_toreal(ov), \
                f"{path}: ktab[{i}] real mismatch: {var_toreal(lv)} != {var_toreal(ov)}"
        elif var_type(ov) == BE_STRING:
            assert be_str2cstr(var_tostr(lv)) == be_str2cstr(var_tostr(ov)), \
                f"{path}: ktab[{i}] string mismatch"

    # Upvalue descriptors
    assert loaded.nupvals == original.nupvals, \
        f"{path}: nupvals mismatch: {loaded.nupvals} != {original.nupvals}"
    for i in range(original.nupvals):
        assert loaded.upvals[i].instack == original.upvals[i].instack, \
            f"{path}: upvals[{i}].instack mismatch"
        assert loaded.upvals[i].idx == original.upvals[i].idx, \
            f"{path}: upvals[{i}].idx mismatch"

    # Sub-protos
    assert loaded.nproto == original.nproto, \
        f"{path}: nproto mismatch: {loaded.nproto} != {original.nproto}"
    for i in range(original.nproto):
        assert_proto_equal(original.ptab[i], loaded.ptab[i],
                           path=f"{path}.ptab[{i}]")



def save_and_load_proto(vm, proto):
    """Save a proto to an in-memory buffer and load it back, returning the loaded closure."""
    # Wrap proto in a closure (save_proto expects a proto, load returns a closure)
    buf = io.BytesIO()
    be_bytecode_save_to_fs(vm, buf, proto)

    # Seek back to start for reading
    buf.seek(0)
    loaded_cl = be_bytecode_load_from_fs(vm, buf)
    return loaded_cl


# ---------------------------------------------------------------------------
# Hypothesis composite strategies for generating random protos
# ---------------------------------------------------------------------------

@composite
def proto_data(draw, allow_sub_protos=True, depth=0):
    """Generate random data for building a bproto."""
    name = draw(safe_strings)
    source = draw(safe_strings)
    argc = draw(integers(min_value=0, max_value=8))
    nstack = draw(integers(min_value=max(argc, 1), max_value=32))
    varg = draw(integers(min_value=0, max_value=3))

    # Generate instructions — avoid OP_GETGBL (6) and OP_SETGBL (7) opcodes
    # which trigger global index fixup during load
    n_code = draw(integers(min_value=1, max_value=10))
    code = []
    for _ in range(n_code):
        # Use opcodes that don't trigger global fixup (not 6 or 7)
        op = draw(sampled_from([0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15]))
        ra = draw(integers(min_value=0, max_value=min(nstack - 1, 255)))
        rkb = draw(integers(min_value=0, max_value=511))
        rkc = draw(integers(min_value=0, max_value=511))
        ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
        code.append(ins)

    # Constants
    n_int = draw(integers(min_value=0, max_value=5))
    n_real = draw(integers(min_value=0, max_value=5))
    n_str = draw(integers(min_value=0, max_value=5))
    ints = [draw(int_constants) for _ in range(n_int)]
    reals = [draw(real_constants) for _ in range(n_real)]
    strs = [draw(safe_strings) for _ in range(n_str)]

    # Upvalue descriptors
    n_upvals = draw(integers(min_value=0, max_value=4))
    upvals = [(draw(upval_instack), draw(upval_idx)) for _ in range(n_upvals)]

    # Sub-protos (limit depth to avoid explosion)
    sub_proto_data = []
    if allow_sub_protos and depth < 2:
        n_sub = draw(integers(min_value=0, max_value=2))
        for _ in range(n_sub):
            sub_proto_data.append(draw(proto_data(allow_sub_protos=(depth < 1),
                                                   depth=depth + 1)))

    return {
        'name': name, 'source': source,
        'argc': argc, 'nstack': nstack, 'varg': varg,
        'code': code, 'ints': ints, 'reals': reals, 'strs': strs,
        'upvals': upvals, 'sub_protos': sub_proto_data,
    }


def build_proto_from_data(vm, data):
    """Recursively build a bproto from generated data dict."""
    sub_protos = []
    for sp_data in data['sub_protos']:
        sub_protos.append(build_proto_from_data(vm, sp_data))

    return make_proto(
        vm,
        name=data['name'],
        source=data['source'],
        argc=data['argc'],
        nstack=data['nstack'],
        varg=data['varg'],
        code_list=data['code'],
        int_consts=data['ints'],
        real_consts=data['reals'],
        str_consts=data['strs'],
        upval_descs=data['upvals'],
        sub_protos=sub_protos if sub_protos else None,
    )


# ---------------------------------------------------------------------------
# Property 9a: Basic round-trip — flat proto with int/real/string constants
# ---------------------------------------------------------------------------
@settings(max_examples=100, deadline=30000)
@given(data=proto_data(allow_sub_protos=False))
def test_flat_proto_roundtrip(data):
    """A flat proto (no sub-protos) should survive save/load unchanged."""
    vm = create_vm()
    original = build_proto_from_data(vm, data)

    loaded_cl = save_and_load_proto(vm, original)
    assert loaded_cl is not None, "be_bytecode_load_from_fs returned None"
    assert loaded_cl.proto is not None, "loaded closure has no proto"

    assert_proto_equal(original, loaded_cl.proto)


# ---------------------------------------------------------------------------
# Property 9b: Round-trip with nested sub-protos
# ---------------------------------------------------------------------------
@settings(max_examples=50, deadline=30000)
@given(data=proto_data(allow_sub_protos=True, depth=0))
def test_nested_proto_roundtrip(data):
    """A proto with nested sub-protos should survive save/load unchanged."""
    vm = create_vm()
    original = build_proto_from_data(vm, data)

    loaded_cl = save_and_load_proto(vm, original)
    assert loaded_cl is not None
    assert loaded_cl.proto is not None

    assert_proto_equal(original, loaded_cl.proto)


# ---------------------------------------------------------------------------
# Property 9c: Empty proto round-trip (no code, no constants, no upvals)
# ---------------------------------------------------------------------------
def test_minimal_proto_roundtrip():
    """A proto with minimal content (1 instruction, no constants) should round-trip."""
    vm = create_vm()
    proto = make_proto(
        vm, name="minimal", source="test.be",
        argc=0, nstack=1, varg=0,
        code_list=[0x00000000],  # single NOP-like instruction (OP_ADD with all zeros)
        int_consts=[], real_consts=[], str_consts=[],
        upval_descs=[],
    )

    loaded_cl = save_and_load_proto(vm, proto)
    assert loaded_cl is not None
    assert_proto_equal(proto, loaded_cl.proto)


# ---------------------------------------------------------------------------
# Property 9d: Byte-level determinism — saving the same proto twice produces
#              identical byte streams
# ---------------------------------------------------------------------------
@settings(max_examples=50, deadline=30000)
@given(data=proto_data(allow_sub_protos=True, depth=0))
def test_save_determinism(data):
    """Saving the same proto twice should produce identical byte streams."""
    vm = create_vm()
    proto = build_proto_from_data(vm, data)

    buf1 = io.BytesIO()
    be_bytecode_save_to_fs(vm, buf1, proto)

    buf2 = io.BytesIO()
    be_bytecode_save_to_fs(vm, buf2, proto)

    assert buf1.getvalue() == buf2.getvalue(), \
        "Two saves of the same proto produced different byte streams"


# ---------------------------------------------------------------------------
# Property 9e: Round-trip preserves constant types exactly
# ---------------------------------------------------------------------------
@settings(max_examples=100, deadline=30000)
@given(
    n_ints=integers(min_value=0, max_value=8),
    n_reals=integers(min_value=0, max_value=8),
    n_strs=integers(min_value=0, max_value=8),
)
def test_constant_types_preserved(n_ints, n_reals, n_strs):
    """Each constant type (int, real, string) should be preserved through round-trip."""
    assume(n_ints + n_reals + n_strs > 0)
    vm = create_vm()

    ints = [i * 1000 - 500 for i in range(n_ints)]
    reals = [float(i) * 3.14 for i in range(n_reals)]
    if BE_USE_SINGLE_FLOAT:
        # Round through float32 so values survive single-precision serialization
        reals = [struct.unpack('<f', struct.pack('<f', r))[0] for r in reals]
    strs = [f"str_{i}" for i in range(n_strs)]

    proto = make_proto(
        vm, name="const_test", source="test.be",
        argc=0, nstack=1, varg=0,
        code_list=[0x00000000],
        int_consts=ints, real_consts=reals, str_consts=strs,
        upval_descs=[],
    )

    loaded_cl = save_and_load_proto(vm, proto)
    loaded = loaded_cl.proto

    # Verify types in order: ints, then reals, then strings
    idx = 0
    for i in range(n_ints):
        assert var_type(loaded.ktab[idx]) == BE_INT, \
            f"Expected BE_INT at index {idx}, got {var_type(loaded.ktab[idx])}"
        assert var_toint(loaded.ktab[idx]) == ints[i]
        idx += 1
    for i in range(n_reals):
        assert var_type(loaded.ktab[idx]) == BE_REAL, \
            f"Expected BE_REAL at index {idx}, got {var_type(loaded.ktab[idx])}"
        assert var_toreal(loaded.ktab[idx]) == reals[i]
        idx += 1
    for i in range(n_strs):
        assert var_type(loaded.ktab[idx]) == BE_STRING, \
            f"Expected BE_STRING at index {idx}, got {var_type(loaded.ktab[idx])}"
        assert be_str2cstr(var_tostr(loaded.ktab[idx])) == strs[i]
        idx += 1


# ---------------------------------------------------------------------------
# Property 9f: Upvalue descriptors survive round-trip
# ---------------------------------------------------------------------------
@settings(max_examples=100, deadline=30000)
@given(
    n_upvals=integers(min_value=1, max_value=16),
)
def test_upval_descriptors_roundtrip(n_upvals):
    """Upvalue descriptors (instack, idx) should be preserved through round-trip."""
    vm = create_vm()

    upvals = [(i % 2, i) for i in range(n_upvals)]

    proto = make_proto(
        vm, name="upval_test", source="test.be",
        argc=0, nstack=1, varg=0,
        code_list=[0x00000000],
        int_consts=[], real_consts=[], str_consts=[],
        upval_descs=upvals,
    )

    loaded_cl = save_and_load_proto(vm, proto)
    loaded = loaded_cl.proto

    assert loaded.nupvals == n_upvals
    for i in range(n_upvals):
        assert loaded.upvals[i].instack == upvals[i][0], \
            f"upvals[{i}].instack mismatch"
        assert loaded.upvals[i].idx == upvals[i][1], \
            f"upvals[{i}].idx mismatch"
