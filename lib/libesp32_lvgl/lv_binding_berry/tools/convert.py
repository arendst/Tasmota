#!/usr/bin/env python3
"""
LVGL -> Berry binding generator (phase 2 of the build pipeline).

This script turns the *normalized* LVGL headers produced by ``preprocessor.py``
into the C and Berry sources that implement the Berry binding.

Inputs (produced by ``preprocessor.py``, see ``gen.sh``)
    mapping/lv_funcs.h   one cleaned C function prototype per line
    mapping/lv_enum.h    enum values, integer constants, symbols, ...

Outputs (written to ``generate/``)
    be_lv_c_mapping.h        C function dispatch tables + widget constructors
    be_lv_classes.h          extern declarations for widget constructors/classes
    be_lvgl_widgets_lib.c    solidified Berry class/widget definitions
    be_lvgl_module.c         the ``lv`` module (global functions + constants)
    LVGL_API_Reference.md    human readable API reference (Markdown)

High level flow (see ``main()``)
    1. ``parse_functions()``  reads ``lv_funcs.h`` and classifies every function
       either as a method of an ``lv`` class/widget or as a global ``lv.*``
       function.
    2. ``parse_constants()``  reads ``lv_enum.h`` into an ordered list of
       module constants.
    3. The four ``generate_*()`` functions emit the output files from that model.

Run it from the ``tools/`` directory (all paths are relative)::

    python3 convert.py

The only diagnostics printed to stderr are the list of callback types that were
encountered and the C types that could not be mapped (useful when porting to a
new LVGL version).


Flash size optimizations (LVGL 9.6.0)
=====================================

The generated tables are compacted in three independent ways. None of them
changes what is visible from Berry: every constant and method that existed
before is still reachable by name with the same value / C function / Berry
signature. Sizes below were measured on ESP32 (xtensa-esp32-elf-gcc -Os) with
all widgets enabled, they are given as orders of magnitude.

The Python generator and the C runtime (``src/lv_berry.c``, ``src/lv_berry.h``)
share layouts and constants that MUST stay in sync. Each pair is marked with a
"must match" comment on both sides:

    Python (this file)              C (src/lv_berry.h / lv_berry.c)
    ------------------------------  ---------------------------------
    CONST_GROUP_MAX_PREFIX_LEN      LV_CONST_GROUP_PREFIX_MAX
    METHOD_SIG_BITS                 LV_METHOD_SIG_BITS
    METHOD_VERBS (order)            lv_method_verbs[] (emitted from METHOD_VERBS)
    STYLE_KINDS                     LV_SKIND_*
    SPROP_OBJ_SET/GET, STYLE_SET    LV_SPROP_*
    STYLE_TYPES_MAX (5 bits)        LV_SPROP_INFO() and `info & 0x1F`


1. `lv` module constants grouped by prefix  (generate/be_lvgl_module.c)
-----------------------------------------------------------------------

Before: one flat sorted ``lv0_constants[]`` of ``be_const_member_t`` (8 bytes
name pointer + value) with full names. 829 constants = ~22.6 KB.

Now: constants are grouped by the token before their first ``_`` and stored
without it (``STYLE_BG_COLOR`` -> ``"BG_COLOR"`` in group ``STYLE``):

    lv0_constants[]              constants that can't be grouped (full name)
    lv0_constants_<PREFIX>[]     one sorted table per group
    lv0_constants_groups[]       sorted index { prefix, table, size }

Result: ~17.7 KB for the same 829 constants (~4.9 KB saved, +176 B code).

``group_constants()`` keeps a constant in the main table when:
    - it has no ``_``, or its prefix is longer than CONST_GROUP_MAX_PREFIX_LEN
    - the remainder does not start with a letter (ex: ``OPA_0``). The runtime
      ``be_map_bin_search()`` skips a leading non-letter as a type marker, so
      ``"0"`` would never be found
    - its value marker is ``>`` (ctype function), not handled by the group
      lookup. Supported markers: int, ``$`` string, ``&`` comptr, ``@`` native
    - the whole group would not save bytes (group entry + prefix string cost)

Runtime: ``lv0_member()`` first searches ``lv0_constants[]`` (standard
``be_const_module_member()``), then ``lv0_member_grouped()``: split at the
first ``_``, binary search the prefix, then the rest inside the group.

``lv._constants()`` (``lv0_constants_as_hash()``) walks both the main table
and all groups, rebuilding full names. It is used by ``tools/lv_export_const.be``
running on the device to regenerate ``lv_haspmota/src/embedded/lv_1_constants.be``
with numerical values (values like ``LV_PART_ITEMS`` only exist once compiled,
which is why this is done on the device and not at build time).

Related: ``ENUM_EXCLUDE_PREFIXES`` in ``lvgl_config.py`` removes rarely used
constants (``LV_DRAW_TASK_*``, ``LV_FS_*``, ``LV_LOG_LEVEL_*``, ...). Note that
``preprocessor.py`` also applies it to ``LV_EXPORT_CONST_INT()`` constants.


2. Compact class method tables  (generate/be_lv_c_mapping.h)
-----------------------------------------------------------

Before: ``be_ntv_func_def_t`` = 16 bytes per method (name, func, return type,
arg type pointers) and full names. 1401 methods = ~46 KB with all widgets.

Now (``lv_ntv_method_t``, 8 bytes)::

    { func, uint16 name offset, uint16 LV_MSIG(verb, sig) }

    lv_method_verbs[]   verb prefixes, index = verb code (3 high bits of `sig`)
                        "", "get_", "get_style_", "is_", "set_", "set_style_"
    lv_method_names[]   one blob of NUL terminated names WITHOUT verb prefix
    lv_method_sigs[]    unique (return_type, arg_type) pairs (13 low bits)

- Names are split by ``method_verb()`` at the LONGEST verb prefix, so
  ``set_style_bg_color`` is verb ``set_style_`` + ``bg_color``, and
  ``set_style_bg_color``, ``get_style_bg_color`` and ``lv_style.set_bg_color``
  all share the same ``"bg_color"`` bytes.
- ``build_name_blob()`` also reuses the tail of a longer name (``"color"``
  points inside ``"bg_color"``). Blob must stay < 64 KB (uint16 offset).
- Only 264 distinct signatures exist, must stay < 8192 (13 bits).
- Entries of a class are sorted by ``(verb code, name)``. ``lv_method_find()``
  in ``lv_berry.c`` takes the longest verb prefix of the requested name and
  binary searches with the SAME order. If you add a verb, keep METHOD_VERBS
  sorted and remember the code is 3 bits (max 8 verbs).
- ``lv_classes[]`` (``lv_ntv_class_def_t``) keeps the class name as first
  field, so the class lookup still uses ``be_map_bin_search()``.
- Each generated line has the full Berry method name in a comment.

Result: compiled ``lv_berry.c`` 50.7 KB -> 30.3 KB (~20 KB saved).

Caveat: the shared name blob and signature table contain the names of ALL
widgets, also those disabled with ``BE_LV_WIDGET_*`` (a few hundred bytes).


3. Generic style accessors  (STYLE_GENERIC_ACCESSORS in lvgl_config.py)
------------------------------------------------------------------------

``obj.set_style_<p>(value, selector)``, ``obj.get_style_<p>(part)`` and
``style.set_<p>(value)`` are not stored as individual method entries. They are
resolved at runtime from:

    lv_style_types[]    { Berry value arg type, Berry return type, C kind }
    lv_style_props[]    { uint16 name offset in lv_method_names,
                          uint8 LV_STYLE_* id,
                          uint8 LV_SPROP_INFO(type index, accessor flags) }
                        sorted by name (without verb)

- ``parse_style_gen()`` reads LVGL's OWN generated sources (paths in
  ``STYLE_GEN_SOURCES``): ``lv_obj_style_gen.c``, ``lv_obj_style_internal_gen.h``
  and ``lv_style_gen.c``. A property is handled generically only if its LVGL
  function body matches the standard generated pattern exactly (check object,
  build ``lv_style_value_t`` from ``.num``/``.ptr``/``.color``, call the generic
  API with ``LV_STYLE_<NAME>``). This guarantees the generic call is strictly
  equivalent. If LVGL changes the pattern, functions simply stay explicit.
- ``split_style_methods()`` also requires the standard Berry signature shape
  (``(lv.obj)<value>i`` / ``(lv.obj)i`` / ``(lv.style)<value>``), then removes
  those entries from the ``obj`` and ``style`` tables. LVGL 8 aliases
  (``set_style_img_opa``...) are included, since they resolve to the C function.
- Stay explicit (by design): ``text_font`` (setter checks value != NULL), the
  ``*_filtered`` getters, ``pad_all``, ``margin_*``, ``size``,
  ``get_style_space_*``, and properties of signed 8/16 bit types.
- C kinds (STYLE_KINDS / LV_SKIND_*): num (int32/enum), u8 (ex: lv_opa_t),
  u16, bool, color, ptr. Small types are found by ``scan_small_typedefs()``.
  Enums are 4 bytes on ESP32 (no -fshort-enums), so they are ``num``.

Runtime (``lv_berry.c``):
- ``lv_x_member()`` searches the class method table first, then for
  ``lv_obj`` and ``lv_style`` only, ``lv_style_member()``: strip
  ``set_style_``/``get_style_``/``set_``, binary search ``lv_style_props``,
  check the accessor flag.
- It returns a native closure ``lv_x_call_style()`` with 4 upvals (wrapper
  func, return type, arg type, ``pk`` = prop id | kind << 8). The closure
  inserts ``pk`` as 2nd argument (after self) and calls ``be_call_c_func()``
  with arg types ``(lv.obj)i<value>i``, ``(lv.obj)ii`` or ``(lv.style)i<value>``,
  so Berry argument type checking is unchanged.
- Wrappers ``lvbe_obj_set_style_prop()``, ``lvbe_obj_get_style_prop()``,
  ``lvbe_style_set_prop()`` call ``lv_obj_set_local_style_prop()``,
  ``lv_obj_get_style_prop()`` and ``lv_style_set_prop()``.
- Colors: Berry passes ``lv.color`` as 0xRRGGBB, which is the bit layout of
  ``lv_color_t {blue, green, red}`` passed by value. ``lvbe_style_value()``
  and the getter convert field by field. Booleans, u8 and u16 are truncated
  exactly like LVGL's generated functions.
- Name based lookup (``introspect.get(obj, "set_style_" + k)`` used by
  HASPmota) works unchanged since it goes through ``member()``. Only code
  that enumerates a class method table won't see these accessors.
  ``LVGL_API_Reference.md`` still documents them.

Result: 132 properties covering 384 former method entries. Tables ~1.9 KB
smaller (code +787 B), and 358 LVGL style functions are no longer referenced
by the mapping: ~236 of them (~4 KB) are not used elsewhere and are removed by
the linker. Total ~6 KB. Set ``STYLE_GENERIC_ACCESSORS = False`` to go back to
explicit entries.


Checking after changes or an LVGL upgrade
-----------------------------------------

- ``./gen.sh`` must run without "Skipping", "Unhandled" or "mapping not used".
- Compare with the previous commit that every method/constant still resolves
  to the same function/value and signature. The verification used for the
  9.6.0 work simulated in Python the C lookups (``lv_method_find()``,
  ``lv_style_member()``, ``lv0_member_grouped()``) on the generated tables of
  ``HEAD`` and of the working tree, and checked sort order, missing names and
  unknown names returning nothing.
- Compile ``src/lv_berry.c`` with ``xtensa-esp32-elf-gcc`` and all
  ``-DBE_LV_WIDGET_*`` to compare sizes (``xtensa-esp32-elf-size -A``).
- On a device, test at least ``set_style_bg_color``/``get_style_bg_color``,
  ``style.set_text_color``, a bool and a u8 property (``set_style_bg_opa``),
  and ``lv.STYLE_BG_COLOR``, ``lv.SYMBOL_OK``, ``lv.OPA_0``.
"""

