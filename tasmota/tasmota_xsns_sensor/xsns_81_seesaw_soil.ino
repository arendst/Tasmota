/*
  xsns_81_seesaw_soil - I2C Capacitance & temperature sensor support for Tasmota

  Copyright (C) 2021  Wayne Ross, Theo Arends, Peter Franck

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
#ifdef USE_SEESAW_SOIL

/*********************************************************************************************\
 * SEESAW_SOIL - Capacitive Soil Moisture & Temperature Sensor
 *
 * I2C Address: 0x36, 0x37, 0x38, 0x39
 *
 * NOTE:  #define SEESAW_SOIL_PUBLISH enables immediate MQTT on soil moisture change
 *        otherwise the moisture value will only be emitted every TelePeriod
 *        #define SEESAW_SOIL_RAW enables displaying analog capacitance input in the
 *        web page for calibration purposes
 *        #define SEESAW_SOIL_PERSISTENT_NAMING to get sensor names indexed by I2C address
 *        (e.g., SeeSoil-36 instead of SeeSoil-1) for consistent naming across restarts
\*********************************************************************************************/

// I2C commands
#define SOIL_COMMAND_TEMP                0x04
#define SOIL_COMMAND_MOIST               0x08
// I2C delays
#define SOIL_DELAY_TEMP                  1           // ms delay between command and reading
#define SOIL_DELAY_MOIST                 5           // ms delay between command and reading

// Convert capacitance into a moisture.
// From observation, a free air reading is at 320, immersed in tap water, reading is 1014
// So let's make a scale that converts those (apparent) facts into a percentage
#define MAX_CAPACITANCE 1020.0f                      // subject to calibration
#define MIN_CAPACITANCE 320                          // subject to calibration
#define CAP_TO_MOIST(c) ((max((int)(c),MIN_CAPACITANCE)-MIN_CAPACITANCE)/(MAX_CAPACITANCE-MIN_CAPACITANCE)*100)

struct SeesawSoil : public SeesawDevice {
  SeesawSoil(uint8_t addr) : SeesawDevice(addr), temperature(NAN), moisture(NAN), state(STATE_IDLE) {
    type = SEESAW_TYPE_SOIL;
#ifdef SEESAW_SOIL_RAW
    capacitance = 0;
#endif
#ifdef SEESAW_SOIL_PUBLISH
    old_moist = 0;
    first_handler_call = true;
#endif
  }

  virtual void Init() override {
    // Device already reset by manager
    state = STATE_COMMAND_TEMP;
    state_time = millis();
    valid = true;
  }

  virtual void Read() override {
    uint32_t time_diff = millis() - state_time;

    switch (state) {
      case STATE_COMMAND_TEMP:
        SendCommand(SOIL_COMMAND_TEMP);
        state = STATE_READ_TEMP;
        break;

      case STATE_READ_TEMP:
        if (time_diff < SOIL_DELAY_TEMP) { return; }
        ReadTemperature();
        state = STATE_COMMAND_MOIST;
        break;

      case STATE_COMMAND_MOIST:
        SendCommand(SOIL_COMMAND_MOIST);
        state = STATE_READ_MOIST;
        break;

      case STATE_READ_MOIST:
        if (time_diff < SOIL_DELAY_MOIST) { return; }
        ReadMoisture();
        state = STATE_COMMAND_TEMP;
        break;

      case STATE_IDLE:
      default:
        state = STATE_COMMAND_TEMP;
        break;
    }
    state_time = millis();
  }

