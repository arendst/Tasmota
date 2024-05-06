# Written by Maximilian Gerhardt <maximilian.gerhardt@rub.de>
# 29th December 2020
# and Christian Baars, Johann Obermeier
# 2023 / 2024
# License: Apache
# Expanded from functionality provided by PlatformIO's espressif32 and espressif8266 platforms, credited below.
# This script provides functions to download the filesystem (LittleFS) from a running ESP32 / ESP8266
# over the serial bootloader using esptool.py, and mklittlefs for extracting.
# run by either using the VSCode task "Custom" -> "Download Filesystem"
# or by doing 'pio run -t downloadfs' (with optional '-e <environment>') from the commandline.
# output will be saved, by default, in the "unpacked_fs" of the project.
# this folder can be changed by writing 'custom_unpack_dir = some_other_dir' in the corresponding platformio.ini
# environment.
import re
import sys
from os.path import isfile, join
from enum import Enum
import os
import tasmotapiolib
import subprocess
import shutil
import json
from colorama import Fore, Back, Style

Import("env")
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = board.get("build.mcu", "esp32")
IS_WINDOWS = sys.platform.startswith("win")


class FSType(Enum):
    LITTLEFS="littlefs"
    FATFS="fatfs"

class FSInfo:
    def __init__(self, fs_type, start, length, page_size, block_size):
        self.fs_type = fs_type
        self.start = start
        self.length = length
        self.page_size = page_size
        self.block_size = block_size
    def __repr__(self):
        return f"FS type {self.fs_type} Start {hex(self.start)} Len {self.length} Page size {self.page_size} Block size {self.block_size}"
    # extract command supposed to be implemented by subclasses
    def get_extract_cmd(self, input_file, output_dir):
        raise NotImplementedError()

class FS_Info(FSInfo):
    def __init__(self, start, length, page_size, block_size):
        self.tool = env["MKFSTOOL"]
        self.tool = join(platform.get_package_dir("tool-mklittlefs"), self.tool)
        super().__init__(FSType.LITTLEFS, start, length, page_size, block_size)
    def __repr__(self):
        return f"{self.fs_type} Start {hex(self.start)} Len {hex(self.length)} Page size {hex(self.page_size)} Block size {hex(self.block_size)}"
    def get_extract_cmd(self, input_file, output_dir):
        return f'"{self.tool}" -b {self.block_size} -s {self.length} -p {self.page_size} --unpack "{output_dir}" "{input_file}"'

# SPIFFS helpers copied from ESP32, https://github.com/platformio/platform-espressif32/blob/develop/builder/main.py
# Copyright 2014-present PlatformIO <contact@platformio.org>
# Licensed under the Apache License, Version 2.0 (the "License");

def _parse_size(value):
    if isinstance(value, int):
        return value
    elif value.isdigit():
        return int(value)
    elif value.startswith("0x"):
        return int(value, 16)
    elif value[-1].upper() in ("K", "M"):
        base = 1024 if value[-1].upper() == "K" else 1024 * 1024
        return int(value[:-1]) * base
    return value

## FS helpers for ESP8266
# copied from https://github.com/platformio/platform-espressif8266/blob/develop/builder/main.py
# Copyright 2014-present PlatformIO <contact@platformio.org>
# Licensed under the Apache License, Version 2.0 (the "License");

def _parse_ld_sizes(ldscript_path):
    assert ldscript_path
    result = {}
    # get flash size from LD script path
    match = re.search(r"\.flash\.(\d+[mk]).*\.ld", ldscript_path)
    if match:
        result['flash_size'] = _parse_size(match.group(1))

    appsize_re = re.compile(
        r"irom0_0_seg\s*:.+len\s*=\s*(0x[\da-f]+)", flags=re.I)
    filesystem_re = re.compile(
        r"PROVIDE\s*\(\s*_%s_(\w+)\s*=\s*(0x[\da-f]+)\s*\)" % "FS"
        if "arduino" in env.subst("$PIOFRAMEWORK")
        else "SPIFFS",
        flags=re.I,
    )
    with open(ldscript_path) as fp:
        for line in fp.readlines():
            line = line.strip()
            if not line or line.startswith("/*"):
                continue
            match = appsize_re.search(line)
            if match:
                result['app_size'] = _parse_size(match.group(1))
                continue
            match = filesystem_re.search(line)
            if match:
                result['fs_%s' % match.group(1)] = _parse_size(
                    match.group(2))
    return result

