/*
  xdrv_98_product_test.ino - Product test for Sonoff-Tasmota

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

//#define PRODUCT_TEST                        // Enable this to include Tuya product test protocol
#define PRODUCT_TEST_DEBUG                  // Enable to compile status messages as shown below

#define PRODUCT_TEST_SSID        "FACILITY_SSID"   // <-- CHANGE THIS TO YOURS

#define PRODUCT_VOLTAGE          "220.0"    // Volt
#define PRODUCT_CURRENT          "392.0"    // milliAmpere
#define PRODUCT_POWER            "87.0"     // Watt
#define PRODUCT_CALIBRATE_CYCLE  20         // Seconds - Need at least 8 seconds

#define PRODUCT_SCAN_INTERVAL    8          // Seconds
#define PRODUCT_SCAN_RETRY       4          // Scan retry count

#define PRODUCT_TEST_VERSION     "3.2"

/*********************************************************************************************\
This code is supported starting with Tasmota version 9.2.0.
The product test will run at power on when both SSids are empty.
The test can be restarted by power cycling off then on.

*** Flash firmware
Write binary firmware.bin
  esptool.py --port COM4 write_flash -fs 1MB -fm dout 0x0 firmware.bin

*** Typical succesful product test with succesful wifi scan
Initial Power ON
  00:00:00 CFG: Use defaults
  00:00:00 TST: Product test v3.0
  00:00:00 TST: 1/6 Product test started using ssid tuya_mdev_test1
  00:00:00 SRC: Restart
  00:00:00 Project sonoff Sonoff Version 6.4.1.19(basic)-2_4_2
  00:00:00 CFG: Saved to flash at FA, Count 2, Bytes 3584
  00:00:00 WIF: WifiManager active for 3 minutes
  00:00:00 WIF: Wifimanager set AccessPoint
  00:00:00 HTP: Web server active on sonoff-2927 with IP address 192.168.4.1
  00:00:01 CFG: Saved to flash at F9, Count 3, Bytes 3584
  00:00:04 TST: Scanned SSId1 indebuurt1
  00:00:04 TST: Scanned SSId2 indebuurt2
  00:00:04 TST: Scanned SSId3 indebuurt3
  00:00:04 TST: Scanned SSId4 tuya_mdev_test1
  00:00:04 TST: 2/6 Wifi found
  00:00:13 APP: Boot Count 1
Button pressed
  00:00:15 TST: 3/6 Button pressed first time - Toggle relay and led once
  00:00:15 APP: Button1 immediate
  00:00:15 SRC: Button
  00:00:15 RSL: stat/sonoff/RESULT = {"POWER":"ON"}
  00:00:15 RSL: stat/sonoff/POWER = ON
Button pressed again
  00:00:20 TST: 4/6 Button pressed second time - Toggle relay and led three times
  00:00:21 RSL: stat/sonoff/RESULT = {"POWER":"OFF"}
  00:00:21 RSL: stat/sonoff/POWER = OFF
  00:00:22 RSL: stat/sonoff/RESULT = {"POWER":"ON"}
  00:00:22 RSL: stat/sonoff/POWER = ON
  00:00:23 RSL: stat/sonoff/RESULT = {"POWER":"OFF"}
  00:00:23 RSL: stat/sonoff/POWER = OFF
  00:00:24 TST: 5/6 Button test finished - Start calibration
  00:00:27 CFG: Saved to flash at FB, Count 4, Bytes 3584
  00:00:27 TST: 6/6 Product test finished - Save to power off
Device can and should be powered off now as it is in an unknown state

*** Restore Product test prerequisite
  Power off device
\*********************************************************************************************/

#ifdef PRODUCT_TEST
/*********************************************************************************************\
 * Product test procedure support
 *
 * - Initiate test if power on and no user SSIDs configured
 * - Init test environment (Led OFF, flash save off)
 * - Wait for wifi scan to find PRODUCT_TEST_SSID then start flashing led every 250 mSec
 * - Wait for user press button then turn relay and led ON
 * - Wait for user press button again then toggle relay and led 3 times and increment bootcount
 * - If energy monitoring is detected calibrate Voltage, Current and Power
 * - Wait for power down
\*********************************************************************************************/

