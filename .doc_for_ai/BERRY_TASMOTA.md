# Berry for Tasmota

This document covers Tasmota-specific Berry features and extensions, complementing the general Berry language reference.

## Introduction

Berry is the next generation scripting language for Tasmota, embedded by default in all ESP32 based firmwares (NOT supported on ESP8266). It is used for advanced scripting, superseding Rules, and enables building drivers, automations, and UI extensions.

## Tasmota-Specific Modules

Beyond standard Berry modules, Tasmota provides additional modules:

| Module | Description | Import |
|--------|-------------|--------|
| `tasmota` | Core integration module | Automatically imported |
| `light` | Light control | Automatically imported |
| `mqtt` | MQTT operations | `import mqtt` |
| `webserver` | Web server extensions | `import webserver` |
| `gpio` | GPIO control | `import gpio` |
| `persist` | Data persistence | `import persist` |
| `path` | File system operations | `import path` |
| `energy` | Energy monitoring | Automatically imported |
| `display` | Display driver integration | `import display` |
| `crypto` | Cryptographic functions | `import crypto` |
| `re` | Regular expressions | `import re` |
| `mdns` | mDNS/Bonjour support | `import mdns` |
| `ULP` | Ultra Low Power coprocessor | `import ULP` |
| `uuid` | UUID generation | `import uuid` |
| `crc` | CRC calculations | `import crc` |

## Additional Resources

For Tasmota-specific Berry features and extensions, please refer to the companion document `BERRY_TASMOTA.md`.

### Tasmota Constants and Enums

```berry
# GPIO constants (gpio module)
gpio.INPUT, gpio.OUTPUT, gpio.PULLUP, gpio.PULLDOWN
gpio.HIGH, gpio.LOW
gpio.REL1, gpio.KEY1, gpio.LED1, gpio.I2C_SCL, gpio.I2C_SDA
# ... many more GPIO function constants

# Serial constants
serial.SERIAL_8N1, serial.SERIAL_7E1, etc.

# Webserver constants  
webserver.HTTP_GET, webserver.HTTP_POST, webserver.HTTP_OPTIONS, webserver.HTTP_ANY
webserver.HTTP_OFF, webserver.HTTP_USER, webserver.HTTP_ADMIN, webserver.HTTP_MANAGER
webserver.HTTP_MANAGER_RESET_ONLY
webserver.BUTTON_MAIN, webserver.BUTTON_CONFIGURATION, webserver.BUTTON_INFORMATION
webserver.BUTTON_MANAGEMENT, webserver.BUTTON_MODULE
```

### Console and REPL

Access Berry console via *Configuration* → *Berry Scripting Console*. The console supports:
- Multi-line input (press Enter twice or click "Run")
- Command history (arrow keys)
- Colorful syntax highlighting
- Berry VM restart with `BrRestart` command

### File System and Loading

Berry files can be source (`.be`) or pre-compiled bytecode (`.bec`):

```berry
load("filename")        # Loads .be or .bec file
tasmota.compile("file.be")  # Compiles .be to .bec
```

**Autostart**: Place `autoexec.be` in filesystem to run Berry code at boot.

### Tasmota Integration Functions

#### Core Tasmota Functions

```berry
# System information
tasmota.get_free_heap()     # Free heap bytes
tasmota.memory()            # Memory stats map
tasmota.arch()              # Architecture: "esp32", "esp32s2", etc.
tasmota.millis()            # Milliseconds since boot
tasmota.yield()             # Give time to low-level functions
tasmota.delay(ms)           # Block execution for ms milliseconds

# Commands and responses
tasmota.cmd("command")      # Execute Tasmota command
tasmota.resp_cmnd_done()    # Respond "Done"
tasmota.resp_cmnd_error()   # Respond "Error"
tasmota.resp_cmnd_str(msg)  # Custom response string
tasmota.resp_cmnd(json)     # Custom JSON response

# Configuration
tasmota.get_option(index)   # Get SetOption value
tasmota.read_sensors()      # Get sensor JSON string
tasmota.wifi()              # WiFi connection info
tasmota.eth()               # Ethernet connection info
```

#### Rules and Events

```berry
# Add rules (similar to Tasmota Rules but more powerful)
tasmota.add_rule("trigger", function)
tasmota.add_rule(["trigger1", "trigger2"], function)  # AND logic
tasmota.remove_rule("trigger")

# Rule function signature
def rule_function(value, trigger, msg)
  # value: trigger value (%value% equivalent)
  # trigger: full trigger string
  # msg: parsed JSON map or original string
end

# Examples
tasmota.add_rule("Dimmer>50", def() print("Bright!") end)
tasmota.add_rule("ANALOG#A1>300", def(val) print("ADC:", val) end)
```

