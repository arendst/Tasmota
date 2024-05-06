/*
  xsns_100_ina3221.ino - INA3221 3-channels Current Sensor support for Tasmota

  Copyright (C) 2021  Barbudor and Theo Arends + fb-pilot
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
 *   can define in your user_config_override.h the following:
 *   #define INA3221_MAX_COUNT      the max number of INA3221 to support
 *   #define INA3221_ADDRESS1       the I2C address of the 1st INA3221
 * For example to support only 2 INA3221 at addresses 0x41 and 0x42 you can use:
 *   #define INA3221_MAX_COUNT      2
 *   #define INA3221_ADDRESS1       0x41
 *   That would leave 0x40 and 0x43 for other devices
 * By defining INA3221_CALC_CHARGE_AH INA3221_CALC_ENERGY_WH the driver adds a estimation of 
 *   energies in Ah and Wh to the output, To reset the energie calculation disable tge 
 *   according INA3221 chanel by setting the shunt to 0.0 and enable it again 
 *   For example :
 *     sensor100 1,0,0,0 (or sensor100 1,,,) and sensor100 1,0.1,0.1,0.2 will reset channel 1
 * By defining INA3221_SUPPLY_SIDE the driver adds the measured Shunt Voltage to the Bus Voltage 
 *     for the cannel with a negativ shunt (shunt <0) thus showing the values of the supply side (IN+) 
 *     otherwise (shunt >0) the load-side (IN-) is shown.
 *     e.g. sensor100 1,-0.1,0.1,-0.2
 *   additionaly the bits set (bit 0,1,2) enable the scanning of the voltage in the according channel
 *     e.g. INA3221_SUPPLY_SIDE = 0x0005 enables enables voltage measurment of channel 1 and 3 for the device 1
 * By defining INA3221_CYCLE_COUNT xx ... the device is scanned all xx * 250ms  range 1 .. 127
 * By defining USE_DEEPSLEEP the device is configured to Power down (reduces the supply current 1mA --> 15µA )
 * Nevertheless, the driver tries to identifiy if the chip as an address is a IN3221
\*********************************************************************************************/

#define XSNS_100                                100
#define XI2C_72                                 72        // See I2CDEVICES.md

// setup of INA3221 config
#ifndef INA3221_CONFIG_INIT
#define INA3221_CONFIG_INIT     INA3221_AVERAGING_16_SAMPLES |\
                                INA3221_VBUS_CONV_TIME_1MS |\
                                INA3221_SHUNT_CONV_TIME_1MS
// that results in a complete conversions sequence in 6,6 ms  and a slope time of 105,6 ms
#endif         // end of setup of INA3221 config

#if  !defined(INA3221_MAX_COUNT)
  #define INA3221_MAX_COUNT                       4
#elif (INA3221_MAX_COUNT > 4)
  #undef INA3221_MAX_COUNT
  #define INA3221_MAX_COUNT                       4
  #warning  **** INA3221 bad INA3221_MAX_COUNT ****
  #warning  **** has to be 1 to 4 ... set to 4 ****
#elif (INA3221_MAX_COUNT < 1)
  #undef INA3221_MAX_COUNT
  #define INA3221_MAX_COUNT                       1
  #warning  **** INA3221 bad INA3221_MAX_COUNT ****
  #warning  **** has to be 1 to 4 ... set to 1 ****
#endif
#if !defined(INA3221_CYCLE_COUNT)
  #define INA3221_CYCLE_COUNT                     1
#elif (INA3221_CYCLE_COUNT < 1)
  #undef INA3221_CYCLE_COUNT
  #define INA3221_CYCLE_COUNT                     1
  #warning  **** INA3221 bad INA3221_CYCLE_COUNT ****
  #warning  **** must be 1 to 127 ... set to 1 ****
