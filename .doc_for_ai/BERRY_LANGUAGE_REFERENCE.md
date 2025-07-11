# Berry Language Reference for Tasmota

Note: this file is supposed to use as a reference manual for Generative AI in a compact form. For Claude AI it costs ~10k tokens.

## Introduction

Berry is an ultra-lightweight, dynamically typed embedded scripting language designed for resource-constrained environments. The language primarily supports procedural programming, with additional support for object-oriented and functional programming paradigms. Berry's key design goal is to run efficiently on embedded devices with very limited memory, making the language highly streamlined while maintaining rich scripting capabilities.

**Tasmota Integration**: Berry is the next generation scripting for Tasmota, embedded by default in all ESP32 based firmwares (NOT supported on ESP8266). It is used for advanced scripting, superseding Rules, and enables building drivers, automations, and UI extensions.

## Basic Information

### Comments

```berry
# This is a line comment
#- This is a
   block comment
-#
```

### Literals

#### Numerical Literals
```berry
40      # Integer literal
0x80    # Hexadecimal literal (integer)
3.14    # Real literal
1.1e-6  # Real literal with scientific notation
```

#### Boolean Literals
```berry
true    # Boolean true
false   # Boolean false
```

#### String Literals
```berry
'this is a string'
"this is a string"
```

String literals can be concatenated without operators:
```berry
s = "a" "b" "c"    # s == "abc"
s = "a"            # Multi-line strings
    "b"
    "c"            # s == "abc"
```

Escape sequences:
- `\a` - Bell
- `\b` - Backspace
- `\f` - Form feed
- `\n` - Newline
- `\r` - Carriage return
- `\t` - Horizontal tab
- `\v` - Vertical tab
- `\\` - Backslash
- `\'` - Single quote
- `\"` - Double quote
- `\?` - Question mark
- `\0` - Null character
- `\ooo` - Character represented by octal number
- `\xhh` - Character represented by hexadecimal number
- `\uXXXX` - Unicode character (UTF-8 encoded)

#### Nil Literal
```berry
nil     # Represents no value
```

### Identifiers

An identifier starts with an underscore or letter, followed by any combination of underscores, letters, or numbers. Berry is case-sensitive.

```berry
a
TestVariable
Test_Var
_init
baseClass
_
```

### Keywords

```
if       elif     else     while     for      def
end      class    break    continue  return   true
false    nil      var      do        import   as
try      except   raise    static
```

## Types and Variables

### Built-in Types

#### Simple Types

- **nil**: Represents no value
- **Integer**: Signed integer (typically 32-bit)
- **Real**: Floating-point number (typically 32-bit)
- **Boolean**: `true` or `false`
- **String**: Sequence of characters
- **Function**: First-class value that can be called
- **Class**: Template for instances
- **Instance**: Object constructed from a class

#### Class Types

- **list**: Ordered collection of elements
- **map**: Key-value pairs collection
- **range**: Integer range
- **bytes**: Byte buffer

### Variables

Variables are dynamically typed in Berry. They can be defined in two ways:

```berry
# Direct assignment (creates variable if it doesn't exist)
a = 1

# Using the var keyword
var a       # Defines a with nil value
var a = 1   # Defines a with value 1
var a, b    # Defines multiple variables
var a = 1, b = 2  # Defines multiple variables with values
```

### Scope and Lifecycle

Variables defined in the outermost block have global scope. Variables defined in inner blocks have local scope.

```berry
var i = 0   # Global scope
do
    var j = 'str'  # Local scope
    print(i, j)    # Both i and j are accessible
end
print(i)    # Only i is accessible here
```

## Expressions

### Operators

#### Arithmetic Operators
- `-` (unary): Negation
- `+`: Addition or string concatenation
- `-`: Subtraction
- `*`: Multiplication
- `/`: Division
- `%`: Modulo (remainder)

#### Relational Operators
- `<`: Less than
- `<=`: Less than or equal to
- `==`: Equal to
- `!=`: Not equal to
- `>=`: Greater than or equal to
- `>`: Greater than

#### Logical Operators
- `&&`: Logical AND (short-circuit)
- `||`: Logical OR (short-circuit)
- `!`: Logical NOT

#### Bitwise Operators
- `~`: Bitwise NOT
- `&`: Bitwise AND
- `|`: Bitwise OR
- `^`: Bitwise XOR
- `<<`: Left shift
- `>>`: Right shift

