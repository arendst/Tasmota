#
# The scipt sets the missing "LDSCRIPT_PATH" when using the command `pio run -t nobuild`
# Adopted from https://github.com/platformio/platform-espressif32/issues/861#issuecomment-1241871437
# Possible now to upload the firmware or the filesystem with (when builded already!):
#
# `pio run -t nobuild -t upload` and `pio run -t nobuild -t uploadfs`
#

Import("env")

import os
from SCons.Script import COMMAND_LINE_TARGETS

board_config = env.BoardConfig()
framework_dir = env.PioPlatform().get_package_dir("framework-arduinoespressif8266")
assert os.path.isdir(framework_dir)

if "nobuild" in COMMAND_LINE_TARGETS:
    env.Replace(
        LDSCRIPT_PATH=os.path.join(
            framework_dir,
            "tools",
            "sdk",
            "ld",
            board_config.get("build.arduino.ldscript"),
        )
    )
#    print("Set LDSCRIPT_PATH to: ", os.path.join(framework_dir,"tools","sdk","ld",board_config.get("build.arduino.ldscript")))
