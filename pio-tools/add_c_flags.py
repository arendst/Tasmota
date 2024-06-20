Import("env")

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=["-Wno-discarded-qualifiers", "-Wno-implicit-function-declaration", "-Wno-incompatible-pointer-types"])


# Remove build flags which are not valid for risc-v
build_flags = " ".join(env['BUILD_FLAGS'])
chip = env.get("BOARD_MCU").lower()

print("build_flags: ", build_flags)
#print(env.Dump())

if "c" in chip:
  print("**************** risc-v *****************")
  build_flags = build_flags.replace(" -mno-target-align", "")
  build_flags = build_flags.replace(" -mtarget-align", "")

new_buildflags = build_flags.split()
print("build_flags: ", build_flags)


env.Replace(
  BUILD_FLAGS=new_buildflags
)