#elif (INA3221_CYCLE_COUNT > 127)
  #undef INA3221_CYCLE_COUNT
  #define INA3221_CYCLE_COUNT                     127
  #warning  **** INA3221 bad INA3221_CYCLE_COUNT ****
  #warning  **** must be 1 to 127 ... set to 127 ****
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
#if (INA3221_NB_CHAN>4)
#error "**** INA3221 too manny channels ****"
#endif
// Config register - ch : 0..2
#define INA3221_REG_CONFIG                      (0x00)

#define INA3221_RESET                           (0x8000)
#define INA3221_ENABLE_MASK                     (0x7000)
#define INA3221_ENABLE_CH(ch)                   (0x4000>>(ch))    // default: set

#define INA3221_AVERAGING_MASK                  (0x0E00)
#define INA3221_AVERAGING_NONE                  (0x0000)
#define INA3221_AVERAGING_4_SAMPLES             (0x0200)
#define INA3221_AVERAGING_16_SAMPLES            (0x0400)
#define INA3221_AVERAGING_64_SAMPLES            (0x0600)
#define INA3221_AVERAGING_128_SAMPLES           (0x0800)
#define INA3221_AVERAGING_256_SAMPLES           (0x0A00)
#define INA3221_AVERAGING_512_SAMPLES           (0x0C00)  // 1 sample
#define INA3221_AVERAGING_1024_SAMPLES          (0x0E00)

#define INA3221_VBUS_CONV_TIME_MASK             (0x01C0)
#define INA3221_VBUS_CONV_TIME_140US            (0x0000)
#define INA3221_VBUS_CONV_TIME_204US            (0x0040)
#define INA3221_VBUS_CONV_TIME_332US            (0x0080)
#define INA3221_VBUS_CONV_TIME_588US            (0x00C0)
#define INA3221_VBUS_CONV_TIME_1MS              (0x0100)  // 1.1ms
#define INA3221_VBUS_CONV_TIME_2MS              (0x0140)  // 2.116ms
#define INA3221_VBUS_CONV_TIME_4MS              (0x0180)  // 4.156ms
#define INA3221_VBUS_CONV_TIME_8MS              (0x01C0)  // 8.244ms

#define INA3221_SHUNT_CONV_TIME_MASK            (0x0038)
#define INA3221_SHUNT_CONV_TIME_140US           (0x0000)
#define INA3221_SHUNT_CONV_TIME_204US           (0x0008)
#define INA3221_SHUNT_CONV_TIME_332US           (0x0010)
#define INA3221_SHUNT_CONV_TIME_588US           (0x0018)
#define INA3221_SHUNT_CONV_TIME_1MS             (0x0020)  // 1.1ms
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
#define INA3221_MODE_SHUNT_AND_BUS_CONTINOUS    (0x0007)  // default


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
#define INA3221_CONV_READY_FLAG                 (0x0001) // The conversion bit is set after all conversions are
                                                         // complete. Conversion ready clearsReading the Mask/Enable register.

// Other registers
#define INA3221_REG_POWER_VALID_UPPER_LIMIT     (0x10)
#define INA3221_REG_POWER_VALID_LOWER_LIMIT     (0x11)
#define INA3221_REG_MANUFACTURER_ID             (0xFE)
#define INA3221_REG_DIE_ID                      (0xFF)

// Constants for manufacturer and device ID
#define INA3221_MANUFACTURER_ID                 (0x5449)     // "TI"
#define INA3221_DIE_ID                          (0x3220)

// General constants
#define INA3221C_BUS_ADC_LSB                    (0.008f)     // VBus ADC LSB is 8mV
#define INA3221C_SHUNT_ADC_LSB                  (0.00004f)   // VShunt ADC LSB is 40µV
#define INA3221_DEFAULT_SHUNT_RESISTOR          (0.1f)

#define INA3221_ENERGY_FACTOR                   (1.0f/(3600.0f*1000.0f))  // reading values all xx ms

#ifdef INA3221_SUPPLY_SIDE
  #define INA3221_ENABLE_CHAN(device)    (((INA3221_SUPPLY_SIDE >> ((device)*4)) & (0xF >> (4-INA3221_NB_CHAN))) << 4)
