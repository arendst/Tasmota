/*
  xsns_100_ina3221.ino - INA3221 3-channels Current Sensor support for Tasmota

  Copyright (C) 2021  Barbudor and Theo Arends
  Based on Barbudor's CircuitPython_INA3221

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
#ifdef USE_INA3221
/*********************************************************************************************\
 * INA3221 - 3 channels High-side DC voltage and current measurment
 * https://www.ti.com/product/INA3221
 *
 * Up to 4 devices can be connected (12 channels)
 * I2C Address: 0x40, 0x41, 0x43 or 0x43
 *
 * IMPORTANT INFORMATION
 * By default the driver is enabled to support up to 4 INA3221 from hte above addresse
 * If you want to enable less addresses in order to use other I2C chip on those addresses you
 * can define in your user_config_override.h the following:
 * #define INA3221_MAX_COUNT      the max number of INA3221 to support
 * #define INA3221_ADDRESS1       the I2C address of the 1st INA3221
 * For example to support only 2 INA3221 at addresses 0x41 and 0x42 you can use:
 * #define INA3221_MAX_COUNT      2
 * #define INA3221_ADDRESS1       0x41
 * That would leave 0x40 and 0x42 for other devices
 * Nevertheless, hte driver tries to identifiy if the chip as an address is a IN3221
\*********************************************************************************************/

#define XSNS_100                                100
#define XI2C_72                                 72        // See I2CDEVICES.md

#ifndef INA3221_MAX_COUNT
#define INA3221_MAX_COUNT                       4
#endif
#if (INA3221_MAX_COUNT > 4)
#error "**** INA3221_MAX_COUNT can't be greater than 4 ****"
#endif

#ifndef INA3221_ADDRESS1
#define INA3221_ADDRESS1                        (0x40)    // 1000000 (A0=GND)
#define INA3221_ADDRESS2                        (0x41)    // 1000000 (A0=VS)
#define INA3221_ADDRESS3                        (0x42)    // 1000010 (A0=SDA)
#define INA3221_ADDRESS4                        (0x43)    // 1000011 (A0=SCL)
#endif
#define INA3221_ADDRESS(index)                  (INA3221_ADDRESS1+(index))
#if ((INA3221_ADDRESS1 + INA3221_MAX_COUNT) > 0x44)
#error "**** INA3221 bad combination for ADDRESS1 and MAX_COUNT ****"
#endif

#define INA3221_NB_CHAN                         (3)

// Config register - ch : 0..2
#define INA3221_REG_CONFIG                      (0x00)

#define INA3221_RESET                           (0x8000)
#define INA3221_ENABLE_MASK                     (0x7000)
#define INA3221_ENABLE_CH(ch)                   (0x4000>>(ch))    // default: set

#define INA3221_AVERAGING_MASK                  (0x0E00)
#define INA3221_AVERAGING_NONE                  (0x0000)  // 1 sample, default
#define INA3221_AVERAGING_4_SAMPLES             (0x0200)
#define INA3221_AVERAGING_16_SAMPLES            (0x0400)
#define INA3221_AVERAGING_64_SAMPLES            (0x0600)
#define INA3221_AVERAGING_128_SAMPLES           (0x0800)
#define INA3221_AVERAGING_256_SAMPLES           (0x0A00)
#define INA3221_AVERAGING_512_SAMPLES           (0x0C00)
#define INA3221_AVERAGING_1024_SAMPLES          (0x0E00)

#define INA3221_VBUS_CONV_TIME_MASK             (0x01C0)
#define INA3221_VBUS_CONV_TIME_140US            (0x0000)
#define INA3221_VBUS_CONV_TIME_204US            (0x0040)
#define INA3221_VBUS_CONV_TIME_332US            (0x0080)
#define INA3221_VBUS_CONV_TIME_588US            (0x00C0)
#define INA3221_VBUS_CONV_TIME_1MS              (0x0100)  // 1.1ms, default
#define INA3221_VBUS_CONV_TIME_2MS              (0x0140)  // 2.116ms
#define INA3221_VBUS_CONV_TIME_4MS              (0x0180)  // 4.156ms
#define INA3221_VBUS_CONV_TIME_8MS              (0x01C0)  // 8.244ms

