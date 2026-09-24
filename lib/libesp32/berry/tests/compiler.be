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
