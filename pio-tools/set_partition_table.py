#
# The scipt sets the missing "LDSCRIPT_PATH" when using the command `pio run -t nobuild`
# Adopted from https://github.com/platformio/platform-espressif32/issues/861#issuecomment-1241871437
# Possible now to upload the firmware or the filesystem with (when builded already!):
#
# `pio run -t nobuild -t upload` and `pio run -t nobuild -t uploadfs`
#

Import("env")

import os
import tasmotapiolib
from os.path import isfile, join
import shutil
from SCons.Script import COMMAND_LINE_TARGETS

board_config = env.BoardConfig()

if "nobuild" in COMMAND_LINE_TARGETS:
    prog_name = join(env.subst("$BUILD_DIR"),"firmware.bin")
    if not os.path.isfile(prog_name):
        #print ("No firmware in path:",join(env.subst("$BUILD_DIR")))
        env.CleanProject()
        cur_env = (env["PIOENV"])
        firm_name = cur_env + ".bin"
        source_firm = tasmotapiolib.get_final_bin_path(env)
        if not os.path.exists(join(env.subst("$BUILD_DIR"))):
            os.makedirs(join(env.subst("$BUILD_DIR")))
        if os.path.isfile(source_firm):
            shutil.copy(source_firm, join(env.subst("$BUILD_DIR")))
            target_ren = join(env.subst("$BUILD_DIR"), firm_name)
            os.rename(target_ren, prog_name)

    if env["PIOPLATFORM"] != "espressif32":
        framework_dir = env.PioPlatform().get_package_dir("framework-arduinoespressif8266")
        assert os.path.isdir(framework_dir)
        env.Replace(
            LDSCRIPT_PATH=os.path.join(
                framework_dir,
                "tools",
                "sdk",
                "ld",
                board_config.get("build.arduino.ldscript"),
           )
        )
#        print("Set LDSCRIPT_PATH to: ", os.path.join(framework_dir,"tools","sdk","ld",board_config.get("build.arduino.ldscript")))

#
# For ESP32 sets the missing "PARTITIONS_TABLE_CSV" when using the command `pio run -t nobuild`
#
    else:
        env.Replace(
            PARTITIONS_TABLE_CSV=os.path.join(
                board_config.get("build.partitions"),
            )
        )
#        print("Set PARTITIONS_TABLE_CSV to: ", os.path.join(board_config.get("build.partitions")))