#define INA3221_SHUNT_CONV_TIME_MASK            (0x0038)
#define INA3221_SHUNT_CONV_TIME_140US           (0x0000)
#define INA3221_SHUNT_CONV_TIME_204US           (0x0008)
#define INA3221_SHUNT_CONV_TIME_332US           (0x0010)
#define INA3221_SHUNT_CONV_TIME_588US           (0x0018)
#define INA3221_SHUNT_CONV_TIME_1MS             (0x0020)  // 1.1ms, default
#define INA3221_SHUNT_CONV_TIME_2MS             (0x0028)  // 2.116ms
#define INA3221_SHUNT_CONV_TIME_4MS             (0x0030)  // 4.156ms
#define INA3221_SHUNT_CONV_TIME_8MS             (0x0038)  // 8.244ms

#define INA3221_MODE_MASK                       (0x0007)
#define INA3221_MODE_POWER_DOWN                 (0x0000)
#define INA3221_MODE_SHUNT_VOLTAGE_TRIGGERED    (0x0001)
#define INA3221_MODE_BUS_VOLTAGE_TRIGGERED      (0x0002)
#define INA3221_MODE_SHUNT_AND_BUS_TRIGGERED    (0x0003)
#define INA3221_MODE_POWER_DOWN2                (0x0004)
#define INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS   (0x0005)
#define INA3221_MODE_BUS_VOLTAGE_CONTINUOUS     (0x0006)
#define INA3221_MODE_SHUNT_AND_BUS_CONTINOUS    (0x0007)

// Other registers - ch = 0..2
#define INA3221_REG_SHUNT_VOLTAGE_CH(ch)        (0x01+((ch)<<1))
#define INA3221_REG_BUS_VOLTAGE_CH(ch)          (0x02+((ch)<<1))
#define INA3221_REG_CRITICAL_ALERT_LIMIT_CH(ch) (0x07+((ch)<<1))
#define INA3221_REG_WARNING_ALERT_LIMIT_CH(ch)  (0x08+((ch)<<1))
#define INA3221_REG_SHUNT_VOLTAGE_SUM           (0x0D)
#define INA3221_REG_SHUNT_VOLTAGE_SUM_LIMIT     (0x0E)

// Mask/enable register - ch = 1..3
#define INA3221_REG_MASK_ENABLE                 (0x0F)
#define INA3221_SUM_CONTROL_CH(ch)              (0x4000>>((ch)-1))
#define INA3221_WARNING_LATCH_ENABLE            (0x0800)
#define INA3221_CRITICAL_LATCH_ENABLE           (0x0400)
#define INA3221_CRITICAL_FLAG_CH(ch)            (0x0200>>((ch)-1))
#define INA3221_SUM_ALERT_FLAG                  (0x0040)
#define INA3221_WARNING_FLAG_CH(ch)             (0x0020>>((ch)-1))
#define INA3221_POWER_ALERT_FLAG                (0x0004)
#define INA3221_TIMING_ALERT_FLAG               (0x0002)
#define INA3221_CONV_READY_FLAG                 (0x0001)

// Other registers
#define INA3221_REG_POWER_VALID_UPPER_LIMIT     (0x10)
#define INA3221_REG_POWER_VALID_LOWER_LIMIT     (0x11)
#define INA3221_REG_MANUFACTURER_ID             (0xFE)
#define INA3221_REG_DIE_ID                      (0xFF)

// Constants for manufacturer and device ID
#define INA3221_MANUFACTURER_ID                 (0x5449)     // "TI"
#define INA3221_DIE_ID                          (0x3220)

// General constants
#define INA3221C_BUS_ADC_LSB                    (0.008)     // VBus ADC LSB is 8mV
#define INA3221C_SHUNT_ADC_LSB                  (0.00004)   // VShunt ADC LSB is 40µV
#define INA3221_DEFAULT_SHUNT_RESISTOR          (0.1)

#ifdef DEBUG_TASMOTA_SENSOR
// temporary strings for floating point in debug messages
char _ina3221_dbg1[FLOATSZ];
char _ina3221_dbg2[FLOATSZ];
#endif

const char INA3221_SENSORCMND_START[] PROGMEM = "{\"" D_CMND_SENSOR "%d\":{\"idx\":%d,\"addr\":\"0x%02X\",\"rshunt\":[";
const char INA3221_SENSORCMND_END[] PROGMEM = "]}}";

struct INA3221_Channel_Data {
  float     voltage;
  float     current;
  float     shunt;
};