import re
import sys
import argparse
from pathlib import Path


# ======================================================================
# Configuration
# ----------------------------------------------------------------------
# Everything in this section is declarative data that typically needs a
# small adjustment when moving to a new LVGL version. No parsing or code
# generation logic lives here.
# ======================================================================

# All version-sensitive declarative data lives in ``lvgl_config.py`` (the single
# place to edit when porting to a new LVGL version). Only generation logic and
# the (version-independent) be_mapping encoding vocabulary remain in this file.
from lvgl_config import (
    COMPRESS_LV_TYPE,
    LV_WIDGETS, LV_WIDGETS_NO_CLASS, LV_PREFIX,
    LV_WIDGET_INHERITANCE, LV_WIDGET_CUSTOM_PTR,
    SYNONYM_FUNCTIONS, SYNTHETIC_CLASSES,
    RETURN_TYPES, SKIPPING_TYPES, GENERIC_CALLBACK_TYPES, CALLBACKS_SEEN_SEED,
    STYLE_GENERIC_ACCESSORS, STYLE_GEN_SOURCES,
)
from lvgl_config import LV_SRC_PREFIX as LV_SRC_INCLUDE_DIR


def get_synonyms(name):
    """Return the list of legacy names that are aliases of ``name``."""
    return [k for k, v in SYNONYM_FUNCTIONS.items() if v == name]


# ======================================================================
# Encoding vocabulary
# ----------------------------------------------------------------------
# Single source of truth for the compact codes understood by the Berry
# ``be_mapping`` runtime. There are two independent vocabularies; keep
# each in sync with its authoritative C reader.
# ======================================================================

class Sig:
    """Function-signature encoding (arguments + return types).

    Authoritative reader: lib/libesp32/berry_mapping/src/be_class_wrapper.c
      - be_call_c_func()        return-type handling
      - be_check_arg_type()     argument-string parsing
      - be_convert_single_elt() per-argument conversion
    """

    # Scalar codes (single char) -- valid as argument and/or return type
    INT      = "i"   # be_int
    BOOL     = "b"   # be_bool
    STRING   = "s"   # const char *  (nil if NULL)
    REAL     = "f"   # be_real (float) -- supported by C, not emitted today
    COMPTR   = "c"   # comptr (generic C pointer)
    ANY      = "."   # any value / no type check

    # Return-only codes
    VOID        = ""    # no return value (nil)
    STRING_FREE = "$"   # string then free()'d -- supported by C, not emitted today
    BYTES       = "&"   # bytes() + implicit (size_t*) arg -- not emitted today

    # Generator-internal marker for a generic (untyped) callback return
    CALLBACK = "C"

    # Constructor return codes ('_p' = instance var holding the pointer)
    CTOR_NONNULL  = "+_p"   # store ptr, raise if NULL
    CTOR_NULLABLE = "=_p"   # store ptr, allow NULL

    # Argument-only control codes
    SKIP      = "-"          # ignore this argument
    BUF_LEN   = "~"          # pass length of the previous bytes() buffer
    VM_PTR    = "@"          # pass the Berry VM pointer (first arg only)
    OPT_START = "["          # following arguments are optional
    VARARG    = "[......]"   # C '...' -> up to 6 optional any-args

    # Markdown rendering
    DOC = {
        INT: "int", BOOL: "bool", STRING: "string",
        COMPTR: "comptr", CALLBACK: "callback", ANY: "\\<any\\>",
    }
    DOC_CLOSURE = "\\<closure\\>"   # typed callback argument
    DOC_VARARG  = "[\\<any\\>]"     # C '...' argument

    @staticmethod
    def instance_arg(cls):    # e.g. '(lv.lv_obj)'
        return f"({cls})"

    @staticmethod
    def callback_arg(cb):     # e.g. '^lv_event_cb^'
        return f"^{cb}^"

    @staticmethod
    def to_doc(code):
        # multi-char class names (e.g. 'lv.obj') render as-is
        return Sig.DOC.get(code, code)


class Member:
    """Const-member value encoding for the ``lv`` module table.

    A value's first char (as found in lv_enum.h) selects the generated key
    prefix and the coc macro used to store it.
    Authoritative reader: lib/libesp32/berry_mapping/src/be_const_members.c
    (be_const_member_dual switches on the key's first char: $ & @ > * /).
    """

    # (value first char) -> (key prefix, coc macro, value transform)
    STRING = ('"', "$", "be_cconst_string", lambda v: v)
    PTR    = ('&', "&", "be_cconst_ptr",    lambda v: v)
    NATIVE = ('@', "@", "be_cconst_ptr",    lambda v: "&" + v[1:])
    CTYPE  = ('>', ">", "be_ctype",         lambda v: v[1:])

    _BY_MARKER = {rule[0]: rule for rule in (STRING, PTR, NATIVE, CTYPE)}

    # markers that `lv0_member()` (lv_berry.c) can decode for grouped constants
    GROUPABLE_MARKERS = {None, '"', '&', '@'}

    @staticmethod
    def marker(value):
        """Return the value's encoding marker, or None for an int (literal or bare enum)."""
        if value is None:
            return None
        return value[0] if value[0] in Member._BY_MARKER else None

    @staticmethod
    def encode(key, value, name=None):
        """Render the ``{ "<name>", macro(value) },`` table entry for one constant.

        ``key`` is the full constant name (used to build ``LV_<key>``), ``name``
        is the name stored in the table, defaults to ``key`` (it is shorter when
        the constant lives in a prefix group, see ``group_constants``)."""
        if name is None:
            name = key
        if value is None:                       # bare enum, resolved by the C compiler
            return f'    {{ "{name}", be_cconst_int(LV_{key}) }},'
        rule = Member._BY_MARKER.get(value[0])
        if rule is None:                        # plain int literal
            return f'    {{ "{name}", be_cconst_int({value}) }},'
        _, prefix, macro, transform = rule
        return f'    {{ "{prefix}{name}", {macro}({transform(value)}) }},'


