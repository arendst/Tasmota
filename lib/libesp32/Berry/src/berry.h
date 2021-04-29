/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BERRY_H
#define BERRY_H

#include <stddef.h>
#include <stdint.h>

#include "berry_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/* do not modify the version number! */
#define BERRY_VERSION           "0.1.10"

#if BE_STACK_TOTAL_MAX < BE_STACK_FREE_MIN * 2
#error "The value of the macro BE_STACK_TOTAL_MAX is too small."
#endif

/* basic type definition */
#if BE_INTGER_TYPE == 0
  #define BE_INTEGER            int
  #define BE_INT_FMTLEN         ""
#elif BE_INTGER_TYPE == 1
  #define BE_INTEGER            long
  #define BE_INT_FMTLEN         "l"
#elif BE_INTGER_TYPE == 2
  #ifdef _WIN32
    #define BE_INTEGER          __int64
    #define BE_INT_FMTLEN       "I64"
  #else
    #define BE_INTEGER          long long
    #define BE_INT_FMTLEN       "ll"
  #endif
#else
  #error "Unsupported integer type for `bint`."
#endif
#define BE_INT_FORMAT           "%" BE_INT_FMTLEN "d"

typedef uint8_t                 bbyte;
typedef BE_INTEGER              bint;

#if BE_USE_SINGLE_FLOAT != 0
  typedef float                 breal;
#else
  typedef double                breal;
#endif

/* boolean values definition */
#ifndef __cplusplus
  #define bbool                 _Bool
  #define bfalse                0
  #define btrue                 1
#else
  #define bbool                 bool
  #define bfalse                false
  #define btrue                 true
#endif

/* error code definition */
enum berrorcode {
    BE_OK = 0,
    BE_EXIT,
    BE_MALLOC_FAIL,
    BE_EXCEPTION,
    BE_SYNTAX_ERROR,
    BE_EXEC_ERROR,
    BE_IO_ERROR
};

/* native-module member type specifier */
#define BE_CNIL                 0
#define BE_CINT                 1
#define BE_CREAL                2
#define BE_CBOOL                3
#define BE_CFUNCTION            4
#define BE_CSTRING              5
#define BE_CMODULE              6

/* API function mark */
#if defined(_WIN32) || defined(__CYGWIN__) /* in Windows */
  #if defined(BERRY_MODULE) /* berry extension module */
    #define BERRY_API           __declspec(dllimport)
  #else /* berry core */
    #define BERRY_API           __declspec(dllexport)
  #endif
#else /* other platforms */
  #define BERRY_API             extern
#endif

/* only linux */
#if defined(_WIN32) || defined(__CYGWIN__) /* in Windows */
  #define BERRY_LOCAL
#elif defined(__GNUC__) /* in GCC */
  #define BERRY_LOCAL           __attribute__ ((visibility ("hidden")))
#else /* other platforms */
  #define BERRY_LOCAL
#endif

#ifdef __cplusplus
#ifdef __cpp_constexpr
  #define BE_CONSTEXPR          constexpr
#else
  #define BE_CONSTEXPR
#endif
#endif

#ifdef __cplusplus
#define BE_EXPORT_VARIABLE      extern "C"
#else
#define BE_EXPORT_VARIABLE
#endif

typedef struct bvm bvm;        /* virtual machine structure */
typedef int (*bntvfunc)(bvm*); /* native function pointer */
struct bclass;

/* native function information */
typedef struct {
    const char *name;
    bntvfunc function;
} bnfuncinfo;

/* native module object node */
typedef struct bntvmodobj {
    const char *name;
    int type;
    union value {
        bint i;
        breal r;
        bbool b;
        bntvfunc f;
        const char *s;
        const void *o;
#ifdef __cplusplus
        BE_CONSTEXPR value(bint v) : i(v) {}
        BE_CONSTEXPR value(breal v) : r(v) {}
        BE_CONSTEXPR value(bbool v) : b(v) {}
        BE_CONSTEXPR value(bntvfunc v) : f(v) {}
        BE_CONSTEXPR value(const char *v) : s(v) {}
        BE_CONSTEXPR value(const void *v) : o(v) {}
#endif
    } u;
#ifdef __cplusplus
    BE_CONSTEXPR bntvmodobj(const char *name) :
        name(name), type(BE_CNIL), u(bint(0)) {}
    BE_CONSTEXPR bntvmodobj(const char *name, bint v) :
        name(name), type(BE_CINT), u(v) {}
    BE_CONSTEXPR bntvmodobj(const char *name, breal v) :
        name(name), type(BE_CREAL), u(v) {}
    BE_CONSTEXPR bntvmodobj(const char *name, bbool v) :
        name(name), type(BE_CBOOL), u(v) {}
    BE_CONSTEXPR bntvmodobj(const char *name, bntvfunc v) :
        name(name), type(BE_CFUNCTION), u(v) {}
    BE_CONSTEXPR bntvmodobj(const char *name, const char *v) :
        name(name), type(BE_CSTRING), u(v) {}
    BE_CONSTEXPR bntvmodobj(const char *name, int _tpye, const void *v) :
        name(name), type(_tpye), u(v) {}
#endif
} bntvmodobj;

