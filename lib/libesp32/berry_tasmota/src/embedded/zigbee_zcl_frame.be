# zigbee zcl_frame class
#
# solidify.dump(zcl_frame,true)

class zcl_frame_ntv end

#@ solidify:zcl_frame,weak
class zcl_frame : zcl_frame_ntv
  var no_bytes

  def init(p)
    super(self).init(p)
    self.no_bytes = bytes()
  end

  def member(name)
    if name == "payload"
      return self._get_bytes(self.payload_ptr)
    else
      return super(self).member(name)
    end
  end

  def setmember(name, val)
    if name == "payload"
      return self._set_bytes(self.payload_ptr, val)
    else
      return super(self).setmember(name, val)
    end
  end

  def tomap()
    var m = super(self).tomap()
    m["shortaddr_hex"] = format("0x%04X", self.shortaddr)
    m["payload"] = self.payload    # add payload object which is a synthetic attribute not included in tomap()
    return m
  end

  def tostring()
    var m = self.tomap()
    return str(m)
  end

end