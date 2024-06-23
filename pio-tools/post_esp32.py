# From: https://github.com/letscontrolit/ESPEasy/blob/mega/tools/pio/post_esp32.py
# Thanks TD-er :)

# Thanks @staars for safeboot and auto resizing LittleFS code and enhancements

# Combines separate bin files with their respective offsets into a single file
# This single file must then be flashed to an ESP32 node with 0 offset.
#
# Original implementation: Bartłomiej Zimoń (@uzi18)
#
# Special thanks to @Jason2866 for helping debug flashing to >4MB flash
# Thanks @jesserockz (esphome) for adapting to use esptool.py with merge_bin
#
# Typical layout of the generated file:
#    Offset | File
# -  0x1000 | ~\.platformio\packages\framework-arduinoespressif32\tools\sdk\esp32\bin\bootloader_dout_40m.bin
# -  0x8000 | ~\Tasmota\.pio\build\<env name>\partitions.bin
# -  0xe000 | ~\.platformio\packages\framework-arduinoespressif32\tools\partitions\boot_app0.bin
# - 0x10000 | ~\Tasmota\<variants_dir>/<env name>-safeboot.bin
# - 0xe0000 | ~\Tasmota\.pio\build\<env name>/firmware.bin
# - 0x3b0000| ~\Tasmota\.pio\build\<env name>/littlefs.bin

env = DefaultEnvironment()
platform = env.PioPlatform()

from genericpath import exists
import os
import sys
from os.path import join
import csv
import requests
import shutil
import subprocess
import codecs
from colorama import Fore, Back, Style
from SCons.Script import COMMAND_LINE_TARGETS

sys.path.append(join(platform.get_package_dir("tool-esptoolpy")))
import esptool

variants_dir = env.BoardConfig().get("build.variants_dir", "")
variant = env.BoardConfig().get("build.variant", "")
sections = env.subst(env.get("FLASH_EXTRA_IMAGES"))
chip = env.get("BOARD_MCU")
mcu_build_variant = env.BoardConfig().get("build.variant", "").lower()

# Copy safeboots firmwares in place when running in Github
github_actions = os.getenv('GITHUB_ACTIONS')
extra_flags = ''.join([element.replace("-D", " ") for element in env.BoardConfig().get("build.extra_flags", "")])
build_flags = ''.join([element.replace("-D", " ") for element in env.GetProjectOption("build_flags")])

if "CORE32SOLO1" in extra_flags or "FRAMEWORK_ARDUINO_SOLO1" in build_flags:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-solo1")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduino-solo1/variants/tasmota")
        if variants_dir:
            shutil.copytree("./firmware/firmware", variants_dir, dirs_exist_ok=True)
elif "CORE32ITEAD" in extra_flags or "FRAMEWORK_ARDUINO_ITEAD" in build_flags:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-ITEAD")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduino-ITEAD/variants/tasmota")
        if variants_dir:
            shutil.copytree("./firmware/firmware", variants_dir, dirs_exist_ok=True)
else:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoespressif32")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduinoespressif32/variants/tasmota")
        if variants_dir:
            shutil.copytree("./firmware/firmware", variants_dir, dirs_exist_ok=True)

# Copy pins_arduino.h to variants folder
if variants_dir:
    mcu_build_variant_path = join(FRAMEWORK_DIR, "variants", mcu_build_variant, "pins_arduino.h")
    custom_variant_build = join(env.subst("$PROJECT_DIR"), variants_dir , mcu_build_variant, "pins_arduino.h")
    os.makedirs(join(env.subst("$PROJECT_DIR"), variants_dir , mcu_build_variant), exist_ok=True)
    shutil.copy(mcu_build_variant_path, custom_variant_build)

if not variants_dir:
    variants_dir = join(FRAMEWORK_DIR, "variants", "tasmota")
    env.BoardConfig().update("build.variants_dir", variants_dir)

def esp32_detect_flashsize():
    uploader = env.subst("$UPLOADER")
    if not "upload" in COMMAND_LINE_TARGETS:
        return "4MB",False
    if not "esptool" in uploader:
        return "4MB",False
    else:
        esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
        esptoolpy_flags = ["flash_id"]
        esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
        try:
            output = subprocess.run(esptoolpy_cmd, capture_output=True).stdout.splitlines()
            for l in output:
                if l.decode().startswith("Detected flash size: "):
                    size = (l.decode().split(": ")[1])
                    print("Did get flash size:", size)
                    stored_flash_size_mb = env.BoardConfig().get("upload.flash_size")
                    stored_flash_size = int(stored_flash_size_mb.split("MB")[0]) * 0x100000
                    detected_flash_size = int(size.split("MB")[0]) * 0x100000
                    if detected_flash_size > stored_flash_size:
                        env.BoardConfig().update("upload.flash_size", size)
                        return size, True
            return "4MB",False
        except subprocess.CalledProcessError as exc:
            print(Fore.YELLOW + "Did get chip info failed with " + str(exc))
            return "4MB",False

