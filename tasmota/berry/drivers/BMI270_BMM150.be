############################################################################################
# I2C driver written in Berry
#
# Support for BMI270 6-axis IMU (accelerometer + gyroscope)
# Found on M5Stack AtomS3R, AtomS3R-Cam, Atom-Matrix v1.1, etc.
#
# Also supports an auxiliary BMM150 magnetometer connected to the BMI270's
# AUX I2C bus (e.g. AtomS3R / AtomS3R-CAM). When detected, the BMI270
# auto-reads BMM150 samples into its own AUX_X_LSB buffer, so from the
# host side the magnetometer is just another register read.
#
# Ported from:
#   https://github.com/m5stack/M5Unified/blob/master/src/utility/imu/BMI270_Class.cpp
#   https://github.com/m5stack/M5Unified/blob/master/src/utility/imu/BMI270_Class.hpp
#   https://github.com/m5stack/M5Unified/blob/master/src/utility/imu/BMI270_config.inl
#
# Register map (inlined below):
#   0x00 CHIP_ID           - must read 0x24
#   0x03 STATUS            - bit 2 = AUX transfer in progress
#   0x04 AUX_X_LSB         - auto-refreshed magnetometer data (8 bytes, LE int16)
#   0x0C ACC_X_LSB         - accel data (6 bytes, little-endian int16)
#   0x12 GYR_X_LSB         - gyro data (6 bytes, little-endian int16)
#   0x21 INTERNAL_STATUS   - low nibble = 1 when init succeeded
#   0x40 ACC_CONF          - accel output data rate, bandwidth, perf mode
#   0x42 GYR_CONF          - gyro output data rate, bandwidth, perf mode
#   0x4B AUX_DEV_ID        - 7-bit I2C address of aux slave shifted left by 1
#   0x4C AUX_IF_CONF       - aux interface: manual/auto, burst length
#   0x4D AUX_RD_ADDR       - aux register to read (triggers transaction)
#   0x4E AUX_WR_ADDR       - aux register to write (triggers transaction)
#   0x4F AUX_WR_DATA       - data byte to write through the aux bus
#   0x58 INT_MAP_DATA      - map data-ready interrupts to INT pins
#   0x59 INIT_CTRL         - 0 before upload, 1 to trigger init
#   0x5B INIT_ADDR_0/1     - word-address for config upload
#   0x5E INIT_DATA         - config-file burst write destination
#   0x6B IF_CONF           - primary interface config (bit 5 = AUX enable)
#   0x7C PWR_CONF          - power save / fast-start options
#   0x7D PWR_CTRL          - enable accel/gyro/temp/aux
#   0x7E CMD               - command register (0xB6 = soft reset)
############################################################################################

