
try
    # Test integer division
    var div = 1/0
    assert(false) # Should not reach this point
except .. as e,m
    
    assert(e == "divzero_error")
    assert(m == "division by zero")
end


try
    # Test integer modulo
    var div = 1%0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

try
    # Test float division
    var div = 1.1/0.0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

try
    # Test float modulo
    var div = 1.1%0.0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end


# Check normal division & modulo
assert(1/2 == 0)
assert(1%2 == 1)
assert(1.0/2.0 == 0.5)
assert(1.0%2.0 == 1.0)
assert(4/2 == 2)
assert(4%2 == 0)