flash_size_from_esp, flash_size_was_overridden = esp32_detect_flashsize()

def patch_partitions_bin(size_string):
    partition_bin_path = join(env.subst("$BUILD_DIR"),"partitions.bin")
    with open(partition_bin_path, 'r+b') as file:
        binary_data = file.read(0xb0)
        import hashlib
        bin_list = list(binary_data)
        size_string = int(size_string[2:],16)
        size_string = f"{size_string:08X}"
        size = codecs.decode(size_string, 'hex_codec') # 0xc50000 -> [00,c5,00,00]
        bin_list[0x89] = size[2]
        bin_list[0x8a] = size[1]
        bin_list[0x8b] = size[0]
        result = hashlib.md5(bytes(bin_list[0:0xa0]))
        partition_data = bytes(bin_list) + result.digest()
        file.seek(0)
        file.write(partition_data)
        print("New partition hash:",result.digest().hex())

def esp32_create_chip_string(chip):
    tasmota_platform_org = env.subst("$BUILD_DIR").split(os.path.sep)[-1]
    tasmota_platform = tasmota_platform_org.split('-')[0]
    if ("CORE32SOLO1" in extra_flags or "FRAMEWORK_ARDUINO_SOLO1" in build_flags) and "tasmota32-safeboot" not in tasmota_platform_org and "tasmota32solo1" not in tasmota_platform_org:
        print(Fore.YELLOW + "Unexpected naming convention in this build environment:" + Fore.RED, tasmota_platform_org)
        print(Fore.YELLOW + "Expected build environment name like " + Fore.GREEN + "'tasmota32solo1-whatever-you-want'")
        print(Fore.YELLOW + "Please correct your actual build environment, to avoid undefined behavior in build process!!")
        tasmota_platform = "tasmota32solo1"
        return tasmota_platform
    if "tasmota" + chip[3:] not in tasmota_platform: # check + fix for a valid name like 'tasmota' + '32c3'
        tasmota_platform = "tasmota" + chip[3:]
        if "-DUSE_USB_CDC_CONSOLE" not in env.BoardConfig().get("build.extra_flags"):
            print(Fore.YELLOW + "Unexpected naming convention in this build environment:" + Fore.RED, tasmota_platform_org)
            print(Fore.YELLOW + "Expected build environment name like " + Fore.GREEN + "'tasmota" + chip[3:] + "-whatever-you-want'")
            print(Fore.YELLOW + "Please correct your actual build environment, to avoid undefined behavior in build process!!")
    return tasmota_platform

def esp32_build_filesystem(fs_size):
    files = env.GetProjectOption("custom_files_upload").splitlines()
    filesystem_dir = join(env.subst("$BUILD_DIR"),"littlefs_data")
    if not os.path.exists(filesystem_dir):
        os.makedirs(filesystem_dir)
    print("Creating filesystem with content:")
    for file in files:
        if "no_files" in file:
            continue
        if "http" and "://" in file:
            response = requests.get(file.split(" ")[0])
            if response.ok:
                target = join(filesystem_dir,file.split(os.path.sep)[-1])
                if len(file.split(" ")) > 1:
                    target = join(filesystem_dir,file.split(" ")[1])
                    print("Renaming",(file.split(os.path.sep)[-1]).split(" ")[0],"to",file.split(" ")[1])
                open(target, "wb").write(response.content)
            else:
                print(Fore.RED + "Failed to download: ",file)
            continue
        if os.path.isdir(file):
            shutil.copytree(file, filesystem_dir, dirs_exist_ok=True)
        else:
            shutil.copy(file, filesystem_dir)
    if not os.listdir(filesystem_dir):
        print("No files added -> will NOT create littlefs.bin and NOT overwrite fs partition!")
        return False
    tool = env.subst(env["MKFSTOOL"])
    cmd = (tool,"-c",filesystem_dir,"-s",fs_size,join(env.subst("$BUILD_DIR"),"littlefs.bin"))
    returncode = subprocess.call(cmd, shell=False)
    # print(returncode)
    return True

def esp32_fetch_safeboot_bin(tasmota_platform):
    safeboot_fw_url = "http://ota.tasmota.com/tasmota32/release/" + tasmota_platform + "-safeboot.bin"
    safeboot_fw_name = join(variants_dir, tasmota_platform + "-safeboot.bin")
    if(exists(safeboot_fw_name)):
        print("safeboot binary already in place.")
        return True
    print("Will download safeboot binary from URL:")
    print(safeboot_fw_url)
    try:
        response = requests.get(safeboot_fw_url)
        open(safeboot_fw_name, "wb").write(response.content)
        print("safeboot binary written to variants dir.")
        return True
    except:
        print(Fore.RED + "Download of safeboot binary failed. Please check your Internet connection.")
        print(Fore.RED + "Creation of " + tasmota_platform + "-factory.bin not possible")
        print(Fore.YELLOW + "Without Internet " + Fore.GREEN + tasmota_platform + "-safeboot.bin" + Fore.YELLOW + " needs to be compiled before " + Fore.GREEN + tasmota_platform)
        return False