#### Assignment Operators
- `=`: Simple assignment
- `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`: Compound assignment
- `:=`: Walrus assignment (assigns and returns value)

#### Domain and Subscript Operators
- `.`: Member access
- `[]`: Subscript access

#### Conditional Operator
- `? :`: Ternary conditional

#### Concatenation Operators
- `+`: String or list concatenation
- `..`: String concatenation or range creation

### Operator Precedence (highest to lowest)

1. `()` (grouping)
2. `()` (function call), `[]` (subscript), `.` (member access)
3. `-` (unary), `!`, `~`
4. `*`, `/`, `%`
5. `+`, `-`
6. `<<`, `>>`
7. `&`
8. `^`
9. `|`
10. `..`
11. `<`, `<=`, `>`, `>=`
12. `==`, `!=`
13. `&&`
14. `||`
15. `? :`
16. `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
17. `:=`

## Statements

### Expression Statements

```berry
a = 1       # Assignment statement
print(a)    # Function call statement
```

### Block

A block is a collection of statements. It defines a scope.

```berry
do
    # This is a block
    var a = 1
    print(a)
end
```

### Conditional Statements

```berry
# Simple if
if condition
    # code executed if condition is true
end

# if-else
if condition
    # code executed if condition is true
else
    # code executed if condition is false
end

# if-elif-else
if condition1
    # code executed if condition1 is true
elif condition2
    # code executed if condition1 is false and condition2 is true
else
    # code executed if both condition1 and condition2 are false
end
```

### Iteration Statements

```berry
# while loop
while condition
    # code executed repeatedly while condition is true
end

# for loop (iterating over a container)
for variable: expression
    # code executed for each element in the container
end

# Examples
for i: 0..5
    print(i)    # Prints 0, 1, 2, 3, 4, 5
end

for item: ['a', 'b', 'c']
    print(item)  # Prints a, b, c
end

for key: map.keys()
    print(key, map[key])  # Iterates over map keys
end
```

### Jump Statements

```berry
# break - exits the loop
while true
    if condition
        break
    end
end

# continue - skips to the next iteration
for i: 0..5
    if i == 3
        continue
    end
    print(i)  # Prints 0, 1, 2, 4, 5
end
```

### Import Statement

```berry
import math              # Import math module
import hardware as hw    # Import hardware module as hw
```

### Exception Handling

```berry
# Raising exceptions
raise 'my_error'                     # Raise an exception
raise 'my_error', 'error message'    # Raise with message

# Catching exceptions
try
    # code that might raise an exception
except 'my_error'
    # code executed if 'my_error' is raised
end

# Catching with exception variable
try
    # code that might raise an exception
except 'my_error' as e
    # e contains the exception value
end

# Catching with exception and message variables
try
    # code that might raise an exception
except 'my_error' as e, msg
    # e contains the exception value, msg contains the message
end

# Catching any exception
try
    # code that might raise an exception
except ..
    # code executed for any exception
end

# Catching multiple exception types
try
    # code that might raise an exception
except 'error1', 'error2' as e, msg
    # code executed if either 'error1' or 'error2' is raised
end
```

## Functions

### Function Definition

```berry
# Named function
def add(a, b)
    return a + b
end

# Anonymous function
add = def (a, b)
    return a + b
end

# Lambda expression (compact form)
add = / a, b -> a + b
```

### Function with Variable Arguments

```berry
def print_all(a, b, *args)
    print(a, b)
    for arg: args
        print(arg)
    end
end

print_all(1, 2, 3, 4, 5)  # args will be [3, 4, 5]
```

### Calling Functions with Dynamic Arguments

```berry
def sum(a, b, c)
    return a + b + c
end

call(sum, 1, 2, 3)        # Calls sum(1, 2, 3)
call(sum, 1, [2, 3])      # Calls sum(1, 2, 3)
```

### Closures

```berry
def counter(start)
    var count = start
    return def()
        count += 1
        return count
    end
end

c = counter(0)
print(c())  # 1
print(c())  # 2
```

## Object-Oriented Programming

### Class Declaration

```berry
class Person
    var name, age
    
    def init(name, age)
        self.name = name
        self.age = age
    end
    
    def greet()
        print("Hello, my name is", self.name)
    end
