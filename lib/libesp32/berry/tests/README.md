# Berry Unit Tests

This directory contains comprehensive unit tests for the Berry scripting language interpreter. The test suite covers language features, built-in libraries, security scenarios, and edge cases to ensure robust and reliable operation.

## Test Overview

**Total Tests:** 51 test files  
**Security Tests:** 4 dedicated security test files  
**Coverage:** Core language features, standard library, error handling, and security vulnerabilities

## Running Tests

```bash
# Run all tests
make test

# Run individual test
./berry tests/test_name.be
```

## Test Files

### Core Language Features

- **assignment.be** - Variable assignment operators and compound assignment expressions
- **bool.be** - Boolean type operations, logical operators, and truth value testing
- **call.be** - Function call mechanisms, parameter passing, and return value handling
- **closure.be** - Closure creation, variable capture, and lexical scoping behavior
- **compiler.be** - Compiler functionality, bytecode generation, and compilation edge cases
- **compound.be** - Compound assignment operators (+=, -=, *=, /=, %=, etc.)
- **cond_expr.be** - Conditional (ternary) operator expressions and evaluation precedence
- **function.be** - Function definition, local variables, nested functions, and scope rules
- **global.be** - Global variable access, module-level variable management
- **lexer.be** - Lexical analysis, token recognition, and source code parsing
- **lexergc.be** - Lexer garbage collection behavior and memory management during parsing
- **parser.be** - Parser functionality, syntax tree construction, and error recovery
- **reference.be** - Reference semantics, object identity, and memory reference behavior
- **relop.be** - Relational operators (<, <=, ==, !=, >, >=) and comparison logic
- **suffix.be** - Suffix operators and postfix expression evaluation
- **vararg.be** - Variable argument functions and parameter list handling
- **walrus.be** - Walrus operator (:=) assignment expressions within conditions

### Data Types and Operations

- **bitwise.be** - Bitwise operators (&, |, ^, ~, <<, >>) and bit manipulation functions
- **bytes.be** - Bytes type operations, binary data handling, and buffer management
- **bytes_b64.be** - Base64 encoding/decoding functionality for bytes objects
- **bytes_fixed.be** - Fixed-size bytes operations and memory-constrained buffer handling
- **int.be** - Integer arithmetic, overflow handling, and numeric type conversions
- **int64.be** - 64-bit integer support, large number operations, and precision handling
- **list.be** - List operations, indexing, iteration, and dynamic array functionality
- **map.be** - Map (dictionary) operations, key-value pairs, and hash table behavior
- **range.be** - Range objects, iteration protocols, and sequence generation
- **string.be** - String operations, concatenation, formatting, and text manipulation

### Object-Oriented Programming

- **class.be** - Class definition, instantiation, and basic object-oriented features
- **class_const.be** - Class constants, static values, and compile-time initialization
- **class_static.be** - Static methods, class-level functions, and shared behavior
- **member_indirect.be** - Indirect member access, dynamic property resolution
- **overload.be** - Operator overloading, method dispatch, and polymorphic behavior
- **subobject.be** - Object composition, nested objects, and hierarchical structures
- **super_auto.be** - Automatic super class method resolution and inheritance chains
- **super_leveled.be** - Multi-level inheritance, method resolution order, and super calls
- **virtual_methods.be** - Virtual method dispatch, polymorphism, and dynamic binding
- **virtual_methods2.be** - Advanced virtual method scenarios and edge cases

### Control Flow and Iteration

- **for.be** - For loop constructs, iteration protocols, and loop variable scoping
- **exceptions.be** - Exception handling, try-catch blocks, and error propagation

### Built-in Libraries and Modules

- **debug.be** - Debug module functionality, introspection, and development tools
- **introspect.be** - Introspection capabilities, object inspection, and runtime reflection
- **introspect_ismethod.be** - Method detection and callable object identification
- **math.be** - Mathematical functions, constants, and numeric operations
- **module.be** - Module system, import mechanisms, and namespace management
- **os.be** - Operating system interface, file operations, and system calls
- **re.be** - Regular expression support, pattern matching, and text processing

### JSON Processing and Security

- **json.be** - Basic JSON parsing, serialization, and data interchange
- **json_advanced.be** - **SECURITY CRITICAL** - Advanced JSON parsing with comprehensive security tests including Unicode buffer overflow protection, malformed input handling, and attack vector prevention
- **json_test_stack_size.be** - JSON parser stack size limits and deep nesting protection

### Memory Management and Performance

- **checkspace.be** - Memory space checking, heap management, and resource monitoring
- **division_by_zero.be** - Division by zero handling, numeric error conditions, and exception safety

## Security Test Coverage

The test suite includes dedicated security tests that protect against:

- **Buffer Overflow Attacks** - Unicode string processing vulnerabilities
- **Memory Exhaustion** - Large input handling and resource limits
- **Stack Overflow** - Deep recursion and nested structure protection
- **Input Validation** - Malformed data handling and sanitization
- **Integer Overflow** - Numeric boundary condition testing

## Test Categories

### 🔧 **Core Language** (17 tests)
Basic language constructs, syntax, and fundamental operations

### 📊 **Data Types** (10 tests)  
Type system, operations, and data structure functionality

### 🏗️ **Object-Oriented** (10 tests)
Classes, inheritance, polymorphism, and object model

### 🔄 **Control Flow** (2 tests)
Loops, conditionals, and program flow control

### 📚 **Libraries** (7 tests)
Built-in modules and standard library functionality

### 🔒 **Security** (4 tests)
Vulnerability prevention and attack resistance

### ⚡ **Performance** (1 test)
Memory management and resource optimization

## Test Quality Assurance

- **Comprehensive Coverage** - Tests cover both common usage patterns and edge cases
- **Security Focus** - Dedicated tests for vulnerability prevention and attack resistance  
- **Regression Prevention** - Tests prevent reintroduction of previously fixed bugs
- **Documentation** - Each test file includes synthetic comments explaining test purpose
- **Automated Execution** - Full test suite runs via `make test` command

## Contributing

When adding new tests:

1. Follow existing naming conventions
2. Include descriptive comments explaining test purpose
3. Cover both positive and negative test cases
4. Add security tests for any new parsing or input handling code
5. Update this README with new test descriptions

## Notes

- All tests pass successfully in the current Berry implementation
- Security tests include comprehensive JSON parsing vulnerability prevention
- Test files use `.be` extension (Berry script files)
- Tests are designed to run independently and can be executed individually
