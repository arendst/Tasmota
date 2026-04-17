# Berry C Extension Reference

Compact FFI reference for writing C extensions that interact with the Berry VM.

## VM Lifecycle
```c
#include "berry.h"
bvm *vm = be_vm_new();                         // create VM + stdlib
be_loadstring(vm, "print('Hi')");              // compile string (macro)
be_loadbuffer(vm, "name", buf, len);           // compile buffer
be_loadfile(vm, "file.be");                    // compile file
be_pcall(vm, 0);                               // protected call
be_call(vm, argc);                             // unprotected call
be_vm_delete(vm);                              // destroy VM
```
Macros: `be_loadstring(vm, s)` = `be_loadbuffer(vm, "string", s, strlen(s))`. `be_dostring(vm, s)` = load + pcall.

Error codes: `BE_OK` `BE_EXIT` `BE_MALLOC_FAIL` `BE_EXCEPTION` `BE_SYNTAX_ERROR` `BE_EXEC_ERROR` `BE_IO_ERROR`

## Virtual Stack

Index `1` = first arg (bottom), `-1` = top. `be_top(vm)` = arg count.

```c
int be_top(bvm *vm);
void be_pop(bvm *vm, int n);
void be_remove(bvm *vm, int index);
int be_absindex(bvm *vm, int index);
void be_moveto(bvm *vm, int from, int to);
void be_stack_require(bvm *vm, int count);     // ensure free slots
```

## Type Checking
```c
be_isnil  be_isbool  be_isint  be_isreal  be_isnumber  be_isstring
be_isfunction  be_isclosure  be_isntvclos  be_isproto
be_isclass  be_isinstance  be_ismodule
be_islist  be_ismap  be_iscomptr  be_iscomobj  be_isbytes
be_isderived  be_ismapinstance  be_islistinstance
```
All: `bbool fn(bvm *vm, int index)`. `be_isfunction` = closure | native func | native closure | ctype_func. `be_isnumber` = int | real.

## Getting Values
```c
bint be_toint(bvm *vm, int index);             // raw cast, does NOT call toint()
breal be_toreal(bvm *vm, int index);
bbool be_tobool(bvm *vm, int index);           // converts per Berry rules, calls tobool()
const char* be_tostring(bvm *vm, int index);   // converts in place, calls tostring()
void* be_tocomptr(bvm *vm, int index);
int be_toindex(bvm *vm, int index);            // like toint but returns C int
const void* be_tobytes(bvm *vm, int index, size_t *len);  // bytes buffer + length
const char* be_typename(bvm *vm, int index);   // "int", "string", "instance", ...
const char* be_classname(bvm *vm, int index);  // class name or nil
int be_strlen(bvm *vm, int index);             // faster than strlen for Berry strings
```

## Pushing Values
```c
void be_pushnil(bvm *vm);
void be_pushbool(bvm *vm, int b);
void be_pushint(bvm *vm, bint i);
void be_pushreal(bvm *vm, breal r);
void be_pushstring(bvm *vm, const char *str);
void be_pushnstring(bvm *vm, const char *str, size_t n);
const char* be_pushfstring(bvm *vm, const char *fmt, ...);  // %d %f %g %s %c %p %%
void be_pushvalue(bvm *vm, int index);         // copy from stack
void be_pushcomptr(bvm *vm, void *ptr);
void* be_pushbytes(bvm *vm, const void *buf, size_t len);
void be_pushntvfunction(bvm *vm, bntvfunc f);
void be_pushntvclosure(bvm *vm, bntvfunc f, int nupvals);
void be_pushclass(bvm *vm, const char *name, const bnfuncinfo *lib);
void be_pushntvclass(bvm *vm, const struct bclass *c);
void be_pushclosure(bvm *vm, void *cl);
void* be_pushbuffer(bvm *vm, size_t size);     // raw buffer
```

## Native Functions

Signature: `int func(bvm *vm)`. Return via macros:
```c
be_return(vm);          // return top of stack
be_return_nil(vm);      // return nil
```

```c
static int my_add(bvm *vm) {
    if (be_top(vm) >= 2 && be_isint(vm, 1) && be_isint(vm, 2)) {
        be_pushint(vm, be_toint(vm, 1) + be_toint(vm, 2));
        be_return(vm);
    }
    be_return_nil(vm);
}
be_regfunc(vm, "myadd", my_add);   // register as global
```

## Exceptions
```c
be_raise(bvm *vm, const char *type, const char *msg);  // noreturn
be_stop_iteration(bvm *vm);                             // raise stop_iteration
be_getexcept(bvm *vm, int code);
be_dumpexcept(bvm *vm);
```

## Member / Global / Module Access
```c
bbool be_getmember(bvm *vm, int index, const char *k);  // push inst.k
bbool be_setmember(bvm *vm, int index, const char *k);  // inst.k = top
bbool be_getmethod(bvm *vm, int index, const char *k);   // push method
bbool be_getglobal(bvm *vm, const char *name);
void be_setglobal(bvm *vm, const char *name);
bbool be_getbuiltin(bvm *vm, const char *name);
void be_getsuper(bvm *vm, int index);
bbool be_setsuper(bvm *vm, int index);
bbool be_classof(bvm *vm, int index);          // push class of instance
bbool be_isderived(bvm *vm, int index);        // check inheritance
bbool be_getmodule(bvm *vm, const char *k);
```

