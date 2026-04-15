"""
Tests for berry_port CLI (__main__.py) and REPL mode.

Uses subprocess.run for CLI tests and mock stdin for REPL tests.
Validates Requirements: 20.4, 25.1, 25.4
"""

import os
import sys
import subprocess
import tempfile
import pytest

BERRY_CMD = [sys.executable, "-m", "berry_port"]


def run_berry(*args, input_text=None, timeout=15):
    """Run berry_port CLI with given args, return CompletedProcess."""
    return subprocess.run(
        [*BERRY_CMD, *args],
        capture_output=True,
        text=True,
        input=input_text,
        timeout=timeout,
    )


# ── Version and Help ────────────────────────────────────────────────────

class TestVersionAndHelp:
    def test_version_flag(self):
        r = run_berry("-v")
        assert r.returncode == 0
        assert "Berry" in r.stdout
        assert "1.1.0" in r.stdout

    def test_help_flag(self):
        r = run_berry("-h")
        assert r.returncode == 0
        assert "Usage:" in r.stdout
        assert "-i" in r.stdout
        assert "-e" in r.stdout
        assert "-c" in r.stdout
        assert "-o" in r.stdout
        assert "-g" in r.stdout
        assert "-s" in r.stdout
        assert "-m" in r.stdout
        assert "-v" in r.stdout
        assert "-h" in r.stdout
        assert "-l" in r.stdout


# ── Execute string (-e) ────────────────────────────────────────────────

class TestExecuteString:
    def test_e_print_hello(self):
        r = run_berry("-e", "print('hello')")
        assert r.returncode == 0
        assert r.stdout.strip() == "hello"

    def test_e_arithmetic(self):
        r = run_berry("-e", "print(1+2)")
        assert r.returncode == 0
        assert r.stdout.strip() == "3"

    def test_e_syntax_error(self):
        r = run_berry("-e", "print(")
        assert r.returncode != 0

    def test_e_multistatement(self):
        r = run_berry("-e", "var x = 42; print(x)")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"

    def test_e_function(self):
        r = run_berry("-e",
            "def fib(n) if n<=1 return n end return fib(n-1)+fib(n-2) end print(fib(10))")
        assert r.returncode == 0
        assert r.stdout.strip() == "55"

    def test_e_print_module(self):
        """Printing a module should output '<module: NAME>' without crashing."""
        r = run_berry("-e", "import debug print(debug)")
        assert r.returncode == 0
        assert r.stdout.strip() == "<module: debug>"

    def test_e_print_multiple_modules(self):
        """Printing various modules should all work (const, native, etc.)."""
        r = run_berry("-e", "import math print(math)")
        assert r.returncode == 0
        assert r.stdout.strip() == "<module: math>"

    def test_e_attrdump_module(self):
        """debug.attrdump(debug) should list module attributes without crashing."""
        r = run_berry("-e", "import debug debug.attrdump(debug)")
        assert r.returncode == 0
        assert "value type <module>" in r.stdout
        assert "attrdump: <function>" in r.stdout

    def test_e_solidify_dump(self):
        """solidify.dump(f) should output C code without crashing."""
        r = run_berry("-e",
            "import solidify def f() return 42 end solidify.dump(f)")
        assert r.returncode == 0
        assert "be_local_closure" in r.stdout
        assert "LDINT" in r.stdout
        assert "RET" in r.stdout


# ── Script file execution ───────────────────────────────────────────────

