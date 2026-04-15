"""
Berry re (regex) module.
Mirrors: Tasmota lib/libesp32/berry_tasmota/src/be_re_lib.c

This module implements the Berry `re` module using Python's native `re` module
instead of the C re1.5 engine. The API matches the Berry re module:
  re.compile, re.search, re.match, re.match2, re.searchall, re.matchall, re.split

The re_pattern class provides: search, match, match2, searchall, matchall, split

Original C code is included as comments for key functions.
"""

import struct

from berry_port import be_api
from berry_port import be_byteslib
from berry_port import re15_compiler
from berry_port import re15_vm
from berry_port import re15_classes
from berry_port import re15_opcodes


# ============================================================================
# Internal helpers
# ============================================================================

def _do_match_search(vm, prog, hay, is_anchored, size_only):
    """Core match/search logic using the re1.5 VM.

    Returns the end position of the match (index into hay) or None.
    Pushes a list of match groups onto the Berry stack if matched, or nil.

    prog: ByteProg instance (compiled regex bytecode)
    is_anchored: True for match (must match from start), False for search
    size_only: True for match2 (first element is match length instead of string)
    """
    nsubp = (prog.sub + 1) * 2
    subp = [None] * nsubp

    result = re15_vm.recursiveloopprog(prog, hay, subp, nsubp, is_anchored)

    if result == 0:
        be_api.be_pushnil(vm)
        return None

    # Build the result list: [full_match, group1, group2, ...]
    be_api.be_newobject(vm, "list")

    # First element: full match (or length for match2)
    if size_only:
        match_len = (subp[1] - subp[0]) if subp[0] is not None and subp[1] is not None else 0
        be_api.be_pushint(vm, match_len)
    else:
        if subp[0] is not None and subp[1] is not None:
            be_api.be_pushstring(vm, hay[subp[0]:subp[1]])
        else:
            be_api.be_pushnil(vm)
    be_api.be_data_push(vm, -2)
    be_api.be_pop(vm, 1)

    # Sub-groups (group 1 through group N)
    for i in range(1, prog.sub + 1):
        si = 2 * i
        ei = 2 * i + 1
        if si < nsubp and ei < nsubp and subp[si] is not None and subp[ei] is not None:
            be_api.be_pushstring(vm, hay[subp[si]:subp[ei]])
        else:
            be_api.be_pushnil(vm)
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)

    be_api.be_pop(vm, 1)  # remove list, keep underlying object
    return subp[1] if subp[1] is not None else None


def _compile_pattern(regex_str):
    """Compile a regex string into a ByteProg using the re1.5 compiler."""
    prog, err = re15_compiler.compilecode(regex_str)
    if err != re15_opcodes.RE1_5_SUCCESS:
        return None
    return prog


# ============================================================================
# re module functions
# ============================================================================

# Berry: `re.compile(pattern:string) -> instance(re_pattern)`
#
# int be_re_compile(bvm *vm) {
#     int32_t argc = be_top(vm);
#     if (argc >= 1 && be_isstring(vm, 1)) {
#         const char * regex_str = be_tostring(vm, 1);
#         ...
#         be_pushntvclass(vm, &be_class_re_pattern);
#         be_call(vm, 0);
#         be_newcomobj(vm, code, &be_commonobj_destroy_generic);
#         be_setmember(vm, -2, "_p");
#         be_pop(vm, 1);
#         be_return(vm);
#     }
#     be_raise(vm, "type_error", NULL);
# }
def be_re_compile(vm):
    argc = be_api.be_top(vm)
    if argc >= 1 and be_api.be_isstring(vm, 1):
        regex_str = be_api.be_tostring(vm, 1)

        # Validate with sizecode first (matches C: sz < 0 → error)
        sz = re15_compiler.sizecode(regex_str)
        if sz < 0:
            be_api.be_raise(vm, "internal_error", "error in regex")

        # Compile to ByteProg (matches C: ret != 0 → error)
        prog, ret = re15_compiler.compilecode(regex_str)
        if ret != re15_opcodes.RE1_5_SUCCESS:
            be_api.be_raise(vm, "internal_error", "error in regex")

        # Create an instance of re_pattern class and store the compiled ByteProg
        be_api.be_getglobal(vm, "re_pattern")
        be_api.be_call(vm, 0)
        be_api.be_pushcomptr(vm, prog)
        be_api.be_setmember(vm, -2, "_p")
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


