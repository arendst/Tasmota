# Tasmota Developer Guide

This file is a summary of the Tasmota Documentation for the "docs" repository. It is provided here for convenience for GenAI to read it easily.

## How Tasmota Works

### Core Architecture

Tasmota is a modular firmware that transforms ESP8266/ESP8285 and ESP32 microcontrollers into intelligent IoT devices. The architecture follows these key principles:

#### 1. Event-Driven System
- Main loop processes events and callbacks
- Non-blocking operations to maintain responsiveness
- Callback system for sensors, drivers, and features
- Timer-based scheduling for periodic tasks

#### 2. Modular Design
- Core functionality always present (WiFi, MQTT, web interface)
- Optional features compiled conditionally using `#define` directives
- Plugin architecture for sensors and peripherals
- Template system for device configuration

#### 3. Communication Hub
- **MQTT**: Primary communication protocol for automation systems
- **HTTP**: Web interface and REST API
- **Serial**: Direct console access for debugging and configuration
- **WebSocket**: Real-time web interface updates

### Firmware Structure

```
tasmota/
├── tasmota.ino                    # Main firmware file
├── tasmota_xdrv_driver/           # Driver files directory (187 files)
│   ├── xdrv_01_9_webserver.ino   # Web server driver
│   ├── xdrv_02_9_mqtt.ino        # MQTT driver
│   ├── xdrv_04_light.ino         # Light driver
│   └── xdrv_##_name.ino          # Other drivers
├── tasmota_xsns_sensor/           # Sensor files directory (143 files)
│   ├── xsns_01_counter.ino       # Counter sensor
│   ├── xsns_02_analog.ino        # Analog sensor
│   └── xsns_##_name.ino          # Other sensors
├── tasmota_xlgt_light/            # Light driver files directory
├── tasmota_xnrg_energy/           # Energy monitoring files directory
├── tasmota_support/               # Support functions directory (29 files)
│   ├── support.ino                # Core support functions
│   ├── settings.ino               # Settings management
│   └── support_*.ino              # Other support modules
├── include/                       # Header files directory (18 files)
│   ├── tasmota.h                  # Main header
│   ├── tasmota_types.h            # Type definitions
│   ├── tasmota_globals.h          # Global variables
│   └── *.h                        # Other headers
└── my_user_config.h              # User configuration overrides
```

### Command System

All Tasmota functionality is accessible through a unified command system:

- Commands can be sent via MQTT, HTTP, serial, or web console
- Format: `Command [parameter]`
- Response format: JSON for structured data
- Backlog support for multiple commands: `Backlog cmd1; cmd2; cmd3`

### GPIO Management

Tasmota uses a flexible GPIO assignment system:

1. **Templates**: Pre-defined GPIO configurations for specific devices
2. **Components**: Logical functions assigned to physical pins
3. **Modules**: Base hardware configurations
4. **Runtime Configuration**: GPIO can be reassigned without recompilation

## Development Environment Setup

### Prerequisites

1. **PlatformIO**: Primary build system
2. **Git**: Version control
3. **Python**: For build scripts and tools
4. **Serial Programmer**: For initial flashing

### Build Configuration

Create `platformio_tasmota_cenv.ini` for custom environments:

```ini
[env:tasmota32-custom]
extends = env:tasmota32
build_flags = ${env:tasmota32.build_flags}
              -DUSE_MY_CUSTOM_FEATURE
```

### User Configuration

Create `tasmota/user_config_override.h`:

```c
#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_

// Enable custom features
#define USE_CUSTOM_SENSOR
#define USE_BERRY_DEBUG

// Disable unused features to save space
#undef USE_DOMOTICZ
#undef USE_KNX

#endif
```

## Driver Development

### Sensor Driver Structure

All sensor drivers follow a standardized pattern:

```c
#ifdef USE_MY_SENSOR
#define XSNS_XX  XX  // Unique sensor ID

bool MySensorDetected = false;

void MySensorInit(void) {
  // Initialize sensor
  if (sensor_detected) {
    MySensorDetected = true;
  }
}

void MySensorEverySecond(void) {
  // Read sensor data
}

void MySensorShow(bool json) {
  if (json) {
    ResponseAppend_P(PSTR(",\"MySensor\":{\"Temperature\":%d}"), temperature);
  }
#ifdef USE_WEBSERVER
  else {
    WSContentSend_PD(HTTP_SNS_TEMP, "MySensor", temperature);
  }
#endif
}

bool Xsns_XX(byte function) {
  bool result = false;
  
  if (i2c_flg) {  // Only for I2C sensors
    switch (function) {
      case FUNC_INIT:
        MySensorInit();
        break;
      case FUNC_EVERY_SECOND:
        MySensorEverySecond();
        break;
      case FUNC_JSON_APPEND:
        MySensorShow(1);
        break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
        MySensorShow(0);
        break;
#endif
    }
  }
  return result;
}
#endif  // USE_MY_SENSOR
```

