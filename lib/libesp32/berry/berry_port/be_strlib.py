"""
Berry string library utility functions.
Mirrors: src/be_strlib.c / src/be_strlib.h

This module implements string utility functions used throughout the Berry
interpreter: concatenation, multiplication, comparison, number-to-string
conversion, value-to-string conversion, string formatting (printf-style),
number parsing, string indexing, path splitting, and escape encoding.

Original C code is included as comments for each function.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

import math
import struct
import sys

from berry_port.berry_conf import (
    BE_INTGER_TYPE,
    BE_USE_SINGLE_FLOAT,
    BE_USE_STRING_MODULE,
)
from berry_port.be_object import (
    BE_NIL, BE_BOOL, BE_INT, BE_REAL, BE_INDEX,
    BE_STRING, BE_CLOSURE, BE_NTVCLOS, BE_NTVFUNC, BE_CTYPE_FUNC,
    BE_CLASS, BE_INSTANCE, BE_MODULE, BE_COMPTR, BE_FUNCTION, BE_NONE,
    SHORT_STR_MAX_LEN,
    bvalue,
    basetype,
    var_type, var_isnil, var_isbool, var_isint, var_isreal, var_isstr,
    var_isinstance, var_isfunction,
    var_tobool, var_toint, var_toreal, var_tostr, var_toobj, var_toidx,
    var_setstr, var_setint, var_setreal, var_setval,
    be_class_name, be_instance_name, be_instance_class,
    str_len,
    cast_instance,
)

# ---------------------------------------------------------------------------
# Integer limits (mirrors C M_IMAX / M_IMIN)
# ---------------------------------------------------------------------------
# #if BE_INTGER_TYPE == 0
#     #define M_IMAX    INT_MAX
#     #define M_IMIN    INT_MIN
# #elif BE_INTGER_TYPE == 1
#     #define M_IMAX    LONG_MAX
#     #define M_IMIN    LONG_MIN
# #else
#     #define M_IMAX    LLONG_MAX
#     #define M_IMIN    LLONG_MIN
# #endif
if BE_INTGER_TYPE == 0:
    M_IMAX = 2**31 - 1
    M_IMIN = -(2**31)
elif BE_INTGER_TYPE == 1:
    M_IMAX = 2**63 - 1   # long on 64-bit
    M_IMIN = -(2**63)
else:
    M_IMAX = 2**63 - 1   # long long
    M_IMIN = -(2**63)

# #if BE_USE_SINGLE_FLOAT == 0
#     #define BREAL_MAX DBL_MAX
#     #define BREAL_MIN DBL_MIN
# #else
#     #define BREAL_MAX FLT_MAX
#     #define BREAL_MIN FLT_MIN
# #endif
if BE_USE_SINGLE_FLOAT == 0:
    BREAL_MAX = sys.float_info.max
    BREAL_MIN = sys.float_info.min
else:
    BREAL_MAX = 3.4028235e+38
    BREAL_MIN = 1.175494e-38

# BE_INT_FORMAT equivalent for Python
BE_INT_FORMAT = "%d"
# BE_INT_FMTLEN — in C this is "ll" for long long; not needed in Python
# but we keep the name for format mode fixup
BE_INT_FMTLEN = ""

# ---------------------------------------------------------------------------
# Helpers (mirrors C macros)
# ---------------------------------------------------------------------------
# #define is_space(c)     ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')
# #define is_digit(c)     ((c) >= '0' && (c) <= '9')
def _is_space(c):
    return c in (' ', '\t', '\r', '\n')

def _is_digit(c):
    return '0' <= c <= '9'


# ---------------------------------------------------------------------------
# Lazy imports to avoid circular dependencies
# ---------------------------------------------------------------------------
def _lazy_be_string():
    import berry_port.be_string as m
    return m

def _lazy_be_class():
    import berry_port.be_class as m
    return m

def _lazy_be_module():
    import berry_port.be_module as m
    return m

def _lazy_be_mem():
    import berry_port.be_mem as m
    return m

def _lazy_be_api():
    import berry_port.be_api as m
    return m

def _lazy_be_vm():
    import berry_port.be_vm as m
    return m


# ---------------------------------------------------------------------------
# static int str_strncasecmp(const char *s1, const char *s2, size_t n)
# {
#     if (n == 0) return 0;
#     while (n-- != 0 && tolower(*s1) == tolower(*s2)) {
#         if (n == 0 || *s1 == '\0' || *s2 == '\0')
#             break;
#         s1++;
#         s2++;
#     }
#     return tolower(*(const unsigned char *)s1)
#         - tolower(*(const unsigned char *)s2);
# }
def str_strncasecmp(s1, s2, n):
    """Case-insensitive comparison of first n characters."""
    if n == 0:
        return 0
    i = 0
    while i < n:
        c1 = s1[i].lower() if i < len(s1) else '\0'
        c2 = s2[i].lower() if i < len(s2) else '\0'
        if c1 != c2 or c1 == '\0' or c2 == '\0':
            return ord(c1) - ord(c2)
        i += 1
    return 0


# ---------------------------------------------------------------------------
# bstring* be_strcat(bvm *vm, bstring *s1, bstring *s2)
# {
#     size_t len = (size_t)str_len(s1) + str_len(s2);
#     if (len <= SHORT_STR_MAX_LEN) {
#         char buf[SHORT_STR_MAX_LEN + 1];
#         strcpy(buf, str(s1));
#         strncat(buf, str(s2), len);
#         return be_newstrn(vm, buf, len);
#     } else {
#         bstring *s = be_newstrn(vm, NULL, len);
#         char *sbuf = (char*)str(s);
#         strcpy(sbuf, str(s1));
#         strcpy(sbuf + str_len(s1), str(s2));
#         return s;
#     }
# }
def be_strcat(vm, s1, s2):
    """Concatenate two Berry strings, returning a new bstring."""
    be_string = _lazy_be_string()
    data1 = be_string.be_str2cstr(s1)
    data2 = be_string.be_str2cstr(s2)
    result = data1 + data2
    return be_string.be_newstrn(vm, result, len(result))


# ---------------------------------------------------------------------------
# bstring* be_strmul(bvm *vm, bstring *s1, bint n)
# {
#     if (n == 1) { return s1; }
#     size_t string_len = (size_t)str_len(s1);
#     if ((n <= 0) || (string_len == 0)) {
#         return be_newstrn(vm, "", 0);
#     }
#     if (n > (bint)(vm->bytesmaxsize / string_len)) {
#         be_raise(vm, "runtime_error", "string multiplication result too large");
#     }
#     size_t total_len = string_len * (size_t)n;
#     if (total_len <= SHORT_STR_MAX_LEN) {
#         ...short path...
#     } else {
#         ...long path...
#     }
# }
def be_strmul(vm, s1, n):
    """Multiply a Berry string n times, returning a new bstring."""
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()
    if n == 1:
        return s1
    string_len = str_len(s1)
    if n <= 0 or string_len == 0:
        return be_string.be_newstrn(vm, "", 0)
    # Overflow check
    if n > vm.bytesmaxsize // string_len:
        be_api.be_raise(vm, "runtime_error", "string multiplication result too large")
    data = be_string.be_str2cstr(s1)
    result = data * n
    return be_string.be_newstrn(vm, result, len(result))


# ---------------------------------------------------------------------------
# int be_strcmp(bstring *s1, bstring *s2)
# {
#     if (be_eqstr(s1, s2)) { return 0; }
#     return strcmp(str(s1), str(s2));
# }
def be_strcmp(s1, s2):
    """Compare two Berry strings. Returns <0, 0, or >0."""
    be_string = _lazy_be_string()
    if be_string.be_eqstr(s1, s2):
        return 0
    a = be_string.be_str2cstr(s1)
    b = be_string.be_str2cstr(s2)
    if a < b:
        return -1
    elif a > b:
        return 1
    return 0


# ---------------------------------------------------------------------------
# bstring* be_num2str(bvm *vm, bvalue *v)
# {
#     char buf[25];
#     if (var_isint(v)) {
#         snprintf(buf, sizeof(buf), BE_INT_FORMAT, var_toint(v));
#     } else if (var_isreal(v)) {
#         snprintf(buf, sizeof(buf), "%g", var_toreal(v));
#     } else {
#         snprintf(buf, sizeof(buf), "(nan)");
#     }
#     return be_newstr(vm, buf);
# }
def be_num2str(vm, v):
    """Convert a numeric bvalue to a bstring."""
    be_string = _lazy_be_string()
    if var_isint(v):
        buf = str(var_toint(v))
    elif var_isreal(v):
        buf = _format_real(var_toreal(v))
    else:
        buf = "(nan)"
    return be_string.be_newstr(vm, buf)


def _format_real(r):
    """Format a real number matching C's %g behavior."""
    if math.isinf(r):
        return "inf" if r > 0 else "-inf"
    if math.isnan(r):
        return "nan"
    # %g: use shortest of %e and %f with 6 significant digits, strip trailing zeros
    s = "%.6g" % r
    return s