struct INA3221_Data {
  struct INA3221_Channel_Data  chan[INA3221_NB_CHAN];
  uint8_t enabled_chan;
  uint8_t i2caddr;
};

struct INA3221_Data *Ina3221Data = nullptr;
uint8_t Ina3221count = 0;
static uint8_t _ina3221_current_device = 0;

#define D_INA3221   "INA3221"
const char INA3221_TYPE[] = D_INA3221;

bool Ina3221SetConfig(uint8_t addr)
{
  // check if device is a INA3221
  uint16_t manufacturer_id = 0, die_id = 0;
  if (!I2cValidRead16(&manufacturer_id, addr, INA3221_REG_MANUFACTURER_ID)
      || (manufacturer_id != INA3221_MANUFACTURER_ID)
      || !I2cValidRead16(&die_id, addr, INA3221_REG_DIE_ID)
      || (die_id != INA3221_DIE_ID)) {
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_INA3221 ":Skipping device at addr:0x%02X not an " D_INA3221), addr);
        AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_INA3221 ":SetConfig: manId=0x%04X, dieId=0x%04X"), manufacturer_id, die_id);
        return false;
      }

  // write default configuration
  uint16_t config = INA3221_ENABLE_MASK |
                    INA3221_AVERAGING_16_SAMPLES |
                    INA3221_VBUS_CONV_TIME_1MS |
                    INA3221_SHUNT_CONV_TIME_1MS |
                    INA3221_MODE_SHUNT_AND_BUS_CONTINOUS;
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ":SetConfig: addr:0x%02X, config=0x%04X"), addr, config);
  // Set Config register
  if (!I2cWrite16(addr, INA3221_REG_CONFIG, config))
    return false;

  return true;
}

#ifdef USE_DEEPSLEEP
bool Ina3221PowerDown(uint8_t device)
{
  uint8_t addr = Ina3221Data[device].i2caddr;
  // write default configuration
  uint16_t config = INA3221_MODE_POWER_DOWN;
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ":PowerDown: addr:0x%02X, config=0x%04X"), addr, config);
  // Set Config register
  if (!I2cWrite16(addr, INA3221_REG_CONFIG, config))
    return false;

  return true;
}
#endif

void Ina3221SetShunt(uint8_t device, uint8_t channel, float shunt)
{
  Ina3221Data[device].chan[channel].shunt = shunt;
  if (shunt > 0.0)
    Ina3221Data[device].enabled_chan |= (1<<channel);
  else
    Ina3221Data[device].enabled_chan &= ~(1<<channel);
}

bool Ina3221Read(uint8_t device, uint8_t channel)
{
  uint8_t addr = Ina3221Data[device].i2caddr;
  int16_t bus_voltage, shunt_voltage;
  struct INA3221_Channel_Data *pChannel = &Ina3221Data[device].chan[channel];

  bus_voltage = I2cReadS16(addr, INA3221_REG_BUS_VOLTAGE_CH(channel));
  DEBUG_SENSOR_LOG(D_INA3221 ":GetBusVoltage: RegVBus[%d:%d](0x%02X) = 0x%04X = %d", device, channel, INA3221_REG_BUS_VOLTAGE_CH(channel), bus_voltage, bus_voltage);
  // Convert to VBus voltage in V
  pChannel->voltage = INA3221C_BUS_ADC_LSB * (float)(bus_voltage >> 3);

  if (pChannel->shunt > 0.0) {
    shunt_voltage = I2cReadS16(addr, INA3221_REG_SHUNT_VOLTAGE_CH(channel));
    DEBUG_SENSOR_LOG(D_INA3221 ":GetShuntVoltage: RegSh[%d:%d](0x%02X) = 0x%04X = %d", device, channel, INA3221_REG_SHUNT_VOLTAGE_CH(channel), shunt_voltage, shunt_voltage);
    // convert to shunt voltage in V
    pChannel->current = INA3221C_SHUNT_ADC_LSB * (float)(shunt_voltage >> 3) / pChannel->shunt;
  } else {
    pChannel->current = INFINITY;
  }

  #ifdef DEBUG_TASMOTA_SENSOR
  dtostrfd(pChannel->voltage,5,_ina3221_dbg1);
  dtostrfd(pChannel->current,5,_ina3221_dbg2);
  DEBUG_SENSOR_LOG(D_INA3221 ":Read[%d:%d]: V=%sV, I=%sA", device, channel, _ina3221_dbg1, _ina3221_dbg2);
  #endif

  return true;
}

