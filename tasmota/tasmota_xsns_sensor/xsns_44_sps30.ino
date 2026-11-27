/*
  xsns_44_sps30.ino - Sensirion SPS30 support for Tasmota

  Copyright (C) 2021  Gerhard Mutz and Theo Arends

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

#if defined(USE_SPS30_SHDLC) || (defined(USE_I2C) && defined(USE_SPS30))
/*********************************************************************************************\
 * SPS30 - Particulate Matter (PM)
 *
 * The interface is selected at compile time (saves flash on ESP82xx):
 *
 * USE_SPS30_SHDLC defined (USE_SPS30 and USE_I2C are not needed):
 *   Serial SHDLC interface, 115200 8N1. Configure GPIOs "SPS30 Tx" and "SPS30 Rx".
 *   SPS30 pin 1 VDD (5V), 2 RX <- ESP TX, 3 TX -> ESP RX, 4 SEL not connected (floating), 5 GND
 *   Datasheet recommends serial for cables longer than 20 cm.
 *   Overrules I2C: I2C code is NOT compiled in, even if USE_SPS30 and USE_I2C are defined.
 *
 * USE_SPS30_SHDLC not defined, USE_SPS30 and USE_I2C defined:
 *   I2C interface, address 0x69.
 *   SPS30 pin 1 VDD (5V), 2 SDA, 3 SCL, 4 SEL connected to GND, 5 GND
\*********************************************************************************************/

#if !defined(USE_SPS30_SHDLC) && defined(USE_I2C) && defined(USE_SPS30)
#define SPS30_I2C
#endif

#define XSNS_44                    44
#ifdef SPS30_I2C
#define XI2C_30                    30  // See I2CDEVICES.md
#endif  // SPS30_I2C

//#define SPS30_ENABLE_SLEEP             // SPS30 v2.0: Adds support for sleep/wakeup to reduce power when not measuring (+0k2 code)
//#define SPS30_PARTS_PER_DECILITER      // Report NCPM as parts per deciliter instead of parts per cm3 (+0k1 code)

//#define SENSIRION_DEBUG                // Adds 1k2 to code size

#ifdef USE_SPS30_SHDLC
#define SPS30_SHDLC_BAUDRATE       115200
#define SPS30_SHDLC_TIMEOUT        50      // Max wait [ms] for a response frame (datasheet: max 20 ms)
#define SPS30_SHDLC_BUFFER_SIZE    64      // Software serial receive buffer (largest frame ~ 47 bytes + byte stuffing)

#include <TasmotaSerial.h>

TasmotaSerial *Sps30Serial = nullptr;

#ifdef SENSIRION_DEBUG
uint8_t Sps30RxDump[16];                   // First bytes received during the last response
uint8_t Sps30RxCount = 0;                  // Number of bytes received during the last response (max 255)
#endif  // SENSIRION_DEBUG
#endif  // USE_SPS30_SHDLC

#ifdef SPS30_I2C
#include <SensirionI2cSps30.h>

SensirionI2cSps30 sps30;
#endif  // SPS30_I2C

struct SPS30DATA_s {
  float PM1_0;
  float PM2_5;
  float PM4_0;
  float PM10;
  float NCPM0_5;
  float NCPM1_0;
  float NCPM2_5;
  float NCPM4_0;
  float NCPM10;
  float TYPSIZ;
  uint8_t running;
} *SPS30DATA = nullptr;

/********************************************************************************************/

#ifdef USE_SPS30_SHDLC
/*********************************************************************************************\
 * Serial SHDLC protocol (Sensirion High-Level Data Link Control)
 *
 * Frame: 0x7E ADR CMD LEN DATA[LEN] CHK 0x7E             (MOSI, master -> sensor)
 *        0x7E ADR CMD STATE LEN DATA[LEN] CHK 0x7E       (MISO, sensor -> master)
 * ADR is always 0. CHK = ~(sum of all bytes between start and stop) before byte stuffing.
 * Bytes 0x7E, 0x7D, 0x11 and 0x13 inside a frame are sent as 0x7D, (byte ^ 0x20).
\*********************************************************************************************/

#define SPS30_SHDLC_START          0x7E
#define SPS30_SHDLC_ESCAPE         0x7D
#define SPS30_SHDLC_MAX_TX         5       // Largest MOSI data field used (write cleaning interval)