#endif

/* show final Defines : 
#ifdef INA3221_CALC_CHARGE_AH
    #define BF_DEF_CHARGE_AH                 defined
#else
    #define BF_DEF_CHARGE_AH                 not defined
#endif
#ifdef INA3221_CALC_ENERGY_WH
    #define BF_DEF_ENERGY_WH                 defined
#else 
    #define BF_DEF_ENERGY_WH                 not defined
#endif
#ifdef INA3221_SUPPLY_SIDE
    #define BF_DEF_SUPPLY_SIDE              defined ...  INA3221_SUPPLY_SIDE
#else
    #define BF_DEF_SUPPLY_SIDE               not defined
#endif
  #pragma message ("\n=========================>"\
    "\n          Sensor XSNS_  = "          BF_TEXT(XSNS_100)" ... I2C-driver = " BF_TEXT(XI2C_72) \
    "\n          first Address = " BF_TEXT(INA3221_ADDRESS1)\
    "\n          init-config = " BF_TEXT(INA3221_CONFIG_INIT)\
    "\n          INA3221_MAX_COUNT = " BF_TEXT(INA3221_MAX_COUNT)\
    "\n          INA3221_CYCLE_COUNT = " BF_TEXT(INA3221_CYCLE_COUNT)\
    "\n          CHARGE_AH = " BF_TEXT(BF_DEF_CHARGE_AH)" ... ENERGY_WH = " BF_TEXT(BF_DEF_ENERGY_WH)\
    "\n          SUPPLY_SIDE = " BF_TEXT(BF_DEF_SUPPLY_SIDE) \
    "\n=========================>\n\n")
// end  show final Defines */

#ifdef DEBUG_TASMOTA_SENSOR
// temporary strings for floating point in debug messages
char _ina3221_dbg1[FLOATSZ];
char _ina3221_dbg2[FLOATSZ];
#endif

const char INA3221_SENSORCMND_START[] PROGMEM = "{\"" D_CMND_SENSOR "%d\":{\"idx\":%d,\"addr\":\"0x%02X\",\"rshunt\":[";
const char INA3221_SENSORCMND_END[] PROGMEM = "]}}";

#if defined(INA3221_CALC_CHARGE_AH) || defined(INA3221_CALC_ENERGY_WH)
uint32_t INA3221_last_millis;
uint32_t INA3221_delta_ms;
#endif

struct INA3221_Channel_Data {
  float     voltage;
  float     current;
  float     shunt;
  #ifdef INA3221_CALC_CHARGE_AH
  float     charge_ah;
  #endif
  #ifdef INA3221_CALC_ENERGY_WH
  float     energy_wh;
  #endif
};

struct INA3221_Data {
  struct INA3221_Channel_Data  chan[INA3221_NB_CHAN];
  uint8_t enabled_chan;
  uint8_t i2caddr;
};

struct INA3221_Data *Ina3221Data = nullptr;
uint8_t Ina3221count = 0;
static int8_t _ina3221_current_device = 0;

#define D_INA3221   "INA3221"
const char INA3221_TYPE[] = D_INA3221;

