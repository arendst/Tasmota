#!/usr/bin/env python
#!/usr/bin/env python
# -*- coding: utf-8 -*-
VER = '1.5.0012'

"""
    decode-config.py - Decode configuration of Sonoff-Tasmota device

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
    or use -f to read out a configuration file saved using Tasmota Web-UI

    For help execute command with argument -h


Usage:
    decode-config.py [-h] [-f <filename>] [-d <host>] [-u <user>]
                     [-p <password>] [--json-indent <integer>]
                     [--json-compact] [--sort] [--unsort] [--raw-values]
                     [--no-raw-values] [--raw-keys] [--no-raw-keys]
                     [--hide-pw] [--unhide-pw] [-o <filename>]
                     [--output-file-format <word>] [-c <filename>]
                     [--exit-on-error-only] [-V]

    Decode configuration of Sonoff-Tasmota device. Args that start with '--' (eg.
    -f) can also be set in a config file (specified via -c). Config file syntax
    allows: key=value, flag=true, stuff=[a,b,c] (for details, see syntax at
    https://goo.gl/R74nmi). If an arg is specified in more than one place, then
    commandline values override config file values which override defaults.

    optional arguments:
      -h, --help            show this help message and exit
      -c <filename>, --config <filename>
                            Config file, can be used instead of command parameter
                            (default: None)
      --exit-on-error-only  exit on error only (default: exit on ERROR and
                            WARNING). Not recommended, used by your own
                            responsibility!

    source:
      -f <filename>, --file <filename>
                            file to retrieve Tasmota configuration from (default:
                            None)'
      -d <host>, --device <host>
                            hostname or IP address to retrieve Tasmota
                            configuration from (default: None)
      -u <user>, --username <user>
                            host HTTP access username (default: admin)
      -p <password>, --password <password>
                            host HTTP access password (default: None)

    config:
      --json-indent <integer>
                            pretty-printed JSON output using indent level
                            (default: 'None'). Use values greater equal 0 to
                            indent or -1 to disabled indent.
      --json-compact        compact JSON output by eliminate whitespace
      --sort                sort json keywords (default)
      --unsort              do not sort json keywords
      --raw-values, --raw   output raw values
      --no-raw-values       output human readable values (default)
      --raw-keys            output bitfield raw keys (default)
      --no-raw-keys         do not output bitfield raw keys
      --hide-pw             hide passwords (default)
      --unhide-pw           unhide passwords
      -o <filename>, --output-file <filename>
                            file to store configuration to (default: None).
                            Replacements: @v=Tasmota version, @f=friendly name
      --output-file-format <word>
                            output format ('json' or 'binary', default: 'json')

    info:
      -V, --version         show program's version number and exit

    Either argument -d <host> or -f <filename> must be given.


Returns:
    0: successful
    1: file not found
    2: configuration version not supported
    3: data size mismatch
    4: data CRC error
    5: configuration file read error
    6: argument error
    9: python module is missing
    4xx, 5xx: HTTP error

"""

import os.path
import io
import sys
def ModuleImportError(module):
    er = str(module)
    print("{}. Try 'pip install {}' to install it".format(er,er.split(' ')[len(er.split(' '))-1]) )
    sys.exit(9)
try:
    import struct
    import re
    import math
    from datetime import datetime
    import json
    import configargparse
    import pycurl
    import urllib2
except ImportError, e:
    ModuleImportError(e)


PROG='{} v{} by Norbert Richter'.format(os.path.basename(sys.argv[0]),VER)

CONFIG_FILE_XOR   = 0x5A
BINARYFILE_MAGIC  = 0x63576223

args = {}
DEFAULTS = {
    'DEFAULT':
    {
        'configfile':   None,
        'exitonwarning':True,
    },
    'source':
    {
        'device':       None,
        'username':     'admin',
        'password':     None,
        'tasmotafile':  None,
    },
    'config':
    {
        'jsonindent':   None,
        'jsoncompact':  False,
        'sort':         True,
        'rawvalues':    False,
        'rawkeys':      True,
        'hidepw':       True,
        'outputfile':   None,
        'outputfileformat': 'json',
    },
}
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
            Define the field interpretation different from simple
            standard types (like char, byte, int) e. g. lists or bit fields
            Can be None, a single integer, a list or a dictionary
                None:
                    None must be given if the field contains a simple value
                    desrcibed by the <format> prefix
                n:
                    Same as [n] below
                [n]:
                    Defines a one-dimensional array of size <n>
                [n, n <,n...>]
                    Defines a multi-dimensional array

        convert (optional)
            Define an output/conversion methode, can be a simple string
            or a previously defined function name.
            'xxx?':
                a string will be evaluate as is replacing all '?' chars
                with the current value. This can also be contain pyhton
                code.
            func:
                a function defines the name of a formating function

