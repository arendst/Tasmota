/*
  xsns_05_ds18x20.ino - DS18x20 temperature sensor support for Tasmota

  Copyright (C) 2021  Theo Arends

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

#ifdef ESP8266
#ifdef USE_DS18x20
/*********************************************************************************************\
 * DS18B20 - Temperature - Multiple sensors
\*********************************************************************************************/

#define XSNS_05              5

//#define USE_DS18x20_RECONFIGURE    // When sensor is lost keep retrying or re-configure
//#define DS18x20_USE_ID_AS_NAME      // Use last 3 bytes for naming of sensors

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit
#define DS2413_CHIPID        0x3A  // GPIO 2bit Port Open Collector

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_WRITE_EEPROM      0x48
#define W1_WRITE_SCRATCHPAD  0x4E
#define W1_READ_SCRATCHPAD   0xBE
#define DS2413_ACCESS_READ   0xF5
#define DS2413_ACCESS_WRITE  0x5A
#define DS2413_ACK_SUCCESS   0xAA
#define DS2413_ACK_ERROR     0xFF
#define DS2413_WRITE_MASK    0xFC
#define DS2413_READ_MASK     0x0F   // PIOstate
#define DS2413_PORTA_OUT     0b0010 // PIOstate
#define DS2413_PORTB_OUT     0b1000
#define DS2413_OUTPUT_MASK   (DS2413_PORTB_OUT | DS2413_PORTA_OUT)
#define DS2413_PORTA_IN      0b0001 // PIOstate
#define DS2413_PORTB_IN      0b0100
#define DS2413_INPUT_MASK    (DS2413_PORTB_IN | DS2413_PORTA_IN)
#define DS2413_ERROR_BIT     0x80

#ifndef DS18X20_MAX_SENSORS // DS18X20_MAX_SENSORS fallback to 8 if not defined in user_config_override.h
#define DS18X20_MAX_SENSORS  8
#endif

#ifndef DS18x20_ADD_DS2413
const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";
uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };
#else
const char kDs18x20Types[] PROGMEM = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850|DS_2413";
uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID, DS2413_CHIPID };
#ifdef USE_WEBSERVER
const char HTTP_SNS_PORT[] PROGMEM = "{s}%s PIO Status{m}%s {e}";
#endif // USE_WEBSERVER
#endif // DS18x20_ADD_DS2413

struct {
  union {
    float temperature;            // DS18x20
    struct {                      // DS2413
      union {
        uint16_t  pub_pioState;   // publish PIOstate and change bits
        struct {
          uint8_t pioState;       // DS2413 PIOstate
          uint8_t change_bits;    // changed PIOstate and ERROR_BIT
        };
      };
      uint8_t pre_pioState;       // prepare inputs change (debounce)
      uint8_t pwr_index;          // Tasmota RelayNumber and RelayNumber+1
    };
  };
  uint8_t address[8];             // Note: address must be word aligned !!!
  float temp_sum;
  uint16_t numread;
  uint8_t index;
  uint8_t valid;
} ds18x20_sensor[DS18X20_MAX_SENSORS];

struct {
#ifdef W1_PARASITE_POWER
  uint32_t w1_power_until = 0;
  uint8_t current_sensor = 0;
#endif
  char name[17];
  uint8_t sensors = 0;
#ifdef DS18x20_ADD_DS2413
  uint8_t ds2413_sensors = 0;
#endif // DS18x20_ADD_DS2413
  uint8_t input_mode = 0;    // INPUT or INPUT_PULLUP (=2)
  int8_t pin = 0;            // Shelly GPIO3 input only
  int8_t pin_out = 0;        // Shelly GPIO00 output only
  bool dual_mode = false;    // Single pin mode
} DS18X20Data;

/*********************************************************************************************\
 * Embedded tuned OneWire library
\*********************************************************************************************/

#define W1_MATCH_ROM         0x55
#define W1_SEARCH_ROM        0xF0

uint8_t onewire_last_discrepancy = 0;
uint8_t onewire_last_family_discrepancy = 0;
bool onewire_last_device_flag = false;
unsigned char onewire_rom_id[8] = { 0 };

/*------------------------------------------------------------------------------------------*/