# ---------------------------------------------------------------------------
# static void module2str(char *buf, size_t buf_len, bvalue *v)
# {
#     const char *name = be_module_name(cast(bmodule*, var_toobj(v)));
#     if (name) {
#         snprintf(buf, buf_len, "<module: %s>", name);
#     } else {
#         snprintf(buf, buf_len, "<module: %p>", var_toobj(v));
#     }
# }
def _module2str(v):
    """Format a module value as a string."""
    be_module = _lazy_be_module()
    obj = var_toobj(v)
    name = be_module.be_module_name(obj)
    if name:
        return "<module: %s>" % name
    else:
        return "<module: 0x%x>" % id(obj)


# ---------------------------------------------------------------------------
# static bstring* sim2str(bvm *vm, bvalue *v)
# {
#     char sbuf[64];
#     switch (var_type(v)) {
#     case BE_NIL:      strcpy(sbuf, "nil"); break;
#     case BE_BOOL:     strcpy(sbuf, var_tobool(v) ? "true" : "false"); break;
#     case BE_INDEX:
#     case BE_INT:      snprintf(sbuf, sizeof(sbuf), BE_INT_FORMAT, var_toint(v)); break;
#     case BE_REAL:     snprintf(sbuf, sizeof(sbuf), "%g", var_toreal(v)); break;
#     case BE_CLOSURE: case BE_NTVCLOS: case BE_NTVFUNC: case BE_CTYPE_FUNC:
#         snprintf(sbuf, sizeof(sbuf), "<function: %p>", var_toobj(v)); break;
#     case BE_CLASS:
#         snprintf(sbuf, sizeof(sbuf), "<class: %s>",
#             str(be_class_name(cast(bclass*, var_toobj(v))))); break;
#     case BE_MODULE:   module2str(sbuf, sizeof(sbuf), v); break;
#     case BE_COMPTR:
#         snprintf(sbuf, sizeof(sbuf), "<ptr: %p>", var_toobj(v)); break;
#     default:          strncpy(sbuf, "(unknown value)", sizeof(sbuf)); break;
#     }
#     return be_newstr(vm, sbuf);
# }
def _sim2str(vm, v):
    """Convert a simple (non-instance) value to a bstring."""
    be_string = _lazy_be_string()
    t = var_type(v)
    if t == BE_NIL:
        sbuf = "nil"
    elif t == BE_BOOL:
        sbuf = "true" if var_tobool(v) else "false"
    elif t == BE_INDEX or t == BE_INT:
        sbuf = str(var_toint(v))
    elif t == BE_REAL:
        sbuf = _format_real(var_toreal(v))
    elif t in (BE_CLOSURE, BE_NTVCLOS, BE_NTVFUNC, BE_CTYPE_FUNC):
        sbuf = "<function: 0x%x>" % id(var_toobj(v))
    elif t == BE_CLASS:
        name_s = be_class_name(var_toobj(v))
        sbuf = "<class: %s>" % be_string.be_str2cstr(name_s)
    elif t == BE_MODULE:
        sbuf = _module2str(v)
    elif t == BE_COMPTR:
        sbuf = "<ptr: 0x%x>" % id(var_toobj(v))
    else:
        sbuf = "(unknown value)"
    return be_string.be_newstr(vm, sbuf)


# ---------------------------------------------------------------------------
# static bstring* ins2str(bvm *vm, int idx)
# {
#     bstring *s = str_literal(vm, "tostring");
#     binstance *obj = var_toobj(vm->reg + idx);
#     int type = be_instance_member(vm, obj, s, vm->top);
#     be_incrtop(vm);
#     if (basetype(type) != BE_FUNCTION) {
#         bstring *name = be_class_name(be_instance_class(obj));
#         ...format "<instance: ClassName()>"...
#     } else {
#         *vm->top = vm->reg[idx];
#         be_dofunc(vm, vm->top - 1, 1);
#         be_stackpop(vm, 1);
#         if (!var_isstr(vm->top)) { ... raise error ... }
#         s = var_tostr(vm->top);
#     }
#     return s;
# }
def _ins2str(vm, idx):
    """Convert an instance value to a bstring by calling its tostring method."""
    be_string = _lazy_be_string()
    be_cls = _lazy_be_class()
    be_api = _lazy_be_api()
    be_vm_mod = _lazy_be_vm()

    s = be_vm_mod.str_literal(vm, "tostring")
    obj = var_toobj(vm.stack[vm.reg_idx + idx])
    # get method 'tostring'
    type_ = be_cls.be_instance_member(vm, obj, s, vm.stack[vm.top_idx])
    be_api.be_incrtop(vm)  # push the obj::tostring to stack
    if basetype(type_) != BE_FUNCTION:
        name = be_class_name(be_instance_class(obj))
        name_str = be_string.be_str2cstr(name)
        be_api.be_stackpop(vm, 1)  # pop the obj::tostring
        s = be_string.be_newstr(vm, "<instance: %s()>" % name_str)
    else:
        var_setval(vm.stack[vm.top_idx], vm.stack[vm.reg_idx + idx])
        be_vm_mod.be_dofunc(vm, vm.top_idx - 1, 1)
        be_api.be_stackpop(vm, 1)  # pop the obj::tostring
        if not var_isstr(vm.stack[vm.top_idx]):
            name_str = be_string.be_str2cstr(be_instance_name(obj))
            if not name_str:
                name_str = "<anonymous>"
            be_api.be_raise(vm, "runtime_error",
                be_api.be_pushfstring(vm,
                    "the value of `%s::tostring()` is not a 'string'",
                    name_str))
        s = var_tostr(vm.stack[vm.top_idx])
    return s


# ---------------------------------------------------------------------------
# void be_val2str(bvm *vm, int index)
# {
#     bstring *s;
#     int idx = be_absindex(vm, index) - 1;
#     bvalue *v = vm->reg + idx;
#     if (var_isstr(v)) return;
#     s = var_isinstance(v) ? ins2str(vm, idx) : sim2str(vm, v);
#     v = vm->reg + idx;
#     var_setstr(v, s);
# }
def be_val2str(vm, index):
    """Convert the value at stack index to a string in-place."""
    be_api = _lazy_be_api()
    idx = be_api.be_absindex(vm, index) - 1
    v = vm.stack[vm.reg_idx + idx]
    if var_isstr(v):
        return  # already a string
    if var_isinstance(v):
        s = _ins2str(vm, idx)
    else:
        s = _sim2str(vm, v)
    # the stack may have changed, re-fetch
    v = vm.stack[vm.reg_idx + idx]
    var_setstr(v, s)


# ---------------------------------------------------------------------------
# static void pushstr(bvm *vm, const char *s, size_t len)
# {
#     bstring *str = be_newstrn(vm, s, len);
#     bvalue *reg = be_incrtop(vm);
#     var_setstr(reg, str);
# }
def _pushstr(vm, s, length):
    """Create a string and push it onto the stack."""
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()
    str_obj = be_string.be_newstrn(vm, s, length)
    idx = be_api.be_incrtop(vm)
    var_setstr(vm.stack[idx], str_obj)