#ifdef INA3221_SUPPLY_SIDE
bool Ina3221WriteConfig(uint8_t device)
{
	uint16_t config = 0x8000;
    for (uint32_t ch = 0; ch < INA3221_NB_CHAN; ch++) {
      if ((Ina3221Data[device].enabled_chan & (1 << ch)) || (Ina3221Data[device].enabled_chan & (1 << (ch+4)))){
		config |= INA3221_ENABLE_CH(ch);
      }
	}
	config |= INA3221_CONFIG_INIT;
	// bf.. INA3221_MODE_SHUNT_AND_BUS_CONTINOUS ableiten aus Ina3221Data[device].enabled_chan device !!
    if (Ina3221Data[device].enabled_chan < 0x0F){
		config |= INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS;
	}
	else if (Ina3221Data[device].enabled_chan == (Ina3221Data[device].enabled_chan & 0xF0) ){
		config |= INA3221_MODE_BUS_VOLTAGE_CONTINUOUS;
	}
	else{
		config |= INA3221_MODE_SHUNT_AND_BUS_CONTINOUS;
	}
/*
#define INA3221_ENABLE_MASK                     (0x7000)
#define INA3221_ENABLE_CH(ch)                   (0x4000>>(ch))    // default: set
#define INA3221_MODE_SHUNT_VOLTAGE_CONTINUOUS   (0x0005)
#define INA3221_MODE_BUS_VOLTAGE_CONTINUOUS     (0x0006)
#define INA3221_MODE_SHUNT_AND_BUS_CONTINOUS    (0x0007)  // default
*/
				
#ifdef DEBUG_TASMOTA_SENSOR
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ":WriteConfig: device=%d,  addr:0x%02X, onfiguration register=0x%04X"),device+1, Ina3221Data[device].i2caddr, config);
#endif

  // Set Config register
  if (!I2cWrite16(Ina3221Data[device].i2caddr, INA3221_REG_CONFIG, config))
    return false;

//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_INA3221 ":WriteConfig: device=%d,  addr:0x%02X, onfiguration register=0x%04X"),device+1, Ina3221Data[device].i2caddr, config);
  return true;
}
#endif

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
#ifndef INA3221_SUPPLY_SIDE
  uint16_t config = INA3221_ENABLE_MASK |
                    INA3221_CONFIG_INIT |
                    INA3221_MODE_SHUNT_AND_BUS_CONTINOUS;
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ":SetConfig: addr:0x%02X, config=0x%04X"), addr, config);
  // Set Config register
  if (!I2cWrite16(addr, INA3221_REG_CONFIG, config))
    return false;
#endif

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
 #ifdef INA3221_SUPPLY_SIDE
  if (fabs(shunt) > 0.0f){
 #else
  if (shunt > 0.0f){
 #endif
    #if (defined(INA3221_CALC_CHARGE_AH) || defined(INA3221_CALC_ENERGY_WH))
	if (!(Ina3221Data[device].enabled_chan & (1<<channel))){
      #ifdef INA3221_CALC_CHARGE_AH
      Ina3221Data[device].chan[channel].charge_ah = 0.0f;
      #endif
      #ifdef INA3221_CALC_ENERGY_WH
	  if ((Ina3221Data[device].enabled_chan & (1<<(channel+4)))){
        Ina3221Data[device].chan[channel].energy_wh = 0.0f;
      }
      #endif		
	}
    #endif		
    Ina3221Data[device].enabled_chan |= (1<<channel);
  }
  else{
    Ina3221Data[device].enabled_chan &= ~(1<<channel);
    Ina3221Data[device].chan[channel].current = NAN;
    #ifdef INA3221_CALC_CHARGE_AH
    Ina3221Data[device].chan[channel].charge_ah = NAN;
    #endif
    #ifdef INA3221_CALC_ENERGY_WH
    Ina3221Data[device].chan[channel].energy_wh = NAN;
    #endif
  }
//  AddLog(LOG_LEVEL_DEBUG, PSTR(D_INA3221 ":enabled: device/channel=%d/%d,shunt=%-3_f enabled=0x%02X"), device+1,channel,&shunt,Ina3221Data[device].enabled_chan);
}

