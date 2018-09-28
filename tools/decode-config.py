#!/usr/bin/env python
# -*- coding: utf-8 -*-

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
    Execute command with option -d to retrieve config data from device or
    use -f to read out a previously saved configuration file.

    For help execute command with argument -h


Usage:
    decode-config.py [-h] [-f <filename>] [-d <host>] [-u <user>]
                     [-p <password>] [--format <word>]
                     [--json-indent <integer>] [--json-compact]
                     [--sort <word>] [--raw] [--unhide-pw] [-o <filename>]
                     [-c <filename>] [-V]

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

    source:
      -f <filename>, --file <filename>
                            file to retrieve Tasmota configuration from (default:
                            None)
      -d <host>, --device <host>
                            hostname or IP address to retrieve Tasmota
                            configuration from (default: None)
      -u <user>, --username <user>
                            host http access username (default: admin)
      -p <password>, --password <password>
                            host http access password (default: None)

    output:
      --format <word>       output format ("json" or "text", default: "json")
      --json-indent <integer>
                            pretty-printed JSON output using indent level
                            (default: "None")
      --json-compact        compact JSON output by eliminate whitespace (default:
                            "not compact")
      --sort <word>         sort result - <word> can be "none" or "name" (default:
                            "name")
      --raw                 output raw values (default: processed)
      --unhide-pw           unhide passwords (default: hide)
      -o <filename>, --output-file <filename>
                            file to store decrypted raw binary configuration to
                            (default: None)

    info:
      -V, --version         show program's version number and exit

    Either argument -d <host> or -f <filename> must be given.


Examples:
    Read configuration from hostname 'sonoff1' and output default json config
        ./decode-config.py -d sonoff1

    Read configuration from file 'Config__6.2.1.dmp' and output default json config
    ./decode-config.py -f Config__6.2.1.dmp

    Read configuration from hostname 'sonoff1' using web login data
        ./decode-config.py -d sonoff1 -u admin -p xxxx

    Read configuration from hostname 'sonoff1' using web login data and unhide passwords
        ./decode-config.py -d sonoff1 -u admin -p xxxx --unhide-pw

    Read configuration from hostname 'sonoff1' using web login data, unhide passwords
    and sort key names
        ./decode-config.py -d sonoff1 -u admin -p xxxx --unhide-pw --sort name
"""

import os.path
import io
import sys
import configargparse
import collections
import struct
import re
import json
try:
    import pycurl
except ImportError:
    print("module <pycurl> not found. Try 'pip pycurl' to install it")
    sys.exit(9)
try:
    import urllib2
except ImportError:
    print("module <urllib2> not found. Try 'pip urllib2' to install it")
    sys.exit(9)


VER = '1.5.0009'
PROG='{} v{} by Norbert Richter'.format(os.path.basename(sys.argv[0]),VER)

CONFIG_FILE_XOR   = 0x5A


args = {}
DEFAULTS = {
    'DEFAULT':
    {
        'configfile':   None,
    },
    'source':
    {
        'device':       None,
        'username':     'admin',
        'password':     None,
        'tasmotafile':  None,
    },
    'output':
    {
        'format':       'json',
        'jsonindent':   None,
        'jsoncompact':  False,
        'sort':         'name',
        'raw':          False,
        'unhide-pw':    False,
        'outputfile':   None,
    },
}


"""
Settings dictionary describes the config file fields definition:

    Each setting name has a tuple containing the following items:

        (format, baseaddr, datadef, <convert>)

    where

        format
            Define the data interpretation.
            For details see struct module format string
              https://docs.python.org/2.7/library/struct.html#format-strings

        baseaddr
            The address (starting from 0) within config data

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
                [{} <,{}...]
                    Defines a bit struct. The items are simply dict
                    {'bitname', bitlen}, the dict order is important.

        convert (optional)
            Define an output/conversion methode, can be a simple string
            or a previously defined function name.
            'xxx':
                a string defines a format specification of the string
                formatter, see
                https://docs.python.org/2.7/library/string.html#format-string-syntax
            func:
                a function defines the name of a formating function

