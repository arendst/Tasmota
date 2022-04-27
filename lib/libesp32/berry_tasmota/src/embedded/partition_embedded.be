#######################################################################
# Partition manager for ESP32 - ESP32C3 - ESP32S2
#
# use : `import partition`
#
# To solidify:
#-
   import solidify load("partition_embedded.be") solidify.dump(partition)
-#
# Provides low-level objects and a Web UI
#######################################################################

var partition = module('partition')

#######################################################################
# Class for a partition table entry
#
#   typedef struct {
#       uint16_t magic;
#       uint8_t  type;
#       uint8_t  subtype;
#       uint32_t offset;
#       uint32_t size;
#       uint8_t  label[16];
#       uint32_t flags;
#   } esp_partition_info_t_simplified;
#
#######################################################################
class Partition_info
  var type
  var subtype
  var start
  var size
  var label
  var flags

  #- remove trailing NULL chars from a bytes buffer before converting to string -#
  #- Berry strings can contain NULL, but this messes up C-Berry interface -#
  static def remove_trailing_zeroes(b)
    var sz = size(b)
    var i = 0
    while i < sz
      if b[-1-i] != 0   break end
      i += 1
    end
    if i > 0
      b.resize(size(b)-i)
    end
    return b
  end
    
  # Init the Parition information structure, either from a bytes() buffer or an empty if no buffer is provided
  def init(raw)
    self.type = 0
    self.subtype = 0
    self.start = 0
    self.size = 0
    self.label = ''
    self.flags = 0

    if !issubclass(bytes, raw)    # no payload, empty partition information
      return
    end

    #- we have a payload, parse it -#
    var magic = raw.get(0,2)
    if magic == 0x50AA    #- partition entry -#

      self.type = raw.get(2,1)
      self.subtype = raw.get(3,1)
      self.start = raw.get(4,4)
      self.size = raw.get(8,4)
      self.label = self.remove_trailing_zeroes(raw[12..27]).asstring()
      self.flags = raw.get(28,4)

    # elif magic == 0xEBEB  #- MD5 -#
    else
      import string
      raise  "internal_error", string.format("invalid magic number %02X", magic)
    end
    
  end

  # check if the parition is an OTA partition
  # if yes, return OTA number (starting at 0)
  # if no, return nil
  def is_ota()
    var sub_type = self.subtype
    if self.type == 0 && (sub_type >= 0x10 && sub_type < 0x20)
      return sub_type - 0x10
    end
  end

  # check if the parition is a SPIFFS partition
  # returns bool
  def is_spiffs()
    return self.type == 1 && self.subtype == 130
  end

  # get the actual image size give of the partition
  # returns -1 if the partition is not an app ota partition
  def get_image_size()
    import flash
    if self.is_ota() == nil return -1 end
    try
      var addr = self.start
      var magic_byte = flash.read(addr, 1).get(0, 1)
      if magic_byte != 0xE9 return -1 end
      
      var seg_count = flash.read(addr+1, 1).get(0, 1)
      # print("Segment count", seg_count)
      
      var seg_offset = addr + 0x20 # sizeof(esp_image_header_t) + sizeof(esp_image_segment_header_t) = 24 + 8

      var seg_num = 0
      while seg_num < seg_count
        # print(string.format("Reading 0x%08X", seg_offset))
        var segment_header = flash.read(seg_offset - 8, 8)
        var seg_start_addr = segment_header.get(0, 4)
        var seg_size = segment_header.get(4,4)
        # print(string.format("Segment %i: flash_offset=0x%08X start_addr=0x%08X size=0x%08X", seg_num, seg_offset, seg_start_addr, seg_size))

        seg_offset += seg_size + 8    # add segment_length + sizeof(esp_image_segment_header_t)

        seg_num += 1
      end
      var total_size = seg_offset - addr + 1 # add 1KB for safety

      # print(string.format("Total size = %i KB", total_size/1024))

      return total_size
    except .. as e, m
      tasmota.log("BRY: Exception> '" + e + "' - " + m, 2)
      return -1
    end
  end

  # Human readable version of Partition information
  # this method is not included in the solidified version to save space,
  # it is included only in the optional application `tapp` version
  def tostring()
    import string
    var type_s = ""
    var subtype_s = ""
    if   self.type == 0  type_s = "app"
      if   self.subtype == 0  subtype_s = "factory"
      elif self.subtype >= 0x10 && self.subtype < 0x20 subtype_s = "ota" + str(self.subtype - 0x10)
      elif self.subtype == 0x20  subtype_s = "test"
      end
    elif self.type == 1  type_s = "data"
      if   self.subtype == 0x00  subtype_s = "otadata"
      elif self.subtype == 0x01  subtype_s = "phy"
      elif self.subtype == 0x02  subtype_s = "nvs"
      elif self.subtype == 0x03  subtype_s = "coredump"
      elif self.subtype == 0x04  subtype_s = "nvskeys"
      elif self.subtype == 0x05  subtype_s = "efuse_em"
      elif self.subtype == 0x80  subtype_s = "esphttpd"
      elif self.subtype == 0x81  subtype_s = "fat"
      elif self.subtype == 0x82  subtype_s = "spiffs"
      end
    end

    # reformat strings
    if type_s != ""    type_s = " (" + type_s + ")" end
    if subtype_s != "" subtype_s = " (" + subtype_s + ")" end
    return string.format("<instance: Partition_info(%d%s,%d%s,0x%08X,0x%08X,'%s',0x%X)>",
                          self.type, type_s,
                          self.subtype, subtype_s,
                          self.start, self.size,
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
partition.Partition_info = Partition_info

#-------------------------------------------------------------
 - OTA Data
 -
 - Selection of the active OTA partition
 -
    typedef struct {
        uint32_t ota_seq;
        uint8_t  seq_label[20];
        uint32_t ota_state;
        uint32_t crc; /* CRC32 of ota_seq field only */
    } esp_ota_select_entry_t;

  - Excerp from esp_ota_ops.c
      esp32_idf use two sector for store information about which partition is running
      it defined the two sector as ota data partition,two structure esp_ota_select_entry_t is saved in the two sector
      named data in first sector as otadata[0], second sector data as otadata[1]
      e.g.
      if otadata[0].ota_seq == otadata[1].ota_seq == 0xFFFFFFFF,means ota info partition is in init status
      so it will boot factory application(if there is),if there's no factory application,it will boot ota[0] application
      if otadata[0].ota_seq != 0 and otadata[1].ota_seq != 0,it will choose a max seq ,and get value of max_seq%max_ota_app_number
      and boot a subtype (mask 0x0F) value is (max_seq - 1)%max_ota_app_number,so if want switch to run ota[x],can use next formulas.
      for example, if otadata[0].ota_seq = 4, otadata[1].ota_seq = 5, and there are 8 ota application,
      current running is (5-1)%8 = 4,running ota[4],so if we want to switch to run ota[7],
      we should add otadata[0].ota_seq (is 4) to 4 ,(8-1)%8=7,then it will boot ota[7]
      if      A=(B - C)%D
      then    B=(A + C)%D + D*n ,n= (0,1,2...)
      so current ota app sub type id is x , dest bin subtype is y,total ota app count is n
      seq will add (x + n*1 + 1 - seq)%n
 -------------------------------------------------------------#
class Partition_otadata
  var maxota          #- number of highest OTA partition, default 1 (double ota0/ota1) -#
  var offset          #- offset of the otadata partition (0x2000 in length), default 0xE000 -#
  var active_otadata  #- which otadata block is active, 0 or 1, i.e. 0xE000 or 0xF000 -#
  var seq0            #- ota_seq of first block -#
  var seq1            #- ota_seq of second block -#

  # the following is not included in the solidified version
  #-------------------------------------------------------------
  - Simple CRC32 imple
  -
  - adapted from Python https://rosettacode.org/wiki/CRC-32#Python
  -------------------------------------------------------------#
  # static def crc32_create_table()
  #   var a = []
  #   for i:0..255
  #     var k = i
  #     for j:0..7
  #       if k & 1
  #         k = (k >> 1) & 0x7FFFFFFF
  #         k ^= 0xedb88320
  #       else
  #         k = (k >> 1) & 0x7FFFFFFF
  #       end
  #     end
  #     a.push(k)
  #   end
  #   return a
  # end
  # static crc32_table = Partition_otadata.crc32_create_table()

  # comptued in advanced
  static crc32_table = [
    0, 1996959894, -301047508, -1727442502, 124634137, 1886057615, -379345611, -1637575261,
    249268274, 2044508324, -522852066, -1747789432, 162941995, 2125561021, -407360249, -1866523247,
    498536548, 1789927666, -205950648, -2067906082, 450548861, 1843258603, -187386543, -2083289657,
    325883990, 1684777152, -43845254, -1973040660, 335633487, 1661365465, -99664541, -1928851979,
    997073096, 1281953886, -715111964, -1570279054, 1006888145, 1258607687, -770865667, -1526024853,
    901097722, 1119000684, -608450090, -1396901568, 853044451, 1172266101, -589951537, -1412350631,
    651767980, 1373503546, -925412992, -1076862698, 565507253, 1454621731, -809855591, -1195530993,
     671266974, 1594198024, -972236366, -1324619484, 795835527, 1483230225, -1050600021, -1234817731,
     1994146192, 31158534, -1731059524, -271249366, 1907459465, 112637215, -1614814043, -390540237,
     2013776290, 251722036, -1777751922, -519137256, 2137656763, 141376813, -1855689577, -429695999,
     1802195444, 476864866, -2056965928, -228458418, 1812370925, 453092731, -2113342271, -183516073,
     1706088902, 314042704, -1950435094, -54949764, 1658658271, 366619977, -1932296973, -69972891,
     1303535960, 984961486, -1547960204, -725929758, 1256170817, 1037604311, -1529756563, -740887301,
     1131014506, 879679996, -1385723834, -631195440, 1141124467, 855842277, -1442165665, -586318647,
     1342533948, 654459306, -1106571248, -921952122, 1466479909, 544179635, -1184443383, -832445281,
     1591671054, 702138776, -1328506846, -942167884, 1504918807, 783551873, -1212326853, -1061524307,
     -306674912, -1698712650, 62317068, 1957810842, -355121351, -1647151185, 81470997, 1943803523,
     -480048366, -1805370492, 225274430, 2053790376, -468791541, -1828061283, 167816743, 2097651377,
     -267414716, -2029476910, 503444072, 1762050814, -144550051, -2140837941, 426522225, 1852507879,
     -19653770, -1982649376, 282753626, 1742555852, -105259153, -1900089351, 397917763, 1622183637,
     -690576408, -1580100738, 953729732, 1340076626, -776247311, -1497606297, 1068828381, 1219638859,
     -670225446, -1358292148, 906185462, 1090812512, -547295293, -1469587627, 829329135, 1181335161,
     -882789492, -1134132454, 628085408, 1382605366, -871598187, -1156888829, 570562233, 1426400815,
     -977650754, -1296233688, 733239954, 1555261956, -1026031705, -1244606671, 752459403, 1541320221,
     -1687895376, -328994266, 1969922972, 40735498, -1677130071, -351390145, 1913087877, 83908371,
     -1782625662, -491226604, 2075208622, 213261112, -1831694693, -438977011, 2094854071, 198958881,
     -2032938284, -237706686, 1759359992, 534414190, -2118248755, -155638181, 1873836001, 414664567,
     -2012718362, -15766928, 1711684554, 285281116, -1889165569, -127750551, 1634467795, 376229701,
     -1609899400, -686959890, 1308918612, 956543938, -1486412191, -799009033, 1231636301, 1047427035,
     -1362007478, -640263460, 1088359270, 936918000, -1447252397, -558129467, 1202900863, 817233897,
     -1111625188, -893730166, 1404277552, 615818150, -1160759803, -841546093, 1423857449, 601450431,
     -1285129682, -1000256840, 1567103746, 711928724, -1274298825, -1022587231, 1510334235, 755167117]

  static def crc32_update(buf, crc)
    crc ^= 0xffffffff
    var crc32_table = partition.Partition_otadata.crc32_table
    for k:0..size(buf)-1
      crc = (crc >> 8 & 0x00FFFFFF) ^ crc32_table[(crc & 0xff) ^ buf[k]]
    end
    return crc ^ 0xffffffff
  end

  #- crc32 for ota_seq as 32 bits unsigned, with init vector -1 -#
  static def crc32_ota_seq(seq)
    return partition.Partition_otadata.crc32_update(bytes().add(seq, 4), 0xFFFFFFFF)
  end

  #---------------------------------------------------------------------#
  # Rest of the class
  #---------------------------------------------------------------------#
  def init(maxota, offset)
    self.maxota = maxota
    if self.maxota == nil  self.maxota = 1 end
    self.offset = offset
    if self.offset == nil  self.offset = 0xE000 end
    self.active_otadata = 0
    self.load()
  end

  #- update ota_max, needs to recompute everything -#
  def set_ota_max(n)
    self.maxota = n
  end

  # change the active OTA partition
  def set_active(n)
    var seq_max = 0     #- current highest seq number -#
    var block_act = 0   #- block number containing the highest seq number -#

    if self.seq0 != nil
      seq_max = self.seq0
      block_act = 0
    end
    if self.seq1 != nil && self.seq1 > seq_max
      seq_max = self.seq1
      block_act = 1
    end
    
    #- compute the next sequence number -#
    var actual_ota = (seq_max - 1) % (self.maxota + 1)
    if actual_ota != n    #- change only if different -#
      if n > actual_ota   seq_max += n - actual_ota
      else                seq_max += (self.maxota + 1) - actual_ota + n
      end
      
      #- update internal structure -#
      if block_act == 1       #- current block is 1, so update block 0 -#
        self.seq0 = seq_max
      else                    #- or write to block 1 -#
        self.seq1 = seq_max
      end
      self._validate()
    end
  end

  #- load otadata from SPI Flash -#
  def load()
    import flash
    var otadata0 = flash.read(0xE000, 32)
    var otadata1 = flash.read(0xF000, 32)
    self.seq0 = otadata0.get(0, 4)   #- ota_seq for block 1 -#
    self.seq1 = otadata1.get(0, 4)   #- ota_seq for block 2 -#
    var valid0 = otadata0.get(28, 4) == self.crc32_ota_seq(self.seq0) #- is CRC32 valid? -#
    var valid1 = otadata1.get(28, 4) == self.crc32_ota_seq(self.seq1) #- is CRC32 valid? -#
    if !valid0   self.seq0 = nil end
    if !valid1   self.seq1 = nil end

    self._validate()
  end

  #- internally used, validate data -#
  def _validate()
    self.active_otadata = 0        #- if none is valid, default to OTA0 -#
    if self.seq0 != nil
      self.active_otadata = (self.seq0 - 1) % (self.maxota + 1)
    end
    if self.seq1 != nil && (self.seq0 == nil || self.seq1 > self.seq0)
      self.active_otadata = (self.seq1 - 1) % (self.maxota + 1)
    end
  end

  # Save partition information to SPI Flash
  def save()
    import flash
    #- check the block number to save, 0 or 1. Choose the highest ota_seq -#
    var block_to_save = -1    #- invalid -#
    var seq_to_save = -1      #- invalid value -#
    
    # check seq0
    if self.seq0 != nil
      seq_to_save = self.seq0
      block_to_save = 0
    end
    if (self.seq1 != nil) && (self.seq1 > seq_to_save)
      seq_to_save = self.seq1
      block_to_save = 1
    end
    # if none was good
    if block_to_save < 0    block_to_save = 0 end
    if seq_to_save < 0      seq_to_save = 1 end

    var offset_to_save = self.offset + 0x1000 * block_to_save   #- default 0xE000 or 0xF000 -#

    var bytes_to_save = bytes()
    bytes_to_save.add(seq_to_save, 4)
    bytes_to_save += bytes("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF")
    bytes_to_save.add(self.crc32_ota_seq(seq_to_save), 4)
    
    #- erase flash area and write -#
    flash.erase(offset_to_save, 0x1000)
    flash.write(offset_to_save, bytes_to_save)
  end

  # Produce a human-readable representation of the object with relevant information
  def tostring()
    import string
    return string.format("<instance: Partition_otadata(ota_active:%d, ota_seq=[%d,%d], ota_max=%d)>",
                          self.active_otadata, self.seq0, self.seq1, self.maxota)
  end
end
partition.Partition_otadata = Partition_otadata

#-------------------------------------------------------------
 - Class for a partition table entry
 -------------------------------------------------------------#
class Partition
  var raw   #- raw bytes of the partition table in flash -#
  var md5   #- md5 hash of partition list -#
  var slots
  var otadata   #- instance of Partition_otadata() -#

  def init()
    self.slots = []
    self.load()
    self.parse()
    self.load_otadata()
  end

  # Load partition information from SPI Flash
  def load()
    import flash
    self.raw = flash.read(0x8000,0x1000)
  end

  #- parse the raw bytes to a structured list of partition items -#
  def parse()
    for i:0..94       # there are maximum 95 slots + md5 (0xC00)
      var item_raw = self.raw[i*32..(i+1)*32-1]
      var magic = item_raw.get(0,2)
      if magic == 0x50AA    #- partition entry -#
        var slot = partition.Partition_info(item_raw)
        self.slots.push(slot)
      elif magic == 0xEBEB  #- MD5 -#
        self.md5 = self.raw[i*32+16..i*33-1]
        break
      else
        break
      end
    end
  end

  def get_ota_slot(n)
    for slot: self.slots
      if slot.is_ota() == n return slot end
    end
    return nil
  end

  #- compute the highest ota<x> partition -#
  def ota_max()
    var ota_max = 0
    for slot:self.slots
      if slot.type == 0 && (slot.subtype >= 0x10 && slot.subtype < 0x20)
        var ota_num = slot.subtype - 0x10
        if ota_num > ota_max  ota_max = ota_num end
      end
    end
    return ota_max
  end

  def load_otadata()
    #- look for otadata partition offset, and max_ota -#
    var otadata_offset = 0xE000   #- default value -#
    var ota_max = self.ota_max()
    for slot:self.slots
      if slot.type == 1 && slot.subtype == 0    #- otadata -#
        otadata_offset = slot.start
      end
    end

    self.otadata = partition.Partition_otadata(ota_max, otadata_offset)
  end

  # get the active OTA app partition number
  def get_active()
    return self.otadata.active_otadata
  end

  #- change the active partition -#
  def set_active(n)
    if n < 0 || n > self.ota_max()  raise "value_error", "Invalid ota partition number" end
    self.otadata.set_ota_max(self.ota_max())    #- update ota_max if it changed -#
    self.otadata.set_active(n)
  end

  # Human readable version of Partition information
  # this method is not included in the solidified version to save space,
  # it is included only in the optional application `tapp` version
  #- convert to human readble -#
  def tostring()
    var ret = "<instance: Partition([\n"
    for slot: self.slots
      ret += "  "
      ret += slot.tostring()
      ret += "\n"
    end
    ret += "],\n  "
    ret += self.otadata.tostring()
    ret += "\n)>"
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
    import flash
    var b = self.tobytes()
    #- erase flash area and write -#
    flash.erase(0x8000, 0x1000)
    flash.write(0x8000, b)
    self.otadata.save()
  end

  #- invalidate SPIFFS partition to force format at next boot -#
  #- we simply erase the first byte of the first 2 blocks in the SPIFFS partition -#
  def invalidate_spiffs()
    import flash
    #- we expect the SPIFFS partition to be the last one -#
    var spiffs = self.slots[-1]
    if !spiffs.is_spiffs() raise 'value_error', 'No SPIFFS partition found' end

    var b = bytes("00")  #- flash memory: we can turn bits from '1' to '0' -#
    flash.write(spiffs.start         , b)    #- block #0 -#
    flash.write(spiffs.start + 0x1000, b)    #- block #1 -#
  end  
end
partition.Partition = Partition

return partition

#- Example

import partition

# read
p = partition.Partition()
print(p)

-#
