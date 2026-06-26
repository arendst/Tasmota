/********************************************************************
** Copyright (c) 2018-2026 Guan Wenliang
** This file is part of the Berry default interpreter.
** skiars@qq.com, https://github.com/Skiars/berry
** See Copyright Notice in the LICENSE file or at
** https://github.com/Skiars/berry/blob/master/LICENSE
********************************************************************/
#ifndef BERRY_CONF_H
#define BERRY_CONF_H

#include <assert.h>

#ifdef COMPILE_BERRY_LIB
  #include "my_user_config.h"
  #include "include/tasmota_configurations.h"
#endif

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
#ifdef TASMOTA
#define BE_INTGER_TYPE                  1           // use long int = uint32_t
#else
#define BE_INTGER_TYPE                  0
#endif

/* Macro: BE_USE_SINGLE_FLOAT
 * Select floating point precision.
 * Use double-precision floating-point numbers when the value
 * is 0 (default), otherwise use single-precision floating-point
 * numbers.
 * Default: 0
 **/
#define BE_USE_SINGLE_FLOAT             1           // use `float` not `double`

/* Macro: BE_BYTES_MAX_SIZE
 * Maximum size in bytes of a `bytes()` object.
 * Putting too much pressure on the memory allocator can do
 * harm, so we limit the maximum size.
 * Default: 32kb
 **/
#define BE_BYTES_MAX_SIZE               (32*1024)   /* 32 kb default value */

/* Macro: BE_USE_PRECOMPILED_OBJECT
 * Use precompiled objects to avoid creating these objects at
 * runtime. Enable this macro can greatly optimize RAM usage.
 * Default: 1
 **/
#define BE_USE_PRECOMPILED_OBJECT       1

/* Macro: BE_USE_COMPACT_KTAB
 * Store each function's constant table (ktab) as a structure-of-arrays:
 * a payload-word array (`union bvaldata`) plus a parallel type-byte
 * array, instead of an array of full `bvalue` (which wastes the padding
 * of the 4-byte type field). On 32-bit targets (single-precision float,
 * 32-bit int) a payload word is 4 bytes, so this saves ~37% of the flash
 * occupied by constant tables of solidified code (executed in place from
 * flash on ESP32). Constant tables are read-only at runtime, so constants
 * are materialized into a scratch value on access; live registers are
 * unchanged. The representation is portable: on 64-bit hosts the payload
 * word is 8 bytes and the feature still works (and can be unit-tested),
 * but the flash win is only meaningful on the 32-bit target.
 * Default: 0 (disabled)
 **/
#ifndef BE_USE_COMPACT_KTAB
#define BE_USE_COMPACT_KTAB             1
#endif

/* Macro: BE_USE_COMPACT_MAP
 * Store the nodes of *constant* (solidified/flash) maps in a packed
 * 12-byte layout instead of the regular 16-byte `bmapnode`. The value's
 * type byte is folded into the spare bits of the key word (`next` is
 * shrunk to 16 bits), removing the 3 padding bytes of the value's type
 * field. On 32-bit targets this saves 25% of the flash used by solidified
 * class/module member maps (executed in place from flash on ESP32).
 *
 * Only read-only (const) maps use the packed layout; mutable runtime maps
 * keep the regular 16-byte layout untouched, so all map mutation paths and
 * their `bvalue*` return contract are unchanged. Const-map reads decode the
 * packed node into a scratch on access. The discriminator at runtime is
 * `gc_isconst(map)`. Limited to 65535 slots per const map (ample).
 *
 * The representation is portable: on 64-bit hosts the node is larger (the
 * payload word is 8 bytes) and the feature still works and is testable, but
 * the flash win is only meaningful on the 32-bit target.
 *
 * NOTE: changing this flag changes the layout of every const map emitted by
 * `coc` and by solidify, so the generated headers must be regenerated
 * (`make prebuild`) and `berry_port/berry_conf.py` must match.
 * Default: 0 (disabled)
 **/
#ifndef BE_USE_COMPACT_MAP
#define BE_USE_COMPACT_MAP              1
#endif

/* Macro: BE_DEBUG_SOURCE_FILE
 * Indicate if each function remembers its source file name
 * 0: do not keep the file name (saves 4 bytes per function)
 * 1: keep the source file name
 * Default: 1
 **/
#define BE_DEBUG_SOURCE_FILE            0

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

