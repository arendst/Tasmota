# Written by Maximilian Gerhardt <maximilian.gerhardt@rub.de>
# 29th December 2020
# License: Apache
# Expanded from functionality provided by PlatformIO's espressif32 and espressif8266 platforms, credited below.
# This script provides functions to download the filesystem (SPIFFS or LittleFS) from a running ESP32 / ESP8266
# over the serial bootloader using esptool.py, and mklittlefs / mkspiffs for extracting.
# run by either using the VSCode task "Custom" -> "Download Filesystem"
# or by doing 'pio run -t downloadfs' (with optional '-e <environment>') from the commandline.
# output will be saved, by default, in the "unpacked_fs" of the project.
# this folder can be changed by writing 'custom_unpack_dir = some_other_dir' in the corresponding platformio.ini
# environment.
import re
import sys
from os.path import isfile, join
from enum import Enum
import typing
from platformio.builder.tools.pioupload import AutodetectUploadPort
import os
import subprocess
import shutil

Import("env")
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = board.get("build.mcu", "esp32")
# Hack for using mklittlefs instead of mkspiffs -> needed since littlefs is not supported with this for ESP32
if env["PIOPLATFORM"] == "espressif32":
    #print("Replace MKSPIFFSTOOL with mklittlefs")
    env.Replace( MKSPIFFSTOOL=platform.get_package_dir("tool-mklittlefs") + '/mklittlefs' )

# needed for later
AutodetectUploadPort(env)

class FSType(Enum):
    SPIFFS="spiffs"
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

class LittleFSInfo(FSInfo):
    def __init__(self, start, length, page_size, block_size):
        if env["PIOPLATFORM"] == "espressif32":
            #for ESP32: retrieve and evaluate, e.g. to mkspiffs_espressif32_arduino
            self.tool = env.subst(env["MKSPIFFSTOOL"])
        else:
            self.tool = env["MKFSTOOL"] # from mkspiffs package
        self.tool = join(platform.get_package_dir("tool-mklittlefs"), self.tool)
        super().__init__(FSType.LITTLEFS, start, length, page_size, block_size)
    def __repr__(self):
        return f"FS type {self.fs_type} Start {hex(self.start)} Len {self.length} Page size {self.page_size} Block size {self.block_size} Tool: {self.tool}"
    def get_extract_cmd(self, input_file, output_dir):
        return [self.tool, "-b", str(self.block_size), "-p", str(self.page_size), "--unpack", output_dir, input_file]


class SPIFFSInfo(FSInfo):
    def __init__(self, start, length, page_size, block_size):
        if env["PIOPLATFORM"] == "espressif32":
            #for ESP32: retrieve and evaluate, e.g. to mkspiffs_espressif32_arduino
            self.tool = env.subst(env["MKSPIFFSTOOL"])
        else:
            self.tool = env["MKFSTOOL"] # from mkspiffs package
        self.tool = join(platform.get_package_dir("tool-mkspiffs"), self.tool)
        super().__init__(FSType.SPIFFS, start, length, page_size, block_size)
    def __repr__(self):
        return f"FS type {self.fs_type} Start {hex(self.start)} Len {self.length} Page size {self.page_size} Block size {self.block_size} Tool: {self.tool}"
    def get_extract_cmd(self, input_file, output_dir):
        return f'"{self.tool}" -b {self.block_size} -p {self.page_size} --unpack "{output_dir}" "{input_file}"'

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

def _parse_partitions(env):
    partitions_csv = env.subst("$PARTITIONS_TABLE_CSV")
    if not isfile(partitions_csv):
        sys.stderr.write("Could not find the file %s with partitions "
                         "table.\n" % partitions_csv)
        env.Exit(1)
        return

    result = []
    next_offset = 0
    with open(partitions_csv) as fp:
        for line in fp.readlines():
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            tokens = [t.strip() for t in line.split(",")]
            if len(tokens) < 5:
                continue
            partition = {
                "name": tokens[0],
                "type": tokens[1],
                "subtype": tokens[2],
                "offset": tokens[3] or next_offset,
                "size": tokens[4],
                "flags": tokens[5] if len(tokens) > 5 else None
            }
            result.append(partition)
            next_offset = (_parse_size(partition['offset']) +
                           _parse_size(partition['size']))
    return result

def esp32_fetch_spiffs_size(env):
    spiffs = None
    for p in _parse_partitions(env):
        if p['type'] == "data" and p['subtype'] == "spiffs":
            spiffs = p
    if not spiffs:
        sys.stderr.write(
            env.subst("Could not find the `spiffs` section in the partitions "
                      "table $PARTITIONS_TABLE_CSV\n"))
        env.Exit(1)
        return
    env["SPIFFS_START"] = _parse_size(spiffs['offset'])
    env["SPIFFS_SIZE"] = _parse_size(spiffs['size'])
    env["SPIFFS_PAGE"] = int("0x100", 16)
    env["SPIFFS_BLOCK"] = int("0x1000", 16)

## FS helpers for ESP8266
# copied from https://github.com/platformio/platform-espressif8266/blob/develop/builder/main.py
# Copyright 2014-present PlatformIO <contact@platformio.org>
# Licensed under the Apache License, Version 2.0 (the "License");

