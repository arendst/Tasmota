# Wrap FILE*-based printf functions on ESP8266 to eliminate newlib's
# _vfprintf_r (saves ~1.6 KB of flash).
#
# Tasmota's logging writes directly to the UART via Arduino's Serial, so the
# libc FILE*-based printf path is dead code. By redirecting it through
# vsnprintf() + fwrite() (already present for logging), the linker can drop
# _vfprintf_r entirely.
#
# To opt out (keep the full newlib printf path), define USE_FULL_PRINTF in
# build_flags for the affected env.

import os

Import("env")

mcu = (env.get("BOARD_MCU") or "").lower()

# Only act for ESP8266 builds.
if mcu == "esp8266":
    build_flags = " ".join(env.get("BUILD_FLAGS") or [])

    if "USE_FULL_PRINTF" not in build_flags:
        # 1) Add the linker wrap flags so that any reference to printf /
        #    vprintf / fprintf is redirected to our __wrap_* implementations.
        env.Append(LINKFLAGS=[
            "-Wl,--wrap=vprintf",
            "-Wl,--wrap=printf",
            "-Wl,--wrap=fprintf",
        ])

        # 2) Compile the wrap stubs and link them into the firmware. The
        #    source file lives next to this script in pio-tools/.
        stubs_src = os.path.join(env.subst("$PROJECT_DIR"), "pio-tools", "printf_stubs.cpp")
        if os.path.isfile(stubs_src):
            env.BuildSources(
                os.path.join("$BUILD_DIR", "printf_stubs"),
                os.path.dirname(stubs_src),
                src_filter=["-<*>", "+<printf_stubs.cpp>"],
            )
        else:
            print("wrap_printf: WARNING - printf_stubs.cpp not found at %s" % stubs_src)
    else:
        print("wrap_printf: USE_FULL_PRINTF defined - keeping full newlib printf path")