end
```

### Static Members

```berry
class MathUtils
    static var PI = 3.14159
    
    static def square(x)
        return x * x
    end
end

print(MathUtils.PI)           # Access static variable
print(MathUtils.square(5))    # Call static method
```

### Inheritance

```berry
class Student : Person
    var school
    
    def init(name, age, school)
        super(self).init(name, age)  # Call parent constructor
        self.school = school
    end
    
    def greet()
        super(self).greet()          # Call parent method
        print("I study at", self.school)
    end
end
```

### Instantiation and Method Calls

```berry
person = Person("John", 30)
person.greet()                # Hello, my name is John

student = Student("Alice", 20, "University")
student.greet()               # Hello, my name is Alice
                              # I study at University
```

### Operator Overloading

```berry
class Vector
    var x, y
    
    def init(x, y)
        self.x = x
        self.y = y
    end
    
    def +(other)
        return Vector(self.x + other.x, self.y + other.y)
    end
    
    def tostring()
        return "Vector(" + str(self.x) + ", " + str(self.y) + ")"
    end
end

v1 = Vector(1, 2)
v2 = Vector(3, 4)
v3 = v1 + v2
print(v3)  # Vector(4, 6)
```

## Built-in Classes

### List

```berry
# Creating lists
l1 = []                # Empty list
l2 = [1, 2, 3]         # List with elements
l3 = list()            # Empty list using constructor
l4 = list(1, 2, 3)     # List with elements using constructor

# Accessing elements
l2[0]                  # First element (1)
l2[-1]                 # Last element (3)

# Range-based access (slicing)
l2[1..3]               # Elements from index 1 to 3 inclusive [2, 3]
l2[1..]                # Elements from index 1 to end [2, 3]
l2[1..-1]              # Elements from index 1 to last element [2, 3]
l2[0..-2]              # All elements except the last one [1, 2]
l2[-2..-1]             # Last two elements [2, 3]

# Modifying lists
l2.push(4)             # Add element to end
l2.pop()               # Remove and return last element
l2.pop(1)              # Remove and return element at index 1
l2.insert(1, 5)        # Insert 5 at index 1
l2.remove(1)           # Remove element at index 1
l2.resize(5)           # Resize list to 5 elements (fills with nil)
l2.clear()             # Remove all elements

# Negative indices for modification
l2[-1] = 10            # Set last element to 10
l2[-2] += 5            # Add 5 to second-to-last element

# Other operations
l2.size()              # Number of elements
l2.concat()            # Join elements as string (no separator)
l2.concat(", ")        # Join elements with separator
l2.reverse()           # Reverse the list
l2.copy()              # Create a shallow copy
l2 + [4, 5]            # Concatenate lists (new list)
l2 .. 4                # Append element (modifies list)

# Finding elements
l2.find(3)             # Returns index of first occurrence or nil if not found

# Iterating over indices
for i: l2.keys()       # Iterate over list indices
    print(i, l2[i])
end

# Comparing lists
[1, 2] == [1, 2]       # true
[1, 2] != [1, 3]       # true
```

### Map

```berry
# Creating maps
m1 = {}                # Empty map
m2 = {"key": "value"}  # Map with key-value pair
m3 = map()             # Empty map using constructor

# Accessing elements
m2["key"]              # Get value by key
m2.find("key")         # Get value by key (returns nil if not found)
m2.find("key", "default")  # Get value with default if not found

# Modifying maps
m2["new_key"] = "new_value"  # Add or update key-value pair
m2.insert("key", "value")    # Insert key-value pair (returns true if inserted, false if key exists)
m2.remove("key")             # Remove key-value pair

# Other operations
m2.size()              # Number of key-value pairs
m2.contains("key")     # Check if key exists
for k: m2.keys()       # Iterate over keys
    print(k, m2[k])
end
```

### Range

```berry
# Creating ranges
r1 = 0..5              # Range from 0 to 5 (inclusive)
r2 = range(0, 5)       # Same using constructor
r3 = 10..              # Range from 10 to MAXINT

# Accessing properties
r1.lower()             # Lower bound (0)
r1.upper()             # Upper bound (5)
r1.incr()              # Increment (default 1)

# Modifying ranges
r1.setrange(1, 6)      # Change range bounds
r1.setrange(1, 10, 2)  # Change range bounds and increment

