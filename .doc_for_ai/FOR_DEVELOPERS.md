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
├── tasmota.ino          # Main firmware file
├── xdrv_*.ino          # Driver files (displays, interfaces, etc.)
├── xsns_*.ino          # Sensor files
├── xlgt_*.ino          # Light driver files
├── xnrg_*.ino          # Energy monitoring files
├── support_*.ino       # Support functions
├── settings.h          # Configuration structure
└── my_user_config.h    # User configuration overrides
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

#### Core System Callbacks

| Function | ID | Purpose | Frequency | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_INIT` | 0 | Initialize driver/sensor | Once at startup | None |
| `FUNC_LOOP` | 1 | Main loop processing | Every loop cycle (~1ms) | None |
| `FUNC_EVERY_50_MSECOND` | 2 | Fast polling operations | Every 50ms | None |
| `FUNC_EVERY_100_MSECOND` | 3 | Medium polling | Every 100ms | None |
| `FUNC_EVERY_200_MSECOND` | 4 | Slower polling | Every 200ms | None |
| `FUNC_EVERY_250_MSECOND` | 5 | Quarter second tasks | Every 250ms | None |
| `FUNC_EVERY_SECOND` | 6 | Regular updates | Every second | None |
| `FUNC_PREP_BEFORE_TELEPERIOD` | 7 | Prepare telemetry data | Before TelePeriod | None |
| `FUNC_JSON_APPEND` | 8 | Add JSON telemetry | Every TelePeriod | None |
| `FUNC_BUTTON_PRESSED` | 9 | Handle button press | On button event | button_index |
| `FUNC_SAVE_BEFORE_RESTART` | 10 | Save critical data | Before restart | None |
| `FUNC_AFTER_TELEPERIOD` | 11 | Post-telemetry cleanup | After TelePeriod | None |

#### Communication Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_COMMAND` | 12 | Process commands | Command received | XdrvMailbox |
| `FUNC_MQTT_SUBSCRIBE` | 13 | Subscribe to MQTT topics | MQTT connect | None |
| `FUNC_MQTT_INIT` | 14 | Initialize MQTT | MQTT setup | None |
| `FUNC_MQTT_DATA` | 15 | Process MQTT data | MQTT message | XdrvMailbox |
| `FUNC_SET_POWER` | 16 | Handle power changes | Power state change | None |
| `FUNC_SHOW_SENSOR` | 17 | Display sensor data | Status request | None |
| `FUNC_RULES_PROCESS` | 18 | Process rules | Rule evaluation | None |

#### Web Interface Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_WEB_ADD_BUTTON` | 19 | Add web buttons | Main page load | None |
| `FUNC_WEB_ADD_MAIN_BUTTON` | 20 | Add main menu button | Main page | None |
| `FUNC_WEB_ADD_HANDLER` | 21 | Add URL handlers | Web server init | None |
| `FUNC_WEB_SENSOR` | 22 | Show sensor on web | Sensor page load | None |
| `FUNC_WEB_ADD_CONSOLE_BUTTON` | 23 | Add console button | Console page | None |
| `FUNC_WEB_ADD_MANAGEMENT_BUTTON` | 24 | Add config button | Config page | None |

#### Network and Protocol Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_NETWORK_UP` | 25 | Network connected | WiFi/Ethernet up | None |
| `FUNC_NETWORK_DOWN` | 26 | Network disconnected | WiFi/Ethernet down | None |
| `FUNC_MQTT_CONNECTED` | 27 | MQTT broker connected | MQTT connect | None |
| `FUNC_MQTT_DISCONNECTED` | 28 | MQTT broker disconnected | MQTT disconnect | None |
| `FUNC_SET_DEVICE_POWER` | 29 | Device power control | Power command | device, power |
| `FUNC_SHOW_SENSOR_JSON` | 30 | JSON sensor output | JSON request | None |

#### Advanced System Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_SERIAL` | 31 | Serial data processing | Serial input | None |
| `FUNC_FREE_MEM` | 32 | Memory cleanup | Low memory | None |
| `FUNC_BUTTON_MULTI_PRESSED` | 33 | Multi-button press | Button combo | button_code |
| `FUNC_ENERGY_EVERY_SECOND` | 34 | Energy monitoring | Every second | None |
| `FUNC_ACTIVE` | 35 | Driver active check | Status query | None |
| `FUNC_PIN_STATE` | 36 | GPIO state change | Pin change | gpio, state |
| `FUNC_TELEPERIOD_RULES_PROCESS` | 37 | Rules after telemetry | Post-TelePeriod | None |

