/*
  xsns_98_luxv30b.ino - Driver for DFRobot V30B lux sensor

  Copyright (C) 2022 Marius Bezuidenhout

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
#ifdef USE_LUXV30B

#ifdef USE_ADS1115
  // Conflicts with ADS1115
  #warning "USE_LUXV30B conflicts with USE_ADS1115. Consider undefining USE_ADS1115 to use this device."
#endif // USE_ADS1115

/**************************************************************************************************
 * Driver for DFRobot V30B Lux sensor
 * ================================================================================================
 * This driver use the I2C mode. Uses address 0x94 and 0x95. Its address cannot be changed.
 * 
 * Supply Voltage: 2.7-6V
 * Operating Current: 0.7mA
 * Detection Range: 0-200klx
 * Accuracy: 0.054lx
 * Operating Temperature Range: -40°C~+85°C
 * I2C Address: 0x4A
 * 
 * Pin assignments:
 * 
 * ------------------------------------------
 * | Num | Label |        Description       |
 *   1      VCC    2.7-6V +
 *   2      SCL    I2C Serial Clock Line
 *   3      SDA    I2C Serial Data
 *   4      GND    Ground
 *   5      EN     Sensor Chip-select Enable/Disable port, High to enable, Low to disable sensor
 * 
 * 
 * You can write the desired configuration to the configuration register(address:0x04), setting 
 * different acquisition accuracy.
 * You can read the light intensity data from the data register(address:0x00~0x03).
 * ------------------------------------------------------------------------------------------
 * |    b7    |    b6    |    b5    |    b4    |    b3    |    b2    |    b1     |    b0    |
 * ------------------------------------------------------------------------------------------
 * |    0     |  MANUAL  |    0     |     0    |    CDR   |               TIM               |
 * ------------------------------------------------------------------------------------------
 * MANUAL  ：Manual configuration register.
 *          0 represents the default automatic mode.In this mode ,CDR and TIM are automatically assigned.
 *          1 represents the configuration of manual mode.In this mode,CDR and TIM can be set by the user.
 * CDR     ：Shunt ratio register.
 *          0 represents the default of not dividing,all the current of the photodiode into the ADC
 *          1 represents the division of 8,as long as 1/8 of the current of the photodiode changes to ADC. 
 *            This mode is used in high brightness situations.
 * TIM[2:0]：Acquisition time.
 *        ------------------------------------------------------------------------------------------------
 *          TIM[2:0]  |  TIME（ms）  |                          Introduction                             |
 *        ------------------------------------------------------------------------------------------------
 *             000    |      800     |            Preferred mode in low light environment                |
 *        ------------------------------------------------------------------------------------------------
 *             001    |      400     |                               ---                                 |
 *        ------------------------------------------------------------------------------------------------
 *             010    |      200     |                               ---                                 |
 *        ------------------------------------------------------------------------------------------------
 *             011    |      100     |   In the strong light environment, select the mode preferentially |
 *        ------------------------------------------------------------------------------------------------
 *             100    |      50      |                       Manual mode only                            |
 *        ------------------------------------------------------------------------------------------------
 *             101    |      250     |                       Manual mode only                            |
 *        ------------------------------------------------------------------------------------------------
 *             110    |      12.5    |                       Manual mode only                            |
 *        ------------------------------------------------------------------------------------------------
 *             111    |      6.25    |                       Manual mode only                            |
 *        ------------------------------------------------------------------------------------------------
 * Accuracy that can be set in manual mode:
 *     -------------------------------------------------------------------------------------------------------------
 *     |                    Light conditions                        |                        |     TIM & CDR       |
 *     -------------------------------------------------------------------------------------------------------------
 *     |   Minimum accuracy    |   Maximum accuracy   |   Maximum   |  Acquisition time(ms)  |    TIM     |   CDR  |
 *     —------------------------------------------------------------------------------------------------------------
 *              0.054                     11.52            2938                800                000           0   
 *              0.09                      23.04            5875                400                001           0   
 *              0.18                      46.08            11750               200                010           0   
 *              0.36                      92.16            23501               100                011           0   
 *              0.36                      92.16            23501               800                000           1   
 *              0.72                      184.32           47002               50                 100           0   
 *              0.72                      184.32           47002               400                001           1   
 *              1.44                      368.64           94003               25                 101           0   
 *              1.44                      368.64           94003               200                010           1   
 *              2.88                      737.28           200000              12.5               110           0   
 *              2.88                      737.28           200000              100                011           1   
 *              5.76                      737.28           200000              6.25               111           0   
 *              5.76                      737.28           200000              50                 100           1   
 *              11.52                     737.28           200000              25                 101           1   
 *              23.04                     737.28           200000              12.5               110           1   
 *              46.08                     737.28           200000              6.25               111           1   
 *     —------------------------------------------------------------------------------------------------------------
 * *************************************************************************************************/

#define XSNS_98        98
#define XI2C_69        69  // See I2CDEVICES.md

#define LUXV30B_ADDR      0x4A  // Two wire library uses 7-bit addresses throughout
#define LUXV30B_DATAREG   0x00   //the address of the data register
#define LUXV30B_CONFREG   0x04   //the address of the configuration register

class LUXV30B
{
  public:
    LUXV30B();
    void detect();
    bool found();
    void read();
    float lux();
  private:
    bool _found;
    uint32_t _lux;
};

LUXV30B::LUXV30B()
{
  _found = false;
  _lux = 0;
}

float LUXV30B::lux()
{
  return ((float)_lux * 1.4) / 1000;
}

bool LUXV30B::found()
{
  return _found;
}

void LUXV30B::detect()
{
  if (I2cActive(LUXV30B_ADDR)) { return; }
  Wire.beginTransmission(LUXV30B_ADDR);
  if (0 == Wire.endTransmission()) {
    _found = true;
    I2cSetActiveFound(LUXV30B_ADDR, "LUXV30B");
  }
}

void LUXV30B::read()
{
  _lux  = I2cRead8(LUXV30B_ADDR, 0);
  delay(8);
  _lux |= I2cRead8(LUXV30B_ADDR, 1) << 8;
  delay(8);
  _lux |= I2cRead8(LUXV30B_ADDR, 2) << 16;
  delay(8);
  _lux |= I2cRead8(LUXV30B_ADDR, 3) << 24;
}

LUXV30B luxv30b;

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns98(byte function)
{
  if (!I2cEnabled(XI2C_69))
  {
    return false;
  }

  bool result = false;

  if (FUNC_INIT == function)
  {
    luxv30b.detect();
  }

  else if (luxv30b.found())
  {
    char lux[FLOATSZ];
    dtostrfd(luxv30b.lux(), 2, lux);

    switch (function)
    {
    case FUNC_EVERY_SECOND:
      luxv30b.read();
      break;
    case FUNC_JSON_APPEND:
      if (0 < luxv30b.lux())
      {
        ResponseAppend_P(PSTR(",\"LUXV30B\":{\"" D_JSON_ILLUMINANCE "\":%s\"}"), lux);
      }
      break;
#ifdef USE_WEBSERVER
    case FUNC_WEB_SENSOR:
      if (0 < luxv30b.lux())
      {
        WSContentSend_PD(PSTR("{s}LUXV30B " D_ILLUMINANCE "{m}%s " D_UNIT_LUX "{e}"), lux);
      }
      break;
#endif // USE_WEBSERVER
    }
  }

  return result;
}

#endif  // USE_LUXV30B
#endif  // USE_I2C