# ---------------------------------------------------------------------------
# static const char* concat2(bvm *vm)
# {
#     bvalue *dst = vm->top - 2;
#     bstring *s1 = var_tostr(dst);
#     bstring *s2 = var_tostr(dst + 1);
#     bstring *s = be_strcat(vm, s1, s2);
#     dst = vm->top - 2;
#     var_setstr(dst, s);
#     --vm->top;
#     return str(s);
# }
def _concat2(vm):
    """Concatenate the top two stack strings, pop one, return Python str."""
    be_string = _lazy_be_string()
    dst_idx = vm.top_idx - 2
    s1 = var_tostr(vm.stack[dst_idx])
    s2 = var_tostr(vm.stack[dst_idx + 1])
    s = be_strcat(vm, s1, s2)
    # the stack may change after be_strcat
    dst_idx = vm.top_idx - 2
    var_setstr(vm.stack[dst_idx], s)
    vm.top_idx -= 1
    return be_string.be_str2cstr(s)


# ---------------------------------------------------------------------------
# const char* be_pushvfstr(bvm *vm, const char *format, va_list arg)
# {
#     pushstr(vm, "", 0);
#     for (;;) {
#         const char *p = strchr(format, '%');
#         if (p == NULL) break;
#         pushstr(vm, format, p - format);
#         concat2(vm);
#         switch (p[1]) {
#         case 's': { ... }
#         case 'd': { ... }
#         case 'f': case 'g': { ... }
#         case 'c': { ... }
#         case '%': { ... }
#         case 'p': { ... }
#         default: pushstr(vm, "(unknown)", 8); break;
#         }
#         concat2(vm);
#         format = p + 2;
#     }
#     pushstr(vm, format, strlen(format));
#     return concat2(vm);
# }
def be_pushvfstr(vm, fmt, args):
    """Format a string using Berry's internal %s/%d/%f/%g/%c/%% format and push result.

    This is the low-level varargs version. `args` is a list of values.
    Returns the resulting Python string.
    """
    be_string = _lazy_be_string()
    arg_idx = 0
    _pushstr(vm, "", 0)
    format_str = fmt
    while True:
        p = format_str.find('%')
        if p < 0:
            break
        _pushstr(vm, format_str[:p], p)
        _concat2(vm)
        spec = format_str[p + 1] if p + 1 < len(format_str) else '\0'
        if spec == 's':
            s = args[arg_idx] if arg_idx < len(args) else "(null)"
            if s is None:
                s = "(null)"
            _pushstr(vm, s, len(s))
            arg_idx += 1
        elif spec == 'd':
            val = args[arg_idx] if arg_idx < len(args) else 0
            arg_idx += 1
            v = bvalue()
            var_setint(v, int(val))
            s_obj = be_num2str(vm, v)
            idx = _lazy_be_api().be_incrtop(vm)
            var_setstr(vm.stack[idx], s_obj)
        elif spec in ('f', 'g'):
            val = args[arg_idx] if arg_idx < len(args) else 0.0
            arg_idx += 1
            v = bvalue()
            var_setreal(v, float(val))
            s_obj = be_num2str(vm, v)
            idx = _lazy_be_api().be_incrtop(vm)
            var_setstr(vm.stack[idx], s_obj)
        elif spec == 'c':
            val = args[arg_idx] if arg_idx < len(args) else 0
            arg_idx += 1
            c = chr(int(val) & 0xFF)
            _pushstr(vm, c, 1)
        elif spec == '%':
            _pushstr(vm, "%", 1)
        elif spec == 'p':
            val = args[arg_idx] if arg_idx < len(args) else None
            arg_idx += 1
            buf = "0x%x" % (id(val) if val is not None else 0)
            _pushstr(vm, buf, len(buf))
        else:
            _pushstr(vm, "(unknown)", 9)
        _concat2(vm)
        format_str = format_str[p + 2:]
    _pushstr(vm, format_str, len(format_str))
    return _concat2(vm)


# ---------------------------------------------------------------------------
# int be_char2hex(int c)
# {
#     if (c >= '0' && c <= '9') { return c - '0'; }
#     else if (c >= 'a' && c <= 'f') { return c - 'a' + 0x0A; }
#     else if (c >= 'A' && c <= 'F') { return c - 'A' + 0x0A; }
#     return -1;
# }
def be_char2hex(c):
    """Convert a hex character to its integer value, or -1 if invalid."""
    if isinstance(c, int):
        c = chr(c)
    if '0' <= c <= '9':
        return ord(c) - ord('0')
    elif 'a' <= c <= 'f':
        return ord(c) - ord('a') + 0x0A
    elif 'A' <= c <= 'F':
        return ord(c) - ord('A') + 0x0A
    return -1


# ---------------------------------------------------------------------------
# BERRY_API bint be_str2int(const char *str, const char **endstr)
# {
#     int c, sign;
#     bint sum = 0;
#     skip_space(str);
#     if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
#         str += 2;
#         while ((c = be_char2hex(*str++)) >= 0) {
#             sum = sum * 16 + c;
#         }
#         if (endstr) { *endstr = str - 1; }
#         return sum;
#     } else {
#         sign = c = *str++;
#         if (c == '+' || c == '-') { c = *str++; }
#         while (is_digit(c)) {
#             if (sign == '-') {
#                 if (sum < M_IMIN / 10) goto overflow_neg;
#                 sum = sum * 10 - (c - '0');
#                 if (sum > 0) goto overflow_neg;
#             } else {
#                 if (sum > M_IMAX / 10) goto overflow_pos;
#                 sum = sum * 10 + (c - '0');
#                 if (sum < 0) goto overflow_pos;
#             }
#             c = *str++;
#         }
#         if (endstr) { *endstr = str - 1; }
#         return sum;
#     }
# overflow_pos: ... return M_IMAX;
# overflow_neg: ... return M_IMIN;
# }
def be_str2int(s, return_end=False):
    """Parse an integer from a string. Supports decimal and 0x hex.

    If return_end is True, returns (value, end_index).
    Otherwise returns just the value.
    """
    pos = 0
    # skip_space
    while pos < len(s) and _is_space(s[pos]):
        pos += 1

    if pos + 1 < len(s) and s[pos] == '0' and s[pos + 1] in ('x', 'X'):
        # hex literal
        pos += 2
        total = 0
        while pos < len(s):
            c = be_char2hex(s[pos])
            if c < 0:
                break
            total = total * 16 + c
            pos += 1
        # Mask to 64-bit signed range
        total = _to_signed64(total)
        if return_end:
            return (total, pos)
        return total
    else:
        # decimal literal
        total = 0
        sign_char = s[pos] if pos < len(s) else '\0'
        c_char = sign_char
        if pos < len(s):
            pos += 1
        if c_char in ('+', '-'):
            c_char = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1

        while c_char != '\0' and _is_digit(c_char):
            digit = ord(c_char) - ord('0')
            if sign_char == '-':
                if total < M_IMIN // 10:
                    # overflow_neg
                    if return_end:
                        return (M_IMIN, pos - 1)
                    return M_IMIN
                total = total * 10 - digit
                if total > 0:
                    if return_end:
                        return (M_IMIN, pos - 1)
                    return M_IMIN
            else:
                if total > M_IMAX // 10:
                    if return_end:
                        return (M_IMAX, pos - 1)
                    return M_IMAX
                total = total * 10 + digit
                if total < 0:
                    if return_end:
                        return (M_IMAX, pos - 1)
                    return M_IMAX
            c_char = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1

        end_pos = pos - 1 if c_char == '\0' and pos > 0 else pos - 1
        # Adjust: if we consumed past the end, endstr points to last valid
        if c_char == '\0':
            end_pos = pos - 1 if pos > 0 else 0
        else:
            end_pos = pos - 1

        if return_end:
            return (total, end_pos)
        return total


def _to_signed64(v):
    """Mask an integer to signed 64-bit range."""
    v = v & 0xFFFFFFFFFFFFFFFF
    if v >= 0x8000000000000000:
        v -= 0x10000000000000000
    return v


