#-------------------------------------------------------------
 - Parser for ESP32 partition
 -
 -------------------------------------------------------------#
partition = module('partition')

import flash
import string

def remove_trailing_zeroes(b)
  while size(b) > 0
    if b.get(size(b)-1,1) == 0
      b.resize(size(b)-1)
    else
      break
    end
  end
  return b
end

#-------------------------------------------------------------
 - Class for a partition table entry
 -------------------------------------------------------------#
class Partition_info
  var type
  var subtype
  var start
  var size
  var label
  var flags

  def init(raw)
    import string

    if raw == nil || !issubclass(bytes, raw)
      self.type = 0
      self.subtype = 0
      self.start = 0
      self.size = 0
      self.label = ''
      self.flags = 0
      return
    end

    #- parse -#
    var magic = raw.get(0,2)
    if magic == 0x50AA    #- partition entry -#

      self.type = raw.get(2,1)
      self.subtype = raw.get(3,1)
      self.start = raw.get(4,4)
      self.size = raw.get(8,4)
      self.label = remove_trailing_zeroes(raw[12..27]).asstring()
      self.flags = raw.get(28,4)

    elif magic == 0xEBEB  #- MD5 -#
    else
      raise  "internal_error", string.format("invalid magic number %02X", magic)
    end
    
  end

  def tostring()
    import string
    return string.format("<instance: Partition_info(%d,%d,0x%08X,0x%08X,'%s',0x%X)>",
                          self.type, self.subtype, self.start, self.size,
                          self.label, self.flags)
  end

  def tobytes()
    #- convert to raw bytes -#
    var b = bytes('AA50')     #- set magic number -#
    b.resize(32).resize(2)    #- pre-reserve 32 bytes -#
    b.add(self.type, 1)
    b.add(self.subtype, 1)
    b.add(self.start, 4)
    b.add(self.size, 4)
    var label = bytes().fromstring(self.label)
    label.resize(16)
    b = b + label
    b.add(self.flags, 4)
    return b
  end

end

#-------------------------------------------------------------
 - Class for a partition table entry
 -------------------------------------------------------------#
class Partition
  var raw   #- raw bytes of the partition table in flash -#
  var md5   #- md5 hash of partition list -#
  var slots

  def init()
    self.slots = []
    self.load()
    self.parse()
  end

  def load()
    #- load partition table from flash -#
    self.raw = flash.read(0x8000,0x1000)
  end

  #- parse the raw bytes to a structured list of partition items -#
  def parse()
    var i
    for i:0..94       # there are maximum 95 slots + md5 (0xC00)
      var item_raw = self.raw[i*32..(i+1)*32-1]
      var magic = item_raw.get(0,2)
      if magic == 0x50AA    #- partition entry -#
        var slot = Partition_info(item_raw)
        self.slots.push(slot)
      elif magic == 0xEBEB  #- MD5 -#
        self.md5 = self.raw[i*32+16..i*33-1]
        break
      else
        break
      end
    end
  end

  #- convert to human readble -#
  def tostring()
    var ret = "<instance: Partition([\n"
    for slot: self.slots
      ret += "  "
      ret += slot.tostring()
      ret += "\n"
    end
    ret += "])"
    return ret
  end

  #- convert the slots to raw bytes, ready to falsh to parition page -#
  def tobytes()
    if size(self.slots) > 95 raise "value_error", "Too many partiition slots" end
    var b = bytes()
    for slot: self.slots
      b += slot.tobytes()
    end
    #- compute MD5 -#
    var md5 = MD5()
    md5.update(b)
    #- add the last segment -#
    b += bytes("EBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFF")
    b += md5.finish()
    #- complete -#
    return b
  end

  #- write back to flash -#
  def save()
    var b = self.tobytes()
    #- erase flash area and write -#
    flash.erase(0x8000,0x1000)
    flash.write(0x8000, b)
  end
end

partition.Partition = Partition

return partition
#Example
#-

import partition
Partition = partition.Partition

# read
p = Partition()
print(p)

-#