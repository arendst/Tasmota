Import("env")
env = DefaultEnvironment()
platform = env.PioPlatform()
import os
import subprocess
from os.path import join

# generate all precompiled Berry structures from multiple modules
CURRENT_DIR = os.getcwd()
BERRY_GEN_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry")
os.chdir(BERRY_GEN_DIR)
cmd = ("python3",join("tools","coc","coc"),"-o","generate","src","default",join("..","berry_tasmota","src"),join("..","berry_mapping","src"),join("..","berry_int64","src"),join("..","..","libesp32_lvgl","lv_binding_berry","src"),join("..","..","libesp32_lvgl","lv_binding_berry","generate"),"-c",join("default","berry_conf.h"))
returncode = subprocess.call(cmd, shell=False)
os.chdir(CURRENT_DIR)