bool Ina3221Read(uint8_t device, uint8_t channel)
{
//  if (channel>INA3221_NB_CHAN) 
//	  return false;
  uint8_t addr = Ina3221Data[device].i2caddr;
  int16_t bus_voltage, shunt_voltage;
  struct INA3221_Channel_Data *pChannel = &Ina3221Data[device].chan[channel];
  
 #ifdef INA3221_SUPPLY_SIDE
  if (Ina3221Data[device].enabled_chan & (0x01 << (channel+4))){
 #endif
    bus_voltage = I2cReadS16(addr, INA3221_REG_BUS_VOLTAGE_CH(channel));
 #ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(D_INA3221 ":GetBusVoltage: RegVBus[%d:%d](0x%02X) = 0x%04X = %d", device, channel, INA3221_REG_BUS_VOLTAGE_CH(channel), bus_voltage, bus_voltage);
 #endif
    // Convert to VBus voltage in V
    pChannel->voltage = INA3221C_BUS_ADC_LSB * (float)(bus_voltage >> 3);
 #ifdef INA3221_SUPPLY_SIDE
//    AddLog(LOG_LEVEL_DEBUG, PSTR(D_INA3221 ":GetBusVoltage: RegVBus[0x%02X:%d:%d](0x%02X) = 0x%04X = %d voltage=%5_f"),addr ,device, channel, INA3221_REG_BUS_VOLTAGE_CH(channel), bus_voltage, bus_voltage, &pChannel->voltage);
  }
  else{
    pChannel->voltage = NAN;
  }
  if ((fabs(pChannel->shunt)) > 0.0f) {
    shunt_voltage = I2cReadS16(addr, INA3221_REG_SHUNT_VOLTAGE_CH(channel)); 
 #ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(D_INA3221 ":GetShuntVoltage: RegSh[%d:%d](0x%02X) = 0x%04X = %d", device, channel, INA3221_REG_SHUNT_VOLTAGE_CH(channel), shunt_voltage, shunt_voltage);
 #endif
    // convert to shunt voltage in V
	if (pChannel->shunt < 0){
      pChannel->voltage += INA3221C_SHUNT_ADC_LSB * (float)(shunt_voltage >> 3);
	}
  pChannel->current = INA3221C_SHUNT_ADC_LSB * (float)(shunt_voltage >> 3) / (fabs(pChannel->shunt));
//    AddLog(LOG_LEVEL_DEBUG, PSTR(D_INA3221 ":GetShuntVoltage: RegSh[%d:%d](0x%02X) = 0x%04X = %d current=%5_f"),device, channel, INA3221_REG_SHUNT_VOLTAGE_CH(channel), shunt_voltage, shunt_voltage, &pChannel->current);
 #else
  if (pChannel->shunt > 0.0f) {
    shunt_voltage = I2cReadS16(addr, INA3221_REG_SHUNT_VOLTAGE_CH(channel));
 #ifdef DEBUG_TASMOTA_SENSOR
    DEBUG_SENSOR_LOG(D_INA3221 ":GetShuntVoltage: RegSh[%d:%d](0x%02X) = 0x%04X = %d", device, channel, INA3221_REG_SHUNT_VOLTAGE_CH(channel), shunt_voltage, shunt_voltage);
 #endif
    // convert to shunt voltage in V
    pChannel->current = INA3221C_SHUNT_ADC_LSB * (float)(shunt_voltage >> 3) / pChannel->shunt;
 #endif
    #ifdef INA3221_CALC_CHARGE_AH
    pChannel->charge_ah += (pChannel->current * (float)INA3221_delta_ms * INA3221_ENERGY_FACTOR);
    #endif
    #ifdef INA3221_CALC_ENERGY_WH
    pChannel->energy_wh += (pChannel->current * pChannel->voltage * (float)INA3221_delta_ms * INA3221_ENERGY_FACTOR);
    #endif
  } 
  else {
	pChannel->current = INFINITY;
    #ifdef INA3221_CALC_CHARGE_AH
    pChannel->charge_ah = INFINITY;
    #endif
    #ifdef INA3221_CALC_ENERGY_WH
    pChannel->energy_wh = INFINITY;
    #endif 
  }

  #ifdef DEBUG_TASMOTA_SENSOR
  dtostrfd(pChannel->voltage,5,_ina3221_dbg1);
  dtostrfd(pChannel->current,5,_ina3221_dbg2);
  #ifdef DEBUG_TASMOTA_SENSOR
  DEBUG_SENSOR_LOG(D_INA3221 ":Read[%d:%d]: V=%sV, I=%sA", device, channel, _ina3221_dbg1, _ina3221_dbg2);
  #endif
 #endif

  return true;
}

/*********************************************************************************************\
 * Command Sensor
\*********************************************************************************************/

bool Ina3221CmndSensor(void)
{
  int argc = ArgC();
  if(argc != 1 && argc != (INA3221_NB_CHAN+1)) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_INA3221 ": Not enough arguments (1 or %d)"),(INA3221_NB_CHAN+1));
    return false;
  }

  char argument[XdrvMailbox.data_len+FLOATSZ];
  uint32_t device = atoi(ArgV(argument,1)) -1;
  if (device >= INA3221_MAX_COUNT || !Ina3221Data[device].i2caddr) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_INA3221 ":invalid device %d"),device+1);
    return false;
  }
  if (argc > 1) {
    for (int channel = 0 ; channel < INA3221_NB_CHAN ; channel++) {
      float shunt = CharToFloat(ArgV(argument,2+channel));
      Ina3221SetShunt(device, channel, shunt);
    }
#ifdef INA3221_SUPPLY_SIDE
    if (!Ina3221WriteConfig(device)){
      #ifdef DEBUG_TASMOTA_SENSOR
      DEBUG_SENSOR_LOG(D_INA3221 "error write configuration %d", device+1);
      #endif
      return false;
	}
#endif
  }
  Response_P(INA3221_SENSORCMND_START, XSNS_100, device +1, Ina3221Data[device].i2caddr);
  for (int channel = 0 ; channel < INA3221_NB_CHAN ; channel++ ) {
    ResponseAppend_P(PSTR("%5_f%c"),&Ina3221Data[device].chan[channel].shunt , ((channel < (INA3221_NB_CHAN-1))?',':'\0'));
  }
  ResponseAppend_P(INA3221_SENSORCMND_END);

  return true;
}

