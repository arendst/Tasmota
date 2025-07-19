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