  virtual void Show(bool json, const char *name) override {
    // Store name for use in Handler() and debug logging
    strlcpy(device_name, name, sizeof(device_name));

    if (json) {
      ResponseAppend_P(PSTR(",\"%s\":{\"" D_JSON_ID "\":\"%02X\",\"" D_JSON_TEMPERATURE "\":%*_f,\"" D_JSON_MOISTURE "\":%u}"),
        name, address,
        Settings->flag2.temperature_resolution, &temperature,
        (uint32_t) moisture);
#ifdef USE_DOMOTICZ
      if (0 == TasmotaGlobal.tele_period) {
        DomoticzTempHumPressureSensor(temperature, moisture, -42.0f);
      }
#endif  // USE_DOMOTICZ
#ifdef USE_KNX
      if (0 == TasmotaGlobal.tele_period) {
        KnxSensor(KNX_TEMPERATURE, temperature);
        KnxSensor(KNX_HUMIDITY, moisture);
      }
#endif // USE_KNX
#ifdef USE_WEBSERVER
    } else {
#ifdef SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_ANALOG, name, 0, capacitance);
#endif // SEESAW_SOIL_RAW
      WSContentSend_PD(HTTP_SNS_MOISTURE, name, (uint32_t) moisture);
      WSContentSend_Temp(name, temperature);
#endif // USE_WEBSERVER
    }
  }

#ifdef SEESAW_SOIL_PUBLISH
  virtual void Handler() override {
    // Publish immediately on moisture change
    if (first_handler_call) {
      first_handler_call = false;
      old_moist = (uint32_t) moisture;
    } else {
      if ((uint32_t) moisture != old_moist) {
        Response_P(PSTR("{"));
        Show(true, device_name);
        ResponseJsonEnd();
        MqttPublishTeleSensor();
        old_moist = (uint32_t) moisture;
      }
    }
  }
#endif // SEESAW_SOIL_PUBLISH

  static const char id[] PROGMEM;

private:
  void SendCommand(uint32_t command) {
    uint8_t regHigh = SEESAW_STATUS_BASE;
    uint8_t regLow;

    switch (command) {
      case SOIL_COMMAND_TEMP:
        regLow = SEESAW_STATUS_TEMP;
        break;
      case SOIL_COMMAND_MOIST:
        regHigh = SEESAW_TOUCH_BASE;
        regLow = SEESAW_TOUCH_CHANNEL_OFFSET;
        break;
      default:
#ifdef DEBUG_SEESAW_SOIL
        AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: ILL CMD:%02X"), command);
#endif
        return;
    }

    Wire.beginTransmission(address);
    Wire.write(regHigh);
    Wire.write(regLow);
    Wire.endTransmission();

#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: ADDR=%02X CMD=%02X"), address, command);
#endif
  }

  void ReadTemperature() {
    uint8_t buf[4];
    bzero(buf, sizeof(buf));
    if (4 != Wire.requestFrom(address, (uint8_t)4)) { return; }

    for (int i = 0; i < 4; i++) {
      buf[i] = Wire.read();
    }

    int32_t ret = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) |
                  ((uint32_t)buf[2] << 8) | (uint32_t)buf[3];
    temperature = ConvertTemp((1.0 / (1UL << 16)) * ret);

#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: READ TEMP ADDR=%02X RET=%X"), address, ret);
#endif
  }

  void ReadMoisture() {
    uint8_t buf[2];
    bzero(buf, sizeof(buf));
    if (2 != Wire.requestFrom(address, (uint8_t)2)) { return; }

    for (int i = 0; i < 2; i++) {
      buf[i] = Wire.read();
    }

    int32_t ret = (uint32_t)buf[0] << 8 | (uint32_t)buf[1];
    moisture = CAP_TO_MOIST(ret);

#ifdef SEESAW_SOIL_RAW
    capacitance = ret;
#endif

#ifdef DEBUG_SEESAW_SOIL
    AddLog(LOG_LEVEL_DEBUG, PSTR("SEE: READ MOIST ADDR=%02X RET=%X"), address, ret);
#endif
  }

  enum State {
    STATE_IDLE,
    STATE_COMMAND_TEMP,
    STATE_READ_TEMP,
    STATE_COMMAND_MOIST,
    STATE_READ_MOIST
  };

  float temperature;
  float moisture;
#ifdef SEESAW_SOIL_RAW
  uint16_t capacitance;
#endif
  State state;
  uint32_t state_time;
#ifdef SEESAW_SOIL_PUBLISH
  uint16_t old_moist;
  bool first_handler_call;
#endif
};

const char SeesawSoil::id[] PROGMEM = "SOIL";

// Factory function implementation
SeesawDevice* SeesawManager::CreateSoilDevice(uint8_t addr) {
  return new SeesawSoil(addr);
}

#endif  // USE_SEESAW_SOIL
#endif  // USE_I2C
