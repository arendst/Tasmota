Import("env")

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=["-Wno-discarded-qualifiers", "-Wno-implicit-function-declaration", "-Wno-incompatible-pointer-types"])


# Remove build flags which are not valid for risc-v
build_flags = env['BUILD_FLAGS']
chip = env.get("BOARD_MCU").lower()

if "c" in chip:
  build_flags.pop(build_flags.index("-mno-target-align"))
  build_flags.pop(build_flags.index("-mtarget-align"))
