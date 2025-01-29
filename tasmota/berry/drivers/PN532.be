#-------------------------------------------------------------
 - Driver for PN532 RFID chip connected in Serial mode
 -------------------------------------------------------------#

#@ solidify:PN532
class PN532
  var ser               # serial object
  var cmd               # last cmd sent or received
  var timer_clear_uid   # time to clear UID

  var uid               # last UID read
  var atqa              #

  static var _TIMER_CLEAR_UID = 4000              # time to clear UID, default 5 seconds
  static var _TIMER_ID = "pn532_timer"            # unique id for the timer
  static var _NOUID = bytes("00000000")
  static var _WAKEUP = bytes("5555000000")
  static var _GETFIRMWAREVERSION = bytes("02")    # PN532_COMMAND_GETFIRMWAREVERSION
  static var _ACK = bytes("0000FF00FF00")         # Ack message
  static var _PRE = bytes("0000FF")               # PN532_PREAMBLE + PN532_STARTCODE1 + PN532_STARTCODE2
  static var _EMPTY = bytes("")
  static var _CONFIG_RETRIESF = bytes("3205020100")  # minimum retries
  static var _SAMCONFIG = bytes("14011400")
  static var _KEY_MIFARE = bytes("FFFFFFFFFFFF")  # default encryption key for MIFARE
  static var _SUCCESS = bytes("00")
  static var _INRELEASE = bytes("5201")           # 
  static var _AUTH = bytes("400160")              # auth prefix
  static var _READ = bytes("400130")

  ######################################################################
  # init(tx, rx)
  #
  # Initialize PN532 driver, with tx/rx GPIOs
  def init(tx, rx)
    if (tx == nil || rx == nil)
      raise "value_error", "tx/rx cannot be nil"
    end
    self.uid = self._NOUID
    self.timer_clear_uid = self._TIMER_CLEAR_UID

    # import gpio
    # gpio.pin_mode(tx, gpio.OUTPUT)
    # gpio.pin_mode(rx, gpio.INPUT)

    self.ser = serial(rx, tx, 115200)
    log(f"NFC: PN532 Rx {rx} Tx {tx}", 3)

    self.start()

    if self.ser
      tasmota.add_driver(self)
    end
  end

  def start()
    try
      # read firmware version
      var b = self.command(self._GETFIRMWAREVERSION, 100)
      log(f"NFC: GetFirmwareVersion '{b.tohex()}'", 3)
      # bytes('32010607')
      log(f"NFC: PN532 NFC Reader detected v{b[1]}.{b[2]}", 2)

      # PN532_setPassiveActivationRetries(0xFF)
      b = self.command(self._CONFIG_RETRIESF, 100)
      # log(f"NFC: setPassiveActivationRetries '{b.tohex()}'", 3)

      # PN532_SAMConfig()
      self.command(self._SAMCONFIG, 100)
      # log(f"NFC: SAMConfig '{b.tohex()}'", 3)

    except "pn532" as _, m
      log(f"NFC: error '{m}' more='{self.ser.read().tohex()}'", 2)
      self.stop()
    end
  end

  def stop()
    tasmota.remove_driver(self)
    self.ser.close()
    self.ser = nil
  end

  ######################################################################
  # command(payload : bytes [, no_response : bool, timeout_ms = 50])
  #
  # Send a command as a bytes buffer (first byte is command)
  # Timeout in millisecond, defaut is 50 ms
  #
  # Returns a bytes() object as the response
  # to the command (checks internally for Ack) or
  # raise an exception of class 'pn532' if something went wrong
  def command(payload, timeout_ms)
    if !self.ser            return nil              end
    if (payload == nil)     payload = self._EMPTY   end
    if (timeout_ms == nil)  timeout_ms = 50         end
    if tasmota.loglevel(4)
      log(f"NFC: sending command '{payload.tohex()}'", 4)
    end
    var ser = self.ser
    var b
    self.cmd = nil            # reset cmd

    b = bytes()
    b.append(self._PRE)
    var length = size(payload) + 1  # length of data field: TFI + DATA
    b.add(length)
    b.add(~length + 1)    # checksum of length
    b.add(0xD4)           # PN532_HOSTTOPN532
    var sum = 0xD4
    if (size(payload) > 0)      # write payload
      self.cmd = payload[0]
      b.append(payload)
      var idx = 0
      while idx < size(payload)
        sum += payload[idx]
        idx += 1
      end
    end
    b.add(~sum + 1)       # checksum of TFI + DATA
    b.add(0x00)           # PN532_POSTAMBLE
    if tasmota.loglevel(4)
      log(f"NFC: sending raw '{b.tohex()}'", 4)
    end
    self._wakeup()            # wakeup
    ser.flush()               # clear the serial buffer just in case
    ser.write(b)

    ########
    # check for Ack
    var until_ms = tasmota.millis(10)       # Ack max timeout is 10ms for 115200
    b = self._read(size(self._ACK), until_ms)
    if (b != self._ACK)
      log(f"NFC: Invalid ACK '{b.tohex()}'", 3)
      raise "pn532", "invalid_ack"
    end
    if tasmota.loglevel(4)
      log(f"NFC: ACK received", 4)
    end

    ########
    # read response
    var ret = self._read_response(self.cmd, timeout_ms)
    if tasmota.loglevel(4)
      log(f"NFC: received response '{ret.tohex()}' cmd 0x{self.cmd:02X}", 4)
    end
    return ret
  end

  def _read_response(cmd_excpected, timeout_ms)
    var until_ms = tasmota.millis(timeout_ms != nil ? timeout_ms : 50)
    var b

    b = self._read(3, until_ms)
    # log(f"NFC: recv pre '{b.tohex()}'", 4)
    if (b != self._PRE)
      log(f"NFC: invalid prefix '{b.tohex()}' expected '{self._PRE.tohex()}'")
      raise 'pn532', 'invalid_pre'
    end

    # Get length of data to be received
    b = self._read(2, until_ms)
    # log(f"NFC: recv len '{b.tohex()}'", 4)
    if ((b[0] + b[1]) & 0xFF) != 0
      log(f"NFC: invalid length '{b.tohex()}'")
      raise 'pn532', 'invalid_len'
    end

    var len = b[0] - 2

    # Get cmd
    b = self._read(2, until_ms)
    # log(f"NFC: recv cmd '{b.tohex()}'", 4)
    var cmd = b[1]
    if (b[0] != 0xD5 #-PN532_PN532TOHOST-#) || (cmd_excpected != nil && self.cmd + 1 != cmd)
      log(f"NFC: invalid command '{b.tohex()}'")
      raise 'pn532', 'invalid_cmd'
    else
      self.cmd = cmd
    end

    var ret
    if (len > 0)
      ret = self._read(len, until_ms)
      # log(f"NFC: recv ret '{ret.tohex()}'", 4)
    else
      ret = bytes()
    end

    # read checksum and end
    b = self._read(2, until_ms)
    # log(f"NFC: recv post '{b.tohex()}'", 4)
    if (b[1] != 0)
      log(f"NFC: invalid frame '{b.tohex()}'")
      raise 'pn532', 'invalid_frame'
    end

    return ret
  end

  ######################################################################
  # _read(len : int [, until_ms = now + 15])
  #
  # Internal function: read exactly `len` bytes or raise an exception if timeout
  # Retuns `bytes()` buffer
  #
  # if `timeout_ms` is `0`, do a non-blocking read and return `nil` if response not available
  # if `len` is `0`, get whatever is in the buffer (no limit) and return `nil` if empty
  #
  # Note: if `len > 0 && timeout_ms > 0` then it returns either `bytes()` or raises an exception
  # does not return `nil` in such case
  def _read(len, until_ms)
    if (until_ms == nil)      until_ms = tasmota.millis(15)     end
    
    if (len <= 0)
      return self.ser.read()
    else
      while !tasmota.time_reached(until_ms)
        if self.ser.available() >= len
          return self.ser.read(len)
        end
      end
      raise "pn532", "timeout"
    end
  end

  ######################################################################
  # wakeup
  #
  # Send a serial sequence to wake up the PN532
  def _wakeup()
    self.ser.write(self._WAKEUP)     # send the wake up seuence
    self.ser.flush()                 # remove any left-overs in buffers
  end

  ######################################################################
  # set_timer_clear_uid(ms)
  #
  # Change the default value of the timer to clear UID, minimum is 1s
  def set_timer_clear_uid(ms)
    ms = tasmota.int(ms, 1000, nil)  # ensure the value is int and within boundaries
    self.timer_clear_uid = ms
  end

  ######################################################################
  # read_passive_target_id(card_baud_rate)
  #
  # card_baud_rate:
  #   0 = MIFARE_ISO14443A
  def read_passive_target_id(card_baud_rate)
    if !self.ser            return nil              end
    if (card_baud_rate == nil)    card_baud_rate = 0  #-MIFARE-#  end

    # first release whatever is on-going
    self.release_card()       # we don't care if there's an error

    var cmd = bytes("4A01")
    cmd.add(card_baud_rate)
    try
      var b = self.command(cmd, 25)       # aggressive 25ms timeout since we don't want to stop Tasmota for too long - response is normally recevied in 15ms
      log(f"NFC: ReadPassiveTargetID '{b.tohex()}'", 4)
      
      if (b[0] > 0) && (b[1] == 1)
        #  b0              Tags Found
        #  b1              Tag Number (only one used in this example)
        #  b2..3           SENS_RES
        #  b4              SEL_RES
        #  b5              NFCID Length
        #  b6..NFCIDLen    NFCID

        # we got a card
        self.atqa = b.get(2, -2)    # read 16 bits, at position 2, Big Endian
        var uid_len = b[5]
        if (uid_len > 0)
          self.uid = b[6 .. uid_len + 5]
          if (tasmota.loglevel(3))
            log(f"NFC: detected MIFARE UID '{self.uid.tohex()}'", 4)
          end
        else
          self.uid = self._NOUID
        end
        return true
      end
    except 'pn532'
    end
    return false
  end

  ######################################################################
  # release_card()
  #
  # close connection to a card. Can be called if no connection is available
  def release_card()
    if !self.ser            return nil              end
    self.command(self._INRELEASE)
    # we don't really care about the result
    # result is bytes('00') if successful or bytes('27') if no connection is ongoing
  end


  ######################################################################
  # authenticate_card(card_baud_rate)
  #
  # authenticate to card:
  # sector: sector number (0..63 for 1K, 0..255 for 4K)
  # auth_B: 'false' = auth_A, 'true' = auth_B
  # key: encryption key or bytes("FFFFFFFFFFFF") if 'nil'
  #
  # return 'true' if succesful
  def authenticate_card(sector, auth_B, key)
    if !self.ser            return nil              end
    if (key == nil)   key = self._KEY_MIFARE      end
    auth_B = bool(auth_B)     # force boolean
    var payload = self._AUTH.copy()
    if (auth_B)   payload[-1] += 1    end   # increment last byte if auth_B
    payload.add(sector)
    payload.append(key)
    payload.append(self.uid)

    var ret = self.command(payload)
    return ret == self._SUCCESS
  end

  ######################################################################
  # reset_uid()
  #
  # Reset the card UID, so we generate a NEW_CARD event
  def reset_uid()
    self.uid = self._NOUID
  end

  ######################################################################
  # read_card(sector)
  #
  # read a sector of 16 bytes (0..63 for 1K, 0..255 for 4K)
  #
  # return bytes(16) or nil
  def read_card(sector)
    if !self.ser            return nil              end
    var payload = self._READ.copy()
    payload.add(sector)

    return self.command(payload)
  end

  ######################################################################
  # card_detected()
  #
  # Called whenever a card is detected on recurrent polling
  def card_detected()
    # override in your subclass
  end

  ######################################################################
  # new_card_detected()
  #
  # Called whenever a new card is detected on recurrent polling (i.e. different UID)
  def new_card_detected()
    # override in your subclass
  end
  
  ######################################################################
  # publish_card_detected(new : bool)
  #
  # Handle the event of a card detected
  def publish_card_detected(new_card)
    # we detected a card
    if new_card
      log(f"NFC: New card detected with UID {self.uid.tohex()}", 3)
      self.new_card_detected()
      var event_card = format('{"PN532":{"NEW_CARD":{"UID":"%s"}}}', self.uid.tohex())
      tasmota.publish_rule(event_card)
    else
      self.card_detected()
    end
    # reset timer
    # remove any previous timer
    tasmota.remove_timer(self._TIMER_ID)
    tasmota.set_timer(self.timer_clear_uid, def () self.reset_uid() end, self._TIMER_ID)
  end

  ######################################################################
  # every_250ms()
  #
  # Probe card 4 times per second
  def every_250ms()
    if !self.ser            return nil              end
    var old_uid = self.uid
    if self.read_passive_target_id(0)
      self.publish_card_detected(old_uid != self.uid)
    end
  end

  ######################################################################
  # web_sensor()
  #
  # Display UID on main page
  def web_sensor()
    if !self.ser            return nil              end
    var msg = format("{s}PN532 UID{m}%s{e}", self.uid.tohex())
    tasmota.web_send(msg)
  end

end

return PN532

#-
# Example

import PN532

class CardReader : PN532
  def init(tx, rx)
    super(self).init(tx, rx)
  end

  def new_card_detected()
    # authenticate to sector 4, auth_A, default key
    if self.authenticate_card(4, false, nil)
      var ret = self.read_card(4)
      if (ret != nil)
        # do your stuff
        print(f"NEW CARD with content {ret[12..15].tohex()}")
      end
    end
  end
end

pn532 = CardReader(22, 23)
-#