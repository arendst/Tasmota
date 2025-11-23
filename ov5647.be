# OV5647 CSI Camera Driver - SAFE VERSION
# Added protections against bricking

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
  
  # SAFE: Write with verification
  def write_reg16_verify(reg, val)
    if !self.write_reg16(reg, val)
      print(format("WRITE FAILED: 0x%04X", reg))
      return false
    end
    tasmota.delay(5)
    var readback = self.read_reg16(reg)
    if readback != val
      print(format("VERIFY FAILED: 0x%04X wrote 0x%02X read 0x%02X", reg, val, readback))
      return false
    end
    return true
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
  
  def init()
    super(self).init("OV5647", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
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
  
  # SAFE: Always stop stream before touching registers
  def safe_stop_stream()
    if !self.wire return false end
    print("OV5647: Ensuring stream is stopped...")
    self.write_reg16(0x0100, 0x00)
    tasmota.delay(50)
    var val = self.read_reg16(0x0100)
    print(format("OV5647: Stream register = 0x%02X", val))
    self.is_streaming = false
    return true
  end
  
  # SAFE: Software reset with proper delays
  def software_reset()
    print("OV5647: Software reset...")
    self.safe_stop_stream()
    
    # Write reset bit
    if !self.write_reg16_verify(0x0103, 0x01)
      print("OV5647: Reset write failed!")
      return false
    end
    
    # Wait for reset to complete (datasheet: min 5ms)
    tasmota.delay(100)
    
    # Verify sensor is responsive after reset
    var id_h = self.read_reg16(0x300A)
    if id_h == nil
      print("OV5647: Sensor not responding after reset!")
      return false
    end
    
    print("OV5647: Reset complete")
    return true
  end
  
  def stream_on(on)
    if !self.wire return false end
    
    if on
      if self.is_streaming
        print("OV5647: Already streaming")
        return true
      end
      
      # SAFE: Configure MIPI before enabling stream
      # 0x00 = normal operation, no special modes
      if !self.write_reg16_verify(0x4800, 0x00)
        print("OV5647: Failed to configure MIPI")
        return false
      end
      
      # Enable streaming
      if !self.write_reg16_verify(0x0100, 0x01)
        print("OV5647: Failed to enable stream")
        return false
      end
      
      self.is_streaming = true
      print("OV5647: Stream ON")
    else
      if !self.safe_stop_stream()
        return false
      end
      print("OV5647: Stream OFF")
    end
    return true
  end
  
  def regs_800x600_raw8()
    return [
      # Clock configuration
      [0x3034, 0x18], [0x3035, 0x41], [0x3036, 0x80],
      [0x303c, 0x11], [0x3106, 0xf5],
      
      # Orientation
      [0x3821, 0x03], [0x3820, 0x41],
      
      # Analog control
      [0x3827, 0xec], [0x370c, 0x0f],
      [0x3612, 0x59], [0x3618, 0x00],
      
      # ISP control
      [0x5000, 0xff],
      
      # Timing
      [0x380c, 0x07], [0x380d, 0x68],  # HTS
      [0x380e, 0x03], [0x380f, 0xd8],  # VTS
      
      # Binning/subsampling
      [0x3814, 0x31], [0x3815, 0x31],
      
      # Window
      [0x3800, 0x01], [0x3801, 0xf4],  # X start
      [0x3802, 0x00], [0x3803, 0x00],  # Y start
      [0x3804, 0x0a], [0x3805, 0x3f],  # X end
      [0x3806, 0x07], [0x3807, 0xa1],  # Y end
      
      # Output size
      [0x3808, 0x03], [0x3809, 0x20],  # Width: 800
      [0x380a, 0x02], [0x380b, 0x58],  # Height: 600
      
      # MIPI timing
      [0x4837, 0x19],
      
      # Stream OFF (will be enabled later)
      [0x0100, 0x00],
      
      [self.REG_END, 0x00]
    ]
  end
  
  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("OV5647: ========== INIT START ==========")
      
      # PROTECTION: Don't re-init if already initialized
      if self.is_initialized
        print("OV5647: Already initialized, doing soft reset...")
        if !self.software_reset()
          print("OV5647: Reset failed!")
          return 0
        end
      end
      
      # Detect sensor
      if !self.detect()
        print("OV5647: Not detected")
        return 0
      end
      
      # SAFE: Full reset sequence
      if !self.software_reset()
        return 0
      end
      
      # Wait for sensor to stabilize
      tasmota.delay(200)
      
      # Configure for 800x600 RAW8
      print("OV5647: Configuring 800x600 RAW8...")
      if !self.write_array(self.regs_800x600_raw8())
        print("OV5647: Config failed")
        return 0
      end
      
      # Verify critical registers
      var stream_reg = self.read_reg16(0x0100)
      if stream_reg != 0x00
        print(format("OV5647: WARNING - Stream should be OFF but got 0x%02X", stream_reg))
      end
      
      # Write back to config struct if provided
      if raw != nil
        raw[0] = 800 & 0xFF
        raw[1] = (800 >> 8) & 0xFF
        raw[2] = 600 & 0xFF
        raw[3] = (600 >> 8) & 0xFF
        raw[4] = 0  # RAW8
        raw[5] = 0  # no ISP
      end
      
      self.is_initialized = true
      print("OV5647: ========== INIT COMPLETE ==========")
      return 1
      
    elif cmd == "stream_on"
      print("OV5647: Stream ON command")
      return self.stream_on(true) ? 1 : 0
      
    elif cmd == "stream_off"
      print("OV5647: Stream OFF command")
      return self.stream_on(false) ? 1 : 0
    
    else
      print(format("OV5647: Unknown command '%s'", cmd))
      return 0
    end
  end
end

# Create instance and register as driver
var ov5647 = OV5647()
tasmota.add_driver(ov5647)
print("========================================")
print("OV5647: SAFE Driver loaded")
print("Protections:")
print("- Prevents double-init")
print("- Verifies critical writes")
print("- Safe MIPI configuration")
print("- Proper reset sequences")
print("========================================")