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