# Berry: `re.search(pattern, payload [, offset]) -> list or nil`
#
# int be_re_search(bvm *vm) {
#     return be_re_match_search(vm, bfalse, bfalse);
# }
def be_re_search(vm):
    return _re_match_search(vm, is_anchored=False, size_only=False)


# Berry: `re.match(pattern, payload [, offset]) -> list or nil`
#
# int be_re_match(bvm *vm) {
#     return be_re_match_search(vm, btrue, bfalse);
# }
def be_re_match(vm):
    return _re_match_search(vm, is_anchored=True, size_only=False)


# Berry: `re.match2(pattern, payload [, offset]) -> list or nil`
#
# int be_re_match2(bvm *vm) {
#     return be_re_match_search(vm, btrue, btrue);
# }
def be_re_match2(vm):
    return _re_match_search(vm, is_anchored=True, size_only=True)


# Berry: `re.searchall(pattern, payload [, limit]) -> list of lists`
def be_re_search_all(vm):
    return _re_match_search_all(vm, is_anchored=False)


# Berry: `re.matchall(pattern, payload [, limit]) -> list of lists`
def be_re_match_all(vm):
    return _re_match_search_all(vm, is_anchored=True)


# Berry: `re.split(pattern, payload [, limit]) -> list of strings`
#
# int be_re_split(bvm *vm) {
#     ...
#     int ret = re_pattern_split_run(vm, code, hay, split_limit);
#     ...
# }
def be_re_split(vm):
    argc = be_api.be_top(vm)
    if argc >= 2 and (be_api.be_isstring(vm, 1) or be_byteslib.be_isbytes(vm, 1)) and be_api.be_isstring(vm, 2):
        hay = be_api.be_tostring(vm, 2)
        split_limit = -1
        if argc >= 3:
            split_limit = be_api.be_toint(vm, 3)

        pattern = _get_pattern_from_arg(vm, 1)
        if pattern is None:
            be_api.be_raise(vm, "internal_error", "error in regex")

        return _split_run(vm, pattern, hay, split_limit)
    be_api.be_raise(vm, "type_error", None)


# Berry: `re.dump(b:bytes) -> nil` — disassemble compiled regex bytecode
def be_re_dump(vm):
    if not be_byteslib.be_isbytes(vm, 1):
        be_api.be_raise(vm, "type_error", "bytes required")
    bufptr, length = be_byteslib.be_tobytes(vm, 1)
    if bufptr is None or length < 12:
        return be_api.be_returnnilvalue(vm)
    data = bytes(bufptr[:length])
    bytelen, prog_len, sub = struct.unpack('<iii', data[:12])
    prog = re15_opcodes.ByteProg()
    prog.bytelen = bytelen
    prog.len = prog_len
    prog.sub = sub
    prog.insts = bytearray(data[12:])
    output = re15_classes.dumpcode(prog)
    print(output, end='')
    return be_api.be_returnnilvalue(vm)


# Berry: `re.compilebytes(pattern:string) -> bytes`
# In the Python port, compilebytes returns a comptr wrapping the compiled pattern
# since we don't have the re1.5 bytecode format. We store it as a comptr.
def be_re_compilebytes(vm):
    argc = be_api.be_top(vm)
    if argc >= 1 and be_api.be_isstring(vm, 1):
        regex_str = be_api.be_tostring(vm, 1)

        # Validate with sizecode first (matches C: sz < 0 → error)
        sz = re15_compiler.sizecode(regex_str)
        if sz < 0:
            be_api.be_raise(vm, "internal_error", "error in regex")

        # Compile to ByteProg
        prog, ret = re15_compiler.compilecode(regex_str)
        if ret != re15_opcodes.RE1_5_SUCCESS:
            be_api.be_raise(vm, "internal_error", "error in regex")

        # Serialize ByteProg as raw bytes: 3 little-endian int32 header + insts
        # Matches C memory layout: struct { int bytelen; int len; int sub; char insts[0]; }
        header = struct.pack('<iii', prog.bytelen, prog.len, prog.sub)
        raw = header + bytes(prog.insts[:prog.bytelen])
        be_byteslib.be_pushbytes(vm, raw, len(raw))
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


