
# Test division by zero error handling

try
    # Test integer division by zero
    var div = 1/0
    assert(false) # Should not reach this point
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

try
    # Test integer modulo by zero
    var div = 1%0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

try
    # Test float division by zero
    var div = 1.1/0.0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

try
    # Test float modulo by zero
    var div = 1.1%0.0
    assert(false)
except .. as e,m
    assert(e == "divzero_error")
    assert(m == "division by zero")
end

# Test normal division & modulo operations
assert(1/2 == 0)
assert(1%2 == 1)
assert(1.0/2.0 == 0.5)
assert(1.0%2.0 == 1.0)
assert(4/2 == 2)
assert(4%2 == 0)
