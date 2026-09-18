#
# Picolibc-specific linker flags.
#
# When the Arduino ESP32 framework is built against picolibc (detected via
# `-specs=picolibc.specs`), redirect the floating-point capable vfprintf / vfscanf
# symbols to picolibc's integer-only variants (__i_vfprintf / __i_vfscanf).
# This drops the float/double formatting code path from the firmware and saves
# a noticeable amount of flash.
#

Import("env")


def _picolibc_in_use(env):
    for var in ("CCFLAGS", "CFLAGS", "CXXFLAGS", "LINKFLAGS"):
        for flag in env.get(var, []):
            if "picolibc.specs" in str(flag):
                return True
    return False


if _picolibc_in_use(env):
    env.Append(LINKFLAGS=[
        "-Wl,--defsym=vfprintf=__i_vfprintf",
        "-Wl,--defsym=vfscanf=__i_vfscanf",
    ])
