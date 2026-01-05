# OV5647 CSI Camera Driver - Based on Espressif Official Driver
# Corrected based on esp_cam_sensor OV5647 implementation

class CSI_Sensor
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
  
  # Set specific bits in a register (read-modify-write)
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

class OV5647 : CSI_Sensor
  static ADDR = 0x36
  static CHIP_ID = 0x5647
  var is_streaming
  var is_initialized
  var line_sync_enable
  
  def init()
    super(self).init("OV5647", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
    self.line_sync_enable = false  # Match CONFIG_CAMERA_OV5647_CSI_LINESYNC_ENABLE default
  end
  
  def detect()
    self.wire = tasmota.wire_scan(self.addr)
    if !self.wire 
      print("OV5647: I2C scan failed")
      return false 
    end
    
    tasmota.delay(10)
    var id_h = self.read_reg16(0x300A)
    var id_l = self.read_reg16(0x300B)
    
    if id_h == nil || id_l == nil 
      print("OV5647: Cannot read chip ID")
      return false 
    end
    
    var chip_id = (id_h << 8) | id_l
    print(format("OV5647: Chip ID = 0x%04X", chip_id))
    
    return chip_id == self.CHIP_ID
  end
  
  # Software reset - Uses bit manipulation like official driver
  def software_reset()
    print("OV5647: Software reset...")
    
    # Stop stream first
    self.write_reg16(0x0100, 0x00)
    tasmota.delay(10)
    
    # Set bit 0 of 0x0103 to trigger reset (self-clearing)
    if !self.set_reg_bits(0x0103, 0, 1, 0x01)
      print("OV5647: Reset failed!")
      return false
    end
    
    # Wait for reset to complete
    tasmota.delay(5)
    
    # Verify sensor is responsive
    var id_h = self.read_reg16(0x300A)
    if id_h == nil
      print("OV5647: Sensor not responding after reset!")
      return false
    end
    
    print("OV5647: Reset complete")
    return true
  end
  
  # MIPI reset registers (from official driver ov5647_mipi_reset_regs)
  def mipi_reset_regs()
    return [
      [0x0100, 0x00],           # enable sleep
      [0x0103, 0x01],           # soft reset
      [self.REG_DELAY, 10],     # 10ms delay
      [0x4800, 0x01],           # clock lane gate (BIT(0))
      [self.REG_END, 0x00]
    ]
  end
  
  # Stream control - matches official driver logic
  def stream_on(on)
    if !self.wire return false end
    
    if on
      if self.is_streaming
        print("OV5647: Already streaming")
        return true
      end
      
      # Configure MIPI - matches official driver
      var mipi_val = self.line_sync_enable ? 0x14 : 0x00
      if !self.write_reg16(0x4800, mipi_val)
        print("OV5647: Failed to configure MIPI")
        return false
      end
      
      # Enable streaming
      if !self.write_reg16(0x0100, 0x01)
        print("OV5647: Failed to enable stream")
        return false
      end
      
      self.is_streaming = true
      print("OV5647: Stream ON")
    else
      # Disable streaming
      if !self.write_reg16(0x0100, 0x00)
        print("OV5647: Failed to disable stream")
        return false
      end
      
      self.is_streaming = false
      print("OV5647: Stream OFF")
    end
    return true
  end
  
  # AE target configuration (from official driver)
  def set_AE_target(target)
    var AE_low = target * 23 / 25   # 0.92
    var AE_high = target * 27 / 25  # 1.08
    var fast_high = AE_high << 1
    if fast_high > 255
      fast_high = 255
    end
    var fast_low = AE_low >> 1
    
    var ret = true
    ret = ret && self.write_reg16(0x3a0f, AE_high)
    ret = ret && self.write_reg16(0x3a10, AE_low)
    ret = ret && self.write_reg16(0x3a1b, AE_high)
    ret = ret && self.write_reg16(0x3a1e, AE_low)
    ret = ret && self.write_reg16(0x3a11, fast_high)
    ret = ret && self.write_reg16(0x3a1f, fast_low)
    return ret
  end
  
  # Register configuration for 800x640 RAW8 @ 50fps
  # EXACT copy from official ESP-IDF driver: ov5647_input_24M_MIPI_2lane_raw8_800x640_50fps
  def regs_800x640_raw8()
    return [
      # PLL and clock configuration
      [0x3034, 0x18],           # RAW8 mode (OV5647_8BIT_MODE)
      [0x3035, 0x41],           # system clk div
      [0x3036, 0x80],           # PLL multiplier: (100MHz * 8 * 4) / 25MHz = 128 = 0x80
      [0x303c, 0x11],
      [0x3106, 0xf5],           # PLL clock divider
      
      # Flip/mirror and binning
      [0x3821, 0x03],           # vertical: bit1=flip, bit0=binning
      [0x3820, 0x41],           # horizontal: bit1=mirror, bit0=binning
      
      # Timing and analog
      [0x3827, 0xec],
      [0x370c, 0x0f],
      [0x3612, 0x59],
      [0x3618, 0x00],
      
      # ISP control
      [0x5000, 0xff],           # LENC enable
      [0x583e, 0xf0],           # LSC max gain
      [0x583f, 0x20],           # LSC min gain
      [0x5002, 0x41],
      [0x5003, 0x08],
      [0x5a00, 0x08],
      
      # Pad control
      [0x3000, 0x00],
      [0x3001, 0x00],
      [0x3002, 0x00],
      [0x3016, 0x08],
      [0x3017, 0xe0],
      [0x3018, 0x44],           # 2 lane MIPI
      [0x301c, 0xf8],
      [0x301d, 0xf0],
      
      # AEC/AGC
      [0x3a18, 0x00],
      [0x3a19, 0xf8],
      [0x3c01, 0x80],
      [0x3c00, 0x40],
      [0x3b07, 0x0c],
      
      # HTS (line time) = 1896
      [0x380c, 0x07],           # HTS high: (1896 >> 8) & 0x1F = 0x07
      [0x380d, 0x68],           # HTS low: 1896 & 0xFF = 0x68
      
      # VTS (frame time) = 984
      [0x380e, 0x03],           # VTS high: (984 >> 8) & 0xFF = 0x03
      [0x380f, 0xd8],           # VTS low: 984 & 0xFF = 0xd8
      
      # Subsampling
      [0x3814, 0x31],
      [0x3815, 0x31],
      [0x3708, 0x64],
      [0x3709, 0x52],
      
      # Window: X start = 500
      [0x3800, 0x01],           # X start high
      [0x3801, 0xf4],           # X start low (500 = 0x1f4)
      
      # Window: Y start = 0
      [0x3802, 0x00],
      [0x3803, 0x00],
      
      # Window: X end = 2623
      [0x3804, 0x0a],           # X end high
      [0x3805, 0x3f],           # X end low (2623 = 0xa3f)
      
      # Window: Y end = 1953
      [0x3806, 0x07],           # Y end high
      [0x3807, 0xa1],           # Y end low (1953 = 0x7a1)
      
      # Output size: 800x640
      [0x3808, 0x03],           # Width high
      [0x3809, 0x20],           # Width low (800 = 0x320)
      [0x380a, 0x02],           # Height high
      [0x380b, 0x80],           # Height low (640 = 0x280)
      
      # Timing offset
      [0x3810, 0x00],
      [0x3811, 0x09],           # X offset = 8 + 1 try to fix BAYER alignment issue
      [0x3812, 0x00],
      [0x3813, 0x00],           # Y offset = 0
      
      # Analog settings
      [0x3630, 0x2e],
      [0x3632, 0xe2],
      [0x3633, 0x23],
      [0x3634, 0x44],
      [0x3636, 0x06],
      [0x3620, 0x64],
      [0x3621, 0xe0],
      [0x3600, 0x37],
      [0x3704, 0xa0],
      [0x3703, 0x5a],
      [0x3715, 0x78],
      [0x3717, 0x01],
      [0x3731, 0x02],
      [0x370b, 0x60],
      [0x3705, 0x1a],
      [0x3f05, 0x02],
      [0x3f06, 0x10],
      [0x3f01, 0x0a],
      
      # AEC control
      [0x3a08, 0x01],
      [0x3a09, 0x27],
      [0x3a0a, 0x00],
      [0x3a0b, 0xf6],
      [0x3a0d, 0x04],
      [0x3a0e, 0x03],
      [0x3a0f, 0x58],
      [0x3a10, 0x50],
      [0x3a1b, 0x58],
      [0x3a1e, 0x50],
      [0x3a11, 0x60],
      [0x3a1f, 0x28],
      
      # BLC
      [0x4001, 0x02],
      [0x4004, 0x02],
      [0x4000, 0x09],
      
      # MIPI timing
      [0x4837, 0x28],           # MIPI pclk period: 1000000000 / (100MHz / 4) = 40 = 0x28
      [0x4050, 0x6e],
      [0x4051, 0x8f],
      
      [self.REG_END, 0x00]
    ]
  end
  
  # Check sensor status - read back critical registers to verify configuration
  def check_status()
    if !self.wire return false end
    
    print("OV5647: ===== REGISTER READBACK =====")
    
    # Stream and MIPI control
    var r_0100 = self.read_reg16(0x0100)  # Stream mode
    var r_4800 = self.read_reg16(0x4800)  # MIPI control
    print(format("OV5647: 0x0100 Stream=%s (expect 0x01 when streaming)", r_0100 != nil ? format("0x%02X", r_0100) : "ERR"))
    print(format("OV5647: 0x4800 MIPI=%s (expect 0x00 or 0x14)", r_4800 != nil ? format("0x%02X", r_4800) : "ERR"))
    
    # PLL configuration
    var r_3034 = self.read_reg16(0x3034)  # Format (0x18 = RAW8)
    var r_3035 = self.read_reg16(0x3035)  # System clk div
    var r_3036 = self.read_reg16(0x3036)  # PLL multiplier
    print(format("OV5647: 0x3034 Format=%s (expect 0x18 RAW8)", r_3034 != nil ? format("0x%02X", r_3034) : "ERR"))
    print(format("OV5647: 0x3035 SysClkDiv=%s (expect 0x41)", r_3035 != nil ? format("0x%02X", r_3035) : "ERR"))
    print(format("OV5647: 0x3036 PLLMult=%s (expect 0x80)", r_3036 != nil ? format("0x%02X", r_3036) : "ERR"))
    
    # MIPI lane config
    var r_3018 = self.read_reg16(0x3018)  # MIPI lane config
    print(format("OV5647: 0x3018 MIPILane=%s (expect 0x44 = 2 lanes)", r_3018 != nil ? format("0x%02X", r_3018) : "ERR"))
    
    # Timing - HTS/VTS
    var r_380c = self.read_reg16(0x380c)
    var r_380d = self.read_reg16(0x380d)
    var r_380e = self.read_reg16(0x380e)
    var r_380f = self.read_reg16(0x380f)
    if r_380c != nil && r_380d != nil
      var hts = (r_380c << 8) | r_380d
      print(format("OV5647: HTS=%d (expect 1896)", hts))
    end
    if r_380e != nil && r_380f != nil
      var vts = (r_380e << 8) | r_380f
      print(format("OV5647: VTS=%d (expect 984)", vts))
    end
    
    # Output size
    var r_3808 = self.read_reg16(0x3808)
    var r_3809 = self.read_reg16(0x3809)
    var r_380a = self.read_reg16(0x380a)
    var r_380b = self.read_reg16(0x380b)
    if r_3808 != nil && r_3809 != nil && r_380a != nil && r_380b != nil
      var width = (r_3808 << 8) | r_3809
      var height = (r_380a << 8) | r_380b
      print(format("OV5647: Output size=%dx%d (expect 800x640)", width, height))
    end
    
    # MIPI timing
    var r_4837 = self.read_reg16(0x4837)
    print(format("OV5647: 0x4837 MIPITiming=%s (expect 0x28)", r_4837 != nil ? format("0x%02X", r_4837) : "ERR"))
    
    # Flip/mirror
    var r_3820 = self.read_reg16(0x3820)
    var r_3821 = self.read_reg16(0x3821)
    print(format("OV5647: 0x3820 HFlip=%s (expect 0x41)", r_3820 != nil ? format("0x%02X", r_3820) : "ERR"))
    print(format("OV5647: 0x3821 VFlip=%s (expect 0x03)", r_3821 != nil ? format("0x%02X", r_3821) : "ERR"))
    
    print("OV5647: ===== END READBACK =====")
    return true
  end
  
  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("OV5647: ========== INIT START ==========")
      
      if self.is_initialized
        print("OV5647: Already initialized")
        return 1
      end
      
      # Detect sensor
      if !self.detect()
        print("OV5647: Not detected")
        return 0
      end
      
      # Software reset
      if !self.software_reset()
        return 0
      end
      tasmota.delay(10)
      
      # Apply MIPI reset configuration
      print("OV5647: Applying MIPI reset...")
      if !self.write_array(self.mipi_reset_regs())
        print("OV5647: MIPI reset failed")
        return 0
      end
      
      # Configure for 800x640 RAW8
      print("OV5647: Configuring 800x640 RAW8...")
      if !self.write_array(self.regs_800x640_raw8())
        print("OV5647: Config failed")
        return 0
      end
      
      # Set AE target (default 0x50 from official driver)
      if !self.set_AE_target(0x50)
        print("OV5647: AE target failed")
      end
      
      # Verify stream is OFF
      var stream_reg = self.read_reg16(0x0100)
      if stream_reg != nil && stream_reg != 0x00
        print(format("OV5647: WARNING - Stream register = 0x%02X", stream_reg))
      end
      
      # Zero-copy: idx contains the memory address of C++ CSI_Config struct
      if idx != 0
        import introspect
        print(format("OV5647: Zero-copy config at 0x%08X", idx))
        
        # Create pointer and map to bytes (direct memory access)
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)  # 28-byte CSI_Config struct (was 24)
        
        print(format("OV5647: Mapped buffer type=%s size=%d", type(b), size(b)))
        
        # Fill struct directly in C++ memory
        # 0-1:   uint16_t width
        # 2-3:   uint16_t height
        # 4-5:   uint16_t max_width
        # 6-7:   uint16_t max_height
        # 8:     uint8_t format
        # 9:     uint8_t lane_num
        # 10-11: uint16_t mipi_clock
        # 12-13: uint16_t crop_x
        # 14-15: uint16_t crop_y
        # 16:    uint8_t binning
        # 17:    uint8_t flags
        # 18-25: char name[8]
        # 26-27: uint8_t reserved[2]
        
        b.set(0, 800, 2)        # width
        b.set(2, 640, 2)        # height
        b.set(4, 2592, 2)       # max_width (OV5647 native: 2592x1944)
        b.set(6, 1944, 2)       # max_height
        b[8] = 0                # format: RAW8
        b[9] = 2                # lanes: 2
        b.set(10, 200, 2)       # mipi_clock: 200 Mbps/lane
        b.set(12, 500, 2)       # crop_x: 500 (from register config)
        b.set(14, 0, 2)         # crop_y: 0
        b[16] = 0               # binning: 0 (1x1)
        b[17] = 0               # flags: 0
        b.setbytes(18, bytes().fromstring("OV5647"))
        
        b[26] = 0               # reserved
        b[27] = 0               # reserved
        
        # Verify what we wrote
        print(format("OV5647: Verify - width=%d height=%d", b.get(0, 2), b.get(2, 2)))
        print(format("OV5647: Verify - max=%dx%d", b.get(4, 2), b.get(6, 2)))
        print(format("OV5647: Verify - format=%d lanes=%d clock=%d", b[8], b[9], b.get(10, 2)))
        print(format("OV5647: Verify - crop_x=%d crop_y=%d", b.get(12, 2), b.get(14, 2)))
        print(format("OV5647: Verify - name[0-3]=%02X %02X %02X %02X", b[18], b[19], b[20], b[21]))
        
        print("OV5647: Zero-copy config complete")
      end
      
      self.is_initialized = true
      print("OV5647: ========== INIT COMPLETE ==========")
      print("OV5647: 800x640 RAW8, GBRG Bayer, 2-lane MIPI @ 400 Mbps")
      return 1
      
    elif cmd == "stream"
      print("OV5647: Stream command:", idx)
      var result = self.stream_on(idx == 1) ? 1 : 0
      # tasmota.delay(10)
      # self.check_status()
      return result
    
    elif cmd == "status"
      return self.check_status() ? 1 : 0
    
    else
      print(format("OV5647: Unknown command '%s'", cmd))
      return 0
    end
  end
end

# Register driver
var ov5647 = OV5647()
tasmota.add_driver(ov5647)
print("========================================")
print("OV5647 Driver - Based on ESP-IDF")
print("Bayer: GBRG (matches official driver)")
print("Format: 800x640 RAW8 @ 50fps")
print("MIPI: 2-lane @ 200 Mbps/lane (400 Mbps total)")
print("========================================")