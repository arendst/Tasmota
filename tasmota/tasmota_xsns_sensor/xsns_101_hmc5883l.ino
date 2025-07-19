/*
  xsns_101_hmc5883l.ino - HMC5883L 3-Axis Digital Compass sensor support for Tasmota

  Copyright (C) 2022  Andreas Achtzehn (inspired by Helge Scheunemann)

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
#ifdef USE_HMC5883L
/*********************************************************************************************\
 * HMC5883L is 3-Axis Digital Compass sensor
 *
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf
 * I2C Address: 0x1E
\*********************************************************************************************/

// Define driver ID
#define XSNS_101                        101
#define XI2C_73                         73  // See I2CDEVICES.md

/* The default I2C address of this chip */
#define HMC5883L_ADDR                   0x1E

/* Register locations */
#define HMC5883L_X_LSB                  0x04
#define HMC5883L_X_MSB                  0x03
#define HMC5883L_Y_LSB                  0x08
#define HMC5883L_Y_MSB                  0x07
#define HMC5883L_Z_LSB                  0x06
#define HMC5883L_Z_MSB                  0x05
#define HMC5883L_STATUS                 0x09
#define HMC5883L_CONFIG_A               0x00
#define HMC5883L_CONFIG_B               0x01
#define HMC5883L_MODE                   0x02
#define HMC5883L_CHIP_ID_A              0x0A
#define HMC5883L_CHIP_ID_B              0x0B
#define HMC5883L_CHIP_ID_C              0x0C

/* Bit values for the STATUS register */
const uint8_t HMC5883L_STATUS_RDY             	= 0b00000001;
const uint8_t HMC5883L_STATUS_LOCK            	= 0b00000010;

/* Modes for the sampling in the MODE register */
const uint8_t HMC5883L_MODE_CONT		= 0b00000000;
const uint8_t HMC5883L_MODE_SINGLE		= 0b00000001;
const uint8_t HMC5883L_MODE_IDLE		= 0b00000010;

/* Gain value mask for CONFIG B register */
const uint8_t HMC5883L_CONFIG_B_GAIN_MASK	= 0b11100000; // shift operation, values 0-7
const uint8_t HMC5883L_CONFIG_B_GAIN_SHIFT	= 5;

/* Averaging value for CONFIG A register: pow(2,N) */
const uint8_t HMC5883L_CONFIG_A_AVG_MASK	= 0b01100000;
const uint8_t HMC5883L_CONFIG_A_AVG_SHIFT	= 5;

/* Data output rate */
const uint8_t HMC5883L_CONFIG_A_RATE_MASK	= 0b00011100;
const uint8_t HMC5883L_CONFIG_A_RATE_SHIFT	= 2;

/* Data measurement mode */
const uint8_t HMC5883L_CONFIG_A_MMODE_NORM	= 0;
const uint8_t HMC5883L_CONFIG_A_MMODE_POSBIAS 	= 1;
const uint8_t HMC5883L_CONFIG_A_MMODE_NEGBIAS 	= 2;
const uint8_t HMC5883L_CONFIG_A_MMODE_MASK    	= 0b00000011;
const uint8_t HMC5883L_CONFIG_A_MMODE_SHIFT	= 0;

/* Data output X register A contains the MSB from the measurement result,
and data output X register B contains the LSB from the measurement result. The value stored in these two registers is a
16-bit value in 2’s complement form, whose range is 0xF800 to 0x07FF. */



    // data field
struct HMC5883L_s {
  int16_t   MX, MY, MZ;
  uint16_t  magnitude;
  int8_t measurement_mode;
  int8_t data_rate;
  int8_t average_mode;
  int8_t gain;
  int8_t mode;
} *HMC5883L = nullptr;


// Change configuration registers of the device
bool HMC5883L_SetConfig() {
  if ( HMC5883L == nullptr ) { return false; }

  uint8_t cfgA = (( (HMC5883L->measurement_mode) << HMC5883L_CONFIG_A_MMODE_SHIFT ) & HMC5883L_CONFIG_A_MMODE_MASK ) |
                 (( (HMC5883L->data_rate       ) << HMC5883L_CONFIG_A_RATE_SHIFT  ) & HMC5883L_CONFIG_A_RATE_MASK  ) |
                 (( (HMC5883L->average_mode    ) << HMC5883L_CONFIG_A_AVG_SHIFT   ) & HMC5883L_CONFIG_A_AVG_MASK   );

  uint8_t cfgB = (( (HMC5883L->gain            ) << HMC5883L_CONFIG_B_GAIN_SHIFT  ) & HMC5883L_CONFIG_B_GAIN_MASK  );

  AddLog(LOG_LEVEL_INFO,"HMC: CONFIG A: %#X CONFIG B: %#X MODE: %#X", cfgA, cfgB, HMC5883L->mode);

  if (I2cWrite8(HMC5883L_ADDR, HMC5883L_CONFIG_A, cfgA ) == false) {
    AddLog(LOG_LEVEL_INFO,"HMC: Setting CONFIG A failed");
    return false;
  }
  if (I2cWrite8(HMC5883L_ADDR, HMC5883L_CONFIG_B, cfgB ) == false) {
    AddLog(LOG_LEVEL_INFO,"HMC: Setting CONFIG B failed");
    return false;
  }
  if (HMC5883L->mode == HMC5883L_MODE_CONT) {
    if (I2cWrite8(HMC5883L_ADDR, HMC5883L_MODE, HMC5883L_MODE_CONT ) == false) {
      AddLog(LOG_LEVEL_INFO,"HMC: Setting continuous mode failed");
      return false;
    }
  }
  return true;
}

