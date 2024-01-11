import debug

class A end
debug.attrdump(A)   #- should not crash -#

# debug.caller()
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
