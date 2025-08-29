# Berry Language Reference for Tasmota

Note: this file is supposed to use as a reference manual for Generative AI in a compact form. For Claude AI it costs ~6k tokens.

## Command Line Options

When running Berry from the command line in Tasmota, several options are available:

```
berry [options]
```

Available options:
- `-s`: Enable strict mode for the Berry compiler
- `-g`: Force named globals in VM (required for solidification)
- `-i`: Enter interactive mode after executing script
- `-l`: Parse all variables in script as local
- `-e`: Load script source string and execute
- `-m <path>`: Custom module search path(s)
- `-c <file>`: Compile script file to bytecode
- `-o <file>`: Save bytecode to file
- `-v`: Show version information
- `-h`: Show help information

Common usage in Tasmota:
```
berry -s -g
```
This runs Berry with strict mode enabled and named globals, which is the recommended configuration for code that will be solidified.

## Introduction

Berry is an ultra-lightweight, dynamically typed embedded scripting language designed for resource-constrained environments. The language primarily supports procedural programming, with additional support for object-oriented and functional programming paradigms. Berry's key design goal is to run efficiently on embedded devices with very limited memory, making the language highly streamlined while maintaining rich scripting capabilities.

**Tasmota Integration**: Berry is integrated into Tasmota firmware. For Tasmota-specific features, see the companion document `BERRY_TASMOTA.md`.

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

**String Multiplication**: The `*` operator supports string multiplication when the left operand is a string and the right operand is an integer or boolean:

```berry
"aze" * 3        # "azeazeaze" - repeat string 3 times
"aze" * 0        # "" - empty string
"aze" * true     # "aze" - string if true
"aze" * false    # "" - empty string if false

# Common use cases:
"  " * indent    # Create indentation spaces
f"{n} time{'s' * bool(n >= 2)}"  # Conditional pluralization
```

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
