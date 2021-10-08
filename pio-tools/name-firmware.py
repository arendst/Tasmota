Import("env")
Import("projenv")
import os
import shutil
import pathlib

import tasmotapiolib


def bin_map_copy(source, target, env):
    firsttarget = pathlib.Path(target[0].path)

    # get locations and file names based on variant
    map_file = tasmotapiolib.get_final_map_path(env)
    bin_file = tasmotapiolib.get_final_bin_path(env)

    # check if new target files exist and remove if necessary
    for f in [map_file, bin_file]:
        if f.is_file():
            f.unlink()

    # copy firmware.bin and map to final destination
    shutil.copy(firsttarget, bin_file)
    shutil.move(tasmotapiolib.get_source_map_path(env), map_file)


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", bin_map_copy)
