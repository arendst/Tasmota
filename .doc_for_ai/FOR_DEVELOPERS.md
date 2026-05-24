# Tasmota Developer Guide

This file is a summary of the Tasmota Documentation for the "docs" repository. It is provided here for convenience for GenAI to read it easily.

For authoritative information, refer to:
- `docs/API.md` — driver/sensor callback IDs reference
- `docs/Sensor-API.md` — sensor driver development guide (note: some information may be outdated)
- `docs/Commands.md` — full command reference
- `docs/Berry.md` — Berry scripting language reference
- `tasmota/include/tasmota.h` — main enums and constants
- `tasmota/include/tasmota_types.h` — `TSettings` structure and other types

## How Tasmota Works

### Core Architecture

Tasmota is a modular firmware that runs on ESP8266/ESP8285 and ESP32 family microcontrollers. The architecture follows these principles:

#### 1. Event-Driven System
- A main loop dispatches events to drivers, sensors, energy and light modules through callback IDs (see `enum XsnsFunctions` in `tasmota/include/tasmota.h`).
- Non-blocking operations are required to maintain responsiveness.
- Periodic callbacks are scheduled at 50 ms, 100 ms, 200 ms, 250 ms and 1 s intervals.

#### 2. Modular Design
- Core functionality (Wi-Fi, MQTT, web interface) is always present.
- Optional features are compiled conditionally using `#define USE_xxx` directives.
- Drivers, sensors, displays, light controllers and energy monitors are organized as numbered `xdrv_##`, `xsns_##`, `xdsp_##`, `xlgt_##` and `xnrg_##` modules.

#### 3. Communication Channels
- **MQTT** — primary protocol for automation systems
- **HTTP** — web interface and REST-style command endpoint (`/cm`)
- **Serial** — console access for debugging and configuration
- **WebSocket** is not used for the standard UI; the web UI uses chunked HTTP plus periodic polling.

### Firmware Source Layout

```
tasmota/
├── tasmota.ino                    # Main firmware entry point
├── my_user_config.h               # Master compile-time configuration (do NOT edit directly)
├── user_config_override.h         # User overrides for compile-time configuration
├── user_config_override_sample.h  # Template for user_config_override.h
├── tasmota_xdrv_driver/           # Driver modules (xdrv_##_*.ino)
├── tasmota_xsns_sensor/           # Sensor modules (xsns_##_*.ino)
├── tasmota_xdsp_display/          # Display modules (xdsp_##_*.ino)
├── tasmota_xlgt_light/            # Light controller modules (xlgt_##_*.ino)
├── tasmota_xnrg_energy/           # Energy monitoring modules (xnrg_##_*.ino)
├── tasmota_xx2c_global/           # Cross-cutting / shared modules
├── tasmota_support/               # Core support code (settings, command parser, I2C, ...)
├── include/                       # Public headers (tasmota.h, tasmota_types.h, tasmota_globals.h, i18n.h, ...)
├── berry/                         # Berry VM and Berry scripts shipped with the firmware
├── language/                      # Localization headers
└── html_uncompressed/, html_compressed/  # Web UI HTML/JS sources
```

The exact number of files in each directory changes frequently as new drivers are added; do not rely on hard-coded counts.

### Configuration Override

To customize a build, copy `tasmota/user_config_override_sample.h` to `tasmota/user_config_override.h` and add your `#define` / `#undef` directives there. Do not edit `my_user_config.h` directly — it is the master configuration shipped with each release.

```c
#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// Enable optional features
#define USE_BERRY_DEBUG

// Disable unused features to save space
#undef USE_DOMOTICZ
#undef USE_KNX

#endif  // _USER_CONFIG_OVERRIDE_H_
```

### Command System

All Tasmota functionality is accessible through a unified command system:
- Commands can be sent via MQTT, HTTP (`/cm?cmnd=...`), serial, or the web console.
- Format: `Command [parameter]`. Command names are case-insensitive.
- Responses are typically JSON.
- Multiple commands can be chained with `Backlog cmd1; cmd2; cmd3`.