# ---------------------------------------------------------------------------
# BERRY_API breal be_str2real(const char *str, const char **endstr)
# {
#     int c, sign;
#     breal sum = 0, deci = 0, point = 0.1;
#     skip_space(str);
#     sign = c = *str++;
#     if (c == '+' || c == '-') c = *str++;
#     while (is_digit(c)) {
#         if (sum > BREAL_MAX / 10) goto overflow;
#         sum = sum * 10 + (c - '0');
#         if (sum > BREAL_MAX) goto overflow;
#         c = *str++;
#     }
#     if (c == '.') {
#         c = *str++;
#         while (is_digit(c)) {
#             deci += (c - '0') * point;
#             point *= 0.1;
#             c = *str++;
#         }
#     }
#     sum += deci;
#     if (c == 'e' || c == 'E') { ... exponent ... }
#     if (endstr) *endstr = str - 1;
#     return sign == '-' ? -sum : sum;
# overflow:
#     return sign == '-' ? -HUGE_VAL : HUGE_VAL;
def _f32(x):
    """Round a Python float through float32, matching C single-precision."""
    return struct.unpack('<f', struct.pack('<f', x))[0]


# }
def be_str2real(s, return_end=False):
    """Parse a real number from a string.

    If return_end is True, returns (value, end_index).
    Otherwise returns just the value.

    When BE_USE_SINGLE_FLOAT is enabled, all intermediate arithmetic is
    rounded through float32 at each step to match C's single-precision
    accumulation behaviour.
    """
    f = _f32 if BE_USE_SINGLE_FLOAT else lambda x: x

    pos = 0
    # skip_space
    while pos < len(s) and _is_space(s[pos]):
        pos += 1

    sign_char = s[pos] if pos < len(s) else '\0'
    c = sign_char
    if pos < len(s):
        pos += 1
    if c in ('+', '-'):
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1

    total = f(0.0)
    # Integer part
    while c != '\0' and _is_digit(c):
        if total > BREAL_MAX / 10:
            # overflow
            end_pos = pos - 1
            val = -math.inf if sign_char == '-' else math.inf
            if return_end:
                return (val, end_pos)
            return val
        total = f(f(total * 10) + (ord(c) - ord('0')))
        if total > BREAL_MAX:
            end_pos = pos - 1
            val = -math.inf if sign_char == '-' else math.inf
            if return_end:
                return (val, end_pos)
            return val
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1

    # Fractional part
    if c == '.':
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1
        point = f(0.1)
        deci = f(0.0)
        while c != '\0' and _is_digit(c):
            digit = f((ord(c) - ord('0')) * point)
            new_deci = f(deci + digit)
            if new_deci < deci:
                break  # precision limit
            deci = new_deci
            point = f(point * f(0.1))
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        total = f(total + deci)

    if total > BREAL_MAX:
        end_pos = pos - 1
        val = -math.inf if sign_char == '-' else math.inf
        if return_end:
            return (val, end_pos)
        return val

    # Scientific notation
    if c in ('e', 'E'):
        c = s[pos] if pos < len(s) else '\0'
        if pos < len(s):
            pos += 1
        esign = 1
        if c == '-':
            esign = -1
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        elif c == '+':
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        e = 0
        while c != '\0' and _is_digit(c):
            if e > 300:
                end_pos = pos - 1
                val = -math.inf if sign_char == '-' else math.inf
                if return_end:
                    return (val, end_pos)
                return val
            e = e * 10 + (ord(c) - ord('0'))
            c = s[pos] if pos < len(s) else '\0'
            if pos < len(s):
                pos += 1
        e *= esign
        ratio = f(0.1) if e < 0 else f(10.0)
        if e < 0:
            e = -e
        while e > 0:
            e -= 1
            total = f(total * ratio)

    # Compute end position
    if c == '\0':
        end_pos = pos - 1 if pos > 0 else 0
    else:
        end_pos = pos - 1

    val = -total if sign_char == '-' else total
    if return_end:
        return (val, end_pos)
    return val


# ---------------------------------------------------------------------------
# BERRY_API const char *be_str2num(bvm *vm, const char *str)
# {
#     const char *sout;
#     bint c, vint = be_str2int(str, &sout);
#     c = *sout;
#     if (c == '.' || c == 'e' || c == 'E') {
#         be_pushreal(vm, be_str2real(str, &sout));
#     } else {
#         be_pushint(vm, vint);
#     }
#     return sout;
# }
def be_str2num(vm, s):
    """Convert a string to a number (int or real) and push onto the VM stack.

    Returns the end position index in the string.
    """
    be_api = _lazy_be_api()
    vint, sout = be_str2int(s, return_end=True)
    c = s[sout] if sout < len(s) else '\0'
    if c in ('.', 'e', 'E'):
        rval, sout = be_str2real(s, return_end=True)
        be_api.be_pushreal(vm, rval)
    else:
        be_api.be_pushint(vm, vint)
    return sout


# ---------------------------------------------------------------------------
# static bstring* string_range(bvm *vm, bstring *str, binstance *range)
# {
#     bint lower, upper;
#     bint size = str_len(str);
#     bvalue temp;
#     be_instance_member(vm, range, be_newstr(vm, "__lower__"), &temp);
#     lower = var_toint(&temp);
#     be_instance_member(vm, range, be_newstr(vm, "__upper__"), &temp);
#     upper = var_toint(&temp);
#     if (upper < 0) { upper = size + upper; }
#     if (lower < 0) { lower = size + lower; }
#     upper = upper < size ? upper : size - 1;
#     lower = lower < 0 ? 0 : lower;
#     if (lower > upper) { return be_newstrn(vm, "", 0); }
#     return be_newstrn(vm, str(str) + lower, upper - lower + 1);
# }
def _string_range(vm, str_obj, range_inst):
    """Extract a substring using a range instance."""
    be_string = _lazy_be_string()
    be_cls = _lazy_be_class()
    size = str_len(str_obj)
    temp = bvalue()
    be_cls.be_instance_member(vm, range_inst, be_string.be_newstr(vm, "__lower__"), temp)
    lower = var_toint(temp)
    be_cls.be_instance_member(vm, range_inst, be_string.be_newstr(vm, "__upper__"), temp)
    upper = var_toint(temp)
    if upper < 0:
        upper = size + upper
    if lower < 0:
        lower = size + lower
    upper = upper if upper < size else size - 1
    lower = lower if lower >= 0 else 0
    if lower > upper:
        return be_string.be_newstrn(vm, "", 0)
    data = be_string.be_str2cstr(str_obj)
    return be_string.be_newstrn(vm, data[lower:upper + 1], upper - lower + 1)


# ---------------------------------------------------------------------------
# bstring* be_strindex(bvm *vm, bstring *str, bvalue *idx)
# {
#     if (var_isint(idx)) {
#         int pos = var_toidx(idx);
#         int size = str_len(str);
#         if (pos < 0) { pos = size + pos; }
#         if ((pos < size) && (pos >= 0)) {
#             return be_newstrn(vm, str(str) + pos, 1);
#         }
#         be_raise(vm, "index_error", "string index out of range");
#     } else if (var_isinstance(idx)) {
#         binstance *ins = var_toobj(idx);
#         const char *cname = str(be_instance_name(ins));
#         if (!strcmp(cname, "range")) {
#             return string_range(vm, str, ins);
#         }
#     }
#     be_raise(vm, "index_error", "string indices must be integers");
#     return NULL;
# }
def be_strindex(vm, str_obj, idx):
    """String subscript operation: str[int] or str[range]."""
    be_string = _lazy_be_string()
    be_api = _lazy_be_api()
    if var_isint(idx):
        pos = var_toidx(idx)
        size = str_len(str_obj)
        if pos < 0:
            pos = size + pos
        if 0 <= pos < size:
            data = be_string.be_str2cstr(str_obj)
            return be_string.be_newstrn(vm, data[pos], 1)
        be_api.be_raise(vm, "index_error", "string index out of range")
    elif var_isinstance(idx):
        ins = var_toobj(idx)
        cname = be_string.be_str2cstr(be_instance_name(ins))
        if cname == "range":
            return _string_range(vm, str_obj, ins)
    be_api.be_raise(vm, "index_error", "string indices must be integers")
    return None


