/*
  xdrv_97_product_test.ino - Product Test Driver for Tasmota

  Copyright (C) 2021  Theo Arends and Adrian Scillato

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

/*********************************************************************************************\
 * Product Test Procedure Support
 *
 * - Initiate test if power on and no user SSIds configured
 * - Init test environment (Led OFF, flash save off)
 * - Wait for Wi-Fi scan to find PRODUCT_TEST_SSId then start flashing led every 250 mSec
 * - Wait for user press button then turn relay and led ON
 * - Wait for user press button again then toggle relay and led 3 times and increment bootcount
 * - If energy monitoring is detected calibrate Voltage, Current and Power
 * - Wait for power down
 * 
 * The test can be restarted by power cycling off then on.
 * 
 *********************************************************************************************
 * This code is supported with Tasmota version 9.5.0 or greater
\*********************************************************************************************/

/*********************************************************************************************
* INSTRUCTIONS:
*
*   To test a particular device, COPY to "user_config_override.h" the following lines and
*   edit them accordingly to your device.
* 
**********************************************************************************************

#define PRODUCT_TEST                            // Enable this to include product test in the firmware

#define PRODUCT_TEST_SSID        "Factory_WiFi" // <-- CHANGE THIS TO YOURS

#define PRODUCT_VOLTAGE          "125.0"        // Voltage for the Calibration Load
#define PRODUCT_CURRENT          "5900.0"        // milliAmpere for the Calibration Load
#define PRODUCT_POWER            "750.0"         // Watt for the Calibration Load (Remember that must be a resistive load)

// Define the device to use. Set the GPIOs template of the device from https://templates.blakadder.com/
#define USER_TEMPLATE    "{\"NAME\":\"Shelly 2.5\",\"GPIO\":[320,0,32,0,224,193,0,0,640,192,608,225,3456,4736],\"FLAG\":0,\"BASE\":18}"  // [Template] Set JSON template

// Make Tasmota to use as Default the USER_TEMPLATE
#ifdef MODULE
#undef MODULE
#endif
#define MODULE           USER_MODULE
#ifdef FALLBACK_MODULE
#undef FALLBACK_MODULE
#endif
#define FALLBACK_MODULE   USER_MODULE

**********************************************************************************************
** Then Compile and Flash
**********************************************************************************************

Write binary firmware.bin with Tasmotizer, or with ESPTool as:
  esptool.py --port COM5 write_flash -fs 1MB -fm dout 0x0 firmware.bin

**********************************************************************************************
** Typical successful product test with successful Wi-Fi scan and power calibration
**********************************************************************************************

Initial Power ON
  00:00:00.001 HDW: ESP8266EX
  00:00:00.038 CFG: Loaded from flash at F7, Count 5
  00:00:00.043 QPC: Count 1
  00:00:00.046 CFG: CR 358/699, Busy 0
  00:00:00.049 CFG: CR 358/699, Busy 0
  00:00:00.151 I2C: ADE7953 found at 0x38
  00:00:00.153 TST: Product test v3.7.1
  00:00:00.062 TST: Num of Inputs: 1, Num of PWMs: 0
  00:00:00.153 TST: 1/6 Product test started using ssid FACTORY_WIFI
  00:00:00.154 SRC: Restart
  00:00:00.157 Project tasmota Tasmota Version 9.5.0.7(tasmota)-2_7_4_9(2021-08-20T16:27:33)
  00:00:00.215 WIF: WifiManager active for 3 minutes
  00:00:00.216 WIF: Wifimanager set AccessPoint
  00:00:00.723 HTP: Web server active on tasmota_FA2DF8-3576 with IP address 192.168.4.1
  00:00:03.129 TST: Scanned SSId1 HOMEWIFI
  00:00:03.130 TST: Scanned SSId2 MrWIFI
  00:00:03.131 TST: Scanned SSId3 FACTORY_WIFI
  00:00:03.165 TST: 2/6 Wifi found
  00:00:09.332 QPC: Reset
Button pressed
  00:00:29.064 TST: 3/6 Button pressed first time - Toggle relay and led once
  00:00:29.066 APP: Button1 immediate
  00:00:29.068 SRC: Button
  00:00:29.070 RSL: RESULT = {"POWER1":"ON"}
  00:00:29.072 RSL: POWER1 = ON
Button pressed again
  00:00:31.664 TST: 4/6 Button pressed second time - Toggle relay and led three times
  00:00:32.298 RSL: RESULT = {"POWER1":"OFF"}
  00:00:32.300 RSL: POWER1 = OFF
  00:00:33.298 RSL: RESULT = {"POWER1":"ON"}
  00:00:33.300 RSL: POWER1 = ON
  00:00:34.298 RSL: RESULT = {"POWER1":"OFF"}
  00:00:34.300 RSL: POWER1 = OFF
  00:00:38.093 TST: 5/6 Start calibration
  00:00:38.096 RSL: RESULT = {"POWER1":"ON"}
  00:00:38.098 RSL: POWER1 = ON
  00:00:41.297 TST: UnCalibrated Readings. Voltage: 125, Current: 5.924, Power: 746
  00:00:42.297 TST: Calibrated Readings. Voltage: 125, Current: 5.914, Power: 745
  00:00:42.300 RSL: RESULT = {"POWER1":"OFF"}
  00:00:42.302 RSL: POWER1 = OFF
  00:00:42.309 TST: 6/6 Product test finished - Safe to power off
Device can be safely powered off now

*** Restore Product test prerequisite
  Power off device
**********************************************************************************************/
#ifdef PRODUCT_TEST