def _get_board_f_flash(env):
    frequency = env.subst("$BOARD_F_FLASH")
    frequency = str(frequency).replace("L", "")
    return int(int(frequency) / 1000000)

def _parse_ld_sizes(ldscript_path):
    assert ldscript_path
    result = {}
    # get flash size from board's manifest
    result['flash_size'] = int(env.BoardConfig().get("upload.maximum_size", 0))
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

def _get_flash_size(env):
    ldsizes = _parse_ld_sizes(env.GetActualLDScript())
    if ldsizes['flash_size'] < 1048576:
        return "%dK" % (ldsizes['flash_size'] / 1024)
    return "%dM" % (ldsizes['flash_size'] / 1048576)

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

def esp8266_get_esptoolpy_reset_flags(resetmethod):
    # no dtr, no_sync
    resets = ("no_reset_no_sync", "soft_reset")
    if resetmethod == "nodemcu":
        # dtr
        resets = ("default_reset", "hard_reset")
    elif resetmethod == "ck":
        # no dtr
        resets = ("no_reset", "soft_reset")

    return ["--before", resets[0], "--after", resets[1]]

## Script interface functions

def get_fs_type_start_and_length():
    platform = env["PIOPLATFORM"]
    if platform == "espressif32":
        print("Retrieving filesystem info for ESP32. Assuming SPIFFS.")
        print("Partition file: " + str(env.subst("$PARTITIONS_TABLE_CSV")))
        esp32_fetch_spiffs_size(env)
        return SPIFFSInfo(env["SPIFFS_START"], env["SPIFFS_SIZE"], env["SPIFFS_PAGE"], env["SPIFFS_BLOCK"])
    elif platform == "espressif8266":
        print("Retrieving filesystem info for ESP8266.")
        filesystem = board.get("build.filesystem", "spiffs")
        if filesystem not in ("spiffs", "littlefs"):
            print("Unrecognized board_build.filesystem option '" + str(filesystem) + "'.")
            env.Exit(1)
        # fetching sizes is the same for all filesystems
        esp8266_fetch_fs_size(env)
        print("FS_START: " + hex(env["FS_START"]))
        print("FS_END: " + hex(env["FS_END"]))
        print("FS_PAGE: " + hex(env["FS_PAGE"]))
        print("FS_BLOCK: " + hex(env["FS_BLOCK"]))
        if filesystem == "spiffs":
            print("Recognized SPIFFS filesystem.")
            return SPIFFSInfo(env["FS_START"], env["FS_END"] - env["FS_START"], env["FS_PAGE"], env["FS_BLOCK"])
        elif filesystem == "littlefs":
            print("Recognized LittleFS filesystem.")
            return LittleFSInfo(env["FS_START"], env["FS_END"] - env["FS_START"], env["FS_PAGE"], env["FS_BLOCK"])
        else:
            print("Unrecongized configuration.")
    pass

def download_fs(fs_info: FSInfo):
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    fs_file = join(env["PROJECT_DIR"], f"downloaded_fs_{hex(fs_info.start)}_{hex(fs_info.length)}.bin")
    esptoolpy_flags = [
            "--chip", mcu,
            "--port", env.subst("$UPLOAD_PORT"),
            "--baud",  env.subst("$UPLOAD_SPEED"),
            "--before", "default_reset",
            "--after", "hard_reset",
            "read_flash",
            hex(fs_info.start),
            hex(fs_info.length),
            fs_file
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    print("Executing flash download command.")
    print(esptoolpy_cmd)
    try:
        returncode = subprocess.call(esptoolpy_cmd, shell=False)
        print("Downloaded filesystem binary.")
        return (True, fs_file)
    except subprocess.CalledProcessError as exc:
        print("Downloading failed with " + str(exc))
        return (False, "")

def unpack_fs(fs_info: FSInfo, downloaded_file: str):
    # by writing custom_unpack_dir = some_dir in the platformio.ini, one can
    # control the unpack directory
    unpack_dir = env.GetProjectOption("custom_unpack_dir", "unpacked_fs")
    #unpack_dir = "unpacked_fs"
    try:
        if os.path.exists(unpack_dir):
            shutil.rmtree(unpack_dir)
    except Exception as exc:
        print("Exception while attempting to remove the folder '" + str(unpack_dir) + "': " + str(exc))
    if not os.path.exists(unpack_dir):
        os.makedirs(unpack_dir)

    cmd = fs_info.get_extract_cmd(downloaded_file, unpack_dir)
    print("Executing extraction command: " + str(cmd))
    try:
        returncode = subprocess.call(cmd, shell=False)
        print("Unpacked filesystem.")
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
    print("Entrypoint")
    #print(env.Dump())
    info = get_fs_type_start_and_length()
    print("Parsed FS info: " + str(info))
    download_ok, downloaded_file = download_fs(info)
    print("Download was okay: " + str(download_ok) + ". File at: "+ str(downloaded_file))
    unpack_ok, unpacked_dir = unpack_fs(info, downloaded_file)
    if unpack_ok is True:
        display_fs(unpacked_dir)

env.AddCustomTarget(
    name="downloadfs",
    dependencies=None,
    actions=[
        command_download_fs
    ],
    title="Download Filesystem",
    description="Downloads and displays files stored in the target ESP32/ESP8266"
)