def esp32_copy_new_safeboot_bin(tasmota_platform,new_local_safeboot_fw):
    print("Copy new local safeboot firmware to variants dir -> using it for further flashing operations")
    safeboot_fw_name = join(variants_dir, tasmota_platform + "-safeboot.bin")
    if os.path.exists(variants_dir):
        try:
            shutil.copy(new_local_safeboot_fw, safeboot_fw_name)
            return True
        except:
            return False


def esp32_create_combined_bin(source, target, env):
    #print("Generating combined binary for serial flashing")
    # The offset from begin of the file where the app0 partition starts
    # This is defined in the partition .csv file
    # factory_offset = -1      # error code value - currently unused
    app_offset = 0x10000     # default value for "old" scheme
    fs_offset = -1           # error code value

    with open(env.BoardConfig().get("build.partitions")) as csv_file:
        print("Read partitions from ",env.BoardConfig().get("build.partitions"))
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if line_count == 0:
                print(f'{",  ".join(row)}')
                line_count += 1
            else:
                print(f'{row[0]}   {row[1]}   {row[2]}   {row[3]}   {row[4]}')
                line_count += 1
                if(row[0] == 'app0'):
                    app_offset = int(row[3],base=16)
                # elif(row[0] == 'factory'):
                #     factory_offset = int(row[3],base=16)
                elif(row[0] == 'spiffs'):
                    partition_size = row[4]
                    if flash_size_was_overridden:
                        print(f"Will override fixed FS partition size from {env.BoardConfig().get('build.partitions')}: {partition_size} ...")
                        partition_size =  hex(int(flash_size_from_esp.split("MB")[0]) * 0x100000 - int(row[3],base=16))
                        print(f"... with computed maximum size from connected {env.get('BOARD_MCU')}: {partition_size}")
                        patch_partitions_bin(partition_size)
                    if esp32_build_filesystem(partition_size):
                        fs_offset = int(row[3],base=16)


    new_file_name = env.subst("$BUILD_DIR/${PROGNAME}.factory.bin")
    firmware_name = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    tasmota_platform = esp32_create_chip_string(chip)

    if not os.path.exists(variants_dir):
        os.makedirs(variants_dir)
    if "safeboot" in firmware_name:
        s_flag = esp32_copy_new_safeboot_bin(tasmota_platform,firmware_name)
    else:
        s_flag = esp32_fetch_safeboot_bin(tasmota_platform)

    if s_flag:  # check if safeboot firmware is existing
        flash_size = env.BoardConfig().get("upload.flash_size", "4MB")
        flash_mode = env["__get_board_flash_mode"](env)
        flash_freq = env["__get_board_f_flash"](env)

        cmd = [
            "--chip",
            chip,
            "merge_bin",
            "-o",
            new_file_name,
            "--flash_mode",
            flash_mode,
            "--flash_freq",
            flash_freq,
            "--flash_size",
            flash_size,
        ]

        print("    Offset | File")
        for section in sections:
            sect_adr, sect_file = section.split(" ", 1)
            print(f" -  {sect_adr} | {sect_file}")
            cmd += [sect_adr, sect_file]

        # "main" firmware to app0 - mandatory, except we just built a new safeboot bin locally
        if ("safeboot" not in firmware_name):
            print(f" - {hex(app_offset)} | {firmware_name}")
            cmd += [hex(app_offset), firmware_name]

        else:
            print("Upload new safeboot binary only")

        upload_protocol = env.subst("$UPLOAD_PROTOCOL")
        if(upload_protocol == "esptool") and (fs_offset != -1):
            fs_bin = join(env.subst("$BUILD_DIR"),"littlefs.bin")
            if exists(fs_bin):
                before_reset = env.BoardConfig().get("upload.before_reset", "default_reset")
                after_reset = env.BoardConfig().get("upload.after_reset", "hard_reset")
                print(f" - {hex(fs_offset)}| {fs_bin}")
                cmd += [hex(fs_offset), fs_bin]
                env.Replace(
                UPLOADERFLAGS=[
                "--chip", chip,
                "--port", '"$UPLOAD_PORT"',
                "--baud", "$UPLOAD_SPEED",
                "--before", before_reset,
                "--after", after_reset,
                "write_flash", "-z",
                "--flash_mode", "${__get_board_flash_mode(__env__)}",
                "--flash_freq", "${__get_board_f_flash(__env__)}",
                "--flash_size", flash_size
                ],
                UPLOADCMD='"$PYTHONEXE" "$UPLOADER" $UPLOADERFLAGS ' + " ".join(cmd[7:])
                )
                print("Will use custom upload command for flashing operation to add file system defined for this build target.")

        if("safeboot" not in firmware_name):
            #print('Using esptool.py arguments: %s' % ' '.join(cmd))
            esptool.main(cmd)


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", esp32_create_combined_bin)
