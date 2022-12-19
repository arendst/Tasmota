/*
  xsns_33_qmc5883l.ino - QMC5883L 3-Axis Digital Compass sensor support for Tasmota

  Copyright (C) 2022  Helge Scheunemann

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_I2C
#ifdef USE_QMC5883L
/*********************************************************************************************\
 * QMC5883L is 3-Axis Digital Compass sensor
 *
 * I2C Address: 0x0D
\*********************************************************************************************/
/*
 DATASHEET

  The QMC5883L is a multi-chip three-axis magnetic sensor. This
  surface -mount, small sized chip has integrated magnetic sensors with
  signal condition ASIC, targeted for high precision applications such as
  compassing, navigation and gaming in drone, robot, mobile and
  personal hand-held devices.
  The QMC5883L is based on our state-of-the-art, high resolution,
  magneto-resistive technology licensed from Honeywell AMR technology.
  Along with custom-designed 16-bit ADC ASIC, it offers the advantages of
  low noise, high accuracy, low power consumption, offset cancellation and
  temperature compensation. QMC5883L enables 1° to 2° compass
  heading accuracy. The I²C serial bus allows for easy interface

  9.1 Register Map
  The table below provides a list of the 8-bit registers embedded in the device and their respective function and
  addresses

  Table 13. Register Map
  Addr.   7       6       5       4       3       2       1      0        Access
  00H     Data Output X LSB Register XOUT[7:0]                            Read only
  01H     Data Output X MSB Register XOUT[15:8]                           Read only
  02H     Data Output Y LSB Register YOUT[7:0]                            Read only
  03H     Data Output Y MSB Register YOUT[15:8]                           Read only
  04H     Data Output Z LSB Register ZOUT[7:0]                            Read only
  05H     Data Output Z MSB Register ZOUT[15:8]                           Read only
  06H                                             DOR     OVL    DRDY     Read only
  07H     TOUT[7:0] Read only
  08H     TOUT[15:8] Read only
  09H     OSR[1:0]        RNG[1:0]        ODR[1:0]        MODE[1:0]       Read/Write
  0AH     SOFT_RST        ROL_P NT                        INT_E NB        R/W, Read only on blanks
  0BH     SET/RESET Period FBR [7:0]                                      Read/Write
  0CH     Reserved                                                        Read only
  0DH     Reserved                                                        Read only

  9.2 Register Definition
  9.2.1 Output Data Register
  Registers 00H ~ 05H store the measurement data from each axis magnetic sensor in continuous-measurement.
  In the continuous measurement mode, the output data is refreshed periodically based on the data update rate
  ODR setup in control registers 1. The data stays the same, regardless of reading status through I2C, until new
  data replaces them. Each axis has 16 bit data width in 2’s complement, i.e., MSB of 01H/03H/05H indicates the
  sign of each axis. The output data of each channel saturates at -32768 and 32767.

  Table 14. Output Data Register
  Addr. 7 6 5 4 3 2 1 0
  00H Data Output X LSB Register XOUT[7:0]
  01H Data Output X MSB Register XOUT[15:8]
  02H Data Output Y LSB Register YOUT[7:0]
  03H Data Output Y MSB Register YOUT[15:8]
  04H Data Output Z LSB Register ZOUT[7:0]
  05H Data Output Z MSB Register ZOUT[15:8]

  9.2.2 Status Register
  There are two status registers located in address 06H and 0CH.
  Register 06H has three bits indicating for status flags, the rest are reserved for factory use. The status registers
  are read only bits.

  Table 15. Status Register 1
  Addr. 7     6     5     4     3     2     1     0
        06H                           DOR   OVL   DRDY

  Data Ready Register (DRDY), it is set when all three axis data is ready, and loaded to the output data registers in
  the continuous measurement mode. It is reset to “0” by reading any data register (00H~05H) through I2C
  commends
  DRDY: “0”: no new data, “1”: new data is ready
  Overflow flag (OVL) is set to “1” if any data of three axis magnetic sensor channels is out of range. The output
  data of each axis saturates at -32768 and 32767, if any of the axis exceeds this range, OVL flag is set to “1”. This
  flag is reset to “0” if next measurement goes back to the range of (-32768, 32767), otherwise, it keeps as “1”.
  OVL: “0”: normal, “1”: data overflow
  Data Skip (DOR) bit is set to “1” if all the channels of output data registers are skipped in reading in the
  continuous-measurement mode. It is reset to “0” by reading any data register (00H~05H) through I2C
  DOR: “0”: normal, “1”: data skipped for reading

  9.2.3 Temperature Data Registers
  Registers 07H-08H store temperature sensor output data. 16 bits temperature sensor output is in 2’s complement.
  Temperature sensor gain is factory-calibrated, but its offset has not been compensated, only relative temperature
  value is accurate. The temperature coefficient is about 100 LSB/℃

  Table 17. Temperature Sensor Output
  Addr. 7 6 5 4 3 2 1 0
  07H   TOUT[7:0]
  08H   TOUT[15:8]

  9.2.4 Control Registers
  Two 8-bits registers are used to control the device configurations.
  Control register 1 is located in address 09H, it sets the operational modes (MODE). output data update rate
  (ODR), magnetic field measurement range or sensitivity of the sensors (RNG) and over sampling rate (OSR).
  Control register 2 is located in address 0AH. It controls Interrupt Pin enabling (INT_ENB), Point roll over function
  enabling(POL_PNT) and soft reset (SOFT_RST).
  Two bits of MODE registers can transfer mode of operations in the device, the two modes are Standby, and
  Continuous measurements. The default mode after Power-on-Reset (POR) is standby. There is no any restriction
  in the transferring between the modes.
  Output data rate is controlled by ODR registers. Four data update frequencies can be selected: 10Hz, 50Hz,
  100Hz and 200Hz. For most of compassing applications, we recommend 10 Hz for low power consumption. For
  gaming, the high update rate such as 100Hz or 200Hz can be used.
  Field ranges of the magnetic sensor can be selected through the register RNG. The full scale field range is
  determined by the application environments. For magnetic clear environment, low field range such as +/- 2gauss
  can be used. The field range goes hand in hand with the sensitivity of the magnetic sensor. The lowest field range
  has the highest sensitivity, therefore, higher resolution.
  Over sample Rate (OSR) registers are used to control bandwidth of an internal digital filter. Larger OSR value
  leads to smaller filter bandwidth, less in-band noise and higher power consumption. It could be used to reach a
  good balance between noise and power. Four over sample ratio can be selected, 64, 128, 256 or 512.

  Table 18. Control Register 1
  Addr 7    6     5     4     3     2     1     0
  09H   OSR[1:0]  RNG[1:0]    ODR[1:0]    MODE[1:0]

  Reg.      Definition          00          01            10          11
  Mode      Mode Control        Standby     Continuous    Reserve     Reserve
  ODR       Output Data Rate    10Hz        50Hz          100Hz       200Hz
  RNG       Full Scale          2G          8G            Reserve     Reserve
  OSR       Over Sample Ratio   512         256           128         64

  Interrupt enabling is controlled by register INT_ENB in control register 2. Once the interrupt is enabled, it will flag
  when new data is in Data Output Registers.
  INT_ENB: “0”: enable interrupt PIN, “1”: disable interrupt PIN
  Pointer roll-over function is controlled by ROL_PNT register. When the point roll-over function is enabled, the I2C
  data pointer automatically rolls between 00H ~ 06H, if I2C read begins at any address among 00H~06H.
  ROL_PNT: “0”: Normal, “1”: Enable pointer roll-over function
  Soft Reset can be done by changing the register SOFT_RST to set. Soft reset can be invoked at any time of any
  mode. For example, if soft reset occurs at the middle of continuous mode reading, QMC5883L immediately
  switches to standby mode due to mode register is reset to “00” in default.
  SOFT_RST: “0”: Normal“1”: Soft reset, restore default value of all registers.
  Table 19. Control Register 2
  Addr.     7         6         5         4          3        2         1         0
  0AH       SOFT_RST  ROL_PNT                                                     INT_ENB

  9.2.5 SET/RESET Period Register
  SET/RESET Period is controlled by FBR [7:0], it is recommended that the register 0BH is written by 0x01.

  Table 20. SET/RESET Period Register
  Addr.     7         6         5         4         3         2         1         0
  0BH       SET/RESET Perio  [7:0]

*/