"""
# config data conversion function and helper
def int2ip(value):
    return '{:d}.{:d}.{:d}.{:d}'.format(value & 0xff, value>>8 & 0xff, value>>16 & 0xff, value>>24 & 0xff)

def password(value):
    if args.hidepw:
        return '********'
    return value


Setting_6_2_1_10 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, hex),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None, '? + 8'),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None, '? + 8'),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None, '? + 8'),
                                     },     0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'display_rotate':               ('B',   0x2FA, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'display_font':                 ('B',   0x312, None),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'energy_max_power_limit_hold':  ('<H',  0x388, None),
    'energy_max_power_limit_window':('<H',  0x38A, None),
    'energy_max_power_safe_limit':  ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, '"0x{:08x}".format(?)'),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                         'time_append_timezone':('<L',  (0x3A0, 1, 2), None),
                                     },     0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'button_debounce':              ('<H',  0x542, None),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
    'rgbwwTable':                   ('B',   0x71A, [5]),
    'energy_frequency_calibration': ('<L',  0x7C8, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_2_1_6 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, hex),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None, '? + 8'),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None, '? + 8'),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None, '? + 8'),
                                     },     0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'display_rotate':               ('B',   0x2FA, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'display_font':                 ('B',   0x312, None),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'energy_max_power_limit_hold':  ('<H',  0x388, None),
    'energy_max_power_limit_window':('<H',  0x38A, None),
    'energy_max_power_safe_limit':  ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, '"0x{:08x}".format(?)'),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                         'time_append_timezone':('<L',  (0x3A0, 1, 2), None),
                                     },     0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'button_debounce':              ('<H',  0x542, None),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
    'energy_frequency_calibration': ('<L',  0x7C8, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_2_1_3 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, hex),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None, '? + 8'),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None, '? + 8'),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None, '? + 8'),
                                     },     0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'display_rotate':               ('B',   0x2FA, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'display_font':                 ('B',   0x312, None),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'energy_max_power_limit_hold':  ('<H',  0x388, None),
    'energy_max_power_limit_window':('<H',  0x38A, None),
    'energy_max_power_safe_limit':  ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, '"0x{:08x}".format(?)'),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                         'time_append_timezone':('<L',  (0x3A0, 1, 2), None),
                                     },     0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'button_debounce':              ('<H',  0x542, None),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_2_1_2 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, hex),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None, '? + 8'),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None, '? + 8'),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None, '? + 8'),
                                     },     0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'display_rotate':               ('B',   0x2FA, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'display_font':                 ('B',   0x312, None),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'energy_max_power_limit_hold':  ('<H',  0x388, None),
    'energy_max_power_limit_window':('<H',  0x38A, None),
    'energy_max_power_safe_limit':  ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, '"0x{:08x}".format(?)'),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1, 1), None),
                                     },     0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'button_debounce':              ('<H',  0x542, None),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_2_1 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, hex),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ({
                                        'raw':      ('B',   0x1A7,        None),
                                        'rule1':    ('B',  (0x1A7, 1, 0), None, '? + 8'),
                                        'rule2':    ('B',  (0x1A7, 1, 1), None, '? + 8'),
                                        'rule3':    ('B',  (0x1A7, 1, 2), None, '? + 8'),
                                     },     0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'display_rotate':               ('B',   0x2FA, None),
    'ledstate':                     ('B',   0x2FB, None),
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'display_font':                 ('B',   0x312, None),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'energy_max_power_limit_hold':  ('<H',  0x388, None),
    'energy_max_power_limit_window':('<H',  0x38A, None),
    'energy_max_power_safe_limit':  ('<H',  0x38C, None),
    'energy_max_power_safe_limit_hold':('<H',  0x38E, None),
    'energy_max_power_safe_limit_window':('<H',  0x390, None),
    'energy_max_energy':            ('<H',  0x392, None),
    'energy_max_energy_start':      ('<H',  0x394, None),
    'mqtt_retry':                   ('<H',  0x396, None),
    'poweronstate':                 ('B',   0x398, None),
    'last_module':                  ('B',   0x399, None),
    'blinktime':                    ('<H',  0x39A, None),
    'blinkcount':                   ('<H',  0x39C, None),
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ({
                                         'raw':                 ('<L',   0x3A0,        None, '"0x{:08x}".format(?)'),
                                         'timers_enable':       ('<L',  (0x3A0, 1, 0), None),
                                         'user_esp8285_enable': ('<L',  (0x3A0, 1,31), None),
                                     },     0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'button_debounce':              ('<H',  0x542, None),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H',  0x718, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_1_1 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'light_rotation':               ('<H',  0x39E, None),
    'flag3':                        ('<L',  0x3A0, None, '"0x{:08x}".format(?)'),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ({
                                        'raw':              ('<L',   0x6F6,         None, '"0x{:08x}".format(?)'),
                                        'pinmode':          ('<L',  (0x6F6, 3,  0), None),
                                        'pullup':           ('<L',  (0x6F6, 1,  3), None),
                                        'saved_state':      ('<L',  (0x6F6, 1,  4), None),
                                        'int_report_mode':  ('<L',  (0x6F6, 2,  5), None),
                                        'int_report_defer': ('<L',  (0x6F6, 4,  7), None),
                                        'int_count_en':     ('<L',  (0x6F6, 1, 11), None),
                                     },     0x6F6, [16]),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_0_0 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'light_rotation':               ('<H',  0x39E, None),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'rule_enabled':                 ({
                                        'raw':      ('B',   0x49F,        None),
                                        'rule1':    ('B',  (0x49F, 1, 0), None),
                                        'rule2':    ('B',  (0x49F, 1, 1), None),
                                        'rule3':    ('B',  (0x49F, 1, 2), None),
                                     },     0x49F, None),
    'rule_once':                    ({
                                        'raw':      ('B',   0x4A0,        None),
                                        'rule1':    ('B',  (0x4A0, 1, 0), None, '? + 4'),
                                        'rule2':    ('B',  (0x4A0, 1, 1), None, '? + 4'),
                                        'rule3':    ('B',  (0x4A0, 1, 2), None, '? + 4'),
                                     },     0x4A0, None),
    'light_fade':                   ('B',   0x4A1, None),
    'light_speed':                  ('B',   0x4A2, None),
    'light_scheme':                 ('B',   0x4A3, None),
    'light_width':                  ('B',   0x4A4, None),
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_5_14_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'tflag':                        ({
                                        'raw':      ('<H',   0x2E2,         None, '"0x{:04x}".format(?)'),
                                        'hemis':    ('<H',  (0x2E2, 1,  0), None),
                                        'week':     ('<H',  (0x2E2, 3,  1), None),
                                        'month':    ('<H',  (0x2E2, 4,  4), None),
                                        'dow':      ('<H',  (0x2E2, 3,  8), None),
                                        'hour':     ('<H',  (0x2E2, 5, 13), None),
                                     },     0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'param':                        ('B',   0x2FC, [18]),
    'toffset':                      ('<h',  0x30E, [2]),
    'state_text':                   ('11s', 0x313, [4]),
    'energy_power_delta':           ('B',   0x33F, None),
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
    'light_rotation':               ('<H',  0x39E, None),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'rules':                        ('512s',0x800, None)
}
Setting_5_13_1 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, '? * 1200'),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2]),
    'mqtt_host':                    ('33s', 0x1E9, None),
    'mqtt_port':                    ('<H',  0x20A, None),
    'mqtt_client':                  ('33s', 0x20C, None),
    'mqtt_user':                    ('33s', 0x22D, None),
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'energy_power_delta':           ('B',   0x33F, None),
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
    'light_rotation':               ('<H',  0x39E, None),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('B',   0x451, None),
    'sbaudrate':                    ('B',   0x452, None),
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
    'knx_GA_registered':            ('B',   0x4A5, None),
    'light_wakeup':                 ('<H',  0x4A6, None),
    'knx_CB_registered':            ('B',   0x4A8, None),
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
    'timer':                        ({
                                        'raw':      ('<L',   0x670,          None, '"0x{:08x}".format(?)'),
                                        'time':     ('<L',  (0x670, 11,  0), None),
                                        'window':   ('<L',  (0x670,  4, 11), None),
                                        'repeat':   ('<L',  (0x670,  1, 15), None),
                                        'days':     ('<L',  (0x670,  7, 16), None),
                                        'device':   ('<L',  (0x670,  4, 23), None),
                                        'power':    ('<L',  (0x670,  2, 27), None),
                                        'mode':     ('<L',  (0x670,  2, 29), None),
                                        'arm':      ('<L',  (0x670,  1, 31), None),
                                     },     0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, 'float(?) / 1000000'),
    'longitude':                    ('i',   0x6B4, None, 'float(?) / 1000000'),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'rules':                        ('512s',0x800, None)
}
Setting_5_12_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
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
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
}
Setting_5_11_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
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
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'display_model':                ('B',   0x2D2, None),
    'display_mode':                 ('B',   0x2D3, None),
    'display_refresh':              ('B',   0x2D4, None),
    'display_rows':                 ('B',   0x2D5, None),
    'display_cols':                 ('B',   0x2D6, [2]),
    'display_address':              ('B',   0x2D8, [8]),
    'display_dimmer':               ('B',   0x2E0, None),
    'display_size':                 ('B',   0x2E1, None),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
}
Setting_5_10_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '"0x{:x}".format(?)'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ({
                                        'raw':                      ('<L',   0x010,         None, '"0x{:08x}".format(?)'),
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
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
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
    'mqtt_pwd':                     ('33s', 0x24E, None, password),
    'mqtt_topic':                   ('33s', 0x26F, None),
    'button_topic':                 ('33s', 0x290, None),
    'mqtt_grptopic':                ('33s', 0x2B1, None),
    'mqtt_fingerprinth':            ('B',   0x2D2, [20]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ({
                                        'raw':      ('<L',   0x2E8,        None, '"0x{:08x}".format(?)'),
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
    'web_password':                 ('33s', 0x4A9, None, password),
    'switchmode':                   ('B',   0x4CA, [4]),
    'ntp_server':                   ('33s', 0x4CE, [3]),
    'ina219_mode':                  ('B',   0x531, None),
    'pulse_timer':                  ('<H',  0x532, [8]),
    'ip_address':                   ('<L',  0x544, [4], int2ip),
    'energy_kWhtotal':              ('<L',  0x554, None),
    'mqtt_fulltopic':               ('100s',0x558, None),
    'flag2':                        ({
                                        'raw':                      ('<L',   0x5BC,         None, '"0x{:08x}".format(?)'),
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
    'rf_code':                      ('B',   0x5D4, [17,9], '"0x{:02x}".format(?)'),
}

Settings = [(0x602010A, 0xe00, Setting_6_2_1_10),
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
class Log:
    INFO = 'INFO'
    WARNING = 'WARNING'
    ERROR = 'ERROR'

    def message(self, msg, typ=None, status=None, jsonformat=False):
        """
        Writes a message to stdout

            @param msg: string
                message to output
                if msg is of type dict, json format will be used
        """
        if jsonformat:
            message = {}
            message['msg'] = msg
            if type is not None:
                message['type'] = typ
            if status is not None:
                message['status'] = status
            print json.dumps( message )
        else:
            print '{}{} {}{} {}'.format(typ if typ is not None else '',
                                        ' ' if status is not None and typ is not None else '',
                                        status if status is not None else '',
                                        ':' if typ is not None else '',
                                        msg)


def exit(status=0, message="end", typ='ERROR', doexit=True):
    """
    Called when the program should be exit

    @param status:
        the exit status program returns to callert
    @param message:
        the message logged before exit
    @param typ:
        message type: 'INFO', 'WARNING' or 'ERROR'
    @param doexit:
        True to exit program, otherwise return
    """

    logger = Log()
    logger.message(message, typ=typ if status!=0 else 'INFO', status=status, jsonformat=True )
    exitcode = status
    if doexit:
        sys.exit(exitcode)


# ----------------------------------------------------------------------
# Tasmota config data handling
# ----------------------------------------------------------------------
def GetFilenameReplaced(filename, configuration):
    """
    Replace variable within a filename

    @param filename:
        original filename possible containing replacements:
        @v:
            Tasmota version
        @f:
            FriendlyName

    @return: New filename with replacements
    """
    v = f1 = f2 = f3 = f4 = ''
    if 'version' in configuration:
        ver = int(str(configuration['version']), 0)
        major = ((ver>>24) & 0xff)
        minor = ((ver>>16) & 0xff)
        release = ((ver>> 8) & 0xff)
        subrelease = (ver & 0xff)
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
        v = "{:d}.{:d}.{:d}{}{}".format( major, minor, release, '.' if (major>=6 and subreleasestr!='') else '', subreleasestr)
        filename = filename.replace('@v', v)
    if 'friendlyname' in configuration:
        filename = filename.replace('@f', configuration['friendlyname'][0] )

    return filename


def GetSettingsCrc(dobj):
    """
    Return binary config data calclulated crc

    @param dobj:
        decrypted binary config data

    @return: 2 byte unsigned integer crc value

    """
    crc = 0
    for i in range(0, len(dobj)):
        if not i in [14,15]: # Skip crc
            crc += ord(dobj[i]) * (i+1)
    return crc & 0xffff


def GetFieldFormat(fielddef):
    """
    Return the format item of field definition

    @param fielddef:
        field format - see "Settings dictionary" above

    @return: <format> from fielddef[0]

    """
    return fielddef[0]


def GetFieldBaseAddr(fielddef):
    """
    Return the format item of field definition

    @param fielddef:
        field format - see "Settings dictionary" above

    @return: <baseaddr>,<bits>,<bitshift> from fielddef[1]

    """
    baseaddr = fielddef[1]
    if isinstance(baseaddr, tuple):
        return baseaddr[0], baseaddr[1], baseaddr[2]

    return baseaddr, 0, 0


def MakeFieldBaseAddr(baseaddr, bitlen, bitshift):
    """
    Return a <baseaddr> based on given arguments

    @param baseaddr:
        baseaddr from Settings definition
    @param bitlen:
        0 or bitlen
    @param bitshift:
        0 or bitshift

    @return: (<baseaddr>,<bitlen>,<bitshift>) if bitlen != 0
            baseaddr if bitlen == 0

    """
    if bitlen!=0:
        return (baseaddr, bitlen, bitshift)
    return baseaddr


def ConvertFieldValue(value, fielddef, raw=False):
    """
    Convert field value based on field desc

    @param value:
        original value read from binary data
    @param fielddef
        field definition - see "Settings dictionary" above
    @param raw
        return raw values (True) or converted values (False)

    @return: (un)converted value
    """
    if not raw and len(fielddef)>3:
        convert = fielddef[3]
        if isinstance(convert,str): # evaluate strings
            try:
                return eval(convert.replace('?','value'))
            except:
                return value
        elif callable(convert):     # use as format function
            return convert(value)
    return value


def GetFieldLength(fielddef):
    """
    Return length of a field in bytes based on field format definition

    @param fielddef:
        field format - see "Settings dictionary" above

    @return: length of field in bytes

    """

    length=0
    format_ = GetFieldFormat(fielddef)

    # get datadef from field definition
    datadef = None
    if len(fielddef)>2:
        datadef = fielddef[2]

    if datadef is not None:
        # fielddef[2] contains a array or int
        # calc size recursive by sum of all elements

        # <datadef> contains a integer list or an single integer value
        if (isinstance(datadef, list) \
            and len(datadef)>0 \
            and isinstance(datadef[0], int)) \
           or isinstance(datadef, int):

            for i in range(0, datadef[0] if isinstance(datadef, list) else datadef ):

                # multidimensional array
                if isinstance(datadef, list) and len(datadef)>1:
                    length += GetFieldLength( (fielddef[0], fielddef[1], fielddef[2][1:]) )

                # single array
                else:
                    length += GetFieldLength( (fielddef[0], fielddef[1], None) )

    else:
        if isinstance(fielddef[0], dict):
            # -> iterate through format_
            addr = -1
            setting = fielddef[0]
            for name in setting:
                baseaddr, bitlen, bitshift = GetFieldBaseAddr(setting[name])
                len_ = GetFieldLength(setting[name])
                if addr != baseaddr:
                    addr = baseaddr
                    length += len_

        else:
            if format_[-1:].lower() in ['b','c','?']:
                length=1
            elif format_[-1:].lower() in ['h']:
                length=2
            elif format_[-1:].lower() in ['i','l','f']:
                length=4
            elif format_[-1:].lower() in ['q','d']:
                length=8
            elif format_[-1:].lower() in ['s','p']:
                # s and p may have a prefix as length
                match = re.search("\s*(\d+)", format_)
                if match:
                    length=int(match.group(0))

    return length


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

    @return: read field value
    """

    result = None

    # get format from field definition
    format_ = GetFieldFormat(fielddef)

    # get baseaddr from field definition
    baseaddr, bitlen, bitshift = GetFieldBaseAddr(fielddef)

    # get datadef from field definition
    datadef = None
    if fielddef is not None and len(fielddef)>2:
        datadef = fielddef[2]

    if datadef is not None:
        result = []

        # <datadef> contains a integer list or an single integer value
        if (isinstance(datadef, list) \
            and len(datadef)>0 \
            and isinstance(datadef[0], int)) \
           or isinstance(datadef, int):

            offset = 0
            for i in range(0, datadef[0] if isinstance(datadef, list) else datadef):

                # multidimensional array
                if isinstance(datadef, list) and len(datadef)>1:
                    if len(fielddef)<4:
                        subfielddef = (fielddef[0], MakeFieldBaseAddr(baseaddr, bitlen, bitshift), datadef[1:])
                    else:
                        subfielddef = (fielddef[0], MakeFieldBaseAddr(baseaddr, bitlen, bitshift), datadef[1:], fielddef[3])

                # single array
                else:
                    if len(fielddef)<4:
                        subfielddef = (fielddef[0], MakeFieldBaseAddr(baseaddr, bitlen, bitshift), None)
                    else:
                        subfielddef = (fielddef[0], MakeFieldBaseAddr(baseaddr, bitlen, bitshift), None, fielddef[3])

                length = GetFieldLength(subfielddef)
                if length != 0 and (fieldname != 'raw' or args.rawkeys):
                    result.append(GetField(dobj, fieldname, subfielddef, raw=raw, addroffset=addroffset+offset))
                offset += length

    else:
        # <format> contains a dict
        if isinstance(fielddef[0], dict):
            # -> iterate through format_
            setting = fielddef[0]
            config = {}
            for name in setting:
                if name != 'raw' or args.rawkeys:
                    config[name] = GetField(dobj, name, setting[name], raw=raw, addroffset=addroffset)
            result = config
        else:
            # a simple value
            if GetFieldLength(fielddef) != 0:
                result = struct.unpack_from(format_, dobj, baseaddr+addroffset)[0]

                if not format_[-1:].lower() in ['s','p']:
                    if bitshift>=0:
                        result >>= bitshift
                    else:
                        result <<= abs(bitshift)
                    if bitlen>0:
                        result &= (1<<bitlen)-1

                # additional processing for strings
                if format_[-1:].lower() in ['s','p']:
                    # use left string until \0
                    s = str(result).split('\0')[0]
                    # remove character > 127
                    result = unicode(s, errors='ignore')

                result = ConvertFieldValue(result, fielddef, raw)

    return result