### GPIO Management

Tasmota assigns logical functions ("components") to physical GPIOs at runtime:
1. **Modules** — predefined hardware base configurations (`Module` command).
2. **Templates** — JSON descriptors that override module GPIO assignments (`Template` command).
3. **GPIO** command — reassigns a single component to a specific pin (`GPIO<x> <component>`).
4. No recompilation is required to change GPIO mapping.

## Driver and Sensor Development

### Naming Convention

Each module has a unique number and a callback function:
- Driver `XX` lives in `tasmota_xdrv_driver/xdrv_XX_<name>.ino` and exposes `bool XdrvXX(uint32_t function)`.
- Sensor `XX` lives in `tasmota_xsns_sensor/xsns_XX_<name>.ino` and exposes `bool XsnsXX(uint32_t function)`.
- Energy `XX` lives in `tasmota_xnrg_energy/xnrg_XX_<name>.ino` and exposes `bool XnrgXX(uint32_t function)`.
- Light `XX` lives in `tasmota_xlgt_light/xlgt_XX_<name>.ino` and exposes `bool XlgtXX(uint32_t function)`.
- Display `XX` lives in `tasmota_xdsp_display/xdsp_XX_<name>.ino` and exposes `bool XdspXX(uint32_t function)`.

The module is enabled at compile time by a `#define USE_<feature>` and declares its ID with `#define XSNS_XX  XX` (or the equivalent prefix).

### Sensor Driver Skeleton

```c
#ifdef USE_MY_SENSOR

#define XSNS_99       99      // unique sensor ID
#define XI2C_99       99      // I2C driver index, only if I2C and registered

bool MySensorDetected = false;

void MySensorInit(void) {
  // ... probe device and set MySensorDetected = true on success
}

void MySensorEverySecond(void) {
  // ... read sensor data
}

void MySensorShow(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"MySensor\":{\"Temperature\":%d}"), temperature);
#ifdef USE_WEBSERVER
  } else {
    WSContentSend_PD(HTTP_SNS_F_TEMP, "MySensor",
                     Settings->flag2.temperature_resolution,  // flag2 is SysMBitfield1 type
                     &temperature, TempUnit());
#endif  // USE_WEBSERVER
  }
}

bool Xsns99(uint32_t function) {
  if (!I2cEnabled(XI2C_99)) { return false; }   // gate by I2C registration

  bool result = false;

  if (FUNC_INIT == function) {
    MySensorInit();
  }
  else if (MySensorDetected) {
    switch (function) {
      case FUNC_EVERY_SECOND:
        MySensorEverySecond();
        break;
      case FUNC_JSON_APPEND:
        MySensorShow(true);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MySensorShow(false);
        break;
#endif
    }
  }
  return result;
}

#endif  // USE_MY_SENSOR
```

Notes:
- The `i2c_flg` global referenced by older guides no longer exists. Use `I2cEnabled(XI2C_xx)` and `I2cSetActiveFound()` to integrate with the I2C driver registry (see `tasmota/tasmota_support/support_a_i2c.ino`).
- Web sensor display strings such as `HTTP_SNS_F_TEMP`, `HTTP_SNS_HUM`, `HTTP_SNS_F_DISTANCE_CM` are declared in `tasmota/include/i18n.h`. Use those rather than inventing new ones when possible.
- `Settings->flag2` is of type `SysMBitfield1` (defined in `tasmota/include/tasmota_types.h`) containing resolution bitfields like `temperature_resolution`, `humidity_resolution`, etc.

### Callback Function Reference

The full list of callback IDs is defined in `enum XsnsFunctions` in `tasmota/include/tasmota.h`. The enum is split into two ranges by the sentinel value `FUNC_return_result = 200`:

- Values **before** `FUNC_return_result` are "fire and forget": the dispatcher ignores the return value.
- Values **after** `FUNC_return_result` are expected to return a `bool` indicating that the callback "consumed" the event (`true` stops further dispatch where applicable).

For the authoritative table of which Driver / Sensor / Display / Energy / Light slot receives which callback and in which order, see `docs/API.md`.

#### Callbacks without return value (selection)

| Function                         | Purpose                                              |
|----------------------------------|------------------------------------------------------|
| `FUNC_SETTINGS_OVERRIDE`         | Adjust default settings before they are loaded      |
| `FUNC_SETUP_RING1` / `RING2`     | Early setup phases                                   |
| `FUNC_PRE_INIT`                  | Once GPIOs have been established                     |
| `FUNC_INIT`                      | End of initialization                                |
| `FUNC_ACTIVE`                    | Driver presence query                                |
| `FUNC_ABOUT_TO_RESTART`          | Just before a restart is triggered                   |
| `FUNC_LOOP` / `FUNC_SLEEP_LOOP`  | Main loop tick                                       |
| `FUNC_EVERY_50_MSECOND`          | Every 50 ms                                          |
| `FUNC_EVERY_100_MSECOND`         | Every 100 ms                                         |
| `FUNC_EVERY_200_MSECOND`         | Every 200 ms (energy slots only)                     |
| `FUNC_EVERY_250_MSECOND`         | Every 250 ms                                         |
| `FUNC_EVERY_SECOND`              | Every second                                         |
| `FUNC_RESET_SETTINGS`            | Settings reset                                       |
| `FUNC_RESTORE_SETTINGS`          | Settings restore                                     |
| `FUNC_SAVE_SETTINGS`             | Settings save                                        |
| `FUNC_SAVE_AT_MIDNIGHT`          | Daily 00:00 housekeeping                             |
| `FUNC_SAVE_BEFORE_RESTART`       | Persist data before a planned restart                |
| `FUNC_INTERRUPT_STOP` / `START`  | Disable / enable interrupts around critical sections |
| `FUNC_AFTER_TELEPERIOD`          | After teleperiod publish                             |
| `FUNC_JSON_APPEND`               | Append JSON to teleperiod payload                    |
| `FUNC_WEB_SENSOR`                | Append HTML rows to the main web page sensor block   |
| `FUNC_WEB_COL_SENSOR`            | Append column-formatted HTML for sensors             |
| `FUNC_MQTT_SUBSCRIBE`            | Issue extra MQTT subscriptions on connect            |
| `FUNC_MQTT_INIT`                 | Once at end of MQTT (re)connect                      |
| `FUNC_SET_POWER`                 | Notification before relays change                    |
| `FUNC_SHOW_SENSOR`               | After `FUNC_JSON_APPEND` completes                   |
| `FUNC_ANY_KEY`                   | Any button/switch interaction                        |
| `FUNC_LED_LINK`                  | Link/status LED control                              |
| `FUNC_ENERGY_EVERY_SECOND`       | Energy driver tick                                   |
| `FUNC_ENERGY_RESET`              | Energy counter reset                                 |
| `FUNC_TELEPERIOD_RULES_PROCESS`  | Rule processing tied to teleperiod                   |
| `FUNC_FREE_MEM`                  | Diagnostic memory dump request                       |
| `FUNC_WEB_ADD_BUTTON`            | Add a button to the configuration page               |
| `FUNC_WEB_ADD_CONSOLE_BUTTON`    | Add a button to the console page                     |
| `FUNC_WEB_ADD_MANAGEMENT_BUTTON` | Add a button to the management page                  |
| `FUNC_WEB_ADD_MAIN_BUTTON`       | Add a button to the main page                        |
| `FUNC_WEB_GET_ARG`               | Custom HTTP argument handling                        |
| `FUNC_WEB_ADD_HANDLER`           | Register additional URL handlers                     |
| `FUNC_SET_SCHEME`                | Light scheme change (light slots)                    |
| `FUNC_HOTPLUG_SCAN`              | Periodic hotplug device scan                         |
| `FUNC_TIME_SYNCED`               | NTP sync completed                                   |
| `FUNC_DEVICE_GROUP_ITEM`         | Device-groups item processing                        |
| `FUNC_NETWORK_UP` / `DOWN`       | Network connectivity changes                         |
| `FUNC_WEB_STATUS_LEFT` / `RIGHT` | Status page columns                                  |