#define SPS30_CMD_START_MEASUREMENT 0x00
#define SPS30_CMD_STOP_MEASUREMENT  0x01
#define SPS30_CMD_READ_MEASUREMENT  0x03
#define SPS30_CMD_SLEEP             0x10
#define SPS30_CMD_WAKEUP            0x11
#define SPS30_CMD_FAN_CLEANING      0x56
#define SPS30_CMD_DEVICE_INFO       0xD0
#define SPS30_CMD_VERSION           0xD1
#define SPS30_CMD_RESET             0xD3

// Serial error codes (> 0 is the execution error code reported by the sensor, see datasheet table 6)
#define SPS30_ERR_TIMEOUT          -1      // No complete frame received
#define SPS30_ERR_FRAME            -2      // Malformed frame / wrong length
#define SPS30_ERR_CHECKSUM         -3      // Checksum mismatch
#define SPS30_ERR_COMMAND          -4      // Response to a different command / address

bool Sps30IsStuffed(uint8_t b) {
  return (SPS30_SHDLC_START == b) || (SPS30_SHDLC_ESCAPE == b) || (0x11 == b) || (0x13 == b);
}

void Sps30ShdlcSend(uint8_t cmd, const uint8_t *tx, uint8_t tx_len) {
  if (tx_len > SPS30_SHDLC_MAX_TX) { tx_len = SPS30_SHDLC_MAX_TX; }

  uint8_t raw[4 + SPS30_SHDLC_MAX_TX];     // ADR CMD LEN DATA CHK
  uint32_t n = 0;
  raw[n++] = 0;                            // ADR
  raw[n++] = cmd;
  raw[n++] = tx_len;
  for (uint32_t i = 0; i < tx_len; i++) {
    raw[n++] = tx[i];
  }
  uint8_t sum = 0;
  for (uint32_t i = 0; i < n; i++) {
    sum += raw[i];
  }
  raw[n++] = (uint8_t)~sum;                // CHK

  uint8_t out[2 + 2 * sizeof(raw)];        // Worst case every byte is stuffed
  uint32_t o = 0;
  out[o++] = SPS30_SHDLC_START;
  for (uint32_t i = 0; i < n; i++) {
    if (Sps30IsStuffed(raw[i])) {
      out[o++] = SPS30_SHDLC_ESCAPE;
      out[o++] = raw[i] ^ 0x20;
    } else {
      out[o++] = raw[i];
    }
  }
  out[o++] = SPS30_SHDLC_START;

  Sps30Serial->write(out, o);
}

int Sps30ShdlcReceive(uint8_t cmd, uint8_t *rx, uint8_t rx_size, uint8_t *rx_len) {
  uint8_t buf[4 + 48 + 1 + 8];             // ADR CMD STATE LEN DATA CHK (+ margin)
  uint32_t count = 0;
  bool in_frame = false;
  bool complete = false;
  bool escape = false;
  uint32_t start = millis();
#ifdef SENSIRION_DEBUG
  Sps30RxCount = 0;
#endif  // SENSIRION_DEBUG

  while (!complete && (TimePassedSince(start) < SPS30_SHDLC_TIMEOUT)) {
    if (!Sps30Serial->available()) {
      delay(1);
      continue;
    }
    uint8_t b = Sps30Serial->read();
#ifdef SENSIRION_DEBUG
    if (Sps30RxCount < sizeof(Sps30RxDump)) { Sps30RxDump[Sps30RxCount] = b; }
    if (Sps30RxCount < 255) { Sps30RxCount++; }
#endif  // SENSIRION_DEBUG
    if (SPS30_SHDLC_START == b) {
      if (in_frame && count) {
        complete = true;                   // Stop byte
      } else {
        in_frame = true;                   // Start byte (or repeated start byte)
        count = 0;
        escape = false;
      }
      continue;
    }
    if (!in_frame) { continue; }           // Garbage before start byte
    if (escape) {
      b ^= 0x20;
      escape = false;
    }
    else if (SPS30_SHDLC_ESCAPE == b) {
      escape = true;
      continue;
    }
    if (count >= sizeof(buf)) { return SPS30_ERR_FRAME; }
    buf[count++] = b;
  }
  if (!complete) { return SPS30_ERR_TIMEOUT; }

  if (count < 5) { return SPS30_ERR_FRAME; }
  uint8_t len = buf[3];
  if (count != (uint32_t)len + 5) { return SPS30_ERR_FRAME; }

  uint8_t sum = 0;
  for (uint32_t i = 0; i < count - 1; i++) {
    sum += buf[i];
  }
  if ((uint8_t)~sum != buf[count - 1]) { return SPS30_ERR_CHECKSUM; }
  if ((0 != buf[0]) || (cmd != buf[1])) { return SPS30_ERR_COMMAND; }

  uint8_t state = buf[2];
  if (state & 0x80) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: Device status register error flag set (cmd 0x%02X)"), cmd);
  }
  if (state & 0x7F) { return state & 0x7F; }  // Execution error code

  if (len) {
    if (rx && (len > rx_size)) { return SPS30_ERR_FRAME; }
    if (rx) { memcpy(rx, &buf[4], len); }
  }
  if (rx_len) { *rx_len = len; }
  return 0;
}