def DeEncrypt(obj):
    """
    Decrpt/Encrypt binary config data

    @param obj:
        binary config data

    @return: decrypted configuration (if obj contains encrypted data)
                     encrypted configuration (if obj contains decrypted data)
    """
    dobj  = obj[0:2]
    for i in range(2, len(obj)):
        dobj += chr( (ord(obj[i]) ^ (CONFIG_FILE_XOR +i)) & 0xff )
    return dobj


def GetTemplateSetting(version):
    """
    Search for template, settings and size to be used depending on given version number

    @param version:
        <int> version number from read binary data to search for

    @return: template, settings to use, None if version is invalid
    """
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

    return template, size, setting


def Decode(obj, raw=True):
    """
    Decodes binary data stream

    @param obj:
        binary config data (decrypted)
    @param raw
        decode raw values (True) or converted values (False)

    @return: configuration dictionary
    """
    # get header data
    version = GetField(obj,  'version', Setting_6_2_1['version'], raw=True)

    template, size, setting = GetTemplateSetting(version)
    # if we did not found a mathching setting
    if template is None:
        exit(2, "Tasmota configuration version 0x{:x} not supported".format(version) )

    # check size if exists
    if 'cfg_size' in setting:
        cfg_size = GetField(obj, 'cfg_size', setting['cfg_size'], raw=True)
        # read size should be same as definied in template
        if cfg_size > size:
            # may be processed
            exit(3, "Number of bytes read does ot match - read {}, expected {} byte".format(cfg_size, template[1]), typ='WARNING', doexit=args.exitonwarning)
        elif cfg_size < size:
            # less number of bytes can not be processed
            exit(3, "Number of bytes read to small to process - read {}, expected {} byte".format(cfg_size, template[1]), typ='ERROR')

    # check crc if exists
    if 'cfg_crc' in setting:
        cfg_crc = GetField(obj, 'cfg_crc', setting['cfg_crc'], raw=True)
    else:
        cfg_crc = GetSettingsCrc(obj)
    if cfg_crc != GetSettingsCrc(obj):
        exit(4, 'Data CRC error, read 0x{:x} should be 0x{:x}'.format(cfg_crc, GetSettingsCrc(obj)), typ='WARNING', doexit=args.exitonwarning)

    # get config
    config = GetField(obj, None, (setting,None,None), raw=raw)

    # add header info
    timestamp = datetime.now()
    config['header'] = {  'timestamp':  timestamp.strftime("%Y-%m-%d %H:%M:%S"),
                          'data':       {
                                            'crc': hex(GetSettingsCrc(obj)),
                                            'size': len(obj),
                                            'template_version': hex(template[0]),
                                            'content':  {
                                                            'crc': hex(cfg_crc),
                                                            'size': cfg_size,
                                                            'version': hex(version),
                                                        },
                                        },
                          'scriptname': os.path.basename(__file__),
                          'scriptversion': VER,
                       }

    return config