### Complete Driver Callback Functions Reference

**VERIFIED FROM SOURCE CODE**: `tasmota/include/tasmota.h` lines 433-454

#### Core System Callbacks (Functions WITHOUT return results)

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_SETTINGS_OVERRIDE` | Override default settings | Before settings load | None |
| `FUNC_SETUP_RING1` | Early setup phase 1 | System initialization | None |
| `FUNC_SETUP_RING2` | Early setup phase 2 | System initialization | None |
| `FUNC_PRE_INIT` | Pre-initialization | Before main init | None |
| `FUNC_INIT` | Initialize driver/sensor | Once at startup | None |
| `FUNC_ACTIVE` | Check if driver is active | Status queries | None |
| `FUNC_ABOUT_TO_RESTART` | Prepare for restart | Before system restart | None |

#### Loop and Timing Callbacks

| Function | Purpose | Frequency | Parameters |
|----------|---------|-----------|-----------|
| `FUNC_LOOP` | Main loop processing | Every loop cycle (~1ms) | None |
| `FUNC_SLEEP_LOOP` | Sleep mode processing | During sleep cycles | None |
| `FUNC_EVERY_50_MSECOND` | Fast polling operations | Every 50ms | None |
| `FUNC_EVERY_100_MSECOND` | Medium polling | Every 100ms | None |
| `FUNC_EVERY_200_MSECOND` | Slower polling | Every 200ms | None |
| `FUNC_EVERY_250_MSECOND` | Quarter second tasks | Every 250ms | None |
| `FUNC_EVERY_SECOND` | Regular updates | Every second | None |

#### Settings and Configuration Callbacks

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_RESET_SETTINGS` | Reset to defaults | Settings reset | None |
| `FUNC_RESTORE_SETTINGS` | Restore from backup | Settings restore | None |
| `FUNC_SAVE_SETTINGS` | Save current settings | Settings save | None |
| `FUNC_SAVE_AT_MIDNIGHT` | Midnight save operations | Daily at 00:00 | None |
| `FUNC_SAVE_BEFORE_RESTART` | Save critical data | Before restart | None |

#### Interrupt and System Control

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_INTERRUPT_STOP` | Stop interrupts | Before critical section | None |
| `FUNC_INTERRUPT_START` | Resume interrupts | After critical section | None |
| `FUNC_FREE_MEM` | Memory cleanup | Low memory conditions | None |

#### Telemetry and JSON Callbacks

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_AFTER_TELEPERIOD` | Post-telemetry cleanup | After TelePeriod | None |
| `FUNC_JSON_APPEND` | Add JSON telemetry | Every TelePeriod | None |
| `FUNC_TELEPERIOD_RULES_PROCESS` | Rules after telemetry | Post-TelePeriod | None |

#### Web Interface Callbacks

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_WEB_SENSOR` | Show sensor on web | Sensor page load | None |
| `FUNC_WEB_COL_SENSOR` | Column sensor display | Web page layout | None |
| `FUNC_WEB_ADD_BUTTON` | Add web buttons | Main page load | None |
| `FUNC_WEB_ADD_CONSOLE_BUTTON` | Add console button | Console page | None |
| `FUNC_WEB_ADD_MANAGEMENT_BUTTON` | Add config button | Config page | None |
| `FUNC_WEB_ADD_MAIN_BUTTON` | Add main menu button | Main page | None |
| `FUNC_WEB_GET_ARG` | Process web arguments | Form submission | None |
| `FUNC_WEB_ADD_HANDLER` | Add URL handlers | Web server init | None |
| `FUNC_WEB_STATUS_LEFT` | Left status column | Status page | None |
| `FUNC_WEB_STATUS_RIGHT` | Right status column | Status page | None |

#### MQTT and Communication Callbacks

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_MQTT_SUBSCRIBE` | Subscribe to MQTT topics | MQTT connect | None |
| `FUNC_MQTT_INIT` | Initialize MQTT | MQTT setup | None |