#define PRODUCT_TEST_DEBUG                      // Enable to show driver status messages by serial as shown above

#define ENERGY_DRIVER_CALIBRATION               // Comment this if you don't want to calibrate the power measurement.

#define PRODUCT_CALIBRATE_CYCLE  20             // Seconds - Need at least 8 seconds

#define PRODUCT_SCAN_INTERVAL    5              // Wi-Fi Scan Interval in Seconds
#define PRODUCT_SCAN_RETRY       1              // Wi-Fi Scan retry count

#define PERCENTAGE_INITIAL_LIGHT "90"
#define PERCENTAGE_TEST_LIGHT    "100"
#define CYCLES_TO_TEST_PWM       600            // 600 sec = 10 min.

#define PRODUCT_TEST_VERSION     "3.7.1B"

#define XDRV_97             97

enum ProductTestStates { PROD_TEST_IDLE, PROD_TEST_START, PROD_TEST_WIFI, PROD_TEST_BUTTON, PROD_TEST_PWM, PROD_TEST_CALIBRATE, PROD_TEST_DONE };

struct PRODUCTTEST {
  uint8_t state = PROD_TEST_IDLE;    // No product test
  uint8_t blink = 3;                 // Toggle relay three times
  uint8_t button_step = 0;           // Either initial power toggle or end of test
  uint8_t last_button = 0;           // Store initial power toggle last button state
  uint8_t scan_interval = 2;         // Seconds after power on
  uint8_t scan_retry = PRODUCT_SCAN_RETRY +1;
  uint8_t calibrate_state = PRODUCT_CALIBRATE_CYCLE;  // Calibrate cycle max seconds
  uint8_t calibrate_timer = 0;       // 100mS timer
  uint8_t num_inputs = 0;            // buttons, switches, touchbuttons
  uint8_t num_pwms = 0;              // pwms to test
  uint8_t pwm_at_test = 1;           // pwm at test
  uint16_t pwm_cycles_to_test = CYCLES_TO_TEST_PWM;  // See CYCLES_TO_TEST_PWM
  
  uint16_t save_data_counter;
  uint8_t ledstate;
  uint8_t button_single;
  uint8_t global_state;
} prodtest;

bool ProductTestTrigger(void)
{
//  AddLog(LOG_LEVEL_INFO, PSTR("TST: resetInfo %d, sta_ssid1 (%s), sta_ssid2 (%s), save_flag %d"), resetInfo.reason, SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), Settings->save_flag);
//  if (TasmotaGlobal.devices_present) { // Is there something to test? (Are there any GPIO definition for outputs - relays, PWM)
    if (((resetInfo.reason == REASON_DEFAULT_RST) || (resetInfo.reason == REASON_EXT_SYS_RST)) &&     // Power on
        !strlen(SettingsText(SET_STASSID1)) &&              // No configured SSID1
        !strlen(SettingsText(SET_STASSID2))                 // No configured SSID2
      ) {
      return true;
    }
//  }
  return false;
}

