
_energy = nil   # avoid compilation error
energy_struct = nil

energy = module("energy")
energy._ptr = nil

def init(m)
    import global
    global._energy = energy_struct(m._ptr)
    return m
end
energy.init = init

def read()
    return _energy.tomap()
end
energy.read = read

def member(k)
    return _energy.(k)
end
energy.member = member

def setmember(k, v)
    _energy.(k) = v
end
energy.setmember = setmember

#@ solidify:energy.init
#@ solidify:energy.read
#@ solidify:energy.member
#@ solidify:energy.setmember