#### Power and Hardware Control

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_SET_POWER` | Handle power changes | Power state change | None |
| `FUNC_SHOW_SENSOR` | Display sensor data | Status request | None |
| `FUNC_ANY_KEY` | Handle any key press | Key event | None |
| `FUNC_LED_LINK` | Control link LED | Network state change | None |
| `FUNC_ENERGY_EVERY_SECOND` | Energy monitoring | Every second | None |
| `FUNC_ENERGY_RESET` | Reset energy counters | Reset command | None |

#### Advanced System Callbacks

| Function | Purpose | When Called | Parameters |
|----------|---------|-------------|-----------|
| `FUNC_SET_SCHEME` | Set color scheme | Theme change | None |
| `FUNC_HOTPLUG_SCAN` | Scan for hotplug devices | Device detection | None |
| `FUNC_TIME_SYNCED` | Time synchronization | NTP sync complete | None |
| `FUNC_DEVICE_GROUP_ITEM` | Device group processing | Group operations | None |
| `FUNC_NETWORK_UP` | Network connected | WiFi/Ethernet up | None |
| `FUNC_NETWORK_DOWN` | Network disconnected | WiFi/Ethernet down | None |

#### Callback Functions WITH Return Results (ID >= 200)

These functions are expected to return boolean results:

| Function | Purpose | When Called | Return Value |
|----------|---------|-------------|--------------|
| `FUNC_PIN_STATE` | GPIO state query | Pin state check | true if handled |
| `FUNC_MODULE_INIT` | Module initialization | Module setup | true if success |
| `FUNC_ADD_BUTTON` | Add button handler | Button config | true if added |
| `FUNC_ADD_SWITCH` | Add switch handler | Switch config | true if added |
| `FUNC_BUTTON_PRESSED` | Handle button press | Button event | true if handled |
| `FUNC_BUTTON_MULTI_PRESSED` | Multi-button press | Button combo | true if handled |
| `FUNC_SET_DEVICE_POWER` | Device power control | Power command | true if handled |
| `FUNC_MQTT_DATA` | Process MQTT data | MQTT message | true if handled |
| `FUNC_SERIAL` | Serial data processing | Serial input | true if handled |
| `FUNC_COMMAND` | Process commands | Command received | true if handled |
| `FUNC_COMMAND_SENSOR` | Sensor commands | Sensor command | true if handled |
| `FUNC_COMMAND_DRIVER` | Driver commands | Driver command | true if handled |
| `FUNC_RULES_PROCESS` | Process rules | Rule evaluation | true if handled |
| `FUNC_SET_CHANNELS` | Set PWM channels | Channel update | true if handled |

#### Callback Implementation Pattern

```c
bool Xdrv_XX(uint8_t function) {
  bool result = false;
  
  switch (function) {
    case FUNC_INIT:
      MyDriverInit();
      break;
    case FUNC_EVERY_SECOND:
      MyDriverEverySecond();
      break;
    case FUNC_COMMAND:
      result = MyDriverCommand();
      break;
    case FUNC_JSON_APPEND:
      MyDriverJsonAppend();
      break;
    case FUNC_WEB_SENSOR:
      MyDriverWebSensor();
      break;
    case FUNC_SAVE_BEFORE_RESTART:
      MyDriverSaveSettings();
      break;
  }
  return result;
}

### I2C Development

```c
// I2C Helper Functions
bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg);
bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg);
uint8_t I2cRead8(uint8_t addr, uint8_t reg);
uint16_t I2cRead16(uint8_t addr, uint8_t reg);
bool I2cWrite8(uint8_t addr, uint8_t reg, uint8_t val);

// Device Detection Pattern
void MySensorDetect(void) {
  if (MySensorDetected) return;
  
  for (uint8_t i = 0; i < SENSOR_MAX_ADDR; i++) {
    uint8_t addr = SENSOR_BASE_ADDR + i;
    if (I2cValidRead8(&sensor_id, addr, SENSOR_ID_REG)) {
      if (sensor_id == EXPECTED_ID) {
        MySensorDetected = true;
        AddLog(LOG_LEVEL_INFO, PSTR("MySensor found at 0x%02X"), addr);
        break;
      }
    }
  }
}
```

## Scripting and Automation

### Rules System

Rules provide event-driven automation:

```
Rule1 ON Switch1#State DO Power1 %value% ENDON
      ON Time#Minute=30 DO Publish stat/topic/alert {"time":"30min"} ENDON
```

### Berry Scripting (ESP32)

Berry is a modern scripting language for advanced automation:

```berry
# Simple sensor reading
import json

def read_sensor()
  var temp = tasmota.read_sensors()
  if temp.contains("Temperature")
    print("Current temperature:", temp["Temperature"])
  end
end

# Set up timer
tasmota.set_timer(5000, read_sensor)

# Web interface extension
def web_add_button()
  webserver.content_send("<button onclick='la(\"&m_toggle=1\");'>Toggle</button>")
end

tasmota.add_driver(web_add_button)
```

### Command Extensions

Add custom commands through Berry or C++:

```berry
def my_command(cmd, idx, payload)
  if cmd == "MYCMD"
    print("Custom command received:", payload)
    tasmota.resp_cmnd_done()
  end
end

tasmota.add_cmd('MYCMD', my_command)
```

