"""
Berry configuration constants.
Mirrors: default/berry_conf.h

This module replicates all BE_* configuration macros from the C
berry_conf.h header as Python constants. Values match the default
build configuration used by the Tasmota toolchain.
"""

# /********************************************************************
# ** Copyright (c) 2018-2020 Guan Wenliang
# ** This file is part of the Berry default interpreter.
# ** skiars@qq.com, https://github.com/Skiars/berry
# ** See Copyright Notice in the LICENSE file or at
# ** https://github.com/Skiars/berry/blob/master/LICENSE
# ********************************************************************/

# ---------------------------------------------------------------------------
# Debug switch
# ---------------------------------------------------------------------------
# /* Macro: BE_DEBUG
#  * Berry interpreter debug switch.
#  * Default: 0
#  **/
# #define BE_DEBUG                        0
BE_DEBUG = 1

# ---------------------------------------------------------------------------
# Integer and float type selection
# ---------------------------------------------------------------------------
# /* Macro: BE_INTGER_TYPE
#  * Select integer length.
#  * 0=int, 1=long, 2=long long
#  * Default: 2
#  */
# #define BE_INTGER_TYPE                  0
BE_INTGER_TYPE = 0

# /* Macro: BE_USE_SINGLE_FLOAT
#  * Select floating point precision.
#  * 0=double (default), 1=single
#  * Default: 0
#  **/
# #define BE_USE_SINGLE_FLOAT             1
BE_USE_SINGLE_FLOAT = 1

# ---------------------------------------------------------------------------
# Bytes max size
# ---------------------------------------------------------------------------
# /* Macro: BE_BYTES_MAX_SIZE
#  * Maximum size in bytes of a `bytes()` object.
#  * Default: 32kb
#  **/
# #define BE_BYTES_MAX_SIZE               (32*1024)
BE_BYTES_MAX_SIZE = 32 * 1024  # 32 kb

# ---------------------------------------------------------------------------
# Debug information
# ---------------------------------------------------------------------------
# /* Macro: BE_DEBUG_SOURCE_FILE
#  * Indicate if each function remembers its source file name.
#  * 0: do not keep the file name (saves 4 bytes per function)
#  * 1: keep the source file name
#  * Default: 1
#  **/
# #define BE_DEBUG_SOURCE_FILE            0
BE_DEBUG_SOURCE_FILE = 0

# /* Macro: BE_DEBUG_RUNTIME_INFO
#  * Set runtime error debugging information.
#  * 0: unable to output source file and line number at runtime.
#  * 1: output source file and line number information at runtime.
#  * 2: the information use uint16_t type (save space).
#  * Default: 1
#  **/
# #define BE_DEBUG_RUNTIME_INFO           1
BE_DEBUG_RUNTIME_INFO = 1

# /* Macro: BE_DEBUG_VAR_INFO
#  * Set variable debugging tracking information.
#  * 0: disable variable debugging tracking information at runtime.
#  * 1: enable variable debugging tracking information at runtime.
#  * Default: 1
#  **/
# #define BE_DEBUG_VAR_INFO               0
BE_DEBUG_VAR_INFO = 0

# ---------------------------------------------------------------------------
# Performance counters and observability
# ---------------------------------------------------------------------------
# /* Macro: BE_USE_PERF_COUNTERS
#  * Use the obshook function to report low-level actions.
#  * Default: 1
#  **/
# #define BE_USE_PERF_COUNTERS            0
BE_USE_PERF_COUNTERS = 0

# /* Macro: BE_VM_OBSERVABILITY_SAMPLING
#  * If BE_USE_PERF_COUNTERS == 1, the observability hook is called
#  * regularly in the VM loop. The value is a power of 2.
#  * Default: 20 (2^20 = ~1 million instructions)
#  **/
# #define BE_VM_OBSERVABILITY_SAMPLING    20
BE_VM_OBSERVABILITY_SAMPLING = 20

# ---------------------------------------------------------------------------
# Stack configuration
# ---------------------------------------------------------------------------
# /* Macro: BE_STACK_TOTAL_MAX
#  * Set the maximum total stack size.
#  * Default: 20000
#  **/
# #define BE_STACK_TOTAL_MAX              20000
BE_STACK_TOTAL_MAX = 20000

# /* Macro: BE_STACK_FREE_MIN
#  * Set the minimum free count of the stack.
#  * Default: 10
#  **/
# #define BE_STACK_FREE_MIN               10
BE_STACK_FREE_MIN = 20

# /* Macro: BE_STACK_START
#  * Set the starting size of the stack at VM creation.
#  * Default: 50
#  **/
# #define BE_STACK_START                  50
BE_STACK_START = 100

# ---------------------------------------------------------------------------
# Constant search size
# ---------------------------------------------------------------------------
# /* Macro: BE_CONST_SEARCH_SIZE
#  * Constants in function are limited to 255. The compiler will look
#  * for a maximum of pre-existing constants to avoid performance
#  * degradation.
#  * Default: 50
#  **/
# #define BE_CONST_SEARCH_SIZE            50
BE_CONST_SEARCH_SIZE = 250