class TestScriptFile:
    def test_run_script_file(self):
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("print('from file')\n")
            f.flush()
            try:
                r = run_berry(f.name)
                assert r.returncode == 0
                assert r.stdout.strip() == "from file"
            finally:
                os.unlink(f.name)

    def test_script_file_not_found(self):
        r = run_berry("/tmp/nonexistent_berry_script_12345.be")
        assert r.returncode != 0

    def test_script_with_args(self):
        """Verify _argv contains positional args after the script."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("print(_argv)\n")
            f.flush()
            try:
                r = run_berry(f.name, "foo", "bar")
                assert r.returncode == 0
                assert "foo" in r.stdout
                assert "bar" in r.stdout
            finally:
                os.unlink(f.name)


# ── _argv global ────────────────────────────────────────────────────────

class TestArgv:
    def test_argv_empty_with_e(self):
        r = run_berry("-e", "print(_argv)")
        assert r.returncode == 0
        assert "[]" in r.stdout

    def test_argv_with_positional_args(self):
        r = run_berry("-e", "print(_argv)", "a", "b", "c")
        # After -e, remaining args are positional — they go into _argv
        # but also the first one is treated as a script file.
        # The _argv should contain ['a', 'b', 'c']
        assert "a" in r.stdout
        assert "b" in r.stdout
        assert "c" in r.stdout


# ── Compiler flags (-g, -s, -l) ────────────────────────────────────────

class TestCompilerFlags:
    def test_strict_mode_undeclared_var(self):
        """In strict mode, reading an undeclared variable inside a function should error."""
        r = run_berry("-s", "-e", "def f() return x end f()")
        assert r.returncode != 0

    def test_named_globals_basic(self):
        """Named globals mode: var + print should work."""
        r = run_berry("-g", "-e", "var a = 42 print(a)")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"

    def test_named_globals_with_strict(self):
        """Named globals + strict mode together."""
        r = run_berry("-g", "-s", "-e", "var a = 42 print(a)")
        assert r.returncode == 0
        assert r.stdout.strip() == "42"

    def test_named_globals_function(self):
        """Named globals mode with function definition and call."""
        r = run_berry("-g", "-e",
            "def add(a, b) return a + b end print(add(3, 4))")
        assert r.returncode == 0
        assert r.stdout.strip() == "7"

    def test_local_mode(self):
        """With -l, variables in script are parsed as local."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("x = 42\nprint(x)\n")
            f.flush()
            try:
                r = run_berry("-l", f.name)
                assert r.returncode == 0
                assert r.stdout.strip() == "42"
            finally:
                os.unlink(f.name)


# ── Bytecode compilation (-c, -o) ──────────────────────────────────────

class TestBytecodeCompilation:
    def test_compile_to_default_output(self):
        """Test -c compiles to a.out by default (in CWD)."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("print('compiled')\n")
            f.flush()
            outfile = "a.out"
            try:
                # Clean up any previous output
                if os.path.exists(outfile):
                    os.unlink(outfile)
                r = run_berry("-c", f.name)
                assert r.returncode == 0
                assert os.path.exists(outfile)
                # Check magic bytes
                with open(outfile, "rb") as bf:
                    magic = bf.read(3)
                    assert magic == b'\xBE\xCD\xFE'
            finally:
                os.unlink(f.name)
                if os.path.exists(outfile):
                    os.unlink(outfile)

    def test_compile_with_output_name(self):
        """Test -c with -o writes to specified file."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("print('compiled')\n")
            f.flush()
            outfile = os.path.join(tempfile.gettempdir(), "test_output.bec")
            try:
                if os.path.exists(outfile):
                    os.unlink(outfile)
                r = run_berry("-c", f.name, "-o", outfile)
                assert r.returncode == 0
                assert os.path.exists(outfile)
                with open(outfile, "rb") as bf:
                    magic = bf.read(3)
                    assert magic == b'\xBE\xCD\xFE'
            finally:
                os.unlink(f.name)
                if os.path.exists(outfile):
                    os.unlink(outfile)


# ── Module path (-m) ───────────────────────────────────────────────────

class TestModulePath:
    def test_module_path_set(self):
        """Test -m sets custom module search path."""
        mod_dir = tempfile.mkdtemp()
        # Create a simple Berry module file
        mod_file = os.path.join(mod_dir, "testmod.be")
        with open(mod_file, "w") as f:
            f.write("# simple module\n")
        try:
            # Just verify -m doesn't crash — actual module loading
            # depends on the module system wiring
            r = run_berry("-m", mod_dir, "-e", "print('ok')")
            assert r.returncode == 0
            assert r.stdout.strip() == "ok"
        finally:
            os.unlink(mod_file)
            os.rmdir(mod_dir)


# ── REPL mode ──────────────────────────────────────────────────────────