## Container Operations (internal list/map values)
```c
void be_newlist(bvm *vm);                      // push new BE_LIST
void be_newmap(bvm *vm);                       // push new BE_MAP
void be_newmodule(bvm *vm);
void be_newobject(bvm *vm, const char *name);  // instantiate + push instance and .p
bbool be_getindex(bvm *vm, int index);         // push container[top]
bbool be_setindex(bvm *vm, int index);         // container[top-1] = top
int be_data_size(bvm *vm, int index);          // element count (-1 if not container)
void be_data_push(bvm *vm, int index);         // append top to list
bbool be_data_insert(bvm *vm, int index);      // insert key(top-1)/value(top)
bbool be_data_remove(bvm *vm, int index);      // remove by key(top)
void be_data_resize(bvm *vm, int index);       // resize list to top
void be_data_reverse(bvm *vm, int index);
bbool be_data_merge(bvm *vm, int index);
```

**Example: build a list instance:**
```c
be_getbuiltin(vm, "list");  // push list class
be_newlist(vm);             // push internal list
be_pushint(vm, 100);
be_data_push(vm, -2);      // append 100
be_pop(vm, 1);
be_call(vm, 1);            // call constructor
be_pop(vm, 1);
be_return(vm);
```

## Iterators (internal containers)
```c
bbool be_pushiter(bvm *vm, int index);
int be_iter_next(bvm *vm, int index);      // 0=fail, 1=list(push val), 2=map(push key+val)
bbool be_iter_hasnext(bvm *vm, int index);
```

## Native Closures (Upvalues)
```c
be_pushntvclosure(vm, func, nupvals);      // create with N upvalue slots
void be_getupval(bvm *vm, int index, int pos);
bbool be_setupval(bvm *vm, int index, int pos);
```

## String Operations
```c
void be_strconcat(bvm *vm, int index);     // str[index] = str[index] + str[top]
const char* be_toescape(bvm *vm, int index, int mode);  // escape string
```

## Reference Stack (cycle detection)
```c
bbool be_refcontains(bvm *vm, int index);
void be_refpush(bvm *vm, int index);
void be_refpop(bvm *vm);
```

## Debug Hooks
```c
be_sethook(vm, "lcr");                    // l=line, c=call, r=return
be_setntvhook(vm, hook_fn, data, mask);   // BE_HOOK_LINE|BE_HOOK_CALL|BE_HOOK_RET
```

## Observability
```c
be_set_obs_hook(vm, hook);                 // events: BE_OBS_PCALL_ERROR, BE_OBS_GC_START, etc.
```

## Key Types
```c
typedef struct bvm bvm;
typedef int (*bntvfunc)(bvm*);
typedef long long bint;                    // configurable via BE_INTGER_TYPE
typedef double breal;                      // float if BE_USE_SINGLE_FLOAT
typedef struct { const char *name; bntvfunc function; } bnfuncinfo;
```

## Compile-Time Construction (coc tool)

The `coc` tool generates C headers for constant objects from `@const_object_info_begin` blocks in source comments.

```c
/* @const_object_info_begin
type name (attributes) {
    members
}
@const_object_info_end */
#include "../generate/be_fixed_name.h"
```

**Types:** `class`, `module`, `map`, `vartab`

**Attributes:** `scope: local|global`, `name: ClassName`, `depend: BE_USE_*`, `strings: weak`

**Member value types:**
```
var                        # auto-incrementing index (instance variable slot)
func(c_func)               # native method (receives self as arg 1)
static_func(c_func)        # static method (no self, BE_STATIC flag)
closure(solidified_fn)     # pre-compiled bytecode
static_closure(fn)         # static pre-compiled bytecode
nil()  int(N)  real(N)  bool(b)  str(s)  comptr(ptr)
class(be_class_xxx)  module(be_module_xxx)
map(m)  list(l)  simple_instance(i)  bytes_instance(b)
```

**Class example:**
```c
/* @const_object_info_begin
class be_class_map (scope: global, name: map) {
    .p, var
    init, func(m_init)
    tostring, func(m_tostring)
    size, func(m_size)
}
@const_object_info_end */
#include "../generate/be_fixed_be_class_map.h"
```

**Module example:**
```c
/* @const_object_info_begin
module math (scope: global, depend: BE_USE_MATH_MODULE) {
    sin, func(m_sin)
    pi, real(M_PI)
}
@const_object_info_end */
#include "../generate/be_fixed_math.h"
```

**Builtin table example:**
```c
/* @const_object_info_begin
vartab m_builtin (scope: local) {
    assert, func(l_assert)
    print, func(l_print)
    list, class(be_class_list)
}
@const_object_info_end */
```

**Build:** `tools/coc/coc -o generate src default -c default/berry_conf.h`

## Runtime Module Registration (non-precompiled)

```c
// Module via attr table
be_native_module_attr_table(mymod) {
    be_native_module_function("func1", m_func1),
    be_native_module_int("const1", 42),
    be_native_module_real("pi", 3.14159),
    be_native_module_str("name", "hello"),
};
be_define_native_module(mymod, NULL);

// Class via bnfuncinfo
static const bnfuncinfo members[] = {
    { ".p", NULL },            // instance variable
    { "init", m_init },
    { "method", m_method },
    { NULL, NULL }
};
be_regclass(vm, "MyClass", members);
```

Module attr macros: `be_native_module_nil`, `_int`, `_real`, `_bool`, `_function`, `_str`, `_module`.

## Module Registration Table

Modules are registered in `default/be_modtab.c`:
```c
be_extern_native_module(mymod);
// Add to be_module_table[] array
```

Classes are registered in `be_class_table`:
```c
be_extern_native_class(my_class);
// Add &be_native_class(my_class) to be_class_table array
```

## Naming Conventions
- `l_*` — library/global functions
- `m_*` — class methods
- `be_class_*` — class objects
- `be_native_module_*` — module objects
- `.p` or `.data` — hidden instance data member (convention for wrapping internal types)
