#!/usr/bin/env python
# -*- coding: utf-8 -*-
VER = '2.0.0002'

"""
    decode-config.py - Backup/Restore Sonoff-Tasmota configuration data

    Copyright (C) 2018 Norbert Richter <nr@prsolution.eu>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


Requirements:
     - Python
     - pip install json pycurl urllib2 configargparse


Instructions:
    Execute command with option -d to retrieve config data from a host
    or use -f to read a configuration file saved using Tasmota Web-UI
    
    For further information read 'decode-config.md'

    For help execute command with argument -h (or -H for advanced help)


Usage: decode-config.py [-f <filename>] [-d <host>] [-P <port>]
                        [-u <username>] [-p <password>] [-i <filename>]
                        [-o <filename>] [-F json|bin|dmp] [-E] [-e]
                        [--json-indent <indent>] [--json-compact]
                        [--json-hide-pw] [--json-unhide-pw] [-h] [-H] [-v]
                        [-V] [-c <filename>] [--ignore-warnings]

    Backup/Restore Sonoff-Tasmota configuration data. Args that start with '--'
    (eg. -f) can also be set in a config file (specified via -c). Config file
    syntax allows: key=value, flag=true, stuff=[a,b,c] (for details, see syntax at
    https://goo.gl/R74nmi). If an arg is specified in more than one place, then
    commandline values override config file values which override defaults.

    optional arguments:
      -c, --config <filename>
                            program config file - can be used to set default
                            command args (default: None)
      --ignore-warnings     do not exit on warnings. Not recommended, used by your
                            own responsibility!

    Source:
      Read/Write Tasmota configuration from/to

      -f, --file, --tasmota-file <filename>
                            file to retrieve/write Tasmota configuration from/to
                            (default: None)'
      -d, --device, --host <host>
                            hostname or IP address to retrieve/send Tasmota
                            configuration from/to (default: None)
      -P, --port <port>     TCP/IP port number to use for the host connection
                            (default: 80)
      -u, --username <username>
                            host HTTP access username (default: admin)
      -p, --password <password>
                            host HTTP access password (default: None)

    Backup/Restore:
      Backup/Restore configuration file specification

      -i, --restore-file <filename>
                            file to restore configuration from (default: None).
                            Replacements: @v=firmware version, @f=device friendly
                            name, @h=device hostname
      -o, --backup-file <filename>
                            file to backup configuration to (default: None).
                            Replacements: @v=firmware version, @f=device friendly
                            name, @h=device hostname
      -F, --backup-type json|bin|dmp
                            backup filetype (default: 'json')
      -E, --extension       append filetype extension for -i and -o filename
                            (default)
      -e, --no-extension    do not append filetype extension, use -i and -o
                            filename as passed

    JSON:
      JSON backup format specification

      --json-indent <indent>
                            pretty-printed JSON output using indent level
                            (default: 'None'). -1 disables indent.
      --json-compact        compact JSON output by eliminate whitespace
      --json-hide-pw        hide passwords (default)
      --json-unhide-pw      unhide passwords

    Info:
      additional information

      -h, --help            show usage help message and exit
      -H, --full-help       show full help message and exit
      -v, --verbose         produce more output about what the program does
      -V, --version         show program's version number and exit

    Either argument -d <host> or -f <filename> must be given.


Returns:
    0: successful
    1: restore skipped
    2: program argument error
    3: file not found
    4: data size mismatch
    5: data CRC error
    6: unsupported configuration version
    7: configuration file read error
    8: JSON file decoding error
    9: Restore file data error
    10: Device data download error
    11: Device data upload error
    20: python module missing
    21: Internal error
    >21: python library exit code
    4xx, 5xx: HTTP errors

"""

class ExitCode:
    OK = 0
    RESTORE_SKIPPED = 1
    ARGUMENT_ERROR = 2
    FILE_NOT_FOUND = 3
    DATA_SIZE_MISMATCH = 4
    DATA_CRC_ERROR = 5
    UNSUPPORTED_VERSION = 6
    FILE_READ_ERROR = 7
    JSON_READ_ERROR = 8
    RESTORE_DATA_ERROR = 9
    DOWNLOAD_CONFIG_ERROR = 10
    UPLOAD_CONFIG_ERROR = 11
    MODULE_NOT_FOUND = 20
    INTERNAL_ERROR = 21

import os.path
import io
import sys, platform
def ModuleImportError(module):
    er = str(module)
    print >> sys.stderr, "{}. Try 'pip install {}' to install it".format(er,er.split(' ')[len(er.split(' '))-1])
    sys.exit(ExitCode.MODULE_NOT_FOUND)
try:
    from datetime import datetime
    import struct
    import socket
    import re
    import math
    import inspect
    import json
    import configargparse
    import pycurl
    import urllib2
except ImportError, e:
    ModuleImportError(e)


PROG='{} v{} by Norbert Richter <nr@prsolution.eu>'.format(os.path.basename(sys.argv[0]),VER)

CONFIG_FILE_XOR     = 0x5A
BINARYFILE_MAGIC    = 0x63576223
STR_ENCODING        = 'utf8'
DEFAULTS            = {
    'DEFAULT':
    {
        'configfile':   None,
        'ignorewarning':False,
    },
    'source':
    {
        'device':       None,
        'port':         80,
        'username':     'admin',
        'password':     None,
        'tasmotafile':  None,
    },
    'backup':
    {
        'restorefile':  None,
        'backupfile':   None,
        'backupfileformat': 'json',
        'extension':    True,
    },
    'jsonformat':
    {
        'jsonindent':   None,
        'jsoncompact':  False,
        'jsonsort':     True,
        'jsonrawvalues':False,
        'jsonrawkeys':  False,
        'jsonhidepw':   False,
    },
}
args = {}
exitcode = 0


"""
Settings dictionary describes the config file fields definition:

    Each setting name has a tuple containing the following items:

        (format, baseaddr, datadef, <convert>)

    where

        format
            Define the data interpretation.
            It is either a string or a tuple containing a string and a
            sub-Settings dictionary.
            'xxx':
                A string is used to interpret the data at <baseaddr>
                The string defines the format interpretion as described
                in 'struct module format string', see
                https://docs.python.org/2.7/library/struct.html#format-strings
                In addition to this format string there is as special
                meaning of a dot '.' - this means a bit with an optional
                prefix length. If no prefix is given, 1 is assumed.
            {}:
                A dictionary describes itself a 'Settings' dictonary (recursive)

        baseaddr
            The address (starting from 0) within config data.
            For bit fields <baseaddr> must be a tuple.
            n:
                Defines a simple address <n> within config data.
                <n> must be a positive integer.
            (n, b, s):
                A tuple defines a bit field:
                <n>
                    is the address within config data (integer)
                <b>
                    how many bits are used (positive integer)
                <s>
                    bit shift <s> (integer)
                    positive <s> shift the result <s> right bits
                    negative <s> shift the result <s> left bits

        datadef
            Data definition, is either a array definition or a
            tuple containing an array definition and min/max values
            Format:  arraydef|(arraydef, min, max)
                arraydef:
                    None:
                        None must be given if the field contains a
                        simple value desrcibed by the <format> prefix
                    n:
                    [n]:
                        Defines a one-dimensional array of size <n>
                    [n, m <,o...>]
                        Defines a multi-dimensional array
                min:
                    defines a minimum valid value or None if all values
                    for this format is allowed.
                max:
                    defines a maximum valid value or None if all values
                    for this format is allowed.

        converter (optional)
            Conversion methode(s): 'xxx'|func or ('xxx'|func, 'xxx'|func)
            Read conversion is used if args.jsonrawvalues is False
            Write conversion is used if jsonrawvalues from restore json
            file is False or args.jsonrawvalues is False.
            Converter is either a single methode 'xxx'|func or a tuple
            Single methode will be used for reading conversion only:
            'xxx':
                string will used for reading conversion and will be
                evaluate as is, this can also contain python code.
                Use '$' for current value.
            func:
                name of a formating function that will be used for
                reading conversion
            None:
                will read as definied in <format>
            (read, write):
                a tuple with 2 objects. Each can be of the same type
                as the single method above ('xxx'|func) or None.
                read:
                    method will be used for read conversion
                    (unpack data from dmp object)
                write:
                    method will be used for write conversion
                    (pack data to dmp object)
                    If write method is None indicates value is
                    readable only and will not be write

"""
def passwordread(value):
    return "********" if args.jsonhidepw else value
def passwordwrite(value):
    return None if value=="********" else value

