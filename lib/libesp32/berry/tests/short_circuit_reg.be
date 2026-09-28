# Regression tests for https://github.com/berry-lang/berry/issues/548
#
# Three places where the compiler emitted code that read a register which was
# never written on the path actually taken. All three involve the result of a
# short-circuit `&&` / `||`:
#
#   1. `&&` / `||` as the left operand of a binary operator
#   2. `&&` / `||` as an index key
#   3. `&&` / `||` as the whole right-hand side of a compound assignment
#
# Cases 1 and 3 only misbehave when the operand that the stale register points
# at is a named local or a parameter, so the helpers below take parameters
# instead of using literals.

import string

def assert_error(f, error_type)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == error_type, 'unexpected error type: ' + str(e))
    end
end

# the message matters for case 3: reading the uninitialized register reports a
# type that has nothing to do with the expression
def assert_error_msg(f, error_type, needle)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == error_type, 'unexpected error type: ' + str(e))
        assert(string.find(m, needle) >= 0, 'unexpected error message: ' + str(m))
    end
end

#-------------------------------------------------------------------------#
# 1. short-circuit as the left operand of a binary operator
#
# The LDBOOL pair writes the bool to a fresh register, but the expression
# descriptor kept pointing at the register of the last short-circuit operand,
# so the binary operator read that operand instead of the bool.
#-------------------------------------------------------------------------#

# the value of `&&` is a bool, never one of its operands
def and_eq(p, q) return (p && q) == q end
assert(and_eq(3, 2) == false)       # true == 2, not 2 == 2
assert(and_eq(1, true) == true)     # true == true
assert(and_eq(0, 2) == false)       # short circuit taken: false == 2

def or_eq(p, q) return (p || q) == q end
assert(or_eq(0, 5) == false)        # true == 5, not 5 == 5
assert(or_eq(0, true) == true)      # true == true

def and_ne(p, q) return (p && q) != q end
assert(and_ne(3, 2) == true)

# because it is a bool, arithmetic on it must be a type error
def and_add(p, q) return (p && q) + 5 end
assert_error(def () return and_add(3, 2) end, 'type_error')   # issue repro
assert_error(def () return and_add(0, 2) end, 'type_error')

def or_add(p, q) return (p || q) + 5 end
assert_error(def () return or_add(0, 2) end, 'type_error')
assert_error(def () return or_add(1, 2) end, 'type_error')

def and_mul(p, q) return (p && q) * q end
assert_error(def () return and_mul(3, 2) end, 'type_error')

def and_sub(p, q) return (p && q) - q end
assert_error(def () return and_sub(3, 2) end, 'type_error')

# nested short-circuits, and a short-circuit on both sides
def and_and(p, q, r) return (p && q) == (q && r) end
assert(and_and(1, 2, 3) == true)    # true == true
assert(and_and(1, 2, 0) == false)   # true == false
assert(and_and(1, 0, 3) == true)    # false == false

# already-correct forms must stay correct
def and_not(p, q) return !(p && q) end
assert(and_not(3, 2) == false)
assert(and_not(0, 2) == true)

def and_cond(p, q) return (p && q) ? q : p end
assert(and_cond(3, 2) == 2)
assert(and_cond(0, 2) == 0)

def and_call(p, q) return str(p && q) end
assert(and_call(3, 2) == 'true')
assert(and_call(0, 2) == 'false')

#-------------------------------------------------------------------------#
# 2. short-circuit as an index key
#
# `package_suffix()` materialized the indexed object after the key, and the
# key had already opened its jump list, so the code loading the object landed
# between the two jumps and was skipped whenever the short circuit was taken.
#-------------------------------------------------------------------------#

m = {true: 'T', false: 'F'}

def m_and(p, q) return m[p && q] end
assert(m_and(0, 1) == 'F')          # issue repro: short circuit taken
assert(m_and(1, 1) == 'T')

def m_or(p, q) return m[p || q] end
assert(m_or(1, 0) == 'T')           # short circuit taken
assert(m_or(0, 0) == 'F')
assert(m_or(0, 1) == 'T')

# the object being indexed can itself be a suffix expression
class Holder var d, e end
var h = Holder()
h.d = {true: 'yes', false: 'no'}
h.e = Holder()
h.e.d = {true: 1, false: 2}

def h_and(p, q) return h.d[p && q] end
assert(h_and(0, 1) == 'no')
assert(h_and(1, 1) == 'yes')

def h_and2(p, q) return h.e.d[p && q] end
assert(h_and2(0, 1) == 2)
assert(h_and2(1, 1) == 1)

# ... and a local, an upvalue and a call result
def l_and(d, p, q) return d[p && q] end
assert(l_and(m, 0, 1) == 'F')
assert(l_and(m, 1, 1) == 'T')

def make_upval()
    var d = {true: 'u1', false: 'u0'}
    return def (p, q) return d[p && q] end
end
var f_upval = make_upval()
assert(f_upval(0, 1) == 'u0')
assert(f_upval(1, 1) == 'u1')

def get_map() return m end
def c_and(p, q) return get_map()[p && q] end
assert(c_and(0, 1) == 'F')
assert(c_and(1, 1) == 'T')

