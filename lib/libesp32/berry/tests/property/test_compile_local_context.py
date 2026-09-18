"""
Tests for the ``compile()`` third argument ``islocal`` (PR #532).

The built-in ``compile(src, source_type, islocal)`` builtin was extended
with an optional third boolean argument selecting the compilation
context:

* ``islocal = false`` (default, global context) — top-level assignments
  in the compiled source become **globals**.
* ``islocal = true`` (local context / "loader" mode) — top-level
  assignments stay **local** to the compiled function, analogous to
  how a module loader is compiled.

This file validates:

  a) The C-level FFI surface exposed by the Python port mirrors the C
     macros (``be_loadbuffer_local``, ``be_loadfile_local``,
     ``be_loadstring_local``).
  b) The Berry-level ``compile()`` builtin accepts the new argument and
     produces the correct scoping behavior in both ``"string"`` and
     ``"file"`` modes.
  c) The Python port's output matches the C binary for every
     observable scenario, so the port's behavior stays bug-compatible
     with upstream.

These are end-to-end behavioral tests rather than Hypothesis property
tests — the feature surface is small (one boolean flag) and the
meaningful property is scope isolation, which does not benefit from
random input generation.
"""

import io
import os
import subprocess
import sys
import tempfile

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port import be_api, be_exec, berry
from berry_port.berry import be_vm_new, be_dostring


# ============================================================================
# Helpers
# ============================================================================

_BERRY_BIN = os.path.join(os.path.dirname(__file__), '..', '..', 'berry')


def _c_binary_available():
    return os.path.isfile(_BERRY_BIN) and os.access(_BERRY_BIN, os.X_OK)


def _run_python(source):
    """Run Berry source on the Python port, return (rc, stdout_str)."""
    vm = be_vm_new()
    old_stdout = sys.stdout
    sys.stdout = captured = io.StringIO()
    try:
        rc = be_dostring(vm, source)
    except Exception:
        rc = -1
    finally:
        sys.stdout = old_stdout
    return rc, captured.getvalue()


def _run_c(source):
    """Run Berry source on the C binary, return (rc, stdout_str) or None."""
    if not _c_binary_available():
        return None
    tmppath = None
    try:
        with tempfile.NamedTemporaryFile(
            mode='w', suffix='.be', delete=False
        ) as f:
            f.write(source + '\n')
            tmppath = f.name
        result = subprocess.run(
            [_BERRY_BIN, tmppath],
            capture_output=True, text=True, timeout=10,
        )
        return result.returncode, result.stdout
    except (subprocess.TimeoutExpired, OSError):
        return None
    finally:
        if tmppath is not None:
            try:
                os.unlink(tmppath)
            except OSError:
                pass


# ============================================================================
# (a) FFI surface — the Python port must expose the new symbols
# ============================================================================

class TestFFISurfaceExposesLocalVariants:
    """PR #532 adds ``_local`` counterparts to the buffer/file/string load
    primitives. The Python port must expose them at the same places the
    C header does, so embedders porting code across languages find them."""

    def test_be_api_has_be_loadbuffer_local(self):
        assert callable(getattr(be_api, 'be_loadbuffer_local', None)), \
            "be_api.be_loadbuffer_local must exist"

    def test_be_exec_has_be_loadbuffer_local(self):
        assert callable(getattr(be_exec, 'be_loadbuffer_local', None)), \
            "be_exec.be_loadbuffer_local must exist"

    def test_be_exec_has_be_loadfile_local(self):
        assert callable(getattr(be_exec, 'be_loadfile_local', None)), \
            "be_exec.be_loadfile_local must exist"

    def test_berry_has_be_loadstring_local(self):
        assert callable(getattr(berry, 'be_loadstring_local', None)), \
            "berry.be_loadstring_local must exist"

    def test_be_loadbuffer_local_compiles_like_be_loadbuffer(self):
        """With islocal=False, be_loadbuffer_local must behave exactly
        like the plain be_loadbuffer."""
        src = "return 1 + 2"
        vm1 = be_vm_new()
        rc1 = be_exec.be_loadbuffer(vm1, "t", src, len(src))
        vm2 = be_vm_new()
        rc2 = be_exec.be_loadbuffer_local(vm2, "t", src, len(src), False)
        assert rc1 == 0
        assert rc2 == 0

    def test_be_loadstring_local_round_trip(self):
        """be_loadstring_local(..., True) must compile a chunk in local
        context and the resulting closure must execute successfully."""
        from berry_port.be_api import be_pcall
        vm = be_vm_new()
        rc = berry.be_loadstring_local(vm, "return 99", True)
        assert rc == 0, f"load failed with rc={rc}"
        rc = be_pcall(vm, 0)
        assert rc == 0, f"call failed with rc={rc}"


# ============================================================================
# (b) Berry-level behavior — scoping semantics of compile()'s islocal flag
# ============================================================================

