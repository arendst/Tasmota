import glob
import os

Import("env")

def FindInoNodes(env):
    src_dir = glob.escape(env.subst("$PROJECT_SRC_DIR"))
    return env.Glob(os.path.join(src_dir, "*.ino")) + env.Glob(
        os.path.join(src_dir, "ino_*", "*.ino")
    )

env.AddMethod(FindInoNodes)