# Using in for loops
for i: 0..5
    print(i)           # Prints 0, 1, 2, 3, 4, 5
end
```

### String Operations

```berry
# String indexing and slicing
s = "hello"
s[0]                   # "h"
s[1]                   # "e"
s[-1]                  # "o" (last character)
s[1..3]                # "ell" (characters from index 1 to 3)
s[1..]                 # "ello" (characters from index 1 to end)
s[1..-1]               # "ello" (characters from index 1 to last)
s[0..-2]               # "hell" (all characters except the last one)
```

### Bytes

```berry
# Creating bytes objects
b1 = bytes()           # Empty bytes
b2 = bytes("1122AA")   # From hex string
b3 = bytes(10)         # Pre-allocated 10 bytes
b4 = bytes(-8)         # Fixed size 8 bytes

# Accessing bytes
b2[0]                  # First byte (0x11)
b2[1..2]               # Bytes from index 1 to 2

# Modifying bytes
b2[0] = 0xFF           # Set byte at index 0
b2.resize(10)          # Resize buffer
b2.clear()             # Clear all bytes

# Reading/writing structured data
b2.get(0, 2)           # Read 2 bytes as unsigned int (little endian)
b2.get(0, -2)          # Read 2 bytes as unsigned int (big endian)
b2.geti(0, 2)          # Read 2 bytes as signed int
b2.set(0, 0x1234, 2)   # Write 2-byte value
b2.add(0x1234, 2)      # Append 2-byte value

# Conversion
b2.tohex()             # Convert to hex string
b2.asstring()          # Convert to raw string
b2.tob64()             # Convert to base64 string
b2.fromhex("AABBCC")   # Load from hex string
b2.fromstring("Hello") # Load from raw string
b2.fromb64("SGVsbG8=") # Load from base64 string
```

### File

```berry
# Opening files
f = open("test.txt", "w")  # Open for writing
f = open("test.txt", "r")  # Open for reading

# Writing to files
f.write("Hello, world!")   # Write string
f.write(bytes("AABBCC"))   # Write bytes

# Reading from files
content = f.read()         # Read entire file
line = f.readline()        # Read one line
raw_data = f.readbytes()   # Read as bytes

# File positioning
f.seek(10)                 # Move to position 10
pos = f.tell()             # Get current position
size = f.size()            # Get file size

# Closing files
f.flush()                  # Flush buffers
f.close()                  # Close file
```

## Standard Libraries

### String Module

```berry
import string

# String operations
string.count("hello", "l")         # Count occurrences (2)
string.find("hello", "lo")         # Find substring (3), returns -1 if not found
string.split("a,b,c", ",")         # Split by separator (["a", "b", "c"])
string.split("hello", 2)           # Split at position (["he", "llo"])

# Character operations
string.byte("A")                   # Get byte value (65)
string.char(65)                    # Get character from byte ('A')

# Case conversion
string.toupper("hello")            # Convert to uppercase ("HELLO")
string.tolower("HELLO")            # Convert to lowercase ("hello")

# String transformation
string.tr("hello", "el", "ip")     # Replace characters ("hippo")
string.replace("hello", "ll", "xx") # Replace substring ("hexxo")
string.escape("hello\n")           # Escape for C strings

# Checking
string.startswith("hello", "he")   # Check prefix (true)
string.startswith("hello", "HE", true) # Case-insensitive check (true)
string.endswith("hello", "lo")     # Check suffix (true)
string.endswith("hello", "LO", true) # Case-insensitive check (true)

# Formatting
string.format("Value: %d", 42)     # Format string
format("Value: %.2f", 3.14159)     # Format (global function)
f"Value: {x}"                      # f-string format
f"Value: {x:.2f}"                  # f-string with format specifier
f"{x=}"                           # f-string debug format
```

### Math Module

```berry
import math

# Constants
math.pi                            # Pi (3.14159...)
math.inf                           # Infinity
math.nan                           # Not a Number
math.imin                          # Smallest possible integer
math.imax                          # Largest possible integer

# Basic functions
math.abs(-5)                       # Absolute value (5)
math.floor(3.7)                    # Round down (3)
math.ceil(3.2)                     # Round up (4)
math.round(3.5)                    # Round to nearest (4)
math.min(1, 2, 3)                  # Minimum value (1)
math.max(1, 2, 3)                  # Maximum value (3)

