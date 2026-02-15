# in preinit.be: load("/imx219.be")
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
  
  def write_array(reg_array)
    var i = 0
    var sz = size(reg_array)
    while i < sz
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
      end
      i += 1
    end
    return true
  end
end

class IMX219 : CSI_Sensor
  static ADDR = 0x10
  static CHIP_ID = 0x0219
  var is_streaming
  var is_initialized
  var width, height
  var mipi_clock
  var format, bin_mode
  
  def init()
    super(self).init("IMX219", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
    self.width = 640
    self.height = 480
    self.mipi_clock = 456
    self.format = 1 
    self.bin_mode = 2 
  end
  
  def detect()
    self.wire = tasmota.wire_scan(self.addr)
    if !self.wire 
      print("IMX219: I2C scan failed")
      return false 
    end
    tasmota.delay(10)
    var id_h = self.read_reg16(0x0000)
    if id_h == 0x02
       var id_l = self.read_reg16(0x0001)
       var chip_id = (id_h << 8) | id_l
       print(format("IMX219: Chip ID = 0x%04X", chip_id))
       return chip_id == self.CHIP_ID
    end
    return false
  end
  
  def stream_on(on)
    if !self.wire return false end
    if on
      if self.is_streaming return true end
      if !self.write_reg16(0x0100, 0x01) return false end
      self.is_streaming = true
      print("IMX219: Stream ON")
    else
      if !self.write_reg16(0x0100, 0x00) return false end
      self.is_streaming = false
      print("IMX219: Stream OFF")
    end
    return true
  end

  def common_regs()
    return [
      [0x0103, 0x01], [self.REG_DELAY, 10], [0x0100, 0x00],
      [0x30eb, 0x05], [0x30eb, 0x0c], [0x300a, 0xff], [0x300b, 0xff],
      [0x30eb, 0x05], [0x30eb, 0x09],
      [0x455e, 0x00], [0x471e, 0x4b], [0x4767, 0x0f], [0x4750, 0x14],
      [0x4540, 0x00], [0x47b4, 0x14], [0x4713, 0x30], [0x478b, 0x10],
      [0x478f, 0x10], [0x4793, 0x10], [0x4797, 0x0e], [0x479b, 0x0e],
      [0x0114, 0x01], [0x0128, 0x00], [0x012a, 0x18], [0x012b, 0x00],
      [0x0301, 0x05], [0x0303, 0x01], [0x0304, 0x03], [0x0305, 0x03],
      [0x0306, 0x00], [0x0307, 0x39], [0x030b, 0x01], 
      [0x030c, 0x00], [0x030d, 0x39], # 456 Mbps
      [self.REG_END, 0x00]
    ]
  end

  def regs_custom(x, y, w, h, bin, fps, fmt)
    self.width = w
    self.height = h
    self.bin_mode = bin
    self.format = fmt
    
    var reg_fmt = (fmt == 0) ? 0x08 : 0x0A      
    var reg_bin = (bin == 2) ? 0x01 : 0x00      
    var scale = (bin == 2) ? 2 : 1
    var eff_w = w * scale
    var eff_h = h * scale
    var native_w = 3280
    var native_h = 2464
    
    var sx = x
    var sy = y
    if x == 0 && y == 0
       sx = (native_w - eff_w) / 2
       sy = (native_h - eff_h) / 2
    end
    
    # 1. Force Even Alignment
    sx = (sx / 2) * 2
    sy = (sy / 2) * 2
    
    var ex = sx + eff_w - 1
    var ey = sy + eff_h - 1
    
    # 3. Timing Calculation (Calibrated PCLK = 182.4 MHz)
    var pclk = 182400000 
    var hts = 3448
    var vts = pclk / (hts * (fps==0?30:fps))
    if vts < h+50 vts = h+50 end
    
    # 4. Exposure & Gain Calculation
    var exposure = vts - 50  # Max integration time
    var gain = 0xE0          # MAX Analog Gain (Was 0xA0)
    
    var reg_oppxck = (fmt == 0) ? 0x08 : 0x0A

    print(format("IMX219: CFG %dx%d (Crop %d,%d) Bin=%d Fmt=%d FPS=%d", w, h, sx, sy, bin, fmt, fps))
    
    return [
      # Geometry & Format
      [0x0164, (sx>>8)], [0x0165, (sx&0xFF)], [0x0166, (ex>>8)], [0x0167, (ex&0xFF)],
      [0x0168, (sy>>8)], [0x0169, (sy&0xFF)], [0x016a, (ey>>8)], [0x016b, (ey&0xFF)],
      [0x016c, (w>>8)], [0x016d, (w&0xFF)],   
      [0x016e, (h>>8)], [0x016f, (h&0xFF)],
      
      # FIX COLOR SWAP (Red/Blue) by rotating readout
      [0x0172, 0x03], 
      
      [0x0174, reg_bin], [0x0175, reg_bin],
      [0x018c, reg_fmt], [0x018d, reg_fmt],
      [0x0309, reg_oppxck], 
      
      # Timing
      [0x0160, (vts>>8)], [0x0161, (vts&0xFF)],
      [0x0162, (hts>>8)], [0x0163, (hts&0xFF)],
      
      # Analog Controls (Group Hold Protected)
      [0x0104, 0x01],                 # Group Hold ON
      
      [0x0157, gain],                 # Analog Gain (Max)
      [0x0158, 0x01], [0x0159, 0x80], # Digital Gain (x1.5) to lift shadows - probably not perfect!!
      
      [0x015a, (exposure >> 8) & 0xFF], # Exposure High
      [0x015b, exposure & 0xFF],        # Exposure Low
      
      [0x0104, 0x00],                 # Group Hold OFF (Commit)
      
      [self.REG_END, 0x00]
    ]
  end

  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("IMX219: ========== INIT START ==========")
      
      if !self.is_initialized
        if !self.detect() return 0 end
        self.is_initialized = true
      end
      
      self.write_reg16(0x0100, 0x00); tasmota.delay(10)
      self.write_reg16(0x0100, 0x01); tasmota.delay(10)
      self.write_reg16(0x0100, 0x00); tasmota.delay(10)
      
      if !self.write_array(self.common_regs()) return 0 end
      
      var req_w=640; var req_h=480; var req_bin=2; var req_fps=30; var req_fmt=1
      var req_x=0; var req_y=0
      var res_idx = 0
      
      if idx != 0
        import introspect
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        res_idx = b[26]
        
        if res_idx == 255
           # MANUAL (wcwindow)
           req_w = b.get(0, 2); req_h = b.get(2, 2)
           req_fmt = b[8]; req_bin = b[16]; req_fps = b[17]
           req_x = b.get(12, 2); req_y = b.get(14, 2)
        elif res_idx == 0 
           # VGA: 640x480, Bin 2, RAW10 (0.3 MP)
           req_w=640; req_h=480; req_bin=2; req_fmt=1
        elif res_idx == 1 
           # 720p: 1280x720, Bin 1, RAW8 (0.9 MP)
           req_w=1280; req_h=720; req_bin=2; req_fmt=0 
        elif res_idx == 2 
           # Full Bin 2: 1640x1232, Bin 2, RAW8 (2.0 MP)
           req_w=1640; req_h=1232; req_bin=2; req_fmt=0
        elif res_idx == 3 
           # 1080p: 1920x1080, Bin 1, RAW8 (2.1 MP)
           req_w=1920; req_h=1080; req_bin=1; req_fmt=0
        elif res_idx == 4
           # Full Native: 3280x2464, Bin 1, RAW8 (8.0 MP)
           # Limit FPS to 10 for bandwidth/stability
           req_w=3280; req_h=2464; req_bin=1; req_fmt=0; req_fps=10
        end
      end
      
      var regs = self.regs_custom(req_x, req_y, req_w, req_h, req_bin, req_fps, req_fmt)
      if !self.write_array(regs) return 0 end
      
      if idx != 0
        import introspect
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        b.set(0, self.width, 2)
        b.set(2, self.height, 2)
        b.set(4, 3280, 2)
        b.set(6, 2464, 2)
        b[8] = self.format
        b[9] = 2 
        b.set(10, self.mipi_clock, 2)
        b[16] = self.bin_mode
        b[17] = req_fps       # CRITICAL for RTSP
        b.setbytes(18, bytes().fromstring("IMX219"))
      end
      
      print("IMX219: ========== INIT COMPLETE ==========")
      return 1
      
    elif cmd == "stream"
      return self.stream_on(idx == 1) ? 1 : 0
    end
    return 0
  end
end

var imx219 = IMX219()
tasmota.add_driver(imx219)