## Complete Settings Structure Reference

### Settings Memory Layout

Tasmota uses a structured settings system stored in flash memory. The main settings structure is defined in `settings.h`:

```c
typedef struct {
  unsigned long cfg_holder;                // 000 v6.0.0a
  unsigned long save_flag;                 // 004
  unsigned long version;                   // 008
  unsigned short flag;                     // 00C
  unsigned short save_data;                // 00E
  int8_t         timezone;                 // 010
  char           ota_url[101];             // 011
  char           mqtt_prefix[3][11];       // 076
  char           serial_delimiter;         // 09D
  uint8_t        seriallog_level;          // 09E
  uint8_t        sta_config;               // 09F
  char           sta_ssid[2][33];          // 0A0
  char           sta_pwd[2][65];           // 102
  char           hostname[33];             // 183
  char           syslog_host[33];          // 1A4
  uint16_t       syslog_port;              // 1C5
  uint8_t        syslog_level;             // 1C7
  uint8_t        webserver;                // 1C8
  uint8_t        weblog_level;             // 1C9
  char           mqtt_fingerprint[2][60];  // 1CA
  char           mqtt_host[33];            // 236
  uint16_t       mqtt_port;                // 257
  char           mqtt_client[33];          // 259
  char           mqtt_user[33];            // 27A
  char           mqtt_pwd[33];             // 29B
  char           mqtt_topic[33];           // 2BC
  char           button_topic[33];         // 2DD
  char           mqtt_grptopic[33];        // 2FE
  uint8_t        display_model;            // 31F
  uint8_t        display_mode;             // 320
  uint8_t        display_refresh;          // 321
  uint8_t        display_rows;             // 322
  uint8_t        display_cols[2];          // 323
  uint8_t        display_address[8];       // 325
  uint8_t        display_dimmer;           // 32D
  uint8_t        display_size;             // 32E
  uint16_t       pwm_frequency;            // 32F
  power_t        power;                    // 331
  uint16_t       pwm_value[MAX_PWMS];      // 335
  int16_t        altitude;                 // 345
  uint16_t       tele_period;              // 347
  uint8_t        ledstate;                 // 349
  uint8_t        param[PARAM_MAX];         // 34A
  int16_t        toffset[2];               // 35A
  uint8_t        display_font;             // 35E
} Settings;

### ESP8266 Constraints

- **Flash**: 1MB total, ~500KB available for firmware
- **RAM**: 80KB total, ~25-30KB available for application
- **Stack**: 4KB maximum

### Optimization Techniques

1. **Use PROGMEM for constants**:
```c
const char MyString[] PROGMEM = "Constant string";
```

2. **Minimize dynamic allocation**:
```c
// Avoid
String result = String(value1) + "," + String(value2);

// Prefer
char result[32];
snprintf(result, sizeof(result), "%d,%d", value1, value2);
```

3. **Use flash-efficient data types**:
```c
// Use uint32_t instead of uint8_t for local variables
// Use uint8_t only in structs to save memory
```

## Communication Protocols

### Command Context Structure

All command handlers receive context through the global XdrvMailbox structure:

```c
struct XDRVMAILBOX {
  bool          grpflg;        // Group flag
  bool          usridx;        // User index flag
  uint16_t      command_code;  // Command code
  uint32_t      index;         // Command index
  uint32_t      data_len;      // Parameter length
  int32_t       payload;       // Numeric parameter
  char         *topic;         // MQTT topic
  char         *data;          // Command parameters
  char         *command;       // Command name
} XdrvMailbox;
```

**Key Fields:**
- `command`: The command name (e.g., "Power", "Status")
- `data`: Raw parameter string
- `payload`: Numeric value of first parameter
- `data_len`: Length of parameter string
- `index`: Command index for numbered commands (Power1, Power2, etc.)

### MQTT Integration

```c
// Publish sensor data
void PublishSensorData(void) {
  Response_P(PSTR("{\"MySensor\":{\"Value\":%d}}"), sensor_value);
  MqttPublishTeleSensor();
}