int Sps30ShdlcCommand(uint8_t cmd, const uint8_t *tx, uint8_t tx_len, uint8_t *rx, uint8_t rx_size, uint8_t *rx_len) {
  if (rx_len) { *rx_len = 0; }
  while (Sps30Serial->available()) { Sps30Serial->read(); }  // Drop stale bytes
  Sps30ShdlcSend(cmd, tx, tx_len);
  return Sps30ShdlcReceive(cmd, rx, rx_size, rx_len);
}

float Sps30BigEndianFloat(const uint8_t *p) {
  uint32_t value = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | p[3];
  float result;
  memcpy(&result, &value, sizeof(result));
  return result;
}

// Returns 0 on success. *new_data is false when the sensor had no new values (empty response)
int Sps30SerialReadMeasurement(float *values, bool *new_data) {
  uint8_t rx[40];                          // 10 x IEEE754 float
  uint8_t rx_len = 0;
  *new_data = false;
  int error = Sps30ShdlcCommand(SPS30_CMD_READ_MEASUREMENT, nullptr, 0, rx, sizeof(rx), &rx_len);
  if (error) { return error; }
  if (0 == rx_len) { return 0; }           // No new measurement available yet
  if (sizeof(rx) != rx_len) { return SPS30_ERR_FRAME; }
  for (uint32_t i = 0; i < 10; i++) {
    values[i] = Sps30BigEndianFloat(&rx[i * 4]);
  }
  *new_data = true;
  return 0;
}
#endif  // USE_SPS30_SHDLC

/********************************************************************************************/

bool Sps30Error(const char* func, int error) {
  bool result = (error != 0);
  if (result) {
#if defined(SENSIRION_DEBUG) && defined(SPS30_I2C)
    char error_msg[64];
    errorToString(error, error_msg, sizeof(error_msg));
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: %s error %d %s"), func, error, error_msg);
#else
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: %s error %d"), func, error);
#if defined(USE_SPS30_SHDLC) && defined(SENSIRION_DEBUG)
    // -1 = nothing/incomplete received: no bytes -> wiring/GPIO/SEL/power, garbage -> baudrate/software serial
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: %s received %d byte(s) %*_H"), func, Sps30RxCount,
      (Sps30RxCount < sizeof(Sps30RxDump)) ? Sps30RxCount : sizeof(Sps30RxDump), Sps30RxDump);
#endif
#endif
  }
  return result;
}

/********************************************************************************************/
// Interface independent actions (interface selected at compile time)

int Sps30StartMeasurement(void) {
#ifdef USE_SPS30_SHDLC
  const uint8_t data[2] = { 0x01, 0x03 };    // Subcommand, output format big-endian IEEE754 float
  return Sps30ShdlcCommand(SPS30_CMD_START_MEASUREMENT, data, sizeof(data), nullptr, 0, nullptr);
#else   // SPS30_I2C
  return sps30.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_FLOAT);
#endif  // USE_SPS30_SHDLC
}

int Sps30StopMeasurement(void) {
#ifdef USE_SPS30_SHDLC
  return Sps30ShdlcCommand(SPS30_CMD_STOP_MEASUREMENT, nullptr, 0, nullptr, 0, nullptr);
#else   // SPS30_I2C
  return sps30.stopMeasurement();
#endif  // USE_SPS30_SHDLC
}