"""
# config data conversion function and helper
def baudrate(value):
    return value * 1200

def int2ip(value):
    return '{:d}.{:d}.{:d}.{:d}'.format(value & 0xff, value>>8 & 0xff, value>>16 & 0xff, value>>24 & 0xff)

def int2geo(value):
    return float(value) / 1000000

def password(value):
    if args.unhidepw:
        return value
    return '********'

def fingerprintstr(value):
    s = list(value)
    result = ''
    for c in s:
        if c in '0123456789abcdefABCDEF':
            result += c
    return result


Setting_6_2_1 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                    {'mqtt_serial': 1},
                                                    {'rules_enabled': 1},
                                                    {'rules_once': 1},
                                                    {'knx_enabled': 1},
                                                    {'device_index_enable': 1},
                                                    {'knx_enable_enhancement': 1},
                                                    {'rf_receive_decimal': 1},
                                                    {'ir_receive_decimal': 1},
                                                    {'hass_light': 1},
                                                    {'global_state': 1}
                                                   ], '0x{:08x}'),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, baudrate),
    'seriallog_level':              ('B',   0x09E, None),
    'sta_config':                   ('B',   0x09F, None),
    'sta_active':                   ('B',   0x0A0, None),
    'sta_ssid':                     ('33s', 0x0A1, [2]),
    'sta_pwd':                      ('65s', 0x0E3, [2], password),
    'hostname':                     ('33s', 0x165, None),
    'syslog_host':                  ('33s', 0x186, None),
    'rule_stop':                    ('B',   0x1A7, None),
    'syslog_port':                  ('<H',  0x1A8, None),
    'syslog_level':                 ('B',   0x1AA, None),
    'webserver':                    ('B',   0x1AB, None),
    'weblog_level':                 ('B',   0x1AC, None),
    'mqtt_fingerprint':             ('20s', 0x1AD, [2], fingerprintstr),
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
    'tflag':                        ('<H',  0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'flag3':                        ('<L',  0x3A0, [
                                                    {'timers_enable': 1},
                                                    {'user_esp8285_enable': 1},
                                                    {'time_append_timezone': 1}
                                                   ], '0x{:08x}'),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('b',   0x451, None),
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
    'rule_enabled':                 ('B',   0x49F, None),
    'rule_once':                    ('B',   0x4A0, None),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 11},
                                                    {'frequency_resolution': 2},
                                                    {'axis_resolution': 2},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
    'switch_debounce':              ('<H',  0x66E, None),
    'timer':                        ('<L',  0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, int2geo),
    'longitude':                    ('i',   0x6B4, None, int2geo),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ('<H',  0x6F6, [16]),
    'mcp230xx_int_prio':            ('B',   0x716, None),
    'mcp230xx_int_timer':           ('<H', 0x718, None),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_1_1 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                    {'mqtt_serial': 1},
                                                    {'rules_enabled': 1},
                                                    {'rules_once': 1},
                                                    {'knx_enabled': 1},
                                                    {'device_index_enable': 1},
                                                    {'knx_enable_enhancement': 1},
                                                    {'rf_receive_decimal': 1},
                                                    {'ir_receive_decimal': 1},
                                                    {'hass_light': 1},
                                                   ], '0x{:08x}'),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, baudrate),
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
    'tflag':                        ('<H',  0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'flag3':                        ('<L',  0x3A0, None),
    'switchmode':                   ('B',   0x3A4, [8]),
    'friendlyname':                 ('33s', 0x3AC, [4]),
    'switch_topic':                 ('33s', 0x430, None),
    'serial_delimiter':             ('b',   0x451, None),
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
    'rule_enabled':                 ('B',   0x49F, None),
    'rule_once':                    ('B',   0x4A0, None),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
    'timer':                        ('<L',  0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, int2geo),
    'longitude':                    ('i',   0x6B4, None, int2geo),
    'knx_physsical_addr':           ('<H',  0x6B8, None),
    'knx_GA_addr':                  ('<H',  0x6BA, [10]),
    'knx_CB_addr':                  ('<H',  0x6CE, [10]),
    'knx_GA_param':                 ('B',   0x6E2, [10]),
    'knx_CB_param':                 ('B',   0x6EC, [10]),
    'mcp230xx_config':              ('<H',  0x6F6, [16]),
    'mems':                         ('10s', 0x7CE, [5]),
    'rules':                        ('512s',0x800, [3])
}
Setting_6_0_0 = {
    'cfg_holder':                   ('<H',  0x000, None),
    'cfg_size':                     ('<H',  0x002, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<H',  0x00C, None),
    'cfg_crc':                      ('<H',  0x00E, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                    {'mqtt_serial': 1},
                                                    {'rules_enabled': 1},
                                                    {'rules_once': 1},
                                                    {'knx_enabled': 1},
                                                    {'device_index_enable': 1},
                                                    {'knx_enable_enhancement': 1},
                                                   ], '0x{:08x}'),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, baudrate),
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
    'tflag':                        ('<H',  0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'serial_delimiter':             ('b',   0x451, None),
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
    'rule_enabled':                 ('B',   0x49F, None),
    'rule_once':                    ('B',   0x4A0, None),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
    'timer':                        ('<L',  0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, int2geo),
    'longitude':                    ('i',   0x6B4, None, int2geo),
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
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                    {'mqtt_serial': 1},
                                                    {'rules_enabled': 1},
                                                    {'rules_once': 1},
                                                    {'knx_enabled': 1},
                                                    {'device_index_enable': 1},
                                                   ], '0x{:08x}'),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, baudrate),
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
    'tflag':                        ('<H',  0x2E2, [2]),
    'pwm_frequency':                ('<H',  0x2E6, None),
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'serial_delimiter':             ('b',   0x451, None),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
    'timer':                        ('<L',  0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, int2geo),
    'longitude':                    ('i',   0x6B4, None, int2geo),
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
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                    {'mqtt_serial': 1},
                                                    {'rules_enabled': 1},
                                                    {'rules_once': 1},
                                                    {'knx_enabled': 1},
                                                   ], '0x{:08x}'),
    'save_data':                    ('<h',  0x014, None),
    'timezone':                     ('b',   0x016, None),
    'ota_url':                      ('101s',0x017, None),
    'mqtt_prefix':                  ('11s', 0x07C, [3]),
    'baudrate':                     ('B',   0x09D, None, baudrate),
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
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'serial_delimiter':             ('b',   0x451, None),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
    'timer':                        ('<L',  0x670, [16]),
    'latitude':                     ('i',   0x6B0, None, int2geo),
    'longitude':                    ('i',   0x6B4, None, int2geo),
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
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                    {'hass_discovery': 1},
                                                    {'not_power_linked': 1},
                                                    {'no_power_on_check': 1},
                                                   ], '0x{:08x}'),
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
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
}
Setting_5_11_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                    {'light_signal': 1},
                                                   ], '0x{:08x}'),
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
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
}
Setting_5_10_0 = {
    'cfg_holder':                   ('<L',  0x000, None),
    'save_flag':                    ('<L',  0x004, None),
    'version':                      ('<L',  0x008, None, '0x{:x}'),
    'bootcount':                    ('<L',  0x00C, None),
    'flag':                         ('<L',  0x010, [
                                                    {'save_state': 1},
                                                    {'button_restrict': 1},
                                                    {'value_units': 1},
                                                    {'mqtt_enabled': 1},
                                                    {'mqtt_response': 1},
                                                    {'mqtt_power_retain': 1},
                                                    {'mqtt_button_retain': 1},
                                                    {'mqtt_switch_retain': 1},
                                                    {'temperature_conversion': 1},
                                                    {'mqtt_sensor_retain': 1},
                                                    {'mqtt_offline': 1},
                                                    {'button_swap': 1},
                                                    {'stop_flash_rotate': 1},
                                                    {'button_single': 1},
                                                    {'interlock': 1},
                                                    {'pwm_control': 1},
                                                    {'ws_clock_reverse': 1},
                                                    {'decimal_text': 1},
                                                   ], '0x{:08x}'),
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
    'power':                        ('<L',  0x2E8, [
                                                    {'Power1': 1},
                                                    {'Power2': 1},
                                                    {'Power3': 1},
                                                    {'Power4': 1},
                                                    {'Power5': 1},
                                                    {'Power6': 1},
                                                    {'Power7': 1},
                                                    {'Power8': 1}
                                                   ], '0x{:02x}'),
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
    'flag2':                        ('<L',  0x5BC, [
                                                    {'spare': 15},
                                                    {'current_resolution': 2},
                                                    {'voltage_resolution': 2},
                                                    {'wattage_resolution': 2},
                                                    {'emulation': 2},
                                                    {'energy_resolution': 3},
                                                    {'pressure_resolution': 2},
                                                    {'humidity_resolution': 2},
                                                    {'temperature_resolution': 2}
                                                   ], '0x{:08x}'),
    'pulse_counter':                ('<L',  0x5C0, [4]),
    'pulse_counter_type':           ('<H',  0x5D0, None),
    'pulse_counter_debounce':       ('<H',  0x5D2, None),
    'rf_code':                      ('B',   0x5D4, [17,9], '0x{:02x}'),
}

Settings = [(0x6020100, 0xe00, Setting_6_2_1),
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


def exit(status=0, message="end"):
    """
    Called when the program should be exit

    @param status:
        the exit status program returns to callert
    @param message:
        the message logged before exit
    """

    logger = Log()
    logger.message(message, typ='ERROR' if status!=0 else 'INFO', status=status, jsonformat=args.format=='json' )
    sys.exit(status)


# ----------------------------------------------------------------------
# Tasmota config data handling
# ----------------------------------------------------------------------
def GetSettingsCrc(dobj):
    """
    Return binary config data calclulated crc

    @param dobj:
        uncrypted binary config data

    @return: 2 byte unsigned integer crc value

    """
    crc = 0
    for i in range(0, len(dobj)):
        if not i in [14,15]: # Skip crc
            crc += ord(dobj[i]) * (i+1)
    return crc & 0xffff


def GetFieldLength(fielddef):
    """
    Return length of a field in bytes based on field format definition

    @param fielddef:
        field format - see python struct module Format String

    @return: length of field in bytes

    """

    length=0

    if fielddef[2] is not None:
        # fielddef[2] contains a array or int
        # calc size recursive by sum of all elements

        # tuple 2 contains a list with integer or an integer value
        if (isinstance(fielddef[2], list) and len(fielddef[2])>0 and isinstance(fielddef[2][0], int)) or isinstance(fielddef[2], int):
             for i in range(0, fielddef[2][0] if isinstance(fielddef[2], list) else fielddef[2] ):
                # multidimensional array
                if isinstance(fielddef[2], list) and len(fielddef[2])>1:
                    length += GetFieldLength( (fielddef[0], fielddef[1], fielddef[2][1:]) )
                else:
                    length += GetFieldLength( (fielddef[0], fielddef[1], None) )
    else:
        if fielddef[0][-1:].lower() in ['b','c','?']:
            length=1
        elif fielddef[0][-1:].lower() in ['h']:
            length=2
        elif fielddef[0][-1:].lower() in ['i','l','f']:
            length=4
        elif fielddef[0][-1:].lower() in ['q','d']:
            length=8
        elif fielddef[0][-1:].lower() in ['s','p']:
            # s and p needs prefix as length
            match = re.search("\s*(\d+)", fielddef[0])
            if match:
                length=int(match.group(0))

    # it's a single value
    return length

def ConvertFieldValue(value, fielddef, raw=False):
    """
    Convert field value based on field desc

    @param value:
        original value read from binary data
    @param fielddef
        field definition (contains possible conversion defiinition)
    @param raw
        return raw values (True) or converted values (False)

    @return: (un)converted value
    """
    if not raw and len(fielddef)>3:
        if isinstance(fielddef[3],str): # use a format string
            return fielddef[3].format(value)
        elif callable(fielddef[3]):     # use a format function
            return fielddef[3](value)
    return value


def GetField(dobj, fieldname, fielddef, raw=False):
    """
    Get field value from definition

    @param dobj:
        uncrypted binary config data
    @param fieldname:
        name of the field
    @param fielddef:
        see Settings desc above
    @param raw
        return raw values (True) or converted values (False)

    @return: read field value
    """

    result = None

    if fielddef[2] is not None:
        result = []

        # tuple 2 contains a list with integer or an integer value
        if (isinstance(fielddef[2], list) and len(fielddef[2])>0 and isinstance(fielddef[2][0], int)) or isinstance(fielddef[2], int):
            addr = fielddef[1]
            for i in range(0, fielddef[2][0] if isinstance(fielddef[2], list) else fielddef[2] ):
                # multidimensional array
                if isinstance(fielddef[2], list) and len(fielddef[2])>1:
                    subfielddef = (fielddef[0], addr, fielddef[2][1:], None if len(fielddef)<4 else fielddef[3])
                else:                         # single array
                    subfielddef = (fielddef[0], addr, None, None if len(fielddef)<4 else fielddef[3])
                length = GetFieldLength(subfielddef)
                if length != 0:
                    result.append(GetField(dobj, fieldname, subfielddef, raw))
                addr += length
        # tuple 2 contains a list with dict
        elif isinstance(fielddef[2], list) and len(fielddef[2])>0 and isinstance(fielddef[2][0], dict):
            d = {}
            value = struct.unpack_from(fielddef[0], dobj, fielddef[1])[0]
            d['base'] = ConvertFieldValue(value, fielddef, raw);
            union = fielddef[2]
            i = 0
            for l in union:
                for name,bits in l.items():
                    bitval = (value & ( ((1<<bits)-1) << i )) >> i
                    d[name] = bitval
                    i += bits
            result = d
    else:
        # it's a single value
        if GetFieldLength(fielddef) != 0:
            result = struct.unpack_from(fielddef[0], dobj, fielddef[1])[0]
            if fielddef[0][-1:].lower() in ['s','p']:
                if ord(result[:1])==0x00 or ord(result[:1])==0xff:
                    result = ''
                s = str(result).split('\0')[0]
                result = unicode(s, errors='replace')
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


def Decode(obj):
    """
    Decodes binary data stream

    @param obj:
        binary config data (decrypted)
    """
    # get header data
    version = GetField(obj,  'version', Setting_6_2_1['version'], raw=True)

    # search setting definition
    template = None
    for cfg in Settings:
        if version >= cfg[0]:
            template = cfg
            break

    # if we did not found a mathching setting
    if template is None:
        exit(2, "Can't handle Tasmota configuration data for version 0x{:x}".format(version) )

    setting = template[2]

    # check size if exists
    if 'cfg_size' in setting:
        cfg_size = GetField(obj, 'cfg_size', setting['cfg_size'], raw=True)
        # if we did not found a mathching setting
        if cfg_size != template[1]:
            exit(2, "Data size does not match. Expected {} bytes, read {} bytes.".format(template[1], cfg_size) )

    # check crc if exists
    if 'cfg_crc' in setting:
        cfg_crc = GetField(obj, 'cfg_crc', setting['cfg_crc'], raw=True)
    else:
        cfg_crc = GetSettingsCrc(obj)
    if cfg_crc != GetSettingsCrc(obj):
        exit(3, 'Data crc error' )

    config = {}
    config['version_template'] = '0x{:x}'.format(template[0])
    for name in setting:
        config[name] = GetField(obj, name, setting[name], args.raw)

    if args.sort == 'name':
        config = collections.OrderedDict(sorted(config.items()))

    if args.format == 'json':
        print json.dumps(config, sort_keys=args.sort=='name', indent=args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )
    else:
        for key,value in config.items():
            print '{} = {}'.format(key, repr(value))



if __name__ == "__main__":
    parser = configargparse.ArgumentParser(description='Decode configuration of Sonoff-Tasmota device.',
                                           epilog='Either argument -d <host> or -f <filename> must be given.')

    source = parser.add_argument_group('source')
    source.add_argument('-f', '--file',
                            metavar='<filename>',
                            dest='tasmotafile',
                            default=DEFAULTS['source']['tasmotafile'],
                            help='file to retrieve Tasmota configuration from (default: {})'.format(DEFAULTS['source']['tasmotafile']))
    source.add_argument('-d', '--device',
                            metavar='<host>',
                            dest='device',
                            default=DEFAULTS['source']['device'],
                            help='hostname or IP address to retrieve Tasmota configuration from (default: {})'.format(DEFAULTS['source']['device']) )
    source.add_argument('-u', '--username',
                            metavar='<user>',
                            dest='username',
                            default=DEFAULTS['source']['username'],
                            help='host http access username (default: {})'.format(DEFAULTS['source']['username']))
    source.add_argument('-p', '--password',
                            metavar='<password>',
                            dest='password',
                            default=DEFAULTS['source']['password'],
                            help='host http access password (default: {})'.format(DEFAULTS['source']['password']))

    output = parser.add_argument_group('output')
    output.add_argument('--format',
                            metavar='<word>',
                            dest='format',
                            choices=['json', 'text'],
                            default=DEFAULTS['output']['format'],
                            help='output format ("json" or "text", default: "{}")'.format(DEFAULTS['output']['format']) )
    output.add_argument('--json-indent',
                            metavar='<integer>',
                            dest='jsonindent',
                            type=int,
                            default=DEFAULTS['output']['jsonindent'],
                            help='pretty-printed JSON output using indent level (default: "{}")'.format(DEFAULTS['output']['jsonindent']) )
    output.add_argument('--json-compact',
                            dest='jsoncompact',
                            action='store_true',
                            default=DEFAULTS['output']['jsoncompact'],
                            help='compact JSON output by eliminate whitespace (default: "{}")'.format('compact' if DEFAULTS['output']['jsoncompact'] else 'not compact') )
    output.add_argument('--sort',
                            metavar='<word>',
                            dest='sort',
                            choices=['none', 'name'],
                            default=DEFAULTS['output']['sort'],
                            help='sort result - <word> can be "none" or "name" (default: "{}")'.format(DEFAULTS['output']['sort']) )
    output.add_argument('--raw',
                            dest='raw',
                            action='store_true',
                            default=DEFAULTS['output']['raw'],
                            help='output raw values (default: {})'.format('raw' if DEFAULTS['output']['raw'] else 'processed') )
    output.add_argument('--unhide-pw',
                            dest='unhidepw',
                            action='store_true',
                            default=DEFAULTS['output']['unhide-pw'],
                            help='unhide passwords (default: {})'.format('unhide' if DEFAULTS['output']['unhide-pw'] else 'hide') )
    output.add_argument('-o', '--output-file',
                            metavar='<filename>',
                            dest='outputfile',
                            default=DEFAULTS['output']['outputfile'],
                            help='file to store decrypted raw binary configuration to (default: {})'.format(DEFAULTS['output']['outputfile']))

    parser.add_argument('-c', '--config',
                            metavar='<filename>',
                            dest='configfile',
                            default=DEFAULTS['DEFAULT']['configfile'],
                            is_config_file=True,
                            help='Config file, can be used instead of command parameter (default: {})'.format(DEFAULTS['DEFAULT']['configfile']) )

    info = parser.add_argument_group('info')
    info.add_argument('-V', '--version',  action='version', version=PROG)

    args = parser.parse_args()

    configobj = None

    if args.device is not None:

        # read config direct from device via http
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

    elif args.tasmotafile is not None:
        # read config from a file

        if not os.path.isfile(args.tasmotafile):    # check file exists
            exit(1, "file '{}' not found".format(args.tasmotafile))
        try:
            tasmotafile = open(args.tasmotafile, "rb")
            configobj = tasmotafile.read()
            tasmotafile.close()
        except Exception, e:
            exit(e[0], e[1])

    else:
        parser.print_help()
        sys.exit(0)

    if configobj is not None and len(configobj)>0:
        cfg = DeEncrypt(configobj)

        if args.outputfile is not None:
            outputfile = open(args.outputfile, "wb")
            outputfile.write(cfg)
            outputfile.close()

        Decode(cfg)

    else:
        exit(4, "Could not read configuration data from {} '{}'".format('device' if args.device is not None else 'file', \
                                                                        args.device if args.device is not None else args.tasmotafile) )

    sys.exit(0)