// Subscribe to commands
bool MyCommand(void) {
  if (XdrvMailbox.data_len > 0) {
    // Process command
    ResponseCmndDone();
    return true;
  }
  ResponseCmndNumber(current_value);
  return true;
}
```

### Web Interface Extensions

```c
#ifdef USE_WEBSERVER
void MySensorWebShow(void) {
  WSContentSend_PD(PSTR(
    "{s}MySensor Temperature{m}%d°C{e}"
    "{s}MySensor Humidity{m}%d%%{e}"),
    temperature, humidity);
}
#endif
```

## Advanced Features

### Template System

Templates define device GPIO configurations:

```json
{
  "NAME":"Custom Device",
  "GPIO":[416,0,418,0,417,2720,0,0,2624,32,2656,224,0,0],
  "FLAG":0,
  "BASE":45
}
```

### Matter Protocol Support

For ESP32 devices, Matter provides standardized IoT communication:

```c
// Matter endpoint configuration
matter.add_endpoint(1, 0x0100);  // On/Off Light
matter.add_endpoint(2, 0x0106);  // Light with dimming
```

### Display Integration

Universal Display Driver supports 50+ display types:

```
DisplayModel 1    # Select display type
DisplayMode 1     # Text mode
DisplayText [s1l1]Hello World
```

## Testing and Debugging

### Debug Options

Enable debugging in `user_config_override.h`:

```c
#define DEBUG_TASMOTA_CORE
#define DEBUG_TASMOTA_DRIVER
#define USE_DEBUG_DRIVER
```

### Serial Debugging

```c
AddLog(LOG_LEVEL_INFO, PSTR("Debug: value=%d"), value);
AddLog(LOG_LEVEL_DEBUG, PSTR("Detailed info: %s"), info_string);
```

### Memory Monitoring

```c
// Check free heap
uint32_t free_heap = ESP.getFreeHeap();
AddLog(LOG_LEVEL_DEBUG, PSTR("Free heap: %d"), free_heap);
```

## Best Practices

### Code Organization

1. **Use consistent naming**: `MySensor` prefix for all functions
2. **Follow callback patterns**: Implement standard driver callbacks
3. **Handle errors gracefully**: Check return values and sensor presence
4. **Document thoroughly**: Include usage examples and pin assignments

### Performance Considerations

1. **Minimize blocking operations**: Use state machines for long operations
2. **Cache sensor readings**: Don't read sensors more often than necessary
3. **Use appropriate data types**: Consider memory usage vs. precision
4. **Optimize for common cases**: Fast path for normal operations

### Security Guidelines

1. **Validate all inputs**: Check command parameters and sensor data
2. **Use secure defaults**: Enable security features by default
3. **Minimize attack surface**: Disable unused network services
4. **Regular updates**: Keep firmware and dependencies current

## Integration Examples

### Home Assistant Discovery

```c
void PublishDiscovery(void) {
  Response_P(PSTR("{"
    "\"name\":\"%s MySensor\","
    "\"stat_t\":\"%s\","
    "\"unit_of_meas\":\"°C\","
    "\"dev_cla\":\"temperature\""
    "}"), SettingsText(SET_DEVICENAME), GetStateTopic());
  
  MqttPublish(GetDiscoveryTopic("sensor", "temperature"), true);
}
```

### Custom Web Interface

```c
const char HTTP_MYSENSOR[] PROGMEM =
  "{s}MySensor{m}"
  "<input type='range' min='0' max='100' value='%d' "
  "onchange='la(\"&mysensor_val=\"+this.value);'>"
  "{e}";

void MySensorWebShow(void) {
  WSContentSend_PD(HTTP_MYSENSOR, current_value);
}
```

This guide provides the foundation for understanding and extending Tasmota. The modular architecture, standardized APIs, and extensive documentation make it an excellent platform for IoT development, whether you're adding simple sensor support or implementing complex automation systems.

## Complete Command Reference

### Core System Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Status` | 0-11 | System status information | `Status 0` |
| `Reset` | 1-6 | Reset device with options | `Reset 1` |
| `Restart` | 1 | Restart device | `Restart 1` |
| `Upgrade` | 1 | Start OTA upgrade | `Upgrade 1` |
| `Upload` | 1 | Start file upload | `Upload 1` |
| `Otaurl` | url | Set OTA URL | `Otaurl http://ota.server/firmware.bin` |
| `Seriallog` | 0-4 | Set serial log level | `Seriallog 2` |
| `Syslog` | 0-4 | Set syslog level | `Syslog 2` |
| `Loghost` | hostname | Set syslog host | `Loghost 192.168.1.100` |
| `Logport` | port | Set syslog port | `Logport 514` |
| `Ipaddress` | x.x.x.x | Set IP address | `Ipaddress 192.168.1.100` |
| `Gateway` | x.x.x.x | Set gateway | `Gateway 192.168.1.1` |
| `Subnetmask` | x.x.x.x | Set subnet mask | `Subnetmask 255.255.255.0` |
| `Dnsserver` | x.x.x.x | Set DNS server | `Dnsserver 8.8.8.8` |
| `Mac` | - | Show MAC address | `Mac` |
| `Hostname` | name | Set hostname | `Hostname tasmota-device` |

