#-
 - Example of I2C driver written in Berry
 -
 - Support for MPU6886 device found in M5Stack
 - Alternative to xsns_85_mpu6886.ino 
 -#

class MPU6886 : Driver
  var enabled, wire
  var gres, ares
  var accel, gyro

  def init()
    self.enabled = false
    if tasmota.i2c_enabled(58) || 1
      var bus = 1
      if wire1.detect(0x68)
        self.wire = wire1
      elif wire2.detect(0x68)
        self.wire = wire2
        bus = 2
      end

      if self.wire
        var v = self.wire.read(0x68,0x75,1)
        if v != 0x19 return end  #- wrong device -#

        self.wire.write(0x68, 0x6B, 0, 1)
        tasmota.delay(10)
        self.wire.write(0x68, 0x6B, 1<<7, 1)    # MPU6886_PWR_MGMT_1
        tasmota.delay(10)
        self.wire.write(0x68, 0x6B, 1<<0, 1)    # MPU6886_PWR_MGMT_1
        tasmota.delay(10)
        self.wire.write(0x68, 0x1C, 0x10, 1)    # MPU6886_ACCEL_CONFIG - AFS_8G
        tasmota.delay(1)
        self.wire.write(0x68, 0x1B, 0x18, 1)    # MPU6886_GYRO_CONFIG - GFS_2000DPS
        tasmota.delay(1)
        self.wire.write(0x68, 0x1A, 0x01, 1)    # MPU6886_CONFIG
        tasmota.delay(1)
        self.wire.write(0x68, 0x19, 0x05, 1)    # MPU6886_SMPLRT_DIV
        tasmota.delay(1)
        self.wire.write(0x68, 0x38, 0x00, 1)    # MPU6886_INT_ENABLE
        tasmota.delay(1)
        self.wire.write(0x68, 0x1D, 0x00, 1)    # MPU6886_ACCEL_CONFIG2
        tasmota.delay(1)
        self.wire.write(0x68, 0x6A, 0x00, 1)    # MPU6886_USER_CTRL
        tasmota.delay(1)
        self.wire.write(0x68, 0x23, 0x00, 1)    # MPU6886_FIFO_EN
        tasmota.delay(1)
        self.wire.write(0x68, 0x37, 0x22, 1)    # MPU6886_INT_PIN_CFG
        tasmota.delay(1)
        self.wire.write(0x68, 0x38, 0x01, 1)    # MPU6886_INT_ENABLE
        tasmota.delay(100)

        self.enabled = true
        self.gres = 2000.0/32768.0
        self.ares = 8.0/32678.0
        print("I2C: MPU6886 detected on bus "+str(bus))
      end
    end
  end

  #- returns a list of 3 axis, float as g acceleration -#
  def read_accel()
    var b = self.wire.read_bytes(0x68,0x3B,6)
    var a1 = b.get(0,-2)
    if a1 >= 0x8000 a1 -= 0x10000 end
    var a2 = b.get(2,-2)
    if a2 >= 0x8000 a2 -= 0x10000 end
    var a3 = b.get(4,-2)
    if a3 >= 0x8000 a3 -= 0x10000 end
    self.accel = [a1 * self.ares, a2 * self.ares, a3 * self.ares]
    return self.accel
  end

  #- returns a list of 3 gyroscopes, int as dps (degree per second)  -#
  def read_gyro()
    var b = self.wire.read_bytes(0x68,0x43,6)
    var g1 = b.get(0,-2)
    if g1 >= 0x8000 g1 -= 0x10000 end
    var g2 = b.get(2,-2)
    if g2 >= 0x8000 g2 -= 0x10000 end
    var g3 = b.get(4,-2)
    if g3 >= 0x8000 g3 -= 0x10000 end
    self.gyro = [int(g1 * self.gres), int(g2 * self.gres), int(g3 * self.gres)]
    return self.gyro
  end

  #- trigger a read every second -#
  def every_second()
    self.read_accel()
    self.read_gyro()
  end

  #- display sensor value in the web UI -#
  def web_sensor()
    import string
    var msg = string.format(
             "{s}MPU6886 acc_x{m}%f G{e}"..
             "{s}MPU6886 acc_y{m}%f G{e}"..
             "{s}MPU6886 acc_z{m}%f G{e}"..
             "{s}MPU6886 gyr_x{m}%i dps{e}"..
             "{s}MPU6886 gyr_y{m}%i dps{e}"..
             "{s}MPU6886 gyr_z{m}%i dps{e}",
              self.accel[0], self.accel[1], self.accel[2], self.gyro[0], self.gyro[1], self.gyro[2])
    tasmota.web_send_decimal(msg)
  end

  #- add sensor value to teleperiod -#
  def json_append()
    import string
    var ax = int(self.accel[0] * 1000)
    var ay = int(self.accel[1] * 1000)
    var az = int(self.accel[2] * 1000)
    var msg = string.format(",\"MPU6886\":{\"AX\":%i,\"AY\":%i,\"AZ\":%i,\"GX\":%i,\"GY\":%i,\"GZ\":%i}",
              ax, ay, az, self.gyro[0], self.gyro[1], self.gyro[2])
    tasmota.response_append(msg)
  end

end
mpu = MPU6886()
tasmota.add_driver(mpu)