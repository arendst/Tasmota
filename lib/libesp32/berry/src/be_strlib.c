/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#include "be_strlib.h"
#include "be_string.h"
#include "be_vm.h"
#include "be_class.h"
#include "be_module.h"
#include "be_exec.h"
#include "be_mem.h"
#include "be_baselib.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define is_space(c)     ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n')
#define is_digit(c)     ((c) >= '0' && (c) <= '9')
#define skip_space(s)   while (is_space(*(s))) { ++(s); }

static int str_strncasecmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0) return 0;

    while (n-- != 0 && tolower(*s1) == tolower(*s2)) {
        if (n == 0 || *s1 == '\0' || *s2 == '\0')
            break;
        s1++;
        s2++;
    }

    return tolower(*(const unsigned char *)s1)
        - tolower(*(const unsigned char *)s2);
}

typedef bint (*str_opfunc)(const char*, const char*, bint, bint);

bstring* be_strcat(bvm *vm, bstring *s1, bstring *s2)
{
    size_t len = (size_t)str_len(s1) + str_len(s2);
    if (len <= SHORT_STR_MAX_LEN) {
        char buf[SHORT_STR_MAX_LEN + 1];
        strcpy(buf, str(s1));
        strncat(buf, str(s2), len);
        return be_newstrn(vm, buf, len);
    } else { /* long string */
        bstring *s = be_newstrn(vm, NULL, len);
        char *sbuf = (char*)str(s);
        strcpy(sbuf, str(s1));
        strcpy(sbuf + str_len(s1), str(s2));
        return s;
    }
}

int be_strcmp(bstring *s1, bstring *s2)
{
    if (be_eqstr(s1, s2)) {
        return 0;
    }
    return strcmp(str(s1), str(s2));
}

bstring* be_num2str(bvm *vm, bvalue *v)
{
    char buf[25];
    if (var_isint(v)) {
        snprintf(buf, sizeof(buf),BE_INT_FORMAT, var_toint(v));
    } else if (var_isreal(v)) {
        snprintf(buf, sizeof(buf), "%g", var_toreal(v));
    } else {
        snprintf(buf, sizeof(buf), "(nan)");
    }
    return be_newstr(vm, buf);
}

static void module2str(char *buf, size_t buf_len, bvalue *v)
{
    const char *name = be_module_name(cast(bmodule*, var_toobj(v)));
    if (name) {
        snprintf(buf, buf_len, "<module: %s>", name);
    } else {
        snprintf(buf, buf_len, "<module: %p>", var_toobj(v));
    }
}

static bstring* sim2str(bvm *vm, bvalue *v)
{
    char sbuf[64]; /* BUG: memory overflow */
    switch (var_type(v)) {
    case BE_NIL:
        strcpy(sbuf, "nil");
        break;
    case BE_BOOL:
        strcpy(sbuf, var_tobool(v) ? "true" : "false");
        break;
    case BE_INDEX:
    case BE_INT:
        snprintf(sbuf, sizeof(sbuf), BE_INT_FORMAT, var_toint(v));
        break;
    case BE_REAL:
        snprintf(sbuf, sizeof(sbuf), "%g", var_toreal(v));
        break;
    case BE_CLOSURE: case BE_NTVCLOS: case BE_NTVFUNC: case BE_CTYPE_FUNC:
        snprintf(sbuf, sizeof(sbuf), "<function: %p>", var_toobj(v));
        break;
    case BE_CLASS:
        snprintf(sbuf, sizeof(sbuf), "<class: %s>",
            str(be_class_name(cast(bclass*, var_toobj(v)))));
        break;
    case BE_MODULE:
        module2str(sbuf, sizeof(sbuf), v);
        break;
    case BE_COMPTR:
        snprintf(sbuf, sizeof(sbuf), "<ptr: %p>", var_toobj(v));
        break;
    default:
        strncpy(sbuf, "(unknown value)", sizeof(sbuf));
        break;
    }
    return be_newstr(vm, sbuf);
}