#### Timers and Scheduling

```berry
# Timers (50ms resolution)
tasmota.set_timer(delay_ms, function)
tasmota.remove_timer(id)
tasmota.defer(function)     # Run in next millisecond

# Cron scheduling
tasmota.add_cron("*/15 * * * * *", function, "id")
tasmota.remove_cron("id")
tasmota.next_cron("id")     # Next execution timestamp

# Time functions
tasmota.rtc()               # Current time info
tasmota.time_dump(timestamp) # Decompose timestamp
tasmota.time_str(timestamp)  # ISO 8601 string
tasmota.strftime(format, timestamp)
tasmota.strptime(time_str, format)
```

#### Device Control

```berry
# Relays and Power
tasmota.get_power()         # Array of relay states
tasmota.set_power(idx, state) # Set relay state

# Lights (use light module)
light.get()                 # Current light status
light.set({"power": true, "bri": 128, "hue": 120})

# Light attributes: power, bri (0-255), hue (0-360), sat (0-255), 
# ct (153-500), rgb (hex string), channels (array)
```

#### Custom Commands

```berry
# Add custom Tasmota commands
def my_command(cmd, idx, payload, payload_json)
  # cmd: command name, idx: command index
  # payload: raw string, payload_json: parsed JSON
  tasmota.resp_cmnd_done()
end

tasmota.add_cmd("MyCmd", my_command)
tasmota.remove_cmd("MyCmd")
```

### Tasmota Drivers

Create complete Tasmota drivers by implementing event methods:

```berry
class MyDriver
  def every_second()     # Called every second
  end
  
  def every_50ms()       # Called every 50ms
  end
  
  def web_sensor()       # Add to web UI
    tasmota.web_send("{s}Sensor{m}Value{e}")
  end
  
  def json_append()      # Add to JSON teleperiod
    tasmota.response_append(',"MySensor":{"Value":123}')
  end
  
  def web_add_main_button()  # Add button to main page
    import webserver
    webserver.content_send("<button onclick='la(\"&myaction=1\");'>My Button</button>")
  end
  
  def button_pressed()   # Handle button press
  end
  
  def mqtt_data(topic, idx, data, databytes)  # Handle MQTT
  end
  
  def save_before_restart()  # Before restart
  end
end

# Register driver
driver = MyDriver()
tasmota.add_driver(driver)
```

### Fast Loop

For near real-time events (200Hz, 5ms intervals):

```berry
def fast_function()
  # High-frequency processing
end

tasmota.add_fast_loop(fast_function)
tasmota.remove_fast_loop(fast_function)
```

### GPIO Control

```berry
import gpio

# GPIO detection and control
gpio.pin_used(gpio.REL1)        # Check if GPIO is used
gpio.pin(gpio.REL1)             # Get physical GPIO number
gpio.digital_write(pin, gpio.HIGH)  # Set GPIO state
gpio.digital_read(pin)          # Read GPIO state
gpio.pin_mode(pin, gpio.OUTPUT) # Set GPIO mode

# PWM control
gpio.set_pwm(pin, duty, phase)  # Set PWM value
gpio.set_pwm_freq(pin, freq)    # Set PWM frequency

# DAC (ESP32 GPIO 25-26, ESP32-S2 GPIO 17-18)
gpio.dac_voltage(pin, voltage_mv)  # Set DAC voltage

# Counters
gpio.counter_read(counter)      # Read counter value
gpio.counter_set(counter, value) # Set counter value
```

### I²C Communication

```berry
# Wire objects: wire1, wire2 for I²C buses
wire1.bus -> int                    # Bus number (read-only)
wire1.enabled() -> bool             # Check if bus initialized
wire1.scan() -> list(int)           # Scan for device addresses (decimal)
wire1.detect(addr:int) -> bool      # Check if device present

# High-level I/O
wire1.read(addr:int, reg:int, size:int) -> int|nil        # Read 1-4 bytes
wire1.write(addr:int, reg:int, val:int, size:int) -> bool # Write 1-4 bytes
wire1.read_bytes(addr:int, reg:int, size:int) -> bytes    # Read byte sequence
wire1.write_bytes(addr:int, reg:int, val:bytes) -> nil    # Write byte sequence

# Low-level control
wire1._begin_transmission(addr:int) -> nil
wire1._end_transmission([stop:bool]) -> nil
wire1._request_from(addr:int, size:int, [stop:bool]) -> nil
wire1._available() -> bool
wire1._read() -> int                # Read single byte
wire1._write(val:int|string) -> nil # Write single byte or string

# Device discovery
wire = tasmota.wire_scan(addr:int, i2c_index:int) -> wire_instance|nil
```