uint8_t OneWireReset(void) {
  uint8_t retries = 125;

  if (!DS18X20Data.dual_mode) {
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    pinMode(DS18X20Data.pin, OUTPUT);
    digitalWrite(DS18X20Data.pin, LOW);
    delayMicroseconds(480);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  } else {
    digitalWrite(DS18X20Data.pin_out, HIGH);
    do {
      if (--retries == 0) {
        return 0;
      }
      delayMicroseconds(2);
    } while (!digitalRead(DS18X20Data.pin));
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(480);
    digitalWrite(DS18X20Data.pin_out, HIGH);
    delayMicroseconds(70);
    uint8_t r = !digitalRead(DS18X20Data.pin);
    delayMicroseconds(410);
    return r;
  }
}

void OneWireWriteBit(uint8_t v) {
  static const uint8_t delay_low[2] = { 65, 10 };
  static const uint8_t delay_high[2] = { 5, 55 };

  v &= 1;
  if (!DS18X20Data.dual_mode) {
    digitalWrite(DS18X20Data.pin, LOW);
    pinMode(DS18X20Data.pin, OUTPUT);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin, HIGH);
  } else {
    digitalWrite(DS18X20Data.pin_out, LOW);
    delayMicroseconds(delay_low[v]);
    digitalWrite(DS18X20Data.pin_out, HIGH);
  }
  delayMicroseconds(delay_high[v]);
}

uint8_t OneWire1ReadBit(void) {
  pinMode(DS18X20Data.pin, OUTPUT);
  digitalWrite(DS18X20Data.pin, LOW);
  delayMicroseconds(3);
  pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

uint8_t OneWire2ReadBit(void) {
  digitalWrite(DS18X20Data.pin_out, LOW);
  delayMicroseconds(3);
  digitalWrite(DS18X20Data.pin_out, HIGH);
  delayMicroseconds(10);
  uint8_t r = digitalRead(DS18X20Data.pin);
  delayMicroseconds(53);
  return r;
}

/*------------------------------------------------------------------------------------------*/

void OneWireWrite(uint8_t v) {
  for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
    OneWireWriteBit((bit_mask & v) ? 1 : 0);
  }
}

uint8_t OneWireRead(void) {
  uint8_t r = 0;

  if (!DS18X20Data.dual_mode) {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire1ReadBit()) {
        r |= bit_mask;
      }
    }
  } else {
    for (uint8_t bit_mask = 0x01; bit_mask; bit_mask <<= 1) {
      if (OneWire2ReadBit()) {
        r |= bit_mask;
      }
    }
  }
  return r;
}

void OneWireSelect(const uint8_t rom[8]) {
  OneWireWrite(W1_MATCH_ROM);
  for (uint32_t i = 0; i < 8; i++) {
    OneWireWrite(rom[i]);
  }
}

uint8_t OneWireSearch(uint8_t *newAddr) {
  uint8_t id_bit_number = 1;
  uint8_t last_zero = 0;
  uint8_t rom_byte_number = 0;
  uint8_t search_result = 0;
  uint8_t id_bit;
  uint8_t cmp_id_bit;
  unsigned char rom_byte_mask = 1;
  unsigned char search_direction;

  if (!onewire_last_device_flag) {
    if (!OneWireReset()) {
      onewire_last_discrepancy = 0;
      onewire_last_device_flag = false;
      onewire_last_family_discrepancy = 0;
      return false;
    }
    OneWireWrite(W1_SEARCH_ROM);
    do {
      if (!DS18X20Data.dual_mode) {
        id_bit     = OneWire1ReadBit();
        cmp_id_bit = OneWire1ReadBit();
      } else {
        id_bit     = OneWire2ReadBit();
        cmp_id_bit = OneWire2ReadBit();
      }
      if ((id_bit == 1) && (cmp_id_bit == 1)) {
        break;
      } else {
        if (id_bit != cmp_id_bit) {
          search_direction = id_bit;
        } else {
          if (id_bit_number < onewire_last_discrepancy) {
            search_direction = ((onewire_rom_id[rom_byte_number] & rom_byte_mask) > 0);
          } else {
            search_direction = (id_bit_number == onewire_last_discrepancy);
          }
          if (search_direction == 0) {
            last_zero = id_bit_number;
            if (last_zero < 9) {
              onewire_last_family_discrepancy = last_zero;
            }
          }
        }
        if (search_direction == 1) {
          onewire_rom_id[rom_byte_number] |= rom_byte_mask;
        } else {
          onewire_rom_id[rom_byte_number] &= ~rom_byte_mask;
        }
        OneWireWriteBit(search_direction);
        id_bit_number++;
        rom_byte_mask <<= 1;
        if (rom_byte_mask == 0) {
          rom_byte_number++;
          rom_byte_mask = 1;
        }
      }
    } while (rom_byte_number < 8);
    if (!(id_bit_number < 65)) {
      onewire_last_discrepancy = last_zero;
      if (onewire_last_discrepancy == 0) {
        onewire_last_device_flag = true;
      }
      search_result = true;
    }
  }
  if (!search_result || !onewire_rom_id[0]) {
    onewire_last_discrepancy = 0;
    onewire_last_device_flag = false;
    onewire_last_family_discrepancy = 0;
    search_result = false;
  }
  for (uint32_t i = 0; i < 8; i++) {
    newAddr[i] = onewire_rom_id[i];
  }
  return search_result;
}

