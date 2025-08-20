# Tasmota Berry Intelligence v1.2.0
## Beta Version - Complete Berry Language & Implementation Knowledge Base

### Executive Summary
Berry is a lightweight embedded scripting language integrated into Tasmota IoT firmware for ESP32 devices. This intelligence provides comprehensive coverage of the Berry language syntax, core features, Tasmota-specific extensions, and complete API documentation for 91+ native libraries and 36+ functional modules.

### System Architecture

#### Core Components
- **Berry VM**: Lightweight virtual machine with garbage collection, 32-bit integers, single-precision floats
- **Native Bindings**: 65+ C/C++ functions exposed to Berry scripts via BE_FUNC_CTYPE_DECLARE
- **Module System**: 36+ functional modules organized by domain (GPIO, I2C, MQTT, etc.)
- **Event System**: Reactive programming through callbacks, rules, and fast_loop (200Hz)
- **Solidification**: Pre-compiled bytecode (.bec) for performance and memory optimization

#### File Organization
```
berry_documentations/
├── src/lib berry/        # Native implementations
│   ├── be_*.c           # C bindings (65 files)
│   ├── embedded/*.be    # Berry scripts (30 files)
│   └── solidify/*.h     # Compiled bytecode (34 files)
├── src/xdrv_52_*.ino    # Arduino integration (42 files)
└── output/              # Generated documentation
```

### Language Syntax

#### Reserved Keywords (31)
```
if          elif        else        while       for
def         end         class       return      break
continue    import      as          var         static
try         except      raise       nil         true
false       self        super       do          and
or          not         in          typeof      classof
classname   isinstance  module
```

#### Data Types
**Primitive**: nil, bool, int (32-bit), real (float), string (UTF-8 immutable)
**Composite**: list (mutable), map (dict), range (iterator), bytes (mutable buffer)
**Function**: closure, function, method
**Object**: class, instance, module, comptr (C pointer)

#### Operators (Precedence High→Low)
1. `()` grouping, `[]` subscript, `.` member
2. `-` unary, `!` `~` NOT
3. `**` power, `*` `/` `//` `%`
4. `+` `-` (binary)
5. `<<` `>>` shift
6. `&` AND, `^` XOR, `|` OR
7. `..` range/concat
8. `<` `<=` `>` `>=` `==` `!=`
9. `&&` logical AND, `||` logical OR
10. `? :` ternary
11. `=` `+=` `-=` `*=` `/=` `//=` `%=` `&=` `|=` `^=` `<<=` `>>=`
12. `:=` walrus assignment

#### Control Flow
```berry
# Conditionals
if condition
  # code
elif other
  # code  
else
  # code
end

# Loops
while condition end
for i : 0..9 end
for item : list end
do # code while condition

# Jumps
break
continue
return [value]
```

#### Functions
```berry
# Standard definition
def func(a, b = default)
  return a + b
end

# Variadic
def func(*args) end

# Anonymous/Lambda
var f = def(x) return x * 2 end
var f = /x -> x * 2
var f = /(x, y) -> x + y

# Dynamic call
call(func, arg1, [arg2, arg3])
```

#### Classes
```berry
class MyClass : ParentClass
  static class_var = 0
  var instance_var
  
  def init(param)
    super(self).init(param)
    self.instance_var = param
  end
  
  # Special methods
  def tostring() end      # String representation
  def item(index) end     # [] getter
  def setitem(idx, val) end # [] setter
  def iter() end          # Iterator
  def next() end          # Iterator next
  def +(other) end        # Operator overload
end
```

#### Exception Handling
```berry
try
  # risky code
except "error_type" as e, msg
  # handle specific
except ..
  # catch all
else
  # no exception
ensure
  # always execute (finally)
end

raise "error_type", "message"
```

Common exceptions: `assert_failed`, `index_error`, `io_error`, `key_error`, `runtime_error`, `stop_iteration`, `syntax_error`, `type_error`, `value_error`, `memory_error`

### Module Taxonomy

#### 1. Core System (4 modules)
**tasmota**: Central orchestration, commands, system state
**introspect**: Reflection, type inspection, dynamic access
**persist**: State persistence to _persist.json
**global**: Global variable access

Key APIs:
```berry
tasmota.cmd("Command")           # Execute command
tasmota.publish(topic, payload)  # MQTT publish
tasmota.add_rule(trigger, func)  # Event handler
tasmota.millis()                 # Milliseconds
tasmota.yield()                  # Yield to system
tasmota.memory()                 # Memory stats
persist.my_value = 123           # Auto-persist
introspect.members(obj)          # Object members
```

#### 2. Communication Protocols (12 modules)

**MQTT**
```berry
mqtt.subscribe(topic, callback)
mqtt.publish(topic, payload, retain)
mqtt.connected()
```

