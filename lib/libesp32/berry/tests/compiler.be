#- test for issue #117 -#

class A var a end
a=A()
a.a = ["foo", "bar"]

s = nil
def fs(m) s = m end

class B
  var b, i
  def nok()
    fs(self.b.a[self.i])    # wrong behavior
  end
  def ok()
    var i = self.i
    fs(self.b.a[i])    # works correctly
  end
end
b=B()
b.i=0
b.b=a

b.nok()
assert(s == "foo")

b.ok()
assert(s == "foo")

# detect a wrong compilation when accessing index
# Berry compilation problem:
#
# ```berry
# def f(self) print(self.a[128]) end
# ```
#
# Compilation assigns unwanted registers:
# ```
#       0x60040001,  //  0000  GETGBL     R1      G1
#       0x540A007F,  //  0001  LDINT      R2      128
#       0x880C0100,  //  0002  GETMBR     R3      R0      K0
#       0x94080602,  //  0003  GETIDX     R2      R3      R2
#       0x5C100400,  //  0004  MOVE       R4      R2         <- PROBLEM
#       0x7C040200,  //  0005  CALL       R1      1
#       0x80000000,  //  0006  RET        0
# ```
#
# With the fix, the integer is retrieved in second place, and erroneous register is not allocated:
# ```
#       0x60040001,  //  0000  GETGBL     R1      G1
#       0x88080100,  //  0001  GETMBR     R2      R0      K0
#       0x540E007F,  //  0002  LDINT      R3      128
#       0x94080403,  //  0003  GETIDX     R2      R2      R3
#       0x7C040200,  //  0004  CALL       R1      1
#       0x80000000,  //  0005  RET        0
# ```
def f(a,b) return b end
l = [1,2,3,4]
assert(f(l[-1],l[-2]) == 3)

# Compilation problem:
# def test()
#   var line = '1234567890'
#   line = line[3..7]
# # print(line)
#   for n : 1..2 end
# end
# test()

# BRY: Exception> 'attribute_error' - the 'range' object has no method '()'
# stack traceback:
#     :5: in function `test`
#     :7: in function `main`
def test()
  var line = '1234567890'
  line = line[3..7]
# print(line)
  for n : 1..2 end
end
test()

# Index assignment whose key is itself an index or member expression.
# The object of the assignment was loaded into a register above the key's
# own object and index, suffix_destreg() released it together with them, and
# freereg ended up below the number of active locals. The statement itself
# was right, but the next one used the register of the last local as a
# scratch register:
#
# ```berry
# def f()
#   var a = 10
#   var b = 20
#   m[m[4]] = 1
#   var c = m[0] + 1    # overwrote `b`: returned [10, 2, 2]
#   return [a, b, c]
# end
# ```
#
# With no local in scope, compilation failed with
# `register overflow (more than 255)` instead.
nk_m = [0, 0, 0, 0, 0]
def nk_f()
  var a = 10
  var b = 20
  nk_m[nk_m[4]] = 1
  var c = nk_m[0] + 1
  return [a, b, c]
end
assert(nk_f() == [10, 20, 2])

# computed inner index and compound assignments
nk_l = [0, 1, 2, 3, 4, 5, 6, 7]
def nk_g(i)
  var a = 10
  var b = 20
  nk_l[nk_l[i + 1]] = 9     # nk_l[2] = 9
  nk_l[nk_l[5]] <<= 1       # nk_l[5] = 10
  nk_l[nk_l[7]] += 1        # nk_l[7] = 8
  var c = nk_l[0] + 1
  return [a, b, c]
end
assert(nk_g(1) == [10, 20, 1])
assert(nk_l == [0, 1, 9, 3, 4, 10, 6, 8])

# the same through instance members
class nk_C
  var map, keys
  def init() self.map = {} self.keys = ['x', 'y'] end
  def set(i, v)
    var n = 42
    self.map[self.keys[i + 1]] = v
    var s = self.keys[0]
    return [n, s]
  end
end
nk_c = nk_C()
assert(nk_c.set(0, 1) == [42, 'x'])
assert(nk_c.map['y'] == 1)

# a member with a computed name as the key
class nk_D var x end
nk_d = nk_D()
nk_d.x = 1
def nk_h()
  var a = 10
  var n = 'x'
  nk_m[nk_d.(n + '')] = 5
  var s = nk_m[0] + 1
  return [a, n, s]
