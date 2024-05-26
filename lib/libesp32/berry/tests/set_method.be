# test setting methods as an external class

class A
    var a
    def init(a)
        self.a = a
    end
    def f0(x) return self end
    def f1(x) return x end
    def f2(x) return self.a end
    static def ff0(x) return _class end
    static def ff1(x) return x end
end

class B
    var b
    def init(b)
        self.b = b
    end
end

class C : B
    var a
    def init(a)
        self.a = a
    end

    def fc0 = A.f0
    def fc1 = A.f1
    def fc2 = A.f2
    static def ffc0 = A.ff0
    static def ffc1 = A.ff1
end

c = C(10)
assert(c.fc0(1) == c)
assert(c.fc1(1) == 1)
assert(c.fc2(1) == 10)
assert(c.ffc0(1) == A)
assert(c.ffc1(1) == 1)
