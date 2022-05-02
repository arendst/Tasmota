# From: https://github.com/letscontrolit/ESPEasy/blob/mega/tools/pio/post_esp32.py
# Thanks TD-er :)

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
# - 0x10000 | ~\Tasmota\.pio\build\<env name>/firmware.bin

Import("env")

env = DefaultEnvironment()
platform = env.PioPlatform()

from genericpath import exists
import os
import sys
from os.path import join
import csv
import requests
import shutil

sys.path.append(join(platform.get_package_dir("tool-esptoolpy")))
import esptool

FRAMEWORK_DIR = platform.get_package_dir("framework-arduinoespressif32")
safemode_dir = join(env["PROJECT_DIR"], "safemode")
variants_dir = join(FRAMEWORK_DIR, "variants", "tasmota")


def esp32_fetch_safemode_bin(chip):
    safemode_fw_url = "https://github.com/Jason2866/Tasmota-specials/raw/firmware/firmware/tasmota32/other/tasmota" + chip[3:] + "-safemode.bin"
    safemode_fw_name = safemode_dir + "/tasmota" + chip[3:] + "-safemode.bin"
    if(exists(safemode_fw_name)):
        print("Safemode binary already downloaded.")
        return safemode_fw_name
    print("Will download safemode binary from URL:")
    print(safemode_fw_url)
    response = requests.get(safemode_fw_url)
    open(safemode_fw_name, "wb").write(response.content)
    print("Safemode binary written to safemode dir.")
    return safemode_fw_name


def esp32_create_combined_bin(source, target, env):
    #print("Generating combined binary for serial flashing")

    # The offset from begin of the file where the app0 partition starts
    # This is defined in the partition .csv file
    factory_offset = -1      # error code value
    app_offset = 0x10000     # default value for "old" scheme
    spiffs_offset = -1       # error code value
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
                elif(row[0] == 'factory'):
                    factory_offset = int(row[3],base=16)
                elif(row[0] == 'spiffs'):
                    spiffs_offset = int(row[3],base=16)
#                    print("Got app_offset from .csv:", row[3])

    new_file_name = env.subst("$BUILD_DIR/${PROGNAME}.factory.bin")
    sections = env.subst(env.get("FLASH_EXTRA_IMAGES"))
    firmware_name = env.subst("$BUILD_DIR/${PROGNAME}.bin")
    chip = env.get("BOARD_MCU")
    esp32_fetch_safemode_bin(chip)
    flash_size = env.BoardConfig().get("upload.flash_size")
    cmd = [
        "--chip",
        chip,
        "merge_bin",
        "-o",
        new_file_name,
        "--flash_size",
        flash_size,
    ]

    print("    Offset | File")
    for section in sections:
        sect_adr, sect_file = section.split(" ", 1)
        print(f" -  {sect_adr} | {sect_file}")
        cmd += [sect_adr, sect_file]

    if os.path.exists(safemode_dir):
        #print("safemode.bin dir exists")
        if os.path.exists(variants_dir):
            #print("variants/tasmota exists")
            shutil.rmtree(variants_dir)
        shutil.copytree(safemode_dir, variants_dir)

    # "main" firmware to app0 - mandatory
    print(f" - {hex(app_offset)} | {firmware_name}")
    cmd += [hex(app_offset), firmware_name]

    #print('Using esptool.py arguments: %s' % ' '.join(cmd))

    esptool.main(cmd)


env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", esp32_create_combined_bin)
