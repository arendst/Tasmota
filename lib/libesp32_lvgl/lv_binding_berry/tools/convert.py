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
)


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

    @staticmethod
    def encode(key, value):
        """Render the ``{ "<key>", macro(value) },`` table entry for one constant."""
        if value is None:                       # bare enum, resolved by the C compiler
            return f'    {{ "{key}", be_cconst_int(LV_{key}) }},'
        rule = Member._BY_MARKER.get(value[0])
        if rule is None:                        # plain int literal
            return f'    {{ "{key}", be_cconst_int({value}) }},'
        _, prefix, macro, transform = rule
        return f'    {{ "{prefix}{key}", {macro}({transform(value)}) }},'


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
#include "../src/lv_colorwheel.h"
"""

C_CLASSES_MAP_OPEN = """
// map of clases
const be_ntv_class_def_t lv_classes[] = {"""

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

        # Per-class method dispatch tables
        for subtype, flv in lv.items():
            print(f"/* `lv_{subtype}` methods */", file=out)
            if subtype in LV_WIDGETS:
                print(f"#ifdef BE_LV_WIDGET_{subtype.upper()}", file=out)
            print(f"const be_ntv_func_def_t lv_{subtype}_func[] = {{", file=out)

            func_out = {}  # used to sort output
            for f in sorted(flv, key=lambda x: x.be_name):
                if f.c_func_name.endswith("_create"):
                    # skip create functions handled in the object constructor (no `create` method)
                    pass
                else:
                    f.add_C_line_to_map(func_out)

            for be_name in sorted(func_out):
                print(func_out[be_name], file=out)

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


def generate_widgets_lib(path, lv):
    """Generate ``be_lvgl_widgets_lib.c``: the solidified Berry class and
    widget definitions (consumed later by the ``coc`` tool)."""
    with open(path, 'w') as out:
        print(WIDGETS_LIB_HEADER, file=out)

        # expose all extern constructor definitions
        for subtype, flv in lv.items():
            print(f"extern int be_ntv_lv_{subtype}_init(bvm *vm);", file=out)
        print(file=out)

        # extern class declarations
        for subtype in sorted(lv):
            print(f"extern const bclass be_class_lv_{subtype};", file=out)
        print(file=out)

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
  return lv_display_get_horizontal_resolution(NULL);
}
static int lv_get_ver_res(void) {
  return lv_display_get_vertical_resolution(NULL);
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
        for k in sorted(lv_module2):
            print(Member.encode(k, lv_module2[k]), file=out)

        print(MODULE_CONSTANTS_CLOSE, file=out)

        print("/********************************************************************/", file=out)


# ======================================================================
# Entry point
# ======================================================================

# Output file names (inside the generate directory)
BE_LV_C_MAPPING = "be_lv_c_mapping.h"
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
    generate_widgets_lib(out_dir / BE_LV_WIDGETS_LIB, lv)
    generate_module(out_dir / BE_LV_MODULE, lv0, lv_module)

    # report C types that were never used or could not be mapped
    mapper.dump_return_types_stats(False)    # True would dump all stats, False only unused


if __name__ == "__main__":
    main()
