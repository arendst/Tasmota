# test for the walrus operator
var a = 1
assert((a := a + 1) == 2)
assert((a := a + 1) == 3)

def f()
    var defer = 10
    var ret = []
    for i:0..100
        if (defer := defer - 1) == 0
            ret.push(i)
            defer = 10
        end
    end
    return ret
end
assert(f() == [9, 19, 29, 39, 49, 59, 69, 79, 89, 99])

# test for expressions with no side effects
def assert_attribute_error(c)
  try
      compile(c)()
      assert(false, 'unexpected execution flow')
  except 'syntax_error' as e, m
  end
end

# below the expressions `a` or `a b` have no side effect and do not generate any code, this is an error when in strict mode
import strict
var a, b
assert_attribute_error("var a,b def f() a b end")
assert_attribute_error("var a,b def f() a end")

# while the following does have side effect
def f() a := b end

# bug when using walrus with member
def id(x) return x end
var a = 1
import global
def f() return id(global.a := 42) end
assert(f() == 42)
# bug: returns <module: global>

def concat(x, y, z) return str(x)+str(y)+str(z) end
var a = 1
import global
def f() return concat(global.a := 1, global.a := 42, global.a := 0) end
assert(f() == "1420")
# bug: returns '1<module: global>42'

# same bug when using index
def id(x) return x end
l = [10,11]
import global
def f() return id(global.l[0] := 42) end
assert(f() == 42)
# bug: returns [42, 11]

# bug when using member for self
class confused_walrus
    var b
    def f()
        var c = 1
        if self.b := true
            c = 2
        end
        return self
    end
end
var ins = confused_walrus()
assert(ins.f() == ins)

# Check overwriting a builtin (https://github.com/berry-lang/berry/issues/416)

def check_overwrite_builtin()
    print := 1
    assert(print == 1)
end

check_overwrite_builtin()
