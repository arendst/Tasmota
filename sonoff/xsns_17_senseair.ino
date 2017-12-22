/*
  xsns_17_senseair_s8.ino - SenseAir S8 CO2 sensor support for Sonoff-Tasmota

  Copyright (C) 2018  Theo Arends

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

#ifdef USE_SENSEAIR
/*********************************************************************************************\
 * SenseAir S8 - CO2 sensor
 *
 * Adapted from EspEasy plugin P052 by Mikael Trieb (mikael__AT__triebconsulting.se)
 **********************************************************************************************
 * Filter usage
 *
 * Select filter usage on low stability readings
\*********************************************************************************************/

#define SENSEAIR_BAUDRATE               9600

const char kSenseairTypes[] PROGMEM = "Kx0|S8";

uint8_t senseair_type = 1;
char senseair_types[7];

uint16_t senseair_co2 = 0;
float senseair_temperature = 0;
float senseair_humidity = 0;

Ticker senseair_ticker;

/*********************************************************************************************\
 * Subset SoftwareSerial
\*********************************************************************************************/

#define SENSEAIR_SERIAL_BUFFER_SIZE     20
#define SENSEAIR_SERIAL_WAIT { while (ESP.getCycleCount() -start < wait) optimistic_yield(1); wait += senseair_serial_bit_time; }

uint8_t senseair_serial_rx_pin;
uint8_t senseair_serial_tx_pin;
uint8_t senseair_serial_in_pos = 0;
uint8_t senseair_serial_out_pos = 0;
uint8_t senseair_serial_buffer[SENSEAIR_SERIAL_BUFFER_SIZE];
unsigned long senseair_serial_bit_time;
unsigned long senseair_serial_bit_time_start;

bool SenseairSerialValidGpioPin(uint8_t pin) {
  return (pin >= 0 && pin <= 5) || (pin >= 9 && pin <= 10) || (pin >= 12 && pin <= 15);
}

bool SenseairSerial(uint8_t receive_pin, uint8_t transmit_pin)
{
  if (!((SenseairSerialValidGpioPin(receive_pin)) && (SenseairSerialValidGpioPin(transmit_pin) || transmit_pin == 16))) {
    return false;
  }
  senseair_serial_rx_pin = receive_pin;
  pinMode(senseair_serial_rx_pin, INPUT);
  attachInterrupt(senseair_serial_rx_pin, SenseairSerialRxRead, FALLING);

  senseair_serial_tx_pin = transmit_pin;
  pinMode(senseair_serial_tx_pin, OUTPUT);
  digitalWrite(senseair_serial_tx_pin, 1);

  senseair_serial_bit_time = ESP.getCpuFreqMHz() *1000000 /SENSEAIR_BAUDRATE;   // 8333
  senseair_serial_bit_time_start = senseair_serial_bit_time + senseair_serial_bit_time /3 -500;  // 10610 ICACHE_RAM_ATTR start delay
//  senseair_serial_bit_time_start = senseair_serial_bit_time;                           // Non ICACHE_RAM_ATTR start delay (experimental)

  return true;
}

int SenseairSerialRead() {
  if (senseair_serial_in_pos == senseair_serial_out_pos) {
    return -1;
  }
  int ch = senseair_serial_buffer[senseair_serial_out_pos];
  senseair_serial_out_pos = (senseair_serial_out_pos +1) % SENSEAIR_SERIAL_BUFFER_SIZE;
  return ch;
}

int SenseairSerialAvailable() {
  int avail = senseair_serial_in_pos - senseair_serial_out_pos;
  if (avail < 0) {
    avail += SENSEAIR_SERIAL_BUFFER_SIZE;
  }
  return avail;
}

void SenseairSerialFlush()
{
  senseair_serial_in_pos = 0;
  senseair_serial_out_pos = 0;
}

size_t SenseairSerialTxWrite(uint8_t b)
{
  unsigned long wait = senseair_serial_bit_time;
  digitalWrite(senseair_serial_tx_pin, HIGH);
  unsigned long start = ESP.getCycleCount();
    // Start bit;
  digitalWrite(senseair_serial_tx_pin, LOW);
  SENSEAIR_SERIAL_WAIT;
  for (int i = 0; i < 8; i++) {
    digitalWrite(senseair_serial_tx_pin, (b & 1) ? HIGH : LOW);
    SENSEAIR_SERIAL_WAIT;
    b >>= 1;
  }
   // Stop bit
  digitalWrite(senseair_serial_tx_pin, HIGH);
  SENSEAIR_SERIAL_WAIT;
  return 1;
}

