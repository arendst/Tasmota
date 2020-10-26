/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 0

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 0

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Disable floating-point API */
#define OP_DISABLE_FLOAT_API 1

/* Enable assertions in code */
/* #undef OP_ENABLE_ASSERTIONS */

/* Enable HTTP support */
/* #undef OP_ENABLE_HTTP */

/* Enable fixed-point calculation */
#define OP_FIXED_POINT 1

/* Enable use of clock_gettime function */
/* #undef OP_HAVE_CLOCK_GETTIME */

/* Enable use of lrintf function */
/* #undef OP_HAVE_LRINTF */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "opus@xiph.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "opusfile"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "opusfile 0.12"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "opusfile"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.12"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define this if the compiler supports __attribute__((
   ifelse([visibility("default")], , [visibility_default],
   [visibility("default")]) )) */
#define SUPPORT_ATTRIBUTE_VISIBILITY_DEFAULT 1

/* Define this if the compiler supports the -fvisibility flag */
#define SUPPORT_FLAG_VISIBILITY 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Enable large inode numbers on Mac OS X 10.5.  */
#ifndef _DARWIN_USE_64_BIT_INODE
# define _DARWIN_USE_64_BIT_INODE 1
#endif

/* Number of bits in a file offset, on hosts where this is settable. */
/* #undef _FILE_OFFSET_BITS */

/* Define for large files, on AIX-style hosts. */
/* #undef _LARGE_FILES */

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* We need at least WindowsXP for getaddrinfo/freeaddrinfo */
/* #undef _WIN32_WINNT */