bool OneWireCrc8(uint8_t *addr) {
  uint8_t crc = 0;
  uint8_t len = 8;

  while (len--) {
    uint8_t inbyte = *addr++;          // from 0 to 7
    for (uint32_t i = 8; i; i--) {
      uint8_t mix = (crc ^ inbyte) & 0x01;
      crc >>= 1;
      if (mix) {
        crc ^= 0x8C;
      }
      inbyte >>= 1;
    }
  }
  return (crc == *addr);               // addr 8
}

/********************************************************************************************/

void Ds18x20Init(void) {
  DS18X20Data.pin = Pin(GPIO_DSB);
  DS18X20Data.input_mode = Settings->flag3.ds18x20_internal_pullup ? INPUT_PULLUP : INPUT;  // SetOption74 - Enable internal pullup for single DS18x20 sensor

  if (PinUsed(GPIO_DSB_OUT)) {
    DS18X20Data.pin_out = Pin(GPIO_DSB_OUT);
    DS18X20Data.dual_mode = true;    // Dual pins mode as used by Shelly
    pinMode(DS18X20Data.pin_out, OUTPUT);
    pinMode(DS18X20Data.pin, DS18X20Data.input_mode);
  }

  onewire_last_discrepancy = 0;
  onewire_last_device_flag = false;
  onewire_last_family_discrepancy = 0;
  for (uint32_t i = 0; i < 8; i++) {
    onewire_rom_id[i] = 0;
  }

  uint64_t ids[DS18X20_MAX_SENSORS];
  DS18X20Data.sensors = 0;
  while (DS18X20Data.sensors < DS18X20_MAX_SENSORS) {
    if (!OneWireSearch(ds18x20_sensor[DS18X20Data.sensors].address)) {
      break;
    }
    if (OneWireCrc8(ds18x20_sensor[DS18X20Data.sensors].address) &&
       ((ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18S20_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS1822_CHIPID) ||
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS18B20_CHIPID) ||
#ifdef DS18x20_ADD_DS2413
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == DS2413_CHIPID) ||
#endif
        (ds18x20_sensor[DS18X20Data.sensors].address[0] == MAX31850_CHIPID))) {
      ds18x20_sensor[DS18X20Data.sensors].index = DS18X20Data.sensors;
      ids[DS18X20Data.sensors] = ds18x20_sensor[DS18X20Data.sensors].address[0];  // Chip id
      for (uint32_t j = 6; j > 0; j--) {
        ids[DS18X20Data.sensors] = ids[DS18X20Data.sensors] << 8 | ds18x20_sensor[DS18X20Data.sensors].address[j];
      }
      DS18X20Data.sensors++;
    }
  }
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
// AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB "1wire DeviceAddress 0x%_X"), ds18x20_sensor[i].address );
    for (uint32_t j = i + 1; j < DS18X20Data.sensors; j++) {
#ifndef DS18x20_ADD_DS2413
      if (ids[ds18x20_sensor[i].index] > ids[ds18x20_sensor[j].index]) {  // Sort ascending
#else
      if ( ((ids[ds18x20_sensor[i].index] > ids[ds18x20_sensor[j].index]) &&
            ((ds18x20_sensor[ds18x20_sensor[i].index].address[0] != DS18B20_CHIPID) ||
             (ds18x20_sensor[ds18x20_sensor[j].index].address[0] == DS18B20_CHIPID))) ||
            ((ds18x20_sensor[ds18x20_sensor[i].index].address[0] != DS18B20_CHIPID) &&
             (ds18x20_sensor[ds18x20_sensor[j].index].address[0] == DS18B20_CHIPID)) ) {
#endif
        std::swap(ds18x20_sensor[i].index, ds18x20_sensor[j].index);
      }
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSORS_FOUND " %d"), DS18X20Data.sensors);
#ifdef DS18x20_ADD_DS2413
  TasmotaGlobal.devices_present -= (DS18X20Data.ds2413_sensors *2);
            // reset no of devices to avoid duplicate ports on duplicate init
  DS18X20Data.ds2413_sensors = 0;
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    if (ds18x20_sensor[i].address[0] == DS2413_CHIPID) {
      ds18x20_sensor[i].pwr_index = TasmotaGlobal.devices_present;
      if (Ds18x20Read(i)) {                             // restore Powerstate
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: Initial pioState %02x, GlobalPower %_X, SettingsPower %_X"), ds18x20_sensor[i].pioState, &TasmotaGlobal.power, &Settings.power);
        if (Settings.flag.save_state) {       // SetOption0 - Save power state
          bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present,
                    bitRead(Settings.power, TasmotaGlobal.devices_present));
          bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present +1,
                    bitRead(Settings.power, TasmotaGlobal.devices_present+1));
        } else if (!Settings.flag3.no_power_feedback) {         // SetOption63
          bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present,
                      (ds18x20_sensor[i].pioState & DS2413_PORTA_OUT)>>1);
          bitWrite(TasmotaGlobal.power, TasmotaGlobal.devices_present+1,
                      (ds18x20_sensor[i].pioState & DS2413_PORTB_OUT)>>3);
        }
        DS2413write(i, TasmotaGlobal.power);
      }
      TasmotaGlobal.devices_present += 2;	          // 2 Outputs (+ 2 Inputs)
      DS18X20Data.ds2413_sensors++;
    }
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("PIO: DS2413 GPIO devices %d"),
                                                  DS18X20Data.ds2413_sensors);