# ---------------------------------------------------------------------------
# size_t be_strlcpy(char *dst, const char *src, size_t maxlen)
# {
#     const size_t srclen = strlen(src);
#     if (srclen + 1 < maxlen) {
#         memcpy(dst, src, srclen + 1);
#     } else if (maxlen != 0) {
#         memcpy(dst, src, maxlen - 1);
#         dst[maxlen-1] = '\0';
#     }
#     return srclen;
# }
def be_strlcpy(src, maxlen):
    """Copy src into a buffer of maxlen, truncating if needed. Returns (result, srclen)."""
    srclen = len(src)
    if srclen + 1 < maxlen:
        return (src, srclen)
    elif maxlen != 0:
        return (src[:maxlen - 1], srclen)
    return ("", srclen)


# ---------------------------------------------------------------------------
# const char* be_splitpath(const char *path)
# {
#     const char *p;
#     for (p = path - 1; *path != '\0'; ++path) {
#         if (*path == '/') { p = path; }
#     }
#     return p + 1;
# }
def be_splitpath(path):
    """Return the filename portion of a path (after last '/')."""
    last_slash = -1
    for i, c in enumerate(path):
        if c == '/':
            last_slash = i
    return path[last_slash + 1:]


# ---------------------------------------------------------------------------
# const char* be_splitname(const char *path)
# {
#     const char *p, *q, *end = path + strlen(path);
#     for (p = end; *p != '.' && p > path; --p);
#     for (q = p; *q == '.' && q > path; --q);
#     if ((q == path && *q == '.') || *q == '/') { return end; }
#     return p;
# }
def be_splitname(path):
    """Return the extension portion of a path (from last '.' before end).

    Returns the index into path where the extension starts, or len(path) if none.
    """
    end = len(path)
    # find last '.'
    p = end - 1
    while p > 0 and path[p] != '.':
        p -= 1
    if p <= 0:
        # no dot found or dot at start
        if p == 0 and path[p] == '.':
            return end
        return end
    # skip consecutive dots
    q = p
    while q > 0 and path[q] == '.':
        q -= 1
    if (q == 0 and path[q] == '.') or path[q] == '/':
        return end
    return p


# ---------------------------------------------------------------------------
# Escape helpers
# ---------------------------------------------------------------------------
# static unsigned escape_length(const char *s, int quote)
# {
#     unsigned c, len = 0, step = quote == '"' ? 5 : 3;
#     for (; (c = *s) != '\0'; ++s) {
#         switch (c) {
#         case '\\': case '\n': case '\r': case '\t':
#             len += 1; break;
#         default:
#             if (c < 0x20) len += step;
#             else if (c == (unsigned)quote) len += 1;
#             break;
#         }
#     }
#     return len;
# }
def _escape_length(s, quote):
    """Calculate extra length needed for escape encoding."""
    length = 0
    step = 5 if quote == '"' else 3
    for c in s:
        o = ord(c)
        if c in ('\\', '\n', '\r', '\t'):
            length += 1
        elif o < 0x20:
            length += step
        elif o == quote:
            length += 1
    return length


# static unsigned eschex(unsigned num)
# {
#     return num <= 9 ? '0' + num : 'a' + num - 10;
# }
def _eschex(num):
    """Convert a nibble to a hex character."""
    return chr(ord('0') + num) if num <= 9 else chr(ord('a') + num - 10)


# static char* escape(char *q, unsigned c, int quote)
# {
#     int json = quote == '"';
#     switch (c) {
#     case '\\': *q++ = '\\'; *q = '\\'; break;
#     case '\n': *q++ = '\\'; *q = 'n'; break;
#     case '\r': *q++ = '\\'; *q = 'r'; break;
#     case '\t': *q++ = '\\'; *q = 't'; break;
#     default:
#         if (c < 0x20) {
#             *q++ = '\\';
#             if (json) { *q++ = 'u'; *q++ = '0'; *q++ = '0'; ... }
#             else { *q++ = 'x'; ... }
#         } else {
#             if (c == (unsigned)quote) *q++ = '\\';
#             *q = (char)c;
#         }
#         break;
#     }
#     return q;
# }
def _escape_char(c, quote):
    """Escape a single character. Returns the escaped string fragment."""
    o = ord(c)
    json = (quote == ord('"')) if isinstance(quote, int) else (quote == '"')
    q_ord = quote if isinstance(quote, int) else ord(quote)
    if c == '\\':
        return '\\\\'
    elif c == '\n':
        return '\\n'
    elif c == '\r':
        return '\\r'
    elif c == '\t':
        return '\\t'
    elif o < 0x20:
        if json:
            return '\\u00' + _eschex(o >> 4) + _eschex(o & 0x0f)
        else:
            return '\\x' + _eschex(o >> 4) + _eschex(o & 0x0f)
    elif o == q_ord:
        return '\\' + c
    else:
        return c


# static void toescape(bvm *vm, int index, int quote)
# {
#     char *buf, *q;
#     const char *p, *s = be_tostring(vm, index);
#     size_t len = (size_t)be_strlen(vm, index);
#     len += escape_length(s, quote) + 2;
#     buf = q = be_pushbuffer(vm, len);
#     *q++ = (char)quote;
#     for (p = s; *p != '\0'; ++p, ++q) {
#         q = escape(q, *p, quote);
#     }
#     *q = (char)quote;
#     be_pushnstring(vm, buf, len);
#     be_moveto(vm, -1, index);
#     be_pop(vm, 2);
# }
def _toescape(vm, index, quote):
    """Escape a string value at stack index with given quote character."""
    be_api = _lazy_be_api()
    s = be_api.be_tostring(vm, index)
    q_char = chr(quote) if isinstance(quote, int) else quote
    # Build escaped string
    parts = [q_char]
    for c in s:
        parts.append(_escape_char(c, q_char))
    parts.append(q_char)
    result = ''.join(parts)
    be_api.be_pushnstring(vm, result, len(result))
    be_api.be_moveto(vm, -1, index)
    be_api.be_pop(vm, 1)


# ---------------------------------------------------------------------------
# BERRY_API const char* be_toescape(bvm *vm, int index, int mode)
# {
#     if (be_isstring(vm, index)) {
#         index = be_absindex(vm, index);
#         toescape(vm, index, mode == 'u' ? '"' : '\'');
#     }
#     return be_tostring(vm, index);
# }
def be_toescape(vm, index, mode):
    """Escape a string at stack index. mode='u' uses double quotes, else single."""
    be_api = _lazy_be_api()
    if be_api.be_isstring(vm, index):
        index = be_api.be_absindex(vm, index)
        quote = '"' if mode == 'u' or mode == ord('u') else "'"
        _toescape(vm, index, quote)
    return be_api.be_tostring(vm, index)


# ===========================================================================
# String module functions (BE_USE_STRING_MODULE)
# ===========================================================================

# ---------------------------------------------------------------------------
# Format helpers
# ---------------------------------------------------------------------------
# #define MAX_FORMAT_MODE     32
# #define FLAGES              "+- #0"
MAX_FORMAT_MODE = 32
FLAGES = "+- #0"


# static const char* skip2dig(const char *s)
def _skip2dig(s, pos):
    """Skip up to 2 digits, return new position."""
    if pos < len(s) and _is_digit(s[pos]):
        pos += 1
    if pos < len(s) and _is_digit(s[pos]):
        pos += 1
    return pos


