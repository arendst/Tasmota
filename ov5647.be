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
  var mipi_clock
  var width, height
  var format, bin_mode
  var resolutions
  
  def init()
    super(self).init("OV5647", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
    self.line_sync_enable = false  # Match CONFIG_CAMERA_OV5647_CSI_LINESYNC_ENABLE default
    self.resolutions = [
      /-> self.regs_800x640_raw8(),
      /-> self.regs_1280x960_raw10(),
      /-> self.regs_1920x1080_raw10(),
      /-> self.regs_1920x1080_raw8_15fps()
    ]
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
    print("OV5647: Configuring 800x640 RAW8...")
    
    # Set resolution and clock parameters
    self.width = 800
    self.height = 640
    self.format = 0  # COLOR_PIXEL_RAW8
    self.bin_mode = 2
    # IDI Clock: 100 MHz, Multiplier: 4, Per lane: (100 * 4) / 2 = 200 Mbps
    self.mipi_clock = 200
    
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
  
  # Register configuration for 1280x960 RAW10 @ 45fps
  # From official ESP-IDF driver: ov5647_mipi_2lane_24Minput_1280x960_raw10_45fps
  def regs_1280x960_raw10()
    print("OV5647: Configuring 1280x960 RAW10...")
    
    # Set resolution and clock parameters
    self.width = 1280
    self.height = 960
    self.format = 1  # COLOR_PIXEL_RAW10
    # IDI Clock: 88.333 MHz, Multiplier: 5, Per lane: (88.333 * 5) / 2 = 220.833 ≈ 221 Mbps
    self.mipi_clock = 221
    self.bin_mode = 2
    
    return [
      # PLL and clock configuration
      [0x3034, 0x1a],           # RAW10 mode (OV5647_10BIT_MODE)
      [0x3035, 0x21],           # system clk div
      [0x3036, 0x6a],           # PLL multiplier
      [0x303c, 0x11],
      [0x3106, 0xf5],           # PLL clock divider
      
      # Flip/mirror and binning
      [0x3820, 0x01],           # horizontal: bit1=mirror, bit0=binning
      [0x3821, 0x03],           # vertical: bit1=flip, bit0=binning
      
      # Timing and analog
      [0x3827, 0xec],
      [0x370c, 0x03],
      [0x3612, 0x59],
      [0x3618, 0x00],
      
      # ISP control
      [0x5000, 0xff],           # LENC enable
      [0x583e, 0xf0],           # LSC max gain
      [0x583f, 0x40],           # LSC min gain
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
      [0x3a18, 0x03],           # gain ceiling
      [0x3a19, 0xff],           # gain ceiling
      [0x3c00, 0x40],
      [0x3b07, 0x0c],
      
      # HTS (line time) = 1796
      [0x380c, 0x07],           # HTS high: (1796 >> 8) = 0x07
      [0x380d, 0x04],           # HTS low: 1796 & 0xFF = 0x04
      
      # VTS (frame time) = 1093
      [0x380e, 0x04],           # VTS high: (1093 >> 8) = 0x04
      [0x380f, 0x45],           # VTS low: 1093 & 0xFF = 0x45
      
      # Subsampling
      [0x3814, 0x31],
      [0x3815, 0x31],
      
      # Output size: 1280x960
      [0x3808, 0x05],           # Width high
      [0x3809, 0x00],           # Width low (1280 = 0x500)
      [0x380a, 0x03],           # Height high
      [0x380b, 0xc0],           # Height low (960 = 0x3c0)
      
      # Window: X start = 24
      [0x3800, 0x00],           # X start high
      [0x3801, 0x18],           # X start low (24 = 0x18)
      
      # Window: Y start = 12
      [0x3802, 0x00],           # Y start high
      [0x3803, 0x0c],           # Y start low (12 = 0x0c)
      
      # Window: X end = 2600
      [0x3804, 0x0a],           # X end high
      [0x3805, 0x27],           # X end low (2600 = 0xa27)
      
      # Window: Y end = 1944
      [0x3806, 0x07],           # Y end high
      [0x3807, 0x97],           # Y end low (1944 = 0x797)
      
      # Timing offset
      [0x3811, 0x05],           # X offset = 4 + 1 to fix BAYER alignment issue
      [0x3813, 0x02],           # Y offset = 2
      
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
      [0x3a09, 0x28],
      [0x3a0a, 0x00],
      [0x3a0b, 0xf6],
      [0x3a0d, 0x08],
      [0x3a0e, 0x06],
      [0x3a0f, 0x58],
      [0x3a10, 0x50],
      [0x3a1b, 0x58],
      [0x3a1e, 0x50],
      [0x3a11, 0x60],
      [0x3a1f, 0x28],
      
      # BLC
      [0x4001, 0x02],
      [0x4004, 0x04],           # BLC line number
      [0x4000, 0x09],
      
      # MIPI timing
      [0x4837, 0x16],           # MIPI pclk period
      [0x4800, 0x24],
      
      # Group hold
      [0x3212, 0xa0],
      
      [self.REG_END, 0x00]
    ]
  end

  # Register configuration for 1920x1080 RAW10 @ 30fps
  # From official ESP-IDF driver: ov5647_mipi_2lane_24Minput_1920x1080_raw10_30fps
  def regs_1920x1080_raw10()
    print("OV5647: Configuring 1920x1080 RAW10 (Espressif esp-video-components)...")
    
    # Set resolution and clock parameters
    self.width = 1920
    self.height = 1080
    self.format = 1  # COLOR_PIXEL_RAW10
    self.bin_mode = 1
    
    # IDI Clock: 81.6667 MHz
    # Line Rate: 81.6667 * 5 = 408.33 Mbps
    self.mipi_clock = 408
    
    return [
      # PLL and clock configuration
      [0x3034, 0x1a],           # RAW10 mode (OV5647_10BIT_MODE)
      [0x3035, 0x21],           # system clk div
      [0x3036, 0x62],           # PLL multiplier (98x)
      [0x303c, 0x11],           # plls_sys_div
      [0x3106, 0xf5],           # PLL clock divider
      
      # Flip/mirror and binning
      [0x3821, 0x02],           # vertical: bit1=flip, bit0=0 (no binning)
      [0x3820, 0x00],           # horizontal: bit1=mirror, bit0=0 (no binning)
      
      # Timing and analog
      [0x3827, 0xec],
      [0x370c, 0x03],
      [0x3612, 0x5b],
      [0x3618, 0x04],
      
      # ISP control
      [0x5000, 0xff],           # LENC en
      [0x583e, 0xf0],           # max gain
      [0x583f, 0x4f],           # min gain
      
      [0x5003, 0x08],
      [0x5a00, 0x08],
      
      # Pad control
      [0x3000, 0x00],
      [0x3001, 0x00],
      [0x3002, 0x00],
      [0x3016, 0x08],
      [0x3017, 0xe0],
      [0x3018, 0x44],           # 2 lane, MIPI
      [0x301c, 0xf8],
      [0x301d, 0xf0],
      
      # AEC/AGC
      [0x3a18, 0x03],           # gain_ceiling, max gain value
      [0x3a19, 0xff],           # gain_ceiling
      [0x3c00, 0x40],
      [0x3b07, 0x0c],
      
      # HTS (line time) = 2271
      [0x380c, 0x08],           # HTS high: 0x08
      [0x380d, 0xdf],           # HTS low: 0xDF (2271)
      
      # VTS (frame time) = 1199
      [0x380e, 0x04],           # VTS high: 0x04
      [0x380f, 0xaf],           # VTS low: 0xAF (1199)
      
      # Subsampling (Disabled)
      [0x3814, 0x11],           # horizontal subsampling (1/1)
      [0x3815, 0x11],           # vertical subsampling (1/1)
      [0x3708, 0x64],
      [0x3709, 0x12],
      
      # Output size: 1920x1080
      [0x3808, 0x07],           # x output high
      [0x3809, 0x80],           # x output low (1920)
      [0x380a, 0x04],           # y output high
      [0x380b, 0x38],           # y output low (1080)
      
      # Window: X start = 348
      [0x3800, 0x01],           # x start high
      [0x3801, 0x5c],           # x start low (348)
      
      # Window: Y start = 434
      [0x3802, 0x01],           # y start high
      [0x3803, 0xb2],           # y start low (434)
      
      # Window: X end = 2275
      [0x3804, 0x08],           # x end high
      [0x3805, 0xe3],           # x end low (2275)
      
      # Window: Y end = 1521
      [0x3806, 0x05],           # y end high
      [0x3807, 0xf1],           # y end low (1521)
      
      # Timing offset
      [0x3811, 0x05],           # x offset: 4
      [0x3813, 0x02],           # y offset: 2
      
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
      [0x3a08, 0x01],           # AEC control, 50Hz step
      [0x3a09, 0x4b],
      [0x3a0a, 0x01],           # 60Hz step
      [0x3a0b, 0x13],
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
      [0x4004, 0x04],           # blc line number
      [0x4000, 0x09],
      
      # MIPI timing
      [0x4837, 0x19],           # 0x19 = 25
      [0x4800, 0x34],
      
      [self.REG_END, 0x00]
    ]
  end

  # Register configuration for 1920x1080 RAW8 @ 15fps
  # Strategy: Lower speed to 200Mbps (PHY Safe) + RAW8 (Bandwidth Safe) + 15fps
  def regs_1920x1080_raw8_15fps()
    print("OV5647: Configuring 1920x1080 RAW8 @ 15fps (Slow Clock Mode)...")
    
    # 1. Geometry & Format
    self.width = 1920
    self.height = 1080
    self.format = 0  # COLOR_PIXEL_RAW8 (Critical change from 0x1A)
    self.bin_mode = 1
    
    # 2. Clock: Use the conservative 200 Mbps setting
    # This guarantees the ESP32-P4 PHY will lock
    self.mipi_clock = 200
    
    # 3. Timing Calculation for 15 FPS
    # PCLK = 100 MHz (Standard for Slow config)
    # HTS (Line Length) = 2271 (Borrowed from Espressif 1080p config)
    # VTS (Frame Length) = PCLK / (HTS * FPS)
    # VTS = 100,000,000 / (2271 * 15) = 2935
    var hts = 2271
    var vts = 2935

    return [
      # === CLOCK: SLOW CONFIG (200 Mbps/lane) ===
      [0x3034, 0x18],           # RAW8 Mode (OV5647_8BIT_MODE) <--- KEY CHANGE
      [0x3035, 0x41],           # SysDiv = 4 (Standard defensive)
      [0x3036, 0x80],           # Mult = 128 -> 200 Mbps/lane
      [0x303c, 0x11], 
      [0x3106, 0xf5],
      
      # === BINNING: DISABLED ===
      [0x3821, 0x02],           # Vertical: Flip=1, Bin=0
      [0x3820, 0x00],           # Horizontal: Mirror=0, Bin=0
      [0x3814, 0x11],           # No Subsampling
      [0x3815, 0x11],
      
      # === GEOMETRY: INPUT CROP ===
      # We use Espressif's 1080p window coordinates.
      # We MUST crop at input because 200Mbps is too slow for full array readout.
      
      # Window: X start = 348 (0x015C)
      [0x3800, 0x01], [0x3801, 0x5c],
      
      # Window: Y start = 434 (0x01B2)
      [0x3802, 0x01], [0x3803, 0xb2],
      
      # Window: X end = 2275 (0x08E3)
      [0x3804, 0x08], [0x3805, 0xe3],
      
      # Window: Y end = 1521 (0x05F1)
      [0x3806, 0x05], [0x3807, 0xf1],
      
      # Output Size: 1920x1080
      [0x3808, 0x07], [0x3809, 0x80],
      [0x380a, 0x04], [0x380b, 0x38],
      
      # ISP Offsets (Matches Espressif)
      [0x3810, 0x00], [0x3811, 0x05],
      [0x3812, 0x00], [0x3813, 0x02],
      
      # === TIMING (Adjusted for 15 FPS) ===
      # HTS = 2271
      [0x380c, (hts >> 8) & 0xFF], [0x380d, hts & 0xFF],
      
      # VTS = 2935 (Slows down frame rate to 15fps)
      [0x380e, (vts >> 8) & 0xFF], [0x380f, vts & 0xFF],
      
      # === ANALOG/ISP (Standard 1080p Settings) ===
      [0x3827,0xec], [0x370c,0x03], [0x3612,0x5b], [0x3618,0x04],
      [0x5000,0xff], [0x583e,0xf0], [0x583f,0x4f],
      [0x5003,0x08], [0x5a00,0x08],
      
      # === MIPI SETUP ===
      [0x3000,0x00], [0x3001,0x00], [0x3002,0x00], 
      [0x3016,0x08], [0x3017,0xe0], [0x3018,0x44], 
      [0x301c,0xf8], [0x301d,0xf0],
      
      [0x3a18,0x03], [0x3a19,0xff], [0x3c00,0x40], [0x3b07,0x0c],
      [0x4001,0x02], [0x4004,0x04], [0x4000,0x09],
      
      # === MIPI TIMING ===
      # 0x28 = 40. Matches the 200Mbps clock (100MHz / 4 lanes logic)
      [0x4837, 0x28], 
      [0x4800, 0x24],
      
      [self.REG_END, 0x00]
    ]
  end



  # NEW: Custom Resolution Calculator
  # Supports Bin 2 (Proven) and Bin 1 (Proven)
  def regs_custom(x, y, w, h, bin, fps)
    
    # 1. Enforce Width Alignment
    if (w % 8) != 0
      w = (w / 8) * 8
    end
    
    # 2. Minimum dimensions
    if w < 16 w = 16 end
    if h < 16 h = 16 end

    print(format("OV5647: Configuring Custom Window %dx%d at (%d,%d) FPS=%d Bin=%d", w, h, x, y, fps, bin))
    
    self.width = w
    self.height = h
    self.format = 0 # RAW8
    self.mipi_clock = 200 # Standard 200Mbps

    var regs = []
    
    if bin == 2
      self.bin_mode = 2
      # === BIN 2 (2x2) CONFIGURATION (Proven Working) ===
      var base_x = 9
      var base_y = 0
      var final_x = base_x + x
      var final_y = base_y + y
      
      if fps == 0 fps = 30 end
      var vts = 100000000 / (1896 * fps)
      if vts < h vts = h + 4 end

      return [
        [0x3034,0x18], [0x3035,0x41], [0x3036,0x80], [0x303c,0x11], [0x3106,0xf5],
        [0x3814,0x31], [0x3815,0x31], 
        [0x3820,0x41], [0x3821,0x03], 
        [0x3827,0xec], [0x370c,0x0f], [0x3612,0x59], [0x3618,0x00],
        [0x5000,0xff], [0x583e,0xf0], [0x583f,0x20], [0x5002,0x41], [0x5003,0x08], [0x5a00,0x08],
        [0x3000,0x00], [0x3001,0x00], [0x3002,0x00], [0x3016,0x08], [0x3017,0xe0], [0x3018,0x44], [0x301c,0xf8], [0x301d,0xf0],
        [0x3a18,0x00], [0x3a19,0xf8], [0x3c01,0x80], [0x3c00,0x40], [0x3b07,0x0c],
        [0x380c,0x07], [0x380d,0x68], 
        [0x380e, (vts >> 8) & 0xFF],  [0x380f, vts & 0xFF],
        [0x3800,0x00], [0x3801,0x00], 
        [0x3802,0x00], [0x3803,0x00], 
        [0x3804,0x0a], [0x3805,0x3f], 
        [0x3806,0x07], [0x3807,0xa1], 
        [0x3808, (w >> 8) & 0xFF], [0x3809, w & 0xFF],
        [0x380a, (h >> 8) & 0xFF], [0x380b, h & 0xFF],
        [0x3810, (final_x >> 8) & 0xFF], [0x3811, final_x & 0xFF],
        [0x3812, (final_y >> 8) & 0xFF], [0x3813, final_y & 0xFF],
        [0x3630,0x2e], [0x3632,0xe2], [0x3633,0x23], [0x3634,0x44], [0x3636,0x06], [0x3620,0x64], [0x3621,0xe0], [0x3600,0x37],
        [0x3704,0xa0], [0x3703,0x5a], [0x3715,0x78], [0x3717,0x01], [0x3731,0x02], [0x370b,0x60], [0x3705,0x1a],
        [0x3f05,0x02], [0x3f06,0x10], [0x3f01,0x0a],
        [0x3a08,0x01], [0x3a09,0x27], [0x3a0a,0x00], [0x3a0b,0xf6], [0x3a0d,0x04], [0x3a0e,0x03], [0x3a0f,0x58], [0x3a10,0x50], [0x3a1b,0x58], [0x3a1e,0x50], [0x3a11,0x60], [0x3a1f,0x28],
        [0x4001,0x02], [0x4004,0x02], [0x4000,0x09],
        [0x4837,0x28], [0x4050,0x6e], [0x4051,0x8f], 
        [self.REG_END,0x00]
      ]
      
   else # Bin == 1 (1080p / Full Res / Cropped)
      self.bin_mode = 1
      if fps > 15 || fps == 0
        fps = 15
      end
      
      # === GEOMETRY CALCULATION ===
      # Base Center + Offset Logic to match Espressif Preset
      # Preset 1920x1080 uses: StartX=348, StartY=434
      # Naive Center (2592-1920)/2 = 336. Diff = +12
      # Naive Center (1944-1080)/2 = 432. Diff = +2
      
      var start_x = (2592 - w) / 2 + 12
      var start_y = (1944 - h) / 2 + 2
      
      # Apply user pan
      start_x = start_x + x
      start_y = start_y + y
      
      # Alignment
      # X must be multiple of 4 (Bayer). Y must be multiple of 2 (Bayer).
      start_x = (start_x / 4) * 4
      start_y = (start_y / 2) * 2
      
      # Input Size Logic (+8 Padding Requirement)
      # Input Window must be larger than Output Size for ISP interpolation.
      # Preset Input: 1928x1088 (Output 1920x1080) -> +8 Padding
      var end_x = start_x + w + 8 - 1
      var end_y = start_y + h + 8 - 1
      
      # VTS Calculation (HTS 2271)
      var hts = 2271
      var vts = 100000000 / (hts * fps)
      if vts < h vts = h + 50 end

      return [
        # Clock: 200 Mbps (Golden Config)
        [0x3034, 0x18], [0x3035, 0x41], [0x3036, 0x80], [0x303c, 0x11], [0x3106, 0xf5],
        
        # Binning DISABLED
        [0x3814, 0x11], [0x3815, 0x11], [0x3820, 0x00], [0x3821, 0x02],
        
        # Input Window (Padded)
        [0x3800, (start_x >> 8) & 0xFF], [0x3801, start_x & 0xFF],
        [0x3802, (start_y >> 8) & 0xFF], [0x3803, start_y & 0xFF],
        [0x3804, (end_x >> 8) & 0xFF],   [0x3805, end_x & 0xFF],
        [0x3806, (end_y >> 8) & 0xFF],   [0x3807, end_y & 0xFF],
        
        # Output Size (Original Requested w/h)
        [0x3808, (w >> 8) & 0xFF], [0x3809, w & 0xFF],
        [0x380a, (h >> 8) & 0xFF], [0x380b, h & 0xFF],
        
        # ISP Offsets (Matches Preset: 5, 2)
        [0x3810, 0x00], [0x3811, 0x05],
        [0x3812, 0x00], [0x3813, 0x02],
        
        # Timing
        [0x380c, (hts >> 8) & 0xFF], [0x380d, hts & 0xFF], 
        [0x380e, (vts >> 8) & 0xFF], [0x380f, vts & 0xFF],
        
        # Analog & AEC (Matches Preset Exactly)
        [0x3827,0xec], [0x370c,0x03], [0x3612,0x5b], [0x3618,0x04],
        [0x5000,0xff], [0x583e,0xf0], [0x583f,0x4f], [0x5003,0x08], [0x5a00,0x08],
        [0x3000,0x00], [0x3001,0x00], [0x3002,0x00], [0x3016,0x08], [0x3017,0xe0], [0x3018,0x44], [0x301c,0xf8], [0x301d,0xf0],
        [0x3a18,0x03], [0x3a19,0xff], [0x3c00,0x40], [0x3b07,0x0c],
        
        # BLC Line Number (Matches Preset: 4)
        [0x4001,0x02], [0x4004,0x04], [0x4000,0x09],
        
        # MIPI Timing
        [0x4837, 0x28], [0x4800, 0x24],
        
        [self.REG_END,0x00]
      ]
    end
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
      
      # Detect sensor only on first init
      if !self.is_initialized
        if !self.detect()
          print("OV5647: Not detected")
          return 0
        end
        self.is_initialized = true
      end
      
      # Software reset (always)
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
      
      # Variables for resolution logic
      var res_idx = 0
      var req_x = 0
      var req_y = 0
      var req_w = 0
      var req_h = 0
      var req_bin = 0
      var req_fps = 0
      
      # Read Configuration from C++ Struct (Byte 26 = res_index)
      if idx != 0
        import introspect
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        res_idx = b[26]
        
        # Check for Custom Mode (255)
        if res_idx == 255
           req_x = b.get(12, 2)   # offset_x
           req_y = b.get(14, 2)   # offset_y
           req_w = b.get(0, 2)    # width (proposed)
           req_h = b.get(2, 2)    # height (proposed)
           req_bin = b[16]        # binning
           req_fps = b[17]        # fps
           print(format("OV5647: Custom Mode Requested: %dx%d @ (%d,%d) FPS=%d", req_w, req_h, req_x, req_y, req_fps))
        else
           print(format("OV5647: Resolution index from C++: %d", res_idx))
        end
      end
      
      # Select Configuration Registers
      var regs = nil
      if res_idx == 255
         # Call custom calculator
         regs = self.regs_custom(req_x, req_y, req_w, req_h, req_bin, req_fps)
      else
         # Validate standard index
         if res_idx < 0 || res_idx >= size(self.resolutions)
           print(format("OV5647: Invalid resolution index %d, using 0", res_idx))
           res_idx = 0
         end
         # Call standard preset
         regs = self.resolutions[res_idx]()
      end
      
      # Apply Registers
      if !self.write_array(regs)
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
      
      # Write-Back Actual Configuration to C++ Struct (Zero-Copy)
      if idx != 0
        import introspect
        print(format("OV5647: Zero-copy config update at 0x%08X", idx))
        
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        
        # Update C++ struct with what we ACTUALLY configured
        b.set(0, self.width, 2)   # width
        b.set(2, self.height, 2)  # height
        b.set(4, 2592, 2)         # max_width
        b.set(6, 1944, 2)         # max_height
        b[8] = self.format        # format
        b[9] = 2                  # lanes
        b.set(10, self.mipi_clock, 2) # mipi_clock
        
        # Don't overwrite request parameters (offset_x/y) unless needed
        # b.set(12, req_x, 2) 
        # b.set(14, req_y, 2)
        
        b[16] = self.bin_mode
        b[17] = req_fps           # fps (Echo back requested FPS or calculated?)
        b.setbytes(18, bytes().fromstring("OV5647"))
        
        b[26] = res_idx           # res_index
        b[27] = 0                 # flags
        
        print(format("OV5647: Config updated back to C++: %dx%d", self.width, self.height))
      end
      
      print("OV5647: ========== INIT COMPLETE ==========")
      return 1
      
    elif cmd == "stream"
      print("OV5647: Stream command:", idx)
      var result = self.stream_on(idx == 1) ? 1 : 0
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
print("========================================")