static bstring* ins2str(bvm *vm, int idx)
{
    bstring *s = str_literal(vm, "tostring");
    binstance *obj = var_toobj(vm->reg + idx);
    /* get method 'tostring' */
    int type = be_instance_member(vm, obj, s, vm->top);
    be_incrtop(vm); /* push the obj::tostring to stack */
    if (basetype(type) != BE_FUNCTION) {
        bstring *name = be_class_name(be_instance_class(obj));
        size_t buf_len = (size_t) str_len(name) + 16;
        char *sbuf = be_malloc(vm, buf_len);
        /* TODO what if sbuf cannot be allocated */
        snprintf(sbuf, buf_len, "<instance: %s()>", str(name));
        be_stackpop(vm, 1); /* pop the obj::tostring */
        s = be_newstr(vm, sbuf);
        be_free(vm, sbuf, (size_t)str_len(name) + 16);
    } else {
        *vm->top = vm->reg[idx];
        be_dofunc(vm, vm->top - 1, 1);
        be_stackpop(vm, 1); /* pop the obj::tostring */
        if (!var_isstr(vm->top)) { /* check the return value */
            const char *name = str(be_instance_name(obj));
            be_raise(vm, "runtime_error", be_pushfstring(vm,
                "the value of `%s::tostring()` is not a 'string'",
                strlen(name) ? name : "<anonymous>"));
        }
        s = var_tostr(vm->top);
    }
    return s;
}

void be_val2str(bvm *vm, int index)
{
    bstring *s;
    int idx = be_absindex(vm, index) - 1;
    bvalue *v = vm->reg + idx;
    if (var_isstr(v)) return; /* do nothing */
    s = var_isinstance(v) ? ins2str(vm, idx) : sim2str(vm, v);
    v = vm->reg + idx; /* the stack may change */
    var_setstr(v, s);
}

static void pushstr(bvm *vm, const char *s, size_t len)
{
    /* to create a string and then update the top pointer,
     * otherwise the GC may crash due to uninitialized values.
     **/
    bstring *str = be_newstrn(vm, s, len);
    bvalue *reg = be_incrtop(vm);
    var_setstr(reg, str);
}

static const char* concat2(bvm *vm)
{
    bvalue *dst = vm->top - 2;
    bstring *s1 = var_tostr(dst);
    bstring *s2 = var_tostr(dst + 1);
    bstring *s = be_strcat(vm, s1, s2);
    be_assert(var_isstr(vm->top - 2) && var_isstr(vm->top - 1));
    dst = vm->top - 2; /* the stack may change */
    var_setstr(dst, s);
    --vm->top;
    return str(s);
}

const char* be_pushvfstr(bvm *vm, const char *format, va_list arg)
{
    pushstr(vm, "", 0);
    for (;;) {
        const char *p = strchr(format, '%');
        if (p == NULL) {
            break;
        }
        pushstr(vm, format, p - format);
        concat2(vm);
        switch (p[1]) {
        case 's': {
            const char *s = va_arg(arg, char*);
            if (s == NULL) {
                s = "(null)";
            }
            pushstr(vm, s, strlen(s));
            break;
        }
        case 'd': {
            bstring *s;
            bvalue *v = be_incrtop(vm);
            var_setint(v, va_arg(arg, int));
            s = be_num2str(vm, v);
            var_setstr(v, s);
            break;
        }
        case 'f': case 'g': {
            bstring *s;
            bvalue *v = be_incrtop(vm);
            var_setreal(v, cast(breal, va_arg(arg, double)));
            s = be_num2str(vm, v);
            var_setstr(v, s);
            break;
        }
        case 'c': {
            char c = cast(char, va_arg(arg, int));
            pushstr(vm, &c, 1);
            break;
        }
        case '%': {
            pushstr(vm, "%", 1);
            break;
        }
        case 'p': {
            char buf[2 * sizeof(void*) + 4];
            snprintf(buf, sizeof(buf), "%p", va_arg(arg, void*));
            pushstr(vm, buf, strlen(buf));
            break;
        }
        default:
            pushstr(vm, "(unknown)", 8);
            break;
        }
        concat2(vm);
        format = p + 2;
    }
    pushstr(vm, format, strlen(format));
    return concat2(vm);
}

