# Berry Repository Structure Map

## Overview
Berry is an ultra-lightweight dynamically typed embedded scripting language designed for lower-performance embedded devices. The interpreter core is less than 40KiB and can run on less than 4KiB heap.

## Directory Structure

### `/src/` - Core Source Code (152 files)
**Main Components:**
- **Virtual Machine**: `be_vm.c` (1419 lines) - Register-based VM execution
- **Parser**: `be_parser.c` (1841 lines) - One-pass compiler and syntax analysis  
- **Lexer**: `be_lexer.c` (914 lines) - Tokenization and lexical analysis
- **API**: `be_api.c` (1179 lines) - External C API interface
- **Code Generation**: `be_code.c` (983 lines) - Bytecode generation
- **Garbage Collector**: `be_gc.c` (613 lines) - Mark-sweep garbage collection

**Data Types & Objects:**
- **Strings**: `be_string.c` (326 lines), `be_strlib.c` (1137 lines)
- **Lists**: `be_list.c` (207 lines), `be_listlib.c` (556 lines)  
- **Maps**: `be_map.c` (354 lines), `be_maplib.c` (265 lines)
- **Classes**: `be_class.c` (374 lines)
- **Functions**: `be_func.c` (183 lines)
- **Bytes**: `be_byteslib.c` (1992 lines) - Binary data handling

**Built-in Libraries:**
- **JSON**: `be_jsonlib.c` (645 lines) - JSON parsing/generation
- **Math**: `be_mathlib.c` (438 lines) - Mathematical functions
- **OS**: `be_oslib.c` (271 lines) - Operating system interface
- **File**: `be_filelib.c` (265 lines) - File I/O operations
- **Debug**: `be_debug.c` (418 lines), `be_debuglib.c` (289 lines)
- **Introspection**: `be_introspectlib.c` (298 lines)
- **Time**: `be_timelib.c` (72 lines)

**Memory & Execution:**
- **Memory Management**: `be_mem.c` (377 lines)
- **Execution**: `be_exec.c` (531 lines)
- **Bytecode**: `be_bytecode.c` (634 lines)
- **Variables**: `be_var.c` (201 lines)
- **Modules**: `be_module.c` (509 lines)

**Headers:**
- **Main Header**: `berry.h` (2395 lines) - Primary API definitions
- **Constants**: `be_constobj.h` (505 lines) - Constant object definitions

### `/tests/` - Unit Tests (54 files)
**Core Language Tests:**
- `assignment.be`, `bool.be`, `class.be`, `closure.be`, `function.be`
- `for.be`, `vararg.be`, `cond_expr.be`, `exceptions.be`

**Data Type Tests:**
- `list.be`, `map.be`, `range.be`, `string.be`, `int.be`, `bytes.be`

**Library Tests:**
- `json.be` (9168 lines) - **Comprehensive JSON security tests**
- `math.be`, `os.be`, `debug.be`, `introspect.be`

**Parser & Compiler Tests:**
- `parser.be`, `lexer.be`, `compiler.be`, `suffix.be`

**Advanced Feature Tests:**
- `virtual_methods.be`, `super_auto.be`, `class_static.be`
- `division_by_zero.be`, `reference.be`, `compound.be`

### `/examples/` - Example Programs (16 files)
- `fib_rec.be` - Fibonacci recursion
- `qsort.be` - Quick sort implementation
- `bintree.be` - Binary tree operations
- `json.be` - JSON usage examples
- `repl.be` - REPL implementation

### `/default/` - Default Configuration (17 files)
- `berry_conf.h` - Configuration settings
- `be_modtab.c` - Module table definitions
- `be_port.c` - Platform-specific code
- `berry.c` - Main executable entry point

### `/generate/` - Generated Files (31 files)
- `be_const_strtab.h` - String table constants
- `be_fixed_*.h` - Fixed/compiled module definitions
- Auto-generated constant definitions

### `/tools/` - Development Tools
**Code Generation:**
- `/coc/` - Compile-on-command tools (13 files)
- Python scripts for code generation and optimization

**Editor Support:**
- `/plugins/vscode/` - Visual Studio Code plugin
- `/plugins/Notepad++/` - Notepad++ syntax highlighting

**Grammar:**
- `berry.ebnf` - EBNF grammar definition
- `berry.bytecode` - Bytecode format specification

## Key Architecture Components

### 1. **Virtual Machine** (`be_vm.c`)
- Register-based VM (not stack-based)
- Optimized for low memory usage
- Handles instruction execution and control flow

### 2. **Parser & Lexer** (`be_parser.c`, `be_lexer.c`)
- One-pass compilation
- Generates bytecode directly
- Error handling and recovery

### 3. **Memory Management** (`be_mem.c`, `be_gc.c`)
- Custom memory allocator
- Mark-sweep garbage collector
- Low memory footprint optimization

### 4. **Type System** 
- **Value Types**: int, real, boolean, string (not class objects)
- **Object Types**: list, map, range, class instances
- Optimized for performance vs. pure OOP

### 5. **Security Features** (Recently Added)
- **JSON Security**: Comprehensive buffer overflow protection
- Unicode handling with proper size calculation
- Input validation and sanitization

## Recent Security Work

### JSON Parser Security (`be_jsonlib.c`)
- **Fixed**: Critical buffer overflow in Unicode handling
- **Added**: Comprehensive security tests (10 test functions)
- **Implemented**: Safe string length calculation
- **Protected**: Against memory exhaustion attacks

## Build System
- **Makefile** - Primary build system
- **CMakeLists.txt** - CMake support
- **library.json** - PlatformIO library definition

## Testing Infrastructure
- **51 unit tests** covering all major features
- **Automated test runner** via `make test`
- **Security regression tests** for vulnerability prevention
- **Cross-platform compatibility tests**

## File Statistics
- **Total Source Files**: ~200 files
- **Core C Code**: ~24,000 lines
- **Test Code**: ~15,000 lines  
- **Documentation**: Comprehensive README and examples
- **Binary Size**: <40KiB interpreter core
- **Memory Usage**: <4KiB heap minimum

This repository represents a complete, production-ready embedded scripting language with comprehensive testing, security features, and development tools.
