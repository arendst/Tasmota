import os
import sys
from os.path import isfile, join
import tasmotapiolib
import subprocess
import json
from colorama import Fore
from platformio.compat import IS_WINDOWS

Import("env")
platform = env["PIOPLATFORM"]
board = env.BoardConfig()
mcu = board.get("build.mcu", "esp32").lower()


def switch_off_ldf():
    """
    Configure `lib_ldf_mode = off` for pre-script execution.
    to avoid the time consuming library dependency resolution
    """
    
    # only do this if one of the optimized targets is requested
    optimized_targets = ["reset_target", "factory_flash"]

    argv_string = " ".join(sys.argv)
    is_optimized_targets = any(target in argv_string for target in optimized_targets)
    
    if is_optimized_targets:
        # Project config modification
        projectconfig = env.GetProjectConfig()
        env_section = "env:" + env["PIOENV"]
        if not projectconfig.has_section(env_section):
            projectconfig.add_section(env_section)
        projectconfig.set(env_section, "lib_ldf_mode", "off")

switch_off_ldf()

def upload_factory(*args, **kwargs):
    upload_speed = join(str(board.get("upload.speed", "115200")))
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    target_firm = join(env.subst("$PROJECT_DIR"),tasmotapiolib.get_final_bin_path(env).with_suffix(".bin" if mcu == "esp8266" else (".factory.bin")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    if "tasmota" in target_firm:
        esptool_flags = [
                "--chip", mcu,
                "--port", upload_port,
                "--baud", env.subst("$UPLOAD_SPEED"),
                "write-flash",
                "0x0",
                target_firm
        ]
        ESPTOOL_EXE = env.get("ERASETOOL") if platform == "espressif8266" else env.get("OBJCOPY")
        esptool_cmd = [ESPTOOL_EXE] + esptool_flags
        print("Flash firmware at address 0x0")
        subprocess.call(esptool_cmd, shell=False)

def esp32_use_external_crashreport(*args, **kwargs):
    try:
        crash_report = env.GetProjectOption("custom_crash_report")
    except:
        print(Fore.RED + "Did not find custom_crash_report section in the current environment!!")
        return
    try:
        crash_report = json.loads(crash_report)
    except:
        print(Fore.RED + "No valid JSON, please use output of STATUS 12 in the console!!")
        return
    print(Fore.GREEN + "Use external crash report (STATUS 12) for debugging:\n", json.dumps(crash_report, sort_keys=True, indent=4))
    epc = crash_report['StatusSTK']['EPC']
    callchain = crash_report['StatusSTK']['CallChain']
    addr2line = ""
    for p in platform.get_installed_packages():
        if "toolchain" in p.path:
            files = os.listdir(join(p.path,"bin"))
            for f in files:
                if "addr2line" in f:
                    addr2line = join(p.path,"bin",f)
    elf_file = join(env.subst("$BUILD_DIR"),env.subst("${PROGNAME}.elf"))
    if isfile(elf_file) is False:
        print(Fore.RED+"Did not find firmware.elf ... please build the current environment first!!")
        return
    enc = "mbcs" if IS_WINDOWS else "utf-8"
    output = (
    subprocess.check_output([addr2line,"-e",elf_file,"-fC","-a",epc])
    .decode(enc)
    .strip()
    .splitlines()
    )
    print(Fore.YELLOW + "There is no way to check, if this data is valid for the given firmware!!")
    print(Fore.GREEN + "Crash at:")
    print(Fore.YELLOW + output[0] + ": \n" + output[1] + " in " + output[2])
    print(Fore.GREEN + "Callchain:")
    for call in callchain:
        output = (
        subprocess.check_output([addr2line,"-e",elf_file,"-fC","-a",call])
        .decode(enc)
        .strip()
        .splitlines()
        )
        print(Fore.YELLOW + output[0]+": \n"+output[1]+" in "+output[2])

def reset_target(*args, **kwargs):
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    esptool_flags = [
        "--no-stub",
        "--chip", mcu,
        "--port", upload_port,
        "flash-id"
    ]
    ESPTOOL_EXE = env.get("ERASETOOL") if platform == "espressif8266" else env.get("OBJCOPY")
    esptool_cmd = [ESPTOOL_EXE] + esptool_flags
    print("Try to reset device")
    subprocess.call(esptool_cmd, shell=False)

# Custom Target Definitions
env.AddCustomTarget(
    name="reset_target",
    dependencies=None,
    actions=[
        reset_target
    ],
    title="Reset connected device",
    description="This command resets the connected device via esptool",
)

env.AddCustomTarget(
    name="factory_flash",
    dependencies=None,
    actions=[
        upload_factory
    ],
    title="Flash factory",
    description="Flash factory firmware"
)

env.AddCustomTarget(
    name="external_crashreport",
    dependencies=None,
    actions=[
        esp32_use_external_crashreport
    ],
    title="External crash report",
    description="Use external crashreport from Tasmotas console output of STATUS 12"
)