// Initialize the device
void HMC5883L_Init() {
  if (!I2cSetDevice(HMC5883L_ADDR)) { return; }

  HMC5883L = (HMC5883L_s *)calloc(1, sizeof(struct HMC5883L_s));
  // standard configuration
  HMC5883L->gain = 5;
  HMC5883L->average_mode = 3;
  HMC5883L->data_rate = 2;
  HMC5883L->measurement_mode = HMC5883L_CONFIG_A_MMODE_NORM;
  HMC5883L->mode = HMC5883L_MODE_SINGLE;

  HMC5883L_SetConfig();

  I2cSetActiveFound(HMC5883L_ADDR, "HMC5883L");
}

//Read the magnetic data
void HMC5883L_ReadData(void) {
  if (HMC5883L->mode == HMC5883L_MODE_SINGLE) {
    if (I2cWrite8(HMC5883L_ADDR, HMC5883L_MODE, HMC5883L_MODE_SINGLE ) == false) { return; }
  }

  uint32_t timeout = millis() + 20;
  while (!(I2cRead8(HMC5883L_ADDR, HMC5883L_STATUS) & HMC5883L_STATUS_RDY)) {
    if (millis() > timeout) { return; }  // Chip not yet ready, next round try again
  }

  HMC5883L->MX = I2cReadS16(HMC5883L_ADDR, HMC5883L_X_MSB);  // Select starting with MSB register
  HMC5883L->MY = I2cReadS16(HMC5883L_ADDR, HMC5883L_Y_MSB);
  HMC5883L->MZ = I2cReadS16(HMC5883L_ADDR, HMC5883L_Z_MSB);

  // calculate magnetic induction magnitude
  HMC5883L->magnitude = SqrtInt((HMC5883L->MX * HMC5883L->MX) + (HMC5883L->MY * HMC5883L->MY) + (HMC5883L->MZ * HMC5883L->MZ));
}

/*********************************************************************************************\
 * Presentation
\*********************************************************************************************/

#ifdef USE_WEBSERVER
const char HTTP_SNS_HMC5883L[] PROGMEM =
    "{s}HMC5883L " D_MX "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}HMC5883L " D_MY "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}HMC5883L " D_MZ "{m}%d " D_UNIT_MICROTESLA "{e}"             // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
    "{s}HMC5883L " D_MAGNETICFLD "{m}%d " D_UNIT_MICROTESLA "{e}";   // {s} = <tr><th>, {m} = </th><td>, {e} = </td></tr>
#endif

void HMC5883L_Show(uint8_t json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"HMC5883L\":{\"" D_JSON_MX "\":%d,\"" D_JSON_MY "\":%d,\"" D_JSON_MZ "\":%d,\"" D_JSON_MAGNETICFLD "\":%u}"),
      HMC5883L->MX, HMC5883L->MY, HMC5883L->MZ, HMC5883L->magnitude);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_HMC5883L, HMC5883L->MX, HMC5883L->MY, HMC5883L->MZ, HMC5883L->magnitude);
#endif
  }
}

// Process configuration commands
bool HMC5883L_Command() {
  bool commandKnown = false;
  char cmd[20];
  char ss2[20];

  subStr(cmd, XdrvMailbox.data, ",", 1);
  int8_t value = atoi(subStr(ss2, XdrvMailbox.data, ",", 2));

  if (strcmp(cmd,"GAIN")) {
    HMC5883L->gain = value;
    commandKnown = true;
  }
  if (strcmp(cmd,"AVG")) {
    HMC5883L->average_mode = value;
    commandKnown = true;
  }
  if (strcmp(cmd,"RATE")) {
    HMC5883L->data_rate = value;
    commandKnown = true;
  }
  if (strcmp(cmd,"MMODE")) {
    HMC5883L->measurement_mode = value;
    commandKnown = true;
  }

  //AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_I2C "HMC: cmd: (%s) value: %d cmdKnown: %d"), cmd, value,commandKnown);

  if (commandKnown == false) { return false; }

  AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_I2C "HMC: Reconfiguring."));

  return HMC5883L_SetConfig();
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xsns101(uint32_t function) {
  if (!I2cEnabled(XI2C_73)) { return false; }

  bool result = false;

  if (FUNC_INIT == function) {
    HMC5883L_Init();
  }
  else if (HMC5883L != nullptr) {
    switch (function) {
      case FUNC_COMMAND_SENSOR:
        if (XSNS_101 == XdrvMailbox.index) {
          result = HMC5883L_Command();  // Return true on success
        }
        break;
      case FUNC_JSON_APPEND:
      	HMC5883L_Show(1);
      	break;
      case FUNC_EVERY_SECOND:
      	HMC5883L_ReadData();
      	break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
      	HMC5883L_Show(0);
      	break;
#endif  // USE_WEBSERVER
    	}
  }
  return result;
}
#endif  // USE_HMC5883L
#endif  // USE_I2C
