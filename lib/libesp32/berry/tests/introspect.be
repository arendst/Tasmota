#- introspect -#
import introspect

#- test for modules -#
m = module("m")
m.a = 1
m.b = def () return "foo" end

assert(introspect.members(m) == ['a', 'b'])
assert(introspect.get(m, 'a') == 1)
assert(type(introspect.get(m, 'b')) == 'function')

introspect.set(m, 'a', 2)
assert(m.a == 2)

#- test for instance -#
class A var a,b static c=1,d=2 def f() end end
a=A()

assert(introspect.members(A) == ['a', 'f', 'b', 'c', 'd']) #- class members -#
assert(introspect.members(a) == ['a', 'f', 'b', 'c', 'd']) #- instance members -#

assert(introspect.get(a, 'c') == 1)
assert(introspect.get(a, 'd') == 2)
assert(introspect.get(a, 'a') == nil)

introspect.set(a, 'a', 3)
assert(a.a == 3)

#- load module dynamically -#
import introspect
m = introspect.module("math") # load module `math`, assign to `m` and don't create a global variable
assert(type(m.pi) == 'real')

#- name -#
import string
assert(introspect.name(string) == 'string')
assert(introspect.name(print) == nil)       # native C function don't have a registered name
assert(introspect.name("foo") == nil)
class A def a() end static def b() end static var c end
assert(introspect.name(A) == 'A')
assert(introspect.name(A.a) == 'a')
assert(introspect.name(A.b) == 'b')
assert(introspect.name(A.c) == nil)

# test introspect get and set
# class and instance
class A
    static var a
    var b
end

a = A()
introspect.set(A, "a", 10)
assert(A.a == 10)
assert(introspect.get(A, "a") == 10)

introspect.set(a, "b", 20)
assert(a.b == 20)
assert(introspect.get(a, "b") == 20)

# module
m = module('m')
introspect.set(m, 'c', 30)
assert(m.c == 30)
assert(introspect.get(m, 'c') == 30)

#- contains: instance, class, module -#
class B var x def f() end end
b = B()
assert(introspect.contains(b, 'x') == true)
assert(introspect.contains(b, 'f') == true)
assert(introspect.contains(b, 'z') == false)
assert(introspect.contains(B, 'x') == true)
assert(introspect.contains(B, 'missing') == false)
m2 = module('m2')
m2.val = 42
assert(introspect.contains(m2, 'val') == true)
assert(introspect.contains(m2, 'nope') == false)
# non-object types always return false
assert(introspect.contains(42, 'x') == false)

#- members with no args dumps globals -#
assert(isinstance(introspect.members(), list))

#- members(nil) also dumps globals -#
assert(isinstance(introspect.members(nil), list))

#- members with unsupported type returns empty list -#
assert(introspect.members(42) == [])

#- get: protected mode returns nil for non-object type -#
assert(introspect.get(42, 'x') == nil)

#- get: unprotected mode, member found -#
class C var p end
c = C()
c.p = 99
assert(introspect.get(c, 'p', false) == 99)

#- get: unprotected mode (true = raise), missing member returns module 'undefined' -#
var r = introspect.get(c, 'no_such', true)
assert(type(r) == 'module')
assert(introspect.name(r) == 'undefined')
# same for class and module
var r2 = introspect.get(C, 'no_such', true)
assert(introspect.name(r2) == 'undefined')
var mx = module('mx')
var r3 = introspect.get(mx, 'no_such', true)
assert(introspect.name(r3) == 'undefined')

#- toptr: string returns ptr -#
assert(type(introspect.toptr('hello')) == 'ptr')

#- toptr: closure returns ptr -#
def myfunc() return 1 end
assert(type(introspect.toptr(myfunc)) == 'ptr')

#- toptr: unsupported type raises value_error -#
try
    introspect.toptr(true)
    assert(false)
except .. as e, msg
    assert(e == 'value_error')
end

#- fromptr / toptr: unsupported types -#
try
    introspect.toptr(1.5)
    assert(false)
except .. as e, msg
    assert(e == 'value_error')
end

#- fromptr: round-trip comptr -> list -#
var lst = [10, 20, 30]
var rp = introspect.toptr(lst)
assert(type(rp) == 'ptr')
var recovered = introspect.fromptr(rp)
assert(isinstance(recovered, list))
assert(recovered == [10, 20, 30])

#- solidified: Berry closure is not solidified -#
def notsolid() return 1 end
assert(introspect.solidified(notsolid) == false)

#- L93: get with no args or non-string key returns nil -#
assert(introspect.get() == nil)
assert(introspect.get('hello', 42) == nil)

#- L137: toptr with no args returns nil -#
assert(introspect.toptr() == nil)

#- L151: solidified with non-function type returns nil or false -#
assert(introspect.solidified('hello') == false)  # string: basetype >= BE_FUNCTION, not solidified
assert(introspect.solidified(42) == nil)          # int: doesn't match, returns nil
assert(introspect.solidified() == nil)            # no args: returns nil

#- L175: fromptr with no args or zero pointer returns nil -#
assert(introspect.fromptr() == nil)
assert(introspect.fromptr(0) == nil)

#- module() returns nil for unknown module name -#
assert(introspect.module("no_such_module_xyz") == nil)