/* Macro: BE_USE_PERF_COUNTERS
 * Use the obshook function to report low-level actions.
 * Default: 0
 **/
#define BE_USE_PERF_COUNTERS            1

/* Macro: BE_VM_OBSERVABILITY_SAMPLING
 * If BE_USE_PERF_COUNTERS == 1
 * then the observability hook is called regularly in the VM loop
 * allowing to stop infinite loops or too-long running code.
 * The value is a power of 2.
 * Default: 20 - which translates to 2^20 or ~1 million instructions
 **/
#define BE_VM_OBSERVABILITY_SAMPLING    20

/* Macro: BE_STACK_TOTAL_MAX
 * Set the maximum total stack size.
 * Default: 20000
 **/
#define BE_STACK_TOTAL_MAX              8000

/* Macro: BE_STACK_FREE_MIN
 * Set the minimum free count of the stack. The stack idles will
 * be checked when a function is called, and the stack will be
 * expanded if the number of free is less than BE_STACK_FREE_MIN.
 * Default: 10
 **/
#define BE_STACK_FREE_MIN               20

/* Macro: BE_STACK_START
 * Set the starting size of the stack at VM creation.
 * Default: 50
 **/
#define BE_STACK_START                  100

/* Macro: BE_CONST_SEARCH_SIZE
 * Constants in function are limited to 255. However the compiler
 * will look for a maximum of pre-existing constants to avoid
 * performance degradation. This may cause the number of constants
 * to be higher than required.
 * Increase is you need to solidify functions.
 * Default: 50
 **/
#define BE_CONST_SEARCH_SIZE            250

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
#ifdef TASMOTA
#define BE_USE_FILE_SYSTEM              0
#else
#define BE_USE_FILE_SYSTEM              1
#endif

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

/* Macro: BE_USE_PREPROCESSOR
 * Enable the preprocessor for conditional compilation
 * (#if/#else/#elif/#endif/#define/#undef) and translatable
 * string expressions ("text"#MACRO). Set to 0 to exclude
 * all preprocessor code from compilation.
 * Default: 1
 **/
#ifdef TASMOTA
#define BE_USE_PREPROCESSOR             0   /* Berry preprocessor not included in Tasmota firmwares */
#else
#define BE_USE_PREPROCESSOR             1   /* Berry preprocessor only used in standalone for solidification */
#endif

/* Macro: BE_PREPROC_MAX_DEPTH
 * Maximum nesting depth for preprocessor conditional blocks.
 * Only used when BE_USE_PREPROCESSOR is enabled.
 * Default: 8
 **/
#define BE_PREPROC_MAX_DEPTH            8

/* Macro: BE_MAX_PARSER_DEPTH
 * Hard limit on parser recursion depth (nested expressions and blocks).
 * Each level costs ~hundreds of bytes of native C stack, so this protects
 * pathological source from overflowing the C stack at compile time.
 * Stored in a bbyte, so values above 255 are clamped.
 * Default: 25 (safe on ESP32 with an 8 KB task stack; well above any
 * realistic hand-written Berry code).
 **/
#define BE_MAX_PARSER_DEPTH             25

/* Macro: BE_USE_DEBUG_HOOK
 * Berry debug hook switch.
 * Default: 0
 **/
#define BE_USE_DEBUG_HOOK               0

/* Macro: BE_USE_DEBUG_GC
 * Enable GC debug mode. This causes an actual gc after each
 * allocation. It's much slower and should not be used
 * in production code.
 * Default: 0
 **/
#define BE_USE_DEBUG_GC                  0

/* Macro: BE_USE_DEBUG_STACK
 * Enable Stack Resize debug mode. At each function call
 * the stack is reallocated at a different memory location
 * and the previous location is cleared with toxic data.
 * Default: 0
 **/
#define BE_USE_DEBUG_STACK               0

/* Macro: BE_USE_MEM_ALIGNED
 * Some embedded processors have special memory areas
 * with read/write constraints of being aligned to 32 bits boundaries.
 * This options tries to move such memory areas to this region.
 * Default: 0
 **/
#ifdef TASMOTA
#define BE_USE_MEM_ALIGNED               1
#else
#define BE_USE_MEM_ALIGNED               0
#endif

/* Macro: BE_USE_XXX_MODULE
 * These macros control whether the related module is compiled.
 * When they are true, they will enable related modules. At this
 * point you can use the import statement to import the module.
 * They will not compile related modules when they are false.
 **/