Setting_5_10_0 = {
    'cfg_holder':                   ('<L',  0x000, '"0x{:08x}".format($)'),
    'save_flag':                    ('<L',  0x004, None, (None, None)),
    'version':                      ('<L',  0x008, None, ('hex($)', None)),
    'bootcount':                    ('<L',  0x00C, None, (None, None)),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                     },     0x010, None),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], (passwordread, passwordwrite)),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('60s', 0x1AD, None),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, (passwordread, passwordwrite)),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'mqtt_fingerprinth':            ('B',   0x2D2, [20]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, ('"0x{:08x}".format($)', None)),
                                        'power1':   ('<L',  (0x2E8, 1, 0), None),
                                        'power2':   ('<L',  (0x2E8, 1, 1), None),
                                        'power3':   ('<L',  (0x2E8, 1, 2), None),
                                        'power4':   ('<L',  (0x2E8, 1, 3), None),
                                        'power5':   ('<L',  (0x2E8, 1, 4), None),
                                        'power6':   ('<L',  (0x2E8, 1, 5), None),
                                        'power7':   ('<L',  (0x2E8, 1, 6), None),
                                        'power8':   ('<L',  (0x2E8, 1, 7), None),
                                     },     0x2E8, None),
    'pwm_value':                    ('<H',  0x2EC, [5]),
    'altitude':                     ('<h',  0x2F6, None),
    'tele_period':                  ('<H',  0x2F8, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [23]),
    'state_text':                   ('11s', 0x313, [4]),
    'domoticz_update_timer':        ('<H',  0x340, None),
    'pwm_range':                    ('<H',  0x342, None),
    'domoticz_relay_idx':           ('<L',  0x344, [4]),
    'domoticz_key_idx':             ('<L',  0x354, [4]),
    'energy_power_calibration':     ('<L',  0x364, None),
    'energy_voltage_calibration':   ('<L',  0x368, None),
    'energy_current_calibration':   ('<L',  0x36C, None),
    'energy_kWhtoday':              ('<L',  0x370, None),
    'energy_kWhyesterday':          ('<L',  0x374, None),
    'energy_kWhdoy':                ('<H',  0x378, None),
    'energy_min_power':             ('<H',  0x37A, None),
    'energy_max_power':             ('<H',  0x37C, None),
    'energy_min_voltage':           ('<H',  0x37E, None),
    'energy_max_voltage':           ('<H',  0x380, None),
    'energy_min_current':           ('<H',  0x382, None),
    'energy_max_current':           ('<H',  0x384, None),
    'energy_max_power_limit':       ('<H',  0x386, None),
    'energy_max_power_limit_hold':          ('<H',  0x388, None),
    'energy_max_power_limit_window':        ('<H',  0x38A, None),
    'energy_max_power_safe_limit':          ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':     ('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':   ('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'sleep':                        ('B',   0x453, None),
    'domoticz_switch_idx':          ('<H',  0x454, [4]),
    'domoticz_sensor_idx':          ('<H',  0x45C, [12]),
    'module':                       ('B',   0x474, None),
    'ws_color':                     ('B',   0x475, [4,3]),
    'ws_width':                     ('B',   0x481, [3]),
    'my_gp':                        ('B',   0x484, [18]),
    'light_pixels':                 ('<H',  0x496, None),
    'light_color':                  ('B',   0x498, [5]),
    'light_correction':             ('B',   0x49D, None),
    'light_dimmer':                 ('B',   0x49E, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'web_password':                 ('33s', 0x4A9, None, (passwordread, passwordwrite)),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], ("socket.inet_ntoa(struct.pack('<L', $))", "struct.unpack('<L', socket.inet_aton($))[0]")),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, ('"0x{:08x}".format($)', None)),
                                        'current_resolution':       ('<L',  (0x5BC, 2, 15), None),
                                        'voltage_resolution':       ('<L',  (0x5BC, 2, 17), None),
                                        'wattage_resolution':       ('<L',  (0x5BC, 2, 19), None),
                                        'emulation':                ('<L',  (0x5BC, 2, 21), None),
                                        'energy_resolution':        ('<L',  (0x5BC, 3, 23), None),
                                        'pressure_resolution':      ('<L',  (0x5BC, 2, 26), None),
                                        'humidity_resolution':      ('<L',  (0x5BC, 2, 28), None),
                                        'temperature_resolution':   ('<L',  (0x5BC, 2, 30), None),
                                     },     0x5BC, None),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format($)'),
}

Setting_5_11_0 = Setting_5_10_0
Setting_5_11_0.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                     },     0x010, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
})
Setting_5_11_0.pop('mqtt_fingerprinth',None)

Setting_5_12_0 = Setting_5_11_0
Setting_5_12_0.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                     },     0x010, None),
})

Setting_5_13_1 = Setting_5_12_0
Setting_5_13_1.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                        'mqtt_serial':              ('<L',  (0x010, 1, 22), None),
                                        'rules_enabled':            ('<L',  (0x010, 1, 23), None),
                                        'rules_once':               ('<L',  (0x010, 1, 24), None),
                                        'knx_enabled':              ('<L',  (0x010, 1, 25), None),
                                     },     0x010, None),
    'baudrate':                     ('B',   0x09D, None, ('$ * 1200','$ / 1200') ),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'energy_power_delta':           ('B',   0x33F, None),
    'light_rotation':               ('<H',  0x39E, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, ('"0x{:08x}".format($)', None)),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, ('float($) / 1000000', 'int($ * 1000000)')),
    'longitude':                    ('i',   0x6B4, None, ('float($) / 1000000', 'int($ * 1000000)')),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'rules':                        ('512s',0x800, None),
})
Setting_5_14_0 = Setting_5_13_1
Setting_5_14_0.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                        'mqtt_serial':              ('<L',  (0x010, 1, 22), None),
                                        'rules_enabled':            ('<L',  (0x010, 1, 23), None),
                                        'rules_once':               ('<L',  (0x010, 1, 24), None),
                                        'knx_enabled':              ('<L',  (0x010, 1, 25), None),
                                        'device_index_enable':      ('<L',  (0x010, 1, 26), None),
                                     },     0x010, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, ('"0x{:04x}".format($)', None)),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
})

Setting_6_0_0 = Setting_5_14_0
Setting_6_0_0.update({
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None, (None, None)),
    'bootcount':                    ('<H',  0x00C, None, (None, None)),
    'cfg_crc':                      ('<H',  0x00E, None, '"0x{:04x}".format($)'),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                        'mqtt_serial':              ('<L',  (0x010, 1, 22), None),
                                        'rules_enabled':            ('<L',  (0x010, 1, 23), None),
                                        'rules_once':               ('<L',  (0x010, 1, 24), None),
                                        'knx_enabled':              ('<L',  (0x010, 1, 25), None),
                                        'device_index_enable':      ('<L',  (0x010, 1, 26), None),
                                        'knx_enable_enhancement':   ('<L',  (0x010, 1, 27), None),
                                     },     0x010, None),
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None, (None,None)),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None, (None,None)),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None),
                                     },     0x4A0, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
})

Setting_6_1_1 = Setting_6_0_0
Setting_6_1_1.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                        'mqtt_serial':              ('<L',  (0x010, 1, 22), None),
                                        'rules_enabled':            ('<L',  (0x010, 1, 23), None),
                                        'rules_once':               ('<L',  (0x010, 1, 24), None),
                                        'knx_enabled':              ('<L',  (0x010, 1, 25), None),
                                        'device_index_enable':      ('<L',  (0x010, 1, 26), None),
                                        'knx_enable_enhancement':   ('<L',  (0x010, 1, 27), None),
                                        'rf_receive_decimal':       ('<L',  (0x010, 1, 28), None),
                                        'ir_receive_decimal':       ('<L',  (0x010, 1, 29), None),
                                        'hass_light':               ('<L',  (0x010, 1, 30), None),
                                     },     0x010, None),
    'flag3':                        ('<L',  0x3A0, None, '"0x{:08x}".format($)'),
    'switchmode':                   ('B',   0x3A4, [8]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, ('"0x{:08x}".format($)', None)),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
})

Setting_6_2_1 = Setting_6_1_1
Setting_6_2_1.update({
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, ('"0x{:08x}".format($)', None)),
                                        'save_state':               ('<L',  (0x010, 1,  0), None),
                                        'button_restrict':          ('<L',  (0x010, 1,  1), None),
                                        'value_units':              ('<L',  (0x010, 1,  2), None),
                                        'mqtt_enabled':             ('<L',  (0x010, 1,  3), None),
                                        'mqtt_response':            ('<L',  (0x010, 1,  4), None),
                                        'mqtt_power_retain':        ('<L',  (0x010, 1,  5), None),
                                        'mqtt_button_retain':       ('<L',  (0x010, 1,  6), None),
                                        'mqtt_switch_retain':       ('<L',  (0x010, 1,  7), None),
                                        'temperature_conversion':   ('<L',  (0x010, 1,  8), None),
                                        'mqtt_sensor_retain':       ('<L',  (0x010, 1,  9), None),
                                        'mqtt_offline':             ('<L',  (0x010, 1, 10), None),
                                        'button_swap':              ('<L',  (0x010, 1, 11), None),
                                        'stop_flash_rotate':        ('<L',  (0x010, 1, 12), None),
                                        'button_single':            ('<L',  (0x010, 1, 13), None),
                                        'interlock':                ('<L',  (0x010, 1, 14), None),
                                        'pwm_control':              ('<L',  (0x010, 1, 15), None),
                                        'ws_clock_reverse':         ('<L',  (0x010, 1, 16), None),
                                        'decimal_text':             ('<L',  (0x010, 1, 17), None),
                                        'light_signal':             ('<L',  (0x010, 1, 18), None),
                                        'hass_discovery':           ('<L',  (0x010, 1, 19), None),
                                        'not_power_linked':         ('<L',  (0x010, 1, 20), None),
                                        'no_power_on_check':        ('<L',  (0x010, 1, 21), None),
                                        'mqtt_serial':              ('<L',  (0x010, 1, 22), None),
                                        'rules_enabled':            ('<L',  (0x010, 1, 23), None),
                                        'rules_once':               ('<L',  (0x010, 1, 24), None),
                                        'knx_enabled':              ('<L',  (0x010, 1, 25), None),
                                        'device_index_enable':      ('<L',  (0x010, 1, 26), None),
                                        'knx_enable_enhancement':   ('<L',  (0x010, 1, 27), None),
                                        'rf_receive_decimal':       ('<L',  (0x010, 1, 28), None),
                                        'ir_receive_decimal':       ('<L',  (0x010, 1, 29), None),
                                        'hass_light':               ('<L',  (0x010, 1, 30), None),
                                        'global_state':             ('<L',  (0x010, 1, 31), None),
                                     },     0x010, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None, (None, None)),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None),
                                     },     0x1A7, None),
    'display_rotate':               ('B',   0x2FA, None),
    'display_font':                 ('B',   0x312, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, ('"0x{:08x}".format($)', None)),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1,31), None),
                                     },     0x3A0, None),
    'button_debounce':              ('<H',  0x542, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, ('"0x{:08x}".format($)', None)),
                                        'axis_resolution':          ('<L',  (0x5BC, 2, 13), None),
                                        'current_resolution':       ('<L',  (0x5BC, 2, 15), None),
                                        'voltage_resolution':       ('<L',  (0x5BC, 2, 17), None),
                                        'wattage_resolution':       ('<L',  (0x5BC, 2, 19), None),
                                        'emulation':                ('<L',  (0x5BC, 2, 21), None),
                                        'energy_resolution':        ('<L',  (0x5BC, 3, 23), None),
                                        'pressure_resolution':      ('<L',  (0x5BC, 2, 26), None),
                                        'humidity_resolution':      ('<L',  (0x5BC, 2, 28), None),
                                        'temperature_resolution':   ('<L',  (0x5BC, 2, 30), None),
                                     },     0x5BC, None),
    'switch_debounce':              ('<H',  0x66E, None),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
})

