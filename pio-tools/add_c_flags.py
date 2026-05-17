Import("env")

import json
from pathlib import Path

build_flags = env['BUILD_FLAGS']
mcu = env.get("BOARD_MCU").lower()
framework_dir = env.PioPlatform().get_package_dir("framework-arduinoespressif32")

# Determine IDF version
idf_major = idf_minor = idf_patch = 0
pkg_json = Path(framework_dir) / "tools" / "esp32-arduino-libs" / "package.json"
with pkg_json.open() as f:
    version_str = json.load(f)["version"].split("+")[0]  # e.g. "5.5.4"
idf_major, idf_minor, idf_patch = (int(x) for x in version_str.split("."))
# print(f"IDF version: {idf_major}.{idf_minor}.{idf_patch}")

# General options that are passed to the C++ compiler
env.Append(CXXFLAGS=["-Wno-volatile"])

# General options that are passed to the C compiler (C only; not C++).
env.Append(CFLAGS=["-Wno-discarded-qualifiers", "-Wno-implicit-function-declaration", "-Wno-incompatible-pointer-types"])

# Remove build flags which are not valid for risc-v
if mcu not in ("esp32", "esp32s2", "esp32s3"):
  try:
    build_flags.pop(build_flags.index("-mno-target-align"))
  except:
    pass
  try:
    build_flags.pop(build_flags.index("-mtarget-align"))
  except:
    pass

# Remove -DUSE_SHA_ROM when building with IDF versions earlier than v5.5.4, 
# as it is not supported in Tasmota code.
if (idf_major, idf_minor, idf_patch) < (5, 5, 4):
  try:
    build_flags.pop(build_flags.index("-DUSE_SHA_ROM"))
    print("Removed -DUSE_SHA_ROM from build flags for compatibility with IDF versions earlier than v5.5.4")
  except:
    pass