# ============================================================================
# Internal: _re_match_search — shared logic for re.match/re.search/re.match2
# ============================================================================

# int be_re_match_search(bvm *vm, bbool is_anchored, bbool size_only) {
#     int32_t argc = be_top(vm);
#     if (argc >= 2 && (be_isstring(vm, 1) || be_isbytes(vm, 1)) && be_isstring(vm, 2)) {
#         const char * hay = be_tostring(vm, 2);
#         ByteProg *code = NULL;
#         int32_t offset = 0;
#         if (argc >= 3 && be_isint(vm, 3)) { offset = be_toint(vm, 3); }
#         int32_t hay_len = strlen(hay);
#         if (offset < 0) { offset = 0; }
#         if (offset >= hay_len) { be_return_nil(vm); }
#         hay += offset;
#         ...compile or use pre-compiled...
#         be_re_match_search_run(vm, code, hay, is_anchored, size_only);
#         ...cleanup...
#         be_return(vm);
#     }
#     be_raise(vm, "type_error", NULL);
# }
def _re_match_search(vm, is_anchored, size_only):
    argc = be_api.be_top(vm)
    if argc >= 2 and (be_api.be_isstring(vm, 1) or be_byteslib.be_isbytes(vm, 1)) and be_api.be_isstring(vm, 2):
        hay = be_api.be_tostring(vm, 2)
        offset = 0
        if argc >= 3 and be_api.be_isint(vm, 3):
            offset = be_api.be_toint(vm, 3)
        hay_len = len(hay)
        if offset < 0:
            offset = 0
        if offset >= hay_len:
            return be_api.be_returnnilvalue(vm)
        hay = hay[offset:]

        # Get or compile pattern
        pattern = _get_pattern_from_arg(vm, 1)
        if pattern is None:
            be_api.be_raise(vm, "type_error", None)

        _do_match_search(vm, pattern, hay, is_anchored, size_only)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


# ============================================================================
# Internal: _re_match_search_all — shared logic for re.searchall/re.matchall
# ============================================================================

# int be_re_match_search_all(bvm *vm, bbool is_anchored) {
#     ...
#     be_newobject(vm, "list");
#     for (int i = limit; i != 0 && hay != NULL; i--) {
#         hay = be_re_match_search_run(vm, code, hay, is_anchored, bfalse);
#         if (hay != NULL) {
#             be_data_push(vm, -2);
#         }
#         be_pop(vm, 1);
#     }
#     be_pop(vm, 1);
#     ...
# }
def _re_match_search_all(vm, is_anchored):
    argc = be_api.be_top(vm)
    if argc >= 2 and (be_api.be_isstring(vm, 1) or be_byteslib.be_isbytes(vm, 1)) and be_api.be_isstring(vm, 2):
        hay = be_api.be_tostring(vm, 2)
        limit = -1
        if argc >= 3:
            limit = be_api.be_toint(vm, 3)

        pattern = _get_pattern_from_arg(vm, 1)
        if pattern is None:
            be_api.be_raise(vm, "type_error", None)

        be_api.be_newobject(vm, "list")
        i = limit
        while i != 0 and hay is not None and len(hay) > 0:
            end_pos = _do_match_search(vm, pattern, hay, is_anchored, False)
            if end_pos is not None:
                be_api.be_data_push(vm, -2)  # add sub list to outer list
            be_api.be_pop(vm, 1)
            if end_pos is None or end_pos == 0:
                break
            hay = hay[end_pos:]
            i -= 1
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