// Define driver ID
#define XSNS_33                          33
#define XI2C_71                          71  // See I2CDEVICES.md

#ifndef QMC5883L_TEMP_SHIFT
#define QMC5883L_TEMP_SHIFT             23   // sensor temperature are not calibrated (only relativ measurement) and need an absolute ground value in °C (see datasheet)
#endif

/* The default I2C address of this chip */
#define QMC5883L_ADDR                   0x0D

/* Register numbers */
#define QMC5883L_X_LSB                  0x00
#define QMC5883L_X_MSB                  0x01
#define QMC5883L_Y_LSB                  0x02
#define QMC5883L_Y_MSB                  0x03
#define QMC5883L_Z_LSB                  0x04
#define QMC5883L_Z_MSB                  0x05
#define QMC5883L_STATUS                 0x06
#define QMC5883L_TEMP_LSB               0x07
#define QMC5883L_TEMP_MSB               0x08
#define QMC5883L_CONFIG                 0x09
#define QMC5883L_CONFIG2                0x0a
#define QMC5883L_RESET                  0x0b
#define QMC5883L_RESERVED               0x0c
#define QMC5883L_CHIP_ID                0x0d

/* Bit values for the STATUS register */
#define QMC5883L_STATUS_DRDY            1
#define QMC5883L_STATUS_OVL             2
#define QMC5883L_STATUS_DOR             4

