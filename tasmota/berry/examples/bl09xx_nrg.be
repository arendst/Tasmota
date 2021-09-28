# Sample Energy Driver for BL0939/BL0940
# Native driver is xnrg_14_bl09xx.ino
# Requires build with USE_BERRY_ENERGY_DRIVER and GPIO defined as Option_A6
# This demonstrates how a energy serial based driver can be created.

import string

class BL09XX : Driver
  var model         # 939 or 940
  var addr          # sub address on the serial bus
  var expected      # 35 bytes for full frame
  var ser           # the serial object
  var rx_buffer     # reception buffer
  var valid         # 0 is not valid, 1 if valid

  var energy_voltage_calibration    # for now, calibration is here as no access to Settings
  var energy_power_calibration
  var energy_current_calibration

  var voltage, current, power, cf_pulses, tps1  # store raw data from sensor
  var temperature

  # -----------------------------------------------------------------------------------
  # calc checksum of a message
  def calc_checksum(msg,len,init_sum)
    var sum
    sum = init_sum
    for i: 0..len-1
      sum += msg[i]
    end
    return 0xFF & (0xFF ^ sum) # some Berry bug on ~ operator
  end

  # -----------------------------------------------------------------------------------
  # send a BL09xx write command
  def write_reg(reg,value)
    var msg
    msg = bytes('')
    msg.add(0xA0 + self.addr)
    msg.add(reg)
    msg.add((value>>16) & 0xFF)
    msg.add((value>> 8) & 0xFF)
    msg.add((value    ) & 0xFF)
    msg.add(self.calc_checksum(msg, 5, 0))
    self.ser.write(msg)
  end

  # -----------------------------------------------------------------------------------
  # send a BL09xx read command
  def read_reg(reg)
    var msg
    msg = bytes('')
    msg.add(0x50 + self.addr)
    msg.add(reg)
    self.ser.write(msg)
  end

  # -----------------------------------------------------------------------------------
  # initialize the registers
  def chip_init()
    # send initialisation sequence to BL09xx
    self.write_reg(0x19, 0x5A5A5A)
    self.write_reg(0x1A, 0x550000)
    self.write_reg(0x18, 0x001000)
    self.write_reg(0x1B, 0xFF4700)
    self.write_reg(0x10, 0x1C1800)
  end

  # -----------------------------------------------------------------------------------
  # init the driver
  def init(model, tx, rx, addr)
    self.model = model
    self.addr = addr
    self.expected = 35
    self.voltage = 0
    self.current = [0, 0]
    self.power = [0, 0]
    self.cf_pulses = [-1, -1]
    self.tps1 = 0
    self.valid = 0
    self.rx_buffer = bytes('')

    # default calibration
    self.energy_voltage_calibration = 17159.0  # must be float
    self.energy_power_calibration   = 713.0    # must be float
    self.energy_current_calibration = 266013.0 # must be float

    # setup tasmota Energy environment
    energy.voltage_common = 1
    energy.frequency_common = 1
    energy.use_overtemp = 1
    energy.phase_count = (model==939) ? 2 : 1
    energy.use_overtemp = 1

    self.ser = serial(tx, rx, 4800, serial.SERIAL_8N1)
    self.ser.flush()
    self.chip_init()
    log("BL9: BL09xx initialized", 3)
  end

  # -----------------------------------------------------------------------------------
  # convert a 24 binary data into a signed value
  def signed24(val)
    if val & 0x800000
      val = -(val^0xFFFFFF)-1
    end
    return val
  end

  # -----------------------------------------------------------------------------------
  # decode BL09xx data frame
  def decode_buffer()
    var expected_crc
    expected_crc = self.calc_checksum(self.rx_buffer, 34, 0x50 + self.addr)
    if self.rx_buffer[0] != 0x55 || self.rx_buffer[34] != expected_crc
      log(string.format("BL9: Received frame error hd=%02X, b34=%02X, crc=%02X", self.rx_buffer[0], self.rx_buffer[34], expected_crc), 3)
      return false
    end

    self.tps1 = self.rx_buffer[29] << 8 | self.rx_buffer[28]
    self.temperature = ((170.0/448.0)*((self.tps1/2.0)-32.0))-45.0;

    self.voltage = self.rx_buffer[12] << 16 | self.rx_buffer[11] << 8 | self.rx_buffer[10];         # V_RMS unsigned

    var tmp
    self.current[0] = self.rx_buffer[6] << 16 | self.rx_buffer[5] << 8 | self.rx_buffer[4];         # IA_RMS unsigned
    tmp = self.signed24(self.rx_buffer[18] << 16 | self.rx_buffer[17] << 8 | self.rx_buffer[16])    # WATT_A signed
    self.power[0] = tmp                                                                             # WATT_A unsigned
    tmp = self.signed24(self.rx_buffer[24] << 16 | self.rx_buffer[23] << 8 | self.rx_buffer[22])    # CFA_CNT signed
    self.cf_pulses[0] = tmp                                                                         # CFA_CNT unsigned

    if energy.phase_count > 1
      self.current[1] = self.rx_buffer[9] << 16 | self.rx_buffer[8]  << 8 | self.rx_buffer[7]       # IB_RMS unsigned
      tmp = self.signed24(self.rx_buffer[21] << 16 | self.rx_buffer[20] << 8 | self.rx_buffer[19])  # WATT_B signed
      self.power[1] = tmp                                                                           # WATT_B unsigned
      tmp = self.signed24(self.rx_buffer[27] << 16 | self.rx_buffer[26] << 8 | self.rx_buffer[25])  # CFB_CNT signed
      self.cf_pulses[1] = tmp                                                                       # CFB_CNT unsigned
    end

    log(string.format("BL9: U %d, I %d/%d, P %d/%d, C %d/%d, T %d", self.voltage, self.current[0], self.current[1],
      self.power[0], self.power[1], self.cf_pulses[0], self.cf_pulses[1], self.tps1), 4)

    return true
  end

  # -----------------------------------------------------------------------------------
  # update Tasmota Energy from raw data
  def update_energy()
    if true #energy.power_on # Powered on
      energy.voltage = real(self.voltage) / self.energy_voltage_calibration
      log(string.format("BL9: Voltage %f, Temp %f", energy.voltage, self.temperature), 4)

      # channel 0
      if self.power[0] > self.energy_power_calibration
        energy.active_power = real(self.power[0]) / self.energy_power_calibration
        energy.current = real(self.current[0]) / self.energy_current_calibration
        log(string.format("BL9: Chan[0] I %f, P %f", energy.current, energy.active_power), 4)
      else
        energy.active_power = 0.0
        energy.current = 0.0
        log(string.format("BL9: Chan[0] I zero, P zero"))
      end

      # channel 1
      if (energy.phase_count > 1) && (self.power[1] > self.energy_power_calibration)
        energy.active_power_2 = real(self.power[1]) / self.energy_power_calibration
        energy.current_2 = real(self.current[1]) / self.energy_current_calibration
        log(string.format("BL9: Chan[1] I %f, P %f", energy.current_2, energy.active_power_2), 4)
      else
        energy.active_power_2 = 0.0
        energy.current_2 = 0.0
        log(string.format("BL9: Chan[1] I zero, P zero"))
      end
    else # Powered off
      energy.voltage = 0.0
      energy.active_power = 0.0
      energy.active_power_2 = 0.0
      energy.current = 0.0
      energy.current_2 = 0.0
    end
  end

  # -----------------------------------------------------------------------------------
  # Every 100ms: check if serial data is available
  def every_100ms()
    var len, avail, done
    avail = self.ser.available()
    if avail > 0
      self.rx_buffer += self.ser.read()
      len = size(self.rx_buffer)
      if len >= self.expected
        #log(string.format("avail:%d", len), 4)
        done = self.decode_buffer()
        if done == true
          self.update_energy()
          self.valid = 1
          self.rx_buffer = self.rx_buffer[self.expected..len-1]
        else
          self.valid = 0
          self.ser.flush()
          self.rx_buffer = bytes('')
        end
      end
    end
  end

  # -----------------------------------------------------------------------------------
  # Every second : send full frame request
  def every_second()
    if energy.data_valid > 4
      self.voltage = 0;
      self.current = [0,0]
      self.power = [0,0]
    end

    self.read_reg(0xAA) # read full frame
  end

  # -----------------------------------------------------------------------------------
  def web_sensor()
    tasmota.web_send_decimal(string.format("{s}BL09XX Temperature{m}%.1f°C{e}", self.temperature))
  end

  # -----------------------------------------------------------------------------------
  def json_append()
    tasmota.response_append(string.format(",\"BL09XX\":{\"Temperature\":%.1f}", self.temperature))
  end
end

# Sonoff Dual R3 v2 uses BL0939
# !! Remove "Serial TX" and "BL039 RX" to user
#   model  = 939
#   rx pin = 26
#   tx pin = 25
#   addr   = 5
BerryEnergy = BL09XX(939, 26, 25, 5)

tasmota.add_driver(BerryEnergy)
