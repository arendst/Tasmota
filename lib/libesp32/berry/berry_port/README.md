# Berry Python Port

A pure-Python implementation of the [Berry scripting language](https://github.com/Skiars/berry) interpreter, ported line-by-line from the original ANSI C99 source code.

## Quick Start

```bash
# Launch the REPL (with named globals + strict mode)
python3 -m berry_port -s -g

# Run a Berry script
python3 -m berry_port -s -g testall.be

# Run the Python property test suite (1300+ tests)
python3 -m pytest tests/property/ -v --tb=short
```

## Why This Exists

Berry is the scripting engine behind [Tasmota](https://tasmota.github.io/docs/Berry/) firmware on ESP32 devices. The Tasmota build toolchain needs to **solidify** Berry code — compile Berry source into C constant objects that get embedded directly in firmware. This solidification step previously required building the C Berry interpreter.

This Python port eliminates that dependency. It runs solidification (and the full Berry interpreter) in pure Python, so the Tasmota toolchain works on any machine with Python 3 — no C compiler, no shared libraries, no platform-specific build steps.

The port is also useful for testing, debugging, and developing Berry language features without a C build cycle.

## Design Philosophy

The port deliberately mirrors the C source code rather than being "Pythonic":

- **1:1 file mapping** — each C source file maps to a Python module (`be_vm.c` → `be_vm.py`, `be_lexer.c` → `be_lexer.py`, etc.), 44 modules total.
- **C function names preserved** — `be_lexer_scan_next`, `be_code_binop`, `be_map_compact` keep their original names, making it easy to diff against the C source when it evolves.
- **Original C code as comments** — every translated function includes the original C implementation as a docstring or comment block.
- **C macros → Python functions/constants** — `IGET_OP`, `var_type`, `gc_setmark` and all enum values keep their C names and integer values.

This makes the port straightforward to maintain: when the C source changes, you can locate the corresponding Python function by name and update it.

## Architecture

The interpreter follows the same pipeline as the C version:

```
Berry Source → Lexer → Parser → Code Generator → bproto (bytecode + constants)
                                                      ↓
                                              VM Execution
                                                      ↓
                                              Solidification → C source output
```

### Core Components

| Component | Module | Role |
|-----------|--------|------|
| Lexer | `be_lexer.py` | Tokenizes Berry source code |
| Parser | `be_parser.py` | One-pass recursive descent, drives code generation |
| Code Generator | `be_code.py` | Emits register-based bytecode instructions |
| VM | `be_vm.py` | Executes 48 opcodes in a register-based dispatch loop |
| GC | `be_gc.py` | Mark-sweep garbage collector with three-color marking |
| Strings | `be_string.py` | FNV-1a hash-based string interning |
| Classes | `be_class.py` | Single-inheritance class/instance system |
| Containers | `be_list.py`, `be_map.py` | List and hash-table map |
| Solidification | `be_solidifylib.py` | Emits C source code from Berry objects |
| Bytecode I/O | `be_bytecode.py` | Save/load compiled bytecode files |
| Public API | `be_api.py` | Stack-based C API (`be_push*`, `be_to*`, `be_pcall`, etc.) |
| Built-in libs | `be_baselib.py`, `be_strlib.py`, `be_jsonlib.py`, `be_mathlib.py`, ... | Standard library functions |

### Key Design Decisions

- **Python exceptions replace `setjmp`/`longjmp`** — Berry's protected call mechanism uses a `BerryException` class hierarchy instead of C's non-local jumps.
- **No memory pools** — the C version has 16/32-byte pools for embedded targets. Python's allocator handles small objects natively. The `gc.usage` counter is still maintained for GC threshold logic.
- **Stack as Python list** — C pointer arithmetic on the value stack translates to integer index operations on a Python list, with `reg_idx`, `top_idx`, and `stacktop_idx` replacing pointer fields.
- **Pure Python, stdlib only** — depends only on standard library modules (`ctypes`, `struct`, `math`, `json`, `os`, `sys`, `re`). No compiled extensions.

## CLI Usage

```
Usage: python3 -m berry_port [options] [script [args]]

Available options are:
  -i        enter interactive mode after executing 'file'
  -l        all variables in 'file' are parsed as local
  -e        load 'script' source string and execute
  -m <path> custom module search path(s) separated by ':'
  -c <file> compile script 'file' to bytecode file
  -o <file> save bytecode to 'file'
  -g        force named globals in VM
  -s        force Berry compiler in strict mode
  -v        show version information
  -h        show help information
```

### Examples

```bash
# Interactive REPL
python3 -m berry_port -s -g

# Run a script
python3 -m berry_port -s -g examples/fib_rec.be

# Execute a one-liner
python3 -m berry_port -e "print('hello from Berry')"

# Compile to bytecode
python3 -m berry_port -c examples/fib_rec.be -o fib.bec

# Run the full Berry test suite
python3 -m berry_port -s -g testall.be

# Add custom module search paths
python3 -m berry_port -m "/path/to/modules:/other/path" script.be
```

The `-s` (strict mode) and `-g` (named globals) flags match the Tasmota toolchain defaults and are recommended for general use.

## Testing

The port is validated at two levels:

### Berry-level tests

The standard Berry test suite (`tests/*.be`, run via `testall.be`) exercises the interpreter end-to-end — the same scripts that validate the C implementation.

```bash
python3 -m berry_port -s -g testall.be
```

### Python property tests

Over 1300 pytest-based tests in `tests/property/` verify specific correctness properties:

```bash
python3 -m pytest tests/property/ -v --tb=short
```

These cover instruction encoding round-trips, string hash equivalence with the C implementation, map compaction determinism, bytecode serialization round-trips, solidification output character-for-character equivalence, lexer token equivalence, exception unwinding, upvalue lifecycle, VM execution equivalence, and more.

## Solidification

The primary use case for the Tasmota toolchain. Compile Berry source and emit C constant objects:

```python
# In a Berry script or REPL:
import solidify
def my_func() return 42 end
solidify.dump(my_func)
```

This outputs C source code using `be_local_closure`, `be_nested_proto`, `be_local_class`, and related macros — character-for-character identical to the C implementation's output. For classes, the solidifier performs constant table compaction (`m_compact_class`), merging per-method constant tables into a shared `be_ktab_` with patched instruction indices.

## Requirements

- Python 3 (standard library only)
- No C compiler or platform-specific build steps needed