/* Oversampling values for the CONFIG register */
#define QMC5883L_CONFIG_OS512           0b00000000
#define QMC5883L_CONFIG_OS256           0b01000000
#define QMC5883L_CONFIG_OS128           0b10000000
#define QMC5883L_CONFIG_OS64            0b11000000

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS          0b00000000
#define QMC5883L_CONFIG_8GAUSS          0b00010000

/* Rate values for the CONFIG register */
#define QMC5883L_CONFIG_10HZ            0b00000000
#define QMC5883L_CONFIG_50HZ            0b00000100
#define QMC5883L_CONFIG_100HZ           0b00001000
#define QMC5883L_CONFIG_200HZ           0b00001100

/* Mode values for the CONFIG register */
#define QMC5883L_CONFIG_STANDBY         0b00000000
#define QMC5883L_CONFIG_CONT            0b00000001

/* Mode values for the CONFIG2 register */
#define QMC5883L_CONFIG2_RESET          0b10000000

    // data field
struct QMC5883L_s {
  int16_t   MX, MY, MZ;
  float     temp;
  uint16_t  scalar;
} *QMC5883L = nullptr;


// Initialise the device
void QMC5883L_Init() {
  if (!I2cSetDevice(QMC5883L_ADDR)) { return; }
  // reset QMC5883L
  if (I2cWrite8(QMC5883L_ADDR, QMC5883L_CONFIG2, QMC5883L_CONFIG2_RESET) == false) { return; }
  // Software Reset
  if (I2cWrite8(QMC5883L_ADDR, QMC5883L_RESET, 0x01) == false) { return; }
  // write config
  if (I2cWrite8(QMC5883L_ADDR, QMC5883L_CONFIG, QMC5883L_CONFIG_OS256 | QMC5883L_CONFIG_8GAUSS | QMC5883L_CONFIG_100HZ | QMC5883L_CONFIG_CONT) == false) { return; }

  I2cSetActiveFound(QMC5883L_ADDR, "QMC5883L");
  QMC5883L = (QMC5883L_s *)calloc(1, sizeof(struct QMC5883L_s));
}

//Read the magnetic data
void QMC5883L_read_data(void) {
  // check if chip is ready to provice data
  if (!(I2cRead8(QMC5883L_ADDR, QMC5883L_STATUS) & QMC5883L_STATUS_DRDY)) { return; }  // Chip not yet ready, next round try again

  QMC5883L->MX = I2cReadS16_LE(QMC5883L_ADDR, QMC5883L_X_LSB);  // Select LSB register
  QMC5883L->MY = I2cReadS16_LE(QMC5883L_ADDR, QMC5883L_Y_LSB);
  QMC5883L->MZ = I2cReadS16_LE(QMC5883L_ADDR, QMC5883L_Z_LSB);

  // calculate scalar magnetic induction
//  QMC5883L->scalar = sqrt((QMC5883L->MX * QMC5883L->MX) + (QMC5883L->MY * QMC5883L->MY) + (QMC5883L->MZ * QMC5883L->MZ));  // 650 bytes larger
  QMC5883L->scalar = SqrtInt((QMC5883L->MX * QMC5883L->MX) + (QMC5883L->MY * QMC5883L->MY) + (QMC5883L->MZ * QMC5883L->MZ));

  // get temperature
  QMC5883L->temp = ConvertTemp((I2cReadS16_LE(QMC5883L_ADDR, QMC5883L_TEMP_LSB) / 100) + QMC5883L_TEMP_SHIFT);  // Temp in celsius
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_QMC5883L[] PROGMEM =
    "{s}QMC5883L " D_MX "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}QMC5883L " D_MY "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}QMC5883L " D_MZ "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}QMC5883L " D_MAGNETICFLD "{m}%d " D_UNIT_MICROTESLA "{e}";   // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif

void QMC5883L_Show(uint8_t json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"QMC5883L\":{\"" D_JSON_MX "\":%d,\"" D_JSON_MY "\":%d,\"" D_JSON_MZ "\":%d,\"" D_JSON_MAGNETICFLD "\":%u,\"" D_JSON_TEMPERATURE "\":%*_f}"),
      QMC5883L->MX, QMC5883L->MY, QMC5883L->MZ, QMC5883L->scalar, Settings->flag2.temperature_resolution, &QMC5883L->temp);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_QMC5883L, QMC5883L->MX, QMC5883L->MY, QMC5883L->MZ, QMC5883L->scalar);
    WSContentSend_Temp("QMC5883L", QMC5883L->temp);
#endif
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns33(uint32_t function) {
  if (!I2cEnabled(XI2C_71)) { return false; }

  if (FUNC_INIT == function) {
    QMC5883L_Init();
  }
  else if (QMC5883L != nullptr) {
    switch (function) {
    case FUNC_JSON_APPEND:
      QMC5883L_Show(1);
      break;
    case FUNC_EVERY_SECOND:
      QMC5883L_read_data();
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      QMC5883L_Show(0);
      break;
#endif  // USE_WEBSERVER
    }
  }
  return true;
}
#endif  // USE_QMC5883L
#endif  // USE_I2C