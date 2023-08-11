Import("env")

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=["-Wno-discarded-qualifiers", "-Wno-implicit-function-declaration"])

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])

# Pass flashmode at build time to macro
tasmota_flash_mode = "-DCONFIG_TASMOTA_FLASHMODE_" + (env.BoardConfig().get("build.flash_mode", "dio")).upper()
env.Append(CXXFLAGS=[tasmota_flash_mode])