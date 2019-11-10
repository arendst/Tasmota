Import('env')
import os

def obj_ren_after_bin(source, target, env):
    #    print("Rename firmware.bin")
    base_dir = os.path.dirname(str(target[0]))
    os.rename(str(target[0]), "{}{}{}.bin".format(base_dir, os.path.sep, str(target[0]).split(os.path.sep)[1]))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", [obj_ren_after_bin]) 
