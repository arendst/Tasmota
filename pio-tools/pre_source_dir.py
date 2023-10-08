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
tasmota_flash_mode = "-DCONFIG_TASMOTA_FLASHMODE_" + (env.BoardConfig().get("build.flash_mode", "dio")).upper()
env.Append(CXXFLAGS=[tasmota_flash_mode])
print(tasmota_flash_mode)
