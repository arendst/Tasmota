# Property Tests

Python-side tests for the Berry Python port. These validate correctness properties of the interpreter — from low-level bit manipulation up to end-to-end solidification output equivalence with the C implementation.

## Running

```bash
# All tests
python3 -m pytest tests/property/ -v --tb=short

# A single file
python3 -m pytest tests/property/test_decoder_roundtrip.py -v

# Tests matching a keyword
python3 -m pytest tests/property/ -k "solidification" -v
```

## Test Patterns

Tests use two main styles:

### Hypothesis property tests

Many files use [Hypothesis](https://hypothesis.readthedocs.io/) to generate random inputs and verify invariants hold across the input space. These are the "property" tests in the formal sense — e.g., "for any valid opcode/RA/RKB/RKC, encoding then decoding produces the original values."

```python
@given(op=opcodes, ra=ra_vals, rkb=rkb_vals, rkc=rkc_vals)
def test_abc_roundtrip(op, ra, rkb, rkc):
    ins = ISET_OP(op) | ISET_RA(ra) | ISET_RKB(rkb) | ISET_RKC(rkc)
    assert IGET_OP(ins) == op
    ...
```

### End-to-end Berry execution tests

Other files compile and run Berry source code through the Python port, checking output against expected values. When the compiled C `berry` binary is available in the repo root, some tests also cross-validate against it.

```python
def test_python_port_output(desc, source, expected):
    result = _run_python(source)
    assert result.stdout.strip() == expected
```

## Test Categories

| File | What it tests |
|------|---------------|
| `test_decoder_roundtrip.py` | Instruction encoding/decoding round-trip (ABC, ABx, sBx formats) |
| `test_vector_growth.py` | Vector capacity doubling strategy matches C `be_nextsize` |
| `test_string_hash.py` | FNV-1a hash equivalence with C `be_strhash` |
| `test_string_interning.py` | Short string interning (≤64 bytes → same object identity) |
| `test_map_compaction.py` | Map slot layout after `be_map_compact` |
| `test_class_member_resolution.py` | Instance member lookup through inheritance chain |
| `test_upvalue_lifecycle.py` | Open/closed upvalue transitions |
| `test_lexer_token_equivalence.py` | Tokenizer output matches C lexer |
| `test_enum_preservation.py` | Python enum constants match C integer values |
| `test_bytecode_roundtrip.py` | Save/load bytecode produces equivalent protos |
| `test_exception_unwinding.py` | try/except/raise stack unwinding |
| `test_memory_tracking.py` | `gc.usage` counter consistency |
| `test_instruction_disassembly.py` | `be_print_inst` text output matches C |
| `test_native_stack_layout.py` | Native function call argument layout |
| `test_toidentifier.py` | String-to-C-identifier `_X` escape encoding |
| `test_solidification_output.py` | Solidified C source is character-for-character identical to C output |
| `test_compilation_equivalence.py` | Bytecode/constants match between Python and C compilers |
| `test_vm_execution.py` | End-to-end program output equivalence |
| `test_json_module.py` | JSON load/dump correctness and edge cases |
| `test_int64lib.py` | int64 class operations |
| `test_os_module.py` | os module (getcwd, chdir, mkdir, listdir, path.*) |
| `test_sys_module.py` | sys.path() returns module search paths |
| `test_re_module.py`, `test_re15_*.py` | Regex engine (re1.5 port) |
| `test_gc_collect.py` | GC mark-sweep cycle |
| `test_vararg.py` | Variadic function argument handling |
| `test_cli_repl.py` | CLI entry point and REPL behavior |
| `test_file_lib.py`, `test_file_read_unicode.py` | File I/O operations |
| `test_global_module.py`, `test_getgbl_builtins.py` | Global/builtin variable access |
| `test_*` (remaining) | Bug regression tests and specific edge cases |

## C Cross-Validation

Tests in `test_vm_execution.py`, `test_solidification_output.py`, and `test_compilation_equivalence.py` can optionally compare output against the compiled C `berry` binary. If `./berry` exists and is executable, these tests run the same input through both implementations and assert identical results. If the binary isn't available, those specific cross-validation tests are skipped — the Python-only assertions still run.
