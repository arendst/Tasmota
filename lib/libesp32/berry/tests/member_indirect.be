#- new syntax for indirect members -#

#- module accessor -#
s_pi = 'pi'

import math
assert(math.('pi') == math.pi)
assert(math.(s_pi) == math.pi)

#- module writer -#
m = module("m")

m.('aa') = 1
m.('a' + 'b') = 2
s_ac = 'ac'
m.(s_ac) = 3
assert(m.aa == 1)
assert(m.ab == 2)
assert(m.ac == 3)
assert(m.('a'+'a') == 1)

#- class accessor -#
class A1
    static a = 1, b = 2
    static s = "foo"
    def f() return 0 end
end
assert(A1.a == 1)
assert(A1.b == 2)
assert(A1.s == "foo")
assert(type(A1.f) == 'function')

#- instance accessor -#
class A2
    var a, b
    static s_a = 'a'
    def init(a,b)
        self.(self.('s_a')) = a
        self.('b') = b
    end
    def f(x)
        return x+1
    end
    def g(a,b)
        return A2(a,b)
    end
end
a = A2(1,2)

#- reading members -#
assert(a.a == 1)
assert(a.b == 2)
assert(a.(A2.s_a) == 1)
assert(a.('b') == 2)

#- writing members -#
a.('a') = 10
a.('bb'[0]) = 11
assert(a.a == 10)
assert(a.b == 11)

#- calling methods -#
assert(a.f(1) == 2)
assert(a.('f')(2) == 3)

#- mulit-level -#
assert(a.('g')(3,4).('a') == 3)
a.('a') = a.g(3,4)
assert(a.a.b == 4)
assert(a.('a').b == 4)
assert(a.('a').('b') == 4)
assert(a.a.('b') == 4)

a.('a').('b') += 1
assert(a.a.b == 5)
