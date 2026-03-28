"""
berry.py — Top-level public API for the Berry Python port.

Mirrors berry.h from the C implementation.
Re-exports key public functions and defines version/error constants.
"""

# /*
# ** berry.h
# **
# ** #define BERRY_VERSION           "1.1.0"
# **
# ** enum berrorcode {
# **     BE_OK = 0,
# **     BE_EXIT,
# **     BE_MALLOC_FAIL,
# **     BE_EXCEPTION,
# **     BE_SYNTAX_ERROR,
# **     BE_EXEC_ERROR,
# **     BE_IO_ERROR
# ** };
# */

# Version
BERRY_VERSION = "1.1.0"

# Error codes — mirrors enum berrorcode in berry.h
BE_OK           = 0
BE_EXIT         = 1
BE_MALLOC_FAIL  = 2
BE_EXCEPTION    = 3
BE_SYNTAX_ERROR = 4
BE_EXEC_ERROR   = 5
BE_IO_ERROR     = 6

# Re-export key public functions from be_api
from berry_port.be_api import be_vm_new, be_vm_delete
from berry_port.be_api import be_loadbuffer
from berry_port.be_api import be_pcall, be_call


# /*
# ** #define be_loadstring(vm, str) \
# **     be_loadbuffer((vm), "string", (str), strlen(str))
# */
def be_loadstring(vm, s):
    """Load Berry source from a string. Mirrors the C macro be_loadstring.

    Normalizes the source string from Python Unicode to UTF-8 byte
    representation (as Latin-1) to match C Berry's raw byte handling.
    """
    s = s.encode('utf-8').decode('latin-1')
    return be_loadbuffer(vm, "string", s, len(s))


# /*
# ** #define be_dostring(vm, s) \
# **     (be_loadstring((vm), (s)) || be_pcall((vm), 0))
# */
def be_dostring(vm, s):
    """Compile and execute a Berry source string. Mirrors the C macro be_dostring."""
    ret = be_loadstring(vm, s)
    if ret != 0:
        return ret
    return be_pcall(vm, 0)
