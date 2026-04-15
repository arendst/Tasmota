"""
Berry garbage collector — minimal stubs.

The C-style mark-sweep GC is dead code in the Python port (Python's own
GC handles object lifetime). This module retains only the functions that
the rest of the codebase actually calls at runtime:

  - be_gc_init / be_gc_deleteall — VM lifecycle
  - be_gc_collect / be_gc_auto / be_gc_setpause / be_gc_setsteprate — no-ops
  - be_gc_memcount — memory usage reporting
  - be_gc_fix / be_gc_unfix / be_gc_fix_set — GC_FIXED / GC_CONST mark flags
"""

from berry_port.be_object import (
    GC_FIXED, GC_CONST,
    gc_isfixed, gc_setfixed, gc_clearfixed, gc_isconst,
)


def be_gc_init(vm):
    """Initialize minimal GC state."""
    vm.gc.usage = 256


def be_gc_deleteall(vm):
    """Clean up open upvalues. Python GC handles objects."""
    uv = vm.upvalist
    while uv is not None:
        uvnext = uv.u_next
        uv = uvnext
    vm.upvalist = None


def be_gc_collect(vm):
    """No-op — Python GC handles memory."""
    pass


def be_gc_auto(vm):
    """No-op — no automatic collection needed."""
    pass


def be_gc_setpause(vm, pause):
    """No-op — pause/unpause has no effect without a collector."""
    pass


def be_gc_setsteprate(vm, rate):
    """No-op — step rate has no effect without a collector."""
    pass


def be_gc_memcount(vm):
    """Return the current tracked memory usage."""
    return vm.gc.usage


def be_gc_fix(vm, obj):
    """Mark an object as fixed (will not be collected)."""
    if not gc_isconst(obj):
        gc_setfixed(obj)


def be_gc_unfix(vm, obj):
    """Remove the fixed mark from an object."""
    if not gc_isconst(obj):
        gc_clearfixed(obj)


def be_gc_fix_set(vm, obj, fix):
    """Set or clear the fixed mark, returning the previous state."""
    was_fixed = gc_isfixed(obj)
    if not gc_isconst(obj):
        if fix:
            gc_setfixed(obj)
        else:
            gc_clearfixed(obj)
    return was_fixed