size_t SenseairSerialWrite(const uint8_t *buffer, size_t size = 1) {
  size_t n = 0;
  while(size--) {
    n += SenseairSerialTxWrite(*buffer++);
  }
  return n;
}

//void SenseairSerialRxRead() ICACHE_RAM_ATTR;  // Add 215 bytes to iram usage
void SenseairSerialRxRead() {
  // Advance the starting point for the samples but compensate for the
  // initial delay which occurs before the interrupt is delivered
  unsigned long wait = senseair_serial_bit_time_start;
  unsigned long start = ESP.getCycleCount();
  uint8_t rec = 0;
  for (int i = 0; i < 8; i++) {
    SENSEAIR_SERIAL_WAIT;
    rec >>= 1;
    if (digitalRead(senseair_serial_rx_pin)) {
      rec |= 0x80;
    }
  }
  // Stop bit
  SENSEAIR_SERIAL_WAIT;
  // Store the received value in the buffer unless we have an overflow
  int next = (senseair_serial_in_pos +1) % SENSEAIR_SERIAL_BUFFER_SIZE;
  if (next != senseair_serial_out_pos) {
    senseair_serial_buffer[senseair_serial_in_pos] = rec;
    senseair_serial_in_pos = next;
  }
  // Must clear this bit in the interrupt register,
  // it gets set even when interrupts are disabled
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, 1 << senseair_serial_rx_pin);
}

/*********************************************************************************************/

void ModbusSend(uint8_t function_code, uint16_t start_address, uint16_t register_count)
{
  uint8_t frame[8];

  frame[0] = 0xFE;  // Any Address
  frame[1] = function_code;
  frame[2] = (uint8_t)(start_address >> 8);
  frame[3] = (uint8_t)(start_address);
  frame[4] = (uint8_t)(register_count >> 8);
  frame[5] = (uint8_t)(register_count);
  uint16_t crc = 0xFFFF;
  for (uint8_t pos = 0; pos < sizeof(frame) -2; pos++) {
    crc ^= (uint16_t)frame[pos];        // XOR byte into least sig. byte of crc
    for (uint8_t i = 8; i != 0; i--) {  // Loop over each bit
      if ((crc & 0x0001) != 0) {        // If the LSB is set
        crc >>= 1;                      // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else {                            // Else LSB is not set
        crc >>= 1;                      // Just shift right
      }
    }
  }
  frame[7] = (uint8_t)((crc >> 8) & 0xFF);
  frame[6] = (uint8_t)(crc & 0xFF);

  SenseairSerialFlush();
  SenseairSerialWrite(frame, sizeof(frame));
}

bool ModbusReceiveReady()
{
  return (SenseairSerialAvailable() >= 5);  // 5 - Error frame, 7 - Ok frame
}

uint8_t ModbusReceive(uint16_t *value)
{
  uint8_t buffer[7];

  uint8_t len = 0;
  while (SenseairSerialAvailable() > 0) {
    buffer[len++] = (uint8_t)SenseairSerialRead();
    if (3 == len) {
      if (buffer[1] & 0x80) {  // fe 84 02 f2 f1
        return buffer[2];      // 1 = Illegal Function, 2 = Illegal Data Address, 3 = Illegal Data Value
      }
    }
  }
  if (len != sizeof(buffer)) {
    return 9;                  // 9 = Unexpected result
  }
  *value = (buffer[3] << 8) | buffer[4];
  return 0;                    // 0 = No error
}

/*********************************************************************************************/

const uint8_t start_addresses[] { 0x1A, 0x00, 0x03, 0x04, 0x05, 0x1C, 0x0A };

uint8_t senseair_read_state = 0;
uint8_t senseair_send_retry = 0;