class TestREPL:
    def test_repl_expression_evaluation(self):
        """REPL evaluates expressions and prints non-nil results."""
        r = run_berry(input_text="1 + 2\n")
        assert r.returncode == 0
        assert "3" in r.stdout

    def test_repl_print(self):
        """REPL print statement."""
        r = run_berry(input_text="print('repl hello')\n")
        assert r.returncode == 0
        assert "repl hello" in r.stdout

    def test_repl_empty_lines(self):
        """REPL handles empty/blank lines without error."""
        r = run_berry(input_text="\n\n\n")
        assert r.returncode == 0

    def test_repl_eof_exits_cleanly(self):
        """REPL exits cleanly on EOF (empty input)."""
        r = run_berry(input_text="")
        assert r.returncode == 0

    def test_repl_syntax_error_continues(self):
        """REPL prints error on syntax error and continues."""
        # Use '1 +' which is incomplete but NOT ending with 'EOS' in the
        # try_return path, then use a clear error like '0/0' division
        r = run_berry(input_text="1 @ 2\nprint('after error')\n")
        assert r.returncode == 0
        assert "after error" in r.stdout

    def test_repl_multiline_function(self):
        """REPL handles multi-line function definition."""
        code = "def f()\nreturn 42\nend\nprint(f())\n"
        r = run_berry(input_text=code)
        assert r.returncode == 0
        assert "42" in r.stdout

    def test_repl_var_assignment_and_use(self):
        """REPL can assign and use variables across lines."""
        code = "var x = 10\nprint(x * 3)\n"
        r = run_berry(input_text=code)
        assert r.returncode == 0
        assert "30" in r.stdout


# ── Interactive mode after script (-i) ─────────────────────────────────

class TestInteractiveAfterScript:
    def test_i_flag_runs_script_then_repl(self):
        """With -i, script runs first, then REPL starts (EOF exits)."""
        with tempfile.NamedTemporaryFile(mode="w", suffix=".be", delete=False) as f:
            f.write("print('script ran')\n")
            f.flush()
            try:
                r = run_berry("-i", f.name, input_text="")
                assert r.returncode == 0
                assert "script ran" in r.stdout
            finally:
                os.unlink(f.name)


# ── Option parsing edge cases ──────────────────────────────────────────

class TestOptionParsing:
    def test_unknown_option(self):
        """Unknown option produces error."""
        r = run_berry("-z")
        assert r.returncode != 0

    def test_combined_v_and_h(self):
        """Both -v and -h can be used together."""
        r = run_berry("-v", "-h")
        assert r.returncode == 0
        assert "Berry" in r.stdout
        assert "Usage:" in r.stdout

    def test_option_parsing_unit_tests(self):
        """Unit test the option parser directly."""
        from berry_port.__main__ import (
            arg_opts, parse_arg, match_opt, is_letter,
            arg_i, arg_c, arg_o, arg_l, arg_h, arg_v,
            arg_e, arg_g, arg_s, arg_err, arg_m,
        )

        # -v
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        assert parse_arg(opt, ["berry", "-v"]) == arg_v

        # -h
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        assert parse_arg(opt, ["berry", "-h"]) == arg_h

        # -i -l
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        args = parse_arg(opt, ["berry", "-i", "-l", "script.be"])
        assert args == (arg_i | arg_l)
        assert opt.idx == 3
        assert ["berry", "-i", "-l", "script.be"][opt.idx:] == ["script.be"]

        # -e with argument
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        args = parse_arg(opt, ["berry", "-e", "print(1)"])
        assert args == arg_e
        assert opt.execute == "print(1)"

        # -c with -o
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        args = parse_arg(opt, ["berry", "-c", "s.be", "-o", "out.bec"])
        assert args == (arg_c | arg_o)
        assert opt.src == "s.be"
        assert opt.dst == "out.bec"

        # -g -s
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        assert parse_arg(opt, ["berry", "-g", "-s"]) == (arg_g | arg_s)

        # -m with path
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        args = parse_arg(opt, ["berry", "-m", "/path", "script.be"])
        assert args == arg_m
        assert opt.modulepath == "/path"
        assert ["berry", "-m", "/path", "script.be"][opt.idx:] == ["script.be"]

        # unknown option
        opt = arg_opts()
        opt.pattern = "m?vhile?gsc?o?"
        assert parse_arg(opt, ["berry", "-z"]) & arg_err

    def test_match_opt(self):
        """Unit test match_opt."""
        from berry_port.__main__ import match_opt
        # 'v' is in pattern
        assert match_opt("m?vhile?gsc?o?", "v") is not None
        # 'e' has '?' after it
        res = match_opt("m?vhile?gsc?o?", "e")
        assert res is not None
        assert res[1] is True  # has_arg
        # 'i' has no '?' after it
        res = match_opt("m?vhile?gsc?o?", "i")
        assert res is not None
        assert res[1] is False  # no arg
        # 'z' not in pattern
        assert match_opt("m?vhile?gsc?o?", "z") is None

    def test_is_letter(self):
        """Unit test is_letter."""
        from berry_port.__main__ import is_letter
        assert is_letter("a") is True
        assert is_letter("Z") is True
        assert is_letter("?") is False
        assert is_letter("1") is False
        assert is_letter("") is False
