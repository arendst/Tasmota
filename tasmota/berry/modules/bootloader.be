#
# Flash bootloader from URL or filesystem
#

class bootloader
  static var _addr = [0x1000, 0x0000]         # possible addresses for bootloader
  static var _sign = bytes('E9')              # signature of the bootloader
  static var _addr_high = 0x8000              # address of next partition after bootloader

  # get the bootloader address, 0x1000 for Xtensa based, 0x0000 for RISC-V based (but might have some exception)
  # we prefer to probed what's already in place rather than manage a hardcoded list of architectures
  # (there is a low risk of collision if the address is 0x0000 and offset 0x1000 is actually E9)
  def get_bootloader_address()
    import flash
    # let's see where we find 0xE9, trying first 0x1000 then 0x0000
    for addr : self._addr
      if flash.read(addr, size(self._sign)) == self._sign
        return addr
      end
    end
    return nil
  end

  #
  # download from URL and store to `bootloader.bin`
  #
  def download(url)
    # address to flash the bootloader
    var addr = self.get_bootloader_address()
    if addr == nil    raise "internal_error", "can't find address for bootloader" end
    
    var cl = webclient()
    cl.begin(url)
    var r = cl.GET()
    if r != 200    raise "network_error", "GET returned "+str(r) end
    var bl_size = cl.get_size()
    if bl_size <= 8291   raise "internal_error", "wrong bootloader size "+str(bl_size) end
    if bl_size > (0x8000 - addr)  raise "internal_error", "bootloader is too large "+str(bl_size / 1024)+"kB" end

    cl.write_file("bootloader.bin")
    cl.close()
  end

  # returns true if ok
  def flash(url)
    var fname = "bootloader.bin"      # default local name
    if url != nil
      if url[0..3] == "http"          # if starts with 'http' download
        self.download(url)
      else
        fname = url                   # else get from file system
      end
    end
    # address to flash the bootloader
    var addr = self.get_bootloader_address()
    if addr == nil    tasmota.log("OTA: can't find address for bootloader", 2) return false end

    var bl = open(fname, "r")
    if bl.readbytes(size(self._sign)) != self._sign
      tasmota.log("OTA: file does not contain a bootloader signature", 2)
      return false
    end
    bl.seek(0)    # reset to start of file

    var bl_size = bl.size()
    if bl_size <= 8291   tasmota.log("OTA: wrong bootloader size "+str(bl_size), 2) return false end
    if bl_size > (0x8000 - addr)  tasmota.log("OTA: bootloader is too large "+str(bl_size / 1024)+"kB", 2) return false end

    tasmota.log("OTA: Flashing bootloader", 2)
    # from now on there is no turning back, any failure means a bricked device
    import flash
    # read current value for bytes 2/3
    var cur_config = flash.read(addr, 4)

    flash.erase(addr, self._addr_high - addr)   # erase the bootloader
    var buf = bl.readbytes(0x1000)              # read by chunks of 4kb
    # put back signature
    buf[2] = cur_config[2]
    buf[3] = cur_config[3]
    while size(buf) > 0
      flash.write(addr, buf, true)        # set flag to no-erase since we already erased it
      addr += size(buf)
      buf = bl.readbytes(0x1000)            # read next chunk
    end
    bl.close()
    tasmota.log("OTA: Booloader flashed, please restart", 2)
    return true
  end
end

return bootloader

#-

### FLASH
import bootloader
bootloader().flash('https://raw.githubusercontent.com/espressif/arduino-esp32/master/tools/sdk/esp32/bin/bootloader_dio_40m.bin')

#bootloader().flash('https://raw.githubusercontent.com/espressif/arduino-esp32/master/tools/sdk/esp32/bin/bootloader_dout_40m.bin')

### FLASH from local file
bootloader().flash("bootloader-tasmota-c3.bin")

#### debug only
bl = bootloader()
print(format("0x%04X", bl.get_bootloader_address()))

-#