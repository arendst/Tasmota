#- Native code used for testing and code solidification -#
#- Do not use it -#

#@ solidify:Wire.read_bytes
#@ solidify:Wire.write_bytes
class Wire
  var bus

  def read_bytes(addr, reg, sz)
    self._begin_transmission(addr)
    self._write(reg)
    self._end_transmission(true)
    self._request_from(addr, sz)
    var ret = bytes(sz)
    while (self._available())
      ret.append(self._read())
    end
    self._end_transmission(true)
    return ret
  end
  
  def write_bytes(addr,reg,b)
    self._begin_transmission(addr)
    self._write(reg)
    self._write(b)
    self._end_transmission()
  end
end
