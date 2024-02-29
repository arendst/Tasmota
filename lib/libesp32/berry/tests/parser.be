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