int Sps30StartFanCleaning(void) {
#ifdef USE_SPS30_SHDLC
  return Sps30ShdlcCommand(SPS30_CMD_FAN_CLEANING, nullptr, 0, nullptr, 0, nullptr);
#else   // SPS30_I2C
  return sps30.startFanCleaning();
#endif  // USE_SPS30_SHDLC
}

#ifdef SPS30_ENABLE_SLEEP
int Sps30Sleep(void) {
#ifdef USE_SPS30_SHDLC
  return Sps30ShdlcCommand(SPS30_CMD_SLEEP, nullptr, 0, nullptr, 0, nullptr);
#else   // SPS30_I2C
  return sps30.sleep();
#endif  // USE_SPS30_SHDLC
}

int16_t Sps30Wakeup(void) {
  // SPS30 v2.0: Switch from sleep to idle mode.
  int16_t localError = 0;
#ifdef USE_SPS30_SHDLC
  // Interface is disabled in sleep mode. A single 0xFF byte generates the low pulse on the RX pin
  // that activates it. The wake-up command must follow within 100 ms.
  // An awake sensor answers with 'command not allowed in current state' which is ignored here.
  Sps30Serial->write((uint8_t)0xFF);
  Sps30ShdlcCommand(SPS30_CMD_WAKEUP, nullptr, 0, nullptr, 0, nullptr);
#else   // SPS30_I2C
  // If the software implementation does not allow to send a I2C-Start-Condition followed by a Stop-Condition,
  // the Wake-up command can be sent twice in succession. In this case the first Wake-up command is ignored, but causes
  // the interface to be activated.
  sps30.wakeUp();
/*
  // Note: Lib v1.0.1 always returns 0 on call to wakeUp()
  if (Sps30Error("WakeUp", sps30.wakeUp())) {
    localError = 1;
  }
*/
  sps30.wakeUp();
#endif  // USE_SPS30_SHDLC
  return localError;
}
#endif  // SPS30_ENABLE_SLEEP

/********************************************************************************************/

#ifdef USE_SPS30_SHDLC
void Sps30SerialInit(void) {
  if (!PinUsed(GPIO_SPS30_TX) || !PinUsed(GPIO_SPS30_RX)) { return; }  // Serial GPIOs not configured
  PowerOnDelay(60);  // Sensor startup time

  Sps30Serial = new TasmotaSerial(Pin(GPIO_SPS30_RX), Pin(GPIO_SPS30_TX), 1, 0, SPS30_SHDLC_BUFFER_SIZE);
  if (!Sps30Serial->begin(SPS30_SHDLC_BAUDRATE)) {
    AddLog(LOG_LEVEL_ERROR, PSTR("SPS: Unable to begin serial (baudrate %d)"), SPS30_SHDLC_BAUDRATE);
    delete Sps30Serial;
    Sps30Serial = nullptr;
    return;
  }
  if (Sps30Serial->hardwareSerial()) {
    ClaimSerial();
    SetSerial(SPS30_SHDLC_BAUDRATE, TS_SERIAL_8N1);
  }
  AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: Serial %s, Tx GPIO%d, Rx GPIO%d"),
    Sps30Serial->hardwareSerial() ? "hardware" : "software", Pin(GPIO_SPS30_TX), Pin(GPIO_SPS30_RX));

#ifdef SPS30_ENABLE_SLEEP
  Sps30Wakeup();                                     // Sensor might have been left in sleep mode
