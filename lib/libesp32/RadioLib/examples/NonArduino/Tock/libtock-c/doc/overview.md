Overview
========

The bulk of Tock applications are written in C.

## Entry Point

Applications written in C that compile against libtock should define a `main`
method with the following signature:

```c
int main(void);
```

Applications **should** return 0 from `main`. Returning non-zero is undefined and the
behavior may change in future versions of `libtock`.
Today, `main` is called from `_start` and includes an implicit `while()` loop:

```c
void _start(void* text_start, void* mem_start, void* memory_len, void* app_heap_break) {
  main();
  while (1) {
    yield();
  }
}
```

Applications should set up a series of event subscriptions in their `main`
method and then return.

## Stack and Heap

Applications can specify their required stack and heap sizes by defining the
make variables `STACK_SIZE` and `APP_HEAP_SIZE`, which default to 2K and 1K
respectively as of this writing.

`libtock` will set the stack pointer during startup. To allow each application
to set its own stack size, the linker script expects a symbol `STACK_SIZE` to
be defined. The Tock build system will define this symbol during linking, using
the make variable `STACK_SIZE`. A consequence of this technique is that
changing the stack size requires that any source file also be touched so that
the app will re-link.

## Libraries

Application code does not need to stand alone, libraries are available that can
be utilized!

### Newlib
Application code written in C has access to most of the [C standard
library](https://en.wikipedia.org/wiki/C_standard_library) which is implemented
by [Newlib](https://en.wikipedia.org/wiki/Newlib). Newlib is focused on
providing capabilities for embedded systems. It provides interfaces such as
`printf`, `malloc`, and `memcpy`. Most, but not all features of the standard
library are available to applications. The built configuration of Newlib is
specified in [build.sh](../userland/newlib/build.sh).

### libtock
In order to interact with the Tock kernel, application code can use the
`libtock` library. The majority of `libtock` are wrappers for interacting
with Tock drivers through system calls. They provide the user a meaningful
function name and arguments and then internally translate these into a
`command`, `subscribe`, etc. Where it makes sense, the libraries also provide
a synchronous interface to a driver using an internal callback and `yield_for`
(example:
[`tmp006_read_sync`](https://github.com/tock/tock/blob/master/userland/libtock/tmp006.c#L19))

`libtock` also provides the startup code for applications
([`crt0.c`](../userland/libtock/crt0.c)),
an implementation for the system calls
([`tock.c`](../userland/libtock/tock.c)),
and pin definitions for platforms.

### libc++
Provides support for C++ apps. See `examples/cxx_hello`.

### libnrfserialization
Provides a pre-compiled library for using the Nordic nRF serialization library
for writing BLE apps.

### lua53
Provides support for running a lua runtime as a Tock app. See
`examples/lua-hello`.

## Style & Format

We try to keep a consistent style in mainline userland code. For C/C++, we use
[uncrustify](https://github.com/uncrustify/uncrustify). High level:

  - Two space character indents.
  - Braces on the same line.
  - Spaces around most operators.

For details, see the [configuration](../userland/tools/uncrustify).

Travis will automatically check formatting. You can format code locally using
`make format`, or check the whole codebase with
[format_all.sh](../userland/examples/format_all.sh). Formatting will overwrite
files when it runs.
