#################################################################################
# Specialized driver for M5Stack UnitC6L Lora
#
# in preinit.be: load("/pi4ioe5v6408_M5Stack_UnitC6L.be")
#################################################################################

class PI4IOE5V6408_UnitC6L : I2C_Driver
  def init(addr)
    if (addr == nil) 
      addr = 0x43
    end
    super(self, I2C_Driver).init("PI4IOE5V6408", addr)

    if self.wire
      var id = self.wire.read(self.addr, 0x01, 1)
      if id == 0
        self.wire = nil

        log("I2C: PI4IOE5V6408 not detected", 3)

        return                      # wrong device
      end

      # M5Unified src/utility/Power_Class.cpp reg_data_array_for_lorac6
      #  PI4IO E0
      #  P0 (UnitC6L + NessoN1:BTN1)
      #  P1 (UnitC6L:NC / NessoN1:BTN2)
      #  P2-P5 NC
      #  P5 LNA Enable
      #  P6 RF Switch
      #  P7 LoRa Reset
      # for LoraC6 internal IOEXP
      self.write8(0x03, 0xE0)       # PI4IO_REG_IO_DIR
      self.write8(0x05, 0x80)       # PI4IO_REG_OUT_SET
      self.write8(0x07, 0x1C)       # PI4IO_REG_OUT_H_IM
      self.write8(0x0D, 0xC3)       # PI4IO_REG_PULL_SEL
      self.write8(0x0B, 0xC3)       # PI4IO_REG_PULL_EN
      self.write8(0x09, 0x03)       # PI4IO_REG_IN_DEF_STA
      self.write8(0x11, 0xFC)       # PI4IO_REG_INT_MASK

      # Example code and docs
      self.write_bit(0x05, 7, 0)    # drive RESET Low
      tasmota.delay(100)            # the recommended delay is 100ms
      self.write_bit(0x05, 7, 1)    # re-enable SX_NRST
      self.write_bit(0x05, 6, 1)    # enable SX_ANT_SW
      self.write_bit(0x05, 5, 1)    # enable SX_LNA_EN
    end
  end
end

var unitc6l = PI4IOE5V6408_UnitC6L()
