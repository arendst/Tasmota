
#################################################################################
#
# class `sonoff_zb_pro_flasher`
#
#################################################################################

class sonoff_zb_pro_flasher
  static CCFG_address = 0x057FD8
  static CCFG_reference = 0xC5FE08C5

  #################################################################################
  # Flashing from Intel HEX files
  #################################################################################
  var filename          # filename of hex file
  var f                 # file object
  var file_checked       # was the file already parsed. It cannot be flashed if not previously parsed and validated
  var file_validated    # was the file already validated. It cannot be flashed if not previously parsed and validated
  var file_hex          # intelhex object
  var flasher           # low-level flasher object (cc2652_flasher instance)

  def init()
    self.file_checked = false
    self.file_validated = false
  end

  def load(filename)
    import intelhex

    if type(filename) != 'string'   raise "value_error", "invalid file name" end
    self.filename = filename
    self.file_hex = intelhex(filename)    # prepare the parser object
    self.file_checked = false
    self.file_validated = false
  end

  #################################################################################
  # check that the HEX file is valid
  # parse it completely once, and verify some values
  #################################################################################
  def check()
    self.file_hex.parse(/ -> self._check_pre(),
                        / address, len, data, offset -> self._check_cb(address, len, data, offset),
                        / -> self._check_post()
                        )
  end

  #################################################################################
  # Flash the firmware to the device
  #
  #################################################################################
  def flash()
    if !self.file_checked
      print("FLH: firmware not checked, use `cc.check()`")
      raise "flash_error", "firmware not checked"
    end
    if !self.file_validated
      print("FLH: firmware not validated, use `cc.check()`")
      raise "flash_error", "firmware not validated"
    end

    import cc2652_flasher   # this stops zigbee and configures serial
    self.flasher = cc2652_flasher

    try
      self.file_hex.parse(/ -> self._flash_pre(),
                          / address, len, data, offset -> self._flash_cb(address, len, data, offset),
                          / -> self._flash_post()
                          )
    except .. as e, m
      self.file_checked = false
      self.file_validated = false
      raise e, m
    end
  end

  #################################################################################
  # Dump firmware to local file
  #
  #################################################################################
  def dump_to_file(filename)
    import cc2652_flasher   # this stops zigbee and configures serial
    self.flasher = cc2652_flasher
    print("FLH: Dump started (takes 3 minutes during which Tasmota is unresponsive)")
    self.flasher.start()
    self.flasher.ping()
    self.flasher.flash_dump_to_file(filename, 0x000000, 0x58000)
    print("FLH: Dump completed")
  end

  #################################################################################
  # low-level
  #################################################################################
  def _flash_pre()
    print("FLH: Flashing started (takes 5-8 minutes during which Tasmota is unresponsive)")
    self.flasher.start()
    self.flasher.ping()
    # erase flash
    self.flasher.flash_erase()
  end

  def _flash_post()
    print("FLH: Flashing completed: OK")
    var flash_crc = self.flasher.cmd_crc32(0x0,0x30000)
    print("FLH: Flash crc32 0x000000 - 0x2FFFF = " + str(flash_crc));
    # tasmota.log("FLH: Verification of HEX file OK", 2)
  end

  def _flash_cb(addr, sz, data, offset)
    var payload = data[offset .. offset + sz - 1]

    # final check
    if size(payload) != sz    raise "flash_error", "incomplete payload" end

    self.flasher.flash_write(addr, payload)
  end


  # start verification (log only)
  def _check_pre()
    print("FLH: Starting verification of HEX file")
    # tasmota.log("FLH: Starting verification of HEX file", 2)
  end

  # don't flash so ignore data
  # check CCFG at location 0x57FD8 (4 bytes)
  def _check_cb(addr, sz, data, offset)
    # print(format("> addr=0x%06X sz=0x%02X data=%s", addr, sz, data[offset..offset+sz-1]))
    var CCFG = self.CCFG_address
    if addr <= CCFG && addr+sz > CCFG+4
      # we have CCFG in the buffer
      var ccfg_bytes = data.get(4 + CCFG - addr, 4)

      if ccfg_bytes != self.CCFG_reference
        raise "value_error", format("incorrect CCFG, BSL is not set to DIO_8 LOW (0x%08X expected 0x%08X)", ccfg_bytes, self.CCFG_reference) end
      self.file_validated = true    # if we are here, it means that the file looks correct
    end
  end

  def _check_post()
    print("FLH: Verification of HEX file OK")
    # tasmota.log("FLH: Verification of HEX file OK", 2)
    self.file_checked = true
  end

end

return sonoff_zb_pro_flasher()


#-
# Flash local firmware

import sonoff_zb_pro_flasher as cc
cc.load("SonoffZBPro_coord_20230507.hex")
cc.check()
cc.flash()

import sonoff_zb_pro_flasher as cc
cc.load("SonoffZBPro_coord_20220219.hex")
cc.check()
cc.flash()


-#

#-
# Dump local firmware

import sonoff_zb_pro_flasher as cc
cc.dump_to_file("SonoffZBPro_dump.bin")

-#
