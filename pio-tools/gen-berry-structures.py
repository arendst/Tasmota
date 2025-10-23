Import("env")

import os
import glob
import subprocess
from os.path import join

# generate all precompiled Berry structures from multiple modules
CURRENT_DIR = os.getcwd()
BERRY_GEN_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry")
os.chdir(BERRY_GEN_DIR)
fileList = glob.glob(join(BERRY_GEN_DIR, "generate", "*"))
for filePath in fileList:
    try:
        os.remove(filePath)
        # print("Deleting file : ", filePath)
    except:
        print("Error while deleting file : ", filePath)
cmd = (env["PYTHONEXE"],join("tools","coc","coc"),"-o","generate","src","default",join("..","berry_tasmota","src"),join("..","berry_matter","src","solidify"),join("..","berry_matter","src"),join("..","berry_custom","src","solidify"),join("..","berry_custom","src"),join("..","berry_animate","src","solidify"),join("..","berry_animate","src"),join("..","berry_animation","src","solidify"),join("..","berry_animation","src"),join("..","berry_tasmota","src","solidify"),join("..","berry_mapping","src"),join("..","berry_int64","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src","solidify"),join("..","..","libesp32_lvgl","lv_binding_berry","generate"),join("..","..","libesp32_lvgl","lv_haspmota","src","solidify"),"-c",join("default","berry_conf.h"))
returncode = subprocess.call(cmd, shell=False)
os.chdir(CURRENT_DIR)