# Size of one `lv_const_group_t` entry plus its alignment, used to decide if a
# prefix group saves flash (pointer + pointer + size_t on 32 bits)
CONST_GROUP_ENTRY_SIZE = 12
CONST_GROUP_MAX_PREFIX_LEN = 15             # must match LV_CONST_GROUP_PREFIX_MAX in lv_berry.c


def group_constants(constants):
    """Split constants into prefix groups to save flash.

    Constants are grouped by the token before their first ``_`` (ex: ``STYLE``
    for ``STYLE_BG_COLOR``), and stored without that prefix (``BG_COLOR``).
    A constant stays in the main table (full name) when:
    - it has no ``_``, or the prefix is too long
    - the remainder does not start with a letter (the runtime binary search
      skips a leading non-letter as a type marker, ex: ``OPA_0``)
    - its marker is not supported by the group lookup (``>`` ctype functions)
    - its group would not save flash

    Returns ``(main, groups)`` where ``main`` is a dict ``{key: value}`` and
    ``groups`` is a dict ``{prefix: {stripped_name: (key, value)}}``.
    """
    candidates = {}
    main = {}
    for k, v in constants.items():
        prefix, sep, rest = k.partition('_')
        if (sep and 0 < len(prefix) <= CONST_GROUP_MAX_PREFIX_LEN and rest[:1].isalpha()
                and Member.marker(v) in Member.GROUPABLE_MARKERS):
            candidates.setdefault(prefix, {})[rest] = (k, v)
        else:
            main[k] = v
    groups = {}
    for prefix, members in candidates.items():
        saved = (len(prefix) + 1) * len(members)            # bytes removed from names
        cost = CONST_GROUP_ENTRY_SIZE + len(prefix) + 1     # group entry + prefix string
        if saved > cost:
            groups[prefix] = members
        else:
            for k, v in members.values():
                main[k] = v
    return main, groups


# ======================================================================
# Type mapping
# ----------------------------------------------------------------------
# ``TypeMapper`` is the single source of truth for turning a C type into
# the compact single/multi-character encoding understood by the Berry
# ``be_mapping`` runtime, and into the human readable form used in the
# Markdown reference.
#
# Encoding cheat-sheet (see be_mapping.c for the authoritative reader):
#   i  int          b  bool        s  string (const char *)
#   c  comptr       C  callback    .  any value / void* return
#   (lv.<class>)    instance of an lv class (argument)
#   ^<cb_type>^     a typed callback argument
#   lv.<class>      class instance return type
# ======================================================================

class TypeMapper:
    # Detect a full function definition.
    # Ex: 'void lv_obj_set_parent(lv_obj_t * obj, lv_obj_t * parent);'
    #   Group 1: 'void'
    #   Group 2: 'lv_obj_set_parent'
    #   Group 3: 'lv_obj_t * obj, lv_obj_t * parent'
    parse_func_regex = re.compile(r"(.*?)\s(\w+)\((.*?)\)")

    # Parse a single call argument.
    # Ex: 'const lv_obj_t * parent' -> 'const ', 'lv_obj_t', ' * ', 'parent'
    # Ex: 'bool auto_fit'           -> '', 'bool', ' ', 'auto_fit'
    parse_arg_regex = re.compile(r"(\w+\s+)?(\w+)([\*\s]+)(\w+)(\[\])?")

    # Type tables and callback lists are defined centrally in lvgl_config.py
    # (the single place to edit for a new LVGL version). They are exposed as
    # class attributes so the rest of the class can use ``TypeMapper.<name>``.
    skipping_type = SKIPPING_TYPES
    return_types = RETURN_TYPES

    def __init__(self):
        # Stats: how many times each C type is converted (to spot dead entries),
        # and which types could not be mapped at all.
        self.return_types_count = {k: 0 for k in TypeMapper.return_types.keys()}
        self.unhandled_types = {}
        # Callback types encountered as *arguments*. Seeded with the types that
        # always exist so the end-of-run report matches historical output.
        self.callbacks_seen = list(CALLBACKS_SEEN_SEED)

    def return_type_to_doc(self, type):
        return Sig.to_doc(type)

    def c_convert_type(self, c_ctype, try_without_t):
        """Convert a C type to its Berry encoding, or ``None`` if unsupported.

        If ``try_without_t`` is set, also try the type with a stripped ``_t``
        suffix (e.g. ``lv_foo_t`` -> ``lv_foo``).
        """
        be_ret = TypeMapper.return_types.get(c_ctype)
        if be_ret:
            self.return_types_count[c_ctype] += 1
            return be_ret

        if try_without_t:
            c_ctype_without_t = re.sub(r"^(\w+?)_t", "\\1", c_ctype)
            be_ret = TypeMapper.return_types.get(c_ctype_without_t)
            if be_ret:
                self.return_types_count[c_ctype_without_t] += 1
                return be_ret

        return None     # fallback

    def type_is_unhandled(self, t):
        self.unhandled_types[t] = self.unhandled_types.get(t, 0) + 1

    def dump_return_types_stats(self, dump_all):
        for k in sorted(self.unhandled_types, key=self.unhandled_types.get, reverse=True):
            print(f"> Unhandled type '{k}' ({self.unhandled_types[k]})", file=sys.stderr)
        if dump_all:
            for k in sorted(self.return_types_count, key=self.return_types_count.get, reverse=True):
                print(f"# '{k}': {self.return_types_count[k]}", file=sys.stderr)
        else:
            # dump only unused
            return_types_count_filtered = [k for k, v in self.return_types_count.items() if v == 0]
            for k in return_types_count_filtered:
                print(f"# mapping not used '{k}'", file=sys.stderr)

    def clean_c_line(self, l_raw):
        l_raw = re.sub(r'//.*$', '', l_raw)                  # remove trailing comments
        l_raw = re.sub(r'LV_ATTRIBUTE_FAST_MEM ', '', l_raw) # remove LV_ATTRIBUTE_FAST_MEM marker
        l_raw = re.sub(r'\s+', ' ', l_raw)                   # replace any multi-space with a single space
        l_raw = l_raw.strip(" \t\n\r")                      # remove leading or trailing spaces
        l_raw = re.sub(r'static ', '', l_raw)                # remove `static` qualifier
        l_raw = re.sub(r'inline ', '', l_raw)                # remove `inline` qualifier
        l_raw = re.sub(r'const\s+char\s*\*', 'constchar *', l_raw)
        l_raw = re.sub(r'^char\s*\*', 'retchar *', l_raw)    # special case for returning a char*
        l_raw = re.sub(r'const ', '', l_raw)
        l_raw = re.sub(r'struct ', '', l_raw)
        return l_raw

    def parse_c_line(self, l_raw):
        g = TypeMapper.parse_func_regex.search(l_raw)
        if g:
            c_return_type = g.group(1)
            c_func_name = g.group(2)
            c_arg_types = g.group(3)
            return (c_return_type, c_func_name, c_arg_types)
        else:
            return (None, None, None)

    def get_widget_return_type(self, c_type):
        c_ret = self.c_convert_type(c_type, False)
        c_ctype_without_t = re.sub(r"_t$", "", c_type)
        if c_ret:
            pass      # done
        elif c_type == "void":
            c_ret = Sig.VOID
        elif c_ctype_without_t in GENERIC_CALLBACK_TYPES:
            c_ret = Sig.CALLBACK   # general callback, if not already captured by explicit type
        elif c_type not in TypeMapper.skipping_type:
            print(f"  // Skipping unsupported return type '{c_type}'", file=sys.stderr)
            self.type_is_unhandled(c_type)
            c_ret = None

        return c_ret

    def get_widget_arg_type(self, c_return_type, c_func_name, c_arg_types):
        """Convert the comma-separated C argument list to its Berry encoding.

        Returns ``(c_args, c_args_doc)`` or ``(None, None)`` if any argument
        type is unsupported (in which case the whole function is dropped).
        """
        c_args = ""
        c_args_doc = []
        args_raw = [x.strip(" \t\n\r") for x in c_arg_types.split(",")]  # split by comma and strip
        for arg_raw in args_raw:

            # check if `void`
            if arg_raw == 'void':      # Special case for 'void' which can't be captured easily in regex
                break

            if arg_raw == '...':          # Special case for '...' which can't be captured easily in regex
                # '...'
                c_args += Sig.VARARG  # allow 6 additional parameters by default
                c_args_doc.append(Sig.DOC_VARARG)
                break

            # Ex: 'const lv_obj_t * parent'   -> 'const ', 'lv_obj_t', ' * ', 'parent', ''
            # Ex: 'bool auto_fit'             -> '', 'bool', ' ', 'auto_fit', ''
            # Ex: 'const lv_coord_t value[]'  -> 'const', 'lv_coord_t', '', 'value', '[]'
            ga = TypeMapper.parse_arg_regex.search(arg_raw)
            if ga:                      # parsing ok?
                # ga.group(1) is first modifier like `const`, we don't use them
                ga_type = ga.group(2)     # main type
                ga_ptr = (ga.group(3).strip(" \t\n\r") == "*")      # (bool) is it a pointer
                ga_2ptr = (ga.group(3).strip(" \t\n\r") == "**")    # (bool) is it a pointer to a pointer
                ga_name = ga.group(4)     # main type name
                ga_array = ga.group(5)    # is it an array `[]`

                ga_full_type = ga_type    # rebuild a complete type with modifier
                if ga_ptr:    ga_full_type += " *"
                if ga_2ptr:   ga_full_type += " **"
                if ga_array:  ga_full_type += " []"

                be_type = self.c_convert_type(ga_full_type, True)

                if be_type == None:
                    # no match, unsupported type
                    if ga_full_type not in TypeMapper.skipping_type:
                        self.type_is_unhandled(ga_full_type)
                    return (None, None)

                # if the type is a single letter, we just add it
                if len(be_type) == 1 and be_type != Sig.CALLBACK:  # callbacks are different
                    c_args += be_type
                    c_args_doc.append(self.return_type_to_doc(be_type))
                else:
                    if be_type.endswith("_cb"):
                        if 'remove_' in c_func_name:    # if the call is to remove the cb, just treat as an 'anything' parameter
                            c_args += Sig.ANY
                            c_args_doc.append(Sig.to_doc(Sig.ANY))
                        else:
                            # it's a callback type, we encode it differently
                            if be_type not in self.callbacks_seen:
                                self.callbacks_seen.append(be_type)
                            c_args += Sig.callback_arg(be_type)
                            c_args_doc.append(Sig.DOC_CLOSURE)
                    else:
                        # we have a high-level type that we treat as a class name, enclose in parenthesis
                        c_args += Sig.instance_arg("lv." + be_type)
                        c_args_doc.append("lv." + be_type)
            else:
                print(f"  // unable to parse arguments '{arg_raw}'", file=sys.stderr)

        return (c_args, c_args_doc)


