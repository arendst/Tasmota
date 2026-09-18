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