int be_char2hex(int c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 0x0A;
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 0x0A;
    }
    return -1;
}

/*******************************************************************
 * the function be_str2int():
 * >>-+------------+--+--+-----+----digits-------+----------------><
 *    '-whitespace-'  |  +- + -+                 |
 *                    |  '- - -'                 |
 *                    |                          |
 *                    +- 0x or 0X ---hex_digits--+
 * 
 *******************************************************************/
BERRY_API bint be_str2int(const char *str, const char **endstr)
{
    int c, sign;
    bint sum = 0;
    skip_space(str);
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        /* hex literal */
        str += 2;       /* skip 0x or 0X */
        while ((c = be_char2hex(*str++)) >= 0) {
            sum = sum * 16 + c;
        }
        if (endstr) {
            *endstr = str - 1;
        }
        return sum;
    } else {
        /* decimal literal */
        sign = c = *str++;
        if (c == '+' || c == '-') {
            c = *str++;
        }
        while (is_digit(c)) {
            sum = sum * 10 + c - '0';
            c = *str++;
        }
        if (endstr) {
            *endstr = str - 1;
        }
        return sign == '-' ? -sum : sum;
    }
}

/*******************************************************************
 * the function be_str2real():
 * >>-+------------+--+-----+--+-digits--+---+--+--------+-+------->
 *    '-whitespace-'  +- + -+  |         '-.-'  '-digits-' |   
 *                    '- - -'  '-.--digits-----------------'   
 * 
 * >--+------------------------+----------------------------------><
 *    '-+-e-+--+-----+--digits-'   
 *      '-E-'  +- + -+             
 *             '- - -'  
 *******************************************************************/
BERRY_API breal be_str2real(const char *str, const char **endstr)
{
    int c, sign;
    breal sum = 0, deci = 0, point = (breal)0.1;
    skip_space(str);
    sign = c = *str++;
    if (c == '+' || c == '-') {
        c = *str++;
    }
    while (is_digit(c)) {
        sum = sum * 10 + c - '0';
        c = *str++;
    }
    if (c == '.') {
        c = *str++;
        while (is_digit(c)) {
            deci = deci + ((breal)c - '0') * point;
            point *= (breal)0.1;
            c = *str++;
        }
    }
    sum = sum + deci;
    if (c == 'e' || c == 'E') {
        int e = 0;
        breal ratio = (c = *str++) == '-' ? (breal)0.1 : 10;
        if (c == '+' || c == '-') {
            c = *str++;
        }
        while (is_digit(c)) {
            e = e * 10 + c - '0';
            c = *str++;
        }
        while (e--) {
            sum *= ratio;
        }
    }
    if (endstr) {
        *endstr = str - 1;
    }
    return sign == '-' ? -sum : sum;
}

/* convert a string to a number (integer or real).
 * 1. skip \s*[\+\-]?\d*
 * 2. matched [.eE]? yes: real, no: integer.
 **/
BERRY_API const char *be_str2num(bvm *vm, const char *str)
{
    const char *sout; 
    bint c, vint = be_str2int(str, &sout);
    c = *sout;
    if (c == '.' || c == 'e' || c == 'E') {
        be_pushreal(vm, be_str2real(str, &sout));
    } else {
        be_pushint(vm, vint);
    }
    return sout;
}

static bstring* string_range(bvm *vm, bstring *str, binstance *range)
{
    bint lower, upper;
    bint size = str_len(str);   /* size of source string */
    /* get index range */
    bvalue temp;
    be_instance_member(vm, range, be_newstr(vm, "__lower__"), &temp);
    lower = var_toint(&temp);
    be_instance_member(vm, range, be_newstr(vm, "__upper__"), &temp);
    upper = var_toint(&temp);
    /* protection scope */
    if (upper < 0) { upper = size + upper; }
    if (lower < 0) { lower = size + lower; }
    upper = upper < size ? upper : size - 1;
    lower = lower < 0 ? 0 : lower;
    if (lower > upper) {
        return be_newstrn(vm, "", 0);   /* empty string */
    }
    return be_newstrn(vm, str(str) + lower, upper - lower + 1);

}