# also as an assignment target
def m_set(p, q, v) m[p && q] = v end
m_set(0, 1, 'F2')
assert(m[false] == 'F2' && m[true] == 'T')
m_set(1, 1, 'T2')
assert(m[false] == 'F2' && m[true] == 'T2')

# The fix is guarded on `hasjump(k)` so that keys without pending jumps keep
# the object-first order introduced by #366. Behaviour for those keys must be
# unchanged; the register count is what #366 is about and tests/compiler.be
# guards that part.
class Arr var a end
var ar = Arr()
ar.a = [0, 1, 2, 3]
assert(ar.a[128 - 126] == 2)
ar.a[128 - 126] = 22
assert(ar.a[2] == 22)
def arr_get(o, i) return o.a[i] end
assert(arr_get(ar, -1) == 3)
def arr_two(o) return o.a[0] + o.a[1] end
assert(arr_two(ar) == 1)

#-------------------------------------------------------------------------#
# 3. short-circuit as the whole right-hand side of a compound assignment
#
# `compound_assign()` did not materialize the left operand before parsing the
# right one, so the load of the left operand was emitted after the right side
# had opened its jump list and was skipped when the short circuit was taken.
# A bare `&&` / `||` as the outermost operator of the right-hand side is the
# only affected shape.
#-------------------------------------------------------------------------#

# `bool` has no arithmetic, so use a class that accepts one to observe the
# value rather than just an error message
class Bump
    var v
    def init(v) self.v = v end
    def +(x) return Bump(self.v + (x ? 1 : 0)) end
    def -(x) return Bump(self.v - (x ? 1 : 0)) end
    def *(x) return Bump(self.v * (x ? 2 : 1)) end
end

# index on the left
var bl = [Bump(10), Bump(20)]
def bl_add(i, p, q) bl[i] += (p && q) end
bl_add(0, 0, 1)                     # short circuit taken -> false -> +0
assert(bl[0].v == 10)
bl_add(0, 1, 1)                     # not taken -> true -> +1
assert(bl[0].v == 11)
def bl_add_or(i, p, q) bl[i] += (p || q) end
bl_add_or(1, 1, 0)                  # short circuit taken -> true -> +1
assert(bl[1].v == 21)
bl_add_or(1, 0, 0)                  # not taken -> false -> +0
assert(bl[1].v == 21)

# every compound operator goes through the same path
def bl_sub(i, p, q) bl[i] -= (p && q) end
bl_sub(0, 0, 1)
assert(bl[0].v == 11)
bl_sub(0, 1, 1)
assert(bl[0].v == 10)
def bl_mul(i, p, q) bl[i] *= (p && q) end
bl_mul(0, 0, 1)
assert(bl[0].v == 10)
bl_mul(0, 1, 1)
assert(bl[0].v == 20)

# member on the left
class BumpHolder var b end
var bh = BumpHolder()
bh.b = Bump(5)
def bh_add(p, q) bh.b += (p && q) end
bh_add(0, 1)
assert(bh.b.v == 5)
bh_add(1, 1)
assert(bh.b.v == 6)

# global on the left
gb = Bump(7)
def gb_add(p, q) gb += (p && q) end
gb_add(0, 1)
assert(gb.v == 7)
gb_add(1, 1)
assert(gb.v == 8)

# upvalue on the left
def make_bump_counter()
    var b = Bump(0)
    return [def (p, q) b += (p && q) end, def () return b.v end]
end
var bc = make_bump_counter()
bc[0](0, 1)
assert(bc[1]() == 0)
bc[0](1, 1)
assert(bc[1]() == 1)

# plain ints on the left still raise, and both paths must blame the bool on the
# right rather than whatever the unwritten register happened to hold
def il_add(l, i, p, q) l[i] += (p && q) end
assert_error_msg(def () il_add([10, 20], 0, 1, 1) end, 'type_error', 'bool')
assert_error_msg(def () il_add([10, 20], 0, 0, 1) end, 'type_error', 'bool')
gi = 1
def gl_add(p, q) gi += (p || q) end
assert_error_msg(def () gl_add(1, 0) end, 'type_error', 'bool')
assert_error_msg(def () gl_add(0, 0) end, 'type_error', 'bool')

# shapes that were already correct because something closes the jumps first
var bl2 = [Bump(1), Bump(2)]
def bl2_expr(i, p, q) bl2[i] += !(p && q) end
bl2_expr(0, 0, 1)
assert(bl2[0].v == 2)
def bl2_cond(i, p, q) bl2[i] += p ? q : p end
bl2_cond(1, 0, 1)
assert(bl2[1].v == 2)

# the register cached for the left operand must still be honoured: the
# `dst`-less variant of the fix breaks plain compound assignment on a suffix
var il = [10, 20, 30]
il[0] += 1
assert(il[0] == 11)
il[1] += il[2]
assert(il[1] == 50)
var idx = 2
il[idx] += il[idx]
assert(il[2] == 60)
var im = {'a': 1}
im['a'] += 4
assert(im['a'] == 5)
