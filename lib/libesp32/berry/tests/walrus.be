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