class BMI270_BMM150 : I2C_Driver
  var accel, gyro, mag         # last readings (mag is nil when no BMM150 found)
  var ares, gres, mres         # scaling factors (g/LSB, dps/LSB, uT/LSB)

  def init(cfg)
    super(self).init("BMI270", 0x68, 58)
    if !self.wire return end

    # check CHIP_ID (must be 0x24 for BMI270)
    if self.read8(0x00) != 0x24
      self.wire = nil
      return
    end

    # software reset
    self.write8(0x7E, 0xB6)                 # CMD = SOFT_RESET
    tasmota.delay(5)

    # disable power-save so config upload works
    self.write8(0x7C, 0x00)                 # PWR_CONF = 0
    tasmota.delay(1)

    # prepare for init: INIT_CTRL = 0
    self.write8(0x59, 0x00)

    # upload the config file in chunks
    self._upload_config(cfg)

    # start initialization
    self.write8(0x59, 0x01)                 # INIT_CTRL = 1

    # wait for internal status to report "init ok" (low nibble == 0x01)
    var retry = 20
    var status = 0
    while (status & 0x0F) != 0x01 && retry > 0
      tasmota.delay(5)
      status = self.read8(0x21)             # INTERNAL_STATUS
      retry -= 1
    end
    if (status & 0x0F) != 0x01
      log(f"I2C: BMI270 init failed, internal_status={status}", 2)
      self.wire = nil
      return
    end

    # map all data interrupts (mirrors the reference driver)
    self.write8(0x58, 0xFF)                 # INT_MAP_DATA

    # accel config: ODR 100Hz, normal filter, performance mode
    # 0xA8 = acc_filter_perf(0x80) | acc_bwp=normal(0x20) | acc_odr=100Hz(0x08)
    self.write8(0x40, 0xA8)                 # ACC_CONF
    # accel range stays at default 0x02 => +/- 8g (register 0x41)

    # gyro config: ODR 200Hz, normal filter, perf + noise modes
    # 0xA9 = gyr_filter_perf(0x80) | gyr_noise_perf(0x40) | gyr_bwp=normal(0x20) | gyr_odr=200Hz(0x09)
    self.write8(0x42, 0xA9)                 # GYR_CONF
    # gyro range stays at default 0x00 => +/- 2000dps (register 0x43)

    # try to set up an auxiliary BMM150 magnetometer on the BMI270's AUX bus
    # On success, BMI270 auto-reads BMM150 samples into AUX_X_LSB (0x04)
    # and PWR_CTRL needs bit 0 (aux_en) set in addition to accel/gyro/temp.
    var pwr_ctrl = 0x0E                      # temp | gyr | acc
    if self._aux_setup_bmm150()
      pwr_ctrl = 0x0F                        # + aux
      self.mres = 10.0 * 4912.0 / 32760.0    # uT per LSB (BMM150 constant)
      self.mag  = [0.0, 0.0, 0.0]
      log("I2C: BMM150 detected on aux bus of " + self.name, 2)
    end

    # enable accel + gyro + temperature (+ aux if BMM150 present)
    self.write8(0x7D, pwr_ctrl)             # PWR_CTRL

    tasmota.delay(5)                        # let sensors produce first samples

    # default ranges are +/-8g and +/-2000dps
    self.ares = 8.0    / 32768.0            # g per LSB
    self.gres = 2000.0 / 32768.0            # dps per LSB
    self.accel = [0.0, 0.0, 0.0]
    self.gyro  = [0, 0, 0]

    tasmota.add_driver(self)
  end

  # Upload the 8192-byte configuration blob to the BMI270
  # in small chunks to stay within the I2C buffer (128 bytes on ESP32).
  # INIT_ADDR_0/1 carries a 2-byte word-address (byte index / 2):
  #   INIT_ADDR_0 = low 4 bits of word-address
  #   INIT_ADDR_1 = word-address >> 4
  def _upload_config(cfg)
    var total = cfg.size()
    var chunk = 32
    var idx = 0
    while idx < total
      var end_off = idx + chunk
      if end_off > total  end_off = total  end
      var word_addr = idx >> 1
      var ab = bytes()
      ab.add(word_addr & 0x0F, 1)           # INIT_ADDR_0
      ab.add((word_addr >> 4) & 0xFF, 1)    # INIT_ADDR_1
      self.wire.write_bytes(self.addr, 0x5B, ab)
      self.wire.write_bytes(self.addr, 0x5E, cfg[idx .. end_off - 1])
      idx = end_off
    end
  end

  # --- AUX (BMM150) helpers ---
  # The BMI270 has an internal I2C master that can read/write an auxiliary
  # sensor (BMM150 magnetometer at 0x10 on AtomS3R). After set-up the
  # BMI270 auto-refreshes AUX_X_LSB (0x04) with BMM150 samples.

  # Put the AUX bus in manual (single-transaction) mode and point it at
  # the given 7-bit aux slave address.
  def _aux_manual_mode(aux_addr)
    self.write8(0x6B, 0x20)                 # IF_CONF: aux enable
    self.write8(0x7C, 0x00)                 # PWR_CONF: no power save
    self.write8(0x7D, 0x0E)                 # PWR_CTRL: temp|gyr|acc (aux off)
    self.write8(0x4C, 0x80)                 # AUX_IF_CONF: manual mode, burst 1
    self.write8(0x4B, aux_addr << 1)        # AUX_DEV_ID (7bit addr shifted left)
  end

  # Write a byte to an aux-slave register (blocking, returns true on success)
  def _aux_write8(reg, val)
    self.write8(0x4F, val)                  # AUX_WR_DATA
    self.write8(0x4E, reg)                  # AUX_WR_ADDR (triggers the transaction)
    var retry = 5
    while (self.read8(0x03) & 0x04) && retry > 0  # STATUS bit 2 = aux busy
      tasmota.delay(1)
      retry -= 1
    end
    return retry > 0
  end

  # Read a byte from an aux-slave register (blocking)
  def _aux_read8(reg)
    self.write8(0x4C, 0x80)                 # AUX_IF_CONF: manual, burst 1
    self.write8(0x4D, reg)                  # AUX_RD_ADDR (triggers the transaction)
    var retry = 5
    while (self.read8(0x03) & 0x04) && retry > 0
      tasmota.delay(1)
      retry -= 1
    end
    return self.read8(0x04)                 # AUX_X_LSB holds the result
  end

  # Probe + set up the BMM150 magnetometer on the AUX bus.
  # Returns true if found and configured for auto-read.
  def _aux_setup_bmm150()
    self._aux_manual_mode(0x10)              # BMM150 I2C address
    self._aux_write8(0x4B, 0x83)             # BMM150 power control: soft reset + power on
    tasmota.delay(3)
    self._aux_read8(0x40)                    # discard first readback
    var who = self._aux_read8(0x40)          # BMM150 CHIP_ID register
    if who != 0x32  return false  end
    self._aux_write8(0x4C, 0x38)             # BMM150 OP_CTRL: normal mode, ODR 30Hz
    # switch back to auto-read burst-8 mode targeting BMM150 data register
    self.write8(0x4C, 0x4F)                  # AUX_IF_CONF: auto, burst 8
    self.write8(0x4D, 0x42)                  # AUX_RD_ADDR: BMM150 data_x_lsb
    return true
  end

  # [ax, ay, az] as floats in g (range +/-8g)
  # BMI270 data is little-endian signed 16-bit
  def read_accel()
    if !self.wire return nil end
    var b = self.wire.read_bytes(self.addr, 0x0C, 6)    # ACC_X_LSB
    var ax = b.geti(0, 2)                               # signed LE
    var ay = b.geti(2, 2)
    var az = b.geti(4, 2)
    self.accel = [ax * self.ares, ay * self.ares, az * self.ares]
    return self.accel
  end

  # [gx, gy, gz] as ints in dps (range +/-2000dps)
  def read_gyro()
    if !self.wire return nil end
    var b = self.wire.read_bytes(self.addr, 0x12, 6)    # GYR_X_LSB
    var gx = b.geti(0, 2)                               # signed LE
    var gy = b.geti(2, 2)
    var gz = b.geti(4, 2)
    self.gyro = [int(gx * self.gres), int(gy * self.gres), int(gz * self.gres)]
    return self.gyro
  end

  # [mx, my, mz] as floats in uT (BMM150 via AUX, or nil if not present)
  # BMM150 packs 13-bit X/Y in bits [15:3] and 15-bit Z in bits [15:1].
  # The BMI270 presents this layout verbatim in AUX_X_LSB..AUX_Z_MSB.
  def read_mag()
    if !self.wire || self.mag == nil  return nil  end
    var b = self.wire.read_bytes(self.addr, 0x04, 6)    # AUX_X_LSB..Z_MSB
    var mx = b.geti(0, 2) >> 3                          # X: signed LE, drop 3 LSB
    var my = b.geti(2, 2) >> 3                          # Y: signed LE, drop 3 LSB
    var mz = b.geti(4, 2) >> 1                          # Z: signed LE, drop 1 LSB
    self.mag = [mx * self.mres, my * self.mres, mz * self.mres]
    return self.mag
  end

  # Triggered every second by Tasmota
  def every_second()
    if !self.wire return nil end
    self.read_accel()
    self.read_gyro()
    if self.mag != nil  self.read_mag()  end
  end

  # Display in Tasmota web UI
  def web_sensor()
    if !self.wire return nil end
    var msg = format(
              "{s}BMI270 acc_x{m}%.3f G{e}"
              "{s}BMI270 acc_y{m}%.3f G{e}"
              "{s}BMI270 acc_z{m}%.3f G{e}"
              "{s}BMI270 gyr_x{m}%i dps{e}"
              "{s}BMI270 gyr_y{m}%i dps{e}"
              "{s}BMI270 gyr_z{m}%i dps{e}",
              self.accel[0], self.accel[1], self.accel[2],
              self.gyro[0],  self.gyro[1],  self.gyro[2])
    if self.mag != nil
      msg += format(
              "{s}BMM150 mag_x{m}%.1f uT{e}"
              "{s}BMM150 mag_y{m}%.1f uT{e}"
              "{s}BMM150 mag_z{m}%.1f uT{e}",
              self.mag[0], self.mag[1], self.mag[2])
    end
    tasmota.web_send_decimal(msg)
  end

  # Append values to teleperiod JSON
  def json_append()
    if !self.wire return nil end
    var ax = int(self.accel[0] * 1000)
    var ay = int(self.accel[1] * 1000)
    var az = int(self.accel[2] * 1000)
    var msg = format(
              ",\"BMI270\":{\"AX\":%i,\"AY\":%i,\"AZ\":%i,\"GX\":%i,\"GY\":%i,\"GZ\":%i}",
              ax, ay, az, self.gyro[0], self.gyro[1], self.gyro[2])
    tasmota.response_append(msg)
    if self.mag != nil
      tasmota.response_append(format(
              ",\"BMM150\":{\"MX\":%.1f,\"MY\":%.1f,\"MZ\":%.1f}",
              self.mag[0], self.mag[1], self.mag[2]))
    end
  end
end

# --- BMI270 configuration blob (8192 bytes, base64 encoded) ---
# Source: https://github.com/m5stack/M5Unified/blob/master/src/utility/imu/BMI270_config.inl
# Uploaded verbatim to the sensor's internal RAM during init.
# Moving the blob outside compiled code so that it's garbage collected after initialization
return BMI270_BMM150(bytes().fromb64(
    "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"
  ))