### MQTT Integration

```berry
import mqtt

# MQTT operations
mqtt.publish(topic:string, payload:string|bytes, [retain:bool, start:int, len:int]) -> nil
mqtt.subscribe(topic:string, [function:closure]) -> nil  # Pattern matching, add wildcards manually
mqtt.unsubscribe(topic:string) -> nil
mqtt.connected() -> bool

# Callback function signature (topic, idx, payload_s, payload_b) -> bool
def mqtt_callback(topic, idx, payload_s, payload_b)
  # topic: full topic, idx: unused, payload_s: string, payload_b: bytes
  return true  # Return true if handled (prevents Tasmota command)
end
```

### Web Server Extensions

```berry
import webserver

# In driver's web_add_handler() method
webserver.on("/my_page", def() 
  webserver.content_send("<html>My Page</html>")
end)

# Request handling
webserver.has_arg("param")      # Check parameter exists
webserver.arg("param")          # Get parameter value
webserver.arg_size()            # Number of parameters

# Response functions
webserver.content_send(html)    # Send HTML content
webserver.content_button()      # Standard button
webserver.html_escape(str)      # Escape HTML
```

### Persistence

```berry
import persist

# Automatic persistence to _persist.json
persist.my_value = 123
persist.save()                  # Force save to flash
persist.has("key")              # Check if key exists
persist.remove("key")           # Remove key
persist.find("key", default)    # Get with default
```

### Network Clients

#### HTTP/HTTPS Client

```berry
cl = webclient()
cl.begin("https://example.com/api")
cl.set_auth("user", "pass")
cl.add_header("Content-Type", "application/json")

result = cl.GET()               # or POST(payload)
if result == 200
  response = cl.get_string()
  # or cl.write_file("filename") for binary
end
cl.close()
```

#### TCP Client

```berry
tcp = tcpclient()
tcp.connect("192.168.1.100", 80)
tcp.write("GET / HTTP/1.0\r\n\r\n")
response = tcp.read()
tcp.close()
```

#### UDP Communication

```berry
u = udp()
u.begin("", 2000)               # Listen on port 2000
u.send("192.168.1.10", 2000, bytes("Hello"))

# Receive (polling)
packet = u.read()               # Returns bytes or nil
if packet
  print("From:", u.remote_ip, u.remote_port)
end
```

### Serial Communication

```berry
ser = serial(rx_gpio, tx_gpio, baud, serial.SERIAL_8N1)
ser.write(bytes("Hello"))       # Send data
data = ser.read()               # Read available data
ser.available()                 # Check bytes available
ser.flush()                     # Flush buffers
ser.close()                     # Close port
```

### Cryptography

```berry
import crypto

# AES Encryption Classes
crypto.AES_CTR(key:bytes(32)).encrypt(data:bytes, iv:bytes(12), cc:int) -> bytes
crypto.AES_CTR(key:bytes(32)).decrypt(data:bytes, iv:bytes(12), cc:int) -> bytes

crypto.AES_GCM(key:bytes(32), iv:bytes(12)).encrypt(data:bytes) -> bytes
crypto.AES_GCM(key:bytes(32), iv:bytes(12)).decrypt(data:bytes) -> bytes
crypto.AES_GCM(key:bytes(32), iv:bytes(12)).tag() -> bytes(16)

crypto.AES_CCM(key:bytes(16|32), iv:bytes(7..13), aad:bytes, data_len:int, tag_len:int)
crypto.AES_CCM.encrypt1/decrypt1(...) -> bool  # Single-call variants

crypto.AES_CBC.encrypt1(key:bytes(16), iv:bytes(16), data:bytes) -> bool
crypto.AES_CBC.decrypt1(key:bytes(16), iv:bytes(16), data:bytes) -> bool

# Elliptic Curve (requires defines)
crypto.EC_C25519().public_key(priv:bytes(32)) -> bytes(32)
crypto.EC_C25519().shared_key(our_priv:bytes(32), their_pub:bytes(32)) -> bytes(32)

crypto.EC_P256().public_key(priv:bytes(32)) -> bytes(65)
crypto.EC_P256().shared_key(our_priv:bytes(32), their_pub:bytes(65)) -> bytes(32)
crypto.EC_P256().mod/neg/mul/muladd(...) -> bytes  # Math operations

# Key Derivation
crypto.HKDF_SHA256().derive(ikm:bytes, salt:bytes, info:bytes, out_len:int) -> bytes
crypto.PBKDF2_HMAC_SHA256().derive(pwd:bytes, salt:bytes, iter:int, out_len:int) -> bytes

# Hashing
crypto.SHA256().update(data:bytes).out() -> bytes(32)
crypto.MD5().update(data:bytes).finish() -> bytes(16)
crypto.HMAC_SHA256(key:bytes).update(data:bytes).out() -> bytes(32)

# RSA (requires define)
crypto.RSA.rs256(private_key_der:bytes, payload:bytes) -> bytes  # JWT signing
```

