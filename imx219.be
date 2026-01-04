# IMX219 CSI Camera Driver - Direct port from Linux kernel driver
# Register sequences from imx219.c (Raspberry Pi / Sony official)

class CSI_Sensor
  # ... (exactly same as OV5647 - unchanged)
  var name
  var wire
  var addr
  
  static REG_END = 0xFFFF
  static REG_DELAY = 0xFFFE
  
  def init(name, addr)
    self.name = name
    self.addr = addr
  end
  
  def write_reg16(reg, val)
    if !self.wire return false end
    self.wire._begin_transmission(self.addr)
    self.wire._write((reg >> 8) & 0xFF)
    self.wire._write(reg & 0xFF)
    self.wire._write(val)
    return self.wire._end_transmission() == 0
  end
  
  def read_reg16(reg)
    if !self.wire return nil end
    self.wire._begin_transmission(self.addr)
    self.wire._write((reg >> 8) & 0xFF)
    self.wire._write(reg & 0xFF)
    var err = self.wire._end_transmission(false)
    if err != 0 return nil end
    self.wire._request_from(self.addr, 1)
    if self.wire._available()
      return self.wire._read()
    end
    return nil
  end
  
  def set_reg_bits(reg, offset, length, value)
    var reg_data = self.read_reg16(reg)
    if reg_data == nil return false end
    var mask = ((1 << length) - 1) << offset
    var new_val = (reg_data & ~mask) | ((value << offset) & mask)
    return self.write_reg16(reg, new_val)
  end
  
  def write_array(reg_array)
    var i = 0
    var count = 0
    while i < size(reg_array)
      var reg = reg_array[i][0]
      var val = reg_array[i][1]
      if reg == self.REG_END
        break
      elif reg == self.REG_DELAY
        tasmota.delay(val)
      else
        if !self.write_reg16(reg, val)
          print(format("%s: FAILED at reg 0x%04X", self.name, reg))
          return false
        end
        count += 1
      end
      i += 1
    end
    print(format("%s: Wrote %d registers", self.name, count))
    return true
  end
end