bool ProductTestSsidFound(void)
{
  bool result = false;

  if (prodtest.scan_interval) {
    prodtest.scan_interval--;
    if (!prodtest.scan_interval) {
      prodtest.scan_retry--;
      if (prodtest.scan_retry) {
        prodtest.scan_interval = PRODUCT_SCAN_INTERVAL;
        int n = WiFi.scanNetworks();
        if (n > 0) {
          for (int i = 0; i < n; i++) {
#ifdef PRODUCT_TEST_DEBUG
            AddLog(LOG_LEVEL_DEBUG, PSTR("TST: Scanned SSId%d %s"), i +1, WiFi.SSID(i).c_str());
#endif
            if (!strcasecmp_P(WiFi.SSID(i).c_str(), PSTR(PRODUCT_TEST_SSID))) { result = true; }
          }
        }
      } else {
        if (Wifi.config_type) {
          TasmotaGlobal.blinks = 255;                 // Re-enable wifi config blinks
        }
        Settings->ledstate = prodtest.ledstate;
        Settings->flag.global_state = prodtest.global_state;
        TasmotaGlobal.save_data_counter = prodtest.save_data_counter;
#ifdef PRODUCT_TEST_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("TST: 6/6 Product test failed"));
#endif
        prodtest.state = PROD_TEST_IDLE;
      }
    }
  }
  return result;
}

void ProductTestLedOff(void)
{
  TasmotaGlobal.blinks = 0;                            // No blinks
  SetLedLink(0);
  SetLedPower(0);
}

void ProductTestInit(void)
{
  char svalue[32];                   // Command and number parameter
  uint32_t channel;

  if (ProductTestTrigger()) {
#ifdef PRODUCT_TEST_DEBUG
    TasmotaGlobal.seriallog_level = 3;                 // Enable more serial logging
    AddLog(LOG_LEVEL_INFO, PSTR("TST: Product test v" PRODUCT_TEST_VERSION));
#endif

    // gather test data
    if (PinUsed(GPIO_KEY1_TC)) {
      prodtest.num_inputs++;
    }
    for (uint32_t i = 0; i < MAX_KEYS; i++) {
      if ((PinUsed(GPIO_KEY1, i)) || (PinUsed(GPIO_KEY1_NP, i)) || (PinUsed(GPIO_KEY1_INV, i)) || (PinUsed(GPIO_KEY1_INV_NP, i))) {
        prodtest.num_inputs++;
      }
    }
    for (uint32_t i = 0; i < MAX_SWITCHES; i++) {
      if ((PinUsed(GPIO_SWT1, i)) || (PinUsed(GPIO_SWT1_NP, i))) {
        prodtest.num_inputs++;
      }
    }
    for (uint32_t i = 0; i < MAX_PWMS; i++) {
      if (PinUsed(GPIO_PWM1, i)) {
        prodtest.num_pwms++;
      }
    }

#ifdef PRODUCT_TEST_DEBUG   
    AddLog(LOG_LEVEL_INFO, PSTR("TST: Num of Inputs: %d, Num of PWMs: %d"),prodtest.num_inputs, prodtest.num_pwms);    
#endif

    // Prepare for product test procedure
    prodtest.save_data_counter = TasmotaGlobal.save_data_counter;
    TasmotaGlobal.save_data_counter = 32000;           // Stop auto saving data - Updating Settings
    prodtest.global_state = Settings->flag.global_state;
    Settings->flag.global_state = 1;      // Disable global state (wifi and mqtt) led blinks as it interferes with the test protocol
    prodtest.ledstate = Settings->ledstate;
    Settings->ledstate = 0;               // Disable led power state as it interferes with the test protocol

    ProductTestLedOff();                 // Signal start of test
    if (prodtest.num_pwms) {             // Signal start of test if it is a Light
      channel = 1;
      if (prodtest.num_pwms>3) {
        channel = 4;
      } else if (3 == prodtest.num_pwms) {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), 3);
        ExecuteCommand(svalue, SRC_IGNORE);
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), 2);
        ExecuteCommand(svalue, SRC_IGNORE);
      }
      snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), channel);
      ExecuteCommand(svalue, SRC_IGNORE);
    }

