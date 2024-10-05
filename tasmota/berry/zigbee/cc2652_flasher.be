
#################################################################################
#
# class `cc2652_flasher`
#
# Flash libraries for CC2652: read, write, verify...
#
# The serial protocol requires the CC2652 to boot in BSL (bootloader) mode.
# On Sonoff Zigbee Bridge Pro, it requires to reset the MCU with DIO_8 low.
#
# When starting the flasher, normal zigbee operations are aborterd. Restarting
# normal zigbee functions requires a Tasmota restart.
#
# Required configuration:
# - Zigbee Rx: must be configured as `Zigbee Rx` or `TCP Rx`
# - Zigbee Tx: must be configured as `Zigbee Tx` or `TCP Tx`
# - Zigbee Reset: must be configured as `Zigbee Rst - 1``
# - Zigbee BSL mode (low): must be configured as `Zigbee Rts - 2`
# For Sonoff Zibeee Bridge Pro: Rx=23 Tx=19 Rst-1=15 Rst-2=22
#
# How to use:
# - `import cc2652_flasher as cc`
#    aborts all zigbee operations and configures the serial port
#    Output: `FLH: cc2652_flasher rx=23 tx=19 rst=15 bsl=22`
#
# - `cc.start()` to start the flasher`
#   restarts the CC2652 in BSL mode, and establishes connection
#   Use `cc.start(true)` to enable debug verbose mode
#
# - `cc.ping()` sends a ping command and waits for ACK (does nothing)
#
# - `cc.cmd_get_chip_id()` returns the chip ID
#
# - `cc.flash_read(addr, len)` reads `len` bytes from address `addr``
#   len must be less or equal than 128 bytes
#   Returns a bytes() object
#
# - `cc.flash_crc32(addr, len)` returns the CRC32 of a flash region
#
# - `cc.flash_write(addr, data)` writes bytes to the flash
#   `data` is a bytes() buffer, its len must be less or equal than 128
#   This call does not erase the flash, so it must have been erased before.
#   The bootloader checks that the bytes were correctly written, i.e. that
#   the appropriate bits were changed from `1` to `0`.
#   Chaning bits from `0` to `1` requires a flash erase.
#
# - `cc.flash_erase()` erase the entire flash.
#   Use with caution. After the flash is erased, there is no valid application
#   in flash so the MCU always starts in BSL bootloader mode until a valid
#   app is flashed.
#
# - `cc.flash_dump_to_file(filename, addr, len)` dumps the CC2652 flash into a file
#   `filename` is the output file in binary format, make sure there are 360KB free in filesystem.
#   Dumping the complete file is done as follows (it takes 3 minutes during which Tasmota is unresponsive):
#   `cc.dump_to_file("cc2652_dump.bin", 0x00000, 0x58000)`
#
#################################################################################

#- Example

import cc2652_flasher as cc
cc.start()
cc.ping()
print(format("0x%08X", cc.cmd_get_chip_id()))
# output: 0x3202F000

# Dumping CC2652 flash into filesystem
# This takes 3 minutes during which Tasmota is unresponsive
#
import cc2652_flasher as cc
cc.start()
cc.flash_dump_to_file("cc2652_dump.bin", 0x00000, 0x58000)

-#

