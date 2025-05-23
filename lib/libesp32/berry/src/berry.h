/**
 * @file       berry.h
 * @brief      Main berry entry API
 * @details    This file is part of the Berry default interpreter.
 * @author     skiars@qq.com, https://github.com/Skiars/berry
 * @copyright  (c) 2018-2022 Guan Wenliang MIT License (https://github.com/Skiars/berry/blob/master/LICENSE)
 */

#ifndef BERRY_H
#define BERRY_H

#include <stddef.h>
#include <stdint.h>

#include "berry_conf.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def BERRY_VERSION
 * @brief do not modify the version number!
 *
 */
#define BERRY_VERSION           "1.1.0"

#if BE_STACK_TOTAL_MAX < BE_STACK_FREE_MIN * 2
#error "The value of the macro BE_STACK_TOTAL_MAX is too small."
#endif

#if BE_INTGER_TYPE == 0
  #define BE_INTEGER            int        /**< BE_INTEGER */
  #define BE_INT_FMTLEN         ""         /**< BE_INT_FMTLEN */
#elif BE_INTGER_TYPE == 1
  #define BE_INTEGER            long       /**< BE_INTEGER */
  #define BE_INT_FMTLEN         "l"        /**< BE_INT_FMTLEN */
#elif BE_INTGER_TYPE == 2
#ifdef _WIN32
    #define BE_INTEGER          __int64    /**< BE_INTEGER */
    #define BE_INT_FMTLEN       "I64"      /**< BE_INT_FMTLEN */
  #else
#define BE_INTEGER              long long  /**< BE_INTEGER */
#define BE_INT_FMTLEN           "ll"       /**< BE_INT_FMTLEN*/
#endif
#else
  #error "Unsupported integer type for `bint`."
#endif
#define BE_INT_FORMAT           "%" BE_INT_FMTLEN "d" /**< BE_INT_FORMAT */

typedef uint8_t bbyte;   /**< bbyte */
typedef BE_INTEGER bint; /**< bint */

#if BE_USE_SINGLE_FLOAT != 0
  typedef float                 breal; /**< breal */
#else
typedef double                  breal; /**< breal */
#endif



#ifndef __cplusplus
#define bbool                   _Bool /**< bbool */
#define bfalse                  0     /**< bfalse */
#define btrue                   1     /**< btrue */
#else
  #define bbool                 bool  /**< bbool */
  #define bfalse                false /**< bfalse */
  #define btrue                 true  /**< btrue */
#endif

/**
 * @enum berrorcode
 * @brief error code definition
 *
 */
enum berrorcode {
    BE_OK = 0,       /**< BE_OK */
    BE_EXIT,         /**< BE_EXIT */
    BE_MALLOC_FAIL,  /**< BE_MALLOC_FAIL */
    BE_EXCEPTION,    /**< BE_EXCEPTION */
    BE_SYNTAX_ERROR, /**< BE_SYNTAX_ERROR */
    BE_EXEC_ERROR,   /**< BE_EXEC_ERROR */
    BE_IO_ERROR      /**< BE_IO_ERROR */
};

/**
 * @{ \name native-module member type specifier.
 */
/**
 * \brief native-module member type specifier.
 *
 */
#define BE_CNIL        0 /**< BE_CNIL */
#define BE_CINT        1 /**< BE_CINT */
#define BE_CREAL       2 /**< BE_CREAL */
#define BE_CBOOL       3 /**< BE_CBOOL */
#define BE_CFUNCTION   4 /**< BE_CFUNCTION */
#define BE_CSTRING     5 /**< BE_CSTRING */
#define BE_CMODULE     6 /**< BE_CMODULE */
/**
 * @}
 */


/**
 * @def BERRY_API
 * @brief API function mark
 *
 */
#if defined(_WIN32) || defined(__CYGWIN__) /* in Windows */
  #if defined(BERRY_MODULE) /* berry extension module */
    #define BERRY_API           __declspec(dllimport)
  #else /* berry core */
    #define BERRY_API           __declspec(dllexport)
  #endif
#else /* other platforms */
  #define BERRY_API             extern
#endif

/**
 * @{ \name only linux.
 */
/**
 * \brief only linux
 *
 */
#if defined(_WIN32) || defined(__CYGWIN__) /* in Windows */
  #define BERRY_LOCAL                                                  /**< BERRY_LOCAL */
#elif defined(__GNUC__) /* in GCC */
#define BERRY_LOCAL           __attribute__ ((visibility ("hidden")))  /**< BERRY_LOCAL */
#else /* other platforms */
  #define BERRY_LOCAL                                                  /**< BERRY_LOCAL */
#endif

#ifdef __cplusplus
#ifdef __cpp_constexpr
  #define BE_CONSTEXPR          constexpr                              /**< BE_CONSTEXPR */
#else
  #define BE_CONSTEXPR                                                 /**< BE_CONSTEXPR */
#endif
#endif

#ifdef __cplusplus
#define BE_EXPORT_VARIABLE      extern "C"                             /**< BE_EXPORT_VARIABLE */
#else
#define BE_EXPORT_VARIABLE                                             /**< BE_EXPORT_VARIABLE */
#endif
/**
 * @}
 */

/**
 * @brief virtual machine structure
 */
typedef struct bvm bvm;

typedef int (*bntvfunc)(bvm*); /**< native function pointer */

/**
 * @struct bclass
 * @brief bclass
 */
struct bclass;

/**
 * @struct bnfuncinfo
 * @brief native function information
 *
 */
typedef struct {
    const char *name;  /**< name */
    bntvfunc function; /**< function */
} bnfuncinfo;

/**
 * @struct bntvmodobj
 * @brief bntvmodobj
 *
 */
typedef struct bntvmodobj {
    const char *name;                               /**< name */
    int type;                                       /**< type */
    union value                                     /**< value */
    {
        bint i;                                     /**< i */
        breal r;                                    /**< r */
        bbool b;                                    /**< b */
        bntvfunc f;                                 /**< f */
        const char *s;                              /**< s */
        const void *o;                              /**< o */
#ifdef __cplusplus
        BE_CONSTEXPR value(bint v) : i(v) {}        /**< i */
        BE_CONSTEXPR value(breal v) : r(v) {}       /**< r */
        BE_CONSTEXPR value(bbool v) : b(v) {}       /**< b */
        BE_CONSTEXPR value(bntvfunc v) : f(v) {}    /**< f */
        BE_CONSTEXPR value(const char *v) : s(v) {} /**< s */
        BE_CONSTEXPR value(const void *v) : o(v) {} /**< o */
#endif
    } u;                                            /**< u */
#ifdef __cplusplus
    BE_CONSTEXPR bntvmodobj(const char *name) :
        name(name), type(BE_CNIL), u(bint(0)) {}    /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, bint v) :
        name(name), type(BE_CINT), u(v) {}          /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, breal v) :
        name(name), type(BE_CREAL), u(v) {}         /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, bbool v) :
        name(name), type(BE_CBOOL), u(v) {}         /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, bntvfunc v) :
        name(name), type(BE_CFUNCTION), u(v) {}     /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, const char *v) :
        name(name), type(BE_CSTRING), u(v) {}       /**< bntvmodobj */
    BE_CONSTEXPR bntvmodobj(const char *name, int _tpye, const void *v) :
        name(name), type(_tpye), u(v) {}            /**< bntvmodobj */
#endif
} bntvmodobj_t; /**< bntvmodobj_t */

/**
 * @struct bntvmodule
 * @brief bntvmodule
 *
 */
