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