Setting_6_2_1_2 = Setting_6_2_1
Setting_6_2_1_2.update({
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, ('"0x{:08x}".format($)', None)),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                     },     0x3A0, None),
})

Setting_6_2_1_3 = Setting_6_2_1_2
Setting_6_2_1_3.update({
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, ('"0x{:08x}".format($)', None)),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                         'time_append_timezone':('<L',  (0x3A0, 1, 2), None),
                                     },     0x3A0, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, ('"0x{:08x}".format($)', None)),
                                        'frequency_resolution':     ('<L',  (0x5BC, 2, 11), None),
                                        'axis_resolution':          ('<L',  (0x5BC, 2, 13), None),
                                        'current_resolution':       ('<L',  (0x5BC, 2, 15), None),
                                        'voltage_resolution':       ('<L',  (0x5BC, 2, 17), None),
                                        'wattage_resolution':       ('<L',  (0x5BC, 2, 19), None),
                                        'emulation':                ('<L',  (0x5BC, 2, 21), None),
                                        'energy_resolution':        ('<L',  (0x5BC, 3, 23), None),
                                        'pressure_resolution':      ('<L',  (0x5BC, 2, 26), None),
                                        'humidity_resolution':      ('<L',  (0x5BC, 2, 28), None),
                                        'temperature_resolution':   ('<L',  (0x5BC, 2, 30), None),
                                     },     0x5BC, None),
})

Setting_6_2_1_6 = Setting_6_2_1_3
Setting_6_2_1_6.update({
    'energy_frequency_calibration': ('<L',  0x7C8, None),
})

Setting_6_2_1_10 = Setting_6_2_1_6
Setting_6_2_1_10.update({
    'rgbwwTable':                   ('B',   0x71A, [5]),
})

Setting_6_2_1_14 = Setting_6_2_1_10
Setting_6_2_1_14.update({
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, ('"0x{:08x}".format($)', None)),
                                        'weight_resolution':        ('<L',  (0x5BC, 2,  9), None),
                                        'frequency_resolution':     ('<L',  (0x5BC, 2, 11), None),
                                        'axis_resolution':          ('<L',  (0x5BC, 2, 13), None),
                                        'current_resolution':       ('<L',  (0x5BC, 2, 15), None),
                                        'voltage_resolution':       ('<L',  (0x5BC, 2, 17), None),
                                        'wattage_resolution':       ('<L',  (0x5BC, 2, 19), None),
                                        'emulation':                ('<L',  (0x5BC, 2, 21), None),
                                        'energy_resolution':        ('<L',  (0x5BC, 3, 23), None),
                                        'pressure_resolution':      ('<L',  (0x5BC, 2, 26), None),
                                        'humidity_resolution':      ('<L',  (0x5BC, 2, 28), None),
                                        'temperature_resolution':   ('<L',  (0x5BC, 2, 30), None),
                                     },     0x5BC, None),
    'weight_item':                  ('<H',  0x7BC, None),
    'weight_max':                   ('<H',  0x7BE, None, ('float($) / 10', 'int($ * 10)')),
    'weight_reference':             ('<L',  0x7C0, None),
    'weight_calibration':           ('<L',  0x7C4, None),
    'web_refresh':                  ('<H',  0x7CC, None),
})

Setting_6_2_1_19 = Setting_6_2_1_14
Setting_6_2_1_19.update({
    'weight_max':                   ('<L',  0x7B8, None, ('float($) / 10', 'int($ * 10)')),
})

Setting_6_2_1_20 = Setting_6_2_1_19
Setting_6_2_1_20.update({
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, ('"0x{:08x}".format($)', None)),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                         'time_append_timezone':('<L',  (0x3A0, 1, 2), None),
                                         'gui_hostname_ip':     ('<L',  (0x3A0, 1, 3), None),
                                     },     0x3A0, None),
})

Setting_6_3_0 = Setting_6_2_1_20
Setting_6_3_0.update({
    'energy_kWhtotal_time':         ('<L',  0x7B4, None),
})

Settings = [
            (0x6030000, 0xe00, Setting_6_3_0),
            (0x6020114, 0xe00, Setting_6_2_1_20),
            (0x6020113, 0xe00, Setting_6_2_1_19),
            (0x602010E, 0xe00, Setting_6_2_1_14),
            (0x602010A, 0xe00, Setting_6_2_1_10),
            (0x6020106, 0xe00, Setting_6_2_1_6),
            (0x6020103, 0xe00, Setting_6_2_1_3),
            (0x6020102, 0xe00, Setting_6_2_1_2),
            (0x6020100, 0xe00, Setting_6_2_1),
            (0x6010100, 0xe00, Setting_6_1_1),
            (0x6000000, 0xe00, Setting_6_0_0),
            (0x50e0000, 0xa00, Setting_5_14_0),
            (0x50d0100, 0xa00, Setting_5_13_1),
            (0x50c0000, 0x670, Setting_5_12_0),
            (0x50b0000, 0x670, Setting_5_11_0),
            (0x50a0000, 0x670, Setting_5_10_0),
           ]

# ----------------------------------------------------------------------
# helper
# ----------------------------------------------------------------------
def GetTemplateSizes():
    """
    Get all possible template sizes as list

    @param version:
        <int> version number from read binary data to search for

    @return:
        template sizes as list []
    """
    sizes = []
    for cfg in Settings:
        sizes.append(cfg[1])
    # return unique sizes only (remove duplicates)
    return list(set(sizes))


def GetTemplateSetting(decode_cfg):
    """
    Search for version, template, size and settings to be used depending on given binary config data

    @param decode_cfg:
        binary config data (decrypted)

    @return:
        version, template, size, settings to use; None if version is invalid
    """
    version = 0x0
    template = size = setting = None
    try:
        version = GetField(decode_cfg,  'version', Setting_6_2_1['version'], raw=True)
        # search setting definition
        template = None
        setting = None
        size = None
        for cfg in Settings:
            if version >= cfg[0]:
                template = cfg
                size = template[1]
                setting = template[2]
                break
    except:
        pass

    return version, template, size, setting


class LogType:
    INFO = 'INFO'
    WARNING = 'WARNING'
    ERROR = 'ERROR'


def message(msg, typ=None, status=None, line=None):
    """
    Writes a message to stdout

    @param msg:
        message to output
    @param typ:
        INFO, WARNING or ERROR
    @param status:
        status number
    """
    print >> sys.stderr, '{styp}{sdelimiter}{sstatus}{slineno}{scolon}{smgs}'.format(\
                            styp=typ if typ is not None else '',
                            sdelimiter=' ' if status is not None and status>0 and typ is not None else '',
                            sstatus=status if status is not None and status>0 else '',
                            scolon=': ' if typ is not None or line is not None else '',
                            smgs=msg,
                            slineno=' (@{:04d})'.format(line) if line is not None else '')


def exit(status=0, msg="end", typ=LogType.ERROR, src=None, doexit=True, line=None):
    """
    Called when the program should be exit

    @param status:
        the exit status program returns to callert
    @param msg:
        the msg logged before exit
    @param typ:
        msg type: 'INFO', 'WARNING' or 'ERROR'
    @param doexit:
        True to exit program, otherwise return
    """

    if src is not None:
        msg = '{} ({})'.format(src, msg)
    message(msg, typ=typ if status!=ExitCode.OK else LogType.INFO, status=status, line=line)
    exitcode = status
    if doexit:
        sys.exit(exitcode)


def ShortHelp(doexit=True):
    """
    Show short help (usage) only - ued by own -h handling

    @param doexit:
        sys.exit with OK if True
    """
    print parser.description
    print
    parser.print_usage()
    print
    print "For advanced help use '{prog} -H' or '{prog} --full-help'".format(prog=os.path.basename(sys.argv[0]))
    if doexit:
        sys.exit(ExitCode.OK)