#ifdef PRODUCT_TEST_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR("TST: 1/6 Product test started using ssid " PRODUCT_TEST_SSID));
#endif  // PRODUCT_TEST_DEBUG
    prodtest.state = PROD_TEST_START;
#ifndef ENERGY_DRIVER_CALIBRATION
    TasmotaGlobal.energy_driver = ENERGY_NONE;
#endif
  } else {
    prodtest.state = PROD_TEST_IDLE;
  }
}

bool ProductTestButton(void)
{
  bool result = false;                   // Ignore

  if (prodtest.state > PROD_TEST_IDLE) {
    result = true;                       // Disable button processing

    if (PROD_TEST_WIFI == prodtest.state) {
      if ((PRESSED == XdrvMailbox.payload) && (NOT_PRESSED == prodtest.last_button)) {  // Button pressed
        if (0 == prodtest.button_step) {
#ifdef PRODUCT_TEST_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("TST: 3/6 Button pressed first time - Toggle relay and led once"));
#endif
          ProductTestLedOff();           // Signal button pressed and activate relay
          Settings->ledstate = 1;         // Enable led power state
          prodtest.button_step++;
          result = false;                // Process normal button press which will turn relay and led on
        } else {
#ifdef PRODUCT_TEST_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("TST: 4/6 Button pressed second time - Toggle relay and led three times"));
#endif
          prodtest.state = PROD_TEST_BUTTON;
          result = true;                 // Disable further button processing
        }
      }
      prodtest.last_button = XdrvMailbox.payload;
    }
  }

  return result;
}

void ProductTestEverySecond(void)
{
  char svalue[32];                   // Command and number parameter
#ifdef PRODUCT_TEST_DEBUG
  char voltage_chr[FLOATSZ];
  char current_chr[FLOATSZ];
  char active_power_chr[FLOATSZ];
#endif
  if (PROD_TEST_IDLE == prodtest.state) {
    return;
  }
  else if (PROD_TEST_DONE == prodtest.state) {
#ifdef PRODUCT_TEST_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR("TST: 6/6 Product test finished - Safe to power off"));
#endif

    while (1) {
      OsWatchLoop();                   // Feed OsWatch timer to prevent restart
      delay(50);                       // Satisfy SDK
      SetLedLink(1);                   // Wifi led on - Successful calibration
    }
  }
  else if (PROD_TEST_START == prodtest.state) {
    TasmotaGlobal.blinks = 0;                          // Disable blinks initiated by WifiManager
    if (ProductTestSsidFound()) {
#ifdef PRODUCT_TEST_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("TST: 2/6 Wifi found"));
#endif
      TasmotaGlobal.sleep = 0;                         // Disable sleep
      TasmotaGlobal.restart_flag = 0;                  // No restart and enable blinks
      Wifi.config_counter = 200;                       // Do not restart the device
      TasmotaGlobal.ota_state_flag = 0;                // No OTA and enable blinks

      prodtest.button_single = Settings->flag.button_single;
      Settings->flag.button_single = 1;   // Allow only single press

      TasmotaGlobal.blinks = 255;                    // Signal wifi connection with blinks

      if (prodtest.num_inputs) {
        prodtest.state = PROD_TEST_WIFI;
      } else if (prodtest.num_pwms) {
        prodtest.state = PROD_TEST_PWM;
        for (uint32_t i = 1; i < prodtest.num_pwms; i++) { 
          snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d 0"), i);
          ExecuteCommand(svalue, SRC_IGNORE); 
        }       
      } else {      
  #ifdef USE_ENERGY_SENSOR
        if (TasmotaGlobal.energy_driver != ENERGY_NONE) {
          prodtest.state = PROD_TEST_CALIBRATE;
        } else {
  #endif //USE_ENERGY_SENSOR
          prodtest.state = PROD_TEST_DONE;
  #ifdef USE_ENERGY_SENSOR
        }
  #endif //USE_ENERGY_SENSOR
      }
    }
  }
  else if (PROD_TEST_PWM == prodtest.state) {
    Wifi.config_counter = 200;    // Do not restart the device
    if (prodtest.pwm_cycles_to_test) {
      prodtest.pwm_cycles_to_test--;

      snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d 0"), (!(prodtest.pwm_at_test-1) ? prodtest.num_pwms : (prodtest.pwm_at_test-1) ) );
      ExecuteCommand(svalue, SRC_IGNORE);

      if (!prodtest.pwm_cycles_to_test) { // At the end of the test, the COLD WHITE must be left ON
        if (prodtest.num_pwms) {             
          uint32_t channel = 1;
          if (prodtest.num_pwms>3) {
            channel = 4;
          } else if (3 == prodtest.num_pwms) {
            snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), 3);
            ExecuteCommand(svalue, SRC_IGNORE);
            snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), 2);
            ExecuteCommand(svalue, SRC_IGNORE);
          }
          snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_INITIAL_LIGHT), channel);
          ExecuteCommand(svalue, SRC_IGNORE);
        }
      } else {
        snprintf_P(svalue, sizeof(svalue), PSTR(D_CMND_CHANNEL "%d " PERCENTAGE_TEST_LIGHT), prodtest.pwm_at_test);
        ExecuteCommand(svalue, SRC_IGNORE);
      }

      prodtest.pwm_at_test++;
      if (prodtest.pwm_at_test > prodtest.num_pwms) {
        prodtest.pwm_at_test = 1;
      }

    } else {      
#ifdef USE_ENERGY_SENSOR
      if (TasmotaGlobal.energy_driver != ENERGY_NONE) {
        prodtest.state = PROD_TEST_CALIBRATE;
      } else {
#endif //USE_ENERGY_SENSOR
#ifdef PRODUCT_TEST_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("TST: 5/6 PWM test finished"));
#endif
        prodtest.state = PROD_TEST_DONE;
#ifdef USE_ENERGY_SENSOR
      }
#endif //USE_ENERGY_SENSOR
    }
  }  
  else if (PROD_TEST_BUTTON == prodtest.state) {
    if (prodtest.blink) {
      prodtest.blink--;
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);  // Toggle relay and led
    } else {
#ifdef USE_ENERGY_SENSOR
      if (TasmotaGlobal.energy_driver != ENERGY_NONE) {
        prodtest.state = PROD_TEST_CALIBRATE;
      } else {
#endif //USE_ENERGY_SENSOR
#ifdef PRODUCT_TEST_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("TST: 5/6 Button test finished"));
#endif
        prodtest.state = PROD_TEST_DONE;
#ifdef USE_ENERGY_SENSOR
      }
#endif //USE_ENERGY_SENSOR
    }
  }