# static const char* get_mode(const char *str, char *buf, size_t buf_len)
# {
#     const char *p = str;
#     while (*p && strchr(FLAGES, *p)) { ++p; }
#     p = skip2dig(p);
#     if (*p == '.') { p = skip2dig(++p); }
#     *(buf++) = '%';
#     ...
#     return p;
# }
def _get_mode(fmt, start):
    """Parse a printf-style format mode starting after '%'.

    Returns (mode_string_with_%, end_position_of_specifier).
    end_position points to the conversion specifier character.
    """
    p = start
    # skip flags
    while p < len(fmt) and fmt[p] in FLAGES:
        p += 1
    # skip width (up to 2 digits)
    p = _skip2dig(fmt, p)
    # skip precision (width after dot, 2 digits at most)
    if p < len(fmt) and fmt[p] == '.':
        p += 1
        p = _skip2dig(fmt, p)
    # p now points to the conversion specifier
    mode = '%' + fmt[start:p + 1] if p < len(fmt) else '%'
    return (mode, p)


# static void mode_fixlen(char *mode, const char *lenmode)
# -- In Python we don't need length modifiers for format strings,
# -- but we keep the function for structural mirroring.
def _mode_fixlen(mode, lenmode):
    """Fix format mode with length modifier (no-op in Python)."""
    # In C this inserts "ll" before the specifier. Python handles int sizes natively.
    return mode


# static bbool convert_to_int(bvm *vm, int index, bint *val)
def _convert_to_int(vm, index):
    """Try to convert stack value at index to int. Returns (success, value)."""
    be_api = _lazy_be_api()
    if be_api.be_isint(vm, index):
        return (True, be_api.be_toint(vm, index))
    # Try calling int() builtin
    try:
        from berry_port.be_baselib import be_baselib_int
        be_api.be_pushntvfunction(vm, be_baselib_int)
        be_api.be_pushvalue(vm, index)
        be_api.be_call(vm, 1)
        be_api.be_pop(vm, 1)
        if be_api.be_isint(vm, -1):
            val = be_api.be_toint(vm, -1)
            be_api.be_pop(vm, 1)
            return (True, val)
        be_api.be_pop(vm, 1)
    except (ImportError, Exception):
        pass
    return (False, 0)


# static bbool convert_to_real(bvm *vm, int index, breal *val)
def _convert_to_real(vm, index):
    """Try to convert stack value at index to real. Returns (success, value)."""
    be_api = _lazy_be_api()
    if be_api.be_isnumber(vm, index):
        return (True, be_api.be_toreal(vm, index))
    try:
        from berry_port.be_baselib import be_baselib_real
        be_api.be_pushntvfunction(vm, be_baselib_real)
        be_api.be_pushvalue(vm, index)
        be_api.be_call(vm, 1)
        be_api.be_pop(vm, 1)
        if be_api.be_isnumber(vm, -1):
            val = be_api.be_toreal(vm, -1)
            be_api.be_pop(vm, 1)
            return (True, val)
        be_api.be_pop(vm, 1)
    except (ImportError, Exception):
        pass
    return (False, 0.0)


# ---------------------------------------------------------------------------
# int be_str_format(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top > 0 && be_isstring(vm, 1)) {
#         int index = 2;
#         const char *format = be_tostring(vm, 1);
#         pushstr(vm, "", 0);
#         for (;;) {
#             char mode[MAX_FORMAT_MODE]; char buf[128];
#             const char *p = strchr(format, '%');
#             if (p == NULL) break;
#             pushstr(vm, format, p - format);
#             concat2(vm);
#             p = get_mode(p + 1, mode, sizeof(mode));
#             ...switch on *p...
#             concat2(vm);
#             format = p + 1;
#             ++index;
#         }
#         pushstr(vm, format, strlen(format));
#         concat2(vm);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def be_str_format(vm):
    """Implement string.format(fmt, ...) — Berry's printf-style formatter."""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top > 0 and be_api.be_isstring(vm, 1):
        index = 2
        fmt = be_api.be_tostring(vm, 1)
        _pushstr(vm, "", 0)
        pos = 0
        while True:
            p = fmt.find('%', pos)
            if p < 0:
                break
            _pushstr(vm, fmt[pos:p], p - pos)
            _concat2(vm)
            mode, spec_pos = _get_mode(fmt, p + 1)
            spec = fmt[spec_pos] if spec_pos < len(fmt) else '\0'
            buf = ""
            if index > top and spec != '%':
                be_api.be_raise(vm, "runtime_error",
                    be_api.be_pushfstring(vm,
                        "bad argument #%d to 'format': no value", index))
            if spec == '%':
                be_api.be_pushstring(vm, "%")
                index -= 1  # compensate the future ++index
            elif spec in ('d', 'i', 'o', 'u', 'x', 'X'):
                ok, val = _convert_to_int(vm, index)
                if ok:
                    # Use Python format with the mode
                    buf = _python_int_format(mode, val)
                be_api.be_pushstring(vm, buf)
            elif spec in ('e', 'E', 'f', 'g', 'G'):
                ok, val = _convert_to_real(vm, index)
                if ok:
                    try:
                        buf = mode % val
                    except (TypeError, ValueError):
                        buf = str(val)
                be_api.be_pushstring(vm, buf)
            elif spec == 'c':
                ok, val = _convert_to_int(vm, index)
                if ok:
                    buf = chr(int(val) & 0xFF)
                be_api.be_pushstring(vm, buf)
            elif spec == 's':
                s = be_api.be_tostring(vm, index)
                slen = be_api.be_strlen(vm, index)
                if slen > 100 and len(mode) == 2:
                    be_api.be_pushvalue(vm, index)
                else:
                    try:
                        buf = mode % s
                    except (TypeError, ValueError):
                        buf = s
                    be_api.be_pushstring(vm, buf)
            elif spec == 'q':
                be_toescape(vm, index, 'q')
                s = be_api.be_tostring(vm, index)
                slen = be_api.be_strlen(vm, index)
                if slen > 100 and len(mode) == 2:
                    be_api.be_pushvalue(vm, index)
                else:
                    be_api.be_pushstring(vm, s)
            else:
                be_api.be_raise(vm, "runtime_error",
                    be_api.be_pushfstring(vm,
                        "invalid option '%%%c' to 'format'", spec))
            _concat2(vm)
            pos = spec_pos + 1
            index += 1
        _pushstr(vm, fmt[pos:], len(fmt) - pos)
        _concat2(vm)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


def _python_int_format(mode, val):
    """Apply a C-style integer format mode using Python formatting."""
    # mode is like '%d', '%04x', '%-10X', '%o', '%u' etc.
    # Map C specifiers to Python format
    spec = mode[-1]
    prefix = mode[1:-1]  # flags + width + precision between % and specifier
    if spec == 'i':
        spec = 'd'
    if spec == 'u':
        # unsigned: mask to positive
        if val < 0:
            val = val & 0xFFFFFFFFFFFFFFFF
        spec = 'd'
    try:
        py_fmt = '%' + prefix + spec
        return py_fmt % val
    except (TypeError, ValueError):
        return str(val)


# ---------------------------------------------------------------------------
# String operation helpers (find, count)
# ---------------------------------------------------------------------------
# static bint str_operation(bvm *vm, str_opfunc func, bint error)
# {
#     int top = be_top(vm);
#     if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
#         int len1 = be_strlen(vm, 1);
#         int len2 = be_strlen(vm, 2);
#         const char *s1 = be_tostring(vm, 1);
#         const char *s2 = be_tostring(vm, 2);
#         bint begin = top >= 3 && be_isint(vm, 3) ? be_toint(vm, 3) : 0;
#         bint end = top >= 4 && be_isint(vm, 4) ? be_toint(vm, 4) : len1;
#         if (begin >= 0 && begin <= len1 && end - begin >= len2) {
#             return func(s1, s2, begin, end - len2);
#         }
#     }
#     return error;
# }
def _str_operation(vm, func, error):
    """Generic string operation dispatcher for find/count."""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 2 and be_api.be_isstring(vm, 1) and be_api.be_isstring(vm, 2):
        len1 = be_api.be_strlen(vm, 1)
        len2 = be_api.be_strlen(vm, 2)
        s1 = be_api.be_tostring(vm, 1)
        s2 = be_api.be_tostring(vm, 2)
        begin = be_api.be_toint(vm, 3) if (top >= 3 and be_api.be_isint(vm, 3)) else 0
        end = be_api.be_toint(vm, 4) if (top >= 4 and be_api.be_isint(vm, 4)) else len1
        if begin >= 0 and begin <= len1 and end - begin >= len2:
            return func(s1, s2, begin, end - len2)
    return error