#endif
}

void Ds18x20Convert(void) {
  OneWireReset();
#ifdef W1_PARASITE_POWER
  // With parasite power address one sensor at a time
  if (++DS18X20Data.current_sensor >= DS18X20Data.sensors)
    DS18X20Data.current_sensor = 0;
  OneWireSelect(ds18x20_sensor[DS18X20Data.current_sensor].address);
#else
  OneWireWrite(W1_SKIP_ROM);           // Address all Sensors on Bus
#endif
  OneWireWrite(W1_CONVERT_TEMP);       // start conversion, no parasite power on at the end
//  delay(750);                          // 750ms should be enough for 12bit conv
}

bool Ds18x20Read(uint8_t sensor) {
  float temperature;
  union {
    uint8_t data[9];
    struct pioState {
      uint8_t read;
      uint8_t changed;
    } sPioState;
  };
  int8_t sign = 1;

  uint8_t index = ds18x20_sensor[sensor].index;
  if (ds18x20_sensor[index].valid) { ds18x20_sensor[index].valid--; }
  for (uint32_t retry = 0; retry < 3; retry++) {
    OneWireReset();
    OneWireSelect(ds18x20_sensor[index].address);
#ifdef DS18x20_ADD_DS2413
    if (ds18x20_sensor[index].address[0] != DS2413_CHIPID) {
#endif
      OneWireWrite(W1_READ_SCRATCHPAD);
      for (uint32_t i = 0; i < 9; i++) {
        data[i] = OneWireRead();
      }
      if (OneWireCrc8(data)) {
        switch(ds18x20_sensor[index].address[0]) {
          case DS18S20_CHIPID: {
            int16_t tempS = (((data[1] << 8) | (data[0] & 0xFE)) << 3) | ((0x10 - data[6]) & 0x0F);
            temperature = ConvertTemp(tempS * 0.0625 - 0.250);
            break;
          }
          case DS1822_CHIPID:
          case DS18B20_CHIPID: {
            if (data[4] != 0x7F) {
              data[4] = 0x7F;                 // Set resolution to 12-bit
              OneWireReset();
              OneWireSelect(ds18x20_sensor[index].address);
              OneWireWrite(W1_WRITE_SCRATCHPAD);
              OneWireWrite(data[2]);          // Th Register
              OneWireWrite(data[3]);          // Tl Register
              OneWireWrite(data[4]);          // Configuration Register
              OneWireSelect(ds18x20_sensor[index].address);
              OneWireWrite(W1_WRITE_EEPROM);  // Save scratchpad to EEPROM
#ifdef W1_PARASITE_POWER
              DS18X20Data.w1_power_until = millis() + 10; // 10ms specified duration for EEPROM write
#endif
            }
            uint16_t temp12 = (data[1] << 8) + data[0];
            if (temp12 > 2047) {
              temp12 = (~temp12) +1;
              sign = -1;
            }
            temperature = ConvertTemp(sign * temp12 * 0.0625);  // Divide by 16
            break;
          }
          case MAX31850_CHIPID: {
            int16_t temp14 = (data[1] << 8) + (data[0] & 0xFC);
            temperature = ConvertTemp(temp14 * 0.0625);  // Divide by 16
            break;
          }
        }
        ds18x20_sensor[index].temperature = temperature;
        if (Settings.flag5.ds18x20_mean) {
          if (ds18x20_sensor[index].numread++ == 0) {
            ds18x20_sensor[index].temp_sum = 0;
          }
          ds18x20_sensor[index].temp_sum += temperature;
        }
        ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
        return true;
      }
<<<<<<< HEAD
#ifdef DS18x20_ADD_DS2413
    } else {                                                    // read DS2413
      OneWireWrite(DS2413_ACCESS_READ);
      sPioState.read = OneWireRead();
      if ((sPioState.read & DS2413_READ_MASK) ==
                                (((~sPioState.read)>>4) & DS2413_READ_MASK)){
                                            // valid read, update output bits
        ds18x20_sensor[index].pioState &= ~DS2413_OUTPUT_MASK;
        ds18x20_sensor[index].pioState |=(sPioState.read &DS2413_OUTPUT_MASK);
                                  // debounce input bits, 1st look for changes
        sPioState.changed = DS2413_INPUT_MASK &
                            (sPioState.read ^ ds18x20_sensor[index].pioState);
        if (sPioState.changed) {
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: Ds2413CheckInput pio %02x read %01x diff %02x"), ds18x20_sensor[index].pioState, sPioState.read, sPioState.changed);
          ds18x20_sensor[index].change_bits |= sPioState.changed;
          for (uint8_t i=0; i<4; i+=2) {    // process each input bit position
            if (bitRead(sPioState.changed, i)) {          // propagate change
              if(bitRead(ds18x20_sensor[index].pre_pioState, i) !=
                                                bitRead(sPioState.read, i)) {
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: Ds2413CheckInputDebounce bit %d changed %02x"), i>>1, ds18x20_sensor[index].pre_pioState);
                bitWrite(ds18x20_sensor[index].pre_pioState, i,
                                                  bitRead(sPioState.read, i));
              } else {                      // bit stable, send MQTT bit state
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: Ds2413CheckInputDebounce bit %d valid %02x"), i>>1, ds18x20_sensor[index].pre_pioState);
                bitWrite(ds18x20_sensor[index].pioState, i,
                                                  bitRead(sPioState.read, i));
                ResponseTime_P(PSTR(",\"DS2413%c%d_INP\":{\"D%i\":%i}}"),
                                  IndexSeparator(), index+1, i>>1,
                                  bitRead(ds18x20_sensor[index].pioState,i));
                MqttPublishPrefixTopicRulesProcess_P(RESULT_OR_STAT,
                                                          PSTR("DS2413_INP"));
                if (Settings.flag3.hass_tele_on_power) {        // SetOption59
                  MqttPublishSensor();              // add tele/%topic%/SENSOR
                }
              }
            }
          }
        } else {                                    // clear any input bounces
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: Ds2413CheckInput Reset Stable State %2x"), ds18x20_sensor[index]. );
          ds18x20_sensor[index].pre_pioState &= ~DS2413_INPUT_MASK;
          ds18x20_sensor[index].pre_pioState |=
                        (ds18x20_sensor[index].pioState & DS2413_INPUT_MASK);
=======
      ds18x20_sensor[index].temperature = temperature;
      if (Settings->flag5.ds18x20_mean) {
        if (ds18x20_sensor[index].numread++ == 0) {
          ds18x20_sensor[index].temp_sum = 0;
>>>>>>> development
        }
        ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
        return true;
      }
      ds18x20_sensor[index].change_bits |= DS2413_ERROR_BIT;
    }
#endif
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_DSB D_SENSOR_CRC_ERROR));
  return false;
}

void Ds18x20Name(uint8_t sensor) {
  uint8_t index = sizeof(ds18x20_chipids);
  while (--index) {
    if (ds18x20_sensor[ds18x20_sensor[sensor].index].address[0] == ds18x20_chipids[index]) {
      break;
    }
  }
  GetTextIndexed(DS18X20Data.name, sizeof(DS18X20Data.name), index, kDs18x20Types);
  if (DS18X20Data.sensors > 1) {
#ifdef DS18x20_USE_ID_AS_NAME
    char address[17];
    for (uint32_t j = 0; j < 3; j++) {
      sprintf(address+2*j, "%02X", ds18x20_sensor[ds18x20_sensor[sensor].index].address[3-j]);  // Only last 3 bytes
    }
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%s"), DS18X20Data.name, IndexSeparator(), address);
#else
    snprintf_P(DS18X20Data.name, sizeof(DS18X20Data.name), PSTR("%s%c%d"), DS18X20Data.name, IndexSeparator(), sensor +1);
#endif
  }
}


#ifdef DS18x20_ADD_DS2413
bool DS2413write(int index, uint32_t power) {
  uint8_t temp = bitRead(power, ds18x20_sensor[index].pwr_index)<<1 |
                          bitRead(power, ds18x20_sensor[index].pwr_index +1)<<3;
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: DS2413-%d Write Portbits 0x%02x"), index+1, temp);
  temp ^= (ds18x20_sensor[index].pioState & DS2413_OUTPUT_MASK);
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: DS2413-%d Write Portchange 0x%02x"), index+1, temp);
  if (temp == 0) {
    return true;
  }
  ds18x20_sensor[index].change_bits |= temp;
  if (ds18x20_sensor[index].valid) { ds18x20_sensor[index].valid--; }
  temp = bitRead(power, ds18x20_sensor[index].pwr_index) |
                      (bitRead(power, ds18x20_sensor[index].pwr_index +1)<<1);
  OneWireReset();
  OneWireSelect(ds18x20_sensor[index].address);
  OneWireWrite(DS2413_ACCESS_WRITE);
  temp |= DS2413_WRITE_MASK;
  OneWireWrite(temp);
  OneWireWrite(~temp);
  temp = OneWireRead();                /* 0xAA=success, 0xFF=failure */
  if (temp == DS2413_ACK_SUCCESS) {
    temp = OneWireRead();
    if ((temp & DS2413_READ_MASK) == (((~temp)>>4) & DS2413_READ_MASK)) {
      ds18x20_sensor[index].pioState &= ~DS2413_OUTPUT_MASK;
      ds18x20_sensor[index].pioState |= (temp & DS2413_OUTPUT_MASK);
// AddLog(LOG_LEVEL_DEBUG, PSTR("PiO: DS2413-%d Write Port 0x%02x"), index+1, temp);
      ds18x20_sensor[index].valid = SENSOR_MAX_MISS;
      return true;
    }
  }
  return false;
}

bool Ds2413update()
{
  bool return_val = true;
  if (DS18X20Data.ds2413_sensors > 0) {
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      if (ds18x20_sensor[i].address[0] == DS2413_CHIPID) {
        for (uint32_t retry = 0; retry < 3; retry++) {
          if (DS2413write(i, XdrvMailbox.index)) {
           break;      // next sensor
          } else {      // signal Error and retry
            return_val = false;
            ds18x20_sensor[i].change_bits |= DS2413_ERROR_BIT;
//AddLog(LOG_LEVEL_DEBUG, PSTR("PIO: DS2413-%d write Error"), ds18x20_sensor[i].index+1);
          }
        }
      }
    }
  }
  return return_val;
}
#endif
/********************************************************************************************/