class HTTPHeader:
    """
    pycurl helper class retrieving the request header
    """
    def __init__(self):
        self.contents = ''

    def clear(self):
        self.contents = ''

    def store(self, _buffer):
        self.contents = "{}{}".format(self.contents, _buffer)

    def response(self):
        header = str(self.contents).split('\n')
        if len(header)>0:
            return header[0].rstrip()
        return ''

    def contenttype(self):
        for item in str(self.contents).split('\n'):
            ditem = item.split(":")
            if ditem[0].strip().lower()=='content-type' and len(ditem)>1:
                return ditem[1].strip()
        return ''

    def __str__(self):
        return self.contents


class CustomHelpFormatter(configargparse.HelpFormatter):
    """
    Class for customizing the help output
    """

    def _format_action_invocation(self, action):
        """
        Reformat multiple metavar output
            -d <host>, --device <host>, --host <host>
        to single output
            -d, --device, --host <host>
        """

        orgstr = configargparse.HelpFormatter._format_action_invocation(self, action)
        if orgstr and orgstr[0] != '-': # only optional arguments
            return orgstr
        res = getattr(action, '_formatted_action_invocation', None)
        if res:
            return res

        options = orgstr.split(', ')
        if len(options) <=1:
            action._formatted_action_invocation = orgstr
            return orgstr

        return_list = []
        for option in options:
            meta = ""
            arg = option.split(' ')
            if len(arg)>1:
                meta = arg[1]
            return_list.append(arg[0])
        if len(meta) >0 and len(return_list) >0:
            return_list[len(return_list)-1] += " "+meta
        action._formatted_action_invocation = ', '.join(return_list)
        return action._formatted_action_invocation


# ----------------------------------------------------------------------
# Tasmota config data handling
# ----------------------------------------------------------------------
class FileType:
    FILE_NOT_FOUND = None
    DMP = 'dmp'
    JSON = 'json'
    BIN = 'bin'
    UNKNOWN = 'unknown'
    INCOMPLETE_JSON = 'incomplete json'
    INVALID_JSON = 'invalid json'
    INVALID_BIN = 'invalid bin'


def GetFileType(filename):
    """
    Get the FileType class member of a given filename

    @param filename:
        filename of the file to analyse

    @return:
        FileType class member
    """
    filetype = FileType.UNKNOWN

    # try filename
    try:
        isfile = os.path.isfile(filename)
        try:
            f = open(filename, "r")
            try:
                # try reading as json
                inputjson = json.load(f)
                if 'header' in inputjson:
                    filetype = FileType.JSON
                else:
                    filetype = FileType.INCOMPLETE_JSON
            except ValueError:
                filetype = FileType.INVALID_JSON
                # not a valid json, get filesize and compare it with all possible sizes
                try:
                    size = os.path.getsize(filename)
                except:
                    filetype = FileType.UNKNOWN
                sizes = GetTemplateSizes()

                # size is one of a dmp file size
                if size in sizes:
                    filetype = FileType.DMP
                elif (size - ((len(hex(BINARYFILE_MAGIC))-2)/2)) in sizes:
                    # check if the binary file has the magic header
                    try:
                        inputfile = open(filename, "rb")
                        inputbin = inputfile.read()
                        inputfile.close()
                        if struct.unpack_from('<L', inputbin, 0)[0] == BINARYFILE_MAGIC:
                            filetype = FileType.BIN
                        else:
                            filetype = FileType.INVALID_BIN

                    except:
                        pass
                # ~ else:
                    # ~ filetype = FileType.UNKNOWN
            finally:
                f.close()
        except:
            filetype = FileType.FILE_NOT_FOUND
    except:
        filetype = FileType.FILE_NOT_FOUND

    return filetype


def GetVersionStr(version):
    """
    Create human readable version string

    @param version:
        version integer

    @return:
        version string
    """
    major = ((version>>24) & 0xff)
    minor = ((version>>16) & 0xff)
    release = ((version>> 8) & 0xff)
    subrelease = (version & 0xff)
    if major>=6:
        if subrelease>0:
            subreleasestr = str(subrelease)
        else:
            subreleasestr = ''
    else:
        if subrelease>0:
            subreleasestr = str(chr(subrelease+ord('a')-1))
        else:
            subreleasestr = ''
    return "{:d}.{:d}.{:d}{}{}".format( major, minor, release, '.' if (major>=6 and subreleasestr!='') else '', subreleasestr)


def MakeValidFilename(filename):
    """
    Make a valid filename

    @param filename:
        filename src
        
    @return:
        valid filename removed invalid chars and replace space with _
    """
    try:
        filename = filename.decode('unicode-escape').translate(dict((ord(char), None) for char in '\/*?:"<>|'))
    except:
        pass
    return str(filename.replace(' ','_'))


def MakeFilename(filename, filetype, decode_cfg):
    """
    Replace variable within a filename

    @param filename:
        original filename possible containing replacements:
        @v:
            Tasmota version
        @f:
            friendlyname
        @h:
            hostname
    @param filetype:
        FileType.x object - creates extension if not None
    @param decode_cfg:
        binary config data (decrypted)

    @return:
        New filename with replacements
    """
    v = f1 = f2 = f3 = f4 = ''
    if 'version' in decode_cfg:
        v = GetVersionStr( int(str(decode_cfg['version']), 0) )
        filename = filename.replace('@v', v)
    if 'friendlyname' in decode_cfg:
        filename = filename.replace('@f', decode_cfg['friendlyname'][0] )
    if 'hostname' in decode_cfg:
        filename = filename.replace('@h', decode_cfg['hostname'] )

    filename = MakeValidFilename(filename)
    ext = ''
    try:
        name, ext = os.path.splitext(filename)
    except:
        pass
    if len(ext) and ext[0]=='.':
        ext = ext[1:]
    if filetype is not None and args.extension and (len(ext)<2 or all(c.isdigit() for c in ext)):
        filename += '.'+filetype.lower()

    return filename


def MakeUrl(host, port=80, location=''):
    """
    Create a Tasmota host url

    @param host:
        hostname or IP of Tasmota host
    @param port:
        port number to use for http connection
    @param location:
        http url location

    @return:
        Tasmota http url
    """
    return "http://{shost}{sdelimiter}{sport}/{slocation}".format(\
            shost=host,
            sdelimiter=':' if port != 80 else '',
            sport=port if port != 80 else '',
            slocation=location )


def PullTasmotaConfig():
    """
    Pull config from Tasmota device/file

    @return:
        binary config data (encrypted) or None on error
    """

    if args.device is not None:
        # read config direct from device via http
    
        c = pycurl.Curl()
        buffer = io.BytesIO()
        c.setopt(c.WRITEDATA, buffer)
        header = HTTPHeader()
        c.setopt(c.HEADERFUNCTION, header.store)
        c.setopt(c.FOLLOWLOCATION, True)
        c.setopt(c.URL, MakeUrl(args.device, args.port, 'dl'))
        if args.username is not None and args.password is not None:
            c.setopt(c.HTTPAUTH, c.HTTPAUTH_BASIC)
            c.setopt(c.USERPWD, args.username + ':' + args.password)
        c.setopt(c.VERBOSE, False)

        responsecode = 200
        try:
            c.perform()
            responsecode = c.getinfo(c.RESPONSE_CODE)
            response = header.response()
        except Exception, e:
            exit(e[0], e[1],line=inspect.getlineno(inspect.currentframe()))
        finally:
            c.close()
            
        if responsecode>=400:
            exit(responsecode, 'HTTP result: {}'.format(header.response()),line=inspect.getlineno(inspect.currentframe()))
        elif header.contenttype()!='application/octet-stream':
            exit(ExitCode.DOWNLOAD_CONFIG_ERROR, "Device did not response properly, may be Tasmota webserver admin mode is disabled (WebServer 2)",line=inspect.getlineno(inspect.currentframe()))
        encode_cfg = buffer.getvalue()

    elif args.tasmotafile is not None:
        # read config from a file
        if not os.path.isfile(args.tasmotafile):    # check file exists
            exit(ExitCode.FILE_NOT_FOUND, "File '{}' not found".format(args.tasmotafile),line=inspect.getlineno(inspect.currentframe()))
        try:
            tasmotafile = open(args.tasmotafile, "rb")
            encode_cfg = tasmotafile.read()
            tasmotafile.close()
        except Exception, e:
            exit(e[0], "'{}' {}".format(args.tasmotafile, e[1]),line=inspect.getlineno(inspect.currentframe()))

    else:
        return None

    return encode_cfg