/* native module object */
typedef struct bntvmodule {
    const char *name; /* native module name */
    const bntvmodobj *attrs; /* native module attributes */
    size_t size; /* native module attribute count */
    const struct bmodule *module; /* const module object */
    bntvfunc init; /* initialization function */
} bntvmodule;

/* native module node definition macro */
#ifndef __cplusplus
#define be_native_module_nil(_name)                     \
    { .name = (_name), .type = BE_CNIL, .u.i = 0 }

#define be_native_module_int(_name, _v)                 \
    { .name = (_name), .type = BE_CINT, .u.i = (bint)(_v) }

#define be_native_module_real(_name, _v)                \
    { .name = (_name), .type = BE_CREAL, .u.r = (breal)(_v) }

#define be_native_module_bool(_name, _b)                \
    { .name = (_name), .type = BE_CBOOL, .u.b = (bbool)(_b) }

#define be_native_module_function(_name, _f)            \
    { .name = (_name), .type = BE_CFUNCTION, .u.f = (_f) }

#define be_native_module_str(_name, _s)                 \
    { .name = (_name), .type = BE_CSTRING, .u.s = (_s) }

#define be_native_module_module(_name, _m)              \
    { .name = (_name), .type = BE_CMODULE, .u.o = &(_m) }
#else
#define be_native_module_nil(_name)                     \
    bntvmodobj(_name)

#define be_native_module_int(_name, _v)                 \
    bntvmodobj(_name, bint(_v))

#define be_native_module_real(_name, _v)                \
    bntvmodobj(_name, breal(_v))

#define be_native_module_bool(_name, _b)                \
    bntvmodobj(_name, bbool(_b))

#define be_native_module_function(_name, _f)            \
    bntvmodobj(_name, _f)

#define be_native_module_str(_name, _s)                 \
    bntvmodobj(_name, _s)

#define be_native_module_module(_name, _m)              \
    bntvmodobj(_name, BE_CMODULE, &(_m))
#endif

#define be_native_module_attr_table(name)               \
    static const bntvmodobj name##_attrs[] =

#define be_native_module(name)  be_native_module_##name

/* native module declaration macro */
#define be_extern_native_module(name)                   \
    extern const bntvmodule be_native_module(name)

