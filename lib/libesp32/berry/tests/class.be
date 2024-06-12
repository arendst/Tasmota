class Test
    var maximum
    def init(maximum)
        self.maximum = maximum
    end
    def iter() # method closure upvalues test
        var i = -1, maximum = self.maximum
        return def ()
            i += 1
            if i > maximum
                raise 'stop_iteration'
            end
            return i
        end
    end
end

var sum = 0
for i : Test(10)
    sum += i
end
assert(sum == 55, 'iteraion sum is ' + str(sum) + ' (expected 55).')

#- test case for class instanciated from module member #103 -#

m = module()
g_i = 0     #- detect side effect from init() -#
class C def init() g_i += 1 end end
m.C = C

#- normal invocation -#
assert(type(C()) == 'instance')
assert(g_i == 1)

#- invoke from module member -#
assert(type(m.C()) == 'instance')
assert(g_i == 2)

class C2 var C1 def init(c) self.C1 = c end end
m.C2 = C2
c2 = m.C2(m.C)

assert(c2.C1 == C)

c3 = m.C2(m.C())
assert(type(c3.C1) == 'instance')
assert(classname(c3.C1) == 'C')

#- an instance member can be a class and called directly -#
class Test_class
    var c
    def init()
        self.c = map
    end
end
c4 = Test_class()
assert(type(c4.c) == 'class')
c5 = c4.c()
assert(type(c5) == 'instance')
assert(classname(c5) == 'map')

#- classof now gets back the class of Berry methods -#
class A
    def f() end
    static def g() end
end
class B : A
    def h() end
end
assert(classof(A.f) == A)
assert(classof(A.g) == A)
assert(classof(B.h) == B)
#- returns nil if native function of not in class -#
assert(classof(int) == nil)
assert(classof(def () end) == nil)