### WiFi Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Ssid1` | ssid | Set WiFi SSID 1 | `Ssid1 MyNetwork` |
| `Ssid2` | ssid | Set WiFi SSID 2 | `Ssid2 BackupNetwork` |
| `Password1` | password | Set WiFi password 1 | `Password1 MyPassword` |
| `Password2` | password | Set WiFi password 2 | `Password2 BackupPassword` |
| `Ap` | 0-2 | Set AP mode | `Ap 1` |
| `WebServer` | 0-2 | Enable web server | `WebServer 1` |
| `WebPassword` | password | Set web password | `WebPassword admin` |
| `WifiConfig` | 0-7 | WiFi configuration mode | `WifiConfig 4` |

### MQTT Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `MqttHost` | hostname | Set MQTT broker | `MqttHost 192.168.1.100` |
| `MqttPort` | port | Set MQTT port | `MqttPort 1883` |
| `MqttUser` | username | Set MQTT username | `MqttUser myuser` |
| `MqttPassword` | password | Set MQTT password | `MqttPassword mypass` |
| `MqttClient` | clientid | Set MQTT client ID | `MqttClient tasmota-device` |
| `Topic` | topic | Set MQTT topic | `Topic tasmota` |
| `GroupTopic` | topic | Set group topic | `GroupTopic tasmotas` |
| `FullTopic` | template | Set full topic template | `FullTopic %prefix%/%topic%/` |
| `Prefix1` | prefix | Set command prefix | `Prefix1 cmnd` |
| `Prefix2` | prefix | Set status prefix | `Prefix2 stat` |
| `Prefix3` | prefix | Set telemetry prefix | `Prefix3 tele` |
| `Publish` | topic payload | Publish MQTT message | `Publish stat/topic/test Hello` |
| `MqttRetry` | seconds | Set MQTT retry time | `MqttRetry 10` |
| `StateText1` | text | Set OFF state text | `StateText1 OFF` |
| `StateText2` | text | Set ON state text | `StateText2 ON` |
| `StateText3` | text | Set TOGGLE state text | `StateText3 TOGGLE` |
| `StateText4` | text | Set HOLD state text | `StateText4 HOLD` |

### Power and Relay Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Power` | 0/1/2 | Control main power | `Power 1` |
| `Power1` | 0/1/2 | Control power 1 | `Power1 ON` |
| `Power2` | 0/1/2 | Control power 2 | `Power2 OFF` |
| `Power3` | 0/1/2 | Control power 3 | `Power3 TOGGLE` |
| `Power4` | 0/1/2 | Control power 4 | `Power4 1` |
| `PowerOnState` | 0-4 | Set power on state | `PowerOnState 1` |
| `PulseTime` | 1-111 | Set pulse time | `PulseTime1 10` |
| `BlinkTime` | 2-3600 | Set blink time | `BlinkTime 10` |
| `BlinkCount` | 0-32000 | Set blink count | `BlinkCount 5` |
| `Interlock` | 0/1 | Enable interlock | `Interlock 1` |
| `Ledstate` | 0-8 | Set LED state | `Ledstate 1` |
| `LedPower` | 0-2 | Control LED power | `LedPower 1` |
| `LedMask` | hex | Set LED mask | `LedMask 0xFF00` |

### Sensor Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `TelePeriod` | 10-3600 | Set telemetry period | `TelePeriod 300` |
| `Resolution` | 0-3 | Set sensor resolution | `Resolution 2` |
| `HumRes` | 0-3 | Set humidity resolution | `HumRes 1` |
| `TempRes` | 0-3 | Set temperature resolution | `TempRes 2` |
| `PressRes` | 0-3 | Set pressure resolution | `PressRes 1` |
| `EnergyRes` | 0-5 | Set energy resolution | `EnergyRes 3` |
| `SpeedUnit` | 1-4 | Set speed unit | `SpeedUnit 1` |
| `WeightRes` | 0-3 | Set weight resolution | `WeightRes 2` |
| `FreqRes` | 0-3 | Set frequency resolution | `FreqRes 2` |

### Timer Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Timer1` | parameters | Configure timer 1 | `Timer1 {"Enable":1,"Time":"06:00","Days":"1111100","Repeat":1,"Action":1}` |
| `Timer2` | parameters | Configure timer 2 | `Timer2 {"Enable":1,"Time":"22:00","Action":0}` |
| `Timers` | 0/1 | Enable/disable timers | `Timers 1` |
| `Latitude` | degrees | Set latitude | `Latitude 52.520008` |
| `Longitude` | degrees | Set longitude | `Longitude 13.404954` |
| `Sunrise` | - | Show sunrise time | `Sunrise` |
| `Sunset` | - | Show sunset time | `Sunset` |