def esp8266_fetch_fs_size(env):
    ldsizes = _parse_ld_sizes(env.GetActualLDScript())
    for key in ldsizes:
        if key.startswith("fs_"):
            env[key.upper()] = ldsizes[key]

    assert all([
        k in env
        for k in ["FS_START", "FS_END", "FS_PAGE", "FS_BLOCK"]
    ])

    # esptool flash starts from 0
    for k in ("FS_START", "FS_END"):
        _value = 0
        if env[k] < 0x40300000:
            _value = env[k] & 0xFFFFF
        elif env[k] < 0x411FB000:
            _value = env[k] & 0xFFFFFF
            _value -= 0x200000  # correction
        else:
            _value = env[k] & 0xFFFFFF
            _value += 0xE00000  # correction

        env[k] = _value

## Script interface functions
def parse_partition_table(content):
    entries = [e for e in content.split(b'\xaaP') if len(e) > 0]
    #print("Partition data:")
    for entry in entries:
        type = entry[1]
        if type in [0x82,0x83]: # SPIFFS or LITTLEFS
            offset = int.from_bytes(entry[2:5], byteorder='little', signed=False)
            size = int.from_bytes(entry[6:9], byteorder='little', signed=False)
            #print("type:",hex(type))
            #print("address:",hex(offset))
            #print("size:",hex(size))
            env["FS_START"] = offset
            env["FS_SIZE"] = size
            env["FS_PAGE"] = int("0x100", 16)
            env["FS_BLOCK"] = int("0x1000", 16)

