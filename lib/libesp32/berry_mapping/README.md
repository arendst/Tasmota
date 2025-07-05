# Berry Mapping to C Functions

A sophisticated library providing seamless integration between Berry scripts and native C functions with minimal effort and optimal code size.

Originally designed for LVGL mapping to Berry (handling hundreds of C functions), this library has evolved into a generalized C-Berry integration mechanism suitable for embedded systems.

## Table of Contents

- [Quick Start](#quick-start)
- [Architecture Overview](#architecture-overview)
- [Type System](#type-system)
- [Function Mapping](#function-mapping)
- [Callbacks](#callbacks)
- [Pre-compiled CType Functions](#pre-compiled-ctype-functions)
- [Configuration](#configuration)
- [Best Practices](#best-practices)
- [Troubleshooting](#troubleshooting)
- [Examples](#examples)

## Quick Start

### Prerequisites

**⚠️ Platform Requirement**: This library requires that `int` and `void*` pointers have the same size (all 32-bit or all 64-bit). This is standard on ESP32 and most embedded 32-bit systems.

### Basic Example

Let's create a simple module with three functions:

**Step 1: Define your C functions**
```c
/* Sum two integers */
int addint(int a, int b) {
    return a + b;
}

/* Convert Fahrenheit to Celsius */
float f2c(float f) {
    return (f - 32.0f) / 1.8f;
}

/* Convert integer to yes/no string */
const char* yesno(int v) {
    return v ? "yes" : "no";
}
```

**Step 2: Create Berry wrapper functions**
```c
#include "be_mapping.h"

int f_addint(bvm *vm) {
    return be_call_c_func(vm, (void*) &addint, "i", "ii");
}

int f_f2c(bvm *vm) {
    return be_call_c_func(vm, (void*) &f2c, "f", "f");
}

int f_yesno(bvm *vm) {
    return be_call_c_func(vm, (void*) &yesno, "s", "i");
}
```

**Step 3: Register the module**
```c
#include "be_constobj.h"

/* @const_object_info_begin
module math_utils (scope: global) {
    addint, func(f_addint)
    f2c, func(f_f2c)
    yesno, func(f_yesno)
}
@const_object_info_end */
#include "../generate/be_fixed_math_utils.h"
```

**Step 4: Use in Berry**
```berry
import math_utils

print(math_utils.addint(5, 3))        # Output: 8
print(math_utils.f2c(100.0))          # Output: 37.777779
print(math_utils.yesno(1))            # Output: "yes"
```

## Architecture Overview

The Berry Mapping library operates through several key components:

```
Berry Script ──→ Type Conversion ──→ C Function ──→ Result Conversion ──→ Berry Return
     ↑                                                                         ↓
Callback System ←──────────────── Parameter Validation ←─────────────────────┘
```

### Core Components

- **Type Conversion Engine**: Automatic conversion between Berry and C types
- **Function Call Orchestration**: Parameter validation and C function invocation
- **Callback Management**: Dynamic C callback generation from Berry functions
- **Memory Management**: Efficient resource handling for embedded systems

## Type System

### Berry to C Type Conversion

| Berry Type | C Type | Conversion Method | Notes |
|------------|--------|-------------------|-------|
| `int` | `intptr_t` | Direct value copy | Auto-converts to `real` if needed |
| `real` | `breal` (float/double) | Union reinterpretation | Size must match `intptr_t` |
| `bool` | `intptr_t` | 0 (false) or 1 (true) | Direct boolean conversion |
| `string` | `const char*` | Pointer reference | Read-only, null-terminated |
| `nil` | `NULL` | Null pointer | Safe null representation |
| `comptr` | `void*` | Direct pointer | Native pointer pass-through |
| `instance` | `void*` | Via `_p` or `.p` member | Recursive extraction |
| `bytes` | `uint8_t*` + size | Buffer + length | Includes size information |

### Type Validation Syntax

The type system uses a compact string notation for validation:

#### Basic Types
- `i` - Integer (`int`)
- `f` - Real/Float (`real`)
- `b` - Boolean (`bool`)
- `s` - String (`string`)
- `c` - Common pointer (`comptr`)
- `.` - Any type (no validation)

#### Special Types
- `-` - Skip argument (ignore, useful for `self`)
- `@` - Berry VM pointer (virtual parameter)
- `~` - Length of previous `bytes()` buffer
- `[...]` - Optional parameters (in brackets)
- `(class)` - Instance of specific class
- `^type^` - Callback with type specification

#### Examples
```c
"ii"           // Two integers
"ifs"          // Integer, float, string
"-ib"          // Skip first arg, then int and bool
"ii[s]"        // Two ints, optional string
"(lv_obj)i"    // lv_obj instance and integer
"^button_cb^"  // Button callback function
```

## Function Mapping

### Core Function: `be_call_c_func()`

```c
int be_call_c_func(bvm *vm, const void *func, const char *return_type, const char *arg_type);
```

**Parameters:**
- `vm` - Berry virtual machine instance
- `func` - Pointer to C function
- `return_type` - How to convert C return value to Berry
- `arg_type` - Parameter validation and conversion specification

### Return Type Specifications

| Return Type | Berry Result | Description |
|-------------|--------------|-------------|
| `""` (empty) | `nil` | No return value (void) |
| `i` | `int` | Integer return |
| `f` | `real` | Float/real return |
| `b` | `bool` | Boolean (non-zero = true) |
| `s` | `string` | String (copied) |
| `$` | `string` | String (freed after copy) |
| `c` | `comptr` | Pointer return |
| `&` | `bytes()` | Buffer with size |
| `class_name` | `instance` | New instance with pointer |
| `+var` | Constructor | Store in instance variable (non-null) |
| `=var` | Constructor | Store in instance variable (null OK) |

### Advanced Parameter Handling

#### Virtual Parameters
```c
// Function signature: int process_buffer(void *data, size_t len)
// Berry mapping: "i", "~"  (buffer length automatically added)
be_call_c_func(vm, &process_buffer, "i", "~");
```

#### Class Instance Parameters
```c
// Expects lv_obj instance, extracts _p member
be_call_c_func(vm, &lv_obj_set_width, "", "(lv_obj)i");
```

#### Callback Parameters
```c
// Converts Berry function to C callback
be_call_c_func(vm, &set_event_handler, "", "^event_cb^");
```

## Callbacks

The callback system enables C code to call Berry functions through generated C function pointers.

### Basic Callback Usage

```berry
import cb

def my_callback(arg1, arg2, arg3, arg4, arg5)
    print("Callback called with:", arg1, arg2, arg3, arg4, arg5)
    return 42
end

var c_callback = cb.gen_cb(my_callback)
print("C callback address:", c_callback)
```

### Callback Limitations

- **Maximum 20 simultaneous callbacks** (configurable via `BE_MAX_CB`)
- **5 parameters maximum** per callback
- **All parameters passed as integers** (use `introspect.toptr()` for pointers)
- **Integer return value** only

### Advanced Callback Handling

#### Custom Callback Handlers
```berry
import cb

def my_handler(func, type_name, self_obj)
    # Custom callback processing
    if type_name == "button_event"
        return cb.gen_cb(func)  # Use default for this type
    end
    return nil  # Let other handlers try
end

cb.add_handler(my_handler)
```

#### Working with Pointer Data
```berry
def buffer_callback(ptr_as_int, size)
    import introspect
    var ptr = introspect.toptr(ptr_as_int)
    var buffer = bytes(ptr, size)
    print("Buffer contents:", buffer)
end
```

## Pre-compiled CType Functions

For better performance and smaller code size, you can pre-compile function definitions:

### CType Function Declaration

```c
// C function
int calculate_area(int width, int height) {
    return width * height;
}

// Pre-compiled declaration
BE_FUNC_CTYPE_DECLARE(calculate_area, "i", "ii")

// Module definition
/* @const_object_info_begin
module geometry (scope: global) {
    area, ctype_func(calculate_area, "i", "ii")
}
@const_object_info_end */
#include "be_fixed_geometry.h"
```

### CType Handler Registration

```c
#include "berry.h"
#include "be_mapping.h"

void initialize_berry_vm(void) {
    bvm *vm = be_vm_new();
    be_set_ctype_func_handler(vm, be_call_ctype_func);
    // ... rest of initialization
}
```

## Configuration

### Compile-time Configuration

```c
// Maximum number of simultaneous callbacks
#define BE_MAX_CB 20

// Enable input validation (recommended)
#define BE_MAPPING_ENABLE_INPUT_VALIDATION 1

// String length limits
#define BE_MAPPING_MAX_NAME_LENGTH 256
#define BE_MAPPING_MAX_MODULE_NAME_LENGTH 64
#define BE_MAPPING_MAX_MEMBER_NAME_LENGTH 192

// Function parameter limits
#define BE_MAPPING_MAX_FUNCTION_ARGS 8
```

### Runtime Configuration

```c
// Disable validation for performance (not recommended)
#undef BE_MAPPING_ENABLE_INPUT_VALIDATION
#define BE_MAPPING_ENABLE_INPUT_VALIDATION 0
```

## Best Practices

### Memory Management
- **Use stack allocation** where possible to minimize heap usage
- **Limit string lengths** to prevent memory exhaustion
- **Clean up callbacks** when VM is destroyed
- **Avoid deep recursion** in type conversion

### Performance Optimization
- **Use CType functions** for frequently called functions
- **Minimize parameter count** (max 8 parameters)
- **Prefer simple types** over complex conversions
- **Cache callback addresses** when possible

### Error Handling
- **Always validate inputs** in production code
- **Use appropriate return types** for error signaling
- **Handle null pointers** gracefully
- **Provide meaningful error messages**

### Embedded Systems
- **Monitor stack usage** with large parameter lists
- **Limit callback count** based on available memory
- **Use fixed-size buffers** instead of dynamic allocation
- **Profile memory usage** in your specific application

## Troubleshooting

### Common Issues

#### "Too few arguments to function 'be_isfunction'"
```c
// WRONG: Direct bvalue usage
if (!be_isfunction(&callback_value)) { ... }

// CORRECT: Check type field directly
if ((callback_value.type & 0x1F) != BE_FUNCTION) { ... }
```

#### "Stack buffer overflow"
```c
// WRONG: Variable length array
char buffer[strlen(input)+1];

// CORRECT: Fixed size buffer with validation
char buffer[MAX_NAME_LENGTH];
if (strlen(input) >= MAX_NAME_LENGTH) {
    be_raise(vm, "value_error", "Input too long");
}
```

#### "Callback not working"
- Check that callback is registered before use
- Verify callback hasn't been garbage collected
- Ensure VM is still valid when callback is invoked
- Check parameter count and types

#### "Type conversion errors"
- Verify parameter type string syntax
- Check that C function signature matches type specification
- Ensure pointer sizes are consistent (32-bit vs 64-bit)
- Validate instance objects have `_p` or `.p` members

### Debugging Tips

1. **Enable input validation** during development
2. **Use simple test cases** to isolate issues
3. **Check Berry stack state** before and after calls
4. **Verify C function signatures** match type specifications
5. **Test with minimal examples** before complex integration

## Examples

### Example 1: GPIO Control
```c
// C functions
void gpio_set_pin(int pin, int value) {
    // Hardware-specific GPIO implementation
}

int gpio_get_pin(int pin) {
    // Hardware-specific GPIO implementation
    return 0; // placeholder
}

// Berry wrappers
int f_gpio_set(bvm *vm) {
    return be_call_c_func(vm, &gpio_set_pin, "", "ii");
}

int f_gpio_get(bvm *vm) {
    return be_call_c_func(vm, &gpio_get_pin, "i", "i");
}

// Module registration
/* @const_object_info_begin
module gpio (scope: global) {
    set_pin, func(f_gpio_set)
    get_pin, func(f_gpio_get)
}
@const_object_info_end */
```

### Example 2: String Processing
```c
// C function with string manipulation
char* process_string(const char* input, int mode) {
    // Process string and return allocated result
    char* result = malloc(strlen(input) + 10);
    sprintf(result, "processed_%d_%s", mode, input);
    return result;
}

// Berry wrapper (note '$' return type for malloc'd string)
int f_process_string(bvm *vm) {
    return be_call_c_func(vm, &process_string, "$", "si");
}
```

### Example 3: Callback Integration
```c
// C function that accepts callback
typedef void (*event_callback_t)(int event_type, void* data);
void register_event_handler(event_callback_t callback) {
    // Register callback with system
}

// Berry wrapper
int f_register_handler(bvm *vm) {
    return be_call_c_func(vm, &register_event_handler, "", "^event_cb^");
}
```

### Example 4: Buffer Operations
```c
// C function working with buffers
int process_buffer(uint8_t* data, size_t length) {
    int sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return sum;
}

// Berry wrapper (note '~' for automatic length parameter)
int f_process_buffer(bvm *vm) {
    return be_call_c_func(vm, &process_buffer, "i", "~");
}
```

```berry
# Usage in Berry
var data = bytes("Hello World")
var result = process_buffer(data)  # Length automatically passed
print("Buffer sum:", result)
```

---

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please ensure:
- Code follows existing style conventions
- New features include documentation and examples
- Changes maintain backward compatibility where possible
- Embedded system constraints are considered

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
    be_set_ctype_func_handler(berry.vm, be_call_ctype_func);  /* register the ctype function handler */
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
