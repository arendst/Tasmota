# Test range objects and iteration

# Helper function to expand range into list
def expand(iter)
    var ret = []
    for i: iter
        ret.push(i)
    end
    return ret
end

# Test basic range syntax
assert(expand(0..5) == [0, 1, 2, 3, 4, 5])
assert(expand(0..0) == [0])
assert(expand(5..0) == [])  # Invalid range

# Test range methods
var r = 1..5
assert(r.lower() == 1)
assert(r.upper() == 5)
assert(r.incr() == 1)

# Test range() function with increment
assert(expand(range(0,5)) == [0, 1, 2, 3, 4, 5])
assert(expand(range(0,5,2)) == [0, 2, 4])
assert(expand(range(0,5,12)) == [0])
assert(expand(range(0,5,-1)) == [])

# Test negative increment
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

# Test error handling - zero increment should raise error
assert_value_error("range(1,2,0)")
