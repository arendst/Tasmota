/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BE_OBJECT_H
#define BE_OBJECT_H

#include "berry.h"

/* basic types, do not change value */
#define BE_NONE        -256     /* unknown type */
#define BE_NIL          0
#define BE_INT          1
#define BE_REAL         2
#define BE_BOOL         3
#define BE_COMPTR       4       /* common pointer */
#define BE_INDEX        5       /* index for instance variable, previously BE_INT */
#define BE_FUNCTION     6

#define BE_GCOBJECT     16      /* from this type can be gced */
#define BE_GCOBJECT_MAX (3<<5)  /* 96 - from this type can't be gced */

#define BE_STRING       16
#define BE_CLASS        17
#define BE_INSTANCE     18
#define BE_PROTO        19
#define BE_LIST         20
#define BE_MAP          21
#define BE_MODULE       22
#define BE_COMOBJ       23      /* common object */

#define BE_NTVFUNC      ((0 << 5) | BE_FUNCTION)    /* 6 - cannot be gced */
#define BE_CLOSURE      ((1 << 5) | BE_FUNCTION)    /* 38 - can be gced */
#define BE_NTVCLOS      ((2 << 5) | BE_FUNCTION)    /* 70 - can be gced*/
#define BE_CTYPE_FUNC   ((3 << 5) | BE_FUNCTION)    /* 102 - cannot be gced */
#define BE_STATIC       (1 << 7)                    /* 128 */

/* values for bproto.varg */
#define BE_VA_VARARG            (1 << 0)    /* function has variable number of arguments */
#define BE_VA_METHOD            (1 << 1)    /* function is a method (this is only a hint) */
#define BE_VA_STATICMETHOD      (1 << 2)    /* the function is a static method and has the class as implicit '_class' variable */
#define BE_VA_SHARED_KTAB       (1 << 3)    /* the funciton has a shared consolidated ktab */
#define BE_VA_NOCOMPACT         (1 << 4)    /* the funciton has a shared consolidated ktab */
#define array_count(a)   (sizeof(a) / sizeof((a)[0]))

#define bcommon_header          \
    struct bgcobject *next;     \
    bbyte type;                 \
    bbyte marked

#define bstring_header          \
    bcommon_header;             \
    bbyte extra;                \
    bbyte slen

typedef struct bgcobject {
    bcommon_header;
} bgcobject;

typedef struct bclosure bclosure;
typedef struct bntvclos bntvclos;
typedef struct bclass bclass;
typedef struct binstance binstance;
typedef struct binstance_arg3 binstance_arg3;
typedef struct blist blist;
typedef struct bmap bmap;
typedef struct bupval bupval;

typedef uint32_t binstruction;

typedef struct bstring {
    bstring_header;
} bstring;

/* the definition of the vector and stack data structures.
 * in fact, the stack is implemented by vector. */
typedef struct bvector {
    int capacity;   /* the number of elements that the vector can store */
    int size;       /* the size of each element (bytes) */
    int count;      /* number of elements of the vector */
    void *data;     /* the data block pointer, if vector is empty,
                       it will point to the first element */
    void *end;      /* pointer to the last element, if the vector is empty,
                       the end pointer will be smaller than the data pointer */
} bvector, bstack;

/* berry value data union, a berry value is always described
 * by the data structure contained in the bvaldata union. */
union bvaldata {
    bbool b;        /* boolean */
    breal r;        /* real number */
    bint i;         /* integer number */
    void *p;        /* object pointer */
    const void *c;  /* const object pointer */
    bstring *s;     /* string pointer */
    bgcobject *gc;  /* GC object */
    bntvfunc nf;    /* native C function */
#ifdef __cplusplus
    BE_CONSTEXPR bvaldata(bbool v) : b(v) {}
    BE_CONSTEXPR bvaldata(breal v) : r(v) {}
    BE_CONSTEXPR bvaldata(bint v) : i(v) {}
    BE_CONSTEXPR bvaldata(void *v) : p(v) {}
    BE_CONSTEXPR bvaldata(const void *v) : c(v) {}
    BE_CONSTEXPR bvaldata(bntvfunc v) : nf(v) {}
#endif
};

/* berry value. for simple types, the value of the data is stored,
 * while the complex type stores a reference to the data. */
typedef struct bvalue {
    union bvaldata v; /* the value data */
    int type;         /* the value type */
} bvalue;

typedef struct {
#if BE_DEBUG_VAR_INFO
    bstring *name;
#endif
    bbyte instack;
    bbyte idx;
} bupvaldesc;

typedef struct {
#if BE_DEBUG_RUNTIME_INFO > 1
    uint16_t linenumber;
    uint16_t endpc;
#else
    int linenumber;
    int endpc;
#endif
} blineinfo;

typedef struct {
    bstring *name;
#if BE_DEBUG_RUNTIME_INFO > 1
    uint16_t beginpc;
    uint16_t endpc;
#else
    int beginpc;
    int endpc;
#endif
} bvarinfo;

typedef struct bproto {
    bcommon_header;
    bbyte nstack; /* number of stack size by this function */
    bbyte nupvals; /* upvalue count */
    bbyte argc; /* argument count */
    bbyte varg; /* variable argument position + 1 */
    int16_t codesize; /* code size */
    int16_t nconst; /* constants count */
    int16_t nproto; /* proto count */
    bgcobject *gray; /* for gc gray list */
    bupvaldesc *upvals;
    bvalue *ktab; /* constants table */
    struct bproto **ptab; /* proto table */
    binstruction *code; /* instructions sequence */
    bstring *name; /* function name */
#if BE_DEBUG_SOURCE_FILE
    bstring *source; /* source file name */
#endif
#if BE_DEBUG_RUNTIME_INFO /* debug information */
    blineinfo *lineinfo;
    int nlineinfo;
#endif
#if BE_DEBUG_VAR_INFO
    bvarinfo *varinfo;
    int nvarinfo;
#endif
} bproto;

