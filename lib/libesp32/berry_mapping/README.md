# Berry mapping to C functions, aka "ctype functions"

This library provides a simplified and semi-automated way to map C functions to Berry functions with minimal effort and minimal code size.

This library was originally designed for LVGL mapping to Berry, which implies mapping of hundreds of C function. It was later extended as a generalized `C` mapping mechanism.

Warning: for this library to work, the `C` stack needs to have the same size for `int` and `void*` pointers (all 32 bits or all 64 bits). Otherwise the layout of the calling stack is too complex to handle. On ESP32 and most embedded 32 bits systems, this should not be an issue.

## Quick example

Let's create a simple module skeleton with 3 functions:

- `addint`: simple function that adds two ints
- `ftoc`: converts Fahrenheit real to Celsius real
- `yesno` that transforms an int into a constant string

Below we have the three pure `C` functions that we want to map:

``` C
/* sum two ints */
int addint(int a, int b)
{
    return a + b;
}

/* returns 'yes' or 'no' depending on v being true */
const char* yesno(int v)
{
    return v ? "yes" : "no";
}

/* fahrenheit to celsius, forcing to float to avoid using double libs */
const float f2c(float f)
{
    return (f - 32.0f) / 1.8f;
}
```

The following mapping is done with this lib:

``` C
#include "be_mapping.c"

int f_addint(bvm *vm) {
    return be_call_c_func(vm, (void*) &addint, "i", "ii");
}

int f_ftoc(bvm *vm) {
    return be_call_c_func(vm, (void*) &ftoc, "f", "f");
}

int f_yesno(bvm *vm) {
    return be_call_c_func(vm, (void*) &yesno, "s", "i");
}
```

Now we add a typical module stub declaring the three functions in a module named `demo`.

``` C
#include "be_constobj.h"

/* @const_object_info_begin
module test (scope: global) {
    addint, func(f_addint)
    ftoc, func(f_ftoc)
    yesno, func(f_yesno)
}
@const_object_info_end */
#include "../generate/be_fixed_demo.h"
```

## Argument types

The core function is `be_call_c_func()` and does the conversion from Berry argument to C argument, with optional type checking.

When calling a `C` function, here are the steps applied:

1. Automatically convert Berry typed argument to implicit `C` types (see below)
2. (optional) Apply type checking and abort the call if arguments are invalid
3. Call the `C` function
4. Apply conversion from `C` result to Berry type


### Conversion from Berry types to C types

`be_call_c_func()` does introspection on Berry types for each argument and applies automatic conversion