/*********************************************************************************************\
 * Examples to test a particular device:
 *
 * Set in user_config_override.h the following:
 *
 * #undef MODULE
 * #define MODULE                      SONOFF_BASIC   // [Module] Select default model (the list is kModuleNiceList() in file tasmota_template.h)
 * #undef FALLBACK_MODULE
 * #define FALLBACK_MODULE             SONOFF_BASIC   // to Select the default model as FALLBACK when the user does a RESET 1
 *
 * or
 *
 * #define USER_TEMPLATE "{\"NAME\":\"Shelly 2.5\",\"GPIO\":[320,0,32,0,224,193,0,0,640,192,608,225,3456,4736],\"FLAG\":0,\"BASE\":18}"  // [Template] Set JSON template
 *
 * For ESP32 examples check user_config.h
 *
\*********************************************************************************************/


#define XDRV_98             98

enum ProductTestStates { PROD_TEST_IDLE, PROD_TEST_START, PROD_TEST_WIFI, PROD_TEST_BUTTON, PROD_TEST_CALIBRATE, PROD_TEST_DONE };

struct PRODUCTTEST {
  uint8_t state = PROD_TEST_IDLE;    // No product test
  uint8_t blink = 3;                 // Toggle relay three times
  uint8_t button_step = 0;           // Either initial power toggle or end of test
  uint8_t last_button = 0;           // Store initial power toggle last button state
  uint8_t scan_interval = 2;         // Seconds after power on
  uint8_t scan_retry = PRODUCT_SCAN_RETRY +1;
  uint8_t calibrate_state = PRODUCT_CALIBRATE_CYCLE;  // Calibrate cycle max seconds
  uint8_t calibrate_timer = 0;       // 100mS timer

  uint16_t save_data_counter;
  uint8_t ledstate;
  uint8_t button_single;
  uint8_t global_state;
} prodtest;

