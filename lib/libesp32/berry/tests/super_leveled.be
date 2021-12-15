#- test for leveled use of super() -#

#- setup -#
class A   def r() return 'a' end def f() return self.r() end end
class B:A def r() return 'b' end def f() return super(self,A).f() + 'b' end end
class C:B def r() return 'c' end def f() return super(self,B).f() + 'c' end end
a=A()
b=B()
c=C()

#- regular behavior -#
assert(classname(a) == 'A')
assert(classname(b) == 'B')
assert(classname(c) == 'C')
assert(a.r() == 'a')
assert(b.r() == 'b')
assert(c.r() == 'c')
assert(a.f() == 'a')

#- standard use of super() -#
assert(super(a) == nil)
assert(super(A) == nil)
assert(classname(super(B)) == 'A')
assert(classname(super(C)) == 'B')
assert(classname(super(super(C))) == 'A')
assert(super(super(super(C))) == nil)

#- super() levele -#
assert(super(a,A) == nil)
assert(super(b,B) == nil)
assert(super(c,C) == nil)
assert(classname(super(c,B)) == 'B')
assert(classname(super(c,A)) == 'A')
assert(super(c,map) == nil) #- not a parent class -#

#- wrapping it all -#
assert(a.f() == 'a')
assert(b.f() == 'bb')

#- the last one is tricky:
   c.f() -> calls f() in class B -> calls f() in class A -> calls r() of overall class hence C
-#
assert(c.f() == 'cbc')