# Exponential and logarithmic
math.sqrt(16)                      # Square root (4)
math.pow(2, 3)                     # Power (8)
math.exp(1)                        # e^x (2.71828...)
math.log(2.71828)                  # Natural logarithm (1)
math.log10(100)                    # Base-10 logarithm (2)

# Trigonometric
math.sin(math.pi/2)                # Sine (1)
math.cos(0)                        # Cosine (1)
math.tan(math.pi/4)                # Tangent (1)
math.asin(1)                       # Arc sine (pi/2)
math.acos(1)                       # Arc cosine (0)
math.atan(1)                       # Arc tangent (pi/4)
math.atan2(1, 1)                   # Arc tangent of y/x (pi/4)

# Angle conversion
math.deg(math.pi)                  # Radians to degrees (180)
math.rad(180)                      # Degrees to radians (pi)

# Random numbers
math.srand(42)                     # Seed random generator
math.rand()                        # Random integer

# Special checks
math.isinf(math.inf)               # Check if value is infinity (true)
math.isnan(math.nan)               # Check if value is NaN (true)
```

### JSON Module

```berry
import json

# Parsing JSON
data = json.load('{"name": "John", "age": 30}')
print(data.name)                   # John

# Error handling with json.load
data = json.load('{"invalid": }')  # Returns nil on parsing error
if data == nil
    print("Invalid JSON")
end

# Generating JSON
person = {
    "name": "Alice",
    "age": 25,
    "hobbies": ["reading", "swimming"]
}
json_str = json.dump(person)       # Compact JSON
json_formatted = json.dump(person, "format")  # Formatted JSON
```

### OS Module

```berry
import os

# Directory operations
os.getcwd()                        # Get current directory
os.chdir("/path/to/dir")           # Change directory
os.mkdir("/path/to/new/dir")       # Create directory
os.remove("/path/to/file")         # Delete file or directory
os.listdir()                       # List current directory
os.listdir("/path")                # List specific directory

# Path operations
os.path.isdir("/path")             # Check if path is directory
os.path.isfile("/path/file.txt")   # Check if path is file
os.path.exists("/path")            # Check if path exists
os.path.split("/path/file.txt")    # Split into ["/path", "file.txt"]
os.path.splitext("file.txt")       # Split into ["file", ".txt"]
os.path.join("path", "file.txt")   # Join into "path/file.txt"

# System operations
os.system("command")               # Execute system command
os.exit()                          # Exit interpreter
```

### Global Module

```berry
import global

# Accessing globals
global_vars = global()             # List of all global variables
global.contains("var_name")        # Check if global exists
value = global.var_name            # Get global value
global.var_name = 42               # Set global value
value = global.("dynamic_name")    # Dynamic access by name
```

### Introspect Module

```berry
import introspect

# Inspecting objects
members = introspect.members(obj)  # List of object members
value = introspect.get(obj, "attr") # Get attribute value
introspect.set(obj, "attr", value) # Set attribute value
name = introspect.name(obj)        # Get object name
is_method = introspect.ismethod(fn) # Check if function is method

# Module operations
mod = introspect.module("math")    # Import module dynamically

# Pointer operations (advanced)
ptr = introspect.toptr(addr)       # Convert int to pointer
addr = introspect.fromptr(ptr)     # Convert pointer to int
```

## Error Handling

### Standard Exceptions

- `assert_failed`: Assertion failed
- `index_error`: Index out of bounds
- `io_error`: IO malfunction
- `key_error`: Key error
- `runtime_error`: VM runtime exception
- `stop_iteration`: End of iterator
- `syntax_error`: Syntax error
- `unrealized_error`: Unrealized function
- `type_error`: Type error

### Raising Exceptions

```berry
raise "my_error"                   # Raise exception
raise "my_error", "message"        # Raise with message
```

### Catching Exceptions

```berry
try
    # Code that might raise an exception
except "my_error"
    # Handle specific exception
end

try
    # Code that might raise an exception
except "error1", "error2"
    # Handle multiple exceptions
end

try
    # Code that might raise an exception
except "my_error" as e
    # e contains the exception value
end

try
    # Code that might raise an exception
except "my_error" as e, msg
    # e contains exception, msg contains message
end

try
    # Code that might raise an exception
