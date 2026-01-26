#- virtual attributes -#

def assert_attribute_error(f)
    try
        f()
        assert(false, 'unexpected execution flow')
    except .. as e, m
        assert(e == 'attribute_error')
    end
end

class Ta
    var a, b, virtual_c
    def init()
        self.a = 1
        self.b = 2
        self.virtual_c = 3
    end
    def member(n)
        if n == 'c' return self.virtual_c end
        return nil
    end
    def setmember(n, v)
        if n == 'c' self.virtual_c = v return true end
        return false
    end
end
ta = Ta()

assert(ta.a == 1)
assert(ta.b == 2)
assert(ta.c == 3)
ta.a = 10
assert(ta.a == 10)
assert(ta.c == 3)
ta.c = 30
assert(ta.c == 30)
assert(ta.virtual_c == 30)
assert_attribute_error(def() ta.d = 0 end)

# bug: if a function is sent to 'setmember()', the internal BE_STATIC flag is added
# which makes the function not callable and scrambles tostring()
class A
  var _v
  def init()
    self._v = {}
  end
  def setmember(name, value)
    self._v[name] = value
  end
end
a = A()
f = def(name, time_ms) return 42 end
a.f = f

assert(a._v['f']() == 42)