#### Callbacks with return value (`bool`, ID ≥ 200)

| Function                    | Purpose                                            |
|-----------------------------|----------------------------------------------------|
| `FUNC_PIN_STATE`            | Override GPIO state at configuration time          |
| `FUNC_MODULE_INIT`          | Module-specific initialization                     |
| `FUNC_ADD_BUTTON`           | Register custom button handler                     |
| `FUNC_ADD_SWITCH`           | Register custom switch handler                     |
| `FUNC_BUTTON_PRESSED`       | Handle a single button press                       |
| `FUNC_BUTTON_MULTI_PRESSED` | Handle multi-press button gestures                 |
| `FUNC_SET_DEVICE_POWER`     | Drive a device-specific relay                      |
| `FUNC_MQTT_DATA`            | Inspect/handle raw MQTT payloads                   |
| `FUNC_SERIAL`               | Process bytes received on the main serial port     |
| `FUNC_COMMAND`              | Generic command dispatch (driver-defined commands) |
| `FUNC_COMMAND_SENSOR`       | Handle `Sensor<id>` commands                       |
| `FUNC_COMMAND_DRIVER`       | Handle `Driver<id>` commands                       |
| `FUNC_RULES_PROCESS`        | Custom rule expansion                              |
| `FUNC_SET_CHANNELS`         | Light channel update                               |

For display drivers a separate set of `FUNC_DISPLAY_*` callbacks exists; see `docs/API.md`.

### Command Context — `XdrvMailbox`

Custom command handlers (`FUNC_COMMAND`, `FUNC_COMMAND_SENSOR`, `FUNC_COMMAND_DRIVER`) read their inputs from the global `XdrvMailbox` structure, defined in `tasmota/tasmota.ino`:

```c
struct XDRVMAILBOX {
  bool          grpflg;        // command was received on the group topic
  bool          usridx;        // command name had a numeric suffix supplied by user
  uint16_t      command_code;  // index into the command table
  uint32_t      index;         // numeric suffix of the command (e.g. 2 for Power2), default 1
  uint32_t      data_len;      // length of `data`
  int32_t       payload;       // numeric value of the first parameter, or -99 if not numeric
  char         *topic;         // MQTT topic the command came from (or nullptr)
  char         *data;          // raw parameter string (writable)
  char         *command;       // command name as parsed
} XdrvMailbox;
```

**Note**: The `Sensor-API.md` document shows an older version of this structure. Refer to `tasmota.ino` for the current definition.

Helper functions to build the JSON response are declared in `tasmota/tasmota_support/support_command.ino`:

- `void ResponseCmnd(void)` — start a `{"<Command>":...` response
- `void ResponseCmndDone(void)` — `{"<Command>":"Done"}`
- `void ResponseCmndNumber(int value)` — `{"<Command>":<value>}`
- `void ResponseCmndIdxNumber(int value)` — `{"<Command><index>":<value>}`
- `void ResponseCmndFloat(float value, uint32_t decimals)` and `ResponseCmndIdxFloat(...)`
- `int Response_P(const char* format, ...)` — replace the response buffer (in `tasmota/tasmota_support/support.ino`)
- `int ResponseAppend_P(const char* format, ...)` — append to the response buffer

To publish the buffer as a sensor telemetry payload:

```c
void MqttPublishTeleSensor(void);   // publishes tele/<topic>/SENSOR
```

### Settings Storage

Persistent settings are kept in flash. The structure is `TSettings`, defined in `tasmota/include/tasmota_types.h`. The global instance is accessed via the pointer `Settings` declared in `tasmota.ino`:

