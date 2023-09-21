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
# - 0x10000 | ~\.platformio/packages/framework-arduinoespressif32/variants/tasmota/\<env name>-safeboot.bin
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
from SCons.Script import COMMAND_LINE_TARGETS

sys.path.append(join(platform.get_package_dir("tool-esptoolpy")))
import esptool

github_actions = os.getenv('GITHUB_ACTIONS')
extra_flags = ''.join([element.replace("-D", " ") for element in env.BoardConfig().get("build.extra_flags", "")])
build_flags = ''.join([element.replace("-D", " ") for element in env.GetProjectOption("build_flags")])

if "CORE32SOLO1" in extra_flags or "FRAMEWORK_ARDUINO_SOLO1" in build_flags:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-solo1")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduino-solo1/variants/tasmota")
elif "CORE32ITEAD" in extra_flags or "FRAMEWORK_ARDUINO_ITEAD" in build_flags:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduino-ITEAD")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduino-ITEAD/variants/tasmota")
else:
    FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoespressif32")
    if github_actions and os.path.exists("./firmware/firmware"):
        shutil.copytree("./firmware/firmware", "/home/runner/.platformio/packages/framework-arduinoespressif32/variants/tasmota")

variants_dir = join(FRAMEWORK_DIR, "variants", "tasmota")

def patch_partitions_bin(size_string):
    partition_bin_path = join(env.subst("$BUILD_DIR"),"partitions.bin")
    with open(partition_bin_path, 'r+b') as file:
        binary_data = file.read(0xb0)
        import hashlib
        bin_list = list(binary_data)
        size = codecs.decode(size_string[2:], 'hex_codec') # 0xc50000 -> [c5,00,00]
        bin_list[0x8a] = size[0]
        bin_list[0x8b] = size[1]
        result = hashlib.md5(bytes(bin_list[0:0xa0]))
        partition_data = bytes(bin_list) + result.digest()
        file.seek(0)
        file.write(partition_data)
        print("New partition hash:",result.digest().hex())

def esp32_detect_flashsize():
    if not "upload" in COMMAND_LINE_TARGETS:
        return "4MB",False
    size = env.get("TASMOTA_flash_size")
    if size == None:
        return "4MB",False
    else:
        return size,True

def esp32_create_chip_string(chip):
    tasmota_platform = env.subst("$BUILD_DIR").split(os.path.sep)[-1]
    tasmota_platform = tasmota_platform.split('-')[0]
    if 'tasmota' + chip[3:] not in tasmota_platform: # quick check for a valid name like 'tasmota' + '32c3'
        print('Unexpected naming conventions in this build environment -> Undefined behavior for further build process!!')
        print("Expected build environment name like 'tasmota32-whatever-you-want'")
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
                print("Failed to download: ",file)
            continue
        if os.path.isdir(file):
            shutil.copytree(file, filesystem_dir, dirs_exist_ok=True)
        else:
            shutil.copy(file, filesystem_dir)
    if not os.listdir(filesystem_dir):
        print("No files added -> will NOT create littlefs.bin and NOT overwrite fs partition!")
        return False
    env.Replace( MKSPIFFSTOOL=platform.get_package_dir("tool-mklittlefs") + '/mklittlefs' )
    tool = env.subst(env["MKSPIFFSTOOL"])
    cmd = (tool,"-c",filesystem_dir,"-s",fs_size,join(env.subst("$BUILD_DIR"),"littlefs.bin"))
    returncode = subprocess.call(cmd, shell=False)
    # print(returncode)
    return True

def esp32_fetch_safeboot_bin(tasmota_platform):
    safeboot_fw_url = "http://ota.tasmota.com/tasmota32/release/" + tasmota_platform + "-safeboot.bin"
    safeboot_fw_name = join(variants_dir, tasmota_platform + "-safeboot.bin")
    if(exists(safeboot_fw_name)):
        print("safeboot binary already in place.")
        return
    print("Will download safeboot binary from URL:")
    print(safeboot_fw_url)
    response = requests.get(safeboot_fw_url)
    open(safeboot_fw_name, "wb").write(response.content)
    print("safeboot binary written to variants dir.")

def esp32_copy_new_safeboot_bin(tasmota_platform,new_local_safeboot_fw):
    print("Copy new local safeboot firmware to variants dir -> using it for further flashing operations")
    safeboot_fw_name = join(variants_dir, tasmota_platform + "-safeboot.bin")
    if os.path.exists(variants_dir):
        shutil.copy(new_local_safeboot_fw, safeboot_fw_name)

def esp32_create_combined_bin(source, target, env):
    #print("Generating combined binary for serial flashing")

    # The offset from begin of the file where the app0 partition starts
    # This is defined in the partition .csv file
    # factory_offset = -1      # error code value - currently unused
    app_offset = 0x10000     # default value for "old" scheme
    fs_offset = -1           # error code value
    flash_size_from_esp, flash_size_was_overridden = esp32_detect_flashsize()

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
    sections = env.subst(env.get("FLASH_EXTRA_IMAGES"))
    firmware_name = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    chip = env.get("BOARD_MCU")
    tasmota_platform = esp32_create_chip_string(chip)

    if "-DUSE_USB_CDC_CONSOLE" in env.BoardConfig().get("build.extra_flags") and "cdc" not in tasmota_platform:
        tasmota_platform += "cdc"
        print("WARNING: board definition uses CDC configuration, but environment name does not -> changing tasmota safeboot binary to:", tasmota_platform + "-safeboot.bin")

    if not os.path.exists(variants_dir):
        os.makedirs(variants_dir)
    if("safeboot" in firmware_name):
        esp32_copy_new_safeboot_bin(tasmota_platform,firmware_name)
    else:
        esp32_fetch_safeboot_bin(tasmota_platform)

    flash_size = env.BoardConfig().get("upload.flash_size", "4MB")
    if flash_size_was_overridden:
        flash_size = flash_size_from_esp
    flash_freq = env.BoardConfig().get("build.f_flash", "40000000L")
    flash_freq = str(flash_freq).replace("L", "")
    flash_freq = str(int(int(flash_freq) / 1000000)) + "m"
    flash_mode = env.BoardConfig().get("build.flash_mode", "dio")
    memory_type = env.BoardConfig().get("build.arduino.memory_type", "qio_qspi")

    if flash_mode == "qio" or flash_mode == "qout":
        flash_mode = "dio"
    if memory_type == "opi_opi" or memory_type == "opi_qspi":
        flash_mode = "dout"
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
    if("safeboot" not in firmware_name):
        print(f" - {hex(app_offset)} | {firmware_name}")
        cmd += [hex(app_offset), firmware_name]

    else:
        print("Upload new safeboot binary only")

#    if(fs_offset != -1):
    upload_port = env.subst("$UPLOAD_PORT")
    if("upload-tasmota.php" not in upload_port) and (fs_offset != -1):
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

    # print('Using esptool.py arguments: %s' % ' '.join(cmd))

    esptool.main(cmd)


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", esp32_create_combined_bin)