# ======================================================================
# Model
# ======================================================================

class LvglFunction:
    """A single LVGL function, ready to be emitted as a Berry method/function.

    ``be_name``        the name exposed to Berry (method or function name)
    ``orig_func_name`` the underlying C symbol
    ``c_ret_type``     Berry encoding of the return type
    ``c_argc``         Berry encoding of the argument list
    ``c_args_doc``     human readable argument list (Markdown)
    """

    def __init__(self, mapper, c_func_name, c_ret_type, c_argc, c_args_doc_list,
                 orig_func_name, be_name):
        # change "lv.lv_" into "lv."
        c_args_doc = ', '.join(c_args_doc_list)
        if COMPRESS_LV_TYPE:
            c_argc = re.sub(r"lv\.lv_", "lv.", c_argc)
            c_args_doc = re.sub(r"lv\.lv_", "lv.", c_args_doc)
        self.c_args_doc = c_args_doc

        self.c_func_name = c_func_name
        self.orig_func_name = orig_func_name
        self.be_name = be_name
        if c_ret_type == Sig.ANY:   c_ret_type = Sig.COMPTR        # if return type is '.', return a comptr
        self.c_argc = c_argc
        if len(c_ret_type) > 1:
            if COMPRESS_LV_TYPE:
                c_ret_type = "lv." + re.sub(r"^lv_", "", c_ret_type)
            else:
                c_ret_type = "lv." + c_ret_type
        self.c_ret_type = c_ret_type
        self.c_ret_type_doc = mapper.return_type_to_doc(c_ret_type)

    def add_C_line_to_map(self, map):
        """Add this function's C mapping table entry into ``map`` keyed by be_name."""
        be_ret_type = self.c_ret_type
        map[self.be_name] = f"  {{ \"{self.be_name}\", {{ (const void*) &{self.orig_func_name}, \"{be_ret_type}\", \"{self.c_argc}\" }} }},"


# ======================================================================
# Parsing
# ======================================================================

def parse_functions(mapper, lv_funcs_file):
    """Parse ``lv_funcs.h`` and classify every supported function.

    Returns ``(lv, lv0)`` where:
      - ``lv``  is an ordered dict ``subtype -> [LvglFunction, ...]`` for every
        class/widget method (the first argument, the implicit ``self``, is
        stripped from the documentation).
      - ``lv0`` is the list of global ``lv.*`` functions.
    """
    # Pre-seed the synthetic LVGL8 classes so they keep a stable position at the
    # front of the generated output (insertion order is significant).
    lv = {c: [] for c in SYNTHETIC_CLASSES}
    lv0 = []        # functions in the `lv` module

    with open(lv_funcs_file) as f:
        for l_raw in f:
            l_raw = mapper.clean_c_line(l_raw)
            if (len(l_raw) == 0): continue

            (c_return_type, c_func_name, c_arg_types) = mapper.parse_c_line(l_raw)
            if (c_return_type == None): continue

            c_ret = mapper.get_widget_return_type(c_return_type)
            if c_ret == None:   continue      # skip if nothing to return

            # convert arguments
            (c_args, c_args_doc_list) = mapper.get_widget_arg_type(c_return_type, c_func_name, c_arg_types)
            if c_args == None: continue      # skip if nothing to return

            # analyze function name and determine if it needs to be assigned to a specific class
            # Ex: c_func_name -> 'lv_obj_set_parent'
            if c_func_name.startswith("_"): continue            # skip low-level
            if c_func_name.startswith("lv_debug_"): continue    # skip debug

            be_func_name = ""

            if c_func_name == "lv_style_init":
                continue   # no need for init as it would collide with native init (and called behind the scene anyways)

            found = False
            for subtype in LV_PREFIX:
                if c_func_name.startswith("lv_" + subtype + "_"):
                    be_name = re.sub("^lv_" + subtype + "_", '', c_func_name)
                    be_func_name = "lvbe_" + subtype + "_" + be_name
                    if subtype not in lv: lv[subtype] = []    # add entry
                    is_right_type = c_args.startswith(f"(lv.lv_{subtype})")   # check if first arg matches class
                    is_obj_arg1 = c_args.startswith("(lv.lv_obj)")            # or first arg is lv_obj
                    is_group_create = (subtype == 'group') and (c_func_name == 'lv_group_create')
                    if is_right_type or is_obj_arg1 or is_group_create:
                        # remove first argument which is implicit
                        c_args_doc_list = c_args_doc_list[1:]
                        lv[subtype].append(LvglFunction(mapper, be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, be_name))
                        # add synonyms
                        for n in get_synonyms(be_name):
                            lv[subtype].append(LvglFunction(mapper, be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, n))

                        found = True
                        break

            if found: continue
            # not found, we treat it as lv top level function
            be_name = re.sub("^lv_", '', c_func_name)
            lv0.append(LvglFunction(mapper, be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, be_name))
            # add synonyms
            for n in get_synonyms(be_name):
                lv0.append(LvglFunction(mapper, be_func_name, c_ret, c_args, c_args_doc_list, c_func_name, n))

    return (lv, lv0)


def parse_constants(lv_enum_file):
    """Parse ``lv_enum.h`` into an ordered list of ``[key, value]`` pairs.

    A ``key`` of ``None`` marks a passthrough comment line. ``value`` is ``None``
    for a bare enum (its value is resolved by the C compiler).
    """
    lv_module = []
    with open(lv_enum_file) as f:
        for l_raw in f:
            l_raw = l_raw.strip(" \t\n\r")    # remove leading or trailing spaces
            if l_raw.startswith("//"):
                lv_module.append([None, l_raw])   # if key is None then add comment line
            l_raw = re.sub(r'//.*$', '', l_raw)  # remove trailing comments
            l_raw = re.sub(r'\s+', '', l_raw)    # remove all spaces
            l_raw = re.sub(r',.*$', '', l_raw)   # remove comma and anything after it
            if (len(l_raw) == 0): continue

            k_v = l_raw.split("=")
            if len(k_v) > 2:
                print(f"Error: cannot match {l_raw}")
                continue
            # extract the key name
            k = k_v[0]
            if k.startswith("_"):
                continue      # skip any label starting with '_'
            k = re.sub('^LV_', '', k)  # remove any LV_ prefix
            v = None
            if len(k_v) == 2:   # value is included
                v = k_v[1]
                if k is None or v is None: continue   # we skip if key is void or value is void

            if not k.isidentifier():
                print(f"Error: {k} is not an identifier")
                continue

            lv_module.append([k, v])    # keep as string or None

    return lv_module


