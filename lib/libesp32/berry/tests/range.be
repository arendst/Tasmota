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

# test ranges
var l

# simple ranges
l = []
for i:1..10 l..i end
assert(l == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

l = []
for i:range(1, 10) l..i end
assert(l == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10])

# range in higher steps
l = []
for i:range(1, 10, 2) l..i end
assert(l == [1, 3, 5, 7, 9])

# negative ranges
l = []
for i:range(1, 10, -2) l..i end
assert(l == [])

l = []
for i:range(30, 0, -3) l..i end
assert(l == [30, 27, 24, 21, 18, 15, 12, 9, 6, 3, 0])

# lower(), upper(), incr() accessors
var r = range(3, 7)
assert(r.lower() == 3)
assert(r.upper() == 7)
assert(r.incr() == 1)

var r2 = range(0, 20, 5)
assert(r2.lower() == 0)
assert(r2.upper() == 20)
assert(r2.incr() == 5)

var r3 = range(10, 0, -2)
assert(r3.lower() == 10)
assert(r3.upper() == 0)
assert(r3.incr() == -2)

# tostring - incr == 1 uses "(lower..upper)" format
assert(str(range(1, 5)) == "(1..5)")

# tostring - incr != 1 uses "range(lower, upper, incr)" format
assert(str(range(0, 10, 3)) == "range(0, 10, 3)")
assert(str(range(10, 0, -1)) == "range(10, 0, -1)")

# setrange - basic usage
var r4 = range(1, 5)
r4.setrange(10, 20)
assert(r4.lower() == 10)
assert(r4.upper() == 20)
assert(r4.incr() == 1)

# setrange - with custom increment
r4.setrange(0, 100, 10)
assert(r4.lower() == 0)
assert(r4.upper() == 100)
assert(r4.incr() == 10)

# setrange - verify iteration after setrange
l = []
var r5 = range(1, 3)
r5.setrange(5, 9, 2)
for i:r5 l..i end
assert(l == [5, 7, 9])

# init error: missing arguments
try
    range(1)
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end

# init error: non-int arguments
try
    range(1, 2.0)
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end

# init error: zero increment
try
    range(1, 10, 0)
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end

# setrange error: missing arguments
try
    var r6 = range(1, 5)
    r6.setrange(1)
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end

# setrange error: non-int arguments
try
    var r7 = range(1, 5)
    r7.setrange(1, "bad")
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end

# setrange error: zero increment
try
    var r8 = range(1, 5)
    r8.setrange(1, 10, 0)
    assert(false, "should raise")
except .. as e, m
    assert(e == 'value_error')
end