#ifdef TASMOTA
  #define BE_USE_STRING_MODULE            1
  #define BE_USE_JSON_MODULE              1
  #define BE_USE_MATH_MODULE              1
  #define BE_USE_TIME_MODULE              0
  #define BE_USE_OS_MODULE                0
  #define BE_USE_GLOBAL_MODULE            1
  #define BE_USE_SYS_MODULE               1
  #define BE_USE_DEBUG_MODULE             0
  #define BE_USE_GC_MODULE                1
  #define BE_USE_SOLIDIFY_MODULE          0
  #define BE_USE_INTROSPECT_MODULE        1
  #define BE_USE_STRICT_MODULE            1
#else
  #define BE_USE_STRING_MODULE            1
  #define BE_USE_JSON_MODULE              1
  #define BE_USE_MATH_MODULE              1
  #define BE_USE_TIME_MODULE              1
  #define BE_USE_OS_MODULE                1
  #define BE_USE_GLOBAL_MODULE            1
  #define BE_USE_SYS_MODULE               1
  #define BE_USE_DEBUG_MODULE             1
  #define BE_USE_GC_MODULE                1
  #define BE_USE_SOLIDIFY_MODULE          1
  #define BE_USE_INTROSPECT_MODULE        1
  #define BE_USE_STRICT_MODULE            1
#endif

#if defined(USE_BERRY_DEBUG) || !defined(TASMOTA)
  #undef BE_USE_DEBUG_MODULE
  #undef BE_USE_SOLIDIFY_MODULE
  #define BE_USE_DEBUG_MODULE             1
  #define BE_USE_SOLIDIFY_MODULE          1
  #define BE_MAPPING_ENABLE_INPUT_VALIDATION   1    // input validation for lv_mapping
  #define USE_MATTER_VERBOSE              1         // enable names for actions and attributes in logs
#endif // USE_BERRY_DEBUG

/* Macro: BE_EXPLICIT_XXX
 * If these macros are defined, the corresponding function will
 * use the version defined by these macros. These macro definitions
 * are not required.
 * The default is to use the functions in the standard library.
 **/
#ifdef __cplusplus
extern "C" {
#endif
  extern void *berry_malloc(size_t size);
  extern void  berry_free(void *ptr);
  extern void *berry_realloc(void *ptr, size_t size);
  extern void *berry_malloc32(size_t size);
#ifdef __cplusplus
}
#endif
#ifdef USE_BERRY_PSRAM
  #define BE_EXPLICIT_MALLOC              berry_malloc
  #define BE_EXPLICIT_FREE                berry_free
  #define BE_EXPLICIT_REALLOC             berry_realloc
#else
  #define BE_EXPLICIT_MALLOC              malloc
  #define BE_EXPLICIT_FREE                free
  #define BE_EXPLICIT_REALLOC             realloc
#endif // USE_BERRY_PSRAM

#define BE_EXPLICIT_ABORT               abort
#define BE_EXPLICIT_EXIT                exit
// #define BE_EXPLICIT_MALLOC              malloc
// #define BE_EXPLICIT_FREE                free
// #define BE_EXPLICIT_REALLOC             realloc

/* Macro: be_assert
 * Berry debug assertion. Only enabled when BE_DEBUG is active.
 * Default: use the assert() function of the standard library.
 **/
#define be_assert(expr)                 assert(expr)

/* Tasmota debug specific */
#ifdef USE_BERRY_DEBUG
  #undef BE_DEBUG_RUNTIME_INFO
  #define BE_DEBUG_RUNTIME_INFO 1 /* record line information in 32 bits to be places in IRAM */
  #undef BE_DEBUG
  #define BE_DEBUG 1
  #undef be_assert
  #define be_assert(expr)                                                        \
      ((expr)                                                                \
      ? (0)                                                \
      : serial_debug("BRY: ASSERT '%s', %s - %i\n", #expr, __FILE__, __LINE__))
  #ifdef USE_LVGL
    #undef BE_STACK_START
    #define BE_STACK_START                  200
  #endif // USE_LVGL
  #ifdef USE_MATTER_DEVICE
    #undef BE_STACK_START
    #define BE_STACK_START                  256
  #endif // USE_MATTER_DEVICE
#endif // USE_BERRY_DEBUG

#endif
