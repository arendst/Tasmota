# OV5647 CSI Camera Driver for Tasmota ESP32-P4
# Version 2 - Fixed for Tasmota driver integration

# Base Sensor Class
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
    self.wire._write((reg >> 8) & 0xFF)  # Fixed: use _write
    self.wire._write(reg & 0xFF)         # Fixed: use _write
    self.wire._write(val)                # Fixed: use _write
    return self.wire._end_transmission() == 0
  end
  
  def read_reg16(reg)
    if !self.wire return nil end
    self.wire._begin_transmission(self.addr)
    self.wire._write((reg >> 8) & 0xFF)  # Fixed: use _write
    self.wire._write(reg & 0xFF)         # Fixed: use _write
    var err = self.wire._end_transmission(false)
    if err != 0 return nil end
    self.wire._request_from(self.addr, 1)  # Fixed: use _request_from
    if self.wire._available()              # Fixed: use _available
      return self.wire._read()             # Fixed: use _read
    end
    return nil
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
          print(format("%s: Failed reg 0x%04X", self.name, reg))
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

# OV5647 5MP Camera Sensor
class OV5647 : CSI_Sensor
  static ADDR = 0x36
  static CHIP_ID = 0x5647
  
  def init()
    super(self).init("OV5647", self.ADDR)
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
  
  def stream_on(on)
    if !self.wire return false end
    if on
      self.write_reg16(0x4800, 0x00)
      self.write_reg16(0x0100, 0x01)
      print("OV5647: Stream ON")
    else
      self.write_reg16(0x0100, 0x00)
      print("OV5647: Stream OFF")
    end
    return true
  end
  
  def regs_800x600_raw8()
    return [
      [0x3034, 0x18], [0x3035, 0x41], [0x3036, 0x80],
      [0x303c, 0x11], [0x3106, 0xf5], [0x3821, 0x03],
      [0x3820, 0x41], [0x3827, 0xec], [0x370c, 0x0f],
      [0x3612, 0x59], [0x3618, 0x00], [0x5000, 0xff],
      [0x380c, 0x07], [0x380d, 0x68],
      [0x380e, 0x03], [0x380f, 0xd8],
      [0x3814, 0x31], [0x3815, 0x31],
      [0x3800, 0x01], [0x3801, 0xf4],
      [0x3802, 0x00], [0x3803, 0x00],
      [0x3804, 0x0a], [0x3805, 0x3f],
      [0x3806, 0x07], [0x3807, 0xa1],
      [0x3808, 0x03], [0x3809, 0x20],
      [0x380a, 0x02], [0x380b, 0x58],
      [0x4837, 0x19], [0x0100, 0x00],
      [self.REG_END, 0x00]
    ]
  end
  
  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("OV5647: Init called")
      if raw != nil
        print(format("OV5647: Config ptr = %s", str(raw)))
      end
      
      # Detect sensor
      if !self.detect()
        print("OV5647: Not detected")
        return 0
      end
      print("OV5647: Detected successfully")
      
      # MIPI reset sequence
      print("OV5647: Applying MIPI reset...")
      self.write_array([
        [0x0100, 0x00], [0x0103, 0x01],
        [self.REG_DELAY, 10], [0x4800, 0x01],
        [self.REG_END, 0x00]
      ])
      tasmota.delay(10)
      
      # Configure for 800x600 RAW8
      print("OV5647: Configuring 800x600 RAW8...")
      if !self.write_array(self.regs_800x600_raw8())
        print("OV5647: Config failed")
        return 0
      end
      
      # Write back to config struct if provided
      if raw != nil
        # Assuming raw is a bytes() object with the config structure
        # Format (example): [width_lo, width_hi, height_lo, height_hi, format, has_isp, ...]
        # You may need to adjust based on actual C struct layout
        raw[0] = 800 & 0xFF         # width low byte
        raw[1] = (800 >> 8) & 0xFF  # width high byte
        raw[2] = 600 & 0xFF         # height low byte
        raw[3] = (600 >> 8) & 0xFF  # height high byte
        raw[4] = 0                  # format = RAW8
        raw[5] = 0                  # has_isp = false
        # lane_bitrate (400 Mbps) at bytes 6-9 if needed
        # lane_num (2) at byte 10 if needed
      end
      
      print("OV5647: Init complete")
      self.stream_on(true)
      return 1
      
    elif cmd == "stream_on"
      print(format("OV5647: Stream command, payload=%s", str(payload)))
      return self.stream_on(payload) ? 1 : 0
      
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
print("OV5647: Driver registered with Tasmota")