class cc2652_flasher
  var ser                     # serial object
  var debug                   # verbose logs?
  var rx, tx, rst, bsl        # GPIO numbers

  # init - abort zigbee operations and starts the serial driver
  # args are optional
  def init(rx, tx, rst, bsl)
    self.debug = false
    self.rx  = (rx  == nil) ? -1 : rx
    self.tx  = (tx  == nil) ? -1 : tx
    self.rst = (rst == nil) ? -1 : rst
    self.bsl = (bsl == nil) ? -1 : bsl
    #
    if self.rx < 0  self.rx = gpio.pin(gpio.ZIGBEE_RX) end
    if self.rx < 0  self.rx = gpio.pin(gpio.TCP_RX) end
    if self.tx < 0  self.tx = gpio.pin(gpio.ZIGBEE_TX) end
    if self.tx < 0  self.tx = gpio.pin(gpio.TCP_TX) end
    if self.rst < 0 self.rst = gpio.pin(gpio.ZIGBEE_RST, 0) end
    if self.bsl < 0 self.bsl = gpio.pin(gpio.ZIGBEE_RST, 1) end
    print(format("FLH: cc2652_flasher rx=%i tx=%i rst=%i bsl=%i", self.rx, self.tx, self.rst, self.bsl))
    # tasmota.log(format("FLH: cc2652_flasher rx=%i tx=%i rst=%i bsl=%i", self.rx, self.tx, self.rst, self.bsl), 3)
    if self.rx < 0 || self.tx < 0 || self.rst < 0 || self.bsl < 0
      raise "value_error", "cc2652_flasher unspecified GPIOs"
    end
    # stop all zigbee activity
    import zigbee
    zigbee.abort()
    # good to go
    self.ser = serial(self.rx, self.tx, 115200)   # initialize UART serial port
  end

  # restart the MCU in BSL mode and establish communication
  def start(debug)
    if debug == nil    debug = false end
    self.debug = bool(debug)
    self.reset_bsl()
    #
    # print("FLH: cc2652_flasher started")
  end

  #################################################################################
  # Low level methods
  #################################################################################

  # restart MCU and enter BSL
  #
  # arg:
  #  ser: serial object
  def reset_bsl()
    self.ser.flush()

    gpio.digital_write(self.bsl, 0)    # trigger BSL

    gpio.digital_write(self.rst, 0)
    tasmota.delay(10)               # wait 10ms
    gpio.digital_write(self.rst, 1)
    tasmota.delay(100)               # wait 100ms

    self.ser.write(bytes("5555"))          # trigger auto baudrate detector
    var ret = self.recv_raw(100)
    if self.debug print("ret=", ret) end
    if ret != bytes('CC')
      raise "protocol_error"
    end
  end

  # received buffer and give up if timeout
  def recv_raw(timeout)
    var due = tasmota.millis() + timeout
    while !tasmota.time_reached(due)
      if self.ser.available()
        var b = self.ser.read()
        if self.debug print("b:",b) end
        while size(b) > 0 && b[0] == 0
          b = b[1..]
        end
        return b
      end
      tasmota.delay(5)        # check every 5ms
    end
    raise "timeout_error", "serial timeout"
  end

  # send simple ACK
  def send_ack()
    if self.debug print("send ACK") end
    self.ser.write(bytes("00CC"))
  end

  # encode payload
  static def encode_payload(b)
    var checksum = 0
    for i:0..size(b)-1
      checksum = (checksum + b[i]) & 0xFF
    end
    var payload = bytes("0000")
    payload[0] = size(b) + 2
    payload[1] = checksum
    payload += b
    payload += bytes("00")
    return payload
  end

  static def decode_ack(b)
    # skip any 00 or CC bytes
    while size(b) > 0 && b[0] == 0
      b = b[1..]
    end
    if size(b) == 0 || b[0] != 0xCC
      raise "serial_error", "missing ACK"
    end
  end

  static def decode_payload(b)
    # skip any 00 or CC bytes
    while size(b) > 0 && (b[0] == 0 || b[0] == 0xCC)
      b = b[1..]
    end

    # check buffer
    var sz = b[0]
    if size(b) < sz || sz < 2    raise "serial_error", "buffer too small" end
    # 
    var payload = b[2..sz-1]

    var checksum = 0
    for i:0..size(payload)-1
      checksum = (checksum + payload[i]) & 0xFF
    end
    if checksum != b[1]   raise "serial_error", "invalid checksum received"  end

    return payload
  end

  # send
  # args:
  #  b: logical bytes() to send
  #  no_response: true if ignore any response, or ignore to get a response
  def send(b, no_response)
    # compute 
    var payload = self.encode_payload(b)
    if self.debug print("sending:", payload) end
    self.ser.write(payload)
    var ret = self.recv_raw(500)
    if self.debug print("ret=", ret) end
    if no_response == true
      #ignore
      self.decode_ack(ret)
      return nil
    else
      payload = self.decode_payload(ret)
      self.send_ack()
      return payload
    end
  end

  # Higher level functions
  # 64 - COMMAND_RET_SUCCESS
  # 65 - COMMAND_RET_UNKNOWN_CMD
  # 66 - COMMAND_RET_INVALID_CMD
  # 67 - COMMAND_RET_INVALID_ADR
  # 68 - COMMAND_RET_FLASH_FAIL
  def cmd_get_status()
    var payload = self.send(bytes("23"))
    return payload[0]
  end

  # Get the value of the 32-bit user ID from the AON_PMCTL JTAGUSERCODE register
  def cmd_get_chip_id()
    var payload = self.send(bytes("28"))
    return payload.get(0, -4)
  end

  def cmd_memory_read(addr, len)
    if len > 128      raise "value_error", "len is bigger than 128" end
    var b = bytes("2A")
    b.add(addr, -4)
    b.add(1)
    b.add(len/4)

    return self.send(b)
  end

  # does not look to be implemented
  # def cmd_memory_write(addr, data)
  #   var sz = size(data)
  #   if sz > 128      raise "value_error", "len is bigger than 128" end
  #   var b = bytes("2B")
  #   b.add(addr, -4)
  #   b.add(1)
  #   b += data
  #   print("cmd_memory_write",b)

  #   return self.send(b)
  # end

  def cmd_download(addr, sz)
    if sz > 128      raise "value_error", "len is bigger than 128" end
    var b = bytes("21")
    b.add(addr, -4)
    b.add(sz, -4)
    if self.debug print("cmd_download",b) end

    return self.send(b, true)
  end

  def cmd_send_data(data)
    var sz = size(data)
    if sz > 128      raise "value_error", "len is bigger than 128" end
    var b = bytes("24")
    b += data
    if self.debug print("cmd_send_data",b) end

    return self.send(b, true)
  end

  # WARNING: this command erases all of the customer-accessible flash sectors
  # After this operation, since CCFG is not configured, the device will always reboot in BSL (bootloader) mode
  # until CCFG is actually re-written
  #
  def cmd_bank_erase()
    self.send(bytes("2C"), true)
  end

  # compute crc32 for a memory range
  # repeat count if forced to 0x00000000 to read each location only once
  def cmd_crc32(addr, len)
    var b = bytes("27")
    b.add(addr, -4)
    b.add(len, -4)
    b.add(0, -4)    # repeat count = 0
    return self.send(b)
  end

  #################################################################################
  # High level methods
  #################################################################################

  def flash_read(addr, len)
    return self.cmd_memory_read(addr, len)
  end

  def flash_crc32(addr, len)
    return self.cmd_crc32(addr, len)
  end
  
  def flash_erase()
    self.cmd_bank_erase()
  end

  # send ping
  def ping()
    self.send(bytes("20"), true)
  end

  # higher level
  def flash_write(addr, data)
    var sz = size(data)
    if sz > 128      raise "value_error", "len is bigger than 128" end

    var ret

    ret = self.cmd_download(addr, size(data))
    #print(">cmd_download", r)
    
    var ack
    ack = self.cmd_get_status()
    if ack != 0x40    raise "serial_error", format("command failed: 0x%02X - 0x%06X (%i)", ack, addr, sz) end
    
    ret = self.cmd_send_data(data)

    ack = self.cmd_get_status()
    if ack != 0x40    raise "serial_error", format("command failed: 0x%02X - 0x%06X (%i)", ack, addr, sz) end

  end

  # dump the flash into a bin file
  def flash_dump_to_file(filename, addr, len)
    var offset = addr
    var f
    
    try
      f = open(filename,"w")
      while len > 0
        var b = self.cmd_memory_read(offset, 32)
        f.write(b)
        offset += 32
        len -= 32
        tasmota.yield()
      end
    except .. as e, m
      if f != nil   f.close() end
      raise e, m
    end
    f.close()
  end
end

return cc2652_flasher()