#endif  // SPS30_ENABLE_SLEEP

  // Switch to power reset state (idle mode). Retry as the sensor might still be starting up
  int error = 0;
  for (uint32_t attempt = 0; attempt < 3; attempt++) {
    error = Sps30ShdlcCommand(SPS30_CMD_RESET, nullptr, 0, nullptr, 0, nullptr);
    if (!error) { break; }
    Sps30Error("Reset", error);
    delay(100);
  }
  if (Sps30Error("Reset", error)) { goto fail; }
  delay(100);                                        // Reset is executed after the response frame

  {
    uint8_t version[7] = { 0 };
    uint8_t version_len = 0;
    if (Sps30Error("Version", Sps30ShdlcCommand(SPS30_CMD_VERSION, nullptr, 0, version, sizeof(version), &version_len))
        || (version_len < 2)) {
      goto fail;
    }

    uint8_t serial_request = 0x03;                   // Serial number
    char serial_number[33] = { 0 };
    uint8_t serial_len = 0;
    if (Sps30Error("Serialnumber", Sps30ShdlcCommand(SPS30_CMD_DEVICE_INFO, &serial_request, 1,
                    (uint8_t*)serial_number, sizeof(serial_number) - 1, &serial_len))) {
      goto fail;
    }
    serial_number[serial_len] = '\0';

    if (Sps30Error("StartMeasurement", Sps30StartMeasurement())) { goto fail; }

    SPS30DATA = (SPS30DATA_s *)calloc(1, sizeof(struct SPS30DATA_s));
    if (nullptr == SPS30DATA) { goto fail; }
    SPS30DATA->running = 1;

    AddLog(LOG_LEVEL_INFO, PSTR("SPS: SPS30 serial interface, serialnumber %s v%d.%d"), serial_number, version[0], version[1]);
    return;
  }

fail:
  AddLog(LOG_LEVEL_INFO, PSTR("SPS: SPS30 not found on serial interface"));
  delete Sps30Serial;
  Sps30Serial = nullptr;
}
#endif  // USE_SPS30_SHDLC

#ifdef SPS30_I2C
void Sps30I2cInit(void) {
  PowerOnDelay(60);  // Sensor startup time (Time after power-on until I2C communication can be started)
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    if (!I2cSetDevice(SPS30_I2C_ADDR_69, bus)) { 
//      Sps30Error("Scan", bus +1);
      continue;
    }
    sps30.begin(I2cGetWire(bus), SPS30_I2C_ADDR_69);

    if (sps30.deviceReset()) {                        // Switch to power reset state (idle mode). Performs delay(100) if no error
      if (Sps30Error("Reset", sps30.deviceReset())) { // See https://github.com/arendst/Tasmota/discussions/24452
        continue;
      }
    }

    uint8_t major;
    uint8_t minor;
//    if (Sps30Error("Version", sps30.readFirmwareVersion(major, minor))) {
    if (sps30.readFirmwareVersion(major, minor)) {
      continue;
    }

    int8_t serial_number[32] = { 0 };
//    if (Sps30Error("Serialnumber", sps30.readSerialNumber(serial_number, sizeof(serial_number)))) {
    if (sps30.readSerialNumber(serial_number, sizeof(serial_number))) {
      continue;
    }
/*
    // (always “00080000” without terminating null-character, recommended to use as product identifier)
    int8_t product_type[8] = { 0 };
    if (Sps30Error("Producttype", sps30.readProductType(product_type, sizeof(product_type)))) {
      continue;
    }
*/
    if (Sps30Error("StartMeasurement", Sps30StartMeasurement())) {
      continue;
    }

    SPS30DATA = (SPS30DATA_s *)calloc(1, sizeof(struct SPS30DATA_s));
    if (nullptr == SPS30DATA) { return; }
    SPS30DATA->running = 1;

    I2cSetActiveFound(SPS30_I2C_ADDR_69, "SPS30", bus);
    AddLog(LOG_LEVEL_DEBUG, PSTR("SPS: SPS30 serialnumber %s v%d.%d"), serial_number, major, minor);
    return;
  }
}
#endif  // SPS30_I2C

void Sps30Init(void) {
#ifdef USE_SPS30_SHDLC
  Sps30SerialInit();
#else   // SPS30_I2C
  if (I2cEnabled(XI2C_30)) {
    Sps30I2cInit();
  }
#endif  // USE_SPS30_SHDLC
}

