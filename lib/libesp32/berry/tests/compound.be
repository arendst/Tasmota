# test bug in compound statements

a = 0
assert(a == 0)
a += 1
assert(a == 1)
a += 10/2
assert(a == 6)

class A var a def init() self.a = 1 end def f(x) self.a+=x/2 end def g(x) self.a = self.a + x/2 end end

a = A()
assert(a.a == 1)
a.f(10)
assert(a.a == 6)
b=A()
assert(b.a == 1)
b.g(10)
assert(b.a == 6)