### GPIO and Template Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Gpio` | pin,function | Set GPIO function | `Gpio 14,21` |
| `Gpios` | - | Show GPIO configuration | `Gpios` |
| `Template` | json | Set device template | `Template {"NAME":"Generic","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":1,"BASE":18}` |
| `Module` | 0-255 | Set device module | `Module 1` |
| `Modules` | - | Show available modules | `Modules` |
| `I2CScan` | - | Scan I2C bus | `I2CScan` |
| `I2CDriver` | driver | Enable I2C driver | `I2CDriver10 1` |

### Display Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Display` | - | Show display info | `Display` |
| `DisplayModel` | 1-16 | Set display model | `DisplayModel 2` |
| `DisplayMode` | 0-5 | Set display mode | `DisplayMode 1` |
| `DisplayDimmer` | 0-100 | Set display brightness | `DisplayDimmer 50` |
| `DisplaySize` | 1-4 | Set display size | `DisplaySize 2` |
| `DisplayRotate` | 0-3 | Set display rotation | `DisplayRotate 2` |
| `DisplayText` | text | Display text | `DisplayText [s1l1]Hello World` |
| `DisplayClear` | - | Clear display | `DisplayClear` |

### Rule Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Rule1` | rule | Set rule 1 | `Rule1 ON Switch1#State DO Power1 %value% ENDON` |
| `Rule2` | rule | Set rule 2 | `Rule2 ON Time#Minute=30 DO Publish stat/alert 30min ENDON` |
| `Rule3` | rule | Set rule 3 | `Rule3 ON Button1#State DO Backlog Power1 TOGGLE; Delay 10; Power2 TOGGLE ENDON` |
| `RuleTimer1` | 0-3600 | Set rule timer 1 | `RuleTimer1 60` |
| `RuleTimer2` | 0-3600 | Set rule timer 2 | `RuleTimer2 120` |
| `Mem1` | value | Set memory 1 | `Mem1 Hello` |
| `Mem2` | value | Set memory 2 | `Mem2 World` |
| `Var1` | value | Set variable 1 | `Var1 42` |
| `Var2` | value | Set variable 2 | `Var2 3.14` |
| `CalcRes` | 0-7 | Set calculation resolution | `CalcRes 2` |

### Berry Script Commands (ESP32)

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `Br` | code | Execute Berry code | `Br print("Hello")` |
| `BrLoad` | filename | Load Berry file | `BrLoad autoexec.be` |
| `BrRun` | filename | Run Berry file | `BrRun script.be` |
| `BrRestart` | - | Restart Berry VM | `BrRestart` |

### Energy Monitoring Commands

| Command | Parameters | Description | Example |
|---------|------------|-------------|---------|
| `PowerCal` | value | Calibrate power | `PowerCal 12530` |
| `VoltageCal` | value | Calibrate voltage | `VoltageCal 1950` |
| `CurrentCal` | value | Calibrate current | `CurrentCal 3500` |
| `PowerSet` | watts | Set power reading | `PowerSet 100` |
| `VoltageSet` | volts | Set voltage reading | `VoltageSet 230` |
| `CurrentSet` | amps | Set current reading | `CurrentSet 0.43` |
| `FrequencySet` | hz | Set frequency reading | `FrequencySet 50` |
| `EnergyReset1` | kWh | Reset energy total | `EnergyReset1 0` |
| `EnergyReset2` | kWh | Reset energy yesterday | `EnergyReset2 0` |
| `EnergyReset3` | kWh | Reset energy today | `EnergyReset3 0` |
| `MaxPower` | watts | Set max power | `MaxPower 3500` |
| `MaxPowerHold` | seconds | Set max power hold | `MaxPowerHold 10` |
| `MaxPowerWindow` | seconds | Set max power window | `MaxPowerWindow 30` |
| `SafePower` | watts | Set safe power | `SafePower 3000` |
| `SafePowerHold` | seconds | Set safe power hold | `SafePowerHold 10` |
| `SafePowerWindow` | seconds | Set safe power window | `SafePowerWindow 30` |

## Complete Logging and Debug Reference

### Log Levels

```c
#define LOG_LEVEL_NONE     0  // No logging
#define LOG_LEVEL_ERROR    1  // Critical errors only
#define LOG_LEVEL_INFO     2  // Errors and info
#define LOG_LEVEL_DEBUG    3  // Errors, info and debug
#define LOG_LEVEL_DEBUG_MORE 4  // All logging
```

### Logging Functions