bool ProductTestTrigger(void)
{
//  AddLog(LOG_LEVEL_INFO, PSTR("TST: resetInfo %d, sta_ssid1 (%s), sta_ssid2 (%s), save_flag %d"), resetInfo.reason, SettingsText(SET_STASSID1), SettingsText(SET_STASSID2), Settings.save_flag);

  if (((resetInfo.reason == REASON_DEFAULT_RST) || (resetInfo.reason == REASON_EXT_SYS_RST)) &&     // Power on
      !strlen(SettingsText(SET_STASSID1)) &&              // No configured SSID1
      !strlen(SettingsText(SET_STASSID2))                 // No configured SSID2
     ) {
    return true;
  }

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
        Settings.ledstate = prodtest.ledstate;
        Settings.flag.global_state = prodtest.global_state;
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
  if (ProductTestTrigger()) {
#ifdef PRODUCT_TEST_DEBUG
    TasmotaGlobal.seriallog_level = 3;                 // Enable more serial logging
    AddLog(LOG_LEVEL_INFO, PSTR("TST: Product test v" PRODUCT_TEST_VERSION));
#endif
    // Prepare for product test procedure
    prodtest.save_data_counter = TasmotaGlobal.save_data_counter;
    TasmotaGlobal.save_data_counter = 32000;           // Stop auto saving data - Updating Settings
    prodtest.global_state = Settings.flag.global_state;
    Settings.flag.global_state = 1;      // Disable global state (wifi and mqtt) led blinks as it interferes with the test protocol
    prodtest.ledstate = Settings.ledstate;
    Settings.ledstate = 0;               // Disable led power state as it interferes with the test protocol

    ProductTestLedOff();                 // Signal start of test
#ifdef PRODUCT_TEST_DEBUG
    AddLog(LOG_LEVEL_INFO, PSTR("TST: 1/6 Product test started using ssid " PRODUCT_TEST_SSID));
#endif  // PRODUCT_TEST_DEBUG
    prodtest.state = PROD_TEST_START;
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
          Settings.ledstate = 1;         // Enable led power state
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
  if (PROD_TEST_IDLE == prodtest.state) {
    return;
  }
  else if (PROD_TEST_START == prodtest.state) {
    TasmotaGlobal.blinks = 0;                          // Disable blinks initiated by WifiManager
    if (ProductTestSsidFound()) {
#ifdef PRODUCT_TEST_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("TST: 2/6 Wifi found"));
#endif
      TasmotaGlobal.sleep = 0;                         // Disable sleep
      TasmotaGlobal.restart_flag = 0;                  // No restart and enable blinks
      TasmotaGlobal.ota_state_flag = 0;                // No OTA and enable blinks

      prodtest.button_single = Settings.flag.button_single;
      Settings.flag.button_single = 1;   // Allow only single press

      TasmotaGlobal.blinks = 255;                    // Signal wifi connection with blinks
      prodtest.state = PROD_TEST_WIFI;
    }
  }
  else if (PROD_TEST_BUTTON == prodtest.state) {
    if (prodtest.blink) {
      prodtest.blink--;
      ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);  // Toggle relay and led
    } else {
      if (TasmotaGlobal.energy_driver != ENERGY_NONE) {
        prodtest.state = PROD_TEST_CALIBRATE;
      } else {
#ifdef PRODUCT_TEST_DEBUG
        AddLog(LOG_LEVEL_INFO, PSTR("TST: 5/6 Button test finished"));
#endif
        prodtest.state = PROD_TEST_DONE;
      }
    }
  }
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
        uint8_t all_done = 0;
        if (HLW_PREF_PULSE == Settings.energy_power_calibration) {
          char set_value[] = PRODUCT_POWER;
          XdrvMailbox.data = set_value;
          XdrvMailbox.data_len = sizeof(set_value);
          Energy.command_code = CMND_POWERSET;
          HlwCommand();                // Get calibrated setting
        } else {
          all_done++;
        }
        if (HLW_UREF_PULSE == Settings.energy_voltage_calibration) {
          char set_value[] = PRODUCT_VOLTAGE;
          XdrvMailbox.data = set_value;
          XdrvMailbox.data_len = sizeof(set_value);
          Energy.command_code = CMND_VOLTAGESET;
          HlwCommand();                // Get calibrated setting
        } else {
          all_done++;
        }
        if (HLW_IREF_PULSE == Settings.energy_current_calibration) {
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
      }
      else if (1 == prodtest.calibrate_state) {
        ExecuteCommandPower(1, POWER_OFF, SRC_IGNORE);  // Off relay and led - stop energy measurment
      }
      else if (0 == prodtest.calibrate_state) {
        // Save calibration data
        uint32_t power_calibration = Settings.energy_power_calibration;
        uint32_t voltage_calibration = Settings.energy_voltage_calibration;
        uint32_t current_calibration = Settings.energy_current_calibration;
        SettingsDefaultSet1();         // Reset any Settings change
        SettingsDefaultSet2();         // Init default settings
        // Store calibration data
        Settings.energy_power_calibration = power_calibration;
        Settings.energy_voltage_calibration = voltage_calibration;
        Settings.energy_current_calibration = current_calibration;
        SettingsSave(2);
        TasmotaGlobal.save_data_counter = 32000;     // Stop auto saving data - Updating Settings
      }
    } else {
      prodtest.state = PROD_TEST_DONE;
      SetLedLink(1);                   // Wifi led on - Succesful calibration
    }
  }
  else if (PROD_TEST_DONE == prodtest.state) {
#ifdef PRODUCT_TEST_DEBUG
      AddLog(LOG_LEVEL_INFO, PSTR("TST: 6/6 Product test finished - Save to power off"));
#endif

    while (1) {
      OsWatchLoop();                   // Feed OsWatch timer to prevent restart
      delay(50);                       // Satisfy SDK
    }
  }
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

bool Xdrv98(uint8_t function)
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