/* string subscript operation */
bstring* be_strindex(bvm *vm, bstring *str, bvalue *idx)
{
    if (var_isint(idx)) {
        int pos = var_toidx(idx);
        int size = str_len(str);
        if (pos < 0) { pos = size + pos; }
        if ((pos < size) && (pos >= 0)) {
            return be_newstrn(vm, str(str) + pos, 1);
        }
        be_raise(vm, "index_error", "string index out of range");
    } else if (var_isinstance(idx)) {
        binstance * ins = var_toobj(idx);
        const char *cname = str(be_instance_name(ins));
        if (!strcmp(cname, "range")) {
            return string_range(vm, str, ins);
        }
        // str(be_instance_name(i))
    }
    be_raise(vm, "index_error", "string indices must be integers");
    return NULL;
}

size_t be_strlcpy(char *dst, const char *src, size_t maxlen)
{
    const size_t srclen = strlen(src);
    if (srclen + 1 < maxlen) {
        memcpy(dst, src, srclen + 1);
    } else if (maxlen != 0) {
        memcpy(dst, src, maxlen - 1);
        dst[maxlen-1] = '\0';
    }
    return srclen;
}

const char* be_splitpath(const char *path)
{
    const char *p;
    for (p = path - 1; *path != '\0'; ++path) {
        if (*path == '/') {
            p = path;
        }
    }
    return p + 1; /* return the file name pointer */
}

const char* be_splitname(const char *path)
{
    const char *p, *q, *end = path + strlen(path);
    for (p = end; *p != '.' && p > path; --p); /* skip [^\.] */
    for (q = p; *q == '.' && q > path; --q); /* skip \. */
    if ((q == path && *q == '.') || *q == '/') {
        return end;
    }
    return p;
}

static unsigned escape_length(const char *s, int quote)
{
    unsigned c, len = 0, step = quote == '"' ? 5 : 3;
    for (; (c = *s) != '\0'; ++s) {
        switch (c) {
        case '\\': case '\n': case '\r': case '\t':
            len += 1;
            break;
        default:
            if (c < 0x20)
                len += step;
            else if (c == (unsigned)quote)
                len += 1;
            break;
        }
    }
    return len;
}

static unsigned eschex(unsigned num)
{
    return num <= 9 ? '0' + num : 'a' + num - 10;
}

/* escape as Berry or JSON */
static char* escape(char *q, unsigned c, int quote)
{
    int json = quote == '"';
    switch (c) {
    case '\\': *q++ = '\\'; *q = '\\'; break;
    case '\n': *q++ = '\\'; *q = 'n'; break;
    case '\r': *q++ = '\\'; *q = 'r'; break;
    case '\t': *q++ = '\\'; *q = 't'; break;
    default:
        if (c < 0x20) { /* other characters are escaped using '\uxxxx' */
            *q++ = '\\';
            if (json) {
                *q++ = 'u'; *q++ = '0'; *q++ = '0';
                *q++ = (char)eschex(c >> 4);
                *q = (char)eschex(c & 0x0f);
            } else {
                *q++ = 'x';
                *q++ = (char)eschex(c >> 4);
                *q = (char)eschex(c & 0x0f);
            }
        } else { /* quotes and unescaped characters */
            if (c == (unsigned)quote)
                *q++ = '\\';
            *q = (char)c;
        }
        break;
    }
    return q;
}

static void toescape(bvm *vm, int index, int quote)
{
    char *buf, *q;
    const char *p, *s = be_tostring(vm, index);
    size_t len = (size_t)be_strlen(vm, index);
    len += escape_length(s, quote) + 2; /* escape length + quote mark */
    buf = q = be_pushbuffer(vm, len);
    *q++ = (char)quote; /* add first quote */
    /* generate escape string */
    for (p = s; *p != '\0'; ++p, ++q) {
        q = escape(q, *p, quote);
    }
    *q = (char)quote; /* add last quote */
    be_pushnstring(vm, buf, len); /* make escape string from buffer */
    be_moveto(vm, -1, index);
    be_pop(vm, 2); /* remove buffer & top string */
}

