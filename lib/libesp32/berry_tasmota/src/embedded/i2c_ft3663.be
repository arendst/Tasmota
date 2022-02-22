#-------------------------------------------------------------
 - Driver for FT3663 - Capacitive Touch Screen
 -------------------------------------------------------------#
class FT3663 : I2C_Driver
  static gest_id_codes = {
    0x00: 0x00,
    0x10: 0x10, # Move Up
    0x14: 0x13, # Move Right
    0x18: 0x11, # Move Down
    0x1C: 0x12, # Move Left
    0x48: 0x20, # Zoom In
    0x49: 0x21, # Zoom Out
      # 0x10 Move Up
      # 0x14 Move Right
      # 0x18 Move Down
      # 0x1C Move Left
      # 0x48 Zoom In
      # 0x49 Zoom Out
      # 0x00 No Gesture
      #
      # TS_Gest_None = 0,
      # TS_Gest_Move_Up = 0x10,
      # TS_Gest_Move_Down = 0x11,
      # TS_Gest_Move_Left = 0x12,
      # TS_Gest_Move_Right = 0x13,
      # TS_Gest_Zoom_In = 0x20,
      # TS_Gest_Zoom_Out = 0x21,
  }
  def init()
    super(self).init("FT3663", 0x38)
    # check that ID from register 0xA8 is 0x11
    var vendid = self.read8(0xA8)
    var chipid = self.read8(0xA3)
    if vendid != 0x11 || chipid != 0x64
      tasmota.log("I2C: ignoring address 0x38, not FT3663", 2)
      self.wire = nil
      return
    end

    # FT3663 is now confirmed
    tasmota.log("TS : FT3663 Touch Screen detected")

    self.write8(0x00, 0x00)     # writeRegister8(FT6X36_REG_DEVICE_MODE, 0x00);
    self.write8(0x80, 22)       # writeRegister8(FT6X36_REG_THRESHHOLD, FT6X36_DEFAULT_THRESHOLD);
    self.write8(0x88, 0x0E)     # writeRegister8(FT6X36_REG_TOUCHRATE_ACTIVE, 0x0E);

    # register ourself
    tasmota.add_driver(self)
  end

  # read touch screen and publish result
  def ts_loop()
    if !self.wire return end
    import display

    var data = self.wire.read_bytes(self.addr, 0, 15)
    # ex: data = bytes('0000018098009B0000FFFFFFFFFFFF')

    var touches = data[0x02] & 0x0F      # number of touches
    var x1 = (data[0x03] & 0x0F) << 8 | data[0x04]
    var y1 = (data[0x05] & 0x0F) << 8 | data[0x06]
    var pressure1 = data[0x07]
    var x2 = (data[0x03] & 0x09) << 8 | data[0x0A]
    var y2 = (data[0x05] & 0x0B) << 8 | data[0x0C]
    var pressure2 = data[0x0D]
    var gesture = self.gest_id_codes.find(data[0x01], 0x00)
    #define FT6X36_REG_DEVICE_MODE			0x00
    #define FT6X36_REG_GESTURE_ID			0x01
    #define FT6X36_REG_NUM_TOUCHES			0x02
    #define FT6X36_REG_P1_XH				0x03
    #define FT6X36_REG_P1_XL				0x04
    #define FT6X36_REG_P1_YH				0x05
    #define FT6X36_REG_P1_YL				0x06
    #define FT6X36_REG_P1_WEIGHT			0x07
    #define FT6X36_REG_P1_MISC				0x08
    #define FT6X36_REG_P2_XH				0x09
    #define FT6X36_REG_P2_XL				0x0A
    #define FT6X36_REG_P2_YH				0x0B
    #define FT6X36_REG_P2_YL				0x0C
    #define FT6X36_REG_P2_WEIGHT			0x0D
    #define FT6X36_REG_P2_MISC				0x0E
    display.touch_update(touches, x1, y1, gesture)
  end

  def every_100ms()
    if self.wire   self.ts_loop() end
  end

end

ts = FT3663()