**HTTP/Web**
```berry
wc = webclient()
wc.begin(url).GET()
webserver.on(path, handler, method)
```

**Network**
- tcpclient/tcpclientasync, tcpserver, udp, mdns, wsserver

**IoT Protocols**
- zigbee (ZCL, device management)
- matter (Matter protocol)
- hue_bridge (Philips Hue emulation)

#### 3. Hardware Interfaces (10 modules)

**GPIO**
```berry
gpio.digital_write(pin, value)
gpio.pin_mode(pin, gpio.OUTPUT)
gpio.dac_voltage(pin, mV)  # ESP32 DAC
```

**I2C (Wire)**
```berry
wire.scan()
wire.write_bytes(addr, reg, val, size)
wire.read_bytes(addr, reg, size)
```

**Serial/OneWire**
```berry
ser = serial(rx, tx, baud, serial.SERIAL_8N1)
ow = onewire(gpio_pin)
```

**Display/Visual**
- display, leds/leds_ntv, light/light_state
- lv_tasmota (LVGL graphics)

#### 4. Cryptography Suite

**Hashing**: SHA256, MD5, HMAC-SHA256
**Symmetric**: AES (GCM, CCM, CTR, CBC), ChaCha20-Poly1305
**Asymmetric**: EC_P256, EC_C25519, ED25519, RSA
**Key Derivation**: PBKDF2, HKDF, SPAKE2+

```berry
sha = crypto.SHA256()
sha.update(data).finish()
aes = crypto.AES_GCM(key32, iv12)
encrypted = aes.encrypt(plain)
```

#### 5. Advanced Features (11 modules)

**Audio**: i2s_audio, audiooutput, audioinput, audio_opus
**ML**: TFL (TensorFlow Lite Micro)
**Data**: unishox (compression), img, crc32, uuid
**System**: flash, ULP (ultra-low-power), partition_core, path

#### 6. File System

```berry
# File operations
f = open("file.txt", "r")  # r, w, a modes
content = f.read()
line = f.readline()
data = f.readbytes()
f.write("text")
f.seek(pos)
f.close()

# Path operations
import path
path.exists("file")
path.listdir("/")
path.mkdir("dir")
path.remove("file")
```

#### 7. Standard Libraries

**string**
```berry
string.format("Value: %d", 42)
string.split("a,b,c", ",")
string.find("hello", "lo")  # Returns 3
f"Value: {x:.2f}"  # f-string formatting
```

**math**
```berry
math.pi, math.inf, math.nan
math.sqrt(16)
math.sin(math.rad(90))
math.rand()  # After math.srand(seed)
```

**json**
```berry
data = json.load('{"key": "value"}')
json_str = json.dump(data, "format")
```

**os**
```berry
os.getcwd()
os.chdir("/path")
os.listdir()
os.path.exists("/file")
os.system("command")
```

**re** (Regular Expressions)
```berry
matches = re.search("pattern", "text")
all = re.searchall("pattern", "text")
parts = re.split("/", "a/b/c")
```

### Command Line Options
```
berry [options] script.be
  -s    Enable strict mode
  -g    Force named globals (for solidification)
  -i    Interactive mode after script
  -l    Parse all variables as local
  -e    Execute string
  -m    Module search path
  -c    Compile to bytecode
  -o    Output bytecode file
  -v    Version info
  -h    Help
```

### Tasmota Integration

#### Driver Development Pattern
```berry
class CustomDriver : Driver
  def init()
    tasmota.add_driver(self)
  end
  
  # Callbacks (optional)
  def every_second() end
  def every_100ms() end
  def every_50ms() end
  
  def web_sensor()
    tasmota.web_send("{s}Sensor{m}Value{e}")
  end
  
  def json_append()
    tasmota.response_append(',"Sensor":{"Value":123}')
  end
  
  def web_add_main_button()
    webserver.content_send("<button>Action</button>")
  end
  
  def save_before_restart() end
end
```

#### Rule System
```berry
# Event patterns
tasmota.add_rule("Power1#state", def(value, trigger, msg)
  print("Power:", value)
end)

# Common triggers
"System#Boot"
"Wifi#Connected"
"Mqtt#Connected"
"Time#Minute"
"ANALOG#A0>500"
"Switch1#State=1"
```

#### Fast Loop (200Hz)
```berry
tasmota.add_fast_loop(def()
  # High-frequency processing
end)
```

#### Custom Commands
```berry
def cmd_handler(cmd, idx, payload, payload_json)
  tasmota.resp_cmnd_done()
end
tasmota.add_cmd("MyCmd", cmd_handler)
```

#### I2C Device Pattern
```berry
class I2CDevice : I2C_Driver
  static ADDR = 0x48
  
  def init()
    super(self, I2C_Driver).init("DeviceName", self.ADDR)
  end
  
  def read_sensor()
    if self.wire
      return self.wire.read_bytes(self.addr, 0x00, 2)
    end
  end
end
```