def PushTasmotaConfig(encode_cfg, host, port, username=DEFAULTS['source']['username'], password=None):
    """
    Upload binary data to a Tasmota host using http

    @param encode_cfg:
        encrypted binary data or filename containing Tasmota encrypted binary config
    @param host:
        hostname or IP of Tasmota device
    @param username:
        optional username for Tasmota web login
    @param password
        optional password for Tasmota web login

    @return
        errorcode, errorstring
        errorcode=0 if success, otherwise http response or exception code
    """
    # ~ return 0, 'OK'

    if isinstance(encode_cfg, bytearray):
        encode_cfg = str(encode_cfg)

    c = pycurl.Curl()
    buffer = io.BytesIO()
    c.setopt(c.WRITEDATA, buffer)
    header = HTTPHeader()
    c.setopt(c.HEADERFUNCTION, header.store)
    c.setopt(c.FOLLOWLOCATION, True)
    # get restore config page first to set internal Tasmota vars
    c.setopt(c.URL, MakeUrl(host, port, 'rs?'))
    if args.username is not None and args.password is not None:
        c.setopt(c.HTTPAUTH, c.HTTPAUTH_BASIC)
        c.setopt(c.USERPWD, args.username + ':' + args.password)
    c.setopt(c.HTTPGET, True)
    c.setopt(c.VERBOSE, False)

    responsecode = 200
    try:
        c.perform()
        responsecode = c.getinfo(c.RESPONSE_CODE)
    except Exception, e:
        c.close()
        return e[0], e[1]

    if responsecode>=400:
        c.close()
        return responsecode, header.response()
    elif header.contenttype()!='text/html':
        c.close()
        return ExitCode.UPLOAD_CONFIG_ERROR, "Device did not response properly, may be Tasmota webserver admin mode is disabled (WebServer 2)"

    # post data
    header.clear()
    c.setopt(c.HEADERFUNCTION, header.store)
    c.setopt(c.POST, 1)
    c.setopt(c.URL, MakeUrl(host, port, 'u2'))
    try:
        isfile = os.path.isfile(encode_cfg)
    except:
        isfile = False
    if isfile:
        c.setopt(c.HTTPPOST, [("file", (c.FORM_FILE, encode_cfg))])
    else:
        # use as binary data
        c.setopt(c.HTTPPOST, [
            ('fileupload', (
                c.FORM_BUFFER, '{sprog}_v{sver}.dmp'.format(sprog=os.path.basename(sys.argv[0]), sver=VER),
                c.FORM_BUFFERPTR, encode_cfg
            )),
        ])

    responsecode = 200
    try:
        c.perform()
        responsecode = c.getinfo(c.RESPONSE_CODE)
    except Exception, e:
        return e[0], e[1]
    c.close()

    if responsecode>=400:
        return responsecode, header.response()
    elif header.contenttype()!='text/html':
        return ExitCode.UPLOAD_CONFIG_ERROR, "Device did not response properly, may be Tasmota webserver admin mode is disabled (WebServer 2)"

    return 0, 'OK'


def DecryptEncrypt(obj):
    """
    Decrpt/Encrypt binary config data

    @param obj:
        binary config data

    @return:
        decrypted configuration (if obj contains encrypted data)
        encrypted configuration (if obj contains decrypted data)
    """
    if isinstance(obj, bytearray):
        obj = str(obj)
    dobj  = obj[0:2]
    for i in range(2, len(obj)):
        dobj += chr( (ord(obj[i]) ^ (CONFIG_FILE_XOR +i)) & 0xff )
    return dobj


def GetSettingsCrc(dobj):
    """
    Return binary config data calclulated crc

    @param dobj:
        decrypted binary config data

    @return:
        2 byte unsigned integer crc value

    """
    if isinstance(dobj, bytearray):
        dobj = str(dobj)
    crc = 0
    for i in range(0, len(dobj)):
        if not i in [14,15]: # Skip crc
            byte = ord(dobj[i])
            crc += byte * (i+1)

    return crc & 0xffff


def GetFieldDef(fielddef):
    
    """
    Get the field def items

    @param fielddef:
        field format - see "Settings dictionary" above

    @return:
        <format>, <baseaddr>, <bits>, <bitshift>, <datadef>, <convert>
        undefined items can be None
    """
    _format = baseaddr = datadef = convert = None
    bits = bitshift = 0
    if len(fielddef)==3:
        # def without convert tuple
        _format, baseaddr, datadef = fielddef
    elif len(fielddef)==4:
        # def with convert tuple
        _format, baseaddr, datadef, convert = fielddef

    if isinstance(baseaddr, (list,tuple)):
        baseaddr, bits, bitshift = baseaddr

    if isinstance(datadef, int):
        # convert single int into list with one item
        datadef = [datadef]
    return _format, baseaddr, bits, bitshift, datadef, convert


def MakeFieldBaseAddr(baseaddr, bits, bitshift):
    """
    Return a <baseaddr> based on given arguments

    @param baseaddr:
        baseaddr from Settings definition
    @param bits:
        0 or bits
    @param bitshift:
        0 or bitshift

    @return:
        (<baseaddr>,<bits>,<bitshift>) if bits != 0
        baseaddr if bits == 0

    """
    if bits!=0:
        return (baseaddr, bits, bitshift)
    return baseaddr


def ConvertFieldValue(value, fielddef, read=True, raw=False):
    """
    Convert field value based on field desc

    @param value:
        original value
    @param fielddef
        field definition - see "Settings dictionary" above
    @param read
        use read conversion if True, otherwise use write conversion
    @param raw
        return raw values (True) or converted values (False)

    @return:
        (un)converted value
    """
    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)

    # call password functions even if raw value should be processed
    if callable(convert) and (convert==passwordread or convert==passwordwrite):
        raw = False
    if isinstance(convert, (list,tuple)) and len(convert)>0 and (convert[0]==passwordread or convert[0]==passwordwrite):
        raw = False
    if isinstance(convert, (list,tuple)) and len(convert)>1 and (convert[1]==passwordread or convert[1]==passwordwrite):
        raw = False

    if not raw and convert is not None:
        if isinstance(convert, (list,tuple)):  # extract read conversion if tuple is given
            if read:
                convert = convert[0]
            else:
                convert = convert[1]
        try:
            if isinstance(convert, str): # evaluate strings
                return eval(convert.replace('$','value'))
            elif callable(convert):     # use as format function
                return convert(value)
        except:
            pass

    return value


def GetFieldMinMax(fielddef):
    """
    Get minimum, maximum of field based on field format definition

    @param fielddef:
        field format - see "Settings dictionary" above

    @return:
        min, max
    """
    minmax = {'c': (0, 1),
              '?': (0, 1),
              'b': (~0x7f, 0x7f),
              'B': (0,     0xff),
              'h': (~0x7fff, 0x7fff),
              'H': (0,       0xffff),
              'i': (~0x7fffffff, 0x7fffffff),
              'I': (0,           0xffffffff),
              'l': (~0x7fffffff, 0x7fffffff),
              'L': (0,           0xffffffff),
              'q': (~0x7fffffffffffffff, 0x7fffffffffffffff),
              'Q': (0,                   0x7fffffffffffffff),
              'f': (sys.float_info.min, sys.float_info.max),
              'd': (sys.float_info.min, sys.float_info.max),
             }
    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)
    _min = 0
    _max = 0
    
    if _format[-1:] in minmax:
        _min, _max = minmax[_format[-1:]]
    elif _format[-1:] in ['s','p']:
        # s and p may have a prefix as length
        match = re.search("\s*(\d+)", _format)
        if match:
            _max=int(match.group(0))
    return _min,_max
    

def GetFieldLength(fielddef):
    """
    Get length of a field in bytes based on field format definition

    @param fielddef:
        field format - see "Settings dictionary" above

    @return:
        length of field in bytes
    """

    length=0
    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)

    if datadef is not None:
        # datadef contains a list
        # calc size recursive by sum of all elements
        if isinstance(datadef, list):
            for i in range(0, datadef[0]):

                # multidimensional array
                if isinstance(datadef, list) and len(datadef)>1:
                    length += GetFieldLength( (fielddef[0], fielddef[1], fielddef[2][1:]) )

                # single array
                else:
                    length += GetFieldLength( (fielddef[0], fielddef[1], None) )

    else:
        if isinstance(_format, dict):
            # -> iterate through _format
            addr = None
            setting = _format
            for name in setting:
                _dummy1, baseaddr, bits, bitshift, _dummy2, _dummy3 = GetFieldDef(setting[name])
                _len = GetFieldLength(setting[name])
                if addr != baseaddr:
                    addr = baseaddr
                    length += _len

        else:
            if _format[-1:] in ['b','B','c','?']:
                length=1
            elif _format[-1:] in ['h','H']:
                length=2
            elif _format[-1:] in ['i','I','l','L','f']:
                length=4
            elif _format[-1:] in ['q','Q','d']:
                length=8
            elif _format[-1:] in ['s','p']:
                # s and p may have a prefix as length
                match = re.search("\s*(\d+)", _format)
                if match:
                    length=int(match.group(0))

    return length


def GetSubfieldDef(fielddef):
    """
    Get subfield definition from a given field definition

    @param fielddef:
        see Settings desc above

    @return:
        subfield definition
    """
    subfielddef = None

    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)
    if isinstance(datadef, list) and len(datadef)>1:
        if len(fielddef)<4:
            subfielddef = (_format, MakeFieldBaseAddr(baseaddr, bits, bitshift), datadef[1:])
        else:
            subfielddef = (_format, MakeFieldBaseAddr(baseaddr, bits, bitshift), datadef[1:], convert)
    # single array
    else:
        if len(fielddef)<4:
            subfielddef = (_format, MakeFieldBaseAddr(baseaddr, bits, bitshift), None)
        else:
            subfielddef = (_format, MakeFieldBaseAddr(baseaddr, bits, bitshift), None, convert)
    return subfielddef


