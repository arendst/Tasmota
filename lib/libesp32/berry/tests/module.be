# test for monkey patching of modules

import string
import introspect

var string_orig = string

introspect.setmodule("string", 42)
import string
assert(string == 42)

# set back original value
introspect.setmodule("string", string_orig)
import string
assert(type(string) == 'module')
assert(str(string) == '<module: string>')

#
# demo, how to monkey patch string with a new function `foo()` returning `bar`
#
import string
import introspect
var string_orig = string            # keep a copy of the original string module
var string_alt = module('string')   # create a new module

# function `super()` is a closure to capture the original value of string
string_alt.super = def()
    return string_orig
end

# function `member()` is a clousre to capture the original value of string module
string_alt.member = def(k)
    import introspect
    return introspect.get(string_orig, k, true)
end

string_alt.foo = def() return 'bar' end

# replace the entry for module "string", from now on each `import string` will use `string_alt`
introspect.setmodule("string", string_alt)
import string

# test the new string module
assert(string.tolower('abCD') == 'abcd')
assert(string.foo() == 'bar')


# --- additional tests for be_module.c coverage ---

# module() builtin: with and without name
var m1 = module('mymod')
var m2 = module()
assert(type(m1) == 'module')
assert(type(m2) == 'module')
assert(str(m1) == '<module: mymod>')
import introspect
assert(introspect.name(m1) == 'mymod')
assert(introspect.name(m2) == nil)

# module member read/write and update of existing key
var m = module('rw')
m.x = 10
assert(m.x == 10)
m.x = 20
assert(m.x == 20)
m.y = 'hello'
assert(m.y == 'hello')

# module caching: repeated import returns the same object
import string as s1
import string as s2
assert(introspect.toptr(s1) == introspect.toptr(s2))

# sys.path() returns a list with at least one entry (be_module_path)
import sys
var p = sys.path()
assert(isinstance(p, list))
assert(p.size() >= 1)

# file-based module loading via sys.path (load_package / load_path / open_libfile / open_script)
sys.path().push('modules')
import binary_heap
assert(type(binary_heap) == 'module')
assert(type(binary_heap.sort) == 'function')
assert(type(binary_heap.make_heap) == 'function')

# introspect.module() loads a module without creating a global (be_module_load)
var math_mod = introspect.module('math')
assert(type(math_mod) == 'module')
assert(type(math_mod.pi) == 'real')

# member() callback: dynamic attribute dispatch (be_module_attr member path)
var m3 = module('dyn')
m3.member = def(k) return 'val_' + k end
assert(m3.foo == 'val_foo')
assert(m3.bar == 'val_bar')

# member() returning undefined triggers attribute_error (BE_NONE path)
var m4 = module('undef_member')
m4.member = def(k) import undefined; return undefined end
try
    var r = m4.nonexistent
    assert(false, 'should have raised')
except .. as e, msg
    assert(e == 'attribute_error')
end

# accessing non-existent key on module with no member() raises attribute_error
var m5 = module('noattr')
m5.foo = 1
try
    var r = m5.bar
    assert(false, 'should have raised')
except .. as e, msg
    assert(e == 'attribute_error')
end

# writing to a const/solidified module triggers setmember path and raises attribute_error
import math
try
    math.myval = 42
    assert(false, 'should have raised')
except .. as e, msg
    assert(e == 'attribute_error')
end