# ============================================================================
# Internal: _split_run — shared split logic
# ============================================================================

# int re_pattern_split_run(bvm *vm, ByteProg *code, const char *hay, int split_limit) {
#     ...
#     be_newobject(vm, "list");
#     while (1) {
#         if (split_limit == 0 || !match) {
#             push remaining; break;
#         }
#         push before match;
#         advance past match;
#         split_limit--;
#     }
#     be_pop(vm, 1);
#     be_return(vm);
# }
def _split_run(vm, pattern, hay, split_limit):
    """Split hay at each non-anchored match of pattern.

    Port of re_pattern_split_run from be_re_lib.c.
    Uses re15_vm.recursiveloopprog for matching.
    """
    nsubp = (pattern.sub + 1) * 2
    sub = [None] * nsubp

    be_api.be_newobject(vm, "list")
    while True:
        if split_limit == 0 or not re15_vm.recursiveloopprog(pattern, hay, sub, nsubp, False):
            # No more splits or no match — push remaining string
            be_api.be_pushstring(vm, hay)
            be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)
            break

        if sub[0] is None or sub[1] is None or sub[0] == sub[1]:
            # Zero-length match — can't split
            be_api.be_raise(vm, "internal_error", "can't match")

        # Push the part before the match
        be_api.be_pushstring(vm, hay[:sub[0]])
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)

        # Advance past the match
        hay = hay[sub[1]:]
        split_limit -= 1

        # Reset sub for next iteration
        for i in range(nsubp):
            sub[i] = None

    be_api.be_pop(vm, 1)  # remove list
    return be_api.be_returnvalue(vm)


# ============================================================================
# Internal: _get_pattern_from_arg — get compiled pattern from string or comptr
# ============================================================================

def _get_pattern_from_arg(vm, index):
    """Get a compiled regex pattern from a Berry stack argument.

    Accepts a string (compiled on the fly), bytes (deserialized ByteProg),
    or a comptr (pre-compiled ByteProg).
    """
    if be_api.be_isstring(vm, index):
        regex_str = be_api.be_tostring(vm, index)
        pattern = _compile_pattern(regex_str)
        if pattern is None:
            be_api.be_raise(vm, "internal_error", "error in regex")
        return pattern
    elif be_byteslib.be_isbytes(vm, index):
        bufptr, length = be_byteslib.be_tobytes(vm, index)
        if bufptr is None or length < 12:
            return None
        data = bytes(bufptr[:length])
        bytelen, prog_len, sub = struct.unpack('<iii', data[:12])
        prog = re15_opcodes.ByteProg()
        prog.bytelen = bytelen
        prog.len = prog_len
        prog.sub = sub
        prog.insts = bytearray(data[12:])
        return prog
    elif be_api.be_iscomptr(vm, index):
        return be_api.be_tocomptr(vm, index)
    return None


# ============================================================================
# re_pattern class methods
# ============================================================================

# int re_pattern_search(bvm *vm) {
#     ...
#     be_getmember(vm, 1, "_p");
#     ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
#     be_re_match_search_run(vm, code, hay, bfalse, bfalse);
#     be_return(vm);
# }
def re_pattern_search(vm):
    return _re_pattern_match_search(vm, is_anchored=False, size_only=False)


def re_pattern_match(vm):
    return _re_pattern_match_search(vm, is_anchored=True, size_only=False)


def re_pattern_match2(vm):
    return _re_pattern_match_search(vm, is_anchored=True, size_only=True)


def re_pattern_search_all(vm):
    return _re_pattern_match_search_all(vm, is_anchored=False)


def re_pattern_match_all(vm):
    return _re_pattern_match_search_all(vm, is_anchored=True)