/********************************************************************************************/

void Ina3221Detect(void)
{
  _ina3221_current_device = 0;
  Ina3221count = 0;
//  ????
  for (uint32_t i = 0; i < INA3221_MAX_COUNT; i++) {
    uint16_t addr = INA3221_ADDRESS(i);
    if (!I2cSetDevice(addr)) { continue; }
    if (!Ina3221Data) {
// bf ... calloc(INA3221_MAX_COUNT ... ??
	Ina3221Data = (struct INA3221_Data*)calloc(INA3221_MAX_COUNT,sizeof(struct INA3221_Data));
      if (!Ina3221Data) {
        AddLog(LOG_LEVEL_ERROR,PSTR(D_INA3221 ": Mem allocation error"));
        return;
      }
    }
	// bf.. Ina3221SetConfig(addr)) erweitern om device !!
      if (Ina3221SetConfig(addr)) {
      I2cSetActiveFound(addr, INA3221_TYPE);
      Ina3221Data[Ina3221count].i2caddr = addr;
      #ifdef INA3221_SUPPLY_SIDE
      Ina3221Data[Ina3221count].enabled_chan = INA3221_ENABLE_CHAN(i);
      if (!Ina3221WriteConfig(Ina3221count)){
        Ina3221count++;
		continue;
	  }
      #else
      Ina3221Data[Ina3221count].enabled_chan = 0;
	  #endif
      for (uint32_t j = 0; j < INA3221_NB_CHAN; j++) {
        Ina3221Data[Ina3221count].chan[j].shunt = 0.0f;
        #ifdef INA3221_CALC_CHARGE_AH
        Ina3221Data[Ina3221count].chan[j].charge_ah = 0.0f;
        #endif
        #ifdef INA3221_CALC_ENERGY_WH
        Ina3221Data[Ina3221count].chan[j].energy_wh = 0.0f;
        #endif
      }
      Ina3221count++;
    }
  }
  if (!Ina3221count && Ina3221Data) {
    free(Ina3221Data);
    Ina3221Data = nullptr;
  }
  #if defined(INA3221_CALC_CHARGE_AH) || defined(INA3221_CALC_ENERGY_WH)
  INA3221_last_millis = millis();
  #endif
}