#### Display and UI Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_DISPLAY_INIT_DRIVER` | 38 | Initialize display | Display setup | None |
| `FUNC_DISPLAY_MODEL` | 39 | Set display model | Display config | None |
| `FUNC_DISPLAY_MODE` | 40 | Set display mode | Mode change | None |
| `FUNC_DISPLAY_POWER` | 41 | Display power control | Power change | None |
| `FUNC_DISPLAY_CLEAR` | 42 | Clear display | Clear command | None |
| `FUNC_DISPLAY_DRAW_HLINE` | 43 | Draw horizontal line | Draw command | x, y, len, color |
| `FUNC_DISPLAY_DRAW_VLINE` | 44 | Draw vertical line | Draw command | x, y, len, color |
| `FUNC_DISPLAY_DRAW_CIRCLE` | 45 | Draw circle | Draw command | x, y, rad, color |
| `FUNC_DISPLAY_FILL_CIRCLE` | 46 | Fill circle | Draw command | x, y, rad, color |
| `FUNC_DISPLAY_DRAW_RECTANGLE` | 47 | Draw rectangle | Draw command | x, y, w, h, color |
| `FUNC_DISPLAY_FILL_RECTANGLE` | 48 | Fill rectangle | Draw command | x, y, w, h, color |
| `FUNC_DISPLAY_TEXT_SIZE` | 49 | Set text size | Text command | size |
| `FUNC_DISPLAY_FONT_SIZE` | 50 | Set font size | Font command | size |
| `FUNC_DISPLAY_DRAW_STRING` | 51 | Draw text string | Text command | x, y, text, color |
| `FUNC_DISPLAY_DRAW_STRING_AT` | 52 | Draw text at position | Text command | x, y, text, color |
| `FUNC_DISPLAY_PRINTF` | 53 | Printf to display | Text command | format, args |
| `FUNC_DISPLAY_ROTATION` | 54 | Set rotation | Rotation command | angle |
| `FUNC_DISPLAY_DRAW_FRAME` | 55 | Draw frame | Draw command | None |

#### Berry Script Integration Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_BERRY_INIT` | 56 | Initialize Berry | Berry startup | None |
| `FUNC_BERRY_LOOP` | 57 | Berry main loop | Every loop | None |
| `FUNC_BERRY_EVERY_50_MSECOND` | 58 | Berry fast timer | Every 50ms | None |
| `FUNC_BERRY_EVERY_100_MSECOND` | 59 | Berry medium timer | Every 100ms | None |
| `FUNC_BERRY_EVERY_SECOND` | 60 | Berry second timer | Every second | None |
| `FUNC_BERRY_RULES_PROCESS` | 61 | Berry rules processing | Rule trigger | None |

#### Matter Protocol Callbacks (ESP32)

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_MATTER_INIT` | 62 | Initialize Matter | Matter startup | None |
| `FUNC_MATTER_LOOP` | 63 | Matter processing | Every loop | None |
| `FUNC_MATTER_EVERY_50_MSECOND` | 64 | Matter fast timer | Every 50ms | None |
| `FUNC_MATTER_EVERY_SECOND` | 65 | Matter second timer | Every second | None |
| `FUNC_MATTER_COMMAND` | 66 | Matter commands | Matter command | None |
| `FUNC_MATTER_JSON_APPEND` | 67 | Matter JSON data | Telemetry | None |

#### Audio and Media Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_AUDIO_INIT` | 68 | Initialize audio | Audio setup | None |
| `FUNC_AUDIO_LOOP` | 69 | Audio processing | Audio loop | None |
| `FUNC_AUDIO_COMMAND` | 70 | Audio commands | Audio command | None |
| `FUNC_AUDIO_SHOW_SENSOR` | 71 | Audio sensor data | Status request | None |

#### Zigbee Protocol Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_ZIGBEE_INIT` | 72 | Initialize Zigbee | Zigbee startup | None |
| `FUNC_ZIGBEE_LOOP` | 73 | Zigbee processing | Every loop | None |
| `FUNC_ZIGBEE_EVERY_50_MSECOND` | 74 | Zigbee fast timer | Every 50ms | None |
| `FUNC_ZIGBEE_COMMAND` | 75 | Zigbee commands | Zigbee command | None |
| `FUNC_ZIGBEE_JSON_APPEND` | 76 | Zigbee JSON data | Telemetry | None |

#### Energy Management Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_ENERGY_INIT` | 77 | Initialize energy monitor | Energy setup | None |
| `FUNC_ENERGY_LOOP` | 78 | Energy processing | Every loop | None |
| `FUNC_ENERGY_SHOW_SENSOR` | 79 | Energy sensor display | Status request | None |
| `FUNC_ENERGY_RESET` | 80 | Reset energy counters | Reset command | None |

#### Light Control Callbacks

| Function | ID | Purpose | When Called | Parameters |
|----------|----|---------|-----------|-----------| 
| `FUNC_LIGHT_INIT` | 81 | Initialize lighting | Light setup | None |
| `FUNC_LIGHT_LOOP` | 82 | Light processing | Every loop | None |
| `FUNC_LIGHT_EVERY_50_MSECOND` | 83 | Light fast updates | Every 50ms | None |
| `FUNC_LIGHT_SCHEME_CHANGE` | 84 | Light scheme change | Scheme update | None |
| `FUNC_LIGHT_POWER_CHANGE` | 85 | Light power change | Power update | None |

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