```c
extern TSettings* Settings;     // see tasmota.ino
```

A few important points:
- Most string fields (Wi-Fi SSIDs and passwords, MQTT host/user/password/topic, hostname, friendly names, NTP servers, etc.) are **not** stored as direct members; they live in a single `text_pool[]` and are accessed by index through `SettingsText()` / `SettingsUpdateText()` (see `tasmota/tasmota_support/settings.ino`).
- The set of valid indexes is `enum SettingsTextIndex` in `tasmota/include/tasmota.h` (e.g. `SET_OTAURL`, `SET_MQTT_HOST`, `SET_HOSTNAME`, `SET_DEVICENAME`, ...).
- `SetOption32`..`SetOption49` are stored in `Settings->param[PARAM8_SIZE]`.
- Many flag bits live in `Settings->flag`, `Settings->flag2` (type `SysMBitfield1`), etc. (bitfield typedefs in `tasmota_types.h`).

Read/write access pattern:

```c
const char* host = SettingsText(SET_MQTT_HOST);
SettingsUpdateText(SET_MQTT_HOST, "broker.example.com");

uint16_t period = Settings->tele_period;
Settings->tele_period = 300;
```

Do not invent field names from older versions of the firmware — always check `tasmota_types.h` and the `SettingsTextIndex` enum.

## Logging

Log levels are defined as an `enum LoggingLevels` in `tasmota/include/tasmota.h`:

```c
enum LoggingLevels { LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO,
                     LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE };
```

The main logging function (in `tasmota/tasmota_support/support.ino`) is:

```c
void AddLog(uint32_t loglevel, PGM_P formatP, ...);
```

The format string is expected to come from PROGMEM, typically wrapped with `PSTR(...)`:

```c
AddLog(LOG_LEVEL_INFO, PSTR("MyDriver: value=%d"), value);
```

There is no `AddLog_P` / `AddLog_P2` macro — call `AddLog` directly.

### Conditional debug macros

Defined in `tasmota/include/tasmota_globals.h` and gated by build-time `#define`s:

```c
#ifdef DEBUG_TASMOTA_CORE
#define DEBUG_CORE_LOG(...)   AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_CORE_LOG(...)
#endif

#ifdef DEBUG_TASMOTA_DRIVER
#define DEBUG_DRIVER_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_DRIVER_LOG(...)
#endif

#ifdef DEBUG_TASMOTA_SENSOR
#define DEBUG_SENSOR_LOG(...) AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_SENSOR_LOG(...)
#endif

#ifdef DEBUG_TASMOTA_TRACE
#define DEBUG_TRACE_LOG(...)  AddLog(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#define DEBUG_TRACE_LOG(...)
#endif

#ifdef USE_DEBUG_DRIVER
#define SHOW_FREE_MEM(WHERE) ShowFreeMem(WHERE);
#else
#define SHOW_FREE_MEM(WHERE)
#endif
```

Enable any of these in `user_config_override.h` by defining the corresponding `DEBUG_TASMOTA_*` symbol. There is no `CHECK_OOM` macro.

### Heap diagnostics

Free heap can be obtained from the standard ESP wrappers:

```c
uint32_t free = ESP.getFreeHeap();
AddLog(LOG_LEVEL_DEBUG, PSTR("Free heap: %u"), free);
```

Tasmota also provides its own helpers in `tasmota/tasmota_support/support_esp.ino` (e.g. `ESP_getFreeHeap()`, `ESP_getMaxAllocHeap()`); check that file for the exact set on your branch.

## I2C Helper API

Defined in `tasmota/tasmota_support/support_a_i2c.ino`. All read/write helpers take an optional `bus` argument (default `0`); on devices with two I2C buses pass `1` for the secondary bus.

### Bus management