# ======================================================================
# Generators
# ----------------------------------------------------------------------
# Each generator owns exactly one output file. Large boilerplate blocks
# are kept as named module-level templates (at column 0) so the dynamic
# generation logic stays readable.
# ======================================================================

DOC_HEADER = """# LVGL Berry API Reference

[//]: # (**********************************************************************)
[//]: # (* Generated code, don't edit                                         *)
[//]: # (**********************************************************************)

"""

DOC_TABLE_HEADER = "Method|Arguments|Return type|LVGL equivalent"
DOC_TABLE_SEP = ":---|:---|:---|:---"


def _doc_row(out, f):
    """Emit one Markdown table row for a function (skip ``*_create``)."""
    if f.c_func_name.endswith("_create"):
        # skip create functions that are handled in the object constructor (no `create` method)
        return
    print(f"{f.be_name}|{f.c_args_doc}|{f.c_ret_type_doc}|[{f.orig_func_name}](https://docs.lvgl.io/9.0/search.html?q={f.orig_func_name})", file=out)


def generate_doc(path, lv, lv0):
    """Generate the Markdown API reference (``LVGL_API_Reference.md``)."""
    with open(path, 'w') as out:
        print(DOC_HEADER, file=out)

        # Start with the `lv` module
        print("## module `lv`", file=out)
        print(file=out)
        print(DOC_TABLE_HEADER, file=out)
        print(DOC_TABLE_SEP, file=out)
        for f in sorted(lv0, key=lambda x: x.be_name):
            print(f"{f.be_name}|{f.c_args_doc}|{f.c_ret_type_doc}|[{f.orig_func_name}](https://docs.lvgl.io/9.0/search.html?q={f.orig_func_name})", file=out)
        print(file=out)

        # Continue with core (non-widget) classes
        print("## Core classes", file=out)
        print(file=out)
        for subtype, flv in sorted(lv.items()):
            if subtype not in LV_WIDGETS and len(flv) > 0:
                print(f"### class `lv.{subtype}`", file=out)
                print(file=out)
                print(DOC_TABLE_HEADER, file=out)
                print(DOC_TABLE_SEP, file=out)
                for f in sorted(flv, key=lambda x: x.be_name):
                    _doc_row(out, f)
                print(file=out)

        # Continue with widgets
        print("## Widgets", file=out)
        print(file=out)
        for subtype in LV_WIDGETS:
            flv = lv.get(subtype)
            if flv:
                print(f"### widget `lv.{subtype}`", file=out)
                print(file=out)
                print(DOC_TABLE_HEADER, file=out)
                print(DOC_TABLE_SEP, file=out)
                for f in sorted(flv, key=lambda x: x.be_name):
                    _doc_row(out, f)
                print(file=out)


C_MAPPING_HEADER = """
/********************************************************************
 * Generated code, don't edit
  *******************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include "be_ctypes.h"
#include "be_mapping.h"
#include "../src/lv_berry.h"
#include "../src/lv_colorwheel.h"
#include "../src/lv_stripes.h"
"""

C_CLASSES_MAP_OPEN = """
// map of clases
const lv_ntv_class_def_t lv_classes[] = {"""


# ----------------------------------------------------------------------
# Compact method tables (see `lv_ntv_method_t` in src/lv_berry.h)
#
# Each method entry is 8 bytes: function pointer, uint16 offset of its name
# in `lv_method_names`, and uint16 combining a verb code (3 high bits) with
# an index in `lv_method_sigs` (13 low bits) holding (return_type, arg_type).
#
# Names are stored without their verb prefix (ex: `set_style_bg_color` is
# verb `set_style_` + `bg_color`), identical names are shared and a name that
# is the tail of another one reuses its bytes.
#
# Entries of a class are sorted by (verb code, name): the runtime binary
# search in `lv_method_find()` (src/lv_berry.c) uses the same order.
# ----------------------------------------------------------------------

# Verb prefixes, the index is the verb code. MUST stay sorted and in sync with
# `lv_method_verbs[]` emitted below (the C code reads it from the generated file)
METHOD_VERBS = ["", "get_", "get_style_", "is_", "set_", "set_style_"]
METHOD_SIG_BITS = 13            # must match LV_METHOD_SIG_BITS in src/lv_berry.h
METHOD_NAMES_MAX = 0xFFFF       # name offset is uint16


def method_verb(name):
    """Split a method name into ``(verb_code, rest)`` using the longest verb prefix."""
    best = 0
    for code, verb in enumerate(METHOD_VERBS):
        if verb and name.startswith(verb) and len(verb) > len(METHOD_VERBS[best]):
            best = code
    return best, name[len(METHOD_VERBS[best]):]


def build_name_blob(names):
    """Pack unique names into one NUL separated blob, reusing tails of longer names.

    Returns ``(pieces, offsets)``: the list of strings to emit in order, and a
    dict ``name -> offset`` in the blob."""
    pieces = []
    offsets = {}
    placed = []         # (string, offset) already in the blob
    pos = 0
    for n in sorted(set(names), key=lambda s: (-len(s), s)):
        for s, o in placed:
            if s.endswith(n):
                offsets[n] = o + len(s) - len(n)
                break
        else:
            offsets[n] = pos
            placed.append((n, pos))
            pieces.append(n)
            pos += len(n) + 1
    if pos > METHOD_NAMES_MAX:
        raise ValueError(f"method names blob too large: {pos} bytes")
    return pieces, offsets


def print_method_tables_header(out, classes, extra_names=()):
    """Emit verbs, name blob and signature table shared by all class tables.

    ``extra_names`` are added to the name blob (ex: generic style properties)."""
    all_names = list(extra_names)
    sigs = {}
    for entries in classes.values():
        for be_name, f in entries.items():
            all_names.append(method_verb(be_name)[1])
            sigs.setdefault((f.c_ret_type, f.c_argc), len(sigs))
    if len(sigs) >= (1 << METHOD_SIG_BITS):
        raise ValueError(f"too many method signatures: {len(sigs)}")
    pieces, offsets = build_name_blob(all_names)

    print("/* verb prefixes, index is the verb code in `lv_ntv_method_t.sig` */", file=out)
    print(f"#define LV_METHOD_VERBS_NB  {len(METHOD_VERBS)}", file=out)
    print("static const char * const lv_method_verbs[LV_METHOD_VERBS_NB] = {", file=out)
    print("  " + ", ".join(f'"{v}"' for v in METHOD_VERBS), file=out)
    print("};", file=out)
    print(file=out)
    print("/* method names without verb prefix, `\\0` separated */", file=out)
    print("static const char lv_method_names[] =", file=out)
    pos = 0
    for p in pieces:
        print(f'  /* {pos:5} */ "{p}\\0"', file=out)
        pos += len(p) + 1
    print(";", file=out)
    print(file=out)
    print("/* (return_type, arg_type) signatures */", file=out)
    print("static const lv_ntv_sig_t lv_method_sigs[] = {", file=out)
    for (ret, args), idx in sorted(sigs.items(), key=lambda kv: kv[1]):
        print(f'  /* {idx:3} */ {{ "{ret}", "{args}" }},', file=out)
    print("};", file=out)
    print(file=out)
    return offsets, sigs


def class_method_entries(flv):
    """Return ``{be_name: LvglFunction}`` for the methods of a class (no `_create`)."""
    entries = {}
    for f in sorted(flv, key=lambda x: x.be_name):
        if f.c_func_name.endswith("_create"):
            continue    # handled in the object constructor (no `create` method)
        entries[f.be_name] = f
    return entries


# ----------------------------------------------------------------------
# Generic style accessors (see STYLE_GENERIC_ACCESSORS in lvgl_config.py)
#
# `obj.set_style_<p>(v, sel)`, `obj.get_style_<p>(part)` and `style.set_<p>(v)`
# are resolved at runtime from `lv_style_props[]` instead of one method entry
# (and one LVGL function) each. Unreferenced LVGL functions are then removed
# by the linker.
# ----------------------------------------------------------------------

# C value kinds, must match LV_SKIND_* in src/lv_berry.h
STYLE_KINDS = {'num': 0, 'u8': 1, 'u16': 2, 'bool': 3, 'color': 4, 'ptr': 5}
# accessor flags, must match LV_SPROP_* in src/lv_berry.h
SPROP_OBJ_SET, SPROP_OBJ_GET, SPROP_STYLE_SET = 1, 2, 4
STYLE_TYPES_MAX = 32            # type index is 5 bits


def scan_small_typedefs(include_dir):
    """Return ``{type_name: c_kind}`` for types declared as ``typedef uint8_t/uint16_t <name>;``."""
    kinds = {}
    for h in Path(include_dir).rglob('*.h'):
        for base, name in re.findall(r'typedef\s+(u?int8_t|u?int16_t)\s+(\w+)\s*;', h.read_text(errors='replace')):
            # signed small types are not supported (None), they stay explicit methods
            kinds[name] = None if not base.startswith('u') else ('u16' if '16' in base else 'u8')
    return kinds


