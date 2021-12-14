<p align="center">
  <h1 align="center">
    <img src="https://gitee.com/mirrors/Berry/raw/master/berry-logo.png" alt="Berry" width=272 height=128>
  </h1>
  <p align="center">The Berry Script Language.</p>
</p>

## Introduction

Berry is a ultra-lightweight dynamically typed embedded scripting language. It is designed for lower-performance embedded devices. The Berry interpreter-core's code size is less than 40KiB and can run on less than 4KiB heap (on ARM Cortex M4 CPU, Thumb ISA and ARMCC compiler).

The interpreter of Berry include a one-pass compiler and register-based VM, all the code is written in ANSI C99. In Berry not every type is a class object. Some simple value types, such as int, real, boolean and string are not class object, but list, map and range are class object. This is a consideration about performance. 
Register-based VM is the same meaning as above.

Berry has the following advantages:

* Lightweight: A well-optimized interpreter with very little resources. Ideal for use in microprocessors.
* Fast: optimized one-pass bytecode compiler and register-based virtual machine.
* Powerful: supports imperative programming, object-oriented programming, functional programming.
* Flexible: Berry is a dynamic type script, and it's intended for embedding in applications. It can provide good dynamic scalability for the host system.
* Simple: simple and natural syntax, support garbage collection, and easy to use FFI (foreign function interface).
* RAM saving: With compile-time object construction, most of the constant objects are stored in read-only code data segments, so the RAM usage of the interpreter is very low when it starts.

## Documents

LaTeX documents repository: [https://github.com/Skiars/berry_doc](https://github.com/Skiars/berry_doc)

Short Manual: [berry_short_manual.pdf](https://github.com/Skiars/berry_doc/releases/download/latest/berry_short_manual.pdf).

Reference Manual: [berry_rm_en_us.pdf](https://github.com/Skiars/berry_doc/releases/download/latest/berry_rm_en_us.pdf), [berry_rm_zh_cn.pdf](https://github.com/Skiars/berry_doc/releases/download/latest/berry_rm_zh_cn.pdf).

Berry's EBNF grammar definition: [tools/grammar/berry.ebnf](./tools/grammar/berry.ebnf)

## Features

* Base Type
  * Nil: `nil`
  * Boolean: `true` and `false`
  * Numerical: Integer (`int`) and Real (`real`)
  * String: Single quotation-mark string and double quotation-mark string
  * Class: Instance template, read only
  * Instance: Object constructed by class
  * Module: Read-write key-value pair table
  * List: Ordered container, like `[1, 2, 3]`
  * Map: Hash Map container, like `{ 'a': 1, 2: 3, 'map': {} }`
  * Range: include a lower and a upper integer value, like `0..5`
* Operator and Expression
  * Assign operator: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
  * Relational operator: `<`, `<=`, `==`, `!=`, `>`, `>=`
  * Logic operator: `&&`, `||`, `!`
  * Arithmetic operator: `+`, `-`, `*`, `/`, `%`
  * Bitwise operator: `&`, `|`, `~`, `^`, `<<`, `>>`
  * Field operator: `.`
  * Subscript operator: `[]`
  * Connect string operator: `+`
  * Conditional operator: `? :`
  * Brackets: `()`
* Control Structure
  * Conditional statement: `if-else`
  * Iteration statement: `while` and `for`
  * Jump statement: `break` and `continue`
* Function
  * Local variable and block scope
  * Return statement
  * Nested functions definition
  * Closure based on Upvalue
  * Anonymous function
  * Lambda expression
* Class
  * Inheritance (only public single inheritance)
  * Method and Operator Overload
  * Constructor method
  * Destructive method
* Module Management
  * Built-in module that takes almost no RAM
  * Extension module support: script module, bytecode file module and shared library (like *.so, *.dll) module
* GC (Garbage collection)
  * Mark-Sweep GC
* Exceptional Handling
  * Throw any exception value using the `raise` statement
  * Multiple catch mode
* Bytecode file support
  * Export function to bytecode file
  * Load the bytecode file and execute

## Build and Run

1. Install the readline library (Windows does not need):

   ``` bash
   sudo apt install libreadline-dev # Ubuntu
   brew install readline            # MacOS
   ```

2. Build (The default compiler is GCC):

   ```
   make
   ```

3. Run:

   ``` bash
   ./berry # Bash or PowerShell
   berry   # Windows CMD
   ```

4. Install (Only Unix-like):

   ``` bash
   make install
   ```

## Editor pulgins

[Visual Studio Code](https://code.visualstudio.com/) pulgin are in this directory: [./tools/pulgins/vscode](./tools/pulgins/vscode).

## Examples

After compiling successfully, use the `berry` command with no parameters to enter the REPL environment:
```
Berry 0.0.1 (build in Dec 24 2018, 18:12:49)
[GCC 8.2.0] on Linux (default)
>
```

Now enter this code:

``` lua
print("Hello world!")
```

You will see this output:

```
Hello world!
```

You can copy this code to the REPL:

``` ruby
def fib(x)
    if x <= 1
        return x
    end
    return fib(x - 1) + fib(x - 2)
end
fib(10)
```

This example code will output the result `55` and you can save the above code to a plain text file (eg test.be) and run this command:

``` bash
./berry test.be
```

This will also get the correct output.

## License

Berry is free software distributed under the [MIT license](./LICENSE).

The Berry interpreter partly referred to [Lua](http://www.lua.org/)'s design. View Lua's license here: http://www.lua.org/license.html.