### File System Operations

```berry
import path

# File/directory operations (SD card: /sd/ subdirectory)
path.exists(file:string) -> bool        # Check file exists
path.isdir(name:string) -> bool         # Check if directory
path.listdir(dir:string) -> list        # List directory contents
path.mkdir(dir:string) -> bool          # Create directory
path.rmdir(dir:string) -> bool          # Remove empty directory
path.remove(file:string) -> bool        # Delete file
path.rename(old:string, new:string) -> bool  # Rename file/folder
path.last_modified(file:string) -> int  # File timestamp (nil if not exists)
path.format(true) -> bool               # Format LittleFS (erases all!)
```

### Regular Expressions

```berry
import re

# Pattern matching
matches = re.search("a.*?b(z+)", "aaaabbbzzz")  # Returns matches array
all_matches = re.searchall('<([a-zA-Z]+)>', html)  # All matches
parts = re.split('/', "path/to/file")  # Split string

# Compiled patterns (faster for reuse)
pattern = re.compilebytes("\\d+")
matches = re.search(pattern, "abc123def")
```

### Energy Monitoring

```berry
# Read energy values
energy.voltage                  # Main phase voltage
energy.current                  # Main phase current  
energy.active_power            # Active power (W)
energy.total                   # Total energy (kWh)

# Multi-phase access
energy.voltage_phases[0]       # Phase 0 voltage
energy.current_phases[1]       # Phase 1 current

# Berry energy driver (with OPTION_A 9 GPIO)
if energy.driver_enabled()
  energy.voltage = 240
  energy.current = 1.5
  energy.active_power = 360    # This drives energy calculation
end
```

### Display Integration

```berry
import display

# Initialize display driver
display.start(display_ini_string)
display.started()              # Check if initialized
display.dimmer(50)             # Set brightness 0-100
display.driver_name()          # Get driver name

# Touch screen updates
display.touch_update(touches, x, y, gesture)
```

### Advanced Features

#### ULP (Ultra Low Power) Coprocessor

```berry
import ULP

ULP.wake_period(0, 500000)     # Configure wake timer
ULP.load(bytecode)             # Load ULP program
ULP.run()                      # Execute ULP program
ULP.set_mem(addr, value)       # Set RTC memory
ULP.get_mem(addr)              # Get RTC memory
```

#### mDNS Support

```berry
import mdns

mdns.start("hostname")         # Start mDNS
mdns.add_service("_http", "_tcp", 80, {"path": "/"})
mdns.stop()                    # Stop mDNS
```

### Error Handling Patterns

Many Tasmota functions return `nil` for errors rather than raising exceptions:

```berry
# Check return values
data = json.load(json_string)
if data == nil
  print("Invalid JSON")
end

# Wire operations
result = wire1.read(addr, reg, 1)
if result == nil
  print("I2C read failed")
end
```

### Best Practices for Tasmota

1. **Memory Management**: Use `tasmota.gc()` to monitor memory usage
2. **Non-blocking**: Use timers instead of `delay()` for long waits
3. **Error Handling**: Always check return values for `nil`
4. **Persistence**: Use `persist` module for settings that survive reboots
5. **Performance**: Use fast_loop sparingly, prefer regular driver events
6. **Debugging**: Enable `#define USE_BERRY_DEBUG` for development

## Common Tasmota Berry Patterns

### Simple Sensor Driver

```berry
class MySensor
  var wire, addr
  
  def init()
    self.addr = 0x48
    self.wire = tasmota.wire_scan(self.addr, 99)  # I2C index 99
    if self.wire
      print("MySensor found on bus", self.wire.bus)
    end
  end
  
  def every_second()
    if !self.wire return end
    var temp = self.wire.read(self.addr, 0x00, 2)  # Read temperature
    self.temperature = temp / 256.0  # Convert to Celsius
  end
  
  def web_sensor()
    if !self.wire return end
    import string
    var msg = string.format("{s}MySensor Temp{m}%.1f °C{e}", self.temperature)
    tasmota.web_send_decimal(msg)
  end
  
  def json_append()
    if !self.wire return end
    import string
    var msg = string.format(',"MySensor":{"Temperature":%.1f}', self.temperature)
    tasmota.response_append(msg)
  end
end

sensor = MySensor()
tasmota.add_driver(sensor)
```

