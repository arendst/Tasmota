#- basic initialization -#

def assert_attribute_error(f)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'attribute_error')
    end
end

class T1
    var a, b
    def init()
        self.a = 1
        self.b = 2
    end
    def f() return true end
    def g() return false end
end
t = T1()

#- warm up -#
assert(t.a == 1)
assert(t.b == 2)
assert(t.f() == true)
assert(t.g() == false)

#- test normal errors when method does not exist -#
assert_attribute_error(/-> t.h())
assert_attribute_error(/-> t.c)

class T2 : T1
    def member(n)
        if (n == 'f1') return / n -> n end
        if (n == 'f2') return /-> 4 end
        if (n == 'a1') return 10 end
    end
end
t2 = T2()

#- test non-regression -#
assert(t2.a == 1)
assert(t2.b == 2)
assert(t2.f() == true)
assert(t2.g() == false)
assert_attribute_error(/-> t2.h())

#- try virtual methods -#
assert(t2.f1() == t2)
assert(t2.f2() == 4)
assert(t2.a1 == 10)

#- module -#
m = module("m")

m.a = 1
assert(m.a == 1)
assert_attribute_error(/-> m.b)

m.member = def(n)
    if n == "b" return 2 end
end

assert(m.b == 2)
assert_attribute_error(/-> m.c)
