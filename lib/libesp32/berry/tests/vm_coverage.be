# Tests targeting uncovered paths in be_vm.c

import string as s

# Helper functions to bypass compile-time type checks
def neg(x) return -x end
def flip(x) return ~x end
def add(a,b) return a+b end
def sub(a,b) return a-b end
def mul(a,b) return a*b end
def div(a,b) return a/b end
def mod(a,b) return a%b end
def conn(a,b) return a..b end
def call_it(f) return f() end
def shl(a,b) return a<<b end
def shr(a,b) return a>>b end

# SHL / SHR opcodes (previously zero coverage)
assert(shl(1, 4) == 16)
assert(shr(16, 2) == 4)
assert(shl(0xFF, 8) == 0xFF00)
assert(shr(0xFF00, 8) == 0xFF)
assert(shl(3, 0) == 3)
assert(shr(3, 0) == 3)

# NEG on integer (line 860 - only real path was hit before)
assert(neg(5) == -5)
assert(neg(0) == 0)
assert(neg(-3) == 3)

# unop_error: unary minus on string (runtime, bypasses compile-time check)
try
    neg("hello")
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '-') > 0)
end

# unop_error: bitwise flip on real
try
    flip(1.5)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '~') > 0)
end

# call_error: calling a non-callable value
try
    call_it(42)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, 'callable') > 0)
end

# binop_error: ADD on incompatible types
try
    add(true, false)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '+') > 0)
end

# binop_error: SUB on incompatible types
try
    sub("a", "b")
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '-') > 0)
end

# binop_error: DIV on incompatible types
try
    div("a", 2)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '/') > 0)
end

# binop_error: MOD on incompatible types
try
    mod("a", 2)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '%') > 0)
end

# binop_error: CONNECT (..) on incompatible types
try
    conn(1.5, 2.5)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '..') > 0)
end

# multiply_str: invalid type for string repetition (float count)
try
    mul("abc", 1.5)
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, '*') > 0)
end

# Instance operator overloads: SUB, MUL, DIV, MOD (ins_binop paths)
# Also covers ins_unop via NEG (-*) and FLIP (~)
class Vec
    var v
    def init(n) self.v = n end
    def -(other) return Vec(self.v - other.v) end
    def -*(other) return Vec(-self.v) end
    def ~(other) return Vec(~self.v) end
    def *(other) return Vec(self.v * other.v) end
    def /(other) return Vec(self.v / other.v) end
    def %(other) return Vec(self.v % other.v) end
end

var a = Vec(10)
var b = Vec(3)

assert(sub(a, b).v == 7)
assert(mul(a, b).v == 30)
assert(div(a, b).v == 3)
assert(mod(a, b).v == 1)
assert(neg(a).v == -10)
assert(flip(b).v == ~3)

# obj2bool fallback: instance without tobool returns btrue (line 298)
class NoTobool
    var x
    def init(n) self.x = n end
end

var obj = NoTobool(0)
assert(bool(obj) == true)

# check_bool error: comparison operator returning non-bool
class BadEq
    def ==(other) return 42 end
end

try
    var r = (BadEq() == BadEq())
    assert(false, 'expected type_error')
except 'type_error' as e, m
    assert(s.count(m, 'bool') > 0)
end

# obj_attribute error: accessing non-existent attribute on instance
class Plain
    var x
    def init() self.x = 1 end
end

try
    var p = Plain()
    var r = p.nonexistent
    assert(false, 'expected attribute_error')
except 'attribute_error' as e, m
    assert(s.count(m, 'nonexistent') > 0)
end

# attribute_error: calling non-existent method on instance
try
    var p = Plain()
    p.no_such_method()
    assert(false, 'expected attribute_error')
except 'attribute_error' as e, m
    assert(s.count(m, 'no_such_method') > 0)
end

# class_attribute error: accessing non-existent static attr on class
class StaticClass
    static x = 10
end

try
    var r = StaticClass.nonexistent
    assert(false, 'expected attribute_error')
except 'attribute_error' as e, m
    assert(s.count(m, 'nonexistent') > 0)
end

# module_attribute error: accessing non-existent module member
import math

try
    var r = math.nonexistent_fn
    assert(false, 'expected attribute_error')
except 'attribute_error' as e, m
    assert(s.count(m, 'nonexistent_fn') > 0)
end

# GETNGBL / SETNGBL: named global access via global module
import global

global.vm_test_ngbl = 99
assert(global.vm_test_ngbl == 99)
global.vm_test_ngbl = 100
assert(global.vm_test_ngbl == 100)
global.undef("vm_test_ngbl")