void Sps30EverySecond(void) {
  if (!SPS30DATA->running) { return; }

  if (TasmotaGlobal.uptime % 10 == 0) {
    float val[10];
    bool valid = false;
#ifdef USE_SPS30_SHDLC
    bool new_data;
    valid = !Sps30Error("Measurement", Sps30SerialReadMeasurement(val, &new_data)) && new_data;
#else   // SPS30_I2C
    valid = !Sps30Error("Measurement", sps30.readMeasurementValuesFloat(
      val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9]));
#endif  // USE_SPS30_SHDLC
    if (valid) {
      float PM1_0 = val[0];
      float PM2_5 = val[1];
      float PM4_0 = val[2];
      float PM10 = val[3];
      float NCPM0_5 = val[4];
      float NCPM1_0 = val[5];
      float NCPM2_5 = val[6];
      float NCPM4_0 = val[7];
      float NCPM10 = val[8];
      float TYPSIZ = val[9];
      SPS30DATA->PM1_0 = PM1_0;
      SPS30DATA->PM2_5 = PM2_5;
      SPS30DATA->PM4_0 = PM4_0;
      SPS30DATA->PM10 = PM10;
#ifdef SPS30_PARTS_PER_DECILITER
      SPS30DATA->NCPM0_5 = NCPM0_5 * 100;  // ppd = 100 cm3
      SPS30DATA->NCPM1_0 = NCPM1_0 * 100;
      SPS30DATA->NCPM2_5 = NCPM2_5 * 100;
      SPS30DATA->NCPM4_0 = NCPM4_0 * 100;
      SPS30DATA->NCPM10 = NCPM10 * 100;
#else   // Number per cm3
      SPS30DATA->NCPM0_5 = NCPM0_5;
      SPS30DATA->NCPM1_0 = NCPM1_0;
      SPS30DATA->NCPM2_5 = NCPM2_5;
      SPS30DATA->NCPM4_0 = NCPM4_0;
      SPS30DATA->NCPM10 = NCPM10;
#endif  // SPS30_PARTS_PER_DECILITER
      SPS30DATA->TYPSIZ = TYPSIZ;
    }
  }

  if (TasmotaGlobal.uptime % 3600 == 0 && TasmotaGlobal.uptime > 60) {
    // should auto clean once per week runtime
    // so count hours, should be in Settings
    Settings->sps30_inuse_hours++;
    if (Settings->sps30_inuse_hours > (7*24)) {
      Settings->sps30_inuse_hours = 0;
      Sps30StartFanCleaning();
      ResponseTime_P(PSTR(",\"SPS30\":\"FanCleaning\"}"));
      MqttPublishTeleSensor();
    }
  }

}

/*********************************************************************************************\
 * Commands
\*********************************************************************************************/