### Custom Command with JSON Response

```berry
def my_status_cmd(cmd, idx, payload, payload_json)
  import string
  var response = {
    "Uptime": tasmota.millis(),
    "FreeHeap": tasmota.get_free_heap(),
    "WiFi": tasmota.wifi("rssi")
  }
  tasmota.resp_cmnd(json.dump(response))
end

tasmota.add_cmd("MyStatus", my_status_cmd)
```

### MQTT Automation

```berry
import mqtt

def handle_sensor_data(topic, idx, payload_s, payload_b)
  var data = json.load(payload_s)
  if data && data.find("temperature")
    var temp = data["temperature"]
    if temp > 25
      tasmota.cmd("Power1 ON")  # Turn on fan
    elif temp < 20  
      tasmota.cmd("Power1 OFF") # Turn off fan
    end
  end
  return true
end

mqtt.subscribe("sensors/+/temperature", handle_sensor_data)
```

### Web UI Button with Action

```berry
class WebButton
  def web_add_main_button()
    import webserver
    webserver.content_send("<p><button onclick='la(\"&toggle_led=1\");'>Toggle LED</button></p>")
  end
  
  def web_sensor()
    import webserver
    if webserver.has_arg("toggle_led")
      # Toggle GPIO2 (built-in LED on many ESP32 boards)
      var pin = 2
      var current = gpio.digital_read(pin)
      gpio.digital_write(pin, !current)
      print("LED toggled to", !current)
    end
  end
end

button = WebButton()
tasmota.add_driver(button)
```

### Scheduled Task with Persistence

```berry
import persist

class ScheduledTask
  def init()
    if !persist.has("task_count")
      persist.task_count = 0
    end
    # Run every 5 minutes
    tasmota.add_cron("0 */5 * * * *", /-> self.run_task(), "my_task")
  end
  
  def run_task()
    persist.task_count += 1
    print("Task executed", persist.task_count, "times")
    
    # Do something useful
    var sensors = tasmota.read_sensors()
    print("Current sensors:", sensors)
    
    persist.save()  # Save counter to flash
  end
end

task = ScheduledTask()
```

### HTTP API Client

```berry
class WeatherAPI
  var api_key, city
  
  def init(key, city_name)
    self.api_key = key
    self.city = city_name
    tasmota.add_cron("0 0 * * * *", /-> self.fetch_weather(), "weather")
  end
  
  def fetch_weather()
    var cl = webclient()
    var url = f"http://api.openweathermap.org/data/2.5/weather?q={self.city}&appid={self.api_key}"
    
    cl.begin(url)
    var result = cl.GET()
    
    if result == 200
      var response = cl.get_string()
      var data = json.load(response)
      if data
        var temp = data["main"]["temp"] - 273.15  # Kelvin to Celsius
        print(f"Weather in {self.city}: {temp:.1f}°C")
        
        # Store in global for other scripts to use
        import global
        global.weather_temp = temp
      end
    end
    cl.close()
  end
end

# weather = WeatherAPI("your_api_key", "London")
```

### Rule-based Automation

```berry
# Advanced rule that combines multiple conditions
tasmota.add_rule(["ANALOG#A0>500", "Switch1#State=1"], 
  def(values, triggers)
    print("Both conditions met:")
    print("ADC value:", values[0])
    print("Switch state:", values[1])
    tasmota.cmd("Power2 ON")  # Activate something
  end
)

# Time-based rule
tasmota.add_rule("Time#Minute=30", 
  def()
    if tasmota.rtc()["hour"] == 18  # 6:30 PM
      tasmota.cmd("Dimmer 20")  # Dim lights for evening
    end
  end
)
```

## Best Practices and Tips

1. **Always check for nil returns** from Tasmota functions
2. **Use timers instead of delay()** to avoid blocking Tasmota
3. **Implement proper error handling** in I²C and network operations  
4. **Use persist module** for settings that should survive reboots
5. **Test memory usage** with `tasmota.gc()` during development
6. **Use fast_loop sparingly** - it runs 200 times per second
7. **Prefer driver events** over polling when possible
8. **Use f-strings** for readable string formatting
9. **Import modules only when needed** to save memory
10. **Use `tasmota.wire_scan()`** instead of manual I²C bus detection
