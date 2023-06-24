
_energy = nil   # avoid compilation error
energy_struct = nil

energy = module("energy")
energy._ptr = nil

def init(m)
    global._energy = nil
    return m
end
energy.init = init

def _deref()            # dereference enery pointer
    if global._energy == nil
        import introspect
        var p = bytes(energy._ptr, 4).get(0, 4)      # dereference pointer
        if p != 0
            global._energy = energy_struct(introspect.toptr(p))
        end
    end
end
energy._deref = _deref

def read()
    energy._deref()
    if global._energy
        return _energy.tomap()
    end
end
energy.read = read

def member(k)
    energy._deref()
    if global._energy
        return _energy.(k)
    end
end
energy.member = member

def setmember(k, v)
    energy._deref()
    if global._energy
        _energy.(k) = v
    end
end
energy.setmember = setmember

#@ solidify:energy.init
#@ solidify:energy._deref
#@ solidify:energy.read
#@ solidify:energy.member
#@ solidify:energy.setmember