void Ds18x20EverySecond(void) {
  if (!DS18X20Data.sensors) { return; }

#ifdef W1_PARASITE_POWER
  // skip access if there is still an eeprom write ongoing
  unsigned long now = millis();
  if (now < DS18X20Data.w1_power_until) { return; }
#endif
  if (TasmotaGlobal.uptime & 1
#ifdef W1_PARASITE_POWER
      // if more than 1 sensor and only parasite power: convert every cycle
      || DS18X20Data.sensors >= 2
#endif
  ) {
    // 2mS
    Ds18x20Convert();          // Start conversion, takes up to one second
  } else {
    for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
      // 12mS per device
      if (!Ds18x20Read(i)) {   // Read temperature
        Ds18x20Name(i);
        AddLogMissed(DS18X20Data.name, ds18x20_sensor[ds18x20_sensor[i].index].valid);
#ifdef USE_DS18x20_RECONFIGURE
        if (!ds18x20_sensor[ds18x20_sensor[i].index].valid) {
          memset(&ds18x20_sensor, 0, sizeof(ds18x20_sensor));
          Ds18x20Init();       // Re-configure
        }
#endif  // USE_DS18x20_RECONFIGURE
      }
    }
  }
}

void Ds18x20Show(bool json) {
  for (uint32_t i = 0; i < DS18X20Data.sensors; i++) {
    uint8_t index = ds18x20_sensor[i].index;
    if (ds18x20_sensor[index].valid) {   // Check for valid temperature
      Ds18x20Name(i);
      if (json) {
        if (Settings->flag5.ds18x20_mean) {
          if ((0 == TasmotaGlobal.tele_period) && ds18x20_sensor[index].numread) {
            ds18x20_sensor[index].temperature = ds18x20_sensor[index].temp_sum / ds18x20_sensor[index].numread;
            ds18x20_sensor[index].numread = 0;
          }
        }
        char address[17];
        for (uint32_t j = 0; j < 6; j++) {
          sprintf(address+2*j, "%02X", ds18x20_sensor[index].address[6-j]);  // Skip sensor type and crc
        }
<<<<<<< HEAD
#ifdef DS18x20_ADD_DS2413
        if(ds18x20_sensor[index].address[0] != DS2413_CHIPID) {
#endif //DS18x20_ADD_DS2413
          ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
            DS18X20Data.name, address, Settings.flag2.temperature_resolution, &ds18x20_sensor[index].temperature);
#ifdef DS18x20_ADD_DS2413
        } else {
          ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" "Port Status" "\":\"%d\"}"),
              DS18X20Data.name, address, ds18x20_sensor[index].pioState);
        }
#endif //DS18x20_ADD_DS2413
=======
        ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%s\",\"" D_JSON_TEMPERATURE "\":%*_f}"),
          DS18X20Data.name, address, Settings->flag2.temperature_resolution, &ds18x20_sensor[index].temperature);
