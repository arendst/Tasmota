# Little convenience script to solidify external berry files as embedded

Import("env")

import os
import sys
from genericpath import exists
from os.path import join
import subprocess
from colorama import Fore, Back, Style
import requests
import re

IS_WINDOWS = sys.platform.startswith("win")

def ensureBerry():
    BERRY_GEN_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry")
    os.chdir(BERRY_GEN_DIR)
    BERRY_EXECUTABLE = join(BERRY_GEN_DIR,"berry")
    if IS_WINDOWS:
        berry_executable = join(BERRY_GEN_DIR,"berry.exe")
    else:
        if os.path.exists(BERRY_EXECUTABLE) == False:
            print("Will compile Berry executable")
            make_cmd = "make"
            subprocess.call(make_cmd, shell=False)
    
    if os.path.exists(BERRY_EXECUTABLE):
        return BERRY_EXECUTABLE
    else:
        return Null

def cleanFolder():
    with open(HEADER_FILE_PATH, 'w') as file:
        code = "#define CUSTOM_NATIVE_MODULES\n#define CUSTOM_NATIVE_CLASSES"
        file.write(code)
    tempfiles = [f for f in os.listdir(join(BERRY_SOLIDIFY_DIR,"src")) if re.match(r'_temp', f)]
    for file in tempfiles:
        os.remove(join(BERRY_SOLIDIFY_DIR,"src",file))
    tempfiles = [f for f in os.listdir(join(BERRY_SOLIDIFY_DIR,"src","embedded")) if ".gitignore" not in f]
    for file in tempfiles:
        if file != ".keep":
            os.remove(join(BERRY_SOLIDIFY_DIR,"src","embedded",file))
    tempfiles = [f for f in os.listdir(join(BERRY_SOLIDIFY_DIR,"src","solidify")) if ".gitignore" not in f]
    for file in tempfiles:
        if file != ".keep":
            os.remove(join(BERRY_SOLIDIFY_DIR,"src","solidify",file))
    

def addEntryToModtab(source): 
    code = source.decode("utf-8")
    class_name = None
    is_module = False


    pattern = (r'''(?<=module\()[^"].*''')  # module??
    result =  re.findall(pattern,code)
    if len(result) > 0:
        class_name = result[0].replace("'","").replace('"','').replace(")","")
        print(class_name+" is a module")
        is_module = True
    else: # just a class
        pattern = (r'(?<=#@ solidify:).*')
        result =  re.findall(pattern,code)
        if len(result) > 0:
            class_name = result[0].split(",")[0]
        if class_name == None:
            print("Could not find class name - is '#@ solidify:' used in Berry file??")
            print(Fore.RED + "Aborting build process!!")
            quit()
    MODTAB_PATH = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry","default","be_modtab.c")
    with open(HEADER_FILE_PATH, 'r') as file:
        code = file.read() # reuse code var for modtab file
    if is_module:
        nmodule = f"&be_native_module({class_name}),"
        if code.find(nmodule) == -1:
            code = code.replace(
                "CUSTOM_NATIVE_MODULES",
                f'CUSTOM_NATIVE_MODULES {nmodule}'
            )
        enmodule = f"be_extern_native_module({class_name});"
        if code.find(enmodule) == -1:
            code += f'\n{enmodule}'
    else:
        nclass = f"&be_native_class({class_name}),"
        if code.find(nclass) == -1:
            code = code.replace(
                "CUSTOM_NATIVE_CLASSES",
                f'CUSTOM_NATIVE_CLASSES {nclass}'
            )
        enclass = f"be_extern_native_class({class_name});"
        if code.find(enclass) == -1:
            code += f'\n{enclass}'

    with open(HEADER_FILE_PATH, 'w') as file:
        file.write(code)

 
def addHeaderFile(name):
    print("Will solidify ",name)
    name = name.split(".")[0]
    data = f"""
/********************************************************************
* {name} module
* 
*******************************************************************/
#include "solidify/solidified_{name}.h"
"""
    file_name = f"_temp_be_{name}_lib.c"
    file_path = join(BERRY_SOLIDIFY_DIR,"src",file_name)
    open(file_path,"w").write(data)

def prepareBerryFiles(files):
    embedded_dir = join("src","embedded")
    for file in files:
        if "http" and "://" in file:
            response = requests.get(file.split(" ")[0])
            if response.ok:
                target = join(embedded_dir,file.split(os.path.sep)[-1])
                if len(file.split(" ")) > 1:
                    target = join(embedded_dir,file.split(" ")[1])
                    print("Renaming",(file.split(os.path.sep)[-1]).split(" ")[0],"to",file.split(" ")[1])
                open(target, "wb").write(response.content)
                addHeaderFile(file.split(os.path.sep)[-1])
                addEntryToModtab(response.content)
            else:
                print(Fore.RED + "Failed to download: ",file)
            continue
        # maybe later ...
        # if os.path.isdir(file):
        #     continue
        # else:
        #     shutil.copy(file, embedded_dir)
    return True


BERRY_SOLIDIFY_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry_custom")
HEADER_FILE_PATH = join(BERRY_SOLIDIFY_DIR,"src","modules.h")
cleanFolder() # always clean up this folder
try:
    files = env.GetProjectOption("custom_berry_solidify")
except:
    print("Nothing more to solidify")
else:
    if env.IsCleanTarget() == False:
        BERRY_EXECUTABLE = ensureBerry()
        
        os.chdir(BERRY_SOLIDIFY_DIR)

        if prepareBerryFiles(files.splitlines()):
            solidify_command = BERRY_EXECUTABLE
            solidify_flags = " -s -g solidify_all.be"
            print("Start solidification for 'berry_custom':")
            subprocess.call(solidify_command + solidify_flags, shell=True)