/*********************************************************************************************\
 * Command Sensor
\*********************************************************************************************/

bool Ina3221CmndSensor(void)
{
  int argc = ArgC();
  if(argc != 1 && argc != 4) {
    return false;
  }

  char argument[XdrvMailbox.data_len+FLOATSZ];
  uint32_t device = atoi(ArgV(argument,1)) -1;
  if (device >= INA3221_MAX_COUNT || !Ina3221Data[device].i2caddr) {
    DEBUG_SENSOR_LOG(D_INA3221 ":Sensor: invalid device %d", device+1);
    return false;
  }

  if (argc > 1) {
    for (int channel = 0 ; channel < INA3221_NB_CHAN ; channel++) {
      float shunt = CharToFloat(ArgV(argument,2+channel));
      Ina3221SetShunt(device, channel, shunt);
    }
  }
  Response_P(INA3221_SENSORCMND_START, XSNS_100, device +1, Ina3221Data[device].i2caddr);
  for (int channel = 0 ; channel < INA3221_NB_CHAN ; channel++ ) {
    dtostrfd(Ina3221Data[device].chan[channel].shunt,5,argument);
    ResponseAppend_P(PSTR("%s%c"), argument, ((channel < (INA3221_NB_CHAN-1))?',':'\0'));
  }
  ResponseAppend_P(INA3221_SENSORCMND_END);

  return true;
}

/********************************************************************************************/

void Ina3221Detect(void)
{
  _ina3221_current_device = 0;
  Ina3221count = 0;
  for (uint32_t i = 0; i < INA3221_MAX_COUNT; i++) {
    uint16_t addr = INA3221_ADDRESS(i);
    if (!I2cSetDevice(addr)) { continue; }
    if (!Ina3221Data) {
      Ina3221Data = (struct INA3221_Data*)calloc(INA3221_MAX_COUNT,sizeof(struct INA3221_Data));
      if (!Ina3221Data) {
        AddLog(LOG_LEVEL_ERROR,PSTR(D_INA3221 ": Mem allocation error"));
        return;
      }
    }
    if (Ina3221SetConfig(addr)) {
      I2cSetActiveFound(addr, INA3221_TYPE);
      Ina3221Data[Ina3221count].i2caddr = addr;
      Ina3221Data[Ina3221count].enabled_chan = 0;
      Ina3221Data[Ina3221count].chan[0].shunt = \
      Ina3221Data[Ina3221count].chan[1].shunt = \
      Ina3221Data[Ina3221count].chan[2].shunt = 0.0;
      Ina3221count++;
    }
  }
  if (!Ina3221count && Ina3221Data) {
    free(Ina3221Data);
    Ina3221Data = nullptr;
  }
}

void Ina3221Every250ms(void)
{
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ": cur:%d, en:%d"), _ina3221_current_device, Ina3221Data[_ina3221_current_device].enabled_chan);
  uint8_t enabled_chan = Ina3221Data[_ina3221_current_device].enabled_chan;
  for (int chan = 0 ; enabled_chan ; chan++, enabled_chan>>=1) {
    if (0x01 & enabled_chan)
      Ina3221Read(_ina3221_current_device, chan);
  }

  if (++_ina3221_current_device >= INA3221_MAX_COUNT)
    _ina3221_current_device = 0;
}

#ifdef USE_WEBSERVER
const char HTTP_SNS_INA3221_HEADER[] PROGMEM =
  "{s}" D_INA3221 "&nbsp;&nbsp;&nbsp;&nbsp;</th><td>&nbsp;</td><td style='text-align:right'>" D_VOLTAGE " </td><td>&nbsp;</td><td style='text-align:right'>" D_CURRENT " </td><td>&nbsp;</td><td style='text-align:right'>" D_POWERUSAGE " {e}";

const char HTTP_SNS_INA3221_DATA[] PROGMEM =
  "{s}%s </th></th><td>&nbsp;</td><td style='text-align:right'>%s " D_UNIT_VOLT " </td><td>&nbsp;</td><td style='text-align:right'>%s " D_UNIT_AMPERE " </td><td>&nbsp;</td><td style='text-align:right'>%s " D_UNIT_WATT " {e}";
#endif  // USE_WEBSERVER