if __name__ == "__main__":
    # program argument processing
    parser = configargparse.ArgumentParser(description='Decode configuration of Sonoff-Tasmota device.',
                                           epilog='Either argument -d <host> or -f <filename> must be given.')

    source = parser.add_argument_group('source')
    source.add_argument('-f', '--file',
                            metavar='<filename>',
                            dest='tasmotafile',
                            default=DEFAULTS['source']['tasmotafile'],
                            help="file to retrieve Tasmota configuration from (default: {})'".format(DEFAULTS['source']['tasmotafile']))
    source.add_argument('-d', '--device',
                            metavar='<host>',
                            dest='device',
                            default=DEFAULTS['source']['device'],
                            help="hostname or IP address to retrieve Tasmota configuration from (default: {})".format(DEFAULTS['source']['device']) )
    source.add_argument('-u', '--username',
                            metavar='<user>',
                            dest='username',
                            default=DEFAULTS['source']['username'],
                            help="host HTTP access username (default: {})".format(DEFAULTS['source']['username']))
    source.add_argument('-p', '--password',
                            metavar='<password>',
                            dest='password',
                            default=DEFAULTS['source']['password'],
                            help="host HTTP access password (default: {})".format(DEFAULTS['source']['password']))

    config = parser.add_argument_group('config')
    config.add_argument('--json-indent',
                            metavar='<integer>',
                            dest='jsonindent',
                            type=int,
                            default=DEFAULTS['config']['jsonindent'],
                            help="pretty-printed JSON output using indent level (default: '{}'). Use values greater equal 0 to indent or -1 to disabled indent.".format(DEFAULTS['config']['jsonindent']) )
    config.add_argument('--json-compact',
                            dest='jsoncompact',
                            action='store_true',
                            default=DEFAULTS['config']['jsoncompact'],
                            help="compact JSON output by eliminate whitespace{}".format(' (default)' if DEFAULTS['config']['jsoncompact'] else '') )

    config.add_argument('--sort',
                            dest='sort',
                            action='store_true',
                            default=DEFAULTS['config']['sort'],
                            help="sort json keywords{}".format(' (default)' if DEFAULTS['config']['sort'] else '') )
    config.add_argument('--unsort',
                            dest='sort',
                            action='store_false',
                            default=DEFAULTS['config']['sort'],
                            help="do not sort json keywords{}".format(' (default)' if not DEFAULTS['config']['sort'] else '') )

    config.add_argument('--raw-values', '--raw',
                            dest='rawvalues',
                            action='store_true',
                            default=DEFAULTS['config']['rawvalues'],
                            help="output raw values{}".format(' (default)' if DEFAULTS['config']['rawvalues'] else '') )
    config.add_argument('--no-raw-values',
                            dest='rawvalues',
                            action='store_false',
                            default=DEFAULTS['config']['rawvalues'],
                            help="output human readable values{}".format(' (default)' if not DEFAULTS['config']['rawvalues'] else '') )

    config.add_argument('--raw-keys',
                            dest='rawkeys',
                            action='store_true',
                            default=DEFAULTS['config']['rawkeys'],
                            help="output bitfield raw keys{}".format(' (default)' if DEFAULTS['config']['rawkeys'] else '') )
    config.add_argument('--no-raw-keys',
                            dest='rawkeys',
                            action='store_false',
                            default=DEFAULTS['config']['rawkeys'],
                            help="do not output bitfield raw keys{}".format(' (default)' if not DEFAULTS['config']['rawkeys'] else '') )

    config.add_argument('--hide-pw',
                            dest='hidepw',
                            action='store_true',
                            default=DEFAULTS['config']['hidepw'],
                            help="hide passwords{}".format(' (default)' if DEFAULTS['config']['hidepw'] else '') )
    config.add_argument('--unhide-pw',
                            dest='hidepw',
                            action='store_false',
                            default=DEFAULTS['config']['hidepw'],
                            help="unhide passwords{}".format(' (default)' if not DEFAULTS['config']['hidepw'] else '') )

    config.add_argument('-o', '--output-file',
                            metavar='<filename>',
                            dest='outputfile',
                            default=DEFAULTS['config']['outputfile'],
                            help="file to store configuration to (default: {}). Replacements: @v=Tasmota version, @f=friendly name".format(DEFAULTS['config']['outputfile']))
    config.add_argument('--output-file-format',
                            metavar='<word>',
                            dest='outputfileformat',
                            choices=['json', 'binary'],
                            default=DEFAULTS['config']['outputfileformat'],
                            help="output format ('json' or 'binary', default: '{}')".format(DEFAULTS['config']['outputfileformat']) )

    parser.add_argument('-c', '--config',
                            metavar='<filename>',
                            dest='configfile',
                            default=DEFAULTS['DEFAULT']['configfile'],
                            is_config_file=True,
                            help="Config file, can be used instead of command parameter (default: {})".format(DEFAULTS['DEFAULT']['configfile']) )
    parser.add_argument('--exit-on-error-only',
                            dest='exitonwarning',
                            action='store_false',
                            default=DEFAULTS['DEFAULT']['exitonwarning'],
                            help="exit on error only (default: {}). Not recommended, used by your own responsibility!".format('exit on ERROR and WARNING' if DEFAULTS['DEFAULT']['exitonwarning'] else 'exit on ERROR') )

    info = parser.add_argument_group('info')
    info.add_argument('-V', '--version',  action='version', version=PROG)

    args = parser.parse_args()

    # default no configuration available
    configobj = None

    # check source args
    if args.device is not None and args.tasmotafile is not None:
        exit(6, "Only one source allowed. Do not use -d and -f together")

    # read config direct from device via http
    if args.device is not None:

        buffer = io.BytesIO()
        url = str("http://{}/dl".format(args.device))
        c = pycurl.Curl()
        c.setopt(c.URL, url)
        c.setopt(c.VERBOSE, 0)
        if args.username is not None and args.password is not None:
            c.setopt(c.HTTPAUTH, c.HTTPAUTH_BASIC)
            c.setopt(c.USERPWD, args.username + ':' + args.password)
        c.setopt(c.WRITEDATA, buffer)
        try:
            c.perform()
        except Exception, e:
            exit(e[0], e[1])
        response = c.getinfo(c.RESPONSE_CODE)
        c.close()
        if response>=400:
            exit(response, 'HTTP returns {}'.format(response) )

        configobj = buffer.getvalue()

    # read config from a file
    elif args.tasmotafile is not None:

        if not os.path.isfile(args.tasmotafile):    # check file exists
            exit(1, "File '{}' not found".format(args.tasmotafile))
        try:
            tasmotafile = open(args.tasmotafile, "rb")
            configobj = tasmotafile.read()
            tasmotafile.close()
        except Exception, e:
            exit(e[0], e[1])

    # no config source given
    else:
        parser.print_help()
        sys.exit(0)

    if configobj is not None and len(configobj)>0:
        cfg = DeEncrypt(configobj)

        configuration = Decode(cfg, args.rawvalues)

        # output to file
        if args.outputfile is not None:
            outputfilename = GetFilenameReplaced(args.outputfile, configuration)
            if args.outputfileformat == 'binary':
                outputfile = open(outputfilename, "wb")
                outputfile.write(struct.pack('<L',BINARYFILE_MAGIC))
                outputfile.write(cfg)
                outputfile.close()

            if args.outputfileformat == 'json':
                configuration = Decode(cfg, raw=True)
                outputfile = open(outputfilename, "w")
                json.dump(configuration, outputfile, sort_keys=args.sort, indent=None if args.jsonindent<0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )
                outputfile.close()

        # output to screen
        else:
            print json.dumps(configuration, sort_keys=args.sort, indent=None if args.jsonindent<0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )

    else:
        exit(5, "Unable to read configuration data from {} '{}'".format('device' if args.device is not None else 'file', \
                                                                        args.device if args.device is not None else args.tasmotafile) )

    sys.exit(exitcode)
