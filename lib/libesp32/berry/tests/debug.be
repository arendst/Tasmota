# Test debug module functionality
import debug

class A end
debug.attrdump(A)   # Should not crash

# Test debug.caller() function
def caller_name_chain()
    import debug
    import introspect
    var i = 1
    var ret = []
    var caller = debug.caller(i)
    while caller
        ret.push(introspect.name(caller))
        i += 1
        caller = debug.caller(i)
    end
    return ret
end
var chain = caller_name_chain()
assert(chain[0] == 'caller_name_chain')

def guess_my_name__()
    return caller_name_chain()
end
chain = guess_my_name__()
print(chain)
assert(chain[0] == 'caller_name_chain')
assert(chain[1] == 'guess_my_name__')

# debug.caller() must not crash on out-of-range or pathological inputs.
# Returns nil (or some valid frame) but never blows the stack / segfaults.
def assert_no_crash(d)
    debug.caller(d)  # value irrelevant, just must not crash
end
assert_no_crash(0)
assert_no_crash(-1)
assert_no_crash(99999)
assert_no_crash(-99999)
# INT_MIN-style edge: largest negative int that fits in a Berry int.
# On 32-bit bint this is -0x80000000, the historical -INT_MIN UB trigger.
assert_no_crash(-0x7FFFFFFF - 1)

# debug.varname() must name every live local, also when a local is
# declared right before a block and its value is computed into its register.
# The API is absent when BE_DEBUG_VAR_INFO is disabled.
import introspect
if introspect.contains(debug, 'varname')
    def varnames()      # names of the caller's locals, by register
        var r = []
        var i = 0
        var n = debug.varname(i, 2)
        while n != nil
            r.push(n)
            i += 1
            n = debug.varname(i, 2)
        end
        return r
    end
    def vi_while(a)
        var n = a + 1
        while n > 10 n -= 1 end
        var t = 5
        return varnames()
    end
    assert(vi_while(12) == ['a', 'n', 't'])     # was ['a', 't']
    def vi_for(a)
        var n = a + 1
        for i : 0 .. 1 n += i end
        var t = 5
        return varnames()
    end
    assert(vi_for(12) == ['a', 'n', 't'])       # was ['a', 't']
    def vi_do(a)
        var n = a * 2
        do n += 1 end
        var t = 5
        return varnames()
    end
    assert(vi_do(12) == ['a', 'n', 't'])        # was ['a', 't']
    def vi_param(a)     # the body starts with a block
        while a > 10 a -= 1 end
        var t = 5
        return varnames()
    end
    assert(vi_param(12) == ['a', 't'])          # was ['t']
    def vi_nested(a)
        var r
        for i : 0 .. 0
            var m = i + a
            while m > 20 m -= 1 end
            var u = 1
            r = varnames()
        end
        return r
    end
    assert(vi_nested(12) == ['a', 'r', '.it', 'i', 'm', 'u'])   # was ['a', 'r', '.it', 'i', 'u']
end
