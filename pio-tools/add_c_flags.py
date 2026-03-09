Import("env")

build_flags = env['BUILD_FLAGS']
mcu = env.get("BOARD_MCU").lower()

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=["-Wno-discarded-qualifiers", "-Wno-implicit-function-declaration", "-Wno-incompatible-pointer-types"])

# Remove build flags which are not valid for risc-v
if mcu in ("esp32c2", "esp32c3", "esp32c5", "esp32c6", "esp32h2", "esp32p4"):
  try:
    build_flags.pop(build_flags.index("-mno-target-align"))
  except:
    pass
  try:
    build_flags.pop(build_flags.index("-mtarget-align"))
  except:
    pass
