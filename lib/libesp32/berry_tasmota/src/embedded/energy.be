
_energy = nil   # avoid compilation error
energy_struct = nil

#@ solidify:bytes_array
# implement an array of simple types
class bytes_array : bytes
    var item_type               # item_type number for each element
    var sz                      # size (number of items in the array)
    var item_size               # size in bytes of each element (inferred from item_type)

    def init(ptr, item_type, sz)
        self.item_type = item_type
        self.sz = sz
        var item_size           # size in bytes of each element
        if   (item_type >= -4) && (item_type <= 4) && (item_type != 0)
            item_size = (item_type > 0) ? item_type : - item_type
        elif (item_type == 5)
            item_size = 4
        else
            raise "value error", "unsupported item_type number"
        end
        self.item_size = item_size
        super(self).init(ptr, self.item_size * self.sz)
    end

    def item(idx)
        if (idx < 0)    idx += self.sz       end
        if (idx < 0) || (idx >= self.sz)
            raise "index_error", "list index out of range"
        end
        if   (self.item_type == 5)
            return self.getfloat(idx * self.item_size)
        else
            return self.get(idx * self.item_size, self.item_type)
        end
    end

    def setitem(idx, v)
        if (idx < 0)    idx += self.sz       end
        if (idx < 0) || (idx >= self.sz)
            raise "index_error", "list assignment index out of range"
        end
        if   (self.item_type == 5)
            return self.setfloat(idx * self.item_size, v)
        else
            return self.set(idx * self.item_size, self.item_type, v)
        end
    end

    def size()
        return self.sz
    end

    def tostring()
        var ret = '['
        var idx = 0
        while (idx < self.sz)
            if idx > 0
                ret += ','
            end
            #
            if   (self.item_type == 5)
                ret += str(self.getfloat(idx * self.item_size))
            else
                ret += str(self.get(idx * self.item_size, self.item_type))
            end

            idx += 1
        end
        ret += ']'
        return ret
    end
end

#@ solidify:energy_phases_float
class energy_phases_float : bytes_array
    def init(ptr)
        import energy
        super(self).init(ptr, 5 #-type float-#, energy._phases)
    end
end


#@ solidify:energy_phases_int32
class energy_phases_int32 : bytes_array
    def init(ptr)
        import energy
        super(self).init(ptr, 4 #-type int32-#, energy._phases)
    end
end

#@ solidify:energy_phases_uint8
class energy_phases_uint8 : bytes_array
    def init(ptr)
        import energy
        super(self).init(ptr, 1 #-type int32-#, energy._phases)
    end
end

#@ solidify:energy_phases_uint16
class energy_phases_uint16 : bytes_array
    def init(ptr)
        import energy
        super(self).init(ptr, 2 #-type uint16-#, energy._phases)
    end
end

energy = module("energy")
energy._ptr = nil               # will be replaced by C code
energy._phases = 8              # will be replaced by C code
energy._phases_float = energy_phases_float
energy._phases_int32 = energy_phases_int32
energy._phases_uint8 = energy_phases_uint8
energy._phases_uint16 = energy_phases_uint16

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

def tomap()
    energy._deref()
    if global._energy
        return _energy.tomap()
    end
end
energy.tomap = tomap

#@ solidify:energy.init
#@ solidify:energy._deref
#@ solidify:energy.read
#@ solidify:energy.member
#@ solidify:energy.setmember
#@ solidify:energy.tomap