def GetField(dobj, fieldname, fielddef, raw=False, addroffset=0):
    """
    Get field value from definition

    @param dobj:
        decrypted binary config data
    @param fieldname:
        name of the field
    @param fielddef:
        see Settings desc above
    @param raw
        return raw values (True) or converted values (False)
    @param addroffset
        use offset for baseaddr (used for recursive calls)

    @return:
        read field value
    """

    if isinstance(dobj, bytearray):
        dobj = str(dobj)

    result = None

    if fieldname == 'raw' and not args.jsonrawkeys:
        return result

    # get field definition
    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)

    # <datadef> contains a integer list
    if isinstance(datadef, list):
        result = []
        offset = 0
        for i in range(0, datadef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            length = GetFieldLength(subfielddef)
            if length != 0:
                result.append(GetField(dobj, fieldname, subfielddef, raw=raw, addroffset=addroffset+offset))
            offset += length

    # <format> contains a dict
    elif isinstance(_format, dict):
        config = {}
        for name in _format:    # -> iterate through _format
            if name != 'raw' or args.jsonrawkeys:
                config[name] = GetField(dobj, name, _format[name], raw=raw, addroffset=addroffset)
        result = config

    # a simple value
    elif isinstance(_format, (str, bool, int, float, long)):
        if GetFieldLength(fielddef) != 0:
            result = struct.unpack_from(_format, dobj, baseaddr+addroffset)[0]

            if not _format[-1:].lower() in ['s','p']:
                if bitshift>=0:
                    result >>= bitshift
                else:
                    result <<= abs(bitshift)
                if bits>0:
                    result &= (1<<bits)-1

            # additional processing for strings
            if _format[-1:].lower() in ['s','p']:
                # use left string until \0
                s = str(result).split('\0')[0]
                # remove character > 127
                result = unicode(s, errors='ignore')

            result = ConvertFieldValue(result, fielddef, read=True, raw=raw)

    else:
        exit(ExitCode.INTERNAL_ERROR, "Wrong mapping format definition: '{}'".format(_format), typ=LogType.WARNING, doexit=not args.ignorewarning, line=inspect.getlineno(inspect.currentframe()))

    return result


def SetField(dobj, fieldname, fielddef, restore, raw=False, addroffset=0, filename=""):
    """
    Get field value from definition

    @param dobj:
        decrypted binary config data
    @param fieldname:
        name of the field
    @param fielddef:
        see Settings desc above
    @param raw
        handle values as raw values (True) or converted (False)
    @param addroffset
        use offset for baseaddr (used for recursive calls)
    @param restore
        restore mapping with the new value(s)
    """
    _format, baseaddr, bits, bitshift, datadef, convert = GetFieldDef(fielddef)
    fieldname = str(fieldname)

    if fieldname == 'raw' and not args.jsonrawkeys:
        return dobj

    # do not write readonly values
    if isinstance(convert, (list,tuple)) and len(convert)>1 and convert[1]==None:
        if args.debug:
            print >> sys.stderr, "SetField(): Readonly '{}' using '{}'/{}{} @{} skipped".format(fieldname, _format, datadef, bits, hex(baseaddr+addroffset))
        return dobj

    # <datadef> contains a list
    if isinstance(datadef, list):
        offset = 0
        if len(restore)>datadef[0]:
            exit(ExitCode.RESTORE_DATA_ERROR, "file '{sfile}', array '{sname}[{selem}]' exceeds max number of elements [{smax}]".format(sfile=filename, sname=fieldname, selem=len(restore), smax=datadef[0]), typ=LogType.WARNING, doexit=not args.ignorewarning, line=inspect.getlineno(inspect.currentframe()))
        for i in range(0, datadef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            length = GetFieldLength(subfielddef)
            if length != 0:
                if i>=len(restore): # restore data list may be shorter than definition
                    break
                try:
                    subrestore = restore[i]
                    dobj = SetField(dobj, fieldname, subfielddef, subrestore, raw=raw, addroffset=addroffset+offset, filename=filename)
                except:
                    pass
            offset += length

    # <format> contains a dict
    elif isinstance(_format, dict):
        for name in _format:    # -> iterate through _format
            if name in restore:
                dobj = SetField(dobj, name, _format[name], restore[name], raw=raw, addroffset=addroffset, filename=filename)

    # a simple value
    elif isinstance(_format, (str, bool, int, float, long)):
        valid = True
        err = "outside range"

        _min, _max = GetFieldMinMax(fielddef)
        value = _value = valid = None
        # simple one value
        if _format[-1:] in ['c']:
            try:
                value = ConvertFieldValue(restore.encode(STR_ENCODING)[0], fielddef, read=False, raw=raw)
            except:
                valid = False
        # bool
        elif _format[-1:] in ['?']:
            try:
                value = ConvertFieldValue(bool(restore), fielddef, read=False, raw=raw)
            except:
                valid = False
        # integer
        elif _format[-1:] in ['b','B','h','H','i','I','l','L','q','Q','P']:
            try:
                value = ConvertFieldValue(restore, fielddef, read=False, raw=raw)
                if isinstance(value, (str, unicode)):
                    value = int(value, 0)
                else:
                    value = int(value)
                # bits
                if bits!=0:
                    value = struct.unpack_from(_format, dobj, baseaddr+addroffset)[0]
                    bitvalue = int(restore)
                    mask = (1<<bits)-1
                    if bitvalue>mask:
                        _min = 0
                        _max = mask
                        _value = bitvalue
                        valid = False
                    else:
                        if bitshift>=0:
                            bitvalue <<= bitshift
                            mask <<= bitshift
                        else:
                            bitvalue >>= abs(bitshift)
                            mask >>= abs(bitshift)
                        value &= (0xffffffff ^ mask)
                        value |= bitvalue
                else:
                    _value = value
            except:
                valid = False
        # float
        elif _format[-1:] in ['f','d']:
            try:
                value = ConvertFieldValue(float(restore), fielddef, read=False, raw=raw)
            except:
                valid = False
        # string
        elif _format[-1:] in ['s','p']:
            try:
                value = ConvertFieldValue(restore.encode(STR_ENCODING), fielddef, read=False, raw=raw)
                # be aware 0 byte at end of string (str must be < max, not <= max)
                _max -= 1
                valid = (len(value)>=_min) and (len(value)<=_max)
                err = "string exceeds max length"
            except:
                valid = False

        if value is None:
            valid = False
        if valid is None:
            valid = (value>=_min) and (value<=_max)
        if _value is None:
            _value = value
        if isinstance(value, (str, unicode)):
            _value = "'{}'".format(_value)

        if valid:
            if args.debug:
                if bits:
                    sbits=" {} bits shift {}".format(bits, bitshift)
                else:
                    sbits = ""
                print >> sys.stderr, "SetField(): Set '{}' using '{}'/{}{} @{} to {}".format(fieldname, _format, datadef, sbits, hex(baseaddr+addroffset), _value)
            struct.pack_into(_format, dobj, baseaddr+addroffset, value)
        else:
            exit(ExitCode.RESTORE_DATA_ERROR, "file '{sfile}', value for name '{sname}': {svalue} {serror} [{smin},{smax}]".format(sfile=filename, sname=fieldname, serror=err, svalue=_value, smin=_min, smax=_max), typ=LogType.WARNING, doexit=not args.ignorewarning)

    return dobj


def Bin2Mapping(decode_cfg, raw=True):
    """
    Decodes binary data stream into pyhton mappings dict

    @param decode_cfg:
        binary config data (decrypted)
    @param raw:
        decode raw values (True) or converted values (False)

    @return:
        config data as mapping dictionary
    """
    if isinstance(decode_cfg, bytearray):
        decode_cfg = str(decode_cfg)

    # get binary header and template to use
    version, template, size, setting = GetTemplateSetting(decode_cfg)

    # if we did not found a mathching setting
    if template is None:
        exit(ExitCode.UNSUPPORTED_VERSION, "Tasmota configuration version 0x{:x} not supported".format(version),line=inspect.getlineno(inspect.currentframe()))

    # check size if exists
    if 'cfg_size' in setting:
        cfg_size = GetField(decode_cfg, 'cfg_size', setting['cfg_size'], raw=True)
        # read size should be same as definied in template
        if cfg_size > size:
            # may be processed
            exit(ExitCode.DATA_SIZE_MISMATCH, "Number of bytes read does ot match - read {}, expected {} byte".format(cfg_size, template[1]), typ=LogType.ERROR,line=inspect.getlineno(inspect.currentframe()))
        elif cfg_size < size:
            # less number of bytes can not be processed
            exit(ExitCode.DATA_SIZE_MISMATCH, "Number of bytes read to small to process - read {}, expected {} byte".format(cfg_size, template[1]), typ=LogType.ERROR,line=inspect.getlineno(inspect.currentframe()))

    # check crc if exists
    if 'cfg_crc' in setting:
        cfg_crc = GetField(decode_cfg, 'cfg_crc', setting['cfg_crc'], raw=True)
    else:
        cfg_crc = GetSettingsCrc(decode_cfg)
    if cfg_crc != GetSettingsCrc(decode_cfg):
        exit(ExitCode.DATA_CRC_ERROR, 'Data CRC error, read 0x{:x} should be 0x{:x}'.format(cfg_crc, GetSettingsCrc(decode_cfg)), typ=LogType.WARNING, doexit=not args.ignorewarning,line=inspect.getlineno(inspect.currentframe()))

    # get config
    config = GetField(decode_cfg, None, (setting,None,None), raw=raw)

    # add header info
    timestamp = datetime.now()
    config['header'] = {'timestamp':timestamp.strftime("%Y-%m-%d %H:%M:%S"),
                        'format':   {
                                    'jsonindent':   args.jsonindent,
                                    'jsoncompact':  args.jsoncompact,
                                    'jsonsort':     args.jsonsort,
                                    'jsonrawvalues':args.jsonrawvalues,
                                    'jsonrawkeys':  args.jsonrawkeys,
                                    'jsonhidepw':   args.jsonhidepw,
                                    },
                        'src':      {
                                    'crc':      hex(cfg_crc),
                                    'size':     cfg_size,
                                    'version':  hex(version),
                                    },
                        'data':     {
                                    'crc':      hex(GetSettingsCrc(decode_cfg)),
                                    'size':     len(decode_cfg),
                                    'version':  hex(template[0]),
                                    },
                        'script':   {
                                    'name':     os.path.basename(__file__),
                                    'version':  VER,
                                    },
                        'os':       (platform.machine(), platform.system(), platform.release(), platform.version(), platform.platform()),
                        'python':   platform.python_version(),
                       }

    return config


def Mapping2Bin(decode_cfg, jsonconfig, filename=""):
    """
    Encodes into binary data stream

    @param decode_cfg:
        binary config data (decrypted)
    @param jsonconfig:
        restore data mapping
    @param filename:
        name of the restore file (for error output only)
        
    @return:
        changed binary config data (decrypted)
    """
    if isinstance(decode_cfg, str):
        decode_cfg = bytearray(decode_cfg)

    
    # get binary header data to use the correct version template from device
    version, template, size, setting = GetTemplateSetting(decode_cfg)

    _buffer = bytearray()
    _buffer.extend(decode_cfg)

    if template is not None:
        try:
            raw = jsonconfig['header']['format']['jsonrawvalues']
        except:
            if 'header' not in jsonconfig:
                errkey = 'header'
            elif 'format' not in jsonconfig['header']:
                errkey = 'header.format'
            elif 'jsonrawvalues' not in jsonconfig['header']['format']:
                errkey = 'header.format.jsonrawvalues'
            exit(ExitCode.RESTORE_DATA_ERROR, "Restore file '{sfile}' name '{skey}' missing, don't know how to evaluate restore data!".format(sfile=filename, skey=errkey), typ=LogType.ERROR, doexit=not args.ignorewarning)

        # iterate through restore data mapping
        for name in jsonconfig:
            # key must exist in both dict
            if name in setting:
                SetField(_buffer, name, setting[name], jsonconfig[name], raw=raw, addroffset=0, filename=filename)
            else:
                if name != 'header':
                    exit(ExitCode.RESTORE_DATA_ERROR, "Restore file '{}' contains obsolete name '{}', skipped".format(filename, name), typ=LogType.WARNING, doexit=not args.ignorewarning)

        crc = GetSettingsCrc(_buffer)
        struct.pack_into(setting['cfg_crc'][0], _buffer, setting['cfg_crc'][1], crc)
        return _buffer

    else:
        exit(ExitCode.UNSUPPORTED_VERSION,"File '{}', Tasmota configuration version 0x{:x} not supported".format(filename, version), typ=LogType.WARNING, doexit=not args.ignorewarning)

    return decode_cfg


def Backup(backupfile, backupfileformat, encode_cfg, decode_cfg, configuration):
    """
    Create backup file

    @param backupfile:
        Raw backup filename from program args
    @param backupfileformat:
        Backup file format
    @param encode_cfg:
        binary config data (encrypted)
    @param decode_cfg:
        binary config data (decrypted)
    @param configuration:
        config data mapppings
    """

    backupfileformat = args.backupfileformat
    try:
        name, ext = os.path.splitext(backupfile)
        if ext.lower() == '.'+FileType.BIN.lower():
            backupfileformat = FileType.BIN
        elif ext.lower() == '.'+FileType.DMP.lower():
            backupfileformat = FileType.DMP
        elif ext.lower() == '.'+FileType.JSON.lower():
            backupfileformat = FileType.JSON
    except:
        pass

    fileformat = ""
    # binary format
    if backupfileformat.lower() == FileType.BIN.lower():
        fileformat = "binary"
        backup_filename = MakeFilename(backupfile, FileType.BIN, configuration)
        try:
            backupfp = open(backup_filename, "wb")
            magic = BINARYFILE_MAGIC
            backupfp.write(struct.pack('<L',magic))
            backupfp.write(decode_cfg)
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))
        finally:
            backupfp.close()

    # Tasmota format
    if backupfileformat.lower() == FileType.DMP.lower():
        fileformat = "Tasmota"
        backup_filename = MakeFilename(backupfile, FileType.DMP, configuration)
        try:
            backupfp = open(backup_filename, "wb")
            backupfp.write(encode_cfg)
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))
        finally:
            backupfp.close()

    # JSON format
    elif backupfileformat.lower() == FileType.JSON.lower():
        fileformat = "JSON"
        backup_filename = MakeFilename(backupfile, FileType.JSON, configuration)
        try:
            backupfp = open(backup_filename, "w")
            json.dump(configuration, backupfp, sort_keys=args.jsonsort, indent=None if args.jsonindent<0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))
        finally:
            backupfp.close()
    if args.verbose:
        srctype = 'device'
        src = args.device
        if args.tasmotafile is not None:
            srctype = 'file'
            src = args.tasmotafile
        message("Backup successful from {} '{}' using {} format to file '{}' ".format(srctype, src, fileformat, backup_filename), typ=LogType.INFO)