BERRY_API const char* be_toescape(bvm *vm, int index, int mode)
{
    if (be_isstring(vm, index)) {
        index = be_absindex(vm, index);
        toescape(vm, index, mode == 'u' ? '"' : '\'');
    }
    return be_tostring(vm, index);
}

#if BE_USE_STRING_MODULE

#define MAX_FORMAT_MODE     32
#define FLAGES              "+- #0"

static const char* skip2dig(const char *s)
{
    if (is_digit(*s)) {
        ++s;
    }
    if (is_digit(*s)) {
        ++s;
    }
    return s;
}

static const char* get_mode(const char *str, char *buf, size_t buf_len)
{
    const char *p = str;
    while (*p && strchr(FLAGES, *p)) { /* skip flags */
        ++p;
    }
    p = skip2dig(p); /* skip width (2 digits at most) */
    if (*p == '.') {
        p = skip2dig(++p); /* skip width (2 digits at most) */
    }
    *(buf++) = '%';
    size_t mode_size = p - str + 1;
    /* Leave 2 bytes for the leading % and the trailing '\0' */
    if (mode_size > buf_len - 2) { 
        mode_size = buf_len - 2;
    }
    strncpy(buf, str, mode_size);
    buf[mode_size] = '\0';
    return p;
}

static void mode_fixlen(char *mode, const char *lenmode)
{
    size_t l = strlen(mode), lm = strlen(lenmode);
    char spec = mode[l - 1];
    strcpy(mode + l - 1, lenmode);
    mode[l + lm - 1] = spec;
    mode[l + lm] = '\0';
}

static bbool convert_to_int(bvm *vm, int index, bint *val)
{
    bbool converted = bfalse;
    if (val == NULL) { return bfalse; }
    if (be_isint(vm, index)) {
        *val = be_toint(vm, index);
        converted = btrue;
    } else {
        be_pushntvfunction(vm, be_baselib_int);
        be_pushvalue(vm, index);
        be_call(vm, 1);
        be_pop(vm, 1);
        if (be_isint(vm, -1)) {
            *val = be_toint(vm, -1);
            converted = btrue;
        }
        be_pop(vm, 1);
    }
    return converted;
}

static bbool convert_to_real(bvm *vm, int index, breal *val)
{
    bbool converted = bfalse;
    if (val == NULL) { return bfalse; }
    if (be_isnumber(vm, index)) {
        *val = be_toreal(vm, index);
        converted = btrue;
    } else {
        be_pushntvfunction(vm, be_baselib_real);
        be_pushvalue(vm, index);
        be_call(vm, 1);
        be_pop(vm, 1);
        if (be_isnumber(vm, -1)) {
            *val = be_toreal(vm, -1);
            converted = btrue;
        }
        be_pop(vm, 1);
    }
    return converted;
}

