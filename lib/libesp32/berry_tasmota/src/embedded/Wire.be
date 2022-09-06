#- Native code used for testing and code solidification -#
#- Do not use it -#

class Wire
  var bus

  def read_bytes(addr,reg,sz)
    self._begin_transmission(addr)
    self._write(reg)
    self._end_transmission(false)
    self._request_from(addr,sz)
    var ret=bytes(sz)
    while (self._available())
      ret..self._read()
    end
    return ret
  end
  
  def write_bytes(addr,reg,b)
    self._begin_transmission(addr)
    self._write(reg)
    self._write(b)
    self._end_transmission()
  end
end