void Ina3221Show(bool json)
{
  char name[FLOATSZ];
  char temp[FLOATSZ];
  char voltage[3*FLOATSZ+3];
  char current[3*FLOATSZ+3];
  char power[3*FLOATSZ+3];

  if (json) {
    // data
    for (int device=0 ; device < Ina3221count ; device++) {
      uint8_t enabled_chan = Ina3221Data[device].enabled_chan;
      if (!enabled_chan) continue;

      if (Ina3221count > 1)
        snprintf_P(name, sizeof(name), PSTR("%s%c%d"), INA3221_TYPE, IndexSeparator(), device +1);
      else
        snprintf_P(name, sizeof(name), PSTR("%s"), INA3221_TYPE);
      voltage[0] = current[0] = power[0] = '\0';

      for (int chan=0 ; enabled_chan ; chan++, enabled_chan>>=1) {
        if (0x01 & enabled_chan) {
          dtostrfd(Ina3221Data[device].chan[chan].voltage, Settings->flag2.voltage_resolution, temp);
          strncat(voltage, temp, sizeof(voltage));
          dtostrfd(Ina3221Data[device].chan[chan].current, Settings->flag2.current_resolution, temp);
          strncat(current, temp, sizeof(voltage));
          dtostrfd(Ina3221Data[device].chan[chan].voltage * Ina3221Data[device].chan[chan].current, Settings->flag2.wattage_resolution, temp);
          strncat(power, temp, sizeof(voltage));
        } //if enabled
        else {
          strncat(voltage, "null", sizeof(voltage));
          strncat(current, "null", sizeof(voltage));
          strncat(power, "null", sizeof(voltage));
        }
        if (0xFE & enabled_chan) {
          strncat(voltage, ",", sizeof(voltage));
          strncat(current, ",", sizeof(voltage));
          strncat(power, ",", sizeof(voltage));
        }
      } // for channel
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":\"0x%02x\",\"" D_JSON_VOLTAGE "\":[%s],\"" D_JSON_CURRENT "\":[%s],\"" D_JSON_POWERUSAGE "\":[%s]}"),
                      name, Ina3221Data[device].i2caddr, voltage, current, power);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzSensor(DZ_VOLTAGE, voltage);
        DomoticzSensor(DZ_CURRENT, current);
      }
#endif  // USE_DOMOTICZ
    } // for device
  } // if json
#ifdef USE_WEBSERVER
  else {
    // header
    WSContentSend_PD(HTTP_SNS_INA3221_HEADER);
    // data
    for (int device=0 ; device < Ina3221count ; device++) {
      uint8_t enabled_chan = Ina3221Data[device].enabled_chan;
      for (int chan=0 ; enabled_chan ; chan++, enabled_chan>>=1) {
        if (0x01 & enabled_chan) {
          if (Ina3221count > 1)
            snprintf_P(name, sizeof(name), PSTR("%s%c%d:%d"), INA3221_TYPE, IndexSeparator(), device +1, chan);
          else
            snprintf_P(name, sizeof(name), PSTR("%s:%d"), INA3221_TYPE, chan);
          dtostrfd(Ina3221Data[device].chan[chan].voltage, Settings->flag2.voltage_resolution, voltage);
          dtostrfd(Ina3221Data[device].chan[chan].current, Settings->flag2.current_resolution, current);
          dtostrfd(Ina3221Data[device].chan[chan].voltage * Ina3221Data[device].chan[chan].current, Settings->flag2.wattage_resolution, power);
          WSContentSend_PD(HTTP_SNS_INA3221_DATA, name, voltage, current, power);
        } // if active
      } // for channel
    } // for device
  }
#endif
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns100(uint32_t function)
{
  if (!I2cEnabled(XI2C_72)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    Ina3221Detect();
  }
  else if (Ina3221Data) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        if (XSNS_100 == XdrvMailbox.index) {
          result = Ina3221CmndSensor();
        }
        break;
      case FUNC_EVERY_250_MSECOND:
        Ina3221Every250ms();
        break;
      case FUNC_JSON_APPEND:
        Ina3221Show(1);
        break;
  #ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ina3221Show(0);
        break;
  #endif  // USE_WEBSERVER
  #ifdef USE_DEEPSLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        for (uint8_t device; device < Ina3221count ; device++)
          Ina3221PowerDown(device);
        break;
  #endif // USE_DEEPSLEEP     }
    }
  }
  return result;
}

#endif  // USE_INA3221
#endif  // USE_I2C