def parse_style_gen(sources, include_dir):
    """Parse LVGL's generated style sources.

    Returns ``{prop: {...}}`` (prop is lower case, ex: ``bg_color``) only for
    functions following the standard generated pattern, so that the generic
    accessor is strictly equivalent. Keys: ``c_prop`` (``LV_STYLE_BG_COLOR``),
    ``kind`` (see STYLE_KINDS) and ``obj_set``/``obj_get``/``style_set`` flags.
    """
    og = Path(sources["obj"]).read_text()
    ig = Path(sources["obj_internal"]).read_text()
    sg = Path(sources["style"]).read_text()
    small = scan_small_typedefs(include_dir)
    small.update({'uint8_t': 'u8', 'int8_t': None, 'uint16_t': 'u16', 'int16_t': None})

    def c_kind(ctype, member):
        if member in ('color', 'ptr'):
            return member
        ctype = ctype.replace('const ', '').strip()
        if ctype == 'bool':
            return 'bool'
        return small.get(ctype, 'num')

    VAL = r'\.(num|ptr|color) = (?:\([^)]*\))?value'
    props = {}

    def add(p, c_prop, ctype, member, flag):
        if c_prop != 'LV_STYLE_' + p.upper() or c_kind(ctype, member) is None:
            return
        e = props.setdefault(p, {'c_prop': c_prop, 'kind': c_kind(ctype, member)})
        if e['kind'] != c_kind(ctype, member):
            return
        e[flag] = True

    for m in re.finditer(r'void lv_obj_set_style_(\w+)\(lv_obj_t \* obj, ([^,]+?) value, lv_style_selector_t selector\)\s*\{\s*'
                         r'LV_CHECK_OBJ\(obj, &lv_obj_class, return\);\s*lv_style_value_t v = \{\s*' + VAL +
                         r'\s*\};\s*lv_obj_set_local_style_prop\(obj, (LV_STYLE_\w+), v, selector\);\s*\}', og):
        add(m.group(1), m.group(4), m.group(2), m.group(3), 'obj_set')
    internal = {}
    for m in re.finditer(r'static inline (.+?) lv_obj_get_style_(\w+)_internal\(const lv_obj_t \* obj, lv_part_t part\)\s*\{\s*'
                         r'LV_ASSERT\(obj != NULL\);\s*lv_style_value_t v = lv_obj_get_style_prop_internal\(obj, part, (LV_STYLE_\w+)\);\s*'
                         r'return (?:\((.+?)\))?v\.(num|ptr|color);\s*\}', ig):
        if (m.group(4) or m.group(1)).strip() == m.group(1).strip():       # cast must be the return type
            internal[m.group(2)] = (m.group(1), m.group(3), m.group(5))
    for m in re.finditer(r'\n[^\n]+? lv_obj_get_style_(\w+)\(const lv_obj_t \* obj, lv_part_t part\)\s*\{\s*'
                         r'LV_CHECK_OBJ\(obj, &lv_obj_class, return [^;]*?\);\s*return lv_obj_get_style_(\w+)_internal\(obj, part\);\s*\}', og):
        if m.group(1) == m.group(2) and m.group(1) in internal:
            ctype, c_prop, member = internal[m.group(1)]
            add(m.group(1), c_prop, ctype, member, 'obj_get')
    for m in re.finditer(r'void lv_style_set_(\w+)\(lv_style_t \* style, (.+?) value\)\s*\{\s*'
                         r'LV_CHECK_ARG\(style != NULL && LV_STYLE_SENTINEL_OK\(style\), return\);\s*lv_style_value_t v = \{\s*' + VAL +
                         r'\s*\};\s*lv_style_set_prop\(style, (LV_STYLE_\w+), v\);\s*\}', sg):
        add(m.group(1), m.group(4), m.group(2), m.group(3), 'style_set')
    return props


def split_style_methods(classes):
    """Remove generic style accessors from the `obj` and `style` class tables.

    Returns ``(style_props, style_types)``:
    - ``style_props``: ``{name: {'c_prop', 'type', 'flags'}}`` where ``name`` is
      the method name without verb (aliases like ``img_opa`` included)
    - ``style_types``: ``{(value_arg, return_type, kind): index}``
    """
    if not STYLE_GENERIC_ACCESSORS:
        return {}, {}
    tools_dir = Path(__file__).parent
    info = parse_style_gen({k: tools_dir / v for k, v in STYLE_GEN_SOURCES.items()},
                           tools_dir / LV_SRC_INCLUDE_DIR)
    # (class, verb, C prefix, info flag, accessor flag)
    rules = [
        ('obj',   'set_style_', 'lv_obj_set_style_', 'obj_set',   SPROP_OBJ_SET),
        ('obj',   'get_style_', 'lv_obj_get_style_', 'obj_get',   SPROP_OBJ_GET),
        ('style', 'set_',       'lv_style_set_',     'style_set', SPROP_STYLE_SET),
    ]
    props = {}
    for subtype, verb, c_prefix, info_flag, flag in rules:
        entries = classes.get(subtype, {})
        for be_name in list(entries):
            f = entries[be_name]
            code, rest = method_verb(be_name)
            if METHOD_VERBS[code] != verb or not f.orig_func_name.startswith(c_prefix):
                continue
            p = info.get(f.orig_func_name[len(c_prefix):])
            if p is None or not p.get(info_flag):
                continue
            # the Berry signature must have the standard shape
            if flag == SPROP_OBJ_SET:
                m = re.fullmatch(r'\(lv\.obj\)(.+)i', f.c_argc)
                value_arg, ret = (m.group(1) if m else None), None
                ok = m is not None and f.c_ret_type == ""
            elif flag == SPROP_OBJ_GET:
                value_arg, ret = None, f.c_ret_type
                ok = f.c_argc == "(lv.obj)i" and f.c_ret_type != ""
            else:
                m = re.fullmatch(r'\(lv\.style\)(.+)', f.c_argc)
                value_arg, ret = (m.group(1) if m else None), None
                ok = m is not None and f.c_ret_type == ""
            if not ok:
                continue
            e = props.setdefault(rest, {'c_prop': p['c_prop'], 'kind': p['kind'], 'value_arg': None, 'ret': None, 'flags': 0})
            if e['c_prop'] != p['c_prop'] or e['kind'] != p['kind']:
                continue
            if value_arg is not None:
                if e['value_arg'] not in (None, value_arg):
                    continue
                e['value_arg'] = value_arg
            if ret is not None:
                if e['ret'] not in (None, ret):
                    continue
                e['ret'] = ret
            e['flags'] |= flag
            del entries[be_name]            # resolved at runtime, no method entry
    # drop names that ended with no accessor (only possible after a conflict)
    props = {k: v for k, v in props.items() if v['flags']}
    types = {}
    for e in props.values():
        key = (e['value_arg'] or "", e['ret'] or "", e['kind'])
        e['type'] = types.setdefault(key, len(types))
    if len(types) > STYLE_TYPES_MAX:
        raise ValueError(f"too many style types: {len(types)}")
    return props, types


def print_style_tables(out, props, types, name_offsets):
    """Emit `lv_style_types[]` and `lv_style_props[]` (sorted by name)."""
    print("/* generic style accessors: value types, see `lv_style_ntv_type_t` */", file=out)
    print("_Static_assert(LV_STYLE_LAST_BUILT_IN_PROP <= 0xFF, \"style prop IDs must fit in uint8_t\");", file=out)
    print("static const lv_style_ntv_type_t lv_style_types[] = {", file=out)
    for (value_arg, ret, kind), idx in sorted(types.items(), key=lambda kv: kv[1]):
        print(f'  /* {idx:2} */ {{ "{value_arg}", "{ret}", LV_SKIND_{kind.upper()} }},', file=out)
    print("};", file=out)
    print(file=out)
    print("/* generic style accessors, sorted by name without verb */", file=out)
    print("static const lv_style_ntv_prop_t lv_style_props[] = {", file=out)
    for name in sorted(props):
        e = props[name]
        flags = " | ".join(n for b, n in ((SPROP_OBJ_SET, "LV_SPROP_OBJ_SET"), (SPROP_OBJ_GET, "LV_SPROP_OBJ_GET"),
                                         (SPROP_STYLE_SET, "LV_SPROP_STYLE_SET")) if e['flags'] & b)
        print(f"  {{ {name_offsets[name]}, {e['c_prop']}, LV_SPROP_INFO({e['type']}, {flags}) }},    /* {name} */", file=out)
    print("};", file=out)
    print("#define LV_STYLE_PROPS_NB  (sizeof(lv_style_props) / sizeof(lv_style_props[0]))", file=out)
    print(file=out)

