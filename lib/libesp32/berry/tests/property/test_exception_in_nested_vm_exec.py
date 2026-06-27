"""
Regression: Berry exception raised from a closure invoked by a native
function while a try/except is active in the outer closure.

Symptom
-------
    ./berry -s -g tests/reference.be   → passes
    python3 -m berry_port -s -g tests/reference.be   → fails with a
        spurious "runtime_error: runtime_error" reported after the
        script has completed successfully.

Root cause
----------
The C interpreter uses setjmp/longjmp: be_throw() longjmps straight to
the OP_EXBLK setjmp point, bypassing every intermediate C frame.

The Python port replaces longjmp with a try/except around each
dispatch loop iteration. When a native library function (e.g. `str(f)`
→ be_baselib_str → be_tostring → _ins2str → be_dofunc → do_closure →
vm_exec) enters a *nested* vm_exec to run a Berry closure, that nested
vm_exec runs its own try/except. If that closure raises, the nested
vm_exec would:

  1. catch the BerryException,
  2. see that vm.exceptstack is non-empty (the frame belongs to an
     OUTER closure's try),
  3. call be_except_block_resume — which unwinds the callstack past
     the nested vm_exec's base frame,
  4. continue its own dispatch loop with a now-stale `clos` / ktab.

The corrupted nested loop then re-raises through the outer-vm_exec
try/except *after* the outer handler has already recovered, leaking
the exception up to be_pcall and producing the spurious error at the
top level.

Fix
---
At entry to vm_exec, snapshot the callstack depth. When catching a
BerryException, if the target handler's `depth` is shallower than this
snapshot, the handler lives in an outer vm_exec — re-raise so the
outer Python frame can catch it, mirroring longjmp's behaviour.

Validates: exception propagation across native/Berry boundaries.
"""

import io
import os
import subprocess
import sys

import pytest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from berry_port.berry import be_vm_new, be_vm_delete, be_dostring, BE_OK


BERRY_CMD = [sys.executable, "-m", "berry_port"]


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def _run_python_script(source):
    """Execute Berry source on the Python port in-process.

    Captures stdout and returns (rc, stdout_str).
    """
    vm = be_vm_new()
    old_stdout = sys.stdout
    sys.stdout = captured = io.StringIO()
    try:
        rc = be_dostring(vm, source)
    finally:
        sys.stdout = old_stdout
        be_vm_delete(vm)
    return rc, captured.getvalue()


def _run_subprocess(source, *extra_args):
    """Run Berry source via `python -m berry_port -e`.

    Returns subprocess.CompletedProcess.
    """
    return subprocess.run(
        [*BERRY_CMD, *extra_args, "-e", source],
        capture_output=True,
        text=True,
        timeout=15,
    )


# ---------------------------------------------------------------------------
# Minimal in-process regression: raise from tostring() called via str()
# ---------------------------------------------------------------------------

class TestExceptionFromNativeDispatchedClosure:
    """be_pcall must not report an exception that was caught by the
    Berry script's own try/except."""

    def test_raise_from_tostring_is_caught(self):
        """The canonical minimal repro: str(f) triggers f.tostring()
        which raises; the outer try/except must catch cleanly and
        execution must continue."""
        rc, out = _run_python_script("""
class Failable
    def tostring()
        raise "internal_error", "FAIL"
    end
end
var f = Failable()
try
    var _ = str(f)
    print("UNREACHED")
except ..
    print("caught")
end
print("after")
""")
        assert rc == BE_OK, f"be_pcall returned {rc}, expected BE_OK"
        assert out == "caught\nafter\n", (
            f"unexpected output: {out!r}"
        )

    def test_raise_from_print_via_str(self):
        """print(str(f)) with failing tostring must not produce spurious
        errors after the script returns."""
        r = _run_subprocess("""
class Failable
    def tostring() raise "internal_error", "FAIL" end
end
var f = Failable()
try
    print(str(f))
except ..
    print("caught")
end
print("done")
""")
        assert r.returncode == 0, (
            f"rc={r.returncode}\nSTDOUT:\n{r.stdout}\nSTDERR:\n{r.stderr}"
        )
        assert r.stdout == "caught\ndone\n", (
            f"unexpected stdout: {r.stdout!r}"
        )
        assert "runtime_error" not in r.stdout + r.stderr, (
            "spurious runtime_error leaked past the Berry try/except"
        )

    def test_reference_test_file(self):
        """The original failing test: tests/reference.be must succeed
        under the Python port with the same flags as the C build."""
        path = os.path.join(os.path.dirname(__file__), '..', 'reference.be')
        assert os.path.exists(path), f"missing {path}"
        r = subprocess.run(
            [*BERRY_CMD, "-s", "-g", path],
            capture_output=True, text=True, timeout=15,
        )
        assert r.returncode == 0, (
            f"rc={r.returncode}\nSTDOUT:\n{r.stdout}\nSTDERR:\n{r.stderr}"
        )
        assert "runtime_error" not in r.stdout + r.stderr, (
            "spurious runtime_error leaked past the Berry try/except"
        )


# ---------------------------------------------------------------------------
# Exception propagates through multiple nested native calls
# ---------------------------------------------------------------------------