int be_str_format(bvm *vm)
{
    int top = be_top(vm);
    if (top > 0 && be_isstring(vm, 1)) {
        int index = 2;
        const char *format = be_tostring(vm, 1);
        pushstr(vm, "", 0);
        for (;;) {
            char mode[MAX_FORMAT_MODE];
            char buf[128];
            const char *p = strchr(format, '%');
            if (p == NULL) {
                break;
            }
            pushstr(vm, format, p - format);
            concat2(vm);
            p = get_mode(p + 1, mode, sizeof(mode));
            buf[0] = '\0';
            if (index > top && *p != '%') {
                be_raise(vm, "runtime_error", be_pushfstring(vm,
                    "bad argument #%d to 'format': no value", index));
            }
            switch (*p) {
            case '%':
                be_pushstring(vm, "%");
                --index;  /* compensate the future ++index */
                break;
            case 'd': case 'i': case 'o':
            case 'u': case 'x': case 'X':
            {
                bint val;
                if (convert_to_int(vm, index, &val)) {
                    mode_fixlen(mode, BE_INT_FMTLEN);
                    snprintf(buf, sizeof(buf), mode, val);
                }
                be_pushstring(vm, buf);
                break;
            }
            case 'e': case 'E':
            case 'f': case 'g': case 'G':
            {
                breal val;
                if (convert_to_real(vm, index, &val)) {
                    snprintf(buf, sizeof(buf), mode, val);
                }
                be_pushstring(vm, buf);
                break;
            }
            case 'c':
            {
                bint val;
                if (convert_to_int(vm, index, &val)) {
                    snprintf(buf, sizeof(buf), "%c", (int)val);
                }
                be_pushstring(vm, buf);
                break;
            }
            case 's': {
                const char *s = be_tostring(vm, index);
                int len = be_strlen(vm, index);
                if (len > 100 && strlen(mode) == 2) {
                    be_pushvalue(vm, index);
                } else {
                    snprintf(buf, sizeof(buf), mode, s);
                    be_pushstring(vm, buf);
                }
                break;
            }
            case 'q': {
                const char *s = be_toescape(vm, index, 'q');
                int len = be_strlen(vm, index);
                if (len > 100 && strlen(mode) == 2) {
                    be_pushvalue(vm, index);
                } else {
                    snprintf(buf, sizeof(buf), "%s", s);
                    be_pushstring(vm, buf);
                }
                break;
            }
            default: /* error */
                be_raise(vm, "runtime_error", be_pushfstring(vm,
                    "invalid option '%%%c' to 'format'", *p));
                break;
            }
            concat2(vm);
            format = p + 1;
            ++index;
        }
        pushstr(vm, format, strlen(format));
        concat2(vm);
        be_return(vm);
    }
    be_return_nil(vm);
}

/* string.op(s1, s2, begin=0, end=length(s2)) */
static bint str_operation(bvm *vm, str_opfunc func, bint error)
{
    int top = be_top(vm);
    /* check the number and type of arguments */
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        /* get the operation string and its length */
        int len1 = be_strlen(vm, 1);
        int len2 = be_strlen(vm, 2);
        const char *s1 = be_tostring(vm, 1);
        const char *s2 = be_tostring(vm, 2);
        /* get begin and end indexes (may use default values) */
        bint begin = top >= 3 && be_isint(vm, 3) ? be_toint(vm, 3) : 0;
        bint end = top >= 4 && be_isint(vm, 4) ? be_toint(vm, 4) : len1;
        /* basic range check:
         * 1. begin position must be greater than 0 and
         *    less than the length of the source string.
         * 2. the length of the pattern string cannot be
         *    less than the matching range (end - begin).
         **/
        if (begin >= 0 && begin <= len1 && end - begin >= len2) {
            /* call the operation function */
            return func(s1, s2, begin, end - len2);
        }
    }
    return error; /* returns the default error value */
}

static bint _sfind(const char *s1, const char *s2, bint begin, bint end)
{
    const char *res = strstr(s1 + begin, s2);
    if (res) {
        bint pos = (bint)(res - s1);
        return pos <= end ? pos : -1;
    }
    return -1;
}

static int str_find(bvm *vm)
{
    be_pushint(vm, str_operation(vm, _sfind, -1));
    be_return(vm);
}

static bint _scount(const char *s1, const char *s2, bint begin, bint end)
{
    bint count = 0;
    const char *res = s1 + begin, *send = s1 + end;
    while ((res = strstr(res, s2)) != NULL && res <= send) {
        count += 1;
        res += 1;
    }
    return count;
}

static int str_count(bvm *vm)
{
    be_pushint(vm, str_operation(vm, _scount, 0));
    be_return(vm);
}

static bbool _split_string(bvm *vm, int top)
{
    if (be_isstring(vm, 2)) {
        const char *res;
        int len1 = be_strlen(vm, 1);
        int len2 = be_strlen(vm, 2);
        const char *s1 = be_tostring(vm, 1);
        const char *s2 = be_tostring(vm, 2);
        bint count = len2 /* match when the pattern string is not empty */
            ? top >= 3 && be_isint(vm, 3) ? be_toint(vm, 3) : len1
            : 0; /* cannot match empty pattern string */
        while (count-- && (res = strstr(s1, s2)) != NULL) {
            be_pushnstring(vm, s1, res - s1);
            be_data_push(vm, -2);
            be_pop(vm, 1);
            s1 = res + len2;
        }
        be_pushstring(vm, s1);
        be_data_push(vm, -2);
        be_pop(vm, 1);
        return btrue;
    }
    return bfalse;
}

