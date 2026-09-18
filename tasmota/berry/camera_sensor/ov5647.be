# in preinit.be: load("/ov5647.be")
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

class OV5647 : CSI_Sensor
  static ADDR = 0x36
  static CHIP_ID = 0x5647
  var is_streaming
  var is_initialized
  var width, height
  var mipi_clock      # Mbps per lane (passed to C++ as CSI_Config.mipi_clock)
  var format, bin_mode
  
  def init()
    super(self).init("OV5647", self.ADDR)
    self.is_streaming = false
    self.is_initialized = false
    self.width = 640
    self.height = 480
    self.mipi_clock = 291
    self.format = 1
    self.bin_mode = 2
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
      return false 
    end
    
    var chip_id = (id_h << 8) | id_l
    print(format("OV5647: Chip ID = 0x%04X", chip_id))
    return chip_id == self.CHIP_ID
  end
  
  def stream_on(on)
    if !self.wire return false end
    if on
      if self.is_streaming return true end
      if !self.write_reg16(0x4800, 0x14) return false end # MIPI enable
      if !self.write_reg16(0x0100, 0x01) return false end # Stream enable
      self.is_streaming = true
      print("OV5647: Stream ON")
    else
      if !self.write_reg16(0x0100, 0x00) return false end
      self.is_streaming = false
      print("OV5647: Stream OFF")
    end
    return true
  end

  # Basic initialization and reset sequence
  def common_regs()
    return [
      [0x0100, 0x00],           # enable sleep
      [0x0103, 0x01],           # soft reset
      [self.REG_DELAY, 10],     # 10ms delay
      [0x4800, 0x01],           # clock lane gate
      [self.REG_END, 0x00]
    ]
  end

  # Dynamic register calculator
  def regs_custom(x, y, w, h, bin, fps, fmt)
    
    # 1. Align Geometry (CSI requires alignment)
    w = (w / 8) * 8
    if w < 16 w = 16 end
    if h < 16 h = 16 end
    if fps < 1 fps = 1 end

    self.width = w
    self.height = h
    self.format = fmt
    self.bin_mode = bin

    # 2. Dynamic PLL multiplier computation
    # OV5647 PLL: pclk = (EXCLK/prediv) * mult / sys_div / pclk_div
    # Both pclk and MIPI lane rate scale linearly with mult.
    # From calibration: pclk_per_mult = pclk / mult (constant per bin path)
    #   bin2: 90200000/216 = 417593 Hz/mult   (sys_div=4)
    #   bin1: 84000000/100 = 840000 Hz/mult   (sys_div=2)
    # MIPI clock per lane (Mbps) vs PLL multiplier:
    #   bin2: 291 Mbps/lane at mult=216 → ~1.347 Mbps/lane per mult unit
    #   bin1: 408 Mbps/lane at mult=100 → ~4.08 Mbps/lane per mult unit
    # Minimum mult from fps: mult >= HTS * VTS_min * fps / pclk_per_mult
    var vts_min = h + 50
    var pll_mult
    if bin == 2
      # pclk_per_mult ≈ 417593, use integer: mult = (HTS*vts*fps + 417592) / 417593
      pll_mult = (1896 * vts_min * fps + 417592) / 417593
      if pll_mult < 175 pll_mult = 175 end  # floor: matches RAW8 baseline
      if pll_mult > 252 pll_mult = 252 end
      if pll_mult >= 128 pll_mult = (pll_mult + 1) & 0xFE end  # even only above 127
      self.mipi_clock = (4 * pll_mult + 1) / 3  # ≈ 1.347 * mult
    else
      # pclk_per_mult = 840000, use integer: mult = (HTS*vts*fps + 839999) / 840000
      pll_mult = (2500 * vts_min * fps + 839999) / 840000
      if pll_mult < 80 pll_mult = 80 end   # floor: matches RAW8 baseline
      if pll_mult > 252 pll_mult = 252 end
      if pll_mult >= 128 pll_mult = (pll_mult + 1) & 0xFE end
      self.mipi_clock = 4 * pll_mult  # ≈ 4.08 * mult
    end

    print(format("OV5647: CFG %dx%d Bin=%d Fmt=%d FPS=%d PLL=0x%02X(%d) MIPI=%dMbps", w, h, bin, fmt, fps, pll_mult, pll_mult, self.mipi_clock))

    # 3. Generate Registers
    if bin == 2
       # ==========================================================
       # BIN 2: HIGH SPEED MODE
       # ==========================================================
       # Center crop calculation for Bin 2 (Max 1296x972)
       var max_w = 1296
       var max_h = 972
       var start_x = (max_w - w) / 2 + x
       var start_y = (max_h - h) / 2 + y
       # Offsets for ISP/Bayer alignment
       var off_x = 9 + start_x
       var off_y = 0 + start_y
       
       var hts = 1896
       var vts = pll_mult * 417593 / (hts * fps)
       if vts < h + 50 vts = h + 50 end

       return [
        [0x3034, fmt == 0 ? 0x18 : 0x1a],
        [0x3035,0x41], 
        [0x3036, pll_mult],
        [0x303c,0x11], 
        [0x3106,0xf5],
        
        [0x3814,0x31], [0x3815,0x31], [0x3820,0x41], [0x3821,0x03], 
        [0x3827,0xec], [0x370c,0x0f], [0x3612,0x59], [0x3618,0x00],
        [0x5000,0xff], [0x583e,0xf0], [0x583f,0x20], [0x5002,0x41], [0x5003,0x08], [0x5a00,0x08],
        [0x3000,0x00], [0x3001,0x00], [0x3002,0x00], [0x3016,0x08], [0x3017,0xe0], 
        [0x3018,0x44], [0x301c,0xf8], [0x301d,0xf0],
        [0x3a18,0x00], [0x3a19,0xf8], [0x3c01,0x80], [0x3c00,0x40], [0x3b07,0x0c],
        
        [0x380c,0x07], [0x380d,0x68], # HTS = 1896
        [0x380e, (vts >> 8) & 0xFF],  [0x380f, vts & 0xFF],
        
        [0x3800,0x00], [0x3801,0x00], [0x3802,0x00], [0x3803,0x00], 
        [0x3804,0x0a], [0x3805,0x3f], [0x3806,0x07], [0x3807,0xa1], 
        [0x3808, (w >> 8) & 0xFF], [0x3809, w & 0xFF],
        [0x380a, (h >> 8) & 0xFF], [0x380b, h & 0xFF],
        [0x3810, (off_x >> 8) & 0xFF], [0x3811, off_x & 0xFF],
        [0x3812, (off_y >> 8) & 0xFF], [0x3813, off_y & 0xFF],
        
        [0x3630,0x2e], [0x3632,0xe2], [0x3633,0x23], [0x3634,0x44], [0x3636,0x06], 
        [0x3620,0x64], [0x3621,0xe0], [0x3600,0x37],
        [0x3704,0xa0], [0x3703,0x5a], [0x3715,0x78], [0x3717,0x01], [0x3731,0x02], 
        [0x370b,0x60], [0x3705,0x1a],
        [0x3f05,0x02], [0x3f06,0x10], [0x3f01,0x0a],
        [0x3a08,0x01], [0x3a09,0x27], [0x3a0a,0x00], [0x3a0b,0xf6], [0x3a0d,0x04], 
        [0x3a0e,0x03], [0x3a0f,0x58], [0x3a10,0x50], [0x3a1b,0x58], [0x3a1e,0x50], 
        [0x3a11,0x60], [0x3a1f,0x28],
        [0x4001,0x02], [0x4004,0x02], [0x4000,0x09],
        [0x4837,0x28], [0x4050,0x6e], [0x4051,0x8f], 
        [self.REG_END,0x00]
       ]

    else
       # ==========================================================
       # BIN 1: FULL RESOLUTION / 1080p LOGIC
       # ==========================================================
       var max_w = 2592
       var max_h = 1944
       
       # Center Crop Logic
       var start_x = (max_w - w) / 2 + x
       var start_y = (max_h - h) / 2 + y
       
       # Sensor windowing requires alignment and offsets
       var win_x = start_x + 12 # Offset from driver legacy
       var win_y = start_y + 2
       
       # Boundaries
       var end_x = win_x + w + 8 - 1
       var end_y = win_y + h + 8 - 1
       if end_x > 2611 end_x = 2611 end # Safety cap
       if end_y > 1953 end_y = 1953 end
       
       # Timing
       var hts = 2500
       var vts = pll_mult * 840000 / (hts * fps)
       if vts < h + 50 vts = h + 50 end

       return [
         # --- SYSTEM & PLL ---
         [0x3034, fmt == 0 ? 0x18 : 0x1a], # MIPI 8-bit / 10-bit mode
         [0x3035, 0x21],                   # PLL System Control
         [0x3036, pll_mult],                 # PLL Multiplier (dynamic)
         [0x303c, 0x11],                   # PLL divider
         [0x3106, 0xf5],                   # SCLK/PCLK Control
         
         # --- TIMING & ISP OFFSET ---
         [0x3814, 0x11], [0x3815, 0x11],   # X/Y Inc (1 = No skip)
         [0x3820, 0x00], [0x3821, 0x02],   # Flip/Mirror (ISP)
         
         # --- GEOMETRY (Calculated above) ---
         [0x3800, (win_x >> 8) & 0xFF], [0x3801, win_x & 0xFF], # X Start
         [0x3802, (win_y >> 8) & 0xFF], [0x3803, win_y & 0xFF], # Y Start
         [0x3804, (end_x >> 8) & 0xFF], [0x3805, end_x & 0xFF], # X End
         [0x3806, (end_y >> 8) & 0xFF], [0x3807, end_y & 0xFF], # Y End
         [0x3808, (w >> 8) & 0xFF],     [0x3809, w & 0xFF],     # Output Width
         [0x380a, (h >> 8) & 0xFF],     [0x380b, h & 0xFF],     # Output Height
         
         # --- TIMING GENERATOR (HTS/VTS) ---
         [0x3810, 0x00], [0x3811, 0x05],   # X Offset (ISP)
         [0x3812, 0x00], [0x3813, 0x02],   # Y Offset (ISP)
         [0x380c, (hts >> 8) & 0xFF], [0x380d, hts & 0xFF], # HTS (Total Width)
         [0x380e, (vts >> 8) & 0xFF], [0x380f, vts & 0xFF], # VTS (Total Height -> FPS)

         # --- ANALOG & SENSOR CORE ---
         [0x3708, 0x64], [0x3709, 0x12],   # Analog Control
         [0x3827, 0xec], [0x370c, 0x03], 
         [0x3612, 0x5b], [0x3618, 0x04],   # Analog Crop/Drive
         
         [0x3630, 0x2e], [0x3632, 0xe2], [0x3633, 0x23], [0x3634, 0x44], [0x3636, 0x06], # Analog Row/Col
         [0x3620, 0x64], [0x3621, 0xe0], [0x3600, 0x37], # Analog Bandgap
         [0x3704, 0xa0], [0x3703, 0x5a], [0x3715, 0x78], [0x3717, 0x01], [0x3731, 0x02], # Readout Control
         [0x370b, 0x60], [0x3705, 0x1a],
         
         # --- LENS SHADING & GAMMA ---
         [0x5000, 0xff], [0x583e, 0xf0], [0x583f, 0x4f], # LENC (Lens Correction)
         [0x5003, 0x08], [0x5a00, 0x08],                 # Gamma enable?

         # --- AEC/AGC (EXPOSURE CONTROL) - TUNED FOR BIN 1 ---
         # Target: Lower brightness to reduce noise
         [0x3a0f, 0x30], [0x3a10, 0x28],   # AEC Stable Range (Ceiling/Floor) - WAS 0x58/0x50
         [0x3a1b, 0x30], [0x3a1e, 0x26],   # AEC Instability Range - WAS 0x58/0x50
         
         # Gain Ceilings (Limit Noise Boost)
         [0x3a18, 0xff], [0x3a19, 0x00],   # is s10 bit, with max 0x3ff
         
         # AEC Algorithm Tuning
         [0x3a08, 0x01], [0x3a09, 0x4b],   # B50/B60 step
         [0x3a0a, 0x01], [0x3a0b, 0x13],   # Exposure limits?
         # [0x3a0d, 0x04], [0x3a0e, 0x03],   # Maximum Vertical Total Size limits for the AEC algorithm
         
         # --- AWB (WHITE BALANCE) ---
         [0x3000, 0x00], [0x3001, 0x00], [0x3002, 0x00], # System Control
         [0x3016, 0x08], [0x3017, 0xe0],   # Drive strength
         [0x3018, 0x44], [0x301c, 0xf8], [0x301d, 0xf0],
         [0x3c00, 0x40], [0x3b07, 0x0c],   # AWB Control

         # --- STATISTICS ---
         [0x3a11, 0x60], [0x3a1f, 0x28],   # Stats Window?
         [0x4001, 0x02], [0x4004, 0x04], [0x4000, 0x09], # BLC (Black Level)
         [0x4837, 0x19], [0x4800, 0x34],   # MIPI Control
         
         [self.REG_END, 0x00]
       ]
    end
  end

  def camera(cmd, idx, payload, raw)
    if cmd == "init"
      print("OV5647: ========== INIT ==========")
      if !self.is_initialized
        if !self.detect() return 0 end
        self.is_initialized = true
      end
      
      # Reset and Apply Common Registers
      if !self.write_array(self.common_regs()) return 0 end
      
      var req_w=640; var req_h=480; var req_bin=2; var req_fps=30; var req_fmt=1
      var req_x=0; var req_y=0
      var res_idx = 0
      
      if idx != 0
        import introspect
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        res_idx = b[26]
        
        if res_idx == 255 # Manual
           req_w = b.get(0, 2); req_h = b.get(2, 2)
           req_fmt = b[8]; req_bin = b[16]; req_fps = b[17]
           req_x = b.get(12, 2); req_y = b.get(14, 2)
        elif res_idx == 0 # 640x480 (Bin 2)
           req_w=640; req_h=480; req_bin=2; req_fmt=1
        elif res_idx == 1 # 720p (Bin 2)
           req_w=1280; req_h=720; req_bin=2; req_fmt=1
        elif res_idx == 2 # Full Bin 2 (1296x972)
           req_w=1296; req_h=972; req_bin=2; req_fmt=1
        elif res_idx == 3 # 1080p (Bin 1)
           req_w=1920; req_h=1080; req_bin=1; req_fmt=1
        elif res_idx == 4 # Full Bin 1 (2592x1944)
           req_w=2592; req_h=1944; req_bin=1; req_fmt=1; req_fps=15
        end
      end
      
      # Calculate and Write Registers
      var regs = self.regs_custom(req_x, req_y, req_w, req_h, req_bin, req_fps, req_fmt)
      if !self.write_array(regs) return 0 end
      
      # Update C++ Structure
      if idx != 0
        import introspect
        var p = introspect.toptr(idx)
        var b = bytes(p, 28)
        b.set(0, self.width, 2)
        b.set(2, self.height, 2)
        b.set(4, 2592, 2)
        b.set(6, 1944, 2)
        b[8] = self.format
        b[9] = 2
        b.set(10, self.mipi_clock, 2)
        b[16] = self.bin_mode
        b[17] = req_fps
        b.setbytes(18, bytes().fromstring("OV5647"))
      end
      
      return 1
      
    elif cmd == "stream"
      return self.stream_on(idx == 1) ? 1 : 0
    end
    return 0
  end
end

var ov5647 = OV5647()
tasmota.add_driver(ov5647)