void SenseairTicker()
{
  uint16_t value = 0;
  bool data_ready = ModbusReceiveReady();

  if (data_ready) {
    uint8_t error = ModbusReceive(&value);
    if (error) {
      snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir response error %d"), error);
      AddLog(LOG_LEVEL_DEBUG);
    } else {
      switch(senseair_read_state) {
        case 0:                          // 0x1A (26) READ_TYPE_LOW - S8: fe 04 02 01 77 ec 92
          senseair_type = 2;
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir type id low %04X"), value);
          AddLog(LOG_LEVEL_DEBUG);
          break;
        case 1:                          // 0x00 (0) READ_ERRORLOG - fe 04 02 00 00 ad 24
          if (value) {
            snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir error %04X"), value);
            AddLog(LOG_LEVEL_DEBUG);
          }
          break;
        case 2:                          // 0x03 (3) READ_CO2 - fe 04 02 06 2c af 59
          senseair_co2 = value;
          break;
        case 3:                          // 0x04 (4) READ_TEMPERATURE - S8: fe 84 02 f2 f1 - Illegal Data Address
          senseair_temperature = ConvertTemp((float)value / 100);
          break;
        case 4:                          // 0x05 (5) READ_HUMIDITY - S8: fe 84 02 f2 f1 - Illegal Data Address
          senseair_humidity = (float)value / 100;
          break;
        case 5:                          // 0x1C (28) READ_RELAY_STATE - S8: fe 04 02 01 54 ad 4b - firmware version
        {
          bool relay_state = value >> 8 & 1;
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir relay state %d"), relay_state);
          AddLog(LOG_LEVEL_DEBUG);
          break;
        }
        case 6:                          // 0x0A (10) READ_TEMP_ADJUSTMENT - S8: fe 84 02 f2 f1 - Illegal Data Address
          snprintf_P(log_data, sizeof(log_data), PSTR(D_LOG_DEBUG "SenseAir temp adjustment %d"), value);
          AddLog(LOG_LEVEL_DEBUG);
          break;
      }
    }
    senseair_read_state++;
    if (2 == senseair_type) {  // S8
      if (3 == senseair_read_state) {
        senseair_read_state = 1;
      }
    } else {                   // K30, K70
      if (sizeof(start_addresses) == senseair_read_state) {
        senseair_read_state = 1;
      }
    }
  }

  if (0 == senseair_send_retry || data_ready) {
    senseair_send_retry = 5;
    ModbusSend(0x04, (uint16_t)start_addresses[senseair_read_state], 1);
  } else {
    senseair_send_retry--;
  }
}

/*********************************************************************************************/

void SenseairInit()
{
  senseair_type = 0;
  if ((pin[GPIO_SAIR_RX] < 99) && (pin[GPIO_SAIR_TX] < 99)) {
    if (SenseairSerial(pin[GPIO_SAIR_RX], pin[GPIO_SAIR_TX])) {
      senseair_type = 1;
      senseair_ticker.attach_ms(510, SenseairTicker);
    }
  }
}

void SenseairShow(boolean json)
{
  char temperature[10];
  char humidity[10];
  dtostrfd(senseair_temperature, Settings.flag2.temperature_resolution, temperature);
  dtostrfd(senseair_humidity, Settings.flag2.temperature_resolution, humidity);
  GetTextIndexed(senseair_types, sizeof(senseair_types), senseair_type -1, kSenseairTypes);
//  uint8_t co2_limit = (senseair_co2 > 1200) ? 3 : (senseair_co2 > 800) ? 2 : 1;
//  uint16_t co2_limit = senseair_co2 / 400;  // <800 = 1(Green), <1200 = 2(Orange), >1200 = 3(Red)

  if (json) {
//    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_CO2 "\":%d,\"" D_LIMIT "\":%d"), mqtt_data, senseair_types, senseair_co2, co2_limit);
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"%s\":{\"" D_CO2 "\":%d"), mqtt_data, senseair_types, senseair_co2);
    if (senseair_type != 2) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s,\"" D_TEMPERATURE "\":%s,\"" D_HUMIDITY "\":%s"), mqtt_data, temperature, humidity);
    }
    snprintf_P(mqtt_data, sizeof(mqtt_data), PSTR("%s}"), mqtt_data);
#ifdef USE_DOMOTICZ
    DomoticzSensor(DZ_AIRQUALITY, senseair_co2);
#endif  // USE_DOMOTICZ
#ifdef USE_WEBSERVER
  } else {
    snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_CO2, mqtt_data, senseair_types, senseair_co2);
    if (senseair_type != 2) {
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_TEMP, mqtt_data, senseair_types, temperature, TempUnit());
      snprintf_P(mqtt_data, sizeof(mqtt_data), HTTP_SNS_HUM, mqtt_data, senseair_types, humidity);
    }
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

#define XSNS_17

boolean Xsns17(byte function)
{
  boolean result = false;

  if (senseair_type) {
    switch (function) {
      case FUNC_XSNS_INIT:
        SenseairInit();
        break;
      case FUNC_XSNS_JSON_APPEND:
        SenseairShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_XSNS_WEB_APPEND:
        SenseairShow(0);
        break;
#endif  // USE_WEBSERVER
    }
  }
  return result;
}

#endif  // USE_SENSEAIR