```c
// Main logging function
void AddLog(uint32_t loglevel, const char* formatP, ...);

// Convenience macros
#define AddLog_P(loglevel, formatP, ...) AddLog(loglevel, PSTR(formatP), ##__VA_ARGS__)
#define AddLog_P2(loglevel, formatP, ...) AddLog(loglevel, formatP, ##__VA_ARGS__)

// Debug logging (only in debug builds)
#ifdef DEBUG_TASMOTA_CORE
  #define DEBUG_CORE_LOG(...) AddLog(__VA_ARGS__)
#else
  #define DEBUG_CORE_LOG(...)
#endif

#ifdef DEBUG_TASMOTA_DRIVER  
  #define DEBUG_DRIVER_LOG(...) AddLog(__VA_ARGS__)
#else
  #define DEBUG_DRIVER_LOG(...)
#endif

#ifdef DEBUG_TASMOTA_SENSOR
  #define DEBUG_SENSOR_LOG(...) AddLog(__VA_ARGS__)
#else
  #define DEBUG_SENSOR_LOG(...)
#endif
```

### Debug Build Options

```c
// Enable in user_config_override.h for debugging
#define DEBUG_TASMOTA_CORE        // Core system debugging
#define DEBUG_TASMOTA_DRIVER      // Driver debugging  
#define DEBUG_TASMOTA_SENSOR      // Sensor debugging
#define USE_DEBUG_DRIVER          // Enable debug driver
#define DEBUG_TASMOTA_PORT Serial // Debug output port
```

### Memory Debugging

```c
// Memory monitoring functions
uint32_t ESP_getFreeHeap(void);
uint32_t ESP_getMaxAllocHeap(void);
uint8_t ESP_getHeapFragmentation(void);
uint32_t ESP_getFreeContStack(void);

// Memory debugging macros
#define SHOW_FREE_MEM(x) AddLog(LOG_LEVEL_DEBUG, PSTR(x " free mem: %d"), ESP_getFreeHeap())
#define CHECK_OOM() if (ESP_getFreeHeap() < 1000) AddLog(LOG_LEVEL_ERROR, PSTR("Low memory: %d"), ESP_getFreeHeap())
```

## Complete I2C Reference

### I2C Configuration

```c
// I2C pins (can be changed via GPIO configuration)
#define I2C_SDA_PIN 4   // Default SDA pin
#define I2C_SCL_PIN 5   // Default SCL pin

// I2C speeds
#define I2C_SPEED_SLOW    50000   // 50kHz
#define I2C_SPEED_STANDARD 100000 // 100kHz  
#define I2C_SPEED_FAST    400000  // 400kHz
#define I2C_SPEED_FAST_PLUS 1000000 // 1MHz
```

### I2C Helper Functions

```c
// Basic I2C operations
bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size);
bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg);
bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg);
bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg);
bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg);
bool I2cValidReadS32(int32_t *data, uint8_t addr, uint8_t reg);
bool I2cValidReadS32_LE(int32_t *data, uint8_t addr, uint8_t reg);

uint8_t I2cRead8(uint8_t addr, uint8_t reg);
uint16_t I2cRead16(uint8_t addr, uint8_t reg);
uint16_t I2cRead16LE(uint8_t addr, uint8_t reg);
int32_t I2cRead24(uint8_t addr, uint8_t reg);
int32_t I2cReadS32(uint8_t addr, uint8_t reg);
int32_t I2cReadS32_LE(uint8_t addr, uint8_t reg);

bool I2cWrite8(uint8_t addr, uint8_t reg, uint8_t val);
bool I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val);
bool I2cWrite16LE(uint8_t addr, uint8_t reg, uint16_t val);

// Buffer operations
uint8_t I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);
uint8_t I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len);

// Device detection
bool I2cActive(uint8_t addr);
void I2cScan(char *devs, unsigned int devs_len);
void I2cResetActive(uint8_t addr, uint8_t count = 1);
void I2cSetActive(uint8_t addr, uint8_t count = 1);
void I2cSetActiveFound(uint8_t addr, const char *types);
```

### I2C Device Detection Pattern

```c
void MySensorDetect(void) {
  if (MySensorDetected) return;
  
  for (uint32_t i = 0; i < SENSOR_MAX_ADDR; i++) {
    uint8_t addr = SENSOR_BASE_ADDR + i;
    if (I2cActive(addr)) continue;  // Address already in use
    
    if (I2cValidRead8(&sensor_id, addr, SENSOR_ID_REG)) {
      if (sensor_id == EXPECTED_SENSOR_ID) {
        I2cSetActiveFound(addr, "MySensor");
        MySensorDetected = true;
        MySensorAddress = addr;
        AddLog(LOG_LEVEL_INFO, PSTR("MySensor found at address 0x%02X"), addr);
        break;
      }
    }
  }
}
```

This comprehensive developer reference provides all the essential information needed to understand, extend, and debug Tasmota firmware. The detailed callback system, complete command reference, GPIO configuration options, and debugging tools give developers everything needed to create robust IoT solutions.