/* berry closure */
struct bclosure {
    bcommon_header;
    bbyte nupvals;
    bgcobject *gray; /* for gc gray list */
    bproto *proto;
    bupval *upvals[1];
};

/* C native function or closure */
struct bntvclos {
    bcommon_header;
    bbyte nupvals;
    bgcobject *gray; /* for gc gray list */
    bntvfunc f;
};

/* common object */
typedef struct {
    bcommon_header;
    void *data;
    bntvfunc destroy;
} bcommomobj;

struct blexer;
typedef const char* (*breader)(struct blexer*, void*, size_t*);

#define cast(_T, _v)            ((_T)(_v))
#define cast_int(_v)            cast(int, _v)
#define cast_bool(_v)           cast(bbool, _v)
#define basetype(_t)            ((_t) & 0x1F)

#define var_type(_v)            ((_v)->type)
#define var_basetype(_v)        basetype((_v)->type)
#define var_primetype(_v)       (var_type(_v) & ~BE_STATIC)
#define var_isstatic(_v)        ((var_type(_v) & BE_STATIC) == BE_STATIC)
#define var_istype(_v, _t)      (var_primetype(_v) == _t)
#define var_settype(_v, _t)     ((_v)->type = _t)
#define var_markstatic(_v)      var_settype(_v, var_type(_v) | BE_STATIC)
#define var_clearstatic(_v)     var_settype(_v, var_type(_v) & ~BE_STATIC)
#define var_setobj(_v, _t, _o)  { (_v)->v.p = _o; var_settype(_v, _t); }

#define var_isnil(_v)           var_istype(_v, BE_NIL)
#define var_isbool(_v)          var_istype(_v, BE_BOOL)
#define var_isint(_v)           var_istype(_v, BE_INT)
#define var_isreal(_v)          var_istype(_v, BE_REAL)
#define var_isstr(_v)           var_istype(_v, BE_STRING)
#define var_isclosure(_v)       var_istype(_v, BE_CLOSURE)
#define var_isntvclos(_v)       var_istype(_v, BE_NTVCLOS)
#define var_isntvfunc(_v)       var_istype(_v, BE_NTVFUNC)
#define var_isfunction(_v)      (var_basetype(_v) == BE_FUNCTION)
#define var_isproto(_v)         var_istype(_v, BE_PROTO)
#define var_isclass(_v)         var_istype(_v, BE_CLASS)
#define var_isinstance(_v)      var_istype(_v, BE_INSTANCE)
#define var_islist(_v)          var_istype(_v, BE_LIST)
#define var_ismap(_v)           var_istype(_v, BE_MAP)
#define var_ismodule(_v)        var_istype(_v, BE_MODULE)
#define var_isindex(_v)         var_istype(_v, BE_INDEX)
#define var_iscomptr(_v)        var_istype(_v, BE_COMPTR)
#define var_isnumber(_v)        (var_isint(_v) || var_isreal(_v))

#define var_setnil(_v)          var_settype(_v, BE_NIL)
#define var_setval(_v, _s)      (*(_v) = *(_s))
#define var_setbool(_v, _b)     { var_settype(_v, BE_BOOL); (_v)->v.b = (bbool)(_b); }
#define var_setint(_v, _i)      { var_settype(_v, BE_INT); (_v)->v.i = (_i); }
#define var_setreal(_v, _r)     { var_settype(_v, BE_REAL); (_v)->v.r = (_r); }
#define var_setstr(_v, _s)      var_setobj(_v, BE_STRING, _s)
#define var_setinstance(_v, _o) var_setobj(_v, BE_INSTANCE, _o)
#define var_setclass(_v, _o)    var_setobj(_v, BE_CLASS, _o)
#define var_setclosure(_v, _o)  var_setobj(_v, BE_CLOSURE, _o)
#define var_setntvclos(_v, _o)  var_setobj(_v, BE_NTVCLOS, _o)
#define var_setntvfunc(_v, _o)  { (_v)->v.nf = (_o); var_settype(_v, BE_NTVFUNC); }
#define var_setlist(_v, _o)     var_setobj(_v, BE_LIST, _o)
#define var_setmap(_v, _o)      var_setobj(_v, BE_MAP, _o)
#define var_setmodule(_v, _o)   var_setobj(_v, BE_MODULE, _o)
#define var_setindex(_v, _i)    { var_settype(_v, BE_INDEX); (_v)->v.i = (_i); }
#define var_setproto(_v, _o)    var_setobj(_v, BE_PROTO, _o)

#define var_tobool(_v)          ((_v)->v.b)
#define var_toint(_v)           ((_v)->v.i)
#define var_toreal(_v)          ((_v)->v.r)
#define var_tostr(_v)           ((_v)->v.s)
#define var_togc(_v)            ((_v)->v.gc)
#define var_toobj(_v)           ((_v)->v.p)
#define var_tontvfunc(_v)       ((_v)->v.nf)
#define var_toidx(_v)           cast_int(var_toint(_v))

const char* be_vtype2str(bvalue *v);
bvalue* be_indexof(bvm *vm, int idx);
void be_commonobj_delete(bvm *vm, bgcobject *obj);
int be_commonobj_destroy_generic(bvm* vm);

#endif
