# zigbee zcl_frame class
#
# solidify.dump(zcl_frame,true)

var zcl_frame_ntv = super(zcl_frame)

class zcl_frame : zcl_frame_ntv
  var no_bytes

  def init(p)
    super(self).init(p)
    self.no_bytes = bytes()
  end

  def member(name)
    if name == "payload"
      return self._get_bytes()
    else
      return super(self).member(name)
    end
  end

  # return a bytes() object from a pre-populated zcl_frame payload
  def _get_bytes()
    import introspect
    var payload_ptr = introspect.toptr(self.payload_ptr)
    if int(payload_ptr) != 0
      var sbuffer = bytes(payload_ptr, -4)
      var sbuffer_len = sbuffer.get(2,2)
      
      return bytes(introspect.toptr(int(payload_ptr) + 4), sbuffer_len)
    else
      return self.no_bytes
    end
  end

  def tomap()
    import string
    var m = super(self).tomap()
    m["shortaddr_hex"] = string.format("0x%04X", self.shortaddr)
    m["payload"] = self.payload    # add payload object which is a synthetic attribute not included in tomap()
    return m
  end

  def tostring()
    var m = self.tomap()
    return str(m)
  end

end