# Berry: `re_pattern.split(s:string [, split_limit:int]) -> list(string)`
def re_pattern_split(vm):
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        split_limit = -1
        if argc >= 3 and be_api.be_isint(vm, 3):
            split_limit = be_api.be_toint(vm, 3)
        hay = be_api.be_tostring(vm, 2)
        be_api.be_getmember(vm, 1, "_p")
        pattern = be_api.be_tocomptr(vm, -1)
        return _split_run(vm, pattern, hay, split_limit)
    be_api.be_raise(vm, "type_error", None)


def _re_pattern_match_search(vm, is_anchored, size_only):
    """Shared logic for re_pattern.search/match/match2."""
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        hay = be_api.be_tostring(vm, 2)
        offset = 0
        if argc >= 3 and be_api.be_isint(vm, 3):
            offset = be_api.be_toint(vm, 3)
        hay_len = len(hay)
        if offset < 0:
            offset = 0
        if offset >= hay_len:
            return be_api.be_returnnilvalue(vm)
        hay = hay[offset:]

        be_api.be_getmember(vm, 1, "_p")
        pattern = be_api.be_tocomptr(vm, -1)
        _do_match_search(vm, pattern, hay, is_anchored, size_only)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


def _re_pattern_match_search_all(vm, is_anchored):
    """Shared logic for re_pattern.searchall/matchall."""
    argc = be_api.be_top(vm)
    if argc >= 2 and be_api.be_isstring(vm, 2):
        hay = be_api.be_tostring(vm, 2)
        limit = -1
        if argc >= 3 and be_api.be_isint(vm, 3):
            limit = be_api.be_toint(vm, 3)

        be_api.be_getmember(vm, 1, "_p")
        pattern = be_api.be_tocomptr(vm, -1)

        be_api.be_newobject(vm, "list")
        i = limit
        while i != 0 and hay is not None and len(hay) > 0:
            end_pos = _do_match_search(vm, pattern, hay, is_anchored, False)
            if end_pos is not None:
                be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)
            if end_pos is None or end_pos == 0:
                break
            hay = hay[end_pos:]
            i -= 1
        be_api.be_pop(vm, 1)
        return be_api.be_returnvalue(vm)
    be_api.be_raise(vm, "type_error", None)


# ============================================================================
# Module and class tables
# ============================================================================

# /* @const_object_info_begin
# module re (scope: global) {
#     compile, func(be_re_compile)
#     compilebytes, func(be_re_compilebytes)
#     search, func(be_re_search)
#     searchall, func(be_re_search_all)
#     match, func(be_re_match)
#     match2, func(be_re_match2)
#     matchall, func(be_re_match_all)
#     split, func(be_re_split)
#     dump, func(be_re_dump)
# }
# @const_object_info_end */

def be_re_module_table():
    """Return the native module attribute table for the re module."""
    return [
        ("compile", be_re_compile),
        ("compilebytes", be_re_compilebytes),
        ("search", be_re_search),
        ("searchall", be_re_search_all),
        ("match", be_re_match),
        ("match2", be_re_match2),
        ("matchall", be_re_match_all),
        ("split", be_re_split),
        ("dump", be_re_dump),
    ]


# /* @const_object_info_begin
# class be_class_re_pattern (scope: global, name: re_pattern) {
#     _p, var
#     search, func(re_pattern_search)
#     searchall, func(re_pattern_search_all)
#     match, func(re_pattern_match)
#     match2, func(re_pattern_match2)
#     matchall, func(re_pattern_match_all)
#     split, func(re_pattern_split)
# }
# @const_object_info_end */

def be_re_pattern_class_members():
    """Return the member list for the re_pattern class."""
    return [
        ("_p", None),
        ("search", re_pattern_search),
        ("searchall", re_pattern_search_all),
        ("match", re_pattern_match),
        ("match2", re_pattern_match2),
        ("matchall", re_pattern_match_all),
        ("split", re_pattern_split),
    ]


def be_load_relib(vm):
    """Register the re_pattern class as a builtin.

    Called during be_loadlibs to make re_pattern available for re.compile().
    """
    be_api.be_regclass(vm, "re_pattern", be_re_pattern_class_members())