# static bint _sfind(const char *s1, const char *s2, bint begin, bint end)
# {
#     const char *res = strstr(s1 + begin, s2);
#     if (res) {
#         bint pos = (bint)(res - s1);
#         return pos <= end ? pos : -1;
#     }
#     return -1;
# }
def _sfind(s1, s2, begin, end):
    """Find s2 in s1[begin:], return position or -1."""
    pos = s1.find(s2, begin)
    if pos >= 0:
        return pos if pos <= end else -1
    return -1


# static int str_find(bvm *vm)
# {
#     be_pushint(vm, str_operation(vm, _sfind, -1));
#     be_return(vm);
# }
def str_find(vm):
    """string.find(s, pattern [, begin [, end]])"""
    be_api = _lazy_be_api()
    be_api.be_pushint(vm, _str_operation(vm, _sfind, -1))
    return be_api.be_returnvalue(vm)


# static bint _scount(const char *s1, const char *s2, bint begin, bint end)
# {
#     bint count = 0;
#     const char *res = s1 + begin, *send = s1 + end;
#     while ((res = strstr(res, s2)) != NULL && res <= send) {
#         count += 1;
#         res += 1;
#     }
#     return count;
# }
def _scount(s1, s2, begin, end):
    """Count occurrences of s2 in s1[begin:end+len(s2)]."""
    count = 0
    pos = begin
    while True:
        pos = s1.find(s2, pos)
        if pos < 0 or pos > end:
            break
        count += 1
        pos += 1
    return count


# static int str_count(bvm *vm)
def str_count(vm):
    """string.count(s, pattern [, begin [, end]])"""
    be_api = _lazy_be_api()
    be_api.be_pushint(vm, _str_operation(vm, _scount, 0))
    return be_api.be_returnvalue(vm)


# ---------------------------------------------------------------------------
# static bbool _split_string(bvm *vm, int top)
# {
#     if (be_isstring(vm, 2)) {
#         const char *res;
#         int len1 = be_strlen(vm, 1);
#         int len2 = be_strlen(vm, 2);
#         const char *s1 = be_tostring(vm, 1);
#         const char *s2 = be_tostring(vm, 2);
#         bint count = len2
#             ? top >= 3 && be_isint(vm, 3) ? be_toint(vm, 3) : len1
#             : 0;
#         while (count-- && (res = strstr(s1, s2)) != NULL) {
#             be_pushnstring(vm, s1, res - s1);
#             be_data_push(vm, -2);
#             be_pop(vm, 1);
#             s1 = res + len2;
#         }
#         be_pushstring(vm, s1);
#         be_data_push(vm, -2);
#         be_pop(vm, 1);
#         return btrue;
#     }
#     return bfalse;
# }
def _split_string(vm, top):
    """Split string by substring delimiter."""
    be_api = _lazy_be_api()
    if be_api.be_isstring(vm, 2):
        len1 = be_api.be_strlen(vm, 1)
        len2 = be_api.be_strlen(vm, 2)
        s1 = be_api.be_tostring(vm, 1)
        s2 = be_api.be_tostring(vm, 2)
        count = 0
        if len2:
            count = be_api.be_toint(vm, 3) if (top >= 3 and be_api.be_isint(vm, 3)) else len1
        else:
            count = 0  # cannot match empty pattern
        while count > 0:
            count -= 1
            pos = s1.find(s2)
            if pos < 0:
                break
            be_api.be_pushnstring(vm, s1[:pos], pos)
            be_api.be_data_push(vm, -2)
            be_api.be_pop(vm, 1)
            s1 = s1[pos + len2:]
        be_api.be_pushstring(vm, s1)
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)
        return True
    return False


# static bbool _split_index(bvm *vm)
# {
#     if (be_isint(vm, 2)) {
#         int len = be_strlen(vm, 1), idx = be_toindex(vm, 2);
#         const char *s = be_tostring(vm, 1);
#         idx = idx > len ? len : idx < -len ? -len : idx;
#         if (idx < 0) { idx += len; }
#         be_pushnstring(vm, s, idx);
#         be_data_push(vm, -2);
#         be_pop(vm, 1);
#         be_pushnstring(vm, s + idx, (size_t)len - idx);
#         be_data_push(vm, -2);
#         be_pop(vm, 1);
#         return btrue;
#     }
#     return bfalse;
# }
def _split_index(vm):
    """Split string at integer index."""
    be_api = _lazy_be_api()
    if be_api.be_isint(vm, 2):
        slen = be_api.be_strlen(vm, 1)
        idx = be_api.be_toindex(vm, 2)
        s = be_api.be_tostring(vm, 1)
        idx = max(-slen, min(idx, slen))
        if idx < 0:
            idx += slen
        be_api.be_pushnstring(vm, s[:idx], idx)
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)
        be_api.be_pushnstring(vm, s[idx:], slen - idx)
        be_api.be_data_push(vm, -2)
        be_api.be_pop(vm, 1)
        return True
    return False


# static int str_split(bvm *vm)
# {
#     int top = be_top(vm);
#     be_newobject(vm, "list");
#     if (top >= 2 && be_isstring(vm, 1)) {
#         if (!_split_index(vm))
#             _split_string(vm, top);
#     }
#     be_pop(vm, 1);
#     be_return(vm);
# }
def str_split(vm):
    """string.split(s, delimiter_or_index [, count])"""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    be_api.be_newobject(vm, "list")
    if top >= 2 and be_api.be_isstring(vm, 1):
        if not _split_index(vm):
            _split_string(vm, top)
    be_api.be_pop(vm, 1)
    return be_api.be_returnvalue(vm)


# ---------------------------------------------------------------------------
# static int str_i2hex(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top && be_isint(vm, 1)) {
#         bint value = be_toint(vm, 1);
#         char fmt[10] = { "%" BE_INT_FMTLEN "X" }, buf[18];
#         if (top >= 2 && be_isint(vm, 2)) {
#             bint num = be_toint(vm, 2);
#             if (num > 0 && num <= 16) {
#                 snprintf(fmt, sizeof(fmt), "%%.%d" BE_INT_FMTLEN "X", (int)num);
#             }
#         }
#         snprintf(buf, sizeof(buf), fmt, value);
#         be_pushstring(vm, buf);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_i2hex(vm):
    """string.hex(value [, digits])"""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top and be_api.be_isint(vm, 1):
        value = be_api.be_toint(vm, 1)
        if top >= 2 and be_api.be_isint(vm, 2):
            num = be_api.be_toint(vm, 2)
            if 0 < num <= 16:
                buf = ("%0*X" % (num, value & 0xFFFFFFFFFFFFFFFF))
            else:
                buf = "%X" % (value & 0xFFFFFFFFFFFFFFFF)
        else:
            buf = "%X" % (value & 0xFFFFFFFFFFFFFFFF)
        be_api.be_pushstring(vm, buf)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_byte(bvm *vm)
# {
#     if (be_top(vm) && be_isstring(vm, 1)) {
#         const bbyte *s = (const bbyte *)be_tostring(vm, 1);
#         be_pushint(vm, *s);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_byte(vm):
    """string.byte(s) — return the byte value of the first character."""
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isstring(vm, 1):
        s = be_api.be_tostring(vm, 1)
        if s:
            be_api.be_pushint(vm, ord(s[0]))
        else:
            be_api.be_pushint(vm, 0)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_char(bvm *vm)