void Ina3221Every250ms(void)
{
  if (++_ina3221_current_device < 0 ){
		return;
  }
  // Conversion-ready flag. CVRF ?? ==>  INA3221_CONV_READY_FLAG
  
  #ifdef DEBUG_TASMOTA_SENSOR
  DEBUG_SENSOR_LOG(PSTR(D_INA3221 ": cur:%d, en:%d"), _ina3221_current_device, Ina3221Data[(uint8_t) _ina3221_current_device].enabled_chan);
  #endif
  uint8_t enabled_chan = Ina3221Data[(uint8_t)(_ina3221_current_device)].enabled_chan;

  if (_ina3221_current_device >= INA3221_MAX_COUNT){
    _ina3221_current_device = (-INA3221_CYCLE_COUNT);
     #if defined(INA3221_CALC_CHARGE_AH) || defined(INA3221_CALC_ENERGY_WH)
     INA3221_delta_ms = millis()-INA3221_last_millis;
     INA3221_last_millis = millis();
     #endif
   }
   else{
     for (int chan = 0 ; enabled_chan ; chan++, enabled_chan>>=1, enabled_chan &= 0xF7) {
       if (0x11 & enabled_chan)
          Ina3221Read((uint8_t)(_ina3221_current_device), chan);
       }
   }
}

#ifdef USE_WEBSERVER
// {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#define INA3221_AL      "<td style='text-align:right'>"
const char HTTP_SNS_INA3221_HEADER[] PROGMEM =
  "{s}" D_INA3221 "&nbsp;&nbsp;&nbsp;&nbsp;</th>" INA3221_AL D_VOLTAGE " </td><td>&nbsp;</td>" INA3221_AL D_CURRENT " </td><td>&nbsp;</td>" INA3221_AL D_POWERUSAGE
    #ifdef INA3221_CALC_CHARGE_AH
    "</td><td>&nbsp;</td>" INA3221_AL D_CHARGE
    #endif
    #ifdef INA3221_CALC_ENERGY_WH
    "</td><td>&nbsp;</td>" INA3221_AL D_ENERGY
    #endif
  "{e}";

 const char HTTP_SNS_INA3221_DATA[] PROGMEM =
 "{s}%s </th>" INA3221_AL " %*_f " D_UNIT_VOLT " </td><td>&nbsp;</td>" INA3221_AL " %*_f " D_UNIT_AMPERE " </td><td>&nbsp;</td>" INA3221_AL " %*_f " D_UNIT_WATT
    #ifdef INA3221_CALC_CHARGE_AH
    "</td><td>&nbsp;</td>" INA3221_AL " %*_f " D_UNIT_CHARGE
    #endif
    #ifdef INA3221_CALC_ENERGY_WH
    "</td><td>&nbsp;</td>" INA3221_AL " %*_f " D_UNIT_WATTHOUR
    #endif
  "{e}";
  #endif  // USE_WEBSERVER