# ---------------------------------------------------------------------------
# String hash cache
# ---------------------------------------------------------------------------
# /* Macro: BE_USE_STR_HASH_CACHE
#  * The short string will hold the hash value when true.
#  * Default: 0
#  **/
# #define BE_USE_STR_HASH_CACHE           0
BE_USE_STR_HASH_CACHE = 0

# ---------------------------------------------------------------------------
# Feature switches
# ---------------------------------------------------------------------------
# /* Macro: BE_USE_FILE_SYSTEM
#  * Default: 1
#  **/
# #define BE_USE_FILE_SYSTEM              1
BE_USE_FILE_SYSTEM = 1

# /* Macro: BE_USE_SCRIPT_COMPILER
#  * Enable compiler.
#  * Default: 1
#  **/
# #define BE_USE_SCRIPT_COMPILER          1
BE_USE_SCRIPT_COMPILER = 1

# /* Macro: BE_USE_BYTECODE_SAVER
#  * Enable save bytecode to file.
#  * Default: 1
#  **/
# #define BE_USE_BYTECODE_SAVER           1
BE_USE_BYTECODE_SAVER = 1

# /* Macro: BE_USE_BYTECODE_LOADER
#  * Enable load bytecode from file.
#  * Default: 1
#  **/
# #define BE_USE_BYTECODE_LOADER          1
BE_USE_BYTECODE_LOADER = 1

# /* Macro: BE_USE_SHARED_LIB
#  * Enable shared library.
#  * Default: 1
#  **/
# #define BE_USE_SHARED_LIB               1
BE_USE_SHARED_LIB = 1

# /* Macro: BE_USE_OVERLOAD_HASH
#  * Allows instances to overload hash methods for Map.
#  * Default: 1
#  **/
# #define BE_USE_OVERLOAD_HASH            1
BE_USE_OVERLOAD_HASH = 1

# /* Macro: BE_USE_PREPROCESSOR
#  * Enable the preprocessor for conditional compilation.
#  * Default: 1
#  **/
# #define BE_USE_PREPROCESSOR             1
BE_USE_PREPROCESSOR = 1

# /* Macro: BE_PREPROC_MAX_DEPTH
#  * Maximum nesting depth for preprocessor conditional blocks.
#  * Default: 8
#  **/
# #define BE_PREPROC_MAX_DEPTH            8
BE_PREPROC_MAX_DEPTH = 8

# /* Macro: BE_USE_DEBUG_HOOK
#  * Berry debug hook switch.
#  * Default: 0
#  **/
# #define BE_USE_DEBUG_HOOK               0
BE_USE_DEBUG_HOOK = 0

# /* Macro: BE_USE_DEBUG_GC
#  * Enable GC debug mode.
#  * Default: 0
#  **/
# #define BE_USE_DEBUG_GC                  0
BE_USE_DEBUG_GC = 0

# /* Macro: BE_USE_DEBUG_STACK
#  * Enable Stack Resize debug mode.
#  * Default: 0
#  **/
# #define BE_USE_DEBUG_STACK               0
BE_USE_DEBUG_STACK = 0

# /* Macro: BE_USE_MEM_ALIGNED
#  * Aligned memory for embedded processors.
#  * Default: 0
#  **/
# #define BE_USE_MEM_ALIGNED               0
BE_USE_MEM_ALIGNED = 0

# ---------------------------------------------------------------------------
# Module enable/disable flags
# ---------------------------------------------------------------------------
# #define BE_USE_STRING_MODULE            1
BE_USE_STRING_MODULE = 1

# #define BE_USE_JSON_MODULE              1
BE_USE_JSON_MODULE = 1

# #define BE_USE_MATH_MODULE              1
BE_USE_MATH_MODULE = 1

# #define BE_USE_TIME_MODULE              1
BE_USE_TIME_MODULE = 1

# #define BE_USE_OS_MODULE                1
BE_USE_OS_MODULE = 1

# Berry re module (regex) — uses Python native re instead of re1.5
BE_USE_RE_MODULE = 1

# #define BE_USE_GLOBAL_MODULE            1
BE_USE_GLOBAL_MODULE = 1

# #define BE_USE_SYS_MODULE               1
BE_USE_SYS_MODULE = 1

# #define BE_USE_DEBUG_MODULE             1
BE_USE_DEBUG_MODULE = 1

# #define BE_USE_GC_MODULE                1
BE_USE_GC_MODULE = 1

# #define BE_USE_SOLIDIFY_MODULE          1
BE_USE_SOLIDIFY_MODULE = 1

# #define BE_USE_INTROSPECT_MODULE        1
BE_USE_INTROSPECT_MODULE = 1

# #define BE_USE_STRICT_MODULE            1
BE_USE_STRICT_MODULE = 1

# ---------------------------------------------------------------------------
# Explicit function overrides (informational — Python uses its own allocator)
# ---------------------------------------------------------------------------
# #define BE_EXPLICIT_ABORT               abort
# #define BE_EXPLICIT_EXIT                exit
# #define BE_EXPLICIT_MALLOC              malloc
# #define BE_EXPLICIT_FREE                free
# #define BE_EXPLICIT_REALLOC             realloc
