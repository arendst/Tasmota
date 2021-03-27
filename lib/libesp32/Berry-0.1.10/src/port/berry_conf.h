/********************************************************************
** Copyright (c) 2018-2020 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BERRY_CONF_H
#define BERRY_CONF_H

#include <assert.h>

/* Macro: BE_DEBUG
 * Berry interpreter debug switch.
 * Default: 0
 **/
#ifndef BE_DEBUG
#define BE_DEBUG                        0
#endif

/* Macro: BE_LONGLONG_INT
 * Select integer length.
 * If the value is 0, use an integer of type int, use a long
 * integer type when the value is 1, and use a long long integer
 * type when the value is 2.
 * Default: 2
 */
#define BE_INTGER_TYPE                  1           // use long int = uint32_t

/* Macro: BE_USE_SINGLE_FLOAT
 * Select floating point precision.
 * Use double-precision floating-point numbers when the value
 * is 0 (default), otherwise use single-precision floating-point
 * numbers.
 * Default: 0
 **/
#define BE_USE_SINGLE_FLOAT             1           // use `float` not `double`

/* Macro: BE_USE_PRECOMPILED_OBJECT
 * Use precompiled objects to avoid creating these objects at
 * runtime. Enable this macro can greatly optimize RAM usage.
 * Default: 1
//  **/
// #ifdef ESP8266
// #define BE_USE_PRECOMPILED_OBJECT       0
// #else
#define BE_USE_PRECOMPILED_OBJECT       1           // will enable later when stabilized
// #endif

/* Macro: BE_DEBUG_RUNTIME_INFO
 * Set runtime error debugging information.
 * 0: unable to output source file and line number at runtime.
 * 1: output source file and line number information at runtime.
 * 2: the information use uint16_t type (save space).
 * Default: 1
 **/
#define BE_DEBUG_RUNTIME_INFO           0

/* Macro: BE_DEBUG_VAR_INFO
 * Set variable debugging tracking information.
 * 0: disable variable debugging tracking information at runtime.
 * 1: enable variable debugging tracking information at runtime.
 * Default: 1
 **/
#define BE_DEBUG_VAR_INFO               0

/* Macro: BE_USE_OBSERVABILITY_HOOK
 * Use the obshook function to report low-level actions.
 * Default: 0
 **/
#define BE_USE_OBSERVABILITY_HOOK       1

/* Macro: BE_STACK_TOTAL_MAX
 * Set the maximum total stack size.
 * Default: 20000
 **/
#define BE_STACK_TOTAL_MAX              4000

/* Macro: BE_STACK_FREE_MIN
 * Set the minimum free count of the stack. The stack idles will
 * be checked when a function is called, and the stack will be
 * expanded if the number of free is less than BE_STACK_FREE_MIN.
 * Default: 10
 **/
#define BE_STACK_FREE_MIN               10

/* Macro: BE_STACK_FREE_MIN
 * The short string will hold the hash value when the value is
 * true. It may be faster but requires more RAM.
 * Default: 0
 **/
#define BE_USE_STR_HASH_CACHE           0

/* Macro: BE_USE_FILE_SYSTEM
 * The file system interface will be used when this macro is true
 * or when using the OS module. Otherwise the file system interface
 * will not be used.
 * Default: 0
 **/
#define BE_USE_FILE_SYSTEM              0

/* Macro: BE_USE_SCRIPT_COMPILER
 * Enable compiler when BE_USE_SCRIPT_COMPILER is not 0, otherwise
 * disable the compiler.
 * Default: 1
 **/
#define BE_USE_SCRIPT_COMPILER          1

/* Macro: BE_USE_BYTECODE_SAVER
 * Enable save bytecode to file when BE_USE_BYTECODE_SAVER is not 0,
 * otherwise disable the feature.
 * Default: 1
 **/
#define BE_USE_BYTECODE_SAVER           1

/* Macro: BE_USE_BYTECODE_LOADER
 * Enable load bytecode from file when BE_USE_BYTECODE_LOADER is not 0,
 * otherwise disable the feature.
 * Default: 1
 **/
#define BE_USE_BYTECODE_LOADER          1

/* Macro: BE_USE_SHARED_LIB
 * Enable shared library  when BE_USE_SHARED_LIB is not 0,
 * otherwise disable the feature.
 * Default: 1
 **/
#define BE_USE_SHARED_LIB               0

/* Macro: BE_USE_OVERLOAD_HASH
 * Allows instances to overload hash methods for use in the
 * built-in Map class. Disable this feature to crop the code
 * size.
 * Default: 1
 **/
#define BE_USE_OVERLOAD_HASH            1

/* Macro: BE_USE_DEBUG_HOOK
 * Berry debug hook switch.
 * Default: 0
 **/
#define BE_USE_DEBUG_HOOK               0

/* Macro: BE_USE_XXX_MODULE
 * These macros control whether the related module is compiled.
 * When they are true, they will enable related modules. At this
 * point you can use the import statement to import the module.
 * They will not compile related modules when they are false.
 **/
#define BE_USE_STRING_MODULE            1
#define BE_USE_JSON_MODULE              1
#define BE_USE_MATH_MODULE              1
#define BE_USE_TIME_MODULE              0
#define BE_USE_OS_MODULE                0
#define BE_USE_SYS_MODULE               0
#define BE_USE_DEBUG_MODULE             1
#define BE_USE_GC_MODULE                1
#define BE_USE_SOLIDIFY_MODULE          1

/* Macro: BE_EXPLICIT_XXX
 * If these macros are defined, the corresponding function will
 * use the version defined by these macros. These macro definitions
 * are not required.
 * The default is to use the functions in the standard library.
 **/
#ifdef USE_BERRY_PSRAM
#ifdef __cplusplus
extern "C" {
#endif
  extern void *berry_malloc(uint32_t size);
  extern void *berry_realloc(void *ptr, size_t size);
#ifdef __cplusplus
}
#endif
  #define BE_EXPLICIT_MALLOC              special_malloc
  #define BE_EXPLICIT_REALLOC             special_realloc
#else
  #define BE_EXPLICIT_MALLOC              malloc
  #define BE_EXPLICIT_REALLOC             realloc
#endif // USE_BERRY_PSRAM

#define BE_EXPLICIT_ABORT               abort
#define BE_EXPLICIT_EXIT                exit
// #define BE_EXPLICIT_MALLOC              malloc
#define BE_EXPLICIT_FREE                free
// #define BE_EXPLICIT_REALLOC             realloc

/* Macro: be_assert
 * Berry debug assertion. Only enabled when BE_DEBUG is active.
 * Default: use the assert() function of the standard library.
 **/
#define be_assert(expr)                 assert(expr)

#endif
