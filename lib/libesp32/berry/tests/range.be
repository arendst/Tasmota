# test for ranges

# expand a range object as list
def expand(iter)
    var ret = []
    for i: iter
        ret.push(i)
    end
    return ret
end

assert(expand(0..5) == [0, 1, 2, 3, 4, 5])
assert(expand(0..0) == [0])
assert(expand(5..0) == [])
var r = 1..5
assert(r.lower() == 1)
assert(r.upper() == 5)
assert(r.incr() == 1)

assert(expand(range(0,5)) == [0, 1, 2, 3, 4, 5])
assert(expand(range(0,5,2)) == [0, 2, 4])
assert(expand(range(0,5,12)) == [0])
assert(expand(range(0,5,-1)) == [])

assert(expand(range(5,0,-1)) == [5, 4, 3, 2, 1, 0])
assert(expand(range(5,0,-2)) == [5, 3, 1])
assert(expand(range(5,5,-2)) == [5])
assert(expand(range(0,5,-2)) == [])

def assert_value_error(c)
    try
        compile(c)()
        assert(false, 'unexpected execution flow')
    except 'value_error' as e, m
    end
end

# range with increment zero shoud raise an error
assert_value_error("range(1,2,0)")