/* native module definition macro */
#ifndef __cplusplus
#define be_define_native_module(_name, _init)           \
    const bntvmodule be_native_module(_name) = {        \
        .name = #_name,                                 \
        .attrs = _name##_attrs,                         \
        .size = sizeof(_name##_attrs)                   \
               / sizeof(_name##_attrs[0]),              \
        .module = NULL,                                 \
        .init = _init                                   \
    }
#else
#define be_define_native_module(_name, _init)           \
    const bntvmodule be_native_module(_name) = {        \
        #_name, _name##_attrs,                          \
        sizeof(_name##_attrs)                           \
            / sizeof(_name##_attrs[0]),                 \
        0, _init                                        \
    }
#endif

/* support for solidified berry functions */
/* native const strings outside of global string hash */
#define be_define_local_const_str(_name, _s, _hash, _len) \
    static const bcstring be_local_const_str_##_name = {  \
        .next = (bgcobject *)NULL,                        \
        .type = BE_STRING,                                \
        .marked = GC_CONST,                               \
        .extra = 0,                                       \
        .slen = _len,                                     \
        .hash = 0,                                        \
        .s = _s                                           \
    }

#define be_local_const_str(_name) (bstring*) &be_local_const_str_##_name

/* conditional macro see  https://stackoverflow.com/questions/11632219/c-preprocessor-macro-specialisation-based-on-an-argument */
#define BE_IIF(cond) BE_IIF_ ## cond
#define BE_IIF_0(t, f) f
#define BE_IIF_1(t, f) t

#if BE_DEBUG_VAR_INFO
  #define be_local_const_upval(ins, idx) { "", ins, idx }
#else
  #define be_local_const_upval(ins, idx) { ins, idx }
#endif

/* conditional block in bproto depending on compilation options */
#if BE_DEBUG_RUNTIME_INFO
  #define PROTO_RUNTIME_BLOCK \
    NULL,     /* varinfo */   \
    0,        /* nvarinfo */
#else
  #define PROTO_RUNTIME_BLOCK
#endif
#if BE_DEBUG_VAR_INFO
  #define PROTO_VAR_INFO_BLOCK\
    NULL,     /* varinfo */   \
    0,        /* nvarinfo */
#else
  #define PROTO_VAR_INFO_BLOCK
#endif

/* define bproto */
#define be_define_local_proto(_name, _nstack, _argc, _is_const, _is_subproto, _is_upval)     \
  static const bproto _name##_proto = {                                           \
    NULL,                       /* bgcobject *next */                             \
    BE_PROTO,                   /* type BE_PROTO */                               \
    0x08,                       /* marked outside of GC */                        \
    (_nstack),                  /* nstack */                                      \
    BE_IIF(_is_upval)(sizeof(_name##_upvals)/sizeof(bupvaldesc),0),/* nupvals */  \
    (_argc),                    /* argc */                                        \
    0,                          /* varg */                                        \
    NULL,                       /* bgcobject *gray */                             \
    BE_IIF(_is_upval)((bupvaldesc*)&_name##_upvals,NULL), /* bupvaldesc *upvals */\
    BE_IIF(_is_const)((bvalue*)&_name##_ktab,NULL), /* ktab */                    \
    BE_IIF(_is_subproto)((struct bproto**)&_name##_subproto,NULL),/* bproto **ptab */\
    (binstruction*) &_name##_code,     /* code */                                 \
    be_local_const_str(_name##_str_name),   /* name */                            \
    sizeof(_name##_code)/sizeof(uint32_t),  /* codesize */                        \
    BE_IIF(_is_const)(sizeof(_name##_ktab)/sizeof(bvalue),0),/* nconst */         \
    BE_IIF(_is_subproto)(sizeof(_name##_subproto)/sizeof(bproto*),0),/* proto */  \
    be_local_const_str(_name##_str_source),    /* source */                       \
    PROTO_RUNTIME_BLOCK                                                           \
    PROTO_VAR_INFO_BLOCK                                                          \
  }

#define be_define_local_closure(_name)        \
  const bclosure _name##_closure = {          \
    NULL,           /* bgcobject *next */     \
    BE_CLOSURE,     /* type BE_CLOSURE */     \
    GC_CONST,       /* marked GC_CONST */     \
    0,              /* nupvals */             \
    NULL,           /* bgcobject *gray */     \
    (bproto*) &_name##_proto, /* proto */     \
    { NULL }        /* upvals */              \
  }


/* debug hook typedefs */
#define BE_HOOK_LINE    1
#define BE_HOOK_CALL    2
#define BE_HOOK_RET     4
#define BE_HOOK_EXCEPT  8

typedef struct bhookinfo {
    int type; /* current hook type */
    int line; /* current linenumber */
    const char *source; /* source path information */
    const char *func_name; /* current function name */
    void *data; /* user extended data */
} bhookinfo;

typedef void(*bntvhook)(bvm *vm, bhookinfo *info);

/* the default assert definition */
#if !BE_DEBUG
  #if defined(be_assert)
    #undef be_assert
  #endif
  #define be_assert(expr)       ((void)0)
#endif

/* Observability hook */

typedef void(*bobshook)(bvm *vm, int event, ...);
enum beobshookevents {
  BE_OBS_GC_START,        // start of GC, arg = allocated size
  BE_OBS_GC_END,          // end of GC, arg = allocated size
};

/* FFI functions */
#define be_writestring(s)       be_writebuffer((s), strlen(s))
#define be_writenewline()       be_writebuffer("\n", 1)

#define be_return(vm)           return be_returnvalue(vm)
#define be_return_nil(vm)       return be_returnnilvalue(vm)

#define be_loadfile(vm, name)   be_loadmode((vm), (name), 0)
#define be_loadmodule(vm, name) be_loadmode((vm), (name), 1)

#define be_loadstring(vm, str) \
    be_loadbuffer((vm), "string", (str), strlen(str))

#define be_dostring(vm, s) \
    (be_loadstring((vm), (s)) || be_pcall((vm), 0))

BERRY_API bint be_str2int(const char *str, const char **endstr);
BERRY_API breal be_str2real(const char *str, const char **endstr);
BERRY_API const char *be_str2num(bvm *vm, const char *str);

BERRY_API int be_top(bvm *vm);
BERRY_API const char *be_typename(bvm *vm, int index);
BERRY_API const char *be_classname(bvm *vm, int index);
BERRY_API bbool be_classof(bvm *vm, int index);
BERRY_API int be_strlen(bvm *vm, int index);
BERRY_API void be_strconcat(bvm *vm, int index);
BERRY_API void be_pop(bvm *vm, int n);
BERRY_API void be_remove(bvm *vm, int index);
BERRY_API int be_absindex(bvm *vm, int index);

BERRY_API bbool be_isnil(bvm *vm, int index);
BERRY_API bbool be_isbool(bvm *vm, int index);
BERRY_API bbool be_isint(bvm *vm, int index);
BERRY_API bbool be_isreal(bvm *vm, int index);
BERRY_API bbool be_isnumber(bvm *vm, int index);
BERRY_API bbool be_isstring(bvm *vm, int index);
BERRY_API bbool be_isclosure(bvm *vm, int index);
BERRY_API bbool be_isntvclos(bvm *vm, int index);
BERRY_API bbool be_isfunction(bvm *vm, int index);
BERRY_API bbool be_isproto(bvm *vm, int index);
BERRY_API bbool be_isclass(bvm *vm, int index);
BERRY_API bbool be_isinstance(bvm *vm, int index);
BERRY_API bbool be_islist(bvm *vm, int index);
BERRY_API bbool be_ismap(bvm *vm, int index);
BERRY_API bbool be_iscomptr(bvm *vm, int index);
BERRY_API bbool be_iscomobj(bvm *vm, int index);
BERRY_API bbool be_isderived(bvm *vm, int index);

BERRY_API bint be_toint(bvm *vm, int index);
BERRY_API breal be_toreal(bvm *vm, int index);
BERRY_API int be_toindex(bvm *vm, int index);
BERRY_API bbool be_tobool(bvm *vm, int index);
BERRY_API const char* be_tostring(bvm *vm, int index);
BERRY_API const char* be_toescape(bvm *vm, int index, int mode);
BERRY_API void* be_tocomptr(bvm* vm, int index);
BERRY_API void be_moveto(bvm *vm, int from, int to);
BERRY_API void be_pushnil(bvm *vm);
BERRY_API void be_pushbool(bvm *vm, int b);
BERRY_API void be_pushint(bvm *vm, bint i);
BERRY_API void be_pushreal(bvm *vm, breal r);
BERRY_API void be_pushstring(bvm *vm, const char *str);
BERRY_API void be_pushnstring(bvm *vm, const char *str, size_t n);
BERRY_API const char* be_pushfstring(bvm *vm, const char *format, ...);
BERRY_API void* be_pushbuffer(bvm *vm, size_t size);
BERRY_API void be_pushvalue(bvm *vm, int index);
BERRY_API void be_pushclosure(bvm *vm, void *cl);
BERRY_API void be_pushntvclosure(bvm *vm, bntvfunc f, int nupvals);
BERRY_API void be_pushntvfunction(bvm *vm, bntvfunc f);
BERRY_API void be_pushclass(bvm *vm, const char *name, const bnfuncinfo *lib);
BERRY_API void be_pushntvclass(bvm *vm, const struct bclass * c);
BERRY_API void be_pushcomptr(bvm *vm, void *ptr);
BERRY_API bbool be_pushiter(bvm *vm, int index);

BERRY_API void be_newlist(bvm *vm);
BERRY_API void be_newmap(bvm *vm);
BERRY_API void be_newmodule(bvm *vm);
BERRY_API void be_newcomobj(bvm *vm, void *data, bntvfunc destory);
BERRY_API void be_newobject(bvm *vm, const char *name);
BERRY_API bbool be_copy(bvm *vm, int index);
BERRY_API bbool be_setname(bvm *vm, int index, const char *name);
BERRY_API bbool be_getglobal(bvm *vm, const char *name);
BERRY_API void be_setglobal(bvm *vm, const char *name);
BERRY_API bbool be_getbuiltin(bvm *vm, const char *name);
BERRY_API bbool be_setmember(bvm *vm, int index, const char *k);
BERRY_API bbool be_getmember(bvm *vm, int index, const char *k);
BERRY_API bbool be_getmethod(bvm *vm, int index, const char *k);
BERRY_API bbool be_getindex(bvm *vm, int index);
BERRY_API bbool be_setindex(bvm *vm, int index);
BERRY_API void be_getupval(bvm *vm, int index, int pos);
BERRY_API bbool be_setupval(bvm *vm, int index, int pos);
BERRY_API bbool be_setsuper(bvm *vm, int index);
BERRY_API void be_getsuper(bvm *vm, int index);
BERRY_API int be_data_size(bvm *vm, int index);
BERRY_API void be_data_push(bvm *vm, int index);
BERRY_API bbool be_data_insert(bvm *vm, int index);
BERRY_API bbool be_data_remove(bvm *vm, int index);
BERRY_API bbool be_data_merge(bvm *vm, int index);
BERRY_API void be_data_resize(bvm *vm, int index);
BERRY_API void be_data_reverse(bvm *vm, int index);
BERRY_API int be_iter_next(bvm *vm, int index);
BERRY_API bbool be_iter_hasnext(bvm *vm, int index);
BERRY_API bbool be_refcontains(bvm *vm, int index);
BERRY_API void be_refpush(bvm *vm, int index);
BERRY_API void be_refpop(bvm *vm);
BERRY_API void be_stack_require(bvm *vm, int count);

/* relop operation APIs */
BERRY_API bbool be_iseq(bvm *vm);
BERRY_API bbool be_isneq(bvm *vm);
BERRY_API bbool be_islt(bvm *vm);
BERRY_API bbool be_isle(bvm *vm);
BERRY_API bbool be_isgt(bvm *vm);
BERRY_API bbool be_isge(bvm *vm);

/* Function call/return APIs */
BERRY_API int be_returnvalue(bvm *vm);
BERRY_API int be_returnnilvalue(bvm *vm);
BERRY_API void be_call(bvm *vm, int argc);
BERRY_API int be_pcall(bvm *vm, int argc);
BERRY_API void be_exit(bvm *vm, int status);

/* exception APIs */
__attribute__((noreturn))
BERRY_API void be_raise(bvm *vm, const char *except, const char *msg);
BERRY_API int be_getexcept(bvm *vm, int code);
BERRY_API void be_dumpvalue(bvm *vm, int index);
BERRY_API void be_dumpexcept(bvm *vm);
BERRY_API void be_stop_iteration(bvm *vm);

BERRY_API void be_regfunc(bvm *vm, const char *name, bntvfunc f);
BERRY_API void be_regclass(bvm *vm, const char *name, const bnfuncinfo *lib);

/* VM management APIs */
BERRY_API bvm* be_vm_new(void);
BERRY_API void be_vm_delete(bvm *vm);

/* Observability hook */
BERRY_API void be_set_obs_hook(bvm *vm, bobshook hook);

/* code load APIs */
BERRY_API int be_loadbuffer(bvm *vm,
    const char *name, const char *buffer, size_t length);
BERRY_API int be_loadmode(bvm *vm, const char *name, bbool islocal);
BERRY_API int be_loadlib(bvm *vm, const char *path);
BERRY_API int be_savecode(bvm *vm, const char *name);

/* module path list APIs */
BERRY_API void be_module_path(bvm *vm);
BERRY_API void be_module_path_set(bvm *vm, const char *path);

/* bytes operations */
BERRY_API void be_pushbytes(bvm *vm, const void *buf, size_t len);
BERRY_API const void* be_tobytes(bvm *vm, int index, size_t *len);

/* registry operation */
BERRY_API int be_register(bvm *vm, int index);
BERRY_API void be_unregister(bvm *vm, int id);
BERRY_API void be_getregister(bvm *vm, int id);

/* debug APIs */
BERRY_API void be_sethook(bvm *vm, const char *mask);
BERRY_API void be_setntvhook(bvm *vm, bntvhook hook, void *data, int mask);

/* basic character IO APIs */
BERRY_API void be_writebuffer(const char *buffer, size_t length);
BERRY_API char* be_readstring(char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