# Each vector is (description, berry_source, expected_stdout).
# These scripts deliberately use ``import global`` + ``global.contains`` so
# they are robust against strict mode (which forbids reading an undeclared
# name at parse time).
_BEHAVIOR_VECTORS = [
    (
        "default is global context",
        "import global\n"
        "var f = compile('x_default = 1', 'string')\n"
        "f()\n"
        "print(global.contains('x_default'), global.x_default)\n",
        "true 1\n",
    ),
    (
        "explicit islocal=false is global context",
        "import global\n"
        "var f = compile('x_explicit_global = 2', 'string', false)\n"
        "f()\n"
        "print(global.contains('x_explicit_global'), global.x_explicit_global)\n",
        "true 2\n",
    ),
    (
        "islocal=true keeps assignments local",
        "import global\n"
        "var f = compile('x_local = 3\\nreturn x_local', 'string', true)\n"
        "var r = f()\n"
        "print(r, global.contains('x_local'))\n",
        "3 false\n",
    ),
    (
        "islocal=true can return a value",
        "var f = compile('return 7 * 6', 'string', true)\n"
        "print(f())\n",
        "42\n",
    ),
    (
        "islocal=true forms a closure with its own locals",
        "var f = compile('var a = 10\\nvar b = 20\\nreturn a + b', 'string', true)\n"
        "print(f())\n",
        "30\n",
    ),
    (
        "repeated invocation: islocal does not pollute across calls",
        "import global\n"
        "var f = compile('n = 100\\nreturn n', 'string', true)\n"
        "print(f(), f(), global.contains('n'))\n",
        "100 100 false\n",
    ),
    (
        "non-boolean third arg is ignored (defaults to global)",
        # The C implementation checks be_isbool(vm, 3); a non-bool is
        # silently treated as the default (global).
        "import global\n"
        "var f = compile('x_nonbool = 4', 'string', 'not-a-bool')\n"
        "f()\n"
        "print(global.contains('x_nonbool'), global.x_nonbool)\n",
        "true 4\n",
    ),
]


@pytest.mark.parametrize("desc,source,expected", _BEHAVIOR_VECTORS)
def test_compile_islocal_behavior(desc, source, expected):
    """The Python port must implement the documented islocal semantics."""
    rc, out = _run_python(source)
    assert rc == 0, f"[{desc}] rc={rc}, out={out!r}"
    assert out == expected, (
        f"[{desc}] output mismatch\n"
        f"  source:   {source!r}\n"
        f"  expected: {expected!r}\n"
        f"  got:      {out!r}"
    )


# ============================================================================
# (c) compile() with "file" + islocal — same semantics via a disk file
# ============================================================================

class TestCompileFileIslocal:
    """compile(path, 'file', islocal) must honor the flag identically to
    the 'string' mode."""

    def _write_tmp(self, contents):
        fd, path = tempfile.mkstemp(suffix='.be')
        try:
            with os.fdopen(fd, 'w') as f:
                f.write(contents)
        except Exception:
            os.unlink(path)
            raise
        return path

    def test_file_local_mode_keeps_assignments_local(self):
        script = self._write_tmp("x_from_file = 123\nreturn x_from_file\n")
        try:
            source = (
                "import global\n"
                f"var f = compile('{script}', 'file', true)\n"
                "var r = f()\n"
                "print(r, global.contains('x_from_file'))\n"
            )
            rc, out = _run_python(source)
            assert rc == 0, f"rc={rc}, out={out!r}"
            assert out == "123 false\n", f"got {out!r}"
        finally:
            os.unlink(script)

    def test_file_default_mode_promotes_to_global(self):
        script = self._write_tmp("x_file_global = 456\n")
        try:
            source = (
                "import global\n"
                f"var f = compile('{script}', 'file')\n"
                "f()\n"
                "print(global.contains('x_file_global'), global.x_file_global)\n"
            )
            rc, out = _run_python(source)
            assert rc == 0, f"rc={rc}, out={out!r}"
            assert out == "true 456\n", f"got {out!r}"
        finally:
            os.unlink(script)

    def test_missing_file_raises_io_error(self):
        """Behavior for missing files must be unchanged — still raises
        io_error regardless of the islocal flag."""
        source = (
            "var f = compile('/nonexistent/path/does/not/exist.be',"
            " 'file', true)\n"
        )
        rc, _ = _run_python(source)
        assert rc != 0, "compiling a missing file must fail"


# ============================================================================
# (d) Cross-validation with the C binary — Python and C must agree
# ============================================================================

if _c_binary_available():
    @pytest.mark.parametrize("desc,source,expected", _BEHAVIOR_VECTORS)
    def test_islocal_matches_c_binary(desc, source, expected):
        """For every scoping scenario, the Python port must emit the same
        output as the C binary. This guards against the port diverging
        from upstream Berry."""
        py_rc, py_out = _run_python(source)
        c = _run_c(source)
        if c is None:
            pytest.skip("C binary execution failed")
        c_rc, c_out = c
        assert py_rc == c_rc, (
            f"[{desc}] rc mismatch: py={py_rc}, c={c_rc}\n"
            f"  py_out={py_out!r}\n  c_out={c_out!r}"
        )
        assert py_out == c_out, (
            f"[{desc}] output mismatch:\n"
            f"  expected: {expected!r}\n"
            f"  python:   {py_out!r}\n"
            f"  c:        {c_out!r}"
        )