C_CLASSES_MAP_CLOSE = """};
const size_t lv_classes_size = sizeof(lv_classes) / sizeof(lv_classes[0]);
"""

C_FONT_THEME_CREATE = """
// create font either empty or from parameter on stack
int lvbe_font_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
int lvbe_theme_create(bvm *vm)       { return be_call_c_func(vm, NULL, "+_p", ""); }
"""


def generate_c_mapping(path, lv):
    """Generate ``be_lv_c_mapping.h``: per-class method tables, the class map,
    and the widget constructor (``_init``) functions."""
    with open(path, 'w') as out:
        print(C_MAPPING_HEADER, file=out)

        # shared verbs, names and signatures
        classes = {subtype: class_method_entries(flv) for subtype, flv in lv.items()}
        style_props, style_types = split_style_methods(classes)
        name_offsets, sigs = print_method_tables_header(out, classes, style_props.keys())
        print_style_tables(out, style_props, style_types, name_offsets)

        # Per-class method dispatch tables, sorted by (verb, name without verb)
        for subtype, entries in classes.items():
            print(f"/* `lv_{subtype}` methods */", file=out)
            if subtype in LV_WIDGETS:
                print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}", file=out)
            print(f"const lv_ntv_method_t lv_{subtype}_func[] = {{", file=out)

            for be_name in sorted(entries, key=method_verb):
                f = entries[be_name]
                verb, rest = method_verb(be_name)
                sig = sigs[(f.c_ret_type, f.c_argc)]
                print(f"  {{ (const void*) &{f.orig_func_name}, {name_offsets[rest]}, LV_MSIG({verb}, {sig}) }},"
                      f"    /* {be_name} */", file=out)

            print("};", file=out)
            if subtype in LV_WIDGETS:
                print(f"#endif // BE_LV_WIDGET_{subtype.upper()}", file=out)
            print(file=out)

        # extern class declarations
        for subtype in sorted(lv):
            print(f"extern const bclass be_class_lv_{subtype};", file=out)
        print(file=out)

        # global map of classes
        print(C_CLASSES_MAP_OPEN, file=out)
        for subtype in sorted(lv):
            if subtype in LV_WIDGETS:
                print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}", file=out)
            print(f"  {{ \"lv_{subtype}\", &be_class_lv_{subtype}, lv_{subtype}_func, sizeof(lv_{subtype}_func) / sizeof(lv_{subtype}_func[0]) }},", file=out)
            if subtype in LV_WIDGETS:
                print(f"#endif // BE_LV_WIDGET_{subtype.upper()}", file=out)
        print(C_CLASSES_MAP_CLOSE, file=out)

        # widget constructors (`_init`): keep only the `_create` functions
        for subtype, flv in lv.items():
            print(f"  /* `lv_{subtype}` methods */", file=out)
            create_found = False        # does the class have an explicit `_create()` method?
            for f in sorted(flv, key=lambda x: x.be_name):
                if f.c_func_name.endswith("_create"):
                    create_found = True
                    if subtype in LV_WIDGETS:
                        print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}", file=out)
                        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"{Sig.CTOR_NONNULL}\", \"{f.c_argc}\"); }}", file=out)
                        print(f"#endif // BE_LV_WIDGET_{subtype.upper()}", file=out)
                    else:
                        print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"{Sig.CTOR_NONNULL}\", \"{f.c_argc}\"); }}", file=out)

            if not create_found and subtype in LV_WIDGETS:
                # there is no explicit create, add one (reusing the last parsed function `f`)
                print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}", file=out)
                print(f"  int be_ntv_lv_{subtype}_init(bvm *vm)       {{ return be_call_c_func(vm, (void*) &{f.orig_func_name}, \"{Sig.CTOR_NONNULL}\", \"{f.c_argc}\"); }}", file=out)
                print(f"#endif // BE_LV_WIDGET_{subtype.upper()}", file=out)

        print(C_FONT_THEME_CREATE, file=out)

        print(file=out)
        print("#ifdef __cplusplus", file=out)
        print("} /* extern \"C\" */", file=out)
        print("#endif", file=out)
        print("/********************************************************************/", file=out)


WIDGETS_LIB_HEADER = """
/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
 
 /********************************************************************
 * Tasmota LVGL classes for widgets
 *******************************************************************/
#include "be_constobj.h"

#include "lvgl.h"

extern int lv0_init(bvm *vm);

extern int lco_init(bvm *vm);           // generic function
extern int lco_tostring(bvm *vm);       // generic function
extern int lco_toint(bvm *vm);          // generic function

extern int lv_x_member(bvm *vm);
extern int lv_x_tostring(bvm *vm);       // generic function

extern int lv_be_style_init(bvm *vm);
extern int lv_be_style_del(bvm *vm);
extern int lv_be_anim_init(bvm *vm);
extern int lv_x_tostring(bvm *vm);

BE_EXPORT_VARIABLE extern const bclass be_class_lv_obj;

extern int lvbe_font_create(bvm *vm);
extern int lvbe_theme_create(bvm *vm);

// adding ad-hoc colorwheel from LVGL8 to LVGL9
extern const lv_obj_class_t lv_colorwheel_class;

// custom Tasmota widget: rectangle filled with an angled stripe pattern
extern const lv_obj_class_t lv_stripes_class;

#include "be_lv_classes.h"

"""

CLASSES_HEADER = """/********************************************************************
 * Generated code, don't edit
 *******************************************************************/
"""

WIDGETS_LIB_CORE_CLASSES = """
/********************************************************************
** Solidified class: lv_style
********************************************************************/
#include "be_fixed_be_class_lv_style.h"
/* @const_object_info_begin
class be_class_lv_style (scope: global, name: lv_style, strings: weak) {
    _p, var
    init, func(lv_be_style_init)
    del, func(lv_be_style_del)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_obj
********************************************************************/
#include "be_fixed_be_class_lv_obj.h"
/* @const_object_info_begin
class be_class_lv_obj (scope: global, name: lv_obj, strings: weak) {
    _p, var
    _class, comptr(&lv_obj_class)
    init, func(be_ntv_lv_obj_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_group
********************************************************************/
#include "be_fixed_be_class_lv_group.h"
/* @const_object_info_begin
class be_class_lv_group (scope: global, name: lv_group, strings: weak) {
    _p, var
    init, func(be_ntv_lv_group_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_indev
********************************************************************/
#include "be_fixed_be_class_lv_indev.h"
/* @const_object_info_begin
class be_class_lv_indev (scope: global, name: lv_indev, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_display
********************************************************************/
#include "be_fixed_be_class_lv_display.h"
/* @const_object_info_begin
class be_class_lv_display (scope: global, name: lv_display, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_timer
********************************************************************/
#include "be_fixed_be_class_lv_timer.h"
/* @const_object_info_begin
class be_class_lv_timer (scope: global, name: lv_timer, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_anim
********************************************************************/
#include "be_fixed_be_class_lv_anim.h"
/* @const_object_info_begin
class be_class_lv_anim (scope: global, name: lv_anim, strings: weak) {
    _p, var
    init, func(lv_be_anim_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_event
********************************************************************/
#include "be_fixed_be_class_lv_event.h"
/* @const_object_info_begin
class be_class_lv_event (scope: global, name: lv_event, strings: weak) {
    _p, var
    init, func(lv0_init)
    tostring, func(lv_x_tostring)
    member, func(lv_x_member)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_font
********************************************************************/
#include "be_fixed_be_class_lv_font.h"
/* @const_object_info_begin
class be_class_lv_font (scope: global, name: lv_font, strings: weak) {
    _p, var
    init, func(lvbe_font_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_theme
********************************************************************/
#include "be_fixed_be_class_lv_theme.h"
/* @const_object_info_begin
class be_class_lv_theme (scope: global, name: lv_theme, strings: weak) {
    _p, var
    init, func(lvbe_theme_create)
    tostring, func(lv_x_tostring)
}
@const_object_info_end */

/********************************************************************
** Solidified class: lv_color
********************************************************************/
#include "be_fixed_be_class_lv_color.h"
/* @const_object_info_begin
class be_class_lv_color (scope: global, name: lv_color, strings: weak) {
    _p, var
    init, func(lco_init)
    tostring, func(lco_tostring)
    toint, func(lco_toint)
}
@const_object_info_end */

/* define extern function for arc_anim_start_angle and arc_anim_end_angle*/
extern void arc_anim_start_angle(void * obj, int32_t v);
extern void arc_anim_end_angle(void * obj, int32_t v);

"""


