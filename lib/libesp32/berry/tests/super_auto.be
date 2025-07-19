#- test for new auto class inference of super() -#

#- test that we can call init() even if it's not defined -#
class Z end
z=Z()
assert(z.init != nil)
z.init()  #- should do nothing -#

#- check the old way still works -#
class A1
    var a
    def init(a)
        self.a = a
    end
end
class B1:A1
    var b
    def init(a,b)
        super(self,A1).init(a)
        self.b = b
    end
end
class C1:B1
    var c
    def init(a,b,c)
        super(self,B1).init(a,b)
        self.c = c
    end
end
#- -#
c1=C1(1,2,3)
assert(c1.a == 1)
assert(c1.b == 2)
assert(c1.c == 3)

#- test simple behavior -#
class A0 var a end
class B0:A0 var b end
class C0:B0 end
c0=C0()
assert(classof(c0) == C0)
assert(classof(super(c0)) == B0)
assert(classof(super(super(c0))) == A0)
assert(super(super(super(c0))) == nil)

assert(super(C0) == B0)
assert(super(super(C0)) == A0)
assert(super(super(super(C0))) == nil)

assert(classof(super(c0,B0)) == B0)
assert(classof(super(c0,A0)) == A0)

#- test auto inference of target superclass -#
class A
    var a
    def init(a)
        self.a = a
    end
end
class B:A
    var b
    def init(a,b)
        super(self).init(a)
        self.b = b
    end
end
class C:B
    var c
    def init(a,b,c)
        super(self).init(a,b)
        self.c = c
    end
end
#- -#
c=C(1,2,3)

assert(c.a == 1)
assert(c.b == 2)
assert(c.c == 3)class A
end
class B:A
    var b
    def init(a,b) super(self).init(a) self.b = b end
end
class C:B
    var c
    def init(a,b,c) super(self).init(a,b) self.c = c end
end
c=C(1,2,3)

#- variant if A2 does not have an init() method, still works -#
class A2
    static a=1
end
class B2:A2
    var b
    def init(a,b) super(self).init(a) self.b = b end
end
class C2:B2
    var c
    def init(a,b,c) super(self).init(a,b) self.c = c end
end
#- -#
c2=C2(1,2,3)
assert(c2.a == 1)
assert(c2.b == 2)
assert(c2.c == 3)

#- difference in behavior whether the second arg is provided or not -#
class A3
end
class B3:A3
    def b1()
        return super(self)
    end
    def b2(c)
        return super(self, c)
    end
end
class C3:B3
end
#- -#
b3=B3()
c3=C3()
assert(classof(c3.b1()) == A3)
assert(classof(b3.b1()) == A3)
assert(classof(c3.b2(B3)) == B3)
assert(classof(c3.b2(A3)) == A3)

assert(classof(c3.b2(nil)) == B3)  #- testing super(self<C3>,nil) in B3::b2() -#

assert(c3.b2(C3) == nil)  #- if specifying the current class, can't find any relevant class in supers -#