def Restore(restorefile, encode_cfg, decode_cfg, configuration):
    """
    Restore from file

    @param encode_cfg:
        binary config data (encrypted)
    @param decode_cfg:
        binary config data (decrypted)
    @param configuration:
        config data mapppings
    """

    new_encode_cfg = None

    restorefilename = MakeFilename(restorefile, None, configuration)
    filetype = GetFileType(restorefilename)

    if filetype == FileType.DMP:
        try:
            restorefp = open(restorefilename, "rb")
            new_encode_cfg = restorefp.read()
            restorefp.close()
        except Exception, e:
            exit(e[0], "'{}' {}".format(restorefilename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    elif filetype == FileType.BIN:
        try:
            restorefp = open(restorefilename, "rb")
            restorebin = restorefp.read()
            restorefp.close()
        except Exception, e:
            exit(e[0], "'{}' {}".format(restorefilename, e[1]),line=inspect.getlineno(inspect.currentframe()))
        header = struct.unpack_from('<L', restorebin, 0)[0]
        if header == BINARYFILE_MAGIC:
            decode_cfg = restorebin[4:]                     # remove header from encrypted config file
            new_encode_cfg = DecryptEncrypt(decode_cfg)     # process binary to binary config

    elif filetype == FileType.JSON or filetype == FileType.INVALID_JSON:
        try:
            restorefp = open(restorefilename, "r")
            jsonconfig = json.load(restorefp)
        except ValueError as e:
            exit(ExitCode.JSON_READ_ERROR, "File '{}' invalid JSON: {}".format(restorefilename, e), line=inspect.getlineno(inspect.currentframe()))
        finally:
            restorefp.close()
        # process json config to binary config
        new_decode_cfg = Mapping2Bin(decode_cfg, jsonconfig, restorefilename)
        new_encode_cfg = DecryptEncrypt(new_decode_cfg)

    elif filetype == FileType.FILE_NOT_FOUND:
        exit(ExitCode.FILE_NOT_FOUND, "File '{}' not found".format(restorefilename),line=inspect.getlineno(inspect.currentframe()))
    elif filetype == FileType.INCOMPLETE_JSON:
        exit(ExitCode.JSON_READ_ERROR, "File '{}' incomplete JSON, missing name 'header'".format(restorefilename),line=inspect.getlineno(inspect.currentframe()))
    elif filetype == FileType.INVALID_BIN:
        exit(ExitCode.FILE_READ_ERROR, "File '{}' invalid BIN format".format(restorefilename),line=inspect.getlineno(inspect.currentframe()))
    else:
        exit(ExitCode.FILE_READ_ERROR, "File '{}' unknown error".format(restorefilename),line=inspect.getlineno(inspect.currentframe()))

    if new_encode_cfg is not None:
        if new_encode_cfg != encode_cfg or args.ignorewarning:
            # write config direct to device via http
            if args.device is not None:
                error_code, error_str = PushTasmotaConfig(new_encode_cfg, args.device, args.port, args.username, args.password)
                if error_code:
                    exit(ExitCode.UPLOAD_CONFIG_ERROR, "Config data upload failed - {}".format(error_str),line=inspect.getlineno(inspect.currentframe()))
                else:
                    if args.verbose:
                        message("Restore successful to device '{}' using restore file '{}'".format(args.device, restorefilename), typ=LogType.INFO)

            # write config from a file
            elif args.tasmotafile is not None:
                try:
                    outputfile = open(args.tasmotafile, "wb")
                    outputfile.write(new_encode_cfg)
                except Exception, e:
                    exit(e[0], "'{}' {}".format(args.tasmotafile, e[1]),line=inspect.getlineno(inspect.currentframe()))
                finally:
                    outputfile.close()
                    if args.verbose:
                        message("Restore successful to file '{}' using restore file '{}'".format(args.tasmotafile, restorefilename), typ=LogType.INFO)

        else:
            global exitcode
            exitcode = ExitCode.RESTORE_SKIPPED
            if args.verbose:
                exit(exitcode, "Configuration data unchanged, upload skipped", typ=LogType.WARNING)


def ParseArgs():
    """
    Program argument parser
    
    @return:
        configargparse.parse_args() result
    """
    global parser
    parser = configargparse.ArgumentParser(description='Backup/Restore Sonoff-Tasmota configuration data.',
                                           epilog='Either argument -d <host> or -f <filename> must be given.',
                                           add_help=False,
                                           formatter_class=lambda prog: CustomHelpFormatter(prog))

    source = parser.add_argument_group('Source', 'Read/Write Tasmota configuration from/to')
    source.add_argument('-f', '--file', '--tasmota-file',
                            metavar='<filename>',
                            dest='tasmotafile',
                            default=DEFAULTS['source']['tasmotafile'],
                            help="file to retrieve/write Tasmota configuration from/to (default: {})'".format(DEFAULTS['source']['tasmotafile']))
    source.add_argument('-d', '--device', '--host',
                            metavar='<host>',
                            dest='device',
                            default=DEFAULTS['source']['device'],
                            help="hostname or IP address to retrieve/send Tasmota configuration from/to (default: {})".format(DEFAULTS['source']['device']) )
    source.add_argument('-P', '--port',
                            metavar='<port>',
                            dest='port',
                            default=DEFAULTS['source']['port'],
                            help="TCP/IP port number to use for the host connection (default: {})".format(DEFAULTS['source']['port']) )
    source.add_argument('-u', '--username',
                            metavar='<username>',
                            dest='username',
                            default=DEFAULTS['source']['username'],
                            help="host HTTP access username (default: {})".format(DEFAULTS['source']['username']))
    source.add_argument('-p', '--password',
                            metavar='<password>',
                            dest='password',
                            default=DEFAULTS['source']['password'],
                            help="host HTTP access password (default: {})".format(DEFAULTS['source']['password']))

    backup = parser.add_argument_group('Backup/Restore', 'Backup/Restore configuration file specification')
    backup.add_argument('-i', '--restore-file',
                            metavar='<filename>',
                            dest='restorefile',
                            default=DEFAULTS['backup']['backupfile'],
                            help="file to restore configuration from (default: {}). Replacements: @v=firmware version, @f=device friendly name, @h=device hostname".format(DEFAULTS['backup']['restorefile']))
    backup.add_argument('-o', '--backup-file',
                            metavar='<filename>',
                            dest='backupfile',
                            default=DEFAULTS['backup']['backupfile'],
                            help="file to backup configuration to (default: {}). Replacements: @v=firmware version, @f=device friendly name, @h=device hostname".format(DEFAULTS['backup']['backupfile']))
    output_file_formats = ['json', 'bin', 'dmp']
    backup.add_argument('-F', '--backup-type',
                            metavar='|'.join(output_file_formats),
                            dest='backupfileformat',
                            choices=output_file_formats,
                            default=DEFAULTS['backup']['backupfileformat'],
                            help="backup filetype (default: '{}')".format(DEFAULTS['backup']['backupfileformat']) )
    backup.add_argument('-E', '--extension',
                            dest='extension',
                            action='store_true',
                            default=DEFAULTS['backup']['extension'],
                            help="append filetype extension for -i and -o filename{}".format(' (default)' if DEFAULTS['backup']['extension'] else '') )
    backup.add_argument('-e', '--no-extension',
                            dest='extension',
                            action='store_false',
                            default=DEFAULTS['backup']['extension'],
                            help="do not append filetype extension, use -i and -o filename as passed{}".format(' (default)' if not DEFAULTS['backup']['extension'] else '') )

    jsonformat = parser.add_argument_group('JSON', 'JSON backup format specification')
    jsonformat.add_argument('--json-indent',
                            metavar='<indent>',
                            dest='jsonindent',
                            type=int,
                            default=DEFAULTS['jsonformat']['jsonindent'],
                            help="pretty-printed JSON output using indent level (default: '{}'). -1 disables indent.".format(DEFAULTS['jsonformat']['jsonindent']) )
    jsonformat.add_argument('--json-compact',
                            dest='jsoncompact',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsoncompact'],
                            help="compact JSON output by eliminate whitespace{}".format(' (default)' if DEFAULTS['jsonformat']['jsoncompact'] else '') )

    jsonformat.add_argument('--json-sort',
                            dest='jsonsort',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsonsort'],
                            help=configargparse.SUPPRESS) #"sort json keywords{}".format(' (default)' if DEFAULTS['jsonformat']['jsonsort'] else '') )
    jsonformat.add_argument('--json-unsort',
                            dest='jsonsort',
                            action='store_false',
                            default=DEFAULTS['jsonformat']['jsonsort'],
                            help=configargparse.SUPPRESS) #"do not sort json keywords{}".format(' (default)' if not DEFAULTS['jsonformat']['jsonsort'] else '') )

    jsonformat.add_argument('--json-raw-values',
                            dest='jsonrawvalues',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsonrawvalues'],
                            help=configargparse.SUPPRESS) #"output raw values{}".format(' (default)' if DEFAULTS['jsonformat']['jsonrawvalues'] else '') )
    jsonformat.add_argument('--json-convert-values',
                            dest='jsonrawvalues',
                            action='store_false',
                            default=DEFAULTS['jsonformat']['jsonrawvalues'],
                            help=configargparse.SUPPRESS) #"output converted, human readable values{}".format(' (default)' if not DEFAULTS['jsonformat']['jsonrawvalues'] else '') )

    jsonformat.add_argument('--json-raw-keys',
                            dest='jsonrawkeys',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsonrawkeys'],
                            help=configargparse.SUPPRESS) #"output bitfield raw keys{}".format(' (default)' if DEFAULTS['jsonformat']['jsonrawkeys'] else '') )
    jsonformat.add_argument('--json-no-raw-keys',
                            dest='jsonrawkeys',
                            action='store_false',
                            default=DEFAULTS['jsonformat']['jsonrawkeys'],
                            help=configargparse.SUPPRESS) #"do not output bitfield raw keys{}".format(' (default)' if not DEFAULTS['jsonformat']['jsonrawkeys'] else '') )

    jsonformat.add_argument('--json-hide-pw',
                            dest='jsonhidepw',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsonhidepw'],
                            help="hide passwords{}".format(' (default)' if DEFAULTS['jsonformat']['jsonhidepw'] else '') )
    jsonformat.add_argument('--json-unhide-pw',
                            dest='jsonhidepw',
                            action='store_false',
                            default=DEFAULTS['jsonformat']['jsonhidepw'],
                            help="unhide passwords{}".format(' (default)' if not DEFAULTS['jsonformat']['jsonhidepw'] else '') )

    info = parser.add_argument_group('Info','additional information')
    info.add_argument('-D', '--debug',
                            dest='debug',
                            action='store_true',
                            help=configargparse.SUPPRESS)
    info.add_argument('-h', '--help',
                            dest='shorthelp',
                            action='store_true',
                            help='show usage help message and exit')
    info.add_argument("-H", "--full-help",
                            action="help",
                            help="show full help message and exit")
    info.add_argument('-v', '--verbose',
                            dest='verbose',
                            action='store_true',
                            help='produce more output about what the program does')
    info.add_argument('-V', '--version',
                            action='version',
                            version=PROG)

    # optional arguments
    parser.add_argument('-c', '--config',
                            metavar='<filename>',
                            dest='configfile',
                            default=DEFAULTS['DEFAULT']['configfile'],
                            is_config_file=True,
                            help="program config file - can be used to set default command args (default: {})".format(DEFAULTS['DEFAULT']['configfile']) )
    parser.add_argument('--ignore-warnings',
                            dest='ignorewarning',
                            action='store_true',
                            default=DEFAULTS['DEFAULT']['ignorewarning'],
                            help="do not exit on warnings{}. Not recommended, used by your own responsibility!".format(' (default)' if DEFAULTS['DEFAULT']['ignorewarning'] else '') )

    args = parser.parse_args()

    if args.debug:
        print >> sys.stderr, parser.format_values()
        print >> sys.stderr, "Settings:"
        for k in args.__dict__:
            print >> sys.stderr, "  "+str(k), "= ",eval('args.{}'.format(k))
    return args


if __name__ == "__main__":
    args = ParseArgs()
    if args.shorthelp:
        ShortHelp()

    # default no configuration available
    encode_cfg = None

    # check source args
    if args.device is not None and args.tasmotafile is not None:
        exit(ExitCode.ARGUMENT_ERROR, "Unable to select source, do not use -d and -f together",line=inspect.getlineno(inspect.currentframe()))

    # pull config from Tasmota device/file
    encode_cfg = PullTasmotaConfig()
    if encode_cfg is None:
        # no config source given
        ShortHelp(False)
        print
        print parser.epilog
        sys.exit(ExitCode.OK)

    if len(encode_cfg) == 0:
        exit(ExitCode.FILE_READ_ERROR, "Unable to read configuration data from {} '{}'".format('device' if args.device is not None else 'file', \
                                                                        args.device if args.device is not None else args.tasmotafile) \
                                                                        ,line=inspect.getlineno(inspect.currentframe()) )
    # decrypt Tasmota config
    decode_cfg = DecryptEncrypt(encode_cfg)

    # decode into mappings dictionary
    configuration = Bin2Mapping(decode_cfg, args.jsonrawvalues)

    # backup to file
    if args.backupfile is not None:
        Backup(args.backupfile, args.backupfileformat, encode_cfg, decode_cfg, configuration)

    # restore from file
    if args.restorefile is not None:
        Restore(args.restorefile, encode_cfg, decode_cfg, configuration)

    # json screen output
    if args.backupfile is None and args.restorefile is None:
        print json.dumps(configuration, sort_keys=args.jsonsort, indent=None if args.jsonindent<0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )

    sys.exit(exitcode)