def get_partition_table():
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    download_speed = join(str(board.get("download.speed", "115200")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    fs_file = join(env["PROJECT_DIR"], "partition_table_from_flash.bin")
    esptoolpy_flags = [
            "--chip", mcu,
            "--port", upload_port,
            "--baud",  download_speed,
            "--before", "default_reset",
            "--after", "hard_reset",
            "read_flash",
            "0x8000",
            "0x1000",
            fs_file
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    try:
        returncode = subprocess.call(esptoolpy_cmd, shell=False)
    except subprocess.CalledProcessError as exc:
        print("Downloading failed with " + str(exc))
    with open(fs_file, mode="rb") as file:
        content = file.read()
        parse_partition_table(content)

def get_fs_type_start_and_length():
    platform = env["PIOPLATFORM"]
    if platform == "espressif32":
        print(f"Retrieving filesystem info for {mcu}.")
        get_partition_table()
        return FS_Info(env["FS_START"], env["FS_SIZE"], env["FS_PAGE"], env["FS_BLOCK"])
    elif platform == "espressif8266":
        print("Retrieving filesystem info for ESP8266.")
        filesystem = board.get("build.filesystem", "littlefs")
        if filesystem not in ("littlefs"):
            print("Unrecognized board_build.filesystem option '" + str(filesystem) + "'.")
            env.Exit(1)
        # fetching sizes is the same for all filesystems
        esp8266_fetch_fs_size(env)
        #print("FS_START: " + hex(env["FS_START"]))
        #print("FS_SIZE: " + hex(env["FS_END"] - env["FS_START"]))
        #print("FS_PAGE: " + hex(env["FS_PAGE"]))
        #print("FS_BLOCK: " + hex(env["FS_BLOCK"]))
        if filesystem == "littlefs":
            print("Recognized LittleFS filesystem.")
            return FS_Info(env["FS_START"], env["FS_END"] - env["FS_START"], env["FS_PAGE"], env["FS_BLOCK"])
        else:
            print("Unrecongized configuration.")
    pass

def download_fs(fs_info: FSInfo):
    print(fs_info)
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    download_speed = join(str(board.get("download.speed", "115200")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    fs_file = join(env.subst("$BUILD_DIR"), f"downloaded_fs_{hex(fs_info.start)}_{hex(fs_info.length)}.bin")
    esptoolpy_flags = [
            "--chip", mcu,
            "--port", upload_port,
            "--baud",  download_speed,
            "--before", "default_reset",
            "--after", "hard_reset",
            "read_flash",
            hex(fs_info.start),
            hex(fs_info.length),
            fs_file
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    print("Download filesystem image")
    try:
        returncode = subprocess.call(esptoolpy_cmd, shell=False)
        return (True, fs_file)
    except subprocess.CalledProcessError as exc:
        print("Downloading failed with " + str(exc))
        return (False, "")

def unpack_fs(fs_info: FSInfo, downloaded_file: str):
    # by writing custom_unpack_dir = some_dir in the platformio.ini, one can
    # control the unpack directory
    unpack_dir = env.GetProjectOption("custom_unpack_dir", "unpacked_fs")
    if not os.path.exists(downloaded_file):
        print(f"ERROR: {downloaded_file} with filesystem not found, maybe download failed due to download_speed setting being too high.")
        assert(0)
    try:
        if os.path.exists(unpack_dir):
            shutil.rmtree(unpack_dir)
    except Exception as exc:
        print("Exception while attempting to remove the folder '" + str(unpack_dir) + "': " + str(exc))
    if not os.path.exists(unpack_dir):
        os.makedirs(unpack_dir)

    cmd = fs_info.get_extract_cmd(downloaded_file, unpack_dir)
    print("Unpack files from filesystem image")
    try:
        returncode = subprocess.call(cmd, shell=True)
        return (True, unpack_dir)
    except subprocess.CalledProcessError as exc:
        print("Unpacking filesystem failed with " + str(exc))
        return (False, "")

def display_fs(extracted_dir):
    # extract command already nicely lists all extracted files.
    # no need to display that ourselves. just display a summary
    file_count = sum([len(files) for r, d, files in os.walk(extracted_dir)])
    print("Extracted " + str(file_count) + " file(s) from filesystem.")

def command_download_fs(*args, **kwargs):
    info = get_fs_type_start_and_length()
    download_ok, downloaded_file = download_fs(info)
    unpack_ok, unpacked_dir = unpack_fs(info, downloaded_file)
    if unpack_ok is True:
        display_fs(unpacked_dir)

def upload_factory(*args, **kwargs):
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_speed = join(str(board.get("upload.speed", "115200")))
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    target_firm = join(env.subst("$PROJECT_DIR"),tasmotapiolib.get_final_bin_path(env).with_suffix(".bin" if mcu == "esp8266" else (".factory.bin")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    if "tasmota" in target_firm:
        esptoolpy_flags = [
                "--chip", mcu,
                "--port", upload_port,
                "--baud", env.subst("$UPLOAD_SPEED"),
                "write_flash",
                "0x0",
                target_firm
        ]
        esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
        print("Flash firmware at address 0x0")
        subprocess.call(esptoolpy_cmd, shell=False)

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
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    esptoolpy_flags = [
        "--no-stub",
        "--chip", mcu,
        "--port", upload_port,
        "flash_id"
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    print("Try to reset device")
    subprocess.call(esptoolpy_cmd, shell=False)


env.AddCustomTarget(
    name="reset_target",
    dependencies=None,
    actions=[
        reset_target
    ],
    title="Reset ESP32 target",
    description="This command resets ESP32x target via esptoolpy",
)


env.AddCustomTarget(
    name="downloadfs",
    dependencies=None,
    actions=[
        command_download_fs
    ],
    title="Download Filesystem",
    description="Downloads and displays files stored in the target ESP32/ESP8266"
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