>>>>>>> development
#ifdef USE_DOMOTICZ
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          DomoticzFloatSensor(DZ_TEMP, ds18x20_sensor[index].temperature);
        }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
        if ((0 == TasmotaGlobal.tele_period) && (0 == i)) {
          KnxSensor(KNX_TEMPERATURE, ds18x20_sensor[index].temperature);
        }
#endif  // USE_KNX
#ifdef DS18x20_ADD_DS2413
        if (0 == TasmotaGlobal.tele_period) {
          ds18x20_sensor[index].change_bits = 0;
        }
#endif //DS18x20_ADD_DS2413
#ifdef USE_WEBSERVER
      } else {
#ifdef DS18x20_ADD_DS2413
        if(ds18x20_sensor[index].address[0] != DS2413_CHIPID) {
#endif //DS18x20_ADD_DS2413
          WSContentSend_Temp(DS18X20Data.name, ds18x20_sensor[index].temperature);
#ifdef DS18x20_ADD_DS2413
        } else {
          char portDisplay[17];
          sprintf(portDisplay, "%c000%c%c%c%c",
              ds18x20_sensor[index].change_bits&0x80 ?'1':'0',
              ds18x20_sensor[index].pioState&0x8  ?'1':'0',
              ds18x20_sensor[index].pioState&0x4  ?'1':'0',
              ds18x20_sensor[index].pioState&0x2  ?'1':'0',
              ds18x20_sensor[index].pioState&0x1  ?'1':'0' );
              WSContentSend_PD(HTTP_SNS_PORT, DS18X20Data.name, portDisplay);
        }
#endif //DS18x20_ADD_DS2413
#endif  // USE_WEBSERVER
      }
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns05(uint8_t function) {
  bool result = false;

  if (PinUsed(GPIO_DSB)) {
    switch (function) {
      case FUNC_INIT:
        Ds18x20Init();
        break;
#ifdef DS18x20_ADD_DS2413
      case FUNC_SET_POWER:
        result = Ds2413update();
        break;
#endif //DS18x20_ADD_DS2413
      case FUNC_EVERY_SECOND:
        Ds18x20EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Ds18x20Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Ds18x20Show(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_DS18x20
#endif  // ESP8266