static bbool _split_index(bvm *vm)
{
    if (be_isint(vm, 2)) {
        int len = be_strlen(vm, 1), idx = be_toindex(vm, 2);
        const char *s = be_tostring(vm, 1);
        idx = idx > len ? len : idx < -len ? -len : idx;
        if (idx < 0) {
            idx += len;
        }
        be_pushnstring(vm, s, idx);
        be_data_push(vm, -2);
        be_pop(vm, 1);
        be_pushnstring(vm, s + idx, (size_t)len - idx);
        be_data_push(vm, -2);
        be_pop(vm, 1);
        return btrue;
    }
    return bfalse;
}

static int str_split(bvm *vm)
{
    int top = be_top(vm);
    be_newobject(vm, "list");
    if (top >= 2 && be_isstring(vm, 1)) {
        if (!_split_index(vm))
            _split_string(vm, top);
    }
    be_pop(vm, 1);
    be_return(vm);
}

static int str_i2hex(bvm *vm)
{
    int top = be_top(vm);
    if (top && be_isint(vm, 1)) {
        bint value = be_toint(vm, 1);
        char fmt[10] = { "%" BE_INT_FMTLEN "X" }, buf[18];
        if (top >= 2 && be_isint(vm, 2)) {
            bint num = be_toint(vm, 2);
            if (num > 0 && num <= 16) {
                snprintf(fmt, sizeof(fmt), "%%.%d" BE_INT_FMTLEN "X", (int)num);
            }
        }
        snprintf(buf, sizeof(buf), fmt, value);
        be_pushstring(vm, buf);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_byte(bvm *vm)
{
    if (be_top(vm) && be_isstring(vm, 1)) {
        const bbyte *s = (const bbyte *)be_tostring(vm, 1);
        be_pushint(vm, *s);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_char(bvm *vm)
{
    if (be_top(vm) && be_isint(vm, 1)) {
        char c = be_toint(vm, 1) & 0xFF;
        be_pushnstring(vm, &c, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

// boolean to select whether we call toupper() or tolower()
static int str_touplower(bvm *vm, bbool up)
{
    if (be_top(vm) && be_isstring(vm, 1)) {
        const char *p, *s = be_tostring(vm, 1);
        size_t len = (size_t)be_strlen(vm, 1);
        char *buf, *q;
        buf = q = be_pushbuffer(vm, len);
        /* convert to lower case */
        for (p = s; *p != '\0'; ++p, ++q) {
            *q = up ? toupper(*p) : tolower(*p);
        }
        be_pushnstring(vm, buf, len); /* make escape string from buffer */
        be_remove(vm, 2); /* remove buffer */
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_tolower(bvm *vm) {
    return str_touplower(vm, bfalse);
}

static int str_toupper(bvm *vm) {
    return str_touplower(vm, btrue);
}

static int str_tr(bvm *vm)
{
    if (be_top(vm) == 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isstring(vm, 3)) {
        const char *p, *s = be_tostring(vm, 1);
        const char *t1 = be_tostring(vm, 2);
        const char *t2 = be_tostring(vm, 3);
        size_t len = (size_t)be_strlen(vm, 1);
        char *buf, *q;
        buf = be_pushbuffer(vm, len);
        /* convert each char */
        for (p = s, q = buf; *p != '\0'; ++p, ++q) {
            const char *p1, *p2;
            *q = *p;  /* default to no change */
            for (p1=t1, p2=t2; *p1 != '\0'; ++p1) {
                if (*p == *p1) {
                    if (*p2) {
                        *q = *p2;
                    } else {
                        q--;    /* remove this char */
                        len--;
                    }
                    break;
                }
                if (*p2) { p2++; }
            }
        }
        be_pushnstring(vm, buf, len); /* make escape string from buffer */
        be_remove(vm, 2); /* remove buffer */
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_replace(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 3 && be_isstring(vm, 1) && be_isstring(vm, 2) && be_isstring(vm, 3)) {
        be_pushntvfunction(vm, &str_split);
        be_pushvalue(vm, 1);
        be_pushvalue(vm, 2);
        be_call(vm, 2);
        be_pop(vm, 2);

        be_getmember(vm, -1, "concat");     /* get `concat` method of list */
        be_pushvalue(vm, -2);               /* get list instance as first arg */
        be_pushvalue(vm, 3);
        be_call(vm, 2);
        be_pop(vm, 2);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_escape(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 1 && be_isstring(vm, 1)) {
        int quote = 'u';
        if (top >= 2 && be_isbool(vm, 2)) {
            if (be_tobool(vm, 1)) {
                quote = 'x';
            }
        }
        be_tostring(vm, 1);
        be_toescape(vm, 1, quote);
        be_pushvalue(vm, 1);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_startswith(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        bbool case_insensitive = bfalse;
        if (top >= 3 && be_isbool(vm, 3)) {
            case_insensitive = be_tobool(vm, 3);
        }
        bbool result = bfalse;
        const char *s = be_tostring(vm, 1);
        const char *p = be_tostring(vm, 2);
        size_t len = (size_t)be_strlen(vm, 2);
        if (case_insensitive) {
            if (str_strncasecmp(s, p, len) == 0) {
                result = btrue;
            }
        } else {
            if (strncmp(s, p, len) == 0) {
                result = btrue;
            }
        }
        be_pushbool(vm, result);
        be_return(vm);
    }
    be_return_nil(vm);
}

static int str_endswith(bvm *vm)
{
    int top = be_top(vm);
    if (top >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
        bbool case_insensitive = bfalse;
        if (top >= 3 && be_isbool(vm, 3)) {
            case_insensitive = be_tobool(vm, 3);
        }
        bbool result = bfalse;
        const char *s = be_tostring(vm, 1);
        const char *p = be_tostring(vm, 2);
        size_t len = (size_t)be_strlen(vm, 2);
        if (case_insensitive) {
            if (str_strncasecmp(s + (int)strlen(s) - (int)len, p, len) == 0) {
                result = btrue;
            }
        } else {
            if (strncmp(s + (int)strlen(s) - (int)len, p, len) == 0) {
                result = btrue;
            }
        }
        be_pushbool(vm, result);
        be_return(vm);
    }
    be_return_nil(vm);
}

#if !BE_USE_PRECOMPILED_OBJECT
be_native_module_attr_table(string) {
    be_native_module_function("format", be_str_format),
    be_native_module_function("count", str_count),
    be_native_module_function("split", str_split),
    be_native_module_function("find", str_find),
    be_native_module_function("hex", str_i2hex),
    be_native_module_function("byte", str_byte),
    be_native_module_function("char", str_char),
    be_native_module_function("tolower", str_tolower),
    be_native_module_function("toupper", str_toupper),
    be_native_module_function("tr", str_tr),
    be_native_module_function("escape", str_escape),
    be_native_module_function("replace", str_replace),
    be_native_module_function("startswith", str_startswith),
    be_native_module_function("endswith", str_endswith),
};

be_define_native_module(string, NULL);
#else
/* @const_object_info_begin
module string (scope: global, depend: BE_USE_STRING_MODULE) {
    format, func(be_str_format)
    count, func(str_count)
    split, func(str_split)
    find, func(str_find)
    hex, func(str_i2hex)
    byte, func(str_byte)
    char, func(str_char)
    tolower, func(str_tolower)
    toupper, func(str_toupper)
    tr, func(str_tr)
    escape, func(str_escape)
    replace, func(str_replace)
    startswith, func(str_startswith)
    endswith, func(str_endswith)
}
@const_object_info_end */
#include "../generate/be_fixed_string.h"
#endif

#endif /* BE_USE_STRING_MODULE */