#ifdef USE_ENERGY_SENSOR
  else if (PROD_TEST_CALIBRATE == prodtest.state) {
    if (prodtest.calibrate_state) {
      prodtest.calibrate_state--;
      if (PRODUCT_CALIBRATE_CYCLE -1 == prodtest.calibrate_state) {
#ifdef PRODUCT_TEST_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("TST: 5/6 Start calibration"));
#endif
        ExecuteCommandPower(1, POWER_ON, SRC_IGNORE);  // On relay and led - start energy measurment
      }
      else if ((prodtest.calibrate_state < PRODUCT_CALIBRATE_CYCLE -4) && (prodtest.calibrate_state > 1)) {
        // Allow a few seconds before starting calibration

        if ((Energy.voltage[0] > 0) && (Energy.current[0] > 0) && (Energy.active_power[0] > 0)) {
          // Readings are valid
#ifdef PRODUCT_TEST_DEBUG
          dtostrfd(Energy.voltage[0], Settings->flag2.voltage_resolution, voltage_chr);
          dtostrfd(Energy.current[0], Settings->flag2.current_resolution, current_chr);
          dtostrfd(Energy.active_power[0], Settings->flag2.wattage_resolution, active_power_chr);
          AddLog(LOG_LEVEL_INFO, PSTR("TST: UnCalibrated Readings. Voltage: %s, Current: %s, Power: %s"),voltage_chr,current_chr,active_power_chr);
#endif
          uint8_t all_done = 0;
          if (HLW_PREF_PULSE == Settings->energy_power_calibration) {
            char set_value[] = PRODUCT_POWER;
            XdrvMailbox.data = set_value;
            XdrvMailbox.data_len = sizeof(set_value);
            Energy.command_code = CMND_POWERSET;
            HlwCommand();                // Get calibrated setting
          } else {
            all_done++;
          }
          if (HLW_UREF_PULSE == Settings->energy_voltage_calibration) {
            char set_value[] = PRODUCT_VOLTAGE;
            XdrvMailbox.data = set_value;
            XdrvMailbox.data_len = sizeof(set_value);
            Energy.command_code = CMND_VOLTAGESET;
            HlwCommand();                // Get calibrated setting
          } else {
            all_done++;
          }
          if (HLW_IREF_PULSE == Settings->energy_current_calibration) {
            char set_value[] = PRODUCT_CURRENT;
            XdrvMailbox.data = set_value;
            XdrvMailbox.data_len = sizeof(set_value);
            Energy.command_code = CMND_CURRENTSET;
            HlwCommand();                // Get calibrated setting
          } else {
            all_done++;
          }
          if (3 == all_done) {
            prodtest.calibrate_state = 2;
          }
        } else {
        if (Wifi.config_type) {
          TasmotaGlobal.blinks = 255;                 // Re-enable wifi config blinks
        }
        Settings->ledstate = prodtest.ledstate;
        Settings->flag.global_state = prodtest.global_state;
        TasmotaGlobal.save_data_counter = prodtest.save_data_counter;
#ifdef PRODUCT_TEST_DEBUG
          AddLog(LOG_LEVEL_INFO, PSTR("TST: 6/6 Product Power Calibration failed"));
#endif
          prodtest.state = PROD_TEST_IDLE;
          ExecuteCommandPower(1, POWER_OFF, SRC_IGNORE);  // Off relay and led - stop energy measurment
        }
      }
      else if (1 == prodtest.calibrate_state) {
#ifdef PRODUCT_TEST_DEBUG
        dtostrfd(Energy.voltage[0], Settings->flag2.voltage_resolution, voltage_chr);
        dtostrfd(Energy.current[0], Settings->flag2.current_resolution, current_chr);
        dtostrfd(Energy.active_power[0], Settings->flag2.wattage_resolution, active_power_chr);
        AddLog(LOG_LEVEL_INFO, PSTR("TST: Calibrated Readings. Voltage: %s, Current: %s, Power: %s"),voltage_chr,current_chr,active_power_chr);
#endif
        ExecuteCommandPower(1, POWER_OFF, SRC_IGNORE);  // Off relay and led - stop energy measurment
      }
      else if (0 == prodtest.calibrate_state) {
        // Save calibration data
        uint32_t power_calibration = Settings->energy_power_calibration;
        uint32_t voltage_calibration = Settings->energy_voltage_calibration;
        uint32_t current_calibration = Settings->energy_current_calibration;
        SettingsDefaultSet1();         // Reset any Settings change
        SettingsDefaultSet2();         // Init default Settings
        SettingsDefaultSet3();         // Init default Settings
        // Store calibration data
        Settings->energy_power_calibration = power_calibration;
        Settings->energy_voltage_calibration = voltage_calibration;
        Settings->energy_current_calibration = current_calibration;
        SettingsSave(2);
        TasmotaGlobal.save_data_counter = 32000;     // Stop auto saving data - Updating Settings
      }
    } else {
      prodtest.state = PROD_TEST_DONE;
      SetLedLink(1);                   // Wifi led on - Successful calibration
    }
  }
#endif //USE_ENERGY_SENSOR 
}

void ProductTestEvery100mSecond(void)
{
  if (PROD_TEST_CALIBRATE == prodtest.state) {
    // Blink wifi led 2 flashes every 3 seconds - Calibrating
    prodtest.calibrate_timer++;
    if (1 == prodtest.calibrate_timer) {
      SetLedLink(0);                   // Wifi led off
    }
    else if (12 == prodtest.calibrate_timer) {
      SetLedLink(1);                   // Wifi led on
    }
    else if (16 == prodtest.calibrate_timer) {
      prodtest.calibrate_timer = 0;
    }
  }
}

/*********************************************************************************************\
 * Interface
\*********************************************************************************************/

bool Xdrv97(uint8_t function)
{
  bool result = false;

  switch (function) {
    case FUNC_EVERY_100_MSECOND:
      ProductTestEvery100mSecond();
      break;
    case FUNC_EVERY_SECOND:
      ProductTestEverySecond();
      break;
    case FUNC_BUTTON_PRESSED:
      result = ProductTestButton();
      break;
    case FUNC_PRE_INIT:
      ProductTestInit();
      break;
  }
  return result;
}

#endif  // PRODUCT_TEST