bool Sps30Command(void) {
  // Sensor44 0 - Stop measurment
  // Sensor44 1 - Start measurment
  // Sensor44 c - Clean
  bool serviced = true;
  if (XdrvMailbox.data_len > 0) {
    char *cp = XdrvMailbox.data;
    if (*cp == 'c') {
      // clean cmd
      Sps30StartFanCleaning();
      Response_P(PSTR("{\"SPS30\":\"FanCleaning\"}"));
      return true;
    }
    else if (*cp == '0' || *cp == '1') {
      SPS30DATA->running = *cp &1;
      if (SPS30DATA->running) {
#ifdef SPS30_ENABLE_SLEEP
        Sps30Wakeup();
#endif  // SPS30_ENABLE_SLEEP
        Sps30StartMeasurement();
      } else {
        Sps30StopMeasurement();
#ifdef SPS30_ENABLE_SLEEP
        Sps30Sleep();
#endif  // SPS30_ENABLE_SLEEP
      }
    } else {
      return false;;
    }
  }
  Response_P(PSTR("{\"SPS30\":\"%s\"}"), SPS30DATA->running ? "Running" : "Stopped");
  return serviced;
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#define D_UNIT_NUMBER_PER_CUBIC_CM "#/cm³"
#define D_TYPICAL_PARTICLE_SIZE "Typical Size"
#define D_NUMBER_ENVIRONMENTAL_CONCENTRATION "NCPM"

#ifdef USE_WEBSERVER
#ifndef SPS30_PARTS_PER_DECILITER
const char HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION[] PROGMEM = "{s}%s " D_NUMBER_ENVIRONMENTAL_CONCENTRATION " %s " D_UNIT_MICROMETER "{m}%1_f " D_UNIT_NUMBER_PER_CUBIC_CM "{e}";
#endif  // No SPS30_PARTS_PER_DECILITER
const char HTTP_SNS_F_TYPICAL_PARTICLE_SIZE[] PROGMEM ="{s}%s " D_TYPICAL_PARTICLE_SIZE "{m}%2_f " D_UNIT_MICROMETER "{e}";
#endif  // USE_WEBSERVER

void Sps30Show(bool json) {
  if (!SPS30DATA->running) { return; }

  if (json) {
    ResponseAppend_P(PSTR(",\"SPS30\":{\"PM1_0\":%2_f,\"PM2_5\":%2_f,\"PM4_0\":%2_f,\"PM10\":%2_f"),
      &SPS30DATA->PM1_0, &SPS30DATA->PM2_5, &SPS30DATA->PM4_0, &SPS30DATA->PM10);
#ifdef SPS30_PARTS_PER_DECILITER
    ResponseAppend_P(PSTR(",\"NCPM0_5\":%d,\"NCPM1_0\":%d,\"NCPM2_5\":%d,\"NCPM4_0\":%d,\"NCPM10\":%d"),
      (uint32_t)SPS30DATA->NCPM0_5, (uint32_t)SPS30DATA->NCPM1_0, (uint32_t)SPS30DATA->NCPM2_5, (uint32_t)SPS30DATA->NCPM4_0, (uint32_t)SPS30DATA->NCPM10);
#else   // Number per cm3
    ResponseAppend_P(PSTR(",\"NCPM0_5\":%2_f,\"NCPM1_0\":%2_f,\"NCPM2_5\":%2_f,\"NCPM4_0\":%2_f,\"NCPM10\":%2_f"),
      &SPS30DATA->NCPM0_5, &SPS30DATA->NCPM1_0, &SPS30DATA->NCPM2_5, &SPS30DATA->NCPM4_0, &SPS30DATA->NCPM10);
#endif  // SPS30_PARTS_PER_DECILITER
    ResponseAppend_P(PSTR(",\"TYPSIZ\":%2_f}"), &SPS30DATA->TYPSIZ);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "1", &SPS30DATA->PM1_0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "2.5", &SPS30DATA->PM2_5);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "4", &SPS30DATA->PM4_0);
    WSContentSend_PD(HTTP_SNS_F_ENVIRONMENTAL_CONCENTRATION, "SPS30", "10", &SPS30DATA->PM10);
#ifdef SPS30_PARTS_PER_DECILITER
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "0.5", (uint32_t)SPS30DATA->NCPM0_5);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "1", (uint32_t)SPS30DATA->NCPM1_0);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "2.5", (uint32_t)SPS30DATA->NCPM2_5);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "4", (uint32_t)SPS30DATA->NCPM4_0);
    WSContentSend_PD(HTTP_SNS_PARTICALS_BEYOND, "SPS30", "10", (uint32_t)SPS30DATA->NCPM10);
#else   // Number per cm3
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "0.5", &SPS30DATA->NCPM0_5);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "1", &SPS30DATA->NCPM1_0);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "2.5", &SPS30DATA->NCPM2_5);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "4", &SPS30DATA->NCPM4_0);
    WSContentSend_PD(HTTP_SNS_F_NUMBER_ENVIRONMENTAL_CONCENTRATION, "SPS30", "10", &SPS30DATA->NCPM10);
#endif  // SPS30_PARTS_PER_DECILITER
    WSContentSend_PD(HTTP_SNS_F_TYPICAL_PARTICLE_SIZE, "SPS30", &SPS30DATA->TYPSIZ);
#endif  // USE_WEBSERVER
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns44(uint32_t function) {
#ifdef SPS30_I2C
  if (!I2cEnabled(XI2C_30)) { return false; }
#endif  // SPS30_I2C

  bool result = false;

  if (FUNC_INIT == function) {
    Sps30Init();
  }
  else if (SPS30DATA) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        Sps30EverySecond();
        break;
      case FUNC_JSON_APPEND:
        Sps30Show(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        Sps30Show(0);
        break;
#endif  // USE_WEBSERVER
#ifdef SPS30_ENABLE_SLEEP
      case FUNC_SAVE_BEFORE_RESTART:
        Sps30Wakeup();         // Need to wakeup before a restart as otherwise SPS30 is not recognised after restart
        break;
#endif  // SPS30_ENABLE_SLEEP
      case FUNC_COMMAND_SENSOR:
        if (XSNS_44 == XdrvMailbox.index) {
          result = Sps30Command();
        }
        break;
    }
  }
  return result;
}

#endif  // USE_SPS30_SHDLC || (USE_I2C && USE_SPS30)
