# Little convenience script to solidify external berry files as embedded

Import("env")

import os
from genericpath import exists
from os.path import join
import subprocess
from colorama import Fore, Back, Style
import requests
import re

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


    pattern = (r'''(?<=module\([\"\']).*[\"\']''')  # module??
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
    with open(file_path,"w") as f:
        f.write(data)

def prepareBerryFiles(files):
    embedded_dir = join("src","embedded")
    for file in files:
        # Remove leading and trailing whitespace, ignore empty lines
        file = file.strip()
        if not file:
            continue

        # Remote URL
        if file.startswith(("http://", "https://")):
            parts = file.split(" ")
            url, alias = parts[0], parts[1] if len(parts) > 1 else None
            url_basename = url.split("/")[-1]
            effective_name = alias or url_basename
            response = requests.get(url)
            if response.ok:
                if alias: print("Renaming", url_basename, "to", alias)
                with open(join(embedded_dir, effective_name), "wb") as f:
                    f.write(response.content)
                addHeaderFile(effective_name)
                addEntryToModtab(response.content)
            else:
                print(Fore.RED + "Failed to download: ", file)
            continue

        # Local path (relative to PROJECT_DIR or absolute)
        src_path = file if os.path.isabs(file) else join(env.subst("$PROJECT_DIR"), file)
        if not os.path.isfile(src_path):
            print(Fore.RED + "File not found: ", src_path)
            continue
        with open(src_path, 'rb') as f:
            source = f.read()
        with open(join(embedded_dir, os.path.basename(src_path)), "wb") as f:
            f.write(source)
        addHeaderFile(os.path.basename(src_path))
        addEntryToModtab(source)

    return True


BERRY_SOLIDIFY_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32","berry_custom")
HEADER_FILE_PATH = join(BERRY_SOLIDIFY_DIR,"src","modules.h")
cleanFolder() # always clean up this folder
try:
    files = env.GetProjectOption("custom_berry_solidify")
except:
    pass  # no custom Berry files to solidify - common case, no need to log
else:
    if env.IsCleanTarget() == False:
        os.chdir(BERRY_SOLIDIFY_DIR)

        # tasmota_defines_for_berry.be is generated post-compilation; create an empty
        # stub if it doesn't exist yet so solidify_all_python.be can import it safely.
        defines_file = join(env.subst("$PROJECT_DIR"), "tasmota", "tasmota_defines_for_berry.be")
        if not os.path.exists(defines_file):
            open(defines_file, 'w').close()

        if prepareBerryFiles(files.splitlines()):
            BERRY_GEN_DIR = join(env.subst("$PROJECT_DIR"), "lib", "libesp32", "berry")
            solidify_env = os.environ.copy()
            existing_pp = solidify_env.get("PYTHONPATH", "")
            solidify_env["PYTHONPATH"] = (
                BERRY_GEN_DIR + (os.pathsep + existing_pp if existing_pp else "")
            )
            solidify_env["PYTHONUTF8"] = "1"
            print("Start solidification for 'berry_custom':")
            proc = subprocess.Popen(
                (env["PYTHONEXE"], "-m", "berry_port", "-s", "-g", "solidify_all_python.be"),
                shell=False,
                env=solidify_env,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
            )
            other_lines = []
            for raw in proc.stdout:
                line = raw.rstrip("\r\n")
                stripped = line.strip()
                if stripped.startswith("Parsing:"):
                    continue
                if stripped.startswith("Skipping:"):
                    continue
                if stripped.startswith("# Output directory"):
                    continue
                other_lines.append(line)
            rc = proc.wait()
            for line in other_lines:
                if line:
                    print(line)
            if rc != 0:
                print(Fore.RED + f"ERROR: solidification failed (rc={rc})")
                env.Exit(rc)