void Ina3221Show(bool json)
{
  char name[FLOATSZ];
//  char temp[FLOATSZ];
  char voltage[INA3221_NB_CHAN*FLOATSZ+3];
  char current[INA3221_NB_CHAN*FLOATSZ+3];
  char power[INA3221_NB_CHAN*FLOATSZ+3];
  #ifdef INA3221_CALC_CHARGE_AH
  char charge_ah[INA3221_NB_CHAN*FLOATSZ+3];
  #endif
  #ifdef INA3221_CALC_ENERGY_WH
  char energy_wh[INA3221_NB_CHAN*FLOATSZ+3];
  #endif
  float pw = 0.0f;
  
  if (json) {
    // data
    for (int device=0 ; device < Ina3221count ; device++) {
      uint8_t enabled_chan = Ina3221Data[device].enabled_chan;
      if (!enabled_chan) continue;

      if (Ina3221count > 1){
        snprintf_P(name, sizeof(name), PSTR("%s%c%d"), INA3221_TYPE, IndexSeparator(), device +1);
      }
      else{
        snprintf_P(name, sizeof(name), PSTR("%s"), INA3221_TYPE);
       }
	  int32_t count_v = 0, count_i = 0, count_p = 0, count_ah = 0, count_wh = 0;
      for (int32_t chan=0 ; enabled_chan; chan++, enabled_chan>>=1, enabled_chan &= 0xF7) {
//        if (0x11 & enabled_chan) {
          count_v += ext_snprintf_P(&voltage[count_v], sizeof(voltage) - count_v, PSTR("%s%*_f"), (chan>0 ? ",":""), Settings->flag2.voltage_resolution, &Ina3221Data[device].chan[chan].voltage);
          count_i += ext_snprintf_P(&current[count_i], sizeof(current) - count_i, PSTR("%s%*_f"), (chan>0 ? ",":""), Settings->flag2.current_resolution, &Ina3221Data[device].chan[chan].current);
          pw = Ina3221Data[device].chan[chan].voltage * Ina3221Data[device].chan[chan].current;
          count_p += ext_snprintf_P(&power[count_p], sizeof(power) - count_p, PSTR("%s%*_f"), (chan>0 ? ",":""), Settings->flag2.wattage_resolution, &pw);
          #ifdef INA3221_CALC_CHARGE_AH
          count_ah += ext_snprintf_P(&charge_ah[count_ah], sizeof(charge_ah) - count_ah, PSTR("%s%*_f"), (chan>0 ? ",":""), Settings->flag2.energy_resolution, &Ina3221Data[device].chan[chan].charge_ah);
          #endif
          #ifdef INA3221_CALC_ENERGY_WH
          count_wh += ext_snprintf_P(&energy_wh[count_wh], sizeof(energy_wh) - count_wh, PSTR("%s%*_f"), (chan>0 ? ",":""), Settings->flag2.energy_resolution, &Ina3221Data[device].chan[chan].energy_wh);
          #endif
//       }  //if enabled
      } // for channel	  
      ResponseAppend_P(PSTR(",\"%s\":{\"Id\":\"0x%02x\",\"" D_JSON_VOLTAGE "\":[%s],\"" D_JSON_CURRENT "\":[%s],\"" D_JSON_POWERUSAGE "\":[%s]"
      #ifdef INA3221_CALC_CHARGE_AH
      ",\"" D_JSON_CHARGE "\":[%s]"
      #endif
      #ifdef INA3221_CALC_ENERGY_WH
      ",\"" D_JSON_ENERGY "\":[%s]"
      #endif
      "}"),name, Ina3221Data[device].i2caddr, voltage, current, power
      #ifdef INA3221_CALC_CHARGE_AH
      , charge_ah
      #endif
      #ifdef INA3221_CALC_ENERGY_WH
      , energy_wh
      #endif
      );
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
      for (int chan=0 ; enabled_chan ; chan++, enabled_chan>>=1, enabled_chan &= 0xF7) {
        if (0x11 & enabled_chan) {
          if (Ina3221count > 1){
            snprintf_P(name, sizeof(name), PSTR("%s%c%d:%d"), INA3221_TYPE, IndexSeparator(), device +1, chan);
          }
		  else{
            snprintf_P(name, sizeof(name), PSTR("%s:%d"), INA3221_TYPE, chan);
          } 
          pw = Ina3221Data[device].chan[chan].voltage * Ina3221Data[device].chan[chan].current;
		  WSContentSend_PD(HTTP_SNS_INA3221_DATA, name, Settings->flag2.voltage_resolution, &Ina3221Data[device].chan[chan].voltage, Settings->flag2.current_resolution, &Ina3221Data[device].chan[chan].current, Settings->flag2.wattage_resolution, &pw
            #ifdef INA3221_CALC_CHARGE_AH
            , Settings->flag2.energy_resolution, &Ina3221Data[device].chan[chan].charge_ah
            #endif
            #ifdef INA3221_CALC_ENERGY_WH
            , Settings->flag2.energy_resolution, &Ina3221Data[device].chan[chan].energy_wh
            #endif
          );
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
