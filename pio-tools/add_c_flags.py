Import("env")

import json
import re
from pathlib import Path

build_flags = env['BUILD_FLAGS']
mcu = env.get("BOARD_MCU").lower()

# Determine IDF version
idf_major = idf_minor = idf_patch = None


def _detect_idf_version():
  platform = env.PioPlatform()

  def _safe_get_package_dir(package_name):
    try:
      return platform.get_package_dir(package_name)
    except Exception:
      return None

  package_dirs = [
    _safe_get_package_dir("framework-arduinoespressif32"),
    _safe_get_package_dir("framework-arduinoespressif32-libs"),
  ]
  pkg_candidates = []

  for package_dir in package_dirs:
    if not package_dir:
      continue
    package_path = Path(package_dir)
    pkg_candidates.extend([
      package_path / "tools" / "esp32-arduino-libs" / "package.json",
      package_path / "package.json",
    ])

  for pkg_json in pkg_candidates:
    if not pkg_json.is_file():
      continue
    try:
      with pkg_json.open() as f:
        version_str = str(json.load(f).get("version", ""))
      match = re.search(r"(\d+)\.(\d+)\.(\d+)", version_str)
      if match:
        return tuple(int(part) for part in match.groups())
    except Exception:
      pass

  return None


idf_version = _detect_idf_version()
if idf_version:
  idf_major, idf_minor, idf_patch = idf_version
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
if idf_version and idf_version < (5, 5, 4):
  try:
    build_flags.pop(build_flags.index("-DUSE_SHA_ROM"))
    print("Removed -DUSE_SHA_ROM from build flags for compatibility with IDF versions earlier than v5.5.4")
  except:
    pass
