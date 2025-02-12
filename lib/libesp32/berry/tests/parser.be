# Test some sparser specific bugs

# https://github.com/berry-lang/berry/issues/396
def f()
    if true
        var a = 1
        a = true ? a+1 : a+2
        return a
    end
end
assert(f() == 2)

# Parser error reported in Feb 2025
def parse_022025()
    var s, value
    var js = {'a':{'a':1}}
    value = js['a']['a']

    if value != nil
        for x:0..1
            return x
        end
    end
end
assert(parse_022025() == 0)