Berry type|converted to C type
:---|:---
`int`|`intptr_t` (i.e. `int` large enough to hold a pointer.<br><br>If a type `r` (real) is expected, the value is silently converted to `breal`
`real`|`breal` (either `float` or `double`)
`bool`|`intptr_t` with value `1` if `true` or `0` if `false`
`string`|`const char*` C string `NULL` terminated (cannot be modified)
`nil`|`void*` with value `NULL`
`comptr`|`void*` native pointer
`instance` of `bytes`|In case of an instance of type `bytes` or any subclass, the argument is converted to the pointer to the internal buffer `_buffer`. This is equivalent to a `C` `struct`
`instance` of any other class|In case of an instance, the engine search for an instance variable `_p` or `.p`, and applies the conversion recursively.<br>This is handy when an instance contains a pointer to a native C structure as `comptr`.

### Argument type checking

This phase is optional and checks that there is the right number of arguments and the right types, according to the type definition described as a string.

Note: callbacks need an explicit type definition to be handled correctly

Argument type|Berry type expected
:---|:---
`i`|`int`
`f`|`real` (if arg is `int` it is silently converted to `real`)
`b`|`bool` (no implicit conversion, use `bool()` to force bool type)
`s`|`string`
`c`|`comptr` (native pointer)
`.`|**any** - no type checking for this argument
`-`|**skip** - skip this argument (handy to discard the `self` implicit argument for methods)
`@`|**Berry VM** (virtual attribute) - adds a pointer to the Berry VM - works only as first argument
`~`|send the length of the previous bytes() buffer (or raise an exception if no length known)
`(<class>)`|`instance` deriving from `<class>` (i.e. of this class or any subclass
`^<callback_type>^`|`function` which is converted to a `C` callback by calling `cb.make_cb()`. The optional `callback_type` string is passed as second argument to `cb.make_cb()` and Berrt `arg #1` (typically `self`) is passed as 3rd argument<br>See below for callbacks
`[<...>]`|arguments in brackets are optional (note: the closing bracket is not strictly necessary but makes it more readable)

Example:

`-ib(lv_obj)` means: 1/ skip arg1, 2/ arg2 must be `int`, 3/ arg3 must be `bool`, 4/ arg4 must be an instance of `lv_obj` or subclass and its attribute `_p` or `.p` is passed. The final `C` function is passed 3 arguments.
`ii[.]` means: the first two arguments must be `int` and there can be an optional third argument of any type.

### Return type

The return type defines how the `C` result (intptr_t) is converted to any other Berry type.

Return type definition|Berry return type
:---|:---
`''` (empty string)|`nil` - no return value, equivalent to `C` `void`
`i`|`int`
`f`|(float) `real` (warning `intptr_t` and `breal` must be of same size)
`s`|`string` - `const char*` is converted to `C` string, a copy of the string is made so the original string can be modified
`b`|`bool` - any non-zero value is converted to `true`
`c`|`comptr` - native pointer
`<class>` or `<module.class>`|Instanciate a new instance for this class, pass the return value as `comptr` (native pointer), and pass a second argument as `comptr(-1)` as a marker for an instance cretion (to distinguish from an simple encapsulation of a pointer)
`+<varable>` of `=<variable>`|Used in class constructor `init()` functions, the return value is passed as `comptr` and stored in the instance variable with name `<variable>`. The variables are typically `_p` or `.p`. `=` prefix indicates that a `NULL` value is accepted, while the `+` prefix raises an exception if the function returns `NULL`. This is typically used when encapsulating a pointer to a `C++` object that is not supposed to be `NULL`.
`&`|`bytes()` object, pointer to buffer returned, and size passed with an additional (size_t*) argument after arguments

## Pre-compiled ctype functions

It is possible to pre-compile Berry modules or classes that reference directly a ctype function definition.

Example:

``` C
/* return type is "i", arg type is "ifs" for int-float-string
int my_ext_func(int x, float s, const char* s) {
    /* do whatever */
    return 0;
}

/* @const_object_info_begin
module my_module (scope: global) {
    my_func, ctype_func(my_ext_func, "i", "ifs")
}
@const_object_info_end */
#include "be_fixed_my_module.h"
```

With this scheme, the definition is passed automatically to the ctype handler.
It relies on an extensibility scheme of Berry.

You need to register the ctype function handler at the launch of the Berry VM:

``` C
#include "berry.h"
#include "be_mapping.h"

void berry_launch(boid)
{
    bvm *vm = be_vm_new();      /* Construct a VM */
    be_set_ctype_func_hanlder(berry.vm, be_call_ctype_func);  /* register the ctype function handler */
}
```

## Callbacks

The library introduces a new module `cb` used to create `C` callbacks that are mapped to Berry functions (native functions, native closures, Berry functions or closures).

Due to the nature of `C` callbacks, each callback must point to a different `C` address. For this reason, the library pre-defines `20` callback addresses of stubs. This should be enough for most use-case; increasing this limit requires to define additional stubs and increases slightly the code size.

The low-level `cb.gen_cb()` takes a Berry callable and returns a `C` callback address. The callback supports up to 5 `C` parameters. For each call, there are 5 Berry arguments passed as `int` converted from `intptr_t`. Each argument can be converted to a `comptr` with `introspect.toptr()` or converted to a `bytes()` structure.

```
> def inc(x) return x+1 end
> import cb
> print(cb.gen_cb(f))
<ptr: 0x40148c18>
```

It is easy to convert an argument to a `bytes()` or `cbytes()` object. In such case, you need to create a `bytes()` object with 2 arguments: first a `comptr` pointer, second the buffer size (note: the buffer will have a fixed size). The `bytes()` buffer is mapped to the `C` structure in memory and can be read or written as long as the address is valid.

```
> # let's assume the callback receives as first argument a pointer to a buffer of 8 bytes
> def get_buf(a)
    import introspect
    var b = bytes(introspect.toptr(a), 8)
    print(b)
  end
> var c = cb.gen_cb(get_buf)

> # let's try manually the conversion with a dummy address
> import introspect
> get_buf(introspect.toptr(0x3ffb2340))
bytes('BD9613807023FB3F')
```

However some callbacks need more information to reuse the same callback in different locations. The `C` mapper will actually call `cb.make_cb(closure, name, self)` and let modules the opportunity to register specific callback handlers.

You can register a hanlder with `cb.add_handler(handler)` where `handler` receives the 3 following arguments `handler(cb:function, name:string, obj:instance)`. The handler must return a `comptr` if it has sucessfully allocated a callback, or return `nil` if it ignores this callback (based on its name for example). `gen_cb()` is called eventually if no handler handled it.