class TestDeeplyNestedNativeDispatch:
    """Raising inside a closure that is reached through several
    native→Berry transitions must unwind cleanly to the matching
    try/except, regardless of the nesting depth."""

    def test_map_tostring_raising(self):
        """map.tostring() iterates and calls _ins2str on each value;
        if a value's tostring() raises, the exception must be caught
        at the Berry try level, not leak past be_pcall."""
        rc, out = _run_python_script("""
class Failable
    def tostring() raise "custom", "oops" end
end
var f = Failable()
var m = {"k": f}
try
    var _ = str(m)
except ..
    print("caught")
end
print("done")
""")
        assert rc == BE_OK
        assert out == "caught\ndone\n", f"got: {out!r}"

    def test_list_tostring_raising(self):
        """Same for list.tostring()."""
        rc, out = _run_python_script("""
class Failable
    def tostring() raise "custom", "oops" end
end
var f = Failable()
var l = [1, 2, f]
try
    var _ = str(l)
except ..
    print("caught")
end
print("done")
""")
        assert rc == BE_OK
        assert out == "caught\ndone\n", f"got: {out!r}"

    def test_nested_try_blocks_raise_from_str(self):
        """Inner try catches; outer try must not see the exception."""
        rc, out = _run_python_script("""
class Failable
    def tostring() raise "E", "msg" end
end
var f = Failable()
try
    try
        var _ = str(f)
    except ..
        print("inner")
    end
    print("between")
except ..
    print("UNREACHED outer")
end
print("end")
""")
        assert rc == BE_OK
        assert out == "inner\nbetween\nend\n", f"got: {out!r}"

    def test_outer_try_catches_when_inner_reraises(self):
        """Inner except re-raises; outer catches."""
        rc, out = _run_python_script("""
class Failable
    def tostring() raise "E", "msg" end
end
var f = Failable()
try
    try
        var _ = str(f)
    except .. as e, m
        print("inner rethrow")
        raise e, m
    end
except .. as e, m
    print("outer caught:", m)
end
print("end")
""")
        assert rc == BE_OK
        assert out == "inner rethrow\nouter caught: msg\nend\n", (
            f"got: {out!r}"
        )


# ---------------------------------------------------------------------------
# Self-referential / recursive tostring (the reference.be scenario)
# ---------------------------------------------------------------------------

class TestSelfReferentialListWithFailingElement:
    """The exact scenario from tests/reference.be: a self-referencing
    list that contains an element with a toggleable failing tostring()
    must produce identical output before and after a failed str()."""

    SCRIPT = """
class failable
    var fail
    def tostring()
        if self.fail
            raise "internal_error", "FAIL"
        else
            return "SUCCESS"
        end
    end
end
var f = failable()
var l1 = [1, 2, f]
l1.push(l1)

assert(str(l1) == "[1, 2, SUCCESS, [...]]")

f.fail = true
try
    var _ = str(l1)
except ..
end

f.fail = false
assert(str(l1) == "[1, 2, SUCCESS, [...]]")
print("ok")
"""

    def test_inprocess(self):
        rc, out = _run_python_script(self.SCRIPT)
        assert rc == BE_OK, f"rc={rc}, out={out!r}"
        assert out.strip() == "ok", f"out={out!r}"

    def test_subprocess_with_g_s_flags(self):
        """Must also pass under -s -g (strict mode + named globals),
        matching the original failing invocation."""
        r = subprocess.run(
            [*BERRY_CMD, "-s", "-g", "-e", self.SCRIPT],
            capture_output=True, text=True, timeout=15,
        )
        assert r.returncode == 0, (
            f"rc={r.returncode}\nSTDOUT:\n{r.stdout}\nSTDERR:\n{r.stderr}"
        )
        assert r.stdout.strip() == "ok"


# ---------------------------------------------------------------------------
# Refstack cleanup: be_except_block_resume must restore refstack
# ---------------------------------------------------------------------------

class TestRefstackRestoredOnException:
    """When tostring() raises mid-traversal of a collection, the ref-
    tracking stack (used to detect circular references) must be restored
    to its pre-try count so later str() calls don't treat unrelated
    containers as already-in-progress."""

    def test_list_refstack_restored(self):
        """After a failed str(l1), subsequent str(l1) must produce the
        full representation, not just '[...]'."""
        rc, out = _run_python_script("""
class F
    var boom
    def tostring()
        if self.boom raise "e", "x" end
        return "OK"
    end
end
var f = F()
var l = [f]
f.boom = true
try
    var _ = str(l)
except ..
end
f.boom = false
print(str(l))
""")
        assert rc == BE_OK
        assert out.strip() == "[OK]", f"got: {out!r}"

    def test_map_refstack_restored(self):
        """Same property for maps: refstack must be cleaned after a
        caught exception so the map is no longer considered 'in progress'."""
        rc, out = _run_python_script("""
class F
    var boom
    def tostring()
        if self.boom raise "e", "x" end
        return "OK"
    end
end
var f = F()
var m = {"k": f}
f.boom = true
try
    var _ = str(m)
except ..
end
f.boom = false
print(str(m))
""")
        assert rc == BE_OK
        assert out.strip() == "{'k': OK}", f"got: {out!r}"


# ---------------------------------------------------------------------------
# Plain try/except (no native dispatch) must still work — no regression
# ---------------------------------------------------------------------------

class TestPlainTryExceptStillWorks:
    """Sanity check that the fix does not regress the straight-forward
    try/except path where no native frame is interposed."""

    def test_plain_raise_catch(self):
        rc, out = _run_python_script("""
try
    raise "foo", "bar"
except ..
    print("caught")
end
print("done")
""")
        assert rc == BE_OK
        assert out == "caught\ndone\n"

    def test_raise_from_berry_function(self):
        rc, out = _run_python_script("""
def boom()
    raise "e", "m"
end
try
    boom()
except ..
    print("caught")
end
print("done")
""")
        assert rc == BE_OK
        assert out == "caught\ndone\n"

    def test_uncaught_exception_still_fails_pcall(self):
        """An exception that is NOT caught must still be reported by
        be_pcall — the fix must not suppress genuine failures."""
        rc, _ = _run_python_script("""
raise "uncaught", "leaked"
""")
        assert rc != BE_OK, "uncaught exception should fail pcall"
