Compilation Environment
=======================

Tock aims to provide a build environment that is easy for application authors
to integrate with. Check out the [examples](../examples) folder for
sample applications. The Tock userland build system will automatically build
with all of the correct flags and generate TABs for all supported Tock
architectures.

To leverage the Tock build system, you must:

  1. Set `TOCK_USERLAND_BASE_DIR` to the path to the Tock userland.
  2. `include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk`.

This `include` should be the _last_ line of the Makefile for most applications.

In addition, you must specify the sources for your application:

  - `C_SRCS`: A list of C files to compile.
  - `CXX_SRCS`: A list of C++ files to compile.
  - `AS_SRCS`: A list of assembly files to compile.
  - `EXTERN_LIBS`: A list of directories for libraries [**compiled for Tock**](#compiling-libraries-for-tock).

## Customizing the build

### Flags

The build system respects all of the standard `CFLAGS` (C only), `CXXFLAGS`
(C++ only), `CPPFLAGS` (C and C++), `ASFLAGS` (asm only).

By default, if you run something like `make CPPFLAGS=-Og`, make will use _only_
the flags specified on the command line, but that means that Tock would lose all
of its PIC-related flags. For that reason, Tock specifies all variables using
make's [override directive](https://www.gnu.org/software/make/manual/html_node/Override-Directive.html).

If you wish to set additional flags in your application Makefiles, you must also
use `override`, or they will be ignored. That is, in your Makefile you must write
`override CPPFLAGS += -Og` rather than just `CPPFLAGS += -Og`.

If you are adding supplemental flags, you can put them anywhere. If you want to
override Tock defaults, you'll need to place these _after_ the `include` directive
in your Makefile.

### Application configuration

Several Tock-specific variables are also useful:

  - `STACK_SIZE`: The minimum application stack size.
  - `APP_HEAP_SIZE`: The minimum heap size for your application.
  - `KERNEL_HEAP_SIZE`: The minimum grant size for your application.
  - `PACKAGE_NAME`: The name for your application. Defaults to current folder.

### Advanced

If you want to see a verbose build that prints all the commands as run, simply
run `make V=1`.

The build system is broken across three files in the `libtock-c` repo:

  - `Configuration.mk`: Sets most variables used.
  - `Helpers.mk`: Generic rules and functions to support the build.
  - `AppMakefile.mk`: Includes the above files and supplies build recipes.

Applications wishing to define their own build rules can include only the
`Configuration.mk` file to ensure all of the flags needed for Tock applications
are included.

## Compiling Libraries for Tock

Libraries used by Tock need all of the same position-independent build flags as
the final application. As Tock builds for all supported architectures by
default, libraries should include images for each supported Tock architecture.

### Let Tock do the work: TockLibrary.mk

As the Tock build requirements (PIC, multiple architectures) are fairly complex,
Tock provides a Makefile that will ensure everything is set up correctly and
generate build rules for you. An example Makefile for `libexample`:

> **libexample/Makefile**
```make
# Base definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := libexample

# Careful! Must be a path that resolves correctly **from where make is invoked**
#
# If you are only ever compiling a standalone library, then it's fine to simply set
$(LIBNAME)_DIR := .
#
# If you will be asking applications to rebuild this library (see the development
# section below), then you'll need to ensure that this directory is still correct
# when invoked from inside the application folder.
#
# Tock accomplishes this for in-tree libraries by having all makefiles
# conditionally set the TOCK_USERLAND_BASE_DIR variable, so that there
# is a common relative path everywhere.
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# Grab all relevant source files. You can list them directly:
$(LIBNAME)_SRCS :=                                      \
    $($LIBNAME)_DIR)\libexample.c                       \
    $($LIBNAME)_DIR)\libexample_helper.c                \
    $($LIBNAME)_DIR)\subfolders_are_fine\otherfile.c

# Or let make find them automatically:
$(LIBNAME)_SRCS  :=                                     \
    $(wildcard $($(LIBNAME)_DIR)/*.c)                   \
    $(wildcard $($(LIBNAME)_DIR)/*.cxx)                 \ # or .cpp or .cc
    $(wildcard $($(LIBNAME)_DIR)/*.s)

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
```

> __Note! `:=` is NOT the same as `=` in make. You must use `:=`.__

### Developing (building) libraries concurrently with applications

When developing a library, often it's useful to have the library rebuild automatically
as part of the application build. Assuming that your library is using `TockLibrary.mk`,
you can simply include the library's Makefile in your application's Makefile:

```make
include $(TOCK_USERLAND_BASE_DIR)/libexample/Makefile
include ../../AppMakefile.mk
```

**Example:** We don't have an in-tree example of a single app that rebuilds
a dedicated library in the Tock repository, but libtock is effectively treated
this way as its Makefile is
[included by AppMakefile.mk](../AppMakefile.mk#L17).

### Pre-built libraries

You can also include pre-built libraries, but recall that Tock supports multiple
architectures, which means you must supply a pre-built image for each.

Pre-built libraries must adhere to the following folder structure:

```
For the library "example"

libexample/                <-- Folder name must match library name
├── Makefile.app           <-- Optional additional rules to include when building apps
├── build
│   ├── cortex-m0          <-- Architecture names match gcc's -mcpu= flag
│   │   └── libexample.a   <-- Library name must match folder name
│   └── cortex-m4
│       └── libexample.a   <-- Library name must match folder name
│
└── root_header.h          <-- The root directory will always be added to include path
└── include                <-- An include/ directory will be added too if it exists
    └── example.h
```

To include a pre-built library, add the _path_ to the root folder to the
variable `EXTERN_LIBS` in your application Makefile, e.g.
`EXTERN_LIBS += ../../libexample`.

**Example:** In the Tock repository, lua53
[ships a pre-built archive](../lua53/build/cortex-m4).

### Manually including libraries

To manually include an external library, add the library to each `LIBS_$(arch)`
(i.e. `LIBS_cortex-m0`) variable. You can include header paths using the
standard search mechanisms (i.e. `CPPFLAGS += -I<path>`).