typedef struct bntvmodule {
    const char *name;              /**< native module name */
    const bntvmodobj_t *attrs;       /**< native module attributes */
    size_t size;                   /**< native module attribute count */
    const struct bmodule *module;  /**< const module object */
} bntvmodule_t; /**< bntvmodule_t */

/**
 * @struct bclass
 * @brief native class object
 */
struct bclass;

/**
 * @brief we need only the pointer to `bclass` here
 */
typedef const struct bclass *bclass_ptr;

/**
 * @brief array of bclass* pointers, NULL terminated
 */
typedef bclass_ptr bclass_array[];

/**
 * @def be_native_module_nil
 * @brief native module node definition macro
 *
 */
#ifndef __cplusplus
#define be_native_module_nil(_name)                     \
    { .name = (_name), .type = BE_CNIL, .u.i = 0 }

/**
 * @def be_native_module_int
 * @brief be_native_module_int
 *
 */
#define be_native_module_int(_name, _v)                 \
    { .name = (_name), .type = BE_CINT, .u.i = (bint)(_v) }

/**
 * @def be_native_module_real
 * @brief be_native_module_real
 *
 */
#define be_native_module_real(_name, _v)                \
    { .name = (_name), .type = BE_CREAL, .u.r = (breal)(_v) }

/**
 * @def be_native_module_bool
 * @brief be_native_module_bool
 *
 */
#define be_native_module_bool(_name, _b)                \
    { .name = (_name), .type = BE_CBOOL, .u.b = (bbool)(_b) }

/**
 * @def be_native_module_function
 * @brief be_native_module_function
 *
 */
#define be_native_module_function(_name, _f)            \
    { .name = (_name), .type = BE_CFUNCTION, .u.f = (_f) }

/**
 * @def be_native_module_str
 * @brief be_native_module_str
 *
 */
#define be_native_module_str(_name, _s)                 \
    { .name = (_name), .type = BE_CSTRING, .u.s = (_s) }

/**
 * @def be_native_module_module
 * @brief be_native_module_module
 *
 */
#define be_native_module_module(_name, _m)              \
    { .name = (_name), .type = BE_CMODULE, .u.o = &(_m) }
#else

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_nil(_name)                     \
    bntvmodobj(_name)

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_int(_name, _v)                 \
    bntvmodobj(_name, bint(_v))

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_real(_name, _v)                \
    bntvmodobj(_name, breal(_v))

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_bool(_name, _b)                \
    bntvmodobj(_name, bbool(_b))

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_function(_name, _f)            \
    bntvmodobj(_name, _f)

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_str(_name, _s)                 \
    bntvmodobj(_name, _s)

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_module(_name, _m)              \
    bntvmodobj(_name, BE_CMODULE, &(_m))
#endif

/**
 * @def be_native_module_attr_table
 * @brief be_native_module_attr_table
 *
 */
#define be_native_module_attr_table(name)               \
    static const bntvmodobj name##_attrs[] =

/**
 * @def be_native_module
 * @brief be_native_module
 *
 */
#define be_native_module(name)  be_native_module_##name

/**
 * @def be_native_class
 * @brief be_native_class
 *
 */
#define be_native_class(name)  be_class_##name

/**
 * @def be_extern_native_module
 * @brief native module declaration macro
 *
 */
#define be_extern_native_module(name)                   \
    extern const bntvmodule_t be_native_module(name)

/**
 * @def be_extern_native_class
 * @brief native class declaration macro
 *
 */
#define be_extern_native_class(name)                   \
    extern const struct bclass be_native_class(name)

/**
 * @def be_define_native_module
 * @brief native module definition macro
 *
 */
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

/**
 * @def be_define_local_const_str
 * @brief support for solidified berry functions
 *        native const strings outside of global string hash
 *
 */
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

/**
 * @def be_nested_const_str
 * @brief new version for more compact literals
 *
 */
#define be_nested_const_str(_s, _hash, _len)  \
    (bstring*) &(const bcstring) {            \
        .next = (bgcobject *)NULL,            \
        .type = BE_STRING,                    \
        .marked = GC_CONST,                   \
        .extra = 0,                           \
        .slen = _len,                         \
        .hash = 0,                            \
        .s = _s                               \
    }

/**
 * @def be_local_const_str
 * @brief be_local_const_str
 *
 */
#define be_local_const_str(_name) (bstring*) &be_local_const_str_##_name

#define BE_IIF(cond) BE_IIF_ ## cond /**< conditional macro see  https://stackoverflow.com/questions/11632219/c-preprocessor-macro-specialisation-based-on-an-argument */
#define BE_IIF_0(t, f) f             /**< conditional macro see  https://stackoverflow.com/questions/11632219/c-preprocessor-macro-specialisation-based-on-an-argument */
#define BE_IIF_1(t, f) t             /**< conditional macro see  https://stackoverflow.com/questions/11632219/c-preprocessor-macro-specialisation-based-on-an-argument */

/**
 * @def be_local_const_upval
 * @brief be_local_const_upval
 *
 */
#if BE_DEBUG_VAR_INFO
  #define be_local_const_upval(ins, idx) { "", ins, idx }
#else
  #define be_local_const_upval(ins, idx) { ins, idx }
#endif

/**
 * @def BE_DEBUG_SOURCE_FILE
 * @brief conditional block in bproto depending on compilation options
 *
 */