#### Timer Management
```berry
tasmota.set_timer(delay_ms, function)  # One-shot
tasmota.add_cron("*/5 * * * * *", func, "id")  # Cron
```

#### Web Handlers
```berry
webserver.on("/api", def()
  if webserver.has_arg("param")
    var value = webserver.arg("param")
    webserver.content_response(json.dump({"result": value}))
  end
end, webserver.HTTP_GET)
```

### Performance & Optimization

#### Memory Management
- Pre-allocate: `bytes(1024)`, `list.resize(100)`
- Clear references: `obj = nil` for GC
- Monitor: `tasmota.memory()`, `tasmota.gc()`
- Use bytes for binary data, not strings

#### Best Practices
1. Cache computed values in loops
2. Use local variables for speed
3. Minimize allocations in hot paths
4. Use `try/except` for cleanup
5. Prefer driver events over polling
6. Limit fast_loop usage (200Hz drain)
7. Check nil returns from Tasmota functions
8. Use persist for reboot-safe settings

#### Security
- Validate all external inputs
- Use HTTPS for sensitive data
- Implement rate limiting
- Sanitize web parameters
- Use crypto.random() for security

### Language Limits
- Integer: -2,147,483,648 to 2,147,483,647
- Stack depth: ~1000 calls
- Local variables: 255 per function
- Function parameters: 255
- No multiple inheritance
- No private/protected members
- No async/await
- No list comprehensions

### Critical Berry Syntax Constraints

#### F-String Limitations
```berry
# ❌ Wrong - Ternary operators in f-strings cause syntax errors
print(f"Status: {enabled ? 'on' : 'off'}")
data['text'] = f"Value: {state ? 'ON' : 'OFF'}"

# ✅ Correct - Extract ternary first
var status = enabled ? "on" : "off"
print(f"Status: {status}")

# ✅ Alternative - Use format() function
print(format("Status: %s", enabled ? 'on' : 'off'))
```

#### Conditional Expressions
```berry
# ❌ Wrong - Python-style not supported
var text = "ON" if state else "OFF"
var emoji = "🟢" if active else "⚫"
var battery_mv = (payload[5] << 8) | payload[6] if size(payload) > 6 else (payload[5] << 8)

# ✅ Correct - Use ternary operator
var text = state ? "ON" : "OFF"
var emoji = active ? "🟢" : "⚫"
var battery_mv = size(payload) > 6 ? ((payload[5] << 8) | payload[6]) : (payload[5] << 8)

# ✅ Correct - Traditional if/else
if state
    var text = "ON"
else
    var text = "OFF"
end
```

#### Hex String Conversion
```berry
# ✅ Correct - bytes() constructor
var byte_obj = bytes("010F0A")

# ✅ Convert to uint8 array if needed
var uint8_array = []
for i: 0..size(byte_obj)-1
    uint8_array.push(byte_obj[i])
end

# ❌ Wrong - tobytes() method doesn't exist
var uint8_array = bytes("010F0A").tobytes()
```

#### Error Prevention Patterns
1. **Extract complex expressions** from f-strings
2. **Validate nil returns** from string.find() and similar
3. **Test incrementally** when building complex expressions
4. **Use format()** as alternative to f-strings for dynamic content

### Platform Compatibility
- **Berry VM**: v0x04 bytecode
- **Tasmota**: 12.x+ required
- **ESP32**: All variants (ESP32, S2, S3, C3)
- **ESP-IDF**: v4.x/v5.x
- **LVGL**: v8.x
- **Features**: Flash write, ULP, Camera, DAC (ESP32 only)

### Module Dependencies
```
tasmota (root)
├── introspect
├── string
├── json
├── persist (optional)
├── webclient (optional)
├── gpio (optional)
└── mqtt (optional)
```

### Version History
- v0.99.3: Alpha - Initial implementation analysis
- v1.0.0: Beta - Complete language & API coverage
  - Added command-line options
  - Added file I/O operations
  - Added standard library details
  - Merged Tasmota integration patterns
  - Added walrus operator and missing operators
  - Complete 91+ libraries documented
- v1.2.0: Beta - Enhanced syntax constraints
  - Added Python-style conditional expression examples
  - Added real-world error patterns from driver development
  - Enhanced conditional expression documentation

### Intelligence Metadata
- **Version**: 1.2.0 (Beta)
- **Coverage**: Complete Berry language + Tasmota implementation + syntax constraints
- **Sources**: 91 C/C++ files, 40 Berry scripts, language specs, TEMP-CONSTRAINTS.md
- **Modules**: 36+ functional domains
- **APIs**: 500+ functions documented
- **Updated**: 2025-08-19