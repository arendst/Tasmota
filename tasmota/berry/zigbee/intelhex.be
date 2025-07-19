# parse intelhex file
#
# use: `import intelhex`
#
# This class allows to open and parse an Intel HEX format file.
# Basic checks are done to make sure that the format is valid.
#
# If anything goes wrong, an exception is raised.
#
# You provide 3 callbacks:
# - pre: method called when file was opened, but content not parsed - no arg, no return (raise an exception if error)
#        this is a good place to erase flash before writing
# - cb:  method called for each line containing data to be flashed. args(address:int, len_in_bytes:int, data:bytes, offset:int)
#            address: full address in Flash (high and low are automatically combined)
#            len_in_bytes: length of the data line, generally max 32 bytes
#            data: bytes() object containing raw bytes
#            offset: offset in bytes() object, skipping first bytes without needing to reallocate buffer
# - post: method called when all data was parsed, and before the file is closed - no arg, no return (raise an exception if error)
#         this is a good place to check CRC32 or any checksum
#

class intelhex
  var filename          # filename of hex file
  var f                 # file object
  var file_parsed       # was the file already parsed. It cannot be flashed if not previously parsed and validated
  var file_validated    # was the file already validated. It cannot be flashed if not previously parsed and validated

  def init(filename)
    self.filename = str(filename)
    self.file_parsed = false
    self.file_validated = true
  end

  def close()
    if self.f != nil
      self.f.close()
      self.f = nil
    end
  end

  # open the intelhex file and parse from start to end
  #
  # Args:
  # - pre: method called when file was opened, but content not parsed - no arg, no return (raise an exception if error)
  #        this is a good place to erase flash before writing
  # - cb:  method called for each line containing data to be flashed. args(address:int, len_in_bytes:int, data:bytes, offset:int)
  #            address: full address in Flash (high and low are automatically combined)
  #            len_in_bytes: length of the data line, generally max 32 bytes
  #            data: bytes() object containing raw bytes
  #            offset: offset in bytes() object, skipping first bytes without needing to reallocate buffer
  # - post: method called when all data was parsed, and before the file is closed - no arg, no return (raise an exception if error)
  #         this is a good place to check CRC32 or any checksum
  #
  def parse(pre, parse_cb, post)
    try
      self.f = open(self.filename, "rb")
      self.file_parsed = true                  # we tried to parse
      pre()

      self.munch_line(parse_cb)

      post()                              # validated is computed internally

    except .. as e, m
      self.close()
      self.file_validated = false
      raise e, m      # re-raise
    end

    self.close()
  end

  # internally used, verify each line
  def munch_line(parse_cb)
    import string
    import crc
    var crc_sum = crc.sum
    var tas = tasmota
    var yield = tasmota.yield

    var offset_high = 0
    var offset_low  = 0
    var b = bytes()
    var b_get = b.get
    var b_fromhex = b.fromhex
    var self_f = self.f
    var readline = self_f.readline
    var defer = 10
    while true
      defer = defer - 1
      if defer <= 0
        yield(tas)        # tasmota.yield() -- faster version
        defer = 10
      end
      var line = readline(self_f)   # self.f.readline()
      if line[-1] == '\n'   line = line[0..-2]  end
      if line[-1] == '\r'   line = line[0..-2]  end
      
      # line = string.tr(line, '\r', '')
      # line = string.tr(line, '\n', '')
      # print(line)
      if line == ""   raise "value_error", "unexpected end of file" end
      if line[0] != ":"   continue end    # ignore empty line or not starting with ':'
      b = b_fromhex(b, line, 1)           # b.fromhex(line, 1)      # convert to bytes, avoid allocating a new object
      var sz = b[0]

      # check size
      if size(b) != sz+5   raise "value_error", f"invalid size for line: {line} {size(b)=} {sz=}"  end

      var record_type = b[3]
      # 00: low address + data
      # 01: end of file
      # 02: Extended Segment Address
      # 04: high address
      if record_type != 0 && record_type != 1 && record_type != 2 && record_type != 4
        raise "value_error", f"unsupported record_type: {record_type} {line=}"
      end

      offset_low = b_get(b, 1, -2)                    # b.get(1,-2)
      var checksum = crc_sum(b)
      if checksum != 0   raise "value_error", f"invalid checksum 0x{checksum:02X}" end

      if record_type == 1    break end    # end of file
      if record_type == 0
        # data
        var address = offset_high + offset_low    # full address
        #var data = b[4..-2]                             # actual payload
        parse_cb(address, sz, b, 4)

        # OK
        # do whatever needed
        # print(format("addr=0x%06X len=0x%02X", address, sz))
      elif record_type == 2
        if offset_low != 0      raise "value_error", "offset_low not null for cmd 02" end
        offset_high = b_get(b, 4, -2) << 4
      elif record_type == 4
        if offset_low != 0      raise "value_error", "offset_low not null for cmd 04" end
        offset_high = b_get(b, 4, -2) << 16

      end
    end
  end

end

return intelhex