# {
#     if (be_top(vm) && be_isint(vm, 1)) {
#         char c = be_toint(vm, 1) & 0xFF;
#         be_pushnstring(vm, &c, 1);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_char(vm):
    """string.char(n) — return a single-character string from byte value."""
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isint(vm, 1):
        c = chr(be_api.be_toint(vm, 1) & 0xFF)
        be_api.be_pushnstring(vm, c, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_touplower(bvm *vm, bbool up)
# {
#     if (be_top(vm) && be_isstring(vm, 1)) {
#         const char *s = be_tostring(vm, 1);
#         size_t len = (size_t)be_strlen(vm, 1);
#         char *buf = be_pushbuffer(vm, len);
#         for (p = s; *p != '\0'; ++p, ++q) {
#             *q = up ? toupper(*p) : tolower(*p);
#         }
#         be_pushnstring(vm, buf, len);
#         be_remove(vm, 2);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def _str_touplower(vm, up):
    """Convert string to upper or lower case."""
    be_api = _lazy_be_api()
    if be_api.be_top(vm) and be_api.be_isstring(vm, 1):
        s = be_api.be_tostring(vm, 1)
        result = s.upper() if up else s.lower()
        be_api.be_pushstring(vm, result)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# static int str_tolower(bvm *vm) { return str_touplower(vm, bfalse); }
def str_tolower(vm):
    """string.tolower(s)"""
    return _str_touplower(vm, False)


# static int str_toupper(bvm *vm) { return str_touplower(vm, btrue); }
def str_toupper(vm):
    """string.toupper(s)"""
    return _str_touplower(vm, True)


# ---------------------------------------------------------------------------
# static int str_tr(bvm *vm)
# {
#     if (be_top(vm) == 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isstring(vm, 3)) {
#         const char *s = be_tostring(vm, 1);
#         const char *t1 = be_tostring(vm, 2);
#         const char *t2 = be_tostring(vm, 3);
#         size_t len = (size_t)be_strlen(vm, 1);
#         char *buf = be_pushbuffer(vm, len);
#         for (p = s, q = buf; *p != '\0'; ++p, ++q) {
#             *q = *p;
#             for (p1=t1, p2=t2; *p1 != '\0'; ++p1) {
#                 if (*p == *p1) {
#                     if (*p2) { *q = *p2; }
#                     else { q--; len--; }
#                     break;
#                 }
#                 if (*p2) { p2++; }
#             }
#         }
#         be_pushnstring(vm, buf, len);
#         be_remove(vm, 2);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_tr(vm):
    """string.tr(s, from_chars, to_chars) — translate characters."""
    be_api = _lazy_be_api()
    if be_api.be_top(vm) == 3 and be_api.be_isstring(vm, 1) and \
       be_api.be_isstring(vm, 2) and be_api.be_isstring(vm, 3):
        s = be_api.be_tostring(vm, 1)
        t1 = be_api.be_tostring(vm, 2)
        t2 = be_api.be_tostring(vm, 3)
        result = []
        for ch in s:
            replaced = False
            p2_idx = 0
            for i, c1 in enumerate(t1):
                if ch == c1:
                    if p2_idx < len(t2):
                        result.append(t2[p2_idx])
                    # else: character is removed (not appended)
                    replaced = True
                    break
                if p2_idx < len(t2):
                    p2_idx += 1
            if not replaced:
                result.append(ch)
        be_api.be_pushstring(vm, ''.join(result))
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_replace(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isstring(vm, 3)) {
#         be_pushntvfunction(vm, &str_split);
#         be_pushvalue(vm, 1);
#         be_pushvalue(vm, 2);
#         be_call(vm, 2);
#         be_pop(vm, 2);
#         be_getmember(vm, -1, "concat");
#         be_pushvalue(vm, -2);
#         be_pushvalue(vm, 3);
#         be_call(vm, 2);
#         be_pop(vm, 2);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_replace(vm):
    """string.replace(s, old, new) — replace all occurrences."""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 3 and be_api.be_isstring(vm, 1) and \
       be_api.be_isstring(vm, 2) and be_api.be_isstring(vm, 3):
        # Use split then concat approach matching C implementation
        be_api.be_pushntvfunction(vm, str_split)
        be_api.be_pushvalue(vm, 1)
        be_api.be_pushvalue(vm, 2)
        be_api.be_call(vm, 2)
        be_api.be_pop(vm, 2)
        # Now top of stack is the list from split
        be_api.be_getmember(vm, -1, "concat")
        be_api.be_pushvalue(vm, -2)  # list instance as first arg
        be_api.be_pushvalue(vm, 3)   # replacement string
        be_api.be_call(vm, 2)
        be_api.be_pop(vm, 2)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_escape(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 1 && be_isstring(vm, 1)) {
#         int quote = 'u';
#         if (top >= 2 && be_isbool(vm, 2)) {
#             if (be_tobool(vm, 1)) { quote = 'x'; }
#         }
#         be_tostring(vm, 1);
#         be_toescape(vm, 1, quote);
#         be_pushvalue(vm, 1);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_escape(vm):
    """string.escape(s [, json_mode])"""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 1 and be_api.be_isstring(vm, 1):
        quote = 'u'
        if top >= 2 and be_api.be_isbool(vm, 2):
            if be_api.be_tobool(vm, 2):
                quote = 'x'
        be_api.be_tostring(vm, 1)
        be_toescape(vm, 1, quote)
        be_api.be_pushvalue(vm, 1)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_startswith(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
#         bbool case_insensitive = bfalse;
#         if (top >= 3 && be_isbool(vm, 3)) {
#             case_insensitive = be_tobool(vm, 3);
#         }
#         bbool result = bfalse;
#         const char *s = be_tostring(vm, 1);
#         const char *p = be_tostring(vm, 2);
#         size_t len = (size_t)be_strlen(vm, 2);
#         if (case_insensitive) {
#             if (str_strncasecmp(s, p, len) == 0) { result = btrue; }
#         } else {
#             if (strncmp(s, p, len) == 0) { result = btrue; }
#         }
#         be_pushbool(vm, result);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_startswith(vm):
    """string.startswith(s, prefix [, case_insensitive])"""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 2 and be_api.be_isstring(vm, 1) and be_api.be_isstring(vm, 2):
        case_insensitive = False
        if top >= 3 and be_api.be_isbool(vm, 3):
            case_insensitive = be_api.be_tobool(vm, 3)
        s = be_api.be_tostring(vm, 1)
        p = be_api.be_tostring(vm, 2)
        plen = len(p)
        if case_insensitive:
            result = str_strncasecmp(s, p, plen) == 0
        else:
            result = s[:plen] == p
        be_api.be_pushbool(vm, result)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)


# ---------------------------------------------------------------------------
# static int str_endswith(bvm *vm)
# {
#     int top = be_top(vm);
#     if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
#         bbool case_insensitive = bfalse;
#         if (top >= 3 && be_isbool(vm, 3)) {
#             case_insensitive = be_tobool(vm, 3);
#         }
#         bbool result = bfalse;
#         const char *s = be_tostring(vm, 1);
#         const char *p = be_tostring(vm, 2);
#         size_t len_s = (size_t)be_strlen(vm, 1);
#         size_t len_p = (size_t)be_strlen(vm, 2);
#         if (len_s >= len_p) {
#             if (case_insensitive) {
#                 if (str_strncasecmp(s + len_s - len_p, p, len_p) == 0) result = btrue;
#             } else {
#                 if (strncmp(s + len_s - len_p, p, len_p) == 0) result = btrue;
#             }
#         }
#         be_pushbool(vm, result);
#         be_return(vm);
#     }
#     be_return_nil(vm);
# }
def str_endswith(vm):
    """string.endswith(s, suffix [, case_insensitive])"""
    be_api = _lazy_be_api()
    top = be_api.be_top(vm)
    if top >= 2 and be_api.be_isstring(vm, 1) and be_api.be_isstring(vm, 2):
        case_insensitive = False
        if top >= 3 and be_api.be_isbool(vm, 3):
            case_insensitive = be_api.be_tobool(vm, 3)
        s = be_api.be_tostring(vm, 1)
        p = be_api.be_tostring(vm, 2)
        len_s = len(s)
        len_p = len(p)
        result = False
        if len_s >= len_p:
            tail = s[len_s - len_p:]
            if case_insensitive:
                result = str_strncasecmp(tail, p, len_p) == 0
            else:
                result = tail == p
        be_api.be_pushbool(vm, result)
        return be_api.be_returnvalue(vm)
    return be_api.be_returnnilvalue(vm)
