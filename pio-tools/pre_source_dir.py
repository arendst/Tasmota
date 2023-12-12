Import("env")

import glob
import os

def FindInoNodes(env):
    src_dir = glob.escape(env.subst("$PROJECT_SRC_DIR"))
    return env.Glob(os.path.join(src_dir, "*.ino")) + env.Glob(
        os.path.join(src_dir, "tasmota_*", "*.ino")
    )

env.AddMethod(FindInoNodes)

# Pass flashmode at build time to macro
memory_type = env.BoardConfig().get("build.arduino.memory_type", "").upper()
flash_mode = env.BoardConfig().get("build.flash_mode", "dio").upper()
if "OPI_" in memory_type:
    flash_mode = "OPI"

tasmota_flash_mode = "-DCONFIG_TASMOTA_FLASHMODE_" + flash_mode
env.Append(CXXFLAGS=[tasmota_flash_mode])
print(tasmota_flash_mode)