#if BE_DEBUG_SOURCE_FILE
  #define PROTO_SOURCE_FILE(n)   \
    ((bstring*) n),                                         /**< source */
  #define PROTO_SOURCE_FILE_STR(n)  \
    be_local_const_str(n##_str_source),                     /**< source */
#else
  #define PROTO_SOURCE_FILE(n)
  #define PROTO_SOURCE_FILE_STR(n)
#endif

/**
 * @def PROTO_RUNTIME_BLOCK
 * @brief conditional block in bproto depending on compilation options
 *
 */
#if BE_DEBUG_RUNTIME_INFO
  #define PROTO_RUNTIME_BLOCK   \
    NULL,     /**< lineinfo */ \
    0,        /**< nlineinfo */
#else
  #define PROTO_RUNTIME_BLOCK
#endif

/**
 * @def PROTO_VAR_INFO_BLOCK
 * @brief PROTO_VAR_INFO_BLOCK
 *
 */
#if BE_DEBUG_VAR_INFO
  #define PROTO_VAR_INFO_BLOCK   \
    NULL,     /**< varinfo */  \
    0,        /**< nvarinfo */
#else
  #define PROTO_VAR_INFO_BLOCK
#endif

/**
 * @def be_define_local_proto
 * @brief define bproto
 *
 */
#define be_define_local_proto(_name, _nstack, _argc, _is_const, _is_subproto, _is_upval)           \
  static const bproto _name##_proto = {                                           \
    NULL,                                                             /**< bgcobject *next */      \
    BE_PROTO,                                                         /**< type BE_PROTO */        \
    0x08,                                                             /**< marked outside of GC */ \
    (_nstack),                                                        /**< nstack */               \
    BE_IIF(_is_upval)(sizeof(_name##_upvals)/sizeof(bupvaldesc),0),   /**< nupvals */              \
    (_argc),                                                          /**< argc */                 \
    0,                                                                /**< varg */                 \
    sizeof(_name##_code)/sizeof(uint32_t),                            /**< codesize */             \
    BE_IIF(_is_const)(sizeof(_name##_ktab)/sizeof(bvalue),0),         /**< nconst */               \
    BE_IIF(_is_subproto)(sizeof(_name##_subproto)/sizeof(bproto*),0), /**< proto */                \
    NULL,                                                             /**< bgcobject *gray */      \
    BE_IIF(_is_upval)((bupvaldesc*)&_name##_upvals,NULL),             /**< bupvaldesc *upvals */   \
    BE_IIF(_is_const)((bvalue*)&_name##_ktab,NULL),                   /**< ktab */                 \
    BE_IIF(_is_subproto)((struct bproto**)&_name##_subproto,NULL),    /**< bproto **ptab */        \
    (binstruction*) &_name##_code,                                    /**< code */                 \
    be_local_const_str(_name##_str_name),                             /**< name */                 \
    PROTO_SOURCE_FILE_STR(_name)                                      /**< source */               \
    PROTO_RUNTIME_BLOCK                                               /**< */                      \
    PROTO_VAR_INFO_BLOCK                                              /**< */                      \
  }

/**
 * @def be_nested_proto
 * @brief new version for more compact literals
 *
 */
#define be_nested_proto(_nstack, _argc, _varg, _has_upval, _upvals, _has_subproto, _protos, _has_const, _ktab, _fname, _source, _code)     \
  & (const bproto) {                                                              \
    NULL,                                                       /**< bgcobject *next */      \
    BE_PROTO,                                                   /**< type BE_PROTO */        \
    0x08,                                                       /**< marked outside of GC */ \
    (_nstack),                                                  /**< nstack */               \
    BE_IIF(_has_upval)(sizeof(*_upvals)/sizeof(bupvaldesc),0),  /**< nupvals */              \
    (_argc),                                                    /**< argc */                 \
    (_varg),                                                    /**< varg */                 \
    sizeof(*_code)/sizeof(binstruction),                        /**< codesize */             \
    BE_IIF(_has_const)(sizeof(*_ktab)/sizeof(bvalue),0),        /**< nconst */               \
    BE_IIF(_has_subproto)(sizeof(*_protos)/sizeof(bproto*),0),  /**< proto */                \
    NULL,                                                       /**< bgcobject *gray */      \
    (bupvaldesc*) _upvals,                                      /**< bupvaldesc *upvals */   \
    (bvalue*) _ktab,                                            /**< ktab */                 \
    (struct bproto**) _protos,                                  /**< bproto **ptab */        \
    (binstruction*) _code,                                      /**< code */                 \
    ((bstring*) _fname),                                        /**< name */                 \
    PROTO_SOURCE_FILE(_source)                                  /**< source */               \
    PROTO_RUNTIME_BLOCK                                         /**< */                      \
    PROTO_VAR_INFO_BLOCK                                        /**< */                      \
  }

/**
 * @def be_define_local_closure
 * @brief be_define_local_closure
 *
 */
#define be_define_local_closure(_name)                \
  const bclosure _name##_closure = {                  \
    NULL,                     /**< bgcobject *next */ \
    BE_CLOSURE,               /**< type BE_CLOSURE */ \
    GC_CONST,                 /**< marked GC_CONST */ \
    0,                        /**< nupvals */         \
    NULL,                     /**< bgcobject *gray */ \
    (bproto*) &_name##_proto, /**< proto */           \
    { NULL }                  /**< upvals */          \
  }

/**
 * @def be_local_closure
 * @brief new version for more compact literals
 *
 */
#define be_local_closure(_name, _proto)       \
  static const bclosure _name##_closure = {   \
    NULL,             /**< bgcobject *next */ \
    BE_CLOSURE,       /**< type BE_CLOSURE */ \
    GC_CONST,         /**< marked GC_CONST */ \
    0,                /**< nupvals */         \
    NULL,             /**< bgcobject *gray */ \
    (bproto*) _proto, /**< proto */           \
    { NULL }          /**< upvals */          \
  }

/**
 * @{ \name debug hook typedefs.
 */
/**
 * \brief debug hook typedefs.
 *
 */
#define BE_HOOK_LINE    1 /**< BE_HOOK_LINE */
#define BE_HOOK_CALL    2 /**< BE_HOOK_CALL */
#define BE_HOOK_RET     4 /**< BE_HOOK_RET */
#define BE_HOOK_EXCEPT  8 /**< BE_HOOK_EXCEPT */
/**
 * @}
 */

/**
 * @struct bhookinfo
 * @brief bhookinfo
 *
 */
typedef struct bhookinfo {
    int type;               /**< current hook type */
    int line;               /**< current line number */
    const char *source;     /**< source path information */
    const char *func_name;  /**< current function name */
    void *data;             /**< user extended data */
} bhookinfo_t; /**< bhookinfo_ */

/**
 * @fn void (*bntvhook)(bvm *vm, bhookinfo *info)
 * @brief void (*bntvhook)(bvm *vm, bhookinfo *info)
 *
 * @param vm virtual machine instance
 * @param info
 */
typedef void (*bntvhook)(bvm *vm, bhookinfo_t *info);

/**
 * @def be_assert
 * @brief the default assert definition
 *
 */
#if !BE_DEBUG
#if defined(be_assert)
#undef be_assert
#endif
#define be_assert(expr)       ((void)0)
#endif

/**
 * @fn void (*bobshook)(bvm *vm, int event, ...)
 * @brief Observability hook
 *
 * @param vm virtual machine instance
 * @param event
 */
typedef void (*bobshook)(bvm *vm, int event, ...);
typedef uint32_t(*bmicrosfnct)(void);

/**
 * @enum beobshookevents
 * @brief beobshookevents
 *
 */
enum beobshookevents {
    BE_OBS_PCALL_ERROR,         /**< called when be_callp() returned an error, most likely an exception */
    BE_OBS_GC_START,            /**< start of GC, arg = allocated size */
    BE_OBS_GC_END,              /**< end of GC, arg = allocated size */
    BE_OBS_VM_HEARTBEAT,        /**< VM heartbeat called every million instructions */
    BE_OBS_STACK_RESIZE_START,  /**< Berry stack resized */
    BE_OBS_MALLOC_FAIL,         /**< Memory allocation failed */
};

typedef int (*bctypefunc)(bvm*, const void*); /**< bctypefunc */

/**
 * @def be_writestring
 * @note FFI function
 * @brief be_writestring
 *
 */
#define be_writestring(s)       be_writebuffer((s), strlen(s))

/**
 * @def be_writenewline
 * @note FFI function
 * @brief be_writenewline
 *
 */
#define be_writenewline()       be_writebuffer("\n", 1)

/**
 * @def be_return
 * @note FFI function
 * @brief be_return
 *
 * @param vm virtual machine instance virtual machine instance
 */
#define be_return(vm)           return be_returnvalue(vm)

/**
 * @def be_return_nil
 * @note FFI function
 * @brief be_return_nil
 *
 * @param vm virtual machine instance virtual machine instance
 */
#define be_return_nil(vm)       return be_returnnilvalue(vm)

/**
 * @def be_loadfile
 * @note FFI function
 * @brief be_loadfile
 *
 * @param vm virtual machine instance virtual machine instance
 * @param name (???)
 */
#define be_loadfile(vm, name)   be_loadmode((vm), (name), 0)

/**
 * @def be_loadfile
 * @note FFI function
 * @brief be_loadfile
 *
 * @param vm virtual machine instance virtual machine instance
 * @param name (???)
 */
#define be_loadfile_local(vm, name, islocal)   be_loadmode((vm), (name), islocal)

/**
 * @def be_loadmodule
 * @note FFI function
 * @brief be_loadmodule
 *
 * @param vm virtual machine instance virtual machine instance
 * @param name
 *
 */
#define be_loadmodule(vm, name) be_loadmode((vm), (name), 1)

/**
 * @def be_loadstring
 * @note FFI function
 * @brief be_loadstring
 *
 * @param vm virtual machine instance virtual machine instance
 * @param str Berry code to be compiled in global context
 *
 */
#define be_loadstring(vm, str) \
    be_loadbuffer((vm), "string", (str), strlen(str))

/**
 * @def be_loadstring_local
 * @note FFI function
 * @brief be_loadstring
 *
 * @param vm virtual machine instance virtual machine instance
 * @param str Berry code to be compiled in local or global context
 *
 */
#define be_loadstring_local(vm, str, islocal) \
    be_loadbuffer_local((vm), "string", (str), strlen(str), islocal)

/**
 * @def be_dostring
 * @note FFI function
 * @brief be_dostring
 *
 * @param vm virtual machine instance virtual machine instance
 * @param s (???)
 *
 */
#define be_dostring(vm, s) \
    (be_loadstring((vm), (s)) || be_pcall((vm), 0))

/**
 * @fn bint be_str2int(const char*, const char**)
 * @note FFI function
 * @brief (???)
 *
 * @param str (???)
 * @param endstr (???)
 * @return (???)
 */
BERRY_API bint be_str2int(const char *str, const char **endstr);

/**
 * @fn breal be_str2real(const char*, const char**)
 * @note FFI function
 * @brief (???)
 *
 * @param str
 * @param endstr (???)
 * @return (???)
 */
BERRY_API breal be_str2real(const char *str, const char **endstr);

/**
 * @fn const char* be_str2num(bvm *vm, const char *str)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param str (???)
 * @return (???)
 */
BERRY_API const char* be_str2num(bvm *vm, const char *str);

/**
 * @fn int be_top(bvm*)
 * @note FFI function
 * @brief returns the absolute index value of the top element in the virtual stack
 *
 * This function returns the absolute index value of the top element in the virtual stack.
 * This value is also the number of elements in the virtual stack (the capacity of the virtual stack).
 * Call this function before adding or subtracting elements in the virtual stack to get the number of parameters of the native function.
 *
 * @param vm virtual machine instance virtual machine instance
 * @return (???)
 */
BERRY_API int be_top(bvm *vm);

/**
 * @fn const char* be_typename(bvm *vm, int index)
 * @note FFI function
 * @brief converts the type of the Berry object into a string and returns it
 *
 * This function converts the type of the Berry object into a string and returns it.
 * For example, it returns "int" for an integer object, and "function" for a function object.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index index of the object to be operated
 * @return string corresponding to the parameter type (see: baselib_type)
 */
BERRY_API const char* be_typename(bvm *vm, int index);

/**
 * @fn const char* be_classname(bvm *vm, int index)
 * @note FFI function
 * @brief converts the type of the Berry object into a string and returns it.
 *
 * This function converts the type of the Berry object into a string and returns it.
 * For example, it returns "int" for an integer object, and "function" for a function object
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index index of the object to be operated
 * @return string corresponding to the parameter type (see: baselib_type)
 */
BERRY_API const char* be_classname(bvm *vm, int index);

/**
 * @fn bool be_classof(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index (???)
 * @return (???)
 */
BERRY_API bbool be_classof(bvm *vm, int index);

/**
 * @fn int be_strlen(bvm*, int)
 * @note FFI function
 * @brief length of the specified Berry string
 *
 * This function returns the number of bytes in the string at index
 * (the ’\0’ characters at the end of the Berry string are not counted).
 * If the value of the index position is not a string, the be_strlen function will return 0.
 * Although the Berry string is compatible with the C string format,
 * it is not recommended to use the strlen function of the C standard library to measure the length of the Berry string.
 * For Berry strings, be_strlen is faster than strlen and has better compatibility.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index index of the object to be operated
 * @return length
 */
BERRY_API int be_strlen(bvm *vm, int index);

/**
 * @fn void be_strconcat(bvm*, int)
 * @note FFI function
 * @brief splice two Berry strings
 *
 * This function will concatenate the string at the parameter position of index with the string at the top position of the stack,
 * and then put the resulting string into the position indexed by index.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index (???)
 */
BERRY_API void be_strconcat(bvm *vm, int index);

/**
 * @fn void be_pop(bvm*, int)
 * @note FFI function
 * @brief pops the value at the top of the stack
 *
 * Note that the value of n cannot exceed the capacity of the stack.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param n number of values to be popped
 */
BERRY_API void be_pop(bvm *vm, int n);

/**
 * @fn void be_remove(bvm*, int)
 * @note FFI function
 * @brief remove a value from the stack
 *
 * After the value at index is moved out, the following values will be filled forward,
 * and the stack capacity will be reduced by one.
 * The value of index cannot exceed the capacity of the stack.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index the object to be removed
 */
BERRY_API void be_remove(bvm *vm, int index);

/**
 * @fn int be_absindex(bvm*, int)
 * @note FFI function
 * @brief absolute index value of a given index value
 *
 * If index is positive, the return value is the value of index.
 * If index is negative, the return value of textttbe_absindex is the absolute index value corresponding to index.
 * When index is a negative value (relative index), its index position cannot be lower than the bottom of the stack.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index index value
 * @return absolute index
 */
BERRY_API int be_absindex(bvm *vm, int index);

/**
 * @fn bool be_isnil(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is nil
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is nil,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isnil(bvm *vm, int index);

/**
 * @fn bool be_isbool(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is bool
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is bool,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isbool(bvm *vm, int index);

/**
 * @fn bool be_isint(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is int
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is int,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isint(bvm *vm, int index);

/**
 * @fn bool be_isreal(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is real
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is real,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isreal(bvm *vm, int index);

/**
 * @fn bool be_isnumber(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is number
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is number,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isnumber(bvm *vm, int index);

/**
 * @fn bool be_isstring(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is string
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is string,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isstring(bvm *vm, int index);

/**
 * @fn bool be_isclosure(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is closure
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is closure,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isclosure(bvm *vm, int index);

/**
 * @fn bool be_isntvclos(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is primitive closure type
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is primitive closure type,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isntvclos(bvm *vm, int index);

/**
 * @fn bool be_isfunction(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is function
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is function,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isfunction(bvm *vm, int index);

/**
 * @fn bool be_isproto(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is proto
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is proto,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isproto(bvm *vm, int index);

/**
 * @fn bool be_isclass(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is class
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is class,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isclass(bvm *vm, int index);

/**
 * @fn bool be_isinstance(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is instance
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is instance,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isinstance(bvm *vm, int index);

/**
 * @fn bool be_ismapinstance(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is instance
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is
 * an instance of class map (or derived). If it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_ismapinstance(bvm *vm, int index);

/**
 * @fn bool be_ismapinstance(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is instance
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is
 * an instance of class list (or derived). If it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_islistinstance(bvm *vm, int index);

/**
 * @fn bool be_ismodule(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is module
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is module,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_ismodule(bvm *vm, int index);

/**
 * @fn bool be_islist(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is list
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is list,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_islist(bvm *vm, int index);

/**
 * @fn bool be_ismap(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is map
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is map,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_ismap(bvm *vm, int index);

/**
 * @fn bool be_iscomptr(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is universal pointer type
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is universal pointer type,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_iscomptr(bvm *vm, int index);

/**
 * @fn bool be_iscomobj(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_iscomobj(bvm *vm, int index);

/**
 * @fn bool be_isderived(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isderived(bvm *vm, int index);

/**
 * @fn bool be_isbytes(bvm*, int)
 * @note FFI function
 * @brief value in virtual stack is instance or sub-instance of class bytes
 *
 * This function returns whether the value indexed by the parameter index in the virtual stack is instance or sub-instance of class bytes,
 * if it is, it returns 1, otherwise it returns 0
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return true/false
 */
BERRY_API bbool be_isbytes(bvm *vm, int index);

/**
 * @fn bint be_toint(bvm*, int)
 * @note FFI function
 * @brief virtual stack to integer type
 *
 * Get the value of the index position of index from the virtual stack and return it as an integer type.
 * This function does not check the correctness of the type.
 * If the value is an instance, the method toint() is called if it exists.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index value index
 * @return (???)
 */
BERRY_API bint be_toint(bvm *vm, int index);

/**
 * @fn breal be_toreal(bvm*, int)
 * @note FFI function
 * @brief virtual stack to floating-point number type
 *
 * Get the value of the index position of index from the virtual stack and return it as an floating-point number type.
 * This function does not check the correctness of the type.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API breal be_toreal(bvm *vm, int index);

/**
 * @fn int be_toindex(bvm*, int)
 * @note FFI function
 * @brief virtual stack to integer type
 *
 * Get the value of the index position of index from the virtual stack and return it as an integer type.
 * This function does not check the correctness of the type.
 * Unlike be_toint, the return value type of be_toindex is int, while the return value of the former is bint.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API int be_toindex(bvm *vm, int index);

/**
 * @fn bool be_tobool(bvm*, int)
 * @note FFI function
 * @brief virtual stack to Boolean type
 *
 * Get the value of the index position of index from the virtual stack and return it as a Boolean type.
 * If the indexed value is not of Boolean type, it will be converted according to the rules in section type_bool,
 * and the conversion process will not cause the indexed value to change.
 * If the value is an instance, the method tobool() is called if it exists.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API bbool be_tobool(bvm *vm, int index);

/**
 * @fn const char* be_tostring(bvm *vm, int index)
 * @note FFI function
 * @brief virtual stack to string
 *
 * Get the value of the index position of index from the virtual stack and return it as a string type.
 * If the indexed value is not a string type, the indexed value will be converted to a string,
 * and the conversion process will replace the value at the indexed position in the virtual stack with the converted string.
 * The string returned by this function always ends with ’\0’ characters.
 * If the value is an instance, the method tostring() is called if it exists.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API const char* be_tostring(bvm *vm, int index);

/**
 * @fn const char* be_toescape(bvm *vm, int index, int mode)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 * @param mode
 * @return (???)
 */
BERRY_API const char* be_toescape(bvm *vm, int index, int mode);

/**
 * @fn void* be_tocomptr(bvm *vm, int index)
 * @note FFI function
 * @brief virtual stack to general pointer
 *
 * Get the value of the index position of index from the virtual stack and return it as a general pointer type.
 * This function does not check the correctness of the type.
 *
 * @param vm virtual machine instance virtual machine instance
 * @param index
 */
BERRY_API void* be_tocomptr(bvm *vm, int index);

/**
 * @fn void be_moveto(bvm*, int, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance virtual machine instance
 * @param from
 * @param to
 */
BERRY_API void be_moveto(bvm *vm, int from, int to);

/**
 * @fn void be_pushnil(bvm*)
 * @note FFI function
 * @brief Push a nil value onto the virtual stack.
 *
 * @param vm virtual machine instance
 *
 */
BERRY_API void be_pushnil(bvm *vm);

/**
 * @fn void be_pushbool(bvm*, int)
 * @note FFI function
 * @brief Push a Boolean value onto the virtual stack
 *
 * Push a Boolean value onto the virtual stack.
 * The parameter b is the boolean value to be pushed onto the stack.
 * When the value is 0, it means false, otherwise it is true.
 *
 * @param vm virtual machine instance
 * @param b
 */
BERRY_API void be_pushbool(bvm *vm, int b);

/**
 * @fn void be_pushint(bvm*, bint)
 * @note FFI function
 * @brief Push an integer value i onto the virtual stack.
 *
 * @param vm virtual machine instance
 * @param i
 */
BERRY_API void be_pushint(bvm *vm, bint i);

/**
 * @fn void be_pushreal(bvm*, breal)
 * @note FFI function
 * @brief Push a floating point value r onto the virtual stack.
 *
 * @param vm virtual machine instance
 * @param r
 */
BERRY_API void be_pushreal(bvm *vm, breal r);

/**
 * @fn void be_pushstring(bvm*, const char*)
 * @note FFI function
 * @brief Push the string str onto the virtual stack.
 *
 * Push the string str onto the virtual stack.
 * The parameter str must point to a C string that ends with a null character ’\0’,
 * and a null pointer cannot be passed in.
 *
 * @param vm virtual machine instance
 * @param str
 */
BERRY_API void be_pushstring(bvm *vm, const char *str);

/**
 * @fn void be_pushnstring(bvm*, const char*, size_t)
 * @note FFI function
 * @brief Push the string str of length n onto the virtual stack.
 *
 * Push the string str of length n onto the virtual stack.
 * The length of the string is subject to the parameter n,
 * and the null character is not used as the end mark of the string.
 *
 * @param vm virtual machine instance
 * @param str
 * @param n
 */
BERRY_API void be_pushnstring(bvm *vm, const char *str, size_t n);

/**
 * @fn const char* be_pushfstring(bvm *vm, const char *format, ...)
 * @note FFI function
 * @brief Push the formatted string into the virtual stack.
 *
 * Push the formatted string into the virtual stack.
 * The parameter format is a formatted string, which contains the text to be pushed onto the stack,
 * and the format parameter contains a label,
 * which can be replaced by the value specified by the subsequent additional parameter and formatted as required.
 * According to the label of the format string, a series of additional parameters may be required,
 * and each additional parameter will replace the corresponding % label in the format parameter.
 *
 * @param vm virtual machine instance
 * @param format
 * @return (???)
 */
BERRY_API const char* be_pushfstring(bvm *vm, const char *format, ...);

/**
 * @fn void* be_pushbuffer(bvm *vm, size_t size)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param size
 */
BERRY_API void* be_pushbuffer(bvm *vm, size_t size);

/**
 * @fn void be_pushvalue(bvm*, int)
 * @note FFI function
 * @brief Push the value with index index onto the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param index
 */
BERRY_API void be_pushvalue(bvm *vm, int index);

/**
 * @fn void be_pushclosure(bvm*, void*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param cl
 */
BERRY_API void be_pushclosure(bvm *vm, void *cl);

/**
 * @fn void be_pushntvclosure(bvm*, bntvfunc, int)
 * @note FFI function
 * @brief Push a native closure onto the top of the virtual stack.
 *
 * Push a native closure onto the top of the virtual stack.
 * The parameter f is the C function pointer of the native closure,
 * and nupvals is the upvalue number of the closure.
 *
 * @param vm virtual machine instance
 * @param f
 * @param nupvals
 */
BERRY_API void be_pushntvclosure(bvm *vm, bntvfunc f, int nupvals);

/**
 * @fn void be_pushntvfunction(bvm*, bntvfunc)
 * @note FFI function
 * @brief Push a native function onto the top of the virtual stack, and the parameter f is the native function pointer.
 *
 * @param vm virtual machine instance
 * @param f
 */
BERRY_API void be_pushntvfunction(bvm *vm, bntvfunc f);

/**
 * @fn void be_pushclass(bvm*, const char*, const bnfuncinfo*)
 * @note FFI function
 * @brief Push a native class onto the top of the virtual stack
 *
 * Push a native class onto the top of the virtual stack.
 * The parameter name is the name of the native class, and the parameter lib is the attribute description of the native class.
 *
 * @param vm virtual machine instance
 * @param name
 * @param lib
 */
BERRY_API void be_pushclass(bvm *vm, const char *name, const bnfuncinfo *lib);

/**
 * @fn void be_pushntvclass(bvm*, const struct bclass*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param c
 */
BERRY_API void be_pushntvclass(bvm *vm, const struct bclass *c);

/**
 * @fn void be_pushcomptr(bvm*, void*)
 * @note FFI function
 * @brief Push a general pointer onto the top of the virtual stack
 *
 * Push a general pointer onto the top of the virtual stack.
 * The general pointer ptr points to a certain C data area.
 * Since the content pointed to by this pointer is not maintained by Berry’s garbage collector,
 * users have to maintain the life cycle of the data themselves.
 *
 * @param vm virtual machine instance
 * @param ptr
 */
BERRY_API void be_pushcomptr(bvm *vm, void *ptr);

/**
 * @fn bool be_pushiter(bvm*, int)
 * @note FFI function
 * @brief Push an iterator onto the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API bbool be_pushiter(bvm *vm, int index);

/**
 * @fn void be_newlist(bvm*)
 * @note FFI function
 * @brief creates a new list value
 *
 * After this function is successfully called, the new list value will be pushed onto the top of the stack.
 * list value is an internal representation of a list, not to be confused with an instance of the list class.
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_newlist(bvm *vm);

/**
 * @fn void be_newmap(bvm*)
 * @note FFI function
 * @brief creates a new map value
 *
 * After this function is successfully called, the new map value will be pushed onto the top of the stack.
 * map value is an internal representation of a list, not to be confused with an instance of the map class.
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_newmap(bvm *vm);

/**
 * @fn void be_newmodule(bvm*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_newmodule(bvm *vm);

/**
 * @fn void be_newcomobj(bvm*, void*, bntvfunc)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param data
 * @param destroy
 */
BERRY_API void be_newcomobj(bvm *vm, void *data, bntvfunc destroy);

/**
 * @fn void be_newobject(bvm*, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 */
BERRY_API void be_newobject(bvm *vm, const char *name);

/**
 * @fn bool be_copy(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 * @return
 */
BERRY_API bbool be_copy(bvm *vm, int index);

/**
 * @fn bool be_setname(bvm*, int, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 * @param name
 * @return (???)
 */
BERRY_API bbool be_setname(bvm *vm, int index, const char *name);

/**
 * @fn bool be_getglobal(bvm*, const char*)
 * @note FFI function
 * @brief pushes the global variable with the specified name onto the stack
 *
 * After this function is called, the global variable named name will be pushed onto the top of the virtual stack
 *
 * @param vm virtual machine instance
 * @param name
 * @return (???)
 */
BERRY_API bbool be_getglobal(bvm *vm, const char *name);

/**
 * @fn void be_setglobal(bvm*, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 */
BERRY_API void be_setglobal(bvm *vm, const char *name);

/**
 * @fn bool be_getbuiltin(bvm*, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 * @return (???)
 */
BERRY_API bbool be_getbuiltin(bvm *vm, const char *name);

/**
 * @fn bool be_setmember(bvm*, int, const char*)
 * @note FFI function
 * @brief set the value of the member variable of the instance object class
 *
 * This function will copy the value at the top of the stack to the member k of the index position instance.
 * Note that the top element of the stack will not pop up automatically.
 *
 * @param vm virtual machine instance
 * @param index index of the instance object
 * @param k name of the member
 * @return (???)
 */
BERRY_API bbool be_setmember(bvm *vm, int index, const char *k);

/**
 * @fn bool be_getmember(bvm*, int, const char*)
 * @note FFI function
 * @brief get the value of the member variable of the instance object class
 *
 * This function pushes the value of the member of the index position instance k onto the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param index index of the instance object
 * @param k name of the member
 * @return (???)
 */
BERRY_API bbool be_getmember(bvm *vm, int index, const char *k);

/**
 * @fn bool be_getmethod(bvm*, int, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 * @param k
 * @return (???)
 */
BERRY_API bbool be_getmethod(bvm *vm, int index, const char *k);

/**
 * @fn bool be_getindex(bvm*, int)
 * @note FFI function
 * @brief get the value of list or map
 *
 * This function is used to get an element from the map or list container (internal values, not instances of map or list classes),
 * and the index of the element is stored at the top of the stack (relative index is  − 1).
 * After calling this function, the value obtained from the container will be pushed onto the top of the stack.
 * If there is no subscript pointed to by the container, the value of nil will be pushed onto the top of the stack.
 *
 * @param vm virtual machine instance
 * @param index index of the object to be operated
 * @return (???)
 */
BERRY_API bbool be_getindex(bvm *vm, int index);

/**
 * @fn bool be_setindex(bvm*, int)
 * @note FFI function
 * @brief set a value in list or map
 *
 * This function is used to write an element of the map or list container.
 * The index of the value to be written in the virtual stack is  − 1,
 * and the index of the subscript of the write position in the virtual stack is  − 2.
 * If the element with the specified subscript does not exist in the container, the write operation will fail.
 *
 * @param vm virtual machine instance
 * @param index index of the object to be operated
 * @return (???)
 */
BERRY_API bbool be_setindex(bvm *vm, int index);

/**
 * @fn void be_getupval(bvm*, int, int)
 * @note FFI function
 * @brief read an Up Value of the native closure
 *
 * The read Up Value will be pushed onto the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param index the native closure index value of the Up Value to be read
 * @param pos position of the Up Value in the native closure Up Value table (numbering starts from 0)
 */
BERRY_API void be_getupval(bvm *vm, int index, int pos);

/**
 * @fn bool be_setupval(bvm*, int, int)
 * @note FFI function
 * @brief set an Up Value of the native closure.
 *
 *  This function obtains a value from the top of the virtual stack and writes it to the target Up Value.
 *  After the operation is completed, the top value of the stack will not be popped from the stack.
 *
 * @param vm virtual machine instance
 * @param index the native closure index value of the Up Value to be read
 * @param pos position of the Up Value in the native closure Up Value table (numbering starts from 0)
 * @return (???)
 */
BERRY_API bbool be_setupval(bvm *vm, int index, int pos);

/**
 * @fn bool be_setsuper(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API bbool be_setsuper(bvm *vm, int index);

/**
 * @fn void be_getsuper(bvm*, int)
 * @note FFI function
 * @brief get the parent object of the base class or instance of the class.
 *
 * If the value at index is a class with a base class, the function will push its base class onto the top of the stack;
 * if the value at index is an object with a parent object,
 * the function will take its parent The object is pushed onto the top of the stack; otherwise,
 * a value of nil is pushed onto the top of the stack.
 *
 * @param vm virtual machine instance
 * @param index the class or object to be operated
 */
BERRY_API void be_getsuper(bvm *vm, int index);

/**
 * @fn int be_data_size(bvm*, int)
 * @note FFI function
 * @brief get the number of elements contained in the container
 *
 * If the value at index is a Map value or List value,
 * the function returns the number of elements contained in the container, otherwise it returns -1.
 *
 * @param vm virtual machine instance
 * @param index index of the container object to be operated
 * @return (???)
 */
BERRY_API int be_data_size(bvm *vm, int index);

/**
 * @fn void be_data_push(bvm*, int)
 * @note FFI function
 * @brief append a new element to the end of the container.
 *
 * The object at index must be a List value.
 * This function gets a value from the top of the stack and appends it to the end of the container.
 * After the operation is completed, the value at the top of the stack will not be popped from the stack.
 *
 * @param vm virtual machine instance
 * @param index index of the container object to be operate
 */
BERRY_API void be_data_push(bvm *vm, int index);

/**
 * @fn bool be_data_insert(bvm*, int)
 * @note FFI function
 * @brief insert a pair of elements into the container
 *
 * The object at index must be a List value or a Map value.
 * The inserted element forms a pair of key-value pairs.
 * The value is stored at the top of the stack, and the key is stored at the previous index on the top of the stack.
 * It should be noted that the key inserted into the Map container cannot be a nil value,
 * and the key inserted into the List container must be an integer value.
 * If the operation is successful, the function will return bture, otherwise it will return bfalse.
 *
 * @param vm virtual machine instance
 * @param index container object to be operated
 * @return (???)
 */
BERRY_API bbool be_data_insert(bvm *vm, int index);

/**
 * @fn bool be_data_remove(bvm*, int)
 * @note FFI function
 * @brief remove an element in the container
 *
 * The object at index must be a List value or Map value.
 * For the Map container, the key to delete the element is stored on the top of the virtual stack
 * (need to be pressed before the function call);
 * for the List container, the index of the element to be deleted is stored on the top of the virtual stack
 * (need to be before the function call) push into).
 * If the operation is successful, the function will return bture, otherwise it will return bfalse.
 *
 * @param vm virtual machine instance
 * @param index container object to be operated.
 * @return (???)
 */
BERRY_API bbool be_data_remove(bvm *vm, int index);

/**
 * @fn bool be_data_merge(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 * @return (???)
 */
BERRY_API bbool be_data_merge(bvm *vm, int index);

/**
 * @fn void be_data_resize(bvm*, int)
 * @note FFI function
 * @brief reset the capacity of the container
 *
 * This function is only available for List containers, and the new capacity is stored on the top of the virtual stack (must be an integer).
 *
 * @param vm virtual machine instance
 * @param index container object to be operated
 */
BERRY_API void be_data_resize(bvm *vm, int index);

/**
 * @fn void be_data_reverse(bvm*, int)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 */
BERRY_API void be_data_reverse(bvm *vm, int index);

/**
 * @fn int be_iter_next(bvm*, int)
 * @note FFI function
 * @brief get the next element of the iterator
 *
 * The iterator object may be an iterator of a List container or a Map container.
 * For the List iterator, this function pushes the iteration result value onto the top of the stack,
 * while for the Map iterator, it pushes the key value into the previous position and the top of the stack respectively.
 * Calling this function will update the iterator.
 * If the function returns 0, the call fails, returning 1 to indicate that the current iterator is a List iterator,
 * and returning 2 to indicate that the current iterator is a Map iterator.
 *
 * @param vm virtual machine instance
 * @param index iterator to be operated
 * @return (???)
 */
BERRY_API int be_iter_next(bvm *vm, int index);

/**
 * @fn bool be_iter_hasnext(bvm*, int)
 * @note FFI function
 * @brief test whether there is another element in the iterator
 *
 * The iterator object may be an iterator of a List container or a Map container.
 * If there are more iterable elements in the iterator, return 1, otherwise return 0.
 *
 * @param vm virtual machine instance
 * @param index iterator to be operated
 * @return (???)
 */
BERRY_API bbool be_iter_hasnext(bvm *vm, int index);

/**
 * @fn bool be_refcontains(bvm*, int)
 * @note FFI function
 * @brief test whether there is a reference to the specified object in the reference stack
 *
 * This function is used to test whether there is a reference to the specified object in the reference stack.
 * It must be used in conjunction with be_refpush and be_refpop.
 * This API can avoid recursion when traversing objects that have their own references.
 * This function is used for the value of an instance type.
 * If there is a reference to the object in the reference stack, it returns 1, otherwise it returns 0.
 *
 * @param vm virtual machine instance
 * @param index object to be operated
 * @return (???)
 */
BERRY_API bbool be_refcontains(bvm *vm, int index);

/**
 * @fn void be_refpush(bvm*, int)
 * @note FFI function
 * @brief Push the reference of the specified object onto the reference stack.
 *
 * This function is used for the value of an instance type.
 *
 * @param vm virtual machine instance
 * @param index object to be operated
 */
BERRY_API void be_refpush(bvm *vm, int index);

/**
 * @fn void be_refpop(bvm*)
 * @note FFI function
 * @brief Pop the object at the top of the reference stack
 *
 * This function is used in pairs with be_refpush
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_refpop(bvm *vm);

/**
 * @fn void be_stack_require(bvm*, int)
 * @note FFI function
 * @brief tests the amount of free space on the stack and expands the stack space if it is insufficient
 *
 * If the free capacity of the virtual stack allocated by the VM to the native function is lower than this value,
 * an expansion operation will be performed.
 *
 * @param vm virtual machine instance
 * @param count required free stack capacity.
 */
BERRY_API void be_stack_require(bvm *vm, int count);

/**
 * @fn bool be_getmodule(bvm*, const char*)
 * @note FFI function
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param k
 * @return (???)
 */
BERRY_API bbool be_getmodule(bvm *vm, const char *k);

/**
 * @fn bool be_iseq(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_iseq(bvm *vm);

/**
 * @fn bool be_isneq(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_isneq(bvm *vm);

/**
 * @fn bool be_islt(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_islt(bvm *vm);

/**
 * @fn bool be_isle(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_isle(bvm *vm);

/**
 * @fn bool be_isgt(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_isgt(bvm *vm);

/**
 * @fn bool be_isge(bvm*)
 * @note relop operation API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bbool be_isge(bvm *vm);

/**
 * @fn int be_returnvalue(bvm*)
 * @note Function call/return API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API int be_returnvalue(bvm *vm);

/**
 * @fn int be_returnnilvalue(bvm*)
 * @note Function call/return API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API int be_returnnilvalue(bvm *vm);

/**
 * @fn void be_call(bvm*, int)
 * @note Function call/return API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param argc
 */
BERRY_API void be_call(bvm *vm, int argc);

/**
 * @fn int be_pcall(bvm*, int)
 * @note Function call/return API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param argc
 * @return (???)
 */
BERRY_API int be_pcall(bvm *vm, int argc);

/**
 * @fn void be_exit(bvm*, int)
 * @note Function call/return API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param status
 */
BERRY_API void be_exit(bvm *vm, int status);

/**
 * @fn void be_raise(bvm*, const char*, const char*)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param except
 * @param msg
 */
#ifdef __GNUC__
__attribute__((noreturn))
#endif
BERRY_API void be_raise(bvm *vm, const char *except, const char *msg);

/**
 * @fn int be_getexcept(bvm*, int)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param code
 * @return (???)
 */
BERRY_API int be_getexcept(bvm *vm, int code);

/**
 * @fn void be_dumpvalue(bvm*, int)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param index
 */
BERRY_API void be_dumpvalue(bvm *vm, int index);

/**
 * @fn void be_dumpexcept(bvm*)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_dumpexcept(bvm *vm);

/**
 * @fn void be_stop_iteration(bvm*)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_stop_iteration(bvm *vm);

/**
 * @fn void be_regfunc(bvm*, const char*, bntvfunc)
 * @note exception API
 * @brief register a native function
 *
 * The specific behavior of this function is related to the value of the BE_USE_PRECOMPILED_OBJECT macro
 * (although the FFI is still available when using the compile-time construction technique,
 * it cannot dynamically register the built-in variables.
 * In this case, please refer to the method of registering the built-in objects.
 *
 * @param vm virtual machine instance
 * @param name name of the native function
 * @param f pointer of the native function
 */
BERRY_API void be_regfunc(bvm *vm, const char *name, bntvfunc f);

/**
 * @fn void be_regclass(bvm*, const char*, const bnfuncinfo*)
 * @note exception API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 * @param lib
 */
BERRY_API void be_regclass(bvm *vm, const char *name, const bnfuncinfo *lib);

/**
 * @fn bvm* be_vm_new(void)
 * @note VM management API
 * @brief Construct a VM
 *
 * @return (???)
 */
BERRY_API bvm* be_vm_new(void);

/**
 * @fn void be_vm_delete(bvm*)
 * @note VM management API
 * @brief Destroy a VM
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_vm_delete(bvm *vm);

/**
 * @fn void be_set_obs_hook(bvm*, bobshook)
 * @note Observability hook
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param hook
 */
BERRY_API void be_set_obs_hook(bvm *vm, bobshook hook);
BERRY_API void be_set_obs_micros(bvm *vm, bmicrosfnct micros);


/**
 * @fn void be_set_ctype_func_handler(bvm*, bctypefunc)
 * @note Observability hook
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param handler
 */
BERRY_API void be_set_ctype_func_handler(bvm *vm, bctypefunc handler);

/**
 * @fn bctypefunc be_get_ctype_func_hanlder(bvm*)
 * @note Observability hook
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @return (???)
 */
BERRY_API bctypefunc be_get_ctype_func_hanlder(bvm *vm);

/**
 * @fn int be_loadbuffer(bvm*, const char*, const char*, size_t)
 * @note code load API
 * @brief load a piece of source code from the buffer and compile it into bytecode, in global context
 *
 * f the compilation is successful, be_loadbuffer will compile the source code into a Berry function and place
 * it on the top of the virtual stack. If the compilation encounters an error, be_loadbuffer will return
 * an error value of type berrorcode (Section errorcode), and if possible, will store the
 * specific error message string at the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param name string, which is usually used to mark the source of the source code
 * @param buffer buffer for storing the source code
 * @param length length of the buffer
 * @return (???)
 */
BERRY_API int be_loadbuffer(bvm *vm, const char *name, const char *buffer, size_t length);

/**
 * @fn int be_loadbuffer_local(bvm*, const char*, const char*, size_t)
 * @note code load API
 * @brief load a piece of source code from the buffer and compile it into bytecode, in local or global context
 *
 * f the compilation is successful, be_loadbuffer will compile the source code into a Berry function and place
 * it on the top of the virtual stack. If the compilation encounters an error, be_loadbuffer will return
 * an error value of type berrorcode (Section errorcode), and if possible, will store the
 * specific error message string at the top of the virtual stack.
 *
 * @param vm virtual machine instance
 * @param name string, which is usually used to mark the source of the source code
 * @param buffer buffer for storing the source code
 * @param length length of the buffer
 * @return (???)
 */
BERRY_API int be_loadbuffer_local(bvm *vm, const char *name, const char *buffer, size_t length, bbool islocal);

/**
 * @fn int be_loadmode(bvm *vm, const char *name, bbool islocal)
 * @note code load API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 * @param islocal
 * @return (???)
 */
BERRY_API int be_loadmode(bvm *vm, const char *name, bbool islocal);

/**
 * @fn int be_loadlib(bvm*, const char*)
 * @note code load API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param path
 * @return (???)
 */
BERRY_API int be_loadlib(bvm *vm, const char *path);

/**
 * @fn int be_savecode(bvm*, const char*)
 * @note code load API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param name
 * @return (???)
 */
BERRY_API int be_savecode(bvm *vm, const char *name);

/**
 * @fn void be_module_path(bvm*)
 * @note module path list API
 * @brief (???)
 *
 * @param vm virtual machine instance
 */
BERRY_API void be_module_path(bvm *vm);

/**
 * @fn void be_module_path_set(bvm*, const char*)
 * @note module path list API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param path
 */
BERRY_API void be_module_path_set(bvm *vm, const char *path);

/**
 * @fn void* be_pushbytes(bvm *vm, const void *buf, size_t len)
 * @note bytes operation
 * @brief Push a bytes() buffer
 *
 * @param vm virtual machine instance
 * @param buf starting at position
 * @param len size
 */
BERRY_API void* be_pushbytes(bvm *vm, const void *buf, size_t len);

/**
 * @fn const void* be_tobytes(bvm *vm, int index, size_t *len)
 * @note bytes operation
 * @brief return virtual stack as a bytes buffer
 *
 * Get the value of the index position of index from the virtual stack and return it as a bytes buffer.
 * The pointer of the buffer is returned, and the size is stored in *len (unless len is NULL).
 * This function works only for instances of the bytes class, or returns NULL.
 *
 * @param vm virtual machine instance
 * @param index index from the virtual stac
 * @param len size
 */
BERRY_API const void* be_tobytes(bvm *vm, int index, size_t *len);

/**
 * @fn void be_sethook(bvm*, const char*)
 * @note debug API
 * @brief (???)
 *
 * @param vm virtual machine instance
 * @param mask
 */
BERRY_API void be_sethook(bvm *vm, const char *mask);

/**
 * @fn void be_setntvhook(bvm*, bntvhook, void*, int)
 * @note debug API
 * @brief  (???)
 *
 * @param vm virtual machine instance
 * @param hook
 * @param data
 * @param mask
 */
BERRY_API void be_setntvhook(bvm *vm, bntvhook hook, void *data, int mask);

/**
 * @fn void be_writebuffer(const char*, size_t)
 * @note basic character IO API
 * @brief implement on berry_port.c
 *
 * Output a piece of data to the standard output device, the parameter
 * `buffer` is the first address of the output data block, and `length` is
 * the length of the output data block. This function outputs to the
 * `stdout` file by default. Inside the interpreter, this function is
 * usually used as a character stream output, not a binary stream.
 *
 * @param buffer
 * @param length
 */
BERRY_API void be_writebuffer(const char *buffer, size_t length);

/**
 * @fn char* be_readstring(char *buffer, size_t size)
 * @note basic character IO API
 * @brief implement on berry_port.c
 *
 * Input a piece of data from the standard input device, and read at most
 * one row of data each time this function is called. The parameter
 * `buffer` is the data buffer passed in by the caller, and the capacity of
 * the buffer is `size`. This function will stop reading and return when
 * the buffer capacity is used up, otherwise it will return when a newline
 * character or end of file character is read. If the function executes
 * successfully, it will directly use the `buffer` parameter as the return
 * value, otherwise it will return `NULL`.
 *
 * This function will add the read line breaks to the read data, and each
 * time the `be_readstring` function is called, it will continue to read
 * from the current position. This function is only called in the
 * implementation of the native function `input`, and the `be_readstring`
 * function may not be implemented when it is not necessary.
 *
 * @param buffer
 * @param size
 * @return (???)
 */
BERRY_API char* be_readstring(char *buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
