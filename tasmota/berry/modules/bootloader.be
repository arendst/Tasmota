#
# Flash bootloader from URL
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

  def flash(url)
    if url != nil
      self.download(url)
    end
    # address to flash the bootloader
    var addr = self.get_bootloader_address()
    if addr == nil    raise "internal_error", "can't find address for bootloader" end

    var bl = open("bootloader.bin", "r")
    if bl.readbytes(size(self._sign)) != self._sign
      raise "value_error", "the file does not contain a bootloader signature"
    end
    bl.seek(0)    # reset to start of file

    var bl_size = bl.size()
    if bl_size <= 8291   raise "internal_error", "wrong bootloader size "+str(bl_size) end
    if bl_size > (0x8000 - addr)  raise "internal_error", "bootloader is too large "+str(bl_size / 1024)+"kB" end

    print("OTA: Flashing bootloader")
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
    print("OTA: Booloader flashed, please restart")

  end
end

return bootloader

#-

### FLASH
import bootloader
bootloader().flash('https://raw.githubusercontent.com/espressif/arduino-esp32/master/tools/sdk/esp32/bin/bootloader_dio_40m.bin')

#bootloader().flash('https://raw.githubusercontent.com/espressif/arduino-esp32/master/tools/sdk/esp32/bin/bootloader_dout_40m.bin')

#### debug only
import string
bl = bootloader()
print(string.format("0x%04X", bl.get_bootloader_address()))

-#