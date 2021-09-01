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
