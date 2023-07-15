def assert_attribute_error(f)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'attribute_error')
    end
end

class A
    static a    #- deprecated syntax -#
    def init() self.b = 2 end
    def f() end 
    var b 
    static var c, s, r  #- preferred syntax -#
end

assert(A.a == nil)
assert(A.c == nil)
assert(A.s == nil)
assert_attribute_error(/-> A.b)
assert_attribute_error(/-> A.d)

a = A()
assert(a.b == 2)
assert(a.a == nil)
assert(a.c == nil)

A.a = 1
A.c = 3
A.s = "foo"
A.r = 3.5
assert(a.a == 1)
assert(a.c == 3)
assert(A.a == 1)
assert(A.c == 3)
import gc gc.collect()
assert(A.s == "foo")
assert(a.s == "foo")
assert(A.r == 3.5)
assert(a.r == 3.5)

#- test valid or invalid methods and members -#

def assert_attribute_error(c)
    try
        compile(c)()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'attribute_error')
    end
end

class A
    var a, g
    static h
    def init() self.a = 1 end
    def f(x, y) return type(self) end
end
a=A()
a.g = def (x, y) return type(x) end
A.h = def (x, y) return type(x) end

assert(type(a.g) == 'function')
assert(type(a.h) == 'function')

assert(a.g(1) == 'int')
assert(a.h(1) == 'int')
assert(A.h(1) == 'int')


class A
    var a
    static def g(x, y) return [x,y] end
    static h = def (x, y) return [x,y] end
    def init() self.a = 1 end
    def f(x, y) return type(self) end
end
a=A()
assert(type(a.g) == 'function')
assert(type(a.h) == 'function')
assert(type(A.g) == 'function')
assert(type(A.h) == 'function')
assert(a.g(1,2) == [1,2])
assert(a.h(1,2) == [1,2])
assert(A.g(1,2) == [1,2])
assert(A.h(1,2) == [1,2])
a.a = def (x,y) return [x,y] end
assert(a.a(1,2) == [1,2])


#- test static initializers -#
class A
    static a = 1, b, c = 3.5, d = 42, e = "foo", f = [1], g = {}
    var aa,ab
end

assert(A.a == 1)
assert(A.b == nil)
assert(A.c == 3.5)
assert(A.d == 42)
assert(A.e == "foo")
assert(A.f == [1])

a = A()
assert(a.a == 1)
assert(a.b == nil)
assert(a.c == 3.5)
assert(a.d == 42)
assert(a.e == "foo")
assert(a.f == [1])
assert(a.g == A.g)
assert(a.aa == nil)
assert(a.ab == nil)

#- used to fail for subclasses -#
class A static a=1 end
class B:A static a=A def f() end static b=1 static c=A end
assert(A.a == 1)
assert(B.a == A)
assert(B.b == 1)
assert(B.c == A)

#- static class get an implicit `_class` variable -#
class A
    static def f(x) return _class end
end
assert(A.f() == A)

#- static class within a class -#
class A
    static class B
        static def f() return 1 end
        def g() return 2 end
    end
end
a = A()
b = A.B()
assert(classname(a) == 'A')
assert(classname(b) == 'B')
assert(A.B.f() == 1)
assert(b.g() == 2)
assert(super(A.B) == nil)

#- `_class` initializer can now be used in initializer code -#
class A
  static var a = 1
  static var b = _class
  static var c = [_class.a, _class.b]
  static def f(x)
    return _class
  end
end
assert(A.a == 1)
assert(A.b == A)
assert(A.c == [1, A])
assert(A.f(1) == A)