except ..
    # Catch all exceptions
end
```

### Error Handling Patterns

Many functions in Berry return `nil` to indicate errors rather than raising exceptions. This is common in functions that parse data or perform operations that might fail:

```berry
# JSON parsing
data = json.load('{"invalid": }')  # Returns nil on parsing error
if data == nil
    print("Invalid JSON")
end

# Map access
value = map.find("key")            # Returns nil if key doesn't exist
if value == nil
    print("Key not found")
end

# String operations
index = string.find("hello", "z")  # Returns -1 if substring not found
if index == -1
    print("Substring not found")
end
```

### Assertions

```berry
assert(condition)                  # Raises assert_failed if condition is false
assert(condition, "message")       # Raises with custom message
```

## Best Practices

### Variable Naming

- Use descriptive names for variables and functions
- Use camelCase or snake_case consistently
- Prefix private members with underscore (convention only)

### Code Organization

- Group related functions and classes
- Use modules for logical separation
- Keep functions small and focused

### Memory Management

- Be mindful of memory usage on embedded systems
- Release resources when no longer needed
- Use fixed-size buffers when appropriate

### Error Handling

- Use exceptions for exceptional conditions
- Check return values for expected errors
- Provide meaningful error messages

### Performance

- Avoid creating unnecessary objects
- Reuse buffers when processing large data
- Use native functions for performance-critical code

## Tasmota-Specific Features

### Tasmota-Specific Modules

Beyond standard Berry modules, Tasmota provides additional modules:

#### `tasmota` - Core integration module (automatically imported)
#### `light` - Light control (automatically imported)  
#### `mqtt` - MQTT operations (`import mqtt`)
#### `webserver` - Web server extensions (`import webserver`)
#### `gpio` - GPIO control (`import gpio`)
#### `persist` - Data persistence (`import persist`)
#### `path` - File system operations (`import path`)
#### `energy` - Energy monitoring (automatically imported)
#### `display` - Display driver integration (`import display`)
#### `crypto` - Cryptographic functions (`import crypto`)
#### `re` - Regular expressions (`import re`)
#### `mdns` - mDNS/Bonjour support (`import mdns`)
#### `ULP` - Ultra Low Power coprocessor (`import ULP`)
#### `uuid` - UUID generation (`import uuid`)
#### `crc` - CRC calculations (`import crc`)

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
# Use wire1 or wire2 for I²C buses
wire1.scan()                    # Scan for devices
wire1.detect(addr)              # Check if device present
wire1.read(addr, reg, size)     # Read from device
wire1.write(addr, reg, val, size) # Write to device
wire1.read_bytes(addr, reg, size)  # Read as bytes
wire1.write_bytes(addr, reg, bytes) # Write bytes

# Find device on any bus
wire = tasmota.wire_scan(addr, i2c_index)
```

### MQTT Integration

```berry
import mqtt

# MQTT operations
mqtt.publish(topic, payload, retain)
mqtt.subscribe(topic, function)  # Subscribe with callback
mqtt.unsubscribe(topic)
mqtt.connected()                 # Check connection status

# Callback function signature
def mqtt_callback(topic, idx, payload_s, payload_b)
  # topic: full topic, payload_s: string, payload_b: bytes
  return true  # Return true if handled
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

# AES encryption
aes = crypto.AES_GCM(key_32_bytes, iv_12_bytes)
encrypted = aes.encrypt(plaintext)
tag = aes.tag()

# Hashing
crypto.SHA256().update(data).finish()  # SHA256 hash
crypto.MD5().update(data).finish()     # MD5 hash

# HMAC
crypto.HMAC_SHA256(key).update(data).finish()
```

### File System Operations

```berry
import path

path.exists("filename")         # Check file exists
path.listdir("/")              # List directory
path.remove("filename")        # Delete file
path.mkdir("dirname")          # Create directory
path.last_modified("file")     # File timestamp
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

### Tasmota Extensions to Standard Modules

#### `bytes` class extensions
```berry
b = bytes("1122AA")               # From hex string
b = bytes(-8)                     # Fixed size buffer
b.tohex()                         # To hex string  
b.tob64()                         # To base64
b.fromhex("AABBCC")              # Load from hex
b.fromb64("SGVsbG8=")            # Load from base64
b.asstring()                      # To raw string
```

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


