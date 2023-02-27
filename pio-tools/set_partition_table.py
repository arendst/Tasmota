#
# The scipt sets the missing "PARTITIONS_TABLE_CSV" when using the command `pio run -t nobuild`
# Adopted from https://github.com/platformio/platform-espressif32/issues/861#issuecomment-1241871437
# Possible now to upload the firmware or the filesystem with (when builded already!):
#
# `pio run -t nobuild -t upload` and `pio run -t nobuild -t uploadfs`
#

Import("env")

import os
from SCons.Script import COMMAND_LINE_TARGETS

board_config = env.BoardConfig()

if "nobuild" in COMMAND_LINE_TARGETS:
    env.Replace(
        PARTITIONS_TABLE_CSV=os.path.join(
            board_config.get("build.partitions"),
        )
    )
#    print("Set PARTITIONS_TABLE_CSV to: ", os.path.join(board_config.get("build.partitions")))