```c
bool I2cBegin(int sda, int scl, uint32_t bus = 0, uint32_t frequency = 100000);
bool I2cSetClock(uint32_t frequency = 0, uint32_t bus = 0);
bool I2cReset(uint32_t bus = 0);
void I2cScan(uint8_t bus = 0);   // prints found addresses to the response buffer
```

### Reads (return `true` on success)

```c
bool I2cValidRead   (uint8_t addr, uint8_t reg, uint8_t size,
                     uint8_t bus = 0, bool sendStop = false);
bool I2cValidRead8  (uint8_t  *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cValidRead16 (uint16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cValidReadS16(int16_t  *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cValidRead16LE  (uint16_t *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cValidReadS16_LE(int16_t  *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cValidRead24 (int32_t  *data, uint8_t addr, uint8_t reg, uint8_t bus = 0);
```

### Reads (return value directly, no error indication)

```c
uint8_t  I2cRead8     (uint8_t addr, uint8_t reg, uint8_t bus = 0);
uint16_t I2cRead16    (uint8_t addr, uint8_t reg, uint8_t bus = 0);
int16_t  I2cReadS16   (uint8_t addr, uint8_t reg, uint8_t bus = 0);
uint16_t I2cRead16LE  (uint8_t addr, uint8_t reg, uint8_t bus = 0);
int16_t  I2cReadS16_LE(uint8_t addr, uint8_t reg, uint8_t bus = 0);
int32_t  I2cRead24    (uint8_t addr, uint8_t reg, uint8_t bus = 0);
```

There are **no** `I2cReadS32` / `I2cReadS32_LE` / `I2cValidReadS32` / `I2cValidReadS32_LE` helpers — handle 32-bit reads with `I2cReadBuffer` if needed.

### Writes (return `true` on success)

```c
bool I2cWrite0 (uint8_t addr, uint8_t reg, uint8_t bus = 0);
bool I2cWrite8 (uint8_t addr, uint8_t reg, uint32_t val, uint8_t bus = 0);
bool I2cWrite16(uint8_t addr, uint8_t reg, uint32_t val, uint8_t bus = 0);
bool I2cWrite  (uint8_t addr, uint8_t reg, uint32_t val, uint8_t size, uint8_t bus = 0);
```

There is no `I2cWrite16LE` helper.

### Buffer transfers (return `false` on success, `true` on error — note inverted convention)

```c
bool I2cReadBuffer (uint8_t addr, int reg, uint8_t *reg_data, uint16_t len, uint8_t bus = 0);
bool I2cReadBuffer0(uint8_t addr, uint8_t *reg_data, uint16_t len, uint8_t bus = 0);
bool I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len, uint8_t bus = 0);
```

### Driver registry / detection

```c
bool I2cActive         (uint32_t addr, uint8_t bus = 0);
void I2cSetActive      (uint32_t addr, uint8_t bus = 0);
void I2cResetActive    (uint32_t addr, uint8_t bus = 0);
void I2cSetActiveFound (uint32_t addr, const char *types, uint8_t bus = 0);
bool I2cSetDevice      (uint32_t addr, uint8_t bus = 0);
bool I2cEnabled        (uint32_t i2c_index);
```

`I2cEnabled(XI2C_<n>)` is the preferred gate at the top of an I2C sensor's callback function, replacing the legacy `i2c_flg` global.

### Detection pattern

```c
void MySensorDetect(void) {
  for (uint32_t bus = 0; bus < MAX_I2C; bus++) {
    for (uint32_t i = 0; i < MY_SENSOR_ADDR_NUM; i++) {
      uint8_t addr = MY_SENSOR_BASE_ADDR + i;
      if (!I2cSetDevice(addr, bus)) { continue; }   // already claimed by another driver

      uint8_t id;
      if (I2cValidRead8(&id, addr, MY_SENSOR_ID_REG, bus) && id == MY_SENSOR_ID_VAL) {
        I2cSetActiveFound(addr, "MySensor", bus);
        // ... store addr/bus, mark detected ...
        return;
      }
    }
  }
}
```