class IMX219 : CSI_Sensor
  static ADDR = 0x10      # Confirmed from Linux driver [web:244]
  static CHIP_ID = 0x0219 # 0x0000 register
  var is_streaming
  var is_initialized
  var line_sync_enable

  def init()
    super(self).init("IMX219", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
    self.line_sync_enable = false
  end

  # Exact Linux chip ID detection
  def detect()
    self.wire = tasmota.wire_scan(self.addr)
    if !self.wire 
      print("IMX219: I2C scan failed")
      return false 
    end
    
    tasmota.delay(10)
    var chip_id = self.read_reg16(0x0000)  # IMX219_REG_CHIP_ID
    if chip_id == nil 
      print("IMX219: Cannot read chip ID")
      return false 
    end
    
    print(format("IMX219: Chip ID = 0x%04X", chip_id))
    return chip_id == self.CHIP_ID
  end

  # IMX219 software standby (matches Linux driver flow)
  def software_reset()
    print("IMX219: Entering standby...")
    self.write_reg16(0x0100, 0x00)  # IMX219_MODE_STANDBY
    tasmota.delay(10)
    
    # Verify responsive
    var id = self.read_reg16(0x0000)
    if id == nil
      print("IMX219: Sensor not responding after standby!")
      return false
    end
    
    print("IMX219: Standby complete")
    return true
  end

  # Exact Linux common registers (imx219_common_regs[])
  def common_regs()
    return [
      [0x0100, 0x00],  # Mode Select -> STANDBY
      
      # Access 3000-5fff registers (Linux sequence)
      [0x30eb, 0x05],
      [0x30eb, 0x0c],
      [0x300a, 0xff],
      [0x300b, 0xff],
      [0x30eb, 0x05],
      [0x30eb, 0x09],
      
      # Undocumented registers (direct from Linux)
      [0x455e, 0x00],
      [0x471e, 0x4b],
      [0x4767, 0x0f],
      [0x4750, 0x14],
      [0x4540, 0x00],
      [0x47b4, 0x14],
      [0x4713, 0x30],
      [0x478b, 0x10],
      [0x478f, 0x10],
      [0x4793, 0x10],
      [0x4797, 0x0e],
      [0x479b, 0x0e],
      
      # Frame Bank A setup
      [0x0170, 0x01],  # X_ODD_INC_A
      [0x0171, 0x01],  # Y_ODD_INC_A
      
      # Output setup
      [0x0128, 0x00],  # DPHY_CTRL_TIMING_AUTO
      [0x012a, 0x60],  # EXCK_FREQ(24MHz) = 24 * 256 = 0x0060
      
      [self.REG_END, 0x00]
    ]
  end

  # Exact Linux 2-lane PLL (imx219_2lane_regs[])
  def pll_2lane_regs()
    return [
      [0x0301, 0x05],  # VTPXCK_DIV
      [0x0303, 0x01],  # VTSYCK_DIV
      [0x0304, 0x03],  # PREPLLCK_VT_DIV (AUTO)
      [0x0305, 0x03],  # PREPLLCK_OP_DIV (AUTO)
      [0x0306, 0x0039], # PLL_VT_MPY = 57 (0x39)
      [0x030b, 0x01],  # OPSYCK_DIV
      [0x030c, 0x0072], # PLL_OP_MPY = 114 (0x72)
      
      [0x0114, 0x01],  # CSI_2_LANE_MODE
      
      [self.REG_END, 0x00]
    ]
  end

  # Mode 0: 1920x1080 RAW10 @ 30fps (supported_modes[1])
  def regs_mode0()
    return [
      # Windowing for 1920x1080 (calculate from Linux logic)
      [0x0164, 0x00a0],  # X_ADD_STA_A (offset from pixel array)
      [0x0166, 0x089f],  # X_ADD_END_A
      [0x0168, 0x0008],  # Y_ADD_STA_A
      [0x016a, 0x0437],  # Y_ADD_END_A
      
      [0x016c, 0x0780],  # X_OUTPUT_SIZE = 1920
      [0x016e, 0x0438],  # Y_OUTPUT_SIZE = 1080
      
      [0x0624, 0x0780],  # TP_WINDOW_WIDTH
      [0x0626, 0x0438],  # TP_WINDOW_HEIGHT
      
      # Timing (from mode table)
      [0x0160, 0x06e3],  # FRM_LENGTH_A = 1763
      [0x0162, 0x0d78],  # LINE_LENGTH_A (min)
      
      # CSI Data Format RAW10
      [0x018c, 0x0a0a],  # (10<<8) | 10
      
      [0x0309, 0x0a],    # OPPXCK_DIV = bpp
      
      [self.REG_END, 0x00]
    ]
  end

  def stream_on(on)
    if !self.wire return false end
    
    if on
      if self.is_streaming
        print("IMX219: Already streaming")
        return true
      end
      
      # Exact Linux: 0x0100 = STREAMING
      if !self.write_reg16(0x0100, 0x01)
        print("IMX219: Failed to enable stream")
        return false
      end
      
      self.is_streaming = true
      print("IMX219: Stream ON")
    else
      if !self.write_reg16(0x0100, 0x00)
        print("IMX219: Failed to disable stream")
        return false
      end
      
      self.is_streaming = false
      print("IMX219: Stream OFF")
    end
    return true
  end

  def set_AE_target(target)
    # Linux uses separate analog/digital gain + exposure
    # For now, placeholder
    return true
  end

  def check_status()
    if !self.wire return false end
    
    print("IMX219: ===== REGISTER READBACK =====")
    
    var r_0100 = self.read_reg16(0x0100)
    var r_0114 = self.read_reg16(0x0114)
    print(format("IMX219: 0x0100 Mode=%s", r_0100 != nil ? format("0x%02X", r_0100) : "ERR"))
    print(format("IMX219: 0x0114 Lanes=%s (expect 0x01=2lane)", r_0114 != nil ? format("0x%02X", r_0114) : "ERR"))
    
    # Output size
    var r_016c = self.read_reg16(0x016c)
    var r_016e = self.read_reg16(0x016e)
    if r_016c != nil && r_016e != nil
      var w = r_016c
      var h = r_016e
      print(format("IMX219: Output=%dx%d", w, h))
    end
    
    print("IMX219: ===== END READBACK =====")
    return true
  end

  # EXACT SAME camera() interface as OV5647
  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("IMX219: ========== INIT START ==========")
      
      if self.is_initialized
        print("IMX219: Already initialized")
        return 1
      end
      
      if !self.detect()
        print("IMX219: Not detected")
        return 0
      end
      
      if !self.software_reset()
        return 0
      end
      
      # Linux sequence: common -> PLL -> mode
      if !self.write_array(self.common_regs())     return 0 end
      if !self.write_array(self.pll_2lane_regs())  return 0 end
      if !self.write_array(self.regs_mode0())      return 0 end
      
      if raw != nil
        # Mode 0: 1920x1080 RAW10, 2 lanes, 456MHz link (from Linux)
        raw[0] = 1920 & 0xFF;      raw[1] = (1920 >> 8) & 0xFF
        raw[2] = 1080 & 0xFF;      raw[3] = (1080 >> 8) & 0xFF
        raw[4] = 1                 # RAW10 (1), RAW8=0
        raw[5] = 0                 # No onboard ISP
        var bitrate = 456          # Mbps/lane from Linux
        raw[6] = bitrate & 0xFF;   raw[7] = (bitrate >> 8) & 0xFF
        raw[8] = (bitrate >> 16) & 0xFF; raw[9] = (bitrate >> 24) & 0xFF
        raw[10] = 2                # 2 lanes
      end
      
      self.is_initialized = true
      print("IMX219: ========== INIT COMPLETE ==========")
      print("IMX219: 1920x1080 RAW10, 2-lane MIPI @ 456 Mbps/lane")
      return 1
      
    elif cmd == "stream"
      return self.stream_on(idx == 1) ? 1 : 0
    
    elif cmd == "status"
      return self.check_status() ? 1 : 0
    
    else
      print(format("IMX219: Unknown '%s'", cmd))
      return 0
    end
  end
end

var imx219 = IMX219()
tasmota.add_driver(imx219)
print("========================================")
print("IMX219 Driver - Linux kernel port")
print("1920x1080 RAW10 @ 30fps, 2-lane CSI")
print("ChipID=0x0219, Addr=0x10")
print("========================================")