Import('env')
import os
import shutil

def name_firmware(source, target, env):
    base_dir = os.path.dirname(str(target[0]))
    new_file = "{}{}{}.bin".format(base_dir, os.path.sep, str(target[0]).split(os.path.sep)[1])
    if os.path.isfile(new_file):
        os.remove(new_file)
    shutil.copyfile(str(target[0]), new_file)

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", [name_firmware])
