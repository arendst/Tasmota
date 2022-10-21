# test for introspect.ismethod
import introspect

# ismethod should return nil for any non-Berry closure
assert(introspect.ismethod() == nil)
assert(introspect.ismethod(true) == nil)
assert(introspect.ismethod("a") == nil)
assert(introspect.ismethod([]) == nil)
assert(introspect.ismethod({}) == nil)
assert(introspect.ismethod(introspect) == nil)      # module
assert(introspect.ismethod(introspect.ismethod) == nil)     # native method

def h() end

class A
    def f() end
    static def g() end
    var h
end
a=A()
a.h = h

assert(introspect.ismethod(h) == false)
assert(introspect.ismethod(A.f) == true)
assert(introspect.ismethod(A.g) == false)

assert(introspect.ismethod(a.f) == true)
assert(introspect.ismethod(a.g) == false)
assert(introspect.ismethod(a.h) == false)