end
assert(nk_h() == [10, 'x', 2])

# no local in scope: used to fail to compile
nk_n = nil
compile("nk_n = [0, 0, 0, 0, 0]  nk_n[nk_n[4]] = 1  nk_n[1] = nk_n[0] + 1")()
assert(nk_n == [1, 2, 0, 0, 0])

# Walrus into a local, with an index or member expression on the right, as a
# call argument. The right side is compiled straight into the register of the
# local, but the registers of its object and index were not released, so the
# value was passed one slot too high:
#
# ```berry
# def g()
#   var p = 0
#   return f(p := l[3], 4)    # passed l and p instead of p and 4
# end
# ```
def wl_f(a, b) return [a, b] end
wl_l = [10, 11, 12, 13, 14]
def wl_g() var p = 0 return wl_f(p := wl_l[3], 4) end
assert(wl_g() == [13, 4])       # was [wl_l, 13]
def wl_h() var p = 0 return wl_f(4, p := wl_l[3]) end
assert(wl_h() == [4, 13])       # was [4, wl_l]
def wl_i(k) var p = 0 return str(p := wl_l[k + 1]) end
assert(wl_i(2) == '13')         # was '3', the index
import global
def wl_j() var p = 0 return wl_f(p := global.wl_l, 4) end
assert(wl_j() == [wl_l, 4])     # was [<module: global>, wl_l]

# Walrus never creates a variable, neither a local, nor a global, nor a local
# shadowing a builtin: the target must already exist. A new local created in
# the middle of an expression took the register right above the other locals,
# which could still hold a temporary, so its value was lost:
#
# ```berry
# def t(a)
#   var r = a * 2 + (n := a + 1)    # computed n + n
#   return [r, n]                   # [22, 11] instead of [31, 11]
# end
# ```
#
# `wl_new` is never defined, as a local or as a global.
import string
def wl_error(code, name)
  try
    compile(code)
  except 'syntax_error' as e, m
    return string.find(m, "cannot create variable '" + name + "' with ':='") >= 0
  end
  return false
end
# new local over a pending temporary, was [22, 11]
assert(wl_error("def t(a) var r = a * 2 + (wl_new := a + 1) return [r, wl_new] end", 'wl_new'))
# new local as a call argument, was type_error: 'int' value is not callable
assert(wl_error("def t(a) return wl_f(a, wl_new := a + 1) end", 'wl_new'))
# new local in a list literal in a block, was 11, the list was overwritten
assert(wl_error("do var a = 10 var r = [a, (wl_new := a + 1)] print(r) end", 'wl_new'))
# new local in the else branch of `?:`, t(true) returned nil
assert(wl_error("def t(c) var r = c ? 1 : (wl_new := 2) return r end", 'wl_new'))
# new local with nothing pending, used to work, now refused as well
assert(wl_error("def t(a) var r = (wl_new := a + 1) + a * 2 return [r, wl_new] end", 'wl_new'))
assert(wl_error("def t(a) if (wl_new := a + 1) > 5 return wl_new end return 0 end", 'wl_new'))
assert(wl_error("def t(k) var p = 0 p = wl_l[k + 1] wl_new := 5 return wl_new end", 'wl_new'))
# new global at module level
assert(wl_error("wl_new := 4", 'wl_new'))
assert(wl_error("wl_r = 2 * 3 + (wl_new := 4)", 'wl_new'))
# local shadowing a builtin
assert(wl_error("def t() print := 1 end", 'print'))
# an existing variable is still assigned: local, global, from a function too
def wl_k(a) var n = 0 var r = a * 2 + (n := a + 1) return [r, n] end
assert(wl_k(10) == [31, 11])
def wl_m(a) var n var r = (n := a + 1) + a * 2 return [r, n] end
assert(wl_m(10) == [31, 11])
def wl_n(a) var n if (n := a + 1) > 5 return n end return 0 end
assert(wl_n(10) == 11)
wl_s = 0
wl_r = 2 * 3 + (wl_s := 4)
assert(wl_r == 10 && wl_s == 4)
def wl_o() return wl_s := 5 end
assert(wl_o() == 5 && wl_s == 5)