def generate_classes_header(path, lv):
    """Generate ``be_lv_classes.h``: extern declarations for the ad-hoc
    LVGL8 colorwheel/stripes classes, the widget constructor (``_init``)
    functions, and the widget class objects (included by
    ``be_lvgl_widgets_lib.c``)."""
    with open(path, 'w') as out:
        print(CLASSES_HEADER, file=out)

        # expose all extern constructor definitions
        for subtype, flv in lv.items():
            print(f"extern int be_ntv_lv_{subtype}_init(bvm *vm);", file=out)
        print(file=out)

        # extern class declarations
        for subtype in sorted(lv):
            print(f"extern const bclass be_class_lv_{subtype};", file=out)


def generate_widgets_lib(path, lv):
    """Generate ``be_lvgl_widgets_lib.c``: the solidified Berry class and
    widget definitions (consumed later by the ``coc`` tool)."""
    with open(path, 'w') as out:
        print(WIDGETS_LIB_HEADER, file=out)

        # hand-written core classes (lv_obj and friends)
        print(WIDGETS_LIB_CORE_CLASSES, file=out)

        # one solidified class per widget
        for subtype, flv in lv.items():
            if subtype == 'obj':  continue    # 'obj' has a special implementation
            if subtype not in LV_WIDGETS:  continue
            super_class = LV_WIDGET_INHERITANCE.get(subtype, "obj")    # superclass, default to lv_obj
            print(f"""/********************************************************************
** Solidified class: lv_{subtype}
********************************************************************/
#include "be_fixed_be_class_lv_{subtype}.h"
/* @const_object_info_begin""", file=out)
            if super_class is not None:
                print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, super: be_class_lv_{super_class}, strings: weak) {{", file=out)
            else:
                print(f"class be_class_lv_{subtype} (scope: global, name: lv_{subtype}, strings: weak) {{", file=out)
                print(f"    _p, var", file=out)
                print(f"    tostring, func(lv_x_tostring)", file=out)
                print(f"    member, func(lv_x_member)", file=out)
            print(f"    init, func(be_ntv_lv_{subtype}_init)", file=out)

            if subtype not in LV_WIDGETS_NO_CLASS:
                print(f"    _class, comptr(&lv_{subtype}_class)", file=out)
            if subtype in LV_WIDGET_CUSTOM_PTR:
                for k, v in LV_WIDGET_CUSTOM_PTR[subtype].items():
                    print(f"    {k}, {v}", file=out)

            print(f"""}}
@const_object_info_end */
""", file=out)


MODULE_LV_FUNC_OPEN = """/********************************************************************
 * LVGL Module
 *******************************************************************/
#include "be_constobj.h"

#include "lvgl.h"
#include "be_mapping.h"
#include "be_ctypes.h"
#include "lv_berry.h"
#include "lv_theme_haspmota.h"

// declare accessors for non-const ints
int32_t be_LV_LAYOUT_GRID(void) { return LV_LAYOUT_GRID; };              BE_VAR_CTYPE_DECLARE(be_LV_LAYOUT_GRID, "i");
int32_t be_LV_LAYOUT_FLEX(void) { return LV_LAYOUT_FLEX; };              BE_VAR_CTYPE_DECLARE(be_LV_LAYOUT_FLEX, "i");

extern int lv0_member(bvm *vm);     // resolve virtual members
extern int lv0_load_font(bvm *vm);

extern lv_ts_calibration_t * lv_get_ts_calibration(void);

static int lv_get_hor_res(void) {
  return lv_display_get_horizontal_resolution(lv_display_get_default());    // LVGL 9.6: NULL display is deprecated
}
static int lv_get_ver_res(void) {
  return lv_display_get_vertical_resolution(lv_display_get_default());      // LVGL 9.6: NULL display is deprecated
}

/* `lv` methods */
const be_ntv_func_def_t lv_func[] = {
"""

MODULE_LV_FUNC_CLOSE = """
};
const size_t lv_func_size = sizeof(lv_func) / sizeof(lv_func[0]);

"""

MODULE_CONSTANTS_OPEN = """

const be_const_member_t lv0_constants[] = {
"""

MODULE_CONSTANTS_CLOSE = """
};

const size_t lv0_constants_size = sizeof(lv0_constants)/sizeof(lv0_constants[0]);

/* Constants grouped by prefix, names are stored without the prefix and its `_` */
"""

MODULE_CONSTANTS_GROUPS_OPEN = """
const lv_const_group_t lv0_constants_groups[] = {"""

MODULE_CONSTANTS_GROUPS_CLOSE = """};

const size_t lv0_constants_groups_size = sizeof(lv0_constants_groups)/sizeof(lv0_constants_groups[0]);
"""

MODULE_TRAILER = """
#include "../src/solidify/solidified_lv.h"

/* @const_object_info_begin
module lv (scope: global, file: lv) {
    init, closure(lv_module_init_closure)
    member, func(lv0_member)
}
@const_object_info_end */
#include "be_fixed_lv.h"
"""


def generate_module(path, lv0, lv_module):
    """Generate ``be_lvgl_module.c``: the ``lv`` module with its global
    functions table and the constant/enum/symbol member table."""
    with open(path, 'w') as out:
        print("/********************************************************************", file=out)
        print(" * Generated code, don't edit", file=out)
        print(" *******************************************************************/", file=out)

        print(MODULE_LV_FUNC_OPEN, file=out)

        # global `lv` functions, sorted by Berry name
        func_out = {}  # used to sort output
        for f in lv0:
            f.add_C_line_to_map(func_out)
        for be_name in sorted(func_out):
            print(func_out[be_name], file=out)

        print(MODULE_LV_FUNC_CLOSE, file=out)

        # constant/enum/symbol member table
        print(MODULE_CONSTANTS_OPEN, file=out)

        # collapse the [key, value] pairs into a dict (drop comment-only lines)
        lv_module2 = {}
        for (k, v) in lv_module:
            if k is not None:
                lv_module2[k] = v

        # Emit the enums, symbols and function pointers (see the `Member` class
        # for the value-encoding contract). Keys are sorted ignoring a leading
        # non-letter prefix, matching the runtime binary search.
        # Most constants go to prefix groups (names stored without the common
        # prefix), the main table only keeps the ones that can't be grouped.
        main, groups = group_constants(lv_module2)
        for k in sorted(main):
            print(Member.encode(k, main[k]), file=out)

        print(MODULE_CONSTANTS_CLOSE, file=out)

        # one sorted table per prefix group
        for prefix in sorted(groups):
            members = groups[prefix]
            print(f"static const be_const_member_t lv0_constants_{prefix}[] = {{", file=out)
            for name in sorted(members):
                k, v = members[name]
                print(Member.encode(k, v, name), file=out)
            print("};", file=out)

        # index of groups, sorted by prefix for binary search
        print(MODULE_CONSTANTS_GROUPS_OPEN, file=out)
        for prefix in sorted(groups):
            print(f'    {{ "{prefix}", lv0_constants_{prefix}, '
                  f'sizeof(lv0_constants_{prefix})/sizeof(lv0_constants_{prefix}[0]) }},', file=out)
        print(MODULE_CONSTANTS_GROUPS_CLOSE, file=out)
        print(MODULE_TRAILER, file=out)

        print("/********************************************************************/", file=out)


# ======================================================================
# Entry point
# ======================================================================

# Output file names (inside the generate directory)
BE_LV_C_MAPPING = "be_lv_c_mapping.h"
BE_LV_CLASSES = "be_lv_classes.h"
BE_LV_WIDGETS_LIB = "be_lvgl_widgets_lib.c"
BE_LV_MODULE = "be_lvgl_module.c"
BE_LV_DOC = "LVGL_API_Reference.md"


def main():
    parser = argparse.ArgumentParser(description="Generate the LVGL Berry binding from normalized headers")
    parser.add_argument("--funcs", default="../mapping/lv_funcs.h",
                        help="input: normalized function prototypes (from preprocessor.py)")
    parser.add_argument("--enums", default="../mapping/lv_enum.h",
                        help="input: enum/constant/symbol definitions (from preprocessor.py)")
    parser.add_argument("--out-dir", default="../generate/",
                        help="output directory for the generated binding files")
    args = parser.parse_args()

    out_dir = Path(args.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)

    mapper = TypeMapper()

    # 1. parse functions and 2. parse constants
    lv, lv0 = parse_functions(mapper, args.funcs)
    print("| callback types" + str(mapper.callbacks_seen), file=sys.stderr)
    lv_module = parse_constants(args.enums)

    # 3. generate the output files (same order as the historical script)
    generate_doc(out_dir / BE_LV_DOC, lv, lv0)
    generate_c_mapping(out_dir / BE_LV_C_MAPPING, lv)
    generate_classes_header(out_dir / BE_LV_CLASSES, lv)
    generate_widgets_lib(out_dir / BE_LV_WIDGETS_LIB, lv)
    generate_module(out_dir / BE_LV_MODULE, lv0, lv_module)

    # report C types that were never used or could not be mapped
    mapper.dump_return_types_stats(False)    # True would dump all stats, False only unused


if __name__ == "__main__":
    main()
