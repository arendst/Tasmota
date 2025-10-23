# Test parser-specific bug fixes

# Test issue #396 - ternary operator in assignment
def f()
    if true
        var a = 1
        a = true ? a+1 : a+2  # Ternary in assignment
        return a
    end
end
assert(f() == 2)

# Test parser error from Feb 2025
def parse_022025()
    var s, value
    var js = {'a':{'a':1}}
    value = js['a']['a']  # Nested map access

    if value != nil
        for x:0..1
            return x
        end
    end
end
assert(parse_022025() == 0)
