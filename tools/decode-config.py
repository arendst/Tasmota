#!/usr/bin/env python
# -*- coding: utf-8 -*-
VER = '2.1.0022'

"""
    decode-config.py - Backup/Restore Sonoff-Tasmota configuration data

    Copyright (C) 2019 Norbert Richter <nr@prsolution.eu>

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
                        [-o <filename>] [-t json|bin|dmp] [-E] [-e] [-F]
                        [--json-indent <indent>] [--json-compact]
                        [--json-hide-pw] [--json-show-pw]
                        [--cmnd-indent <indent>] [--cmnd-groups]
                        [--cmnd-nogroups] [--cmnd-sort] [--cmnd-unsort]
                        [-c <filename>] [-S] [-T json|cmnd|command]
                        [-g {Display,Domoticz,Internal,KNX,Led,Logging,MCP230xx,MQTT,Main,Management,Pow,Sensor,Serial,SetOption,SonoffRF,System,Timers,Wifi} [{Display,Domoticz,Internal,KNX,Led,Logging,MCP230xx,MQTT,Main,Management,Pow,Sensor,Serial,SetOption,SonoffRF,System,Timers,Wifi} ...]]
                        [--ignore-warnings] [-h] [-H] [-v] [-V]

    Backup/Restore Sonoff-Tasmota configuration data. Args that start with '--'
    (eg. -f) can also be set in a config file (specified via -c). Config file
    syntax allows: key=value, flag=true, stuff=[a,b,c] (for details, see syntax at
    https://goo.gl/R74nmi). If an arg is specified in more than one place, then
    commandline values override config file values which override defaults.

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
      Backup & restore specification

      -i, --restore-file <filename>
                            file to restore configuration from (default: None).
                            Replacements: @v=firmware version from config,
                            @f=device friendly name from config, @h=device
                            hostname from config, @H=device hostname from device
                            (-d arg only)
      -o, --backup-file <filename>
                            file to backup configuration to (default: None).
                            Replacements: @v=firmware version from config,
                            @f=device friendly name from config, @h=device
                            hostname from config, @H=device hostname from device
                            (-d arg only)
      -t, --backup-type json|bin|dmp
                            backup filetype (default: 'json')
      -E, --extension       append filetype extension for -i and -o filename
                            (default)
      -e, --no-extension    do not append filetype extension, use -i and -o
                            filename as passed
      -F, --force-restore   force restore even configuration is identical

    JSON output:
      JSON format specification

      --json-indent <indent>
                            pretty-printed JSON output using indent level
                            (default: 'None'). -1 disables indent.
      --json-compact        compact JSON output by eliminate whitespace
      --json-hide-pw        hide passwords
      --json-show-pw, --json-unhide-pw
                            unhide passwords (default)

    Tasmota command output:
      Tasmota command output format specification

      --cmnd-indent <indent>
                            Tasmota command grouping indent level (default: '2').
                            0 disables indent
      --cmnd-groups         group Tasmota commands (default)
      --cmnd-nogroups       leave Tasmota commands ungrouped
      --cmnd-sort           sort Tasmota commands (default)
      --cmnd-unsort         leave Tasmota commands unsorted

    Common:
      Optional arguments

      -c, --config <filename>
                            program config file - can be used to set default
                            command args (default: None)
      -S, --output          display output regardsless of backup/restore usage
                            (default do not output on backup or restore usage)
      -T, --output-format json|cmnd|command
                            display output format (default: 'json')
      -g, --group {Display,Domoticz,Internal,KNX,Led,Logging,MCP230xx,MQTT,Main,Management,Pow,Sensor,Serial,SetOption,SonoffRF,System,Timers,Wifi}
                            limit data processing to command groups (default no
                            filter)
      --ignore-warnings     do not exit on warnings. Not recommended, used by your
                            own responsibility!

    Info:
      Extra information

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

# ======================================================================
# imports
# ======================================================================
import os.path
import io
import sys, platform
def ModuleImportError(module):
    er = str(module)
    print >> sys.stderr, "{}. Try 'pip install {}' to install it".format(er,er.split(' ')[len(er.split(' '))-1])
    sys.exit(ExitCode.MODULE_NOT_FOUND)
try:
    from datetime import datetime
    import time
    import copy
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

# ======================================================================
# globals
# ======================================================================
PROG='{} v{} by Norbert Richter <nr@prsolution.eu>'.format(os.path.basename(sys.argv[0]),VER)

CONFIG_FILE_XOR     = 0x5A
BINARYFILE_MAGIC    = 0x63576223
STR_ENCODING        = 'utf8'
HIDDEN_PASSWORD     = '********'
INTERNAL            = 'Internal'

DEFAULTS            = {
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
        'forcerestore': False,
    },
    'jsonformat':
    {
        'jsonindent':   None,
        'jsoncompact':  False,
        'jsonsort':     True,
        'jsonhidepw':   False,
    },
    'cmndformat':
    {
        'cmndindent':   2,
        'cmndgroup':    True,
        'cmndsort':     True,
    },
    'common':
    {
        'output':       False,
        'outputformat': 'json',
        'configfile':   None,
        'ignorewarning':False,
        'filter':       None,
    },
}
args = {}
exitcode = 0


# ======================================================================
# Settings mapping
# ======================================================================
"""
Settings dictionary describes the config file fields definition:

    <setting> = { <name> : <def> }
    
    <name>: "string"
        a python valid dictionary key (string)

    <def>:  ( <format>, <addrdef>, <datadef> [,<converter>] )
        a tuple containing the following items:

            <format>:   <formatstring> | <setting>
                data type & format definition
                <formatstring>: <string>
                    defines the use of data at <addrdef>
                    format is defined in 'struct module format string'
                    see
                    https://docs.python.org/2.7/library/struct.html#format-strings
                <setting>:      <setting>
                    A dictionary describes a (sub)setting dictonary
                    and can recursively define another <setting>

            <addrdef>:  <baseaddr> | (<baseaddr>, <bits>, <bitshift>)
                address definition
                <baseaddr>: <uint>
                    The address (starting from 0) within binary config data.
                <bits>:     <uint>
                    number of bits used (positive integer)
                <bitshift>: <int>
                    bit shift <bitshift>:
                    <bitshift> >= 0: shift the result right
                    <bitshift> <  0: shift the result left

            <datadef>:  <arraydef> | (<arraydef>, <validate> [,cmd])
                data definition
                <arraydef>: None | <dim> | [<dim>] | [<dim> ,<dim>...]
                    None:
                        Single value, not an array
                    <dim>:  <int>
                    [<dim>]
                        Defines a one-dimensional array of size <n>
                    [<dim> ,<dim>...]
                        Defines a one- or multi-dimensional array
                <validate>: <function>
                    value validation function
                <cmd>:  (<group>, <tasmotacmnd>)
                    Tasmota command definition
                    <group>:        <string>
                        command group string
                    <tasmotacmnd>:   <function>
                        convert data into Tasmota command function

            <converter>:    <readconverter> | (<readconverter>, <writeconverter>)
                read/write converter
                <readconverter>:    None | <function>
                    Will be used in Bin2Mapping to convert values read
                    from the binary data object into mapping dictionary
                    None
                        None indicates not read conversion
                    <function>
                        to convert value from binary object to JSON.
                <writeconverter>:   None | False | <function>
                    Will be used in Mapping2Bin to convert values read
                    from mapping dictionary before write to binary
                    data object
                    None
                        None indicates not write conversion
                    False
                        False indicates the value is readonly and will
                        not be written into the binary object.
                    <function>
                        to convert value from JSON back to binary object

        Common definitions
        
        <function>: <functionname> | <string> | None
            function to be called or string to evaluate:
            <functionname>:
                A function name will be called with one or two parameter:
                    The value to be processed 
                    (optional) the current array index (1,n)
            <string>
                A string will be evaluate as is. The following
                placeholder can be used to replace it by runtime values:
                '$':
                    will be replaced by the mapping name value
                '#':
                    will be replace by array index (if any)
                '@':
                    can be used as reference to other mapping values
                see definition below for examples

        <string>:   'string' | "string"
            characters enclosed in ' or "

        <int>:      integer
             numbers in the range -2147483648 through 2147483647
        <uint>:     unsigned integer
             numbers in the range 0 through 4294967295
            
"""
# ----------------------------------------------------------------------
# Settings helper
# ----------------------------------------------------------------------
def passwordread(value):
    return HIDDEN_PASSWORD if args.jsonhidepw else value
def passwordwrite(value):
    return None if value == HIDDEN_PASSWORD else value
def bitsRead(x, n=0, c=1):
    """
    Reads bit(s) of a number
    
    @param x:
        the number from which to read
    
    @param n:
        which bit position to read
    
    @param c:
        how many bits to read (1 if omitted)
    
    @return:
        the bit value(s)
    """
    if isinstance(x,str):
        x = int(x, 0)
    if isinstance(x,str):
        n = int(n, 0)

    if n >= 0:
        x >>= n
    else:
        x <<= abs(n)
    if c>0:
        x &= (1<<c)-1
    return x

    
def MqttFingerprint(value, idx=None):
    fingerprint = ""
    for i in value:
        fingerprint += "{:02x} ".format(ord(i))
    return "MqttFingerprint{} {}".format('' if idx is None else idx, fingerprint.strip())

    
# ----------------------------------------------------------------------
# Tasmota configuration data definition
# ----------------------------------------------------------------------
Groups = ('Main','Sensor','Timers','Management','Wifi','MQTT','Serial','SetOption','Logging','Pow','Led','KNX','Domoticz','Display','MCP230xx')
Setting_5_10_0 = {
                              # <format>, <addrdef>, <datadef> [,<converter>]
    'cfg_holder':                   ('<L',  0x000,       (None, None,                           (INTERNAL,      None)), '"0x{:08x}".format($)' ),
    'save_flag':                    ('<L',  0x004,       (None, None,                           ('System',      None)), (None,      False) ),
    'version':                      ('<L',  0x008,       (None, None,                           (INTERNAL,      None)), ('hex($)',  False) ),
    'bootcount':                    ('<L',  0x00C,       (None, None,                           ('System',      None)), (None,      False) ),
    'flag':                         ({
        'save_state':               ('<L', (0x010,1, 0), (None, None,                           ('Management',  '"SetOption0 {}".format($)')) ),
        'button_restrict':          ('<L', (0x010,1, 1), (None, None,                           ('Management',  '"SetOption1 {}".format($)')) ),
        'value_units':              ('<L', (0x010,1, 2), (None, None,                           ('MQTT',        '"SetOption2 {}".format($)')) ),
        'mqtt_enabled':             ('<L', (0x010,1, 3), (None, None,                           ('MQTT',        '"SetOption3 {}".format($)')) ),
        'mqtt_response':            ('<L', (0x010,1, 4), (None, None,                           ('MQTT',        '"SetOption4 {}".format($)')) ),
        'mqtt_power_retain':        ('<L', (0x010,1, 5), (None, None,                           ('Main',        '"PowerRetain {}".format($)')) ),
        'mqtt_button_retain':       ('<L', (0x010,1, 6), (None, None,                           ('MQTT',        '"ButtonRetain {}".format($)')) ),
        'mqtt_switch_retain':       ('<L', (0x010,1, 7), (None, None,                           ('MQTT',        '"SwitchRetain {}".format($)')) ),
        'temperature_conversion':   ('<L', (0x010,1, 8), (None, None,                           ('Sensor',      '"SetOption8 {}".format($)')) ),
        'mqtt_sensor_retain':       ('<L', (0x010,1, 9), (None, None,                           ('MQTT',        '"SensorRetain {}".format($)')) ),
        'mqtt_offline':             ('<L', (0x010,1,10), (None, None,                           ('MQTT',        '"SetOption10 {}".format($)')) ),
        'button_swap':              ('<L', (0x010,1,11), (None, None,                           ('Main',        '"SetOption11 {}".format($)')) ),
        'stop_flash_rotate':        ('<L', (0x010,1,12), (None, None,                           ('Management',  '"SetOption12 {}".format($)')) ),
        'button_single':            ('<L', (0x010,1,13), (None, None,                           ('Main',        '"SetOption13 {}".format($)')) ),
        'interlock':                ('<L', (0x010,1,14), (None, None,                           ('Main',        '"SetOption14 {}".format($)')) ),
        'pwm_control':              ('<L', (0x010,1,15), (None, None,                           ('Main',        '"SetOption15 {}".format($)')) ),
        'ws_clock_reverse':         ('<L', (0x010,1,16), (None, None,                           ('SetOption',   '"SetOption16 {}".format($)')) ),
        'decimal_text':             ('<L', (0x010,1,17), (None, None,                           ('SetOption',   '"SetOption17 {}".format($)')) ),
                                    },      0x010,       (None, None,                           ('*',           None)), (None,      False) ),
    'save_data':                    ('<h',  0x014,       (None, '0 <= $ <= 3600',               ('Management',  '"SaveData {}".format($)')) ),
    'timezone':                     ('b',   0x016,       (None, '-13 <= $ <= 13 or $==99',      ('Management',  '"Timezone {}".format($)')) ),
    'ota_url':                      ('101s',0x017,       (None, None,                           ('Main',        '"OtaUrl {}".format($)')) ),
    'mqtt_prefix':                  ('11s', 0x07C,       ([3],  None,                           ('MQTT',        '"Prefix{} {}".format(#,$)')) ),
    'seriallog_level':              ('B',   0x09E,       (None, '0 <= $ <= 5',                  ('Logging',     '"SerialLog {}".format($)')) ),
    'sta_config':                   ('B',   0x09F,       (None, '0 <= $ <= 5',                  ('Wifi',        '"WifiConfig {}".format($)')) ),
    'sta_active':                   ('B',   0x0A0,       (None, '0 <= $ <= 1',                  ('Wifi',        '"AP {}".format($)')) ),
    'sta_ssid':                     ('33s', 0x0A1,       ([2],  None,                           ('Wifi',        '"SSId{} {}".format(#,$)')) ),
    'sta_pwd':                      ('65s', 0x0E3,       ([2],  None,                           ('Wifi',        '"Password{} {}".format(#,$)')), (passwordread,passwordwrite) ),
    'hostname':                     ('33s', 0x165,       (None, None,                           ('Wifi',        '"Hostname {}".format($)')) ),
    'syslog_host':                  ('33s', 0x186,       (None, None,                           ('Logging',     '"LogHost {}".format($)')) ),
    'syslog_port':                  ('<H',  0x1A8,       (None, '1 <= $ <= 32766',              ('Logging',     '"LogPort {}".format($)')) ),
    'syslog_level':                 ('B',   0x1AA,       (None, '0 <= $ <= 4',                  ('Logging',     '"SysLog {}".format($)')) ),
    'webserver':                    ('B',   0x1AB,       (None, '0 <= $ <= 2',                  ('Wifi',        '"WebServer {}".format($)')) ),
    'weblog_level':                 ('B',   0x1AC,       (None, '0 <= $ <= 4',                  ('Logging',     '"WebLog {}".format($)')) ),
    'mqtt_fingerprint':             ('60s', 0x1AD,       (None, None,                           ('MQTT',        None)) ),
    'mqtt_host':                    ('33s', 0x1E9,       (None, None,                           ('MQTT',        '"MqttHost {}".format($)')) ),
    'mqtt_port':                    ('<H',  0x20A,       (None, None,                           ('MQTT',        '"MqttPort {}".format($)')) ),
    'mqtt_client':                  ('33s', 0x20C,       (None, None,                           ('MQTT',        '"MqttClient {}".format($)')) ),
    'mqtt_user':                    ('33s', 0x22D,       (None, None,                           ('MQTT',        '"MqttUser {}".format($)')) ),
    'mqtt_pwd':                     ('33s', 0x24E,       (None, None,                           ('MQTT',        '"MqttPassword {}".format($)')), (passwordread,passwordwrite) ),
    'mqtt_topic':                   ('33s', 0x26F,       (None, None,                           ('MQTT',        '"FullTopic {}".format($)')) ),
    'button_topic':                 ('33s', 0x290,       (None, None,                           ('MQTT',        '"ButtonTopic {}".format($)')) ),
    'mqtt_grptopic':                ('33s', 0x2B1,       (None, None,                           ('MQTT',        '"GroupTopic {}".format($)')) ),
    'mqtt_fingerprinth':            ('B',   0x2D2,       ([20], None,                           ('MQTT',        None)) ),
    'pwm_frequency':                ('<H',  0x2E6,       (None, '$==1 or 100 <= $ <= 4000',     ('Management',  '"PwmFrequency {}".format($)')) ),
    'power':                        ({
        'power1':                   ('<L', (0x2E8,1,0),  (None, None,                           ('Main',        '"Power1 {}".format($)')) ),
        'power2':                   ('<L', (0x2E8,1,1),  (None, None,                           ('Main',        '"Power2 {}".format($)')) ),
        'power3':                   ('<L', (0x2E8,1,2),  (None, None,                           ('Main',        '"Power3 {}".format($)')) ),
        'power4':                   ('<L', (0x2E8,1,3),  (None, None,                           ('Main',        '"Power4 {}".format($)')) ),
        'power5':                   ('<L', (0x2E8,1,4),  (None, None,                           ('Main',        '"Power5 {}".format($)')) ),
        'power6':                   ('<L', (0x2E8,1,5),  (None, None,                           ('Main',        '"Power6 {}".format($)')) ),
        'power7':                   ('<L', (0x2E8,1,6),  (None, None,                           ('Main',        '"Power7 {}".format($)')) ),
        'power8':                   ('<L', (0x2E8,1,7),  (None, None,                           ('Main',        '"Power8 {}".format($)')) ),
                                    },      0x2E8,       (None, None,                           ('Main',        None)), (None,      False) ),
    'pwm_value':                    ('<H',  0x2EC,       ([5],  '0 <= $ <= 1023',               ('Management',  '"Pwm{} {}".format(#,$)')) ),
    'altitude':                     ('<h',  0x2F6,       (None, '-30000 <= $ <= 30000',         ('Sensor',      '"Altitude {}".format($)')) ),
    'tele_period':                  ('<H',  0x2F8,       (None, '0 <= $ <= 1 or 10 <= $ <= 3600',('MQTT',       '"TelePeriod {}".format($)')) ),
    'ledstate':                     ('B',   0x2FB,       (None, '0 <= ($ & 0x7) <= 7',          ('Main',        '"LedState {}".format($)')) ),
    'param':                        ('B',   0x2FC,       ([23], None,                           ('SetOption',   '"SetOption{} {}".format(#+31,$)')) ),
    'state_text':                   ('11s', 0x313,       ([4],  None,                           ('MQTT',        '"StateText{} {}".format(#,$)')) ),
    'domoticz_update_timer':        ('<H',  0x340,       (None, '0 <= $ <= 3600',               ('Domoticz',    '"DomoticzUpdateTimer {}".format($)')) ),
    'pwm_range':                    ('<H',  0x342,       (None, '$==1 or 255 <= $ <= 1023',     ('Management',  '"PwmRange {}".format($)')) ),
    'domoticz_relay_idx':           ('<L',  0x344,       ([4],  None,                           ('Domoticz',    '"DomoticzIdx{} {}".format(#,$)')) ),
    'domoticz_key_idx':             ('<L',  0x354,       ([4],  None,                           ('Domoticz',    '"DomoticzKeyIdx{} {}".format(#,$)')) ),
    'energy_power_calibration':     ('<L',  0x364,       (None, None,                           ('Pow',         '"PowerSet {}".format($)')) ),
    'energy_voltage_calibration':   ('<L',  0x368,       (None, None,                           ('Pow',         '"VoltageSet {}".format($)')) ),
    'energy_current_calibration':   ('<L',  0x36C,       (None, None,                           ('Pow',         '"CurrentSet {}".format($)')) ),
    'energy_kWhtoday':              ('<L',  0x370,       (None, '0 <= $ <= 42500',              ('Pow',         '"EnergyReset1 {}".format($)')) ),
    'energy_kWhyesterday':          ('<L',  0x374,       (None, '0 <= $ <= 42500',              ('Pow',         '"EnergyReset2 {}".format($)')) ),
    'energy_kWhdoy':                ('<H',  0x378,       (None, None,                           ('Pow',         None)) ),
    'energy_min_power':             ('<H',  0x37A,       (None, None,                           ('Pow',         '"PowerLow {}".format($)')) ),
    'energy_max_power':             ('<H',  0x37C,       (None, None,                           ('Pow',         '"PowerHigh {}".format($)')) ),
    'energy_min_voltage':           ('<H',  0x37E,       (None, None,                           ('Pow',         '"VoltageLow {}".format($)')) ),
    'energy_max_voltage':           ('<H',  0x380,       (None, None,                           ('Pow',         '"VoltageHigh {}".format($)')) ),
    'energy_min_current':           ('<H',  0x382,       (None, None,                           ('Pow',         '"CurrentLow {}".format($)')) ),
    'energy_max_current':           ('<H',  0x384,       (None, None,                           ('Pow',         '"CurrentHigh {}".format($)')) ),
    'energy_max_power_limit':       ('<H',  0x386,       (None, None,                           ('Pow',         '"MaxPower {}".format($)')) ),
    'energy_max_power_limit_hold':  ('<H',  0x388,       (None, None,                           ('Pow',         '"MaxPowerHold {}".format($)')) ),
    'energy_max_power_limit_window':('<H',  0x38A,       (None, None,                           ('Pow',         '"MaxPowerWindow {}".format($)')) ),
    'energy_max_power_safe_limit':  ('<H',  0x38C,       (None, None,                           ('Pow',         '"SavePower {}".format($)')) ),
    'energy_max_power_safe_limit_hold':
                                    ('<H',  0x38E,       (None, None,                           ('Pow',         '"SavePowerHold {}".format($)')) ),
    'energy_max_power_safe_limit_window':
                                    ('<H',  0x390,       (None, None,                           ('Pow',         '"SavePowerWindow {}".format($)')) ),
    'energy_max_energy':            ('<H',  0x392,       (None, None,                           ('Pow',         '"MaxEnergy {}".format($)')) ),
    'energy_max_energy_start':      ('<H',  0x394,       (None, None,                           ('Pow',         '"MaxEnergyStart {}".format($)')) ),
    'mqtt_retry':                   ('<H',  0x396,       (None, '10 <= $ <= 32000',             ('MQTT',        '"MqttRetry {}".format($)')) ),
    'poweronstate':                 ('B',   0x398,       (None, '0 <= $ <= 5',                  ('Main',        '"PowerOnState {}".format($)')) ),
    'last_module':                  ('B',   0x399,       (None, None,                           ('System',      None)) ),
    'blinktime':                    ('<H',  0x39A,       (None, '2 <= $ <= 3600',               ('Main',        '"BlinkTime {}".format($)')) ),
    'blinkcount':                   ('<H',  0x39C,       (None, '0 <= $ <= 32000',              ('Main',        '"BlinkCount {}".format($)')) ),
    'friendlyname':                 ('33s', 0x3AC,       ([4],  None,                           ('Management',  '"FriendlyName{} {}".format(#,$)')) ),
    'switch_topic':                 ('33s', 0x430,       (None, None,                           ('MQTT',        '"SwitchTopic {}".format($)')) ),
    'sleep':                        ('B',   0x453,       (None, '0 <= $ <= 250',                ('Management',  '"Sleep {}".format($)')) ),
    'domoticz_switch_idx':          ('<H',  0x454,       ([4],  None,                           ('Domoticz',    '"DomoticzSwitchIdx{} {}".format(#,$)')) ),
    'domoticz_sensor_idx':          ('<H',  0x45C,       ([12], None,                           ('Domoticz',    '"DomoticzSensorIdx{} {}".format(#,$)')) ),
    'module':                       ('B',   0x474,       (None, None,                           ('Management',  '"Module {}".format($)')) ),
    'ws_color':                     ('B',   0x475,       ([4,3],None,                           ('Led',         None)) ),
    'ws_width':                     ('B',   0x481,       ([3],  None,                           ('Led',         None)) ),
    'my_gp':                        ('B',   0x484,       ([18], None,                           ('Management',  '"Gpio{} {}".format(#,$)')) ),
    'light_pixels':                 ('<H',  0x496,       (None, '1 <= $ <= 512',                ('Led',         '"Pxels {}".format($)')) ),
    'light_color':                  ('B',   0x498,       ([5],  None,                           ('Led',         None)) ),
    'light_correction':             ('B',   0x49D     ,  (None, '0 <= $ <= 1',                  ('Led',         '"LedTable {}".format($)')) ),
    'light_dimmer':                 ('B',   0x49E,       (None, '0 <= $ <= 100',                ('Led',         '"Wakeup {}".format($)')) ),
    'light_fade':                   ('B',   0x4A1,       (None, '0 <= $ <= 1',                  ('Led',         '"Fade {}".format($)')) ),
    'light_speed':                  ('B',   0x4A2,       (None, '1 <= $ <= 20',                 ('Led',         '"Speed {}".format($)')) ),
    'light_scheme':                 ('B',   0x4A3,       (None, None,                           ('Led',         '"Scheme {}".format($)')) ),
    'light_width':                  ('B',   0x4A4,       (None, '0 <= $ <= 4',                  ('Led',         '"Width {}".format($)')) ),
    'light_wakeup':                 ('<H',  0x4A6,       (None, '0 <= $ <= 3100',               ('Led',         '"WakeUpDuration {}".format($)')) ),
    'web_password':                 ('33s', 0x4A9,       (None, None,                           ('Wifi',        '"WebPassword {}".format($)')), (passwordread,passwordwrite) ),
    'switchmode':                   ('B',   0x4CA,       ([4],  '0 <= $ <= 7',                  ('Main',        '"SwitchMode{} {}".format(#,$)')) ),
    'ntp_server':                   ('33s', 0x4CE,       ([3],  None,                           ('Wifi',        '"NtpServer{} {}".format(#,$)')) ),
    'ina219_mode':                  ('B',   0x531,       (None, '0 <= $ <= 7',                  ('Sensor',      '"Sensor13 {}".format($)')) ),
    'pulse_timer':                  ('<H',  0x532,       ([8],  '0 <= $ <= 64900',              ('Main',        '"PulseTime{} {}".format(#,$)')), ("float($)/10 if 1 <= $ <= 111 else $-100 if $ != 0 else 0", "int($*10) if 0.1 <= $ < 12 else $+100 if $ != 0 else 0") ),
    'ip_address':                   ('<L',  0x544,       ([4],  None,                           ('Wifi',        '"IPAddress{} {}".format(#,$)')), ("socket.inet_ntoa(struct.pack('<L', $))", "struct.unpack('<L', socket.inet_aton($))[0]")),
    'energy_kWhtotal':              ('<L',  0x554,       (None, None,                           ('Pow',         None)) ),
    'mqtt_fulltopic':               ('100s',0x558,       (None, None,                           ('MQTT',        '"FullTopic {}".format($)')) ),
    'flag2':                        ({
        'current_resolution':       ('<L', (0x5BC,2,15), (None, '0 <= $ <= 3',                  ('Pow',         '"AmpRes {}".format($)')) ),
        'voltage_resolution':       ('<L', (0x5BC,2,17), (None, '0 <= $ <= 3',                  ('Pow',         '"VoltRes {}".format($)')) ),
        'wattage_resolution':       ('<L', (0x5BC,2,19), (None, '0 <= $ <= 3',                  ('Pow',         '"WattRes {}".format($)')) ),
        'emulation':                ('<L', (0x5BC,2,21), (None, '0 <= $ <= 2',                  ('Management',  '"Emulation {}".format($)')) ),
        'energy_resolution':        ('<L', (0x5BC,3,23), (None, '0 <= $ <= 5',                  ('Pow',         '"EnergyRes {}".format($)')) ),
        'pressure_resolution':      ('<L', (0x5BC,2,26), (None, '0 <= $ <= 3',                  ('Sensor',      '"PressRes {}".format($)')) ),
        'humidity_resolution':      ('<L', (0x5BC,2,28), (None, '0 <= $ <= 3',                  ('Sensor',      '"HumRes {}".format($)')) ),
        'temperature_resolution':   ('<L', (0x5BC,2,30), (None, '0 <= $ <= 3',                  ('Sensor',      '"TempRes {}".format($)')) ),
                                    },      0x5BC,       (None, None,                           ('*',           None)), (None,      False) ),
    'pulse_counter':                ('<L',  0x5C0,       ([4],  None,                           ('Sensor',      '"Counter{} {}".format(#,$)')) ),
    'pulse_counter_type':           ('<H',  0x5D0,       (None, None,                           ('Sensor',      '"CounterType {}".format($)')) ),
    'pulse_counter_type':           ({
        'pulse_counter_type1':      ('<H', (0x5D0,1,0),  (None, None,                           ('Sensor',      '"CounterType1 {}".format($)')) ),
        'pulse_counter_type2':      ('<H', (0x5D0,1,1),  (None, None,                           ('Sensor',      '"CounterType2 {}".format($)')) ),
        'pulse_counter_type3':      ('<H', (0x5D0,1,2),  (None, None,                           ('Sensor',      '"CounterType3 {}".format($)')) ),
        'pulse_counter_type4':      ('<H', (0x5D0,1,3),  (None, None,                           ('Sensor',      '"CounterType4 {}".format($)')) ),
                                    },      0x5D0,       (None, None,                           ('Sensor',      None)), (None,      False) ),
    'pulse_counter_debounce':       ('<H',  0x5D2,       (None, '0 <= $ <= 3200',               ('Sensor',      '"CounterDebounce {}".format($)')) ),
    'rf_code':                      ('B',   0x5D4,       ([17,9],None,                          ('SonoffRF',    None)), '"0x{:02x}".format($)'),
}
# ======================================================================
Setting_5_11_0 = copy.deepcopy(Setting_5_10_0)
Setting_5_11_0.update               ({
    'display_model':                ('B',   0x2D2,       (None, '0 <= $ <= 16',                 ('Display',     '"Model {}".format($)')) ),
    'display_mode':                 ('B',   0x2D3,       (None, '0 <= $ <= 5',                  ('Display',     '"Mode {}".format($)')) ),
    'display_refresh':              ('B',   0x2D4,       (None, '1 <= $ <= 7',                  ('Display',     '"Refresh {}".format($)')) ),
    'display_rows':                 ('B',   0x2D5,       (None, '1 <= $ <= 32',                 ('Display',     '"Rows {}".format($)')) ),
    'display_cols':                 ('B',   0x2D6,       ([2],  '1 <= $ <= 40',                 ('Display',     '"Cols{} {}".format(#,$)')) ),
    'display_address':              ('B',   0x2D8,       ([8],  None,                           ('Display',     '"Address{} {}".format(#,$)')) ),
    'display_dimmer':               ('B',   0x2E0,       (None, '0 <= $ <= 100',                ('Display',     '"Dimmer {}".format($)')) ),
    'display_size':                 ('B',   0x2E1,       (None, '1 <= $ <= 4',                  ('Display',     '"Size {}".format($)')) ),
                                    })
Setting_5_11_0['flag'][0].update    ({
        'light_signal':             ('<L', (0x010,1,18), (None, None,                           ('Sensor',      '"SetOption18 {}".format($)')) ),
                                    })
Setting_5_11_0.pop('mqtt_fingerprinth',None)
# ======================================================================
Setting_5_12_0 = copy.deepcopy(Setting_5_11_0)
Setting_5_12_0['flag'][0].update    ({
        'hass_discovery':           ('<L', (0x010,1,19), (None, None,                           ('SetOption',   '"SetOption19 {}".format($)')) ),
        'not_power_linked':         ('<L', (0x010,1,20), (None, None,                           ('Led',         '"SetOption20 {}".format($)')) ),
        'no_power_on_check':        ('<L', (0x010,1,21), (None, None,                           ('Pow',         '"SetOption21 {}".format($)')) ),
                                    })
# ======================================================================
Setting_5_13_1 = copy.deepcopy(Setting_5_12_0)
Setting_5_13_1['flag'][0].update    ({
        'mqtt_serial':              ('<L', (0x010,1,22), (None, None,                           ('SetOption',   '"SetOption22 {}".format($)')) ),
        'rules_enabled':            ('<L', (0x010,1,23), (None, None,                           ('SetOption',   '"SetOption23 {}".format($)')) ),
        'rules_once':               ('<L', (0x010,1,24), (None, None,                           ('SetOption',   '"SetOption24 {}".format($)')) ),
        'knx_enabled':              ('<L', (0x010,1,25), (None, None,                           ('KNX',         '"KNX_ENABLED {}".format($)')) ),
                                    })
Setting_5_13_1.update               ({
    'baudrate':                     ('B',   0x09D,       (None, None,                           ('Serial',      '"Baudrate {}".format($)')), ('$ * 1200','$ / 1200') ),
    'mqtt_fingerprint':             ('20s', 0x1AD,       ([2],  None,                           ('MQTT',        MqttFingerprint)) ),
    'energy_power_delta':           ('B',   0x33F,       (None, None,                           ('Pow',         '"PowerDelta {}".format($)')) ),
    'light_rotation':               ('<H',  0x39E,       (None, None,                           ('Led',         '"Rotation {}".format($)')) ),
    'serial_delimiter':             ('B',   0x451,       (None, None,                           ('Serial',      '"SerialDelimiter {}".format($)')) ),
    'sbaudrate':                    ('B',   0x452,       (None, None,                           ('Serial',      '"SBaudrate {}".format($)')), ('$ * 1200','$ / 1200') ),
    'knx_GA_registered':            ('B',   0x4A5,       (None, None,                           ('KNX',         None)) ),
    'knx_CB_registered':            ('B',   0x4A8,       (None, None,                           ('KNX',         None)) ),
    'timer':                        ({
        'value':                    ('<L',  0x670,       (None, None,                           ('Timers',      '"Timer{} {{\\\"Arm\\\":{arm},\\\"Mode\\\":{mode},\\\"Time\\\":\\\"{tsign}{time}\\\",\\\"Window\\\":{window},\\\"Days\\\":\\\"{days}\\\",\\\"Repeat\\\":{repeat},\\\"Output\\\":{device},\\\"Action\\\":{power}}}".format(#, arm=bitsRead($,31),mode=bitsRead($,29,2),tsign="-" if bitsRead($,29,2)>0 and bitsRead($,0,11)>(12*60) else "",time=time.strftime("%H:%M",time.gmtime((bitsRead($,0,11) if bitsRead($,29,2)==0 else bitsRead($,0,11) if bitsRead($,0,11)<=(12*60) else bitsRead($,0,11)-(12*60))*60)),window=bitsRead($,11,4),repeat=bitsRead($,15),days="{:07b}".format(bitsRead($,16,7))[::-1],device=bitsRead($,23,4)+1,power=bitsRead($,27,2) )')), ('"0x{:08x}".format($)', False) ),
        'time':                     ('<L', (0x670,11, 0),(None, '0 <= $ < 1440',                ('Timers',      None)) ),
        'window':                   ('<L', (0x670, 4,11),(None, None,                           ('Timers',      None)) ),
        'repeat':                   ('<L', (0x670, 1,15),(None, None,                           ('Timers',      None)) ),
        'days':                     ('<L', (0x670, 7,16),(None, None,                           ('Timers',      None)), '"0b{:07b}".format($)' ),
        'device':                   ('<L', (0x670, 4,23),(None, None,                           ('Timers',      None)) ),
        'power':                    ('<L', (0x670, 2,27),(None, None,                           ('Timers',      None)) ),
        'mode':                     ('<L', (0x670, 2,29),(None, '0 <= $ <= 3',                  ('Timers',      None)) ),
        'arm':                      ('<L', (0x670, 1,31),(None, None,                           ('Timers',      None)) ),
                                    },      0x670,       ([16], None,                           ('Timers',      None)) ),
    'latitude':                     ('i',   0x6B0,       (None, None,                           ('Timers',      '"Latitude {}".format($)')),  ('float($) / 1000000', 'int($ * 1000000)')),
    'longitude':                    ('i',   0x6B4,       (None, None,                           ('Timers',      '"Longitude {}".format($)')), ('float($) / 1000000', 'int($ * 1000000)')),
    'knx_physsical_addr':           ('<H',  0x6B8,       (None, None,                           ('KNX',         None)) ),
    'knx_GA_addr':                  ('<H',  0x6BA,       ([10], None,                           ('KNX',         None)) ),
    'knx_CB_addr':                  ('<H',  0x6CE,       ([10], None,                           ('KNX',         None)) ),
    'knx_GA_param':                 ('B',   0x6E2,       ([10], None,                           ('KNX',         None)) ),
    'knx_CB_param':                 ('B',   0x6EC,       ([10], None,                           ('KNX',         None)) ),
    'rules':                        ('512s',0x800,       (None, None,                           ('Management',  '"Rule {}".format("\\"" if len($)==0 else $)')) ),
                                    })
# ======================================================================
Setting_5_14_0 = copy.deepcopy(Setting_5_13_1)
Setting_5_14_0['flag'][0].update    ({
        'device_index_enable':      ('<L', (0x010,1,26), (None, None,                           ('Main',        '"SetOption26 {}".format($)')) ),
                                    })
Setting_5_14_0['flag'][0].pop('rules_once',None)
Setting_5_14_0.update               ({
    'tflag':                        ({
        'hemis':                    ('<H', (0x2E2,1, 0), (None, None,                           ('Management',  None)) ),
        'week':                     ('<H', (0x2E2,3, 1), (None, '0 <= $ <= 4',                  ('Management',  None)) ),
        'month':                    ('<H', (0x2E2,4, 4), (None, '1 <= $ <= 12',                 ('Management',  None)) ),
        'dow':                      ('<H', (0x2E2,3, 8), (None, '1 <= $ <= 7',                  ('Management',  None)) ),
        'hour':                     ('<H', (0x2E2,5,11), (None, '0 <= $ <= 23',                 ('Management',  None)) ),
                                    },      0x2E2,       ([2],  None,                           ('Management',  None)), (None,      False) ),
    'param':                        ('B',   0x2FC,       ([18], None,                           ('SetOption',   '"SetOption{} {}".format(#+31,$)')) ),
    'toffset':                      ('<h',  0x30E,       ([2],  None,                           ('Management',  '"{cmnd} {hemis},{week},{month},{dow},{hour},{toffset}".format(cmnd="TimeSTD" if idx==1 else "TimeDST", hemis=@["tflag"][#-1]["hemis"], week=@["tflag"][#-1]["week"], month=@["tflag"][#-1]["month"], dow=@["tflag"][#-1]["dow"], hour=@["tflag"][#-1]["hour"], toffset=value)')) ),
                                    })
# ======================================================================
Setting_6_0_0 = copy.deepcopy(Setting_5_14_0)
Setting_6_0_0.update({
    'cfg_holder':                   ('<H',  0x000,       (None, None,                           ('System',      None)), ),
    'cfg_size':                     ('<H',  0x002,       (None, None,                           (INTERNAL,      None)), (None, False)),
    'bootcount':                    ('<H',  0x00C,       (None, None,                           ('System',      None)), (None, False)),
    'cfg_crc':                      ('<H',  0x00E,       (None, None,                           (INTERNAL,      None)), '"0x{:04x}".format($)'),
    'rule_enabled':                 ({
        'rule1':                    ('B',  (0x49F,1,0),  (None, None,                           ('Management',  '"Rule1 {}".format($)')) ),
        'rule2':                    ('B',  (0x49F,1,1),  (None, None,                           ('Management',  '"Rule2 {}".format($)')) ),
        'rule3':                    ('B',  (0x49F,1,2),  (None, None,                           ('Management',  '"Rule3 {}".format($)')) ),
                                    },      0x49F,       (None, None,                           ('Management',  None)), (None,      False) ),
    'rule_once':                    ({
        'rule1':                    ('B',  (0x4A0,1,0),  (None, None,                           ('Management',  '"Rule1 {}".format($+4)')) ),
        'rule2':                    ('B',  (0x4A0,1,1),  (None, None,                           ('Management',  '"Rule2 {}".format($+4)')) ),
        'rule3':                    ('B',  (0x4A0,1,2),  (None, None,                           ('Management',  '"Rule3 {}".format($+4)')) ),
                                    },      0x4A0,       (None, None,                           ('Management',  None)), (None,      False) ),
    'mems':                         ('10s', 0x7CE,       ([5],  None,                           ('Management',  '"Mem{} {}".format(#,"\\"" if len($)==0 else $)')) ),
    'rules':                        ('512s',0x800,       ([3],  None,                           ('Management',  '"Rule{} {}".format(#,"\\"" if len($)==0 else $)')) ),
})
Setting_6_0_0['flag'][0].update     ({
        'knx_enable_enhancement':   ('<L', (0x010,1,27), (None, None,                           ('KNX',         '"KNX_ENHANCED {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_1_1 = copy.deepcopy(Setting_6_0_0)
Setting_6_1_1.update                ({
    'flag3':                        ('<L',  0x3A0,       (None, None,                           ('System',      None)), '"0x{:08x}".format($)' ),
    'switchmode':                   ('B',   0x3A4,       ([8],  '0 <= $ <= 7',                  ('Main',        '"SwitchMode{} {}".format(#,$)')) ),
    'mcp230xx_config':              ({
        'value':                    ('<L',  0x6F6,       (None, None,                           ('MCP230xx',    '"Sensor29 {pin},{pinmode},{pullup},{intmode}".format(pin=#-1, pinmode=@["mcp230xx_config"][#-1]["pinmode"], pullup=@["mcp230xx_config"][#-1]["pullup"], intmode=@["mcp230xx_config"][#-1]["int_report_mode"])')), ('"0x{:08x}".format($)', False) ),
        'pinmode':                  ('<L', (0x6F6,3, 0), (None, '0 <= $ <= 5',                  ('MCP230xx',    None)) ),
        'pullup':                   ('<L', (0x6F6,1, 3), (None, None,                           ('MCP230xx',    None)) ),
        'saved_state':              ('<L', (0x6F6,1, 4), (None, None,                           ('MCP230xx',    None)) ),
        'int_report_mode':          ('<L', (0x6F6,2, 5), (None, None,                           ('MCP230xx',    None)) ),
        'int_report_defer':         ('<L', (0x6F6,4, 7), (None, None,                           ('MCP230xx',    None)) ),
        'int_count_en':             ('<L', (0x6F6,1,11), (None, None,                           ('MCP230xx',    None)) ),
                                     },     0x6F6,       ([16], None,                           ('MCP230xx',    None)), (None,      False) ),
                                    })
Setting_6_1_1['flag'][0].update     ({
        'rf_receive_decimal':       ('<L', (0x010,1,28), (None, None,                           ('SetOption' ,  '"SetOption28 {}".format($)')) ),
        'ir_receive_decimal':       ('<L', (0x010,1,29), (None, None,                           ('SetOption',   '"SetOption29 {}".format($)')) ),
        'hass_light':               ('<L', (0x010,1,30), (None, None,                           ('SetOption',   '"SetOption30 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_2_1 = copy.deepcopy(Setting_6_1_1)
Setting_6_2_1.update                ({
    'rule_stop':                    ({
        'rule1':                    ('B',  (0x1A7,1,0),  (None, None,                           ('Management',  '"Rule1 {}".format($+8)')) ),
        'rule2':                    ('B',  (0x1A7,1,1),  (None, None,                           ('Management',  '"Rule2 {}".format($+8)')) ),
        'rule3':                    ('B',  (0x1A7,1,2),  (None, None,                           ('Management',  '"Rule3 {}".format($+8)')) ),
                                     },     0x1A7,        None),
    'display_rotate':               ('B',   0x2FA,       (None, '0 <= $ <= 3',                  ('Display',     '"Rotate {}".format($)')) ),
    'display_font':                 ('B',   0x312,       (None, '1 <= $ <= 4',                  ('Display',     '"Font {}".format($)')) ),
    'flag3':                        ({
         'timers_enable':           ('<L', (0x3A0,1, 0), (None, None,                           ('Timers',      '"Timers {}".format($)')) ),
         'user_esp8285_enable':     ('<L', (0x3A0,1,31), (None, None,                           ('System',      None)) ),
                                    },      0x3A0,       (None, None,                           ('*',           None)), (None,      False) ),
    'button_debounce':              ('<H',  0x542,       (None, '40 <= $ <= 1000',              ('Main',        '"ButtonDebounce {}".format($)')) ),
    'switch_debounce':              ('<H',  0x66E,       (None, '40 <= $ <= 1000',              ('Main',        '"SwitchDebounce {}".format($)')) ),
    'mcp230xx_int_prio':            ('B',   0x716,       (None, None,                           ('MCP230xx',    None)) ),
    'mcp230xx_int_timer':           ('<H',  0x718,       (None, None,                           ('MCP230xx',    None)) ),
                                    })
Setting_6_2_1['flag'][0].pop('rules_enabled',None)
Setting_6_2_1['flag'][0].update     ({
        'mqtt_serial_raw':          ('<L', (0x010,1,23), (None, None,                           ('SetOption',   '"SetOption23 {}".format($)')) ),
        'global_state':             ('<L', (0x010,1,31), (None, None,                           ('SetOption',   '"SetOption31 {}".format($)')) ),
                                    })
Setting_6_2_1['flag2'][0].update    ({
    # currently unsupported Tasmota command, should be Sensor32, still needs to implement
    'axis_resolution':              ('<L', (0x5BC,2,13), (None, None,                           ('System',      None)) ),
                                    })
# ======================================================================
Setting_6_2_1_2 = copy.deepcopy(Setting_6_2_1)
Setting_6_2_1_2['flag3'][0].update  ({
         'user_esp8285_enable':     ('<L', (0x3A0,1, 1), (None, None,                           ('SetOption',   '"SetOption51 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_2_1_3 = copy.deepcopy(Setting_6_2_1_2)
Setting_6_2_1_3['flag2'][0].update  ({
        'frequency_resolution':     ('<L', (0x5BC,2,11), (None, '0 <= $ <= 3',                  ('Pow',         '"FreqRes {}".format($)')) ),
                                    })
Setting_6_2_1_3['flag3'][0].update  ({
        'time_append_timezone':     ('<L', (0x3A0,1, 2), (None, None,                           ('SetOption',   '"SetOption52 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_2_1_6 = copy.deepcopy(Setting_6_2_1_3)
Setting_6_2_1_6.update({
    'energy_frequency_calibration': ('<L',  0x7C8,       (None, '45000 < $ < 65000',            ('Pow',         '"FrequencySet {}".format($)')) ),
})
# ======================================================================
Setting_6_2_1_10 = copy.deepcopy(Setting_6_2_1_6)
Setting_6_2_1_10.update({
    'rgbwwTable':                   ('B',   0x71A,       ([5],  None,                           ('System',      None)) ), # RGBWWTable 255,135,70,255,255
})
# ======================================================================
Setting_6_2_1_14 = copy.deepcopy(Setting_6_2_1_10)
Setting_6_2_1_14.update({
    'weight_item':                  ('<H',  0x7BC,       (None, None,                           ('Management',  '"Sensor34 6 {}".format($)')), ('int($ * 10)', 'float($) / 10') ),            # undocumented
    'weight_max':                   ('<H',  0x7BE,       (None, None,                           ('Management',  '"Sensor34 5 {}".format($)')), ('float($) / 1000', 'int($ * 1000)') ),        # undocumented
    'weight_reference':             ('<L',  0x7C0,       (None, None,                           ('Management',  '"Sensor34 3 {}".format($)')) ),     # undocumented
    'weight_calibration':           ('<L',  0x7C4,       (None, None,                           ('Management',  '"Sensor34 4 {}".format($)')) ),     # undocumented
    'web_refresh':                  ('<H',  0x7CC,       (None, '1000 <= $ <= 10000',           ('Management',  '"WebRefresh {}".format($)')) ),     # undocumented
})
Setting_6_2_1_14['flag2'][0].update ({
        'weight_resolution':        ('<L', (0x5BC,2, 9), (None, '0 <= $ <= 3',                  ('Management',  '"WeightRes {}".format($)')) ),      # undocumented
                                    })
# ======================================================================
Setting_6_2_1_19 = copy.deepcopy(Setting_6_2_1_14)
Setting_6_2_1_19.update({
    'weight_item':                  ('<L',  0x7B8,       (None, None,                           ('Management',  '"Sensor34 6 {}".format($)')), ('int($ * 10)', 'float($) / 10') ),            # undocumented
})
Setting_6_2_1_20 = Setting_6_2_1_19
Setting_6_2_1_20['flag3'][0].update ({
        'gui_hostname_ip':          ('<L', (0x3A0,1,3),  (None, None,                           ('SetOption',   '"SetOption53 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0 = copy.deepcopy(Setting_6_2_1_20)
Setting_6_3_0.update({
    'energy_kWhtotal_time':         ('<L',  0x7B4,       (None, None,                           ('System',           None)) ),
})
# ======================================================================
Setting_6_3_0_2 = copy.deepcopy(Setting_6_3_0)
Setting_6_3_0_2.update({
    'timezone_minutes':             ('B',   0x66D,       (None, None,                           ('System',           None)) ),
})
Setting_6_3_0_2['flag'][0].pop('rules_once',None)
Setting_6_3_0_2['flag'][0].update   ({
        'pressure_conversion':      ('<L', (0x010,1,24), (None, None,                           ('SetOption',   '"SetOption24 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_4 = copy.deepcopy(Setting_6_3_0_2)
Setting_6_3_0_4.update({
    'drivers':                      ('<L',  0x794,       ([3],  None,                           ('System',           None)), '"0x{:08x}".format($)' ),
    'monitors':                     ('<L',  0x7A0,       (None, None,                           ('System',           None)), '"0x{:08x}".format($)' ),
    'sensors':                      ('<L',  0x7A4,       ([3],  None,                           ('System',           None)), '"0x{:08x}".format($)' ),
    'displays':                     ('<L',  0x7B0,       (None, None,                           ('System',           None)), '"0x{:08x}".format($)' ),
})
Setting_6_3_0_4['flag3'][0].update ({
        'tuya_apply_o20':           ('<L', (0x3A0,1, 4), (None, None,                           ('SetOption',   '"SetOption54 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_8 = copy.deepcopy(Setting_6_3_0_4)
Setting_6_3_0_8['flag3'][0].update ({
        'hass_short_discovery_msg': ('<L', (0x3A0,1, 5), (None, None,                           ('SetOption',   '"SetOption55 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_10 = copy.deepcopy(Setting_6_3_0_8)
Setting_6_3_0_10['flag3'][0].update ({
        'use_wifi_scan':            ('<L', (0x3A0,1, 6), (None, None,                           ('SetOption',   '"SetOption56 {}".format($)')) ),
        'use_wifi_rescan':          ('<L', (0x3A0,1, 7), (None, None,                           ('SetOption',   '"SetOption57 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_11 = copy.deepcopy(Setting_6_3_0_10)
Setting_6_3_0_11['flag3'][0].update ({
        'receive_raw':          	('<L', (0x3A0,1, 8), (None, None,                           ('SetOption',   '"SetOption58 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_13 = copy.deepcopy(Setting_6_3_0_11)
Setting_6_3_0_13['flag3'][0].update ({
        'hass_tele_on_power':       ('<L', (0x3A0,1, 9), (None, None,                           ('SetOption',   '"SetOption59 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_14 = copy.deepcopy(Setting_6_3_0_13)
Setting_6_3_0_14['flag2'][0].update ({
        'calc_resolution':          ('<L', (0x5BC,3, 6), (None, '0 <= $ <= 7',                  ('Management',  '"CalcRes {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_15 = copy.deepcopy(Setting_6_3_0_14)
Setting_6_3_0_15['flag3'][0].update ({
        'sleep_normal':             ('<L', (0x3A0,1,10), (None, None,                           ('SetOption',   '"SetOption60 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_3_0_16 = copy.deepcopy(Setting_6_3_0_15)
Setting_6_3_0_16['mcp230xx_config'][0].update ({
        'int_retain_flag':          ('<L', (0x6F6,1,12), (None, None,                           ('MCP230xx',    None)) ),
                                    })
Setting_6_3_0_16['flag3'][0].update ({
        'button_switch_force_local':('<L', (0x3A0,1,11), (None, None,                           ('SetOption',   '"SetOption61 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_4_0_2 = copy.deepcopy(Setting_6_3_0_16)
Setting_6_4_0_2['flag3'][0].pop('hass_short_discovery_msg',None)
# ======================================================================
Setting_6_4_1_4 = copy.deepcopy(Setting_6_4_0_2)
Setting_6_4_1_4['flag3'][0].update ({
        'mdns_enabled':             ('<L', (0x3A0,1, 5), (None, None,                           ('SetOption',   '"SetOption55 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_4_1_7 = copy.deepcopy(Setting_6_4_1_4)
Setting_6_4_1_7['flag3'][0].update ({
        'no_pullup':                ('<L', (0x3A0,1,12), (None, None,                           ('SetOption',   '"SetOption62 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_4_1_8 = copy.deepcopy(Setting_6_4_1_7)
Setting_6_4_1_8['flag3'][0].update ({
        'split_interlock':          ('<L', (0x3A0,1,13), (None, None,                           ('SetOption',   '"SetOption63 {}".format($)')) ),
                                    })
# ======================================================================
Setting_6_4_1_11 = copy.deepcopy(Setting_6_4_1_8)
Setting_6_4_1_11['flag3'][0].pop('split_interlock',None)
Setting_6_4_1_11.update            ({
    'interlock':                    ('B',   0x4CA,       ([4],  None,                           ('Main',        None)), '"0x{:02x}".format($)' ),
                                    })
# ======================================================================
Setting_6_4_1_13 = copy.deepcopy(Setting_6_4_1_11)
Setting_6_4_1_13.update            ({
    'SensorBits1':                 ({
        'mhz19b_abc_disable':       ('B',  (0x717,1, 7), (None, None,                           ('Sensor',      '"Sensor15 {}".format($)')) ),
                                    },      0x717,       (None, None,                           ('*',           None)), (None,      False) ),
                                    })
# ======================================================================
Setting_6_4_1_16 = copy.deepcopy(Setting_6_4_1_13)
Setting_6_4_1_16.update({
    'user_template_base':          ('B',   0x71F,       (None,  None,                           ('Management',  '"Template {}".format($)')), ('$ + 1','$ - 1') ),
    'user_template':               ({
        'name':                     ('15s', 0x720, (None, None,                                 ('Management',  '"Template {{\\\"NAME\\\":\\\"{}\\\"}}".format($)' )) ),
        'gpio00':                   ('B',   0x72F, (None, None,                                 ('Management',  '"Template {{\\\"GPIO\\\":[{},{},{},{},{},{},{},{},{},{},{},{},{}]}}".format(@["user_template"]["gpio00"],@["user_template"]["gpio01"],@["user_template"]["gpio02"],@["user_template"]["gpio03"],@["user_template"]["gpio04"],@["user_template"]["gpio05"],@["user_template"]["gpio09"],@["user_template"]["gpio10"],@["user_template"]["gpio12"],@["user_template"]["gpio13"],@["user_template"]["gpio14"],@["user_template"]["gpio15"],@["user_template"]["gpio16"])')) ),
        'gpio01':                   ('B',   0x730, (None, None,                                 ('Management',  None)) ),
        'gpio02':                   ('B',   0x731, (None, None,                                 ('Management',  None)) ),
        'gpio03':                   ('B',   0x732, (None, None,                                 ('Management',  None)) ),
        'gpio04':                   ('B',   0x733, (None, None,                                 ('Management',  None)) ),
        'gpio05':                   ('B',   0x734, (None, None,                                 ('Management',  None)) ),
        'gpio09':                   ('B',   0x735, (None, None,                                 ('Management',  None)) ),
        'gpio10':                   ('B',   0x736, (None, None,                                 ('Management',  None)) ),
        'gpio12':                   ('B',   0x737, (None, None,                                 ('Management',  None)) ),
        'gpio13':                   ('B',   0x738, (None, None,                                 ('Management',  None)) ),
        'gpio14':                   ('B',   0x739, (None, None,                                 ('Management',  None)) ),
        'gpio15':                   ('B',   0x73A, (None, None,                                 ('Management',  None)) ),
        'gpio16':                   ('B',   0x73B, (None, None,                                 ('Management',  None)) ),
        'flag':                         ({
            'value':                        ('B',   0x73C     ,  (None, None,                   ('Management',  '"Template {{\\\"FLAG\\\":{}}}".format($)')) ),
            'adc0':                         ('B',  (0x73C,1,0),  (None, None,                   ('Management',  None)) ),
            'pullup':                       ('B',  (0x73C,1,1),  (None, None,                   ('Management',  None)) ),
                                         },      0x73C,       (None, None,                      ('Management',  None))
                                        ),
                                    },      0x720,       (None, None,                           ('Management',  None)) 
                                   ),
})
# ======================================================================
Setting_6_4_1_17 = copy.deepcopy(Setting_6_4_1_16)
Setting_6_4_1_17['flag3'][0].pop('no_pullup',None)
# ======================================================================
Setting_6_4_1_18 = copy.deepcopy(Setting_6_4_1_17)
Setting_6_4_1_18['flag3'][0].update ({
        'no_hold_retain':                ('<L', (0x3A0,1,12), (None, None,                      ('SetOption',   '"SetOption62 {}".format($)')) ),
                                    })
# ======================================================================
Settings = [
            (0x6040112, 0xe00, Setting_6_4_1_18),
            (0x6040111, 0xe00, Setting_6_4_1_17),
            (0x6040110, 0xe00, Setting_6_4_1_16),
            (0x604010D, 0xe00, Setting_6_4_1_13),
            (0x604010B, 0xe00, Setting_6_4_1_11),
            (0x6040108, 0xe00, Setting_6_4_1_8),
            (0x6040107, 0xe00, Setting_6_4_1_7),
            (0x6040104, 0xe00, Setting_6_4_1_4),
            (0x6040002, 0xe00, Setting_6_4_0_2),
            (0x6030010, 0xe00, Setting_6_3_0_16),
            (0x603000F, 0xe00, Setting_6_3_0_15),
            (0x603000E, 0xe00, Setting_6_3_0_14),
            (0x603000D, 0xe00, Setting_6_3_0_13),
            (0x603000B, 0xe00, Setting_6_3_0_11),
            (0x603000A, 0xe00, Setting_6_3_0_10),
            (0x6030008, 0xe00, Setting_6_3_0_8),
            (0x6030004, 0xe00, Setting_6_3_0_4),
            (0x6030002, 0xe00, Setting_6_3_0_2),
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

# ======================================================================
# Common helper
# ======================================================================
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
                            sdelimiter=' ' if status is not None and status > 0 and typ is not None else '',
                            sstatus=status if status is not None and status > 0 else '',
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
        if len(header) > 0:
            return header[0].rstrip()
        return ''

    def contenttype(self):
        for item in str(self.contents).split('\n'):
            ditem = item.split(":")
            if ditem[0].strip().lower() == 'content-type' and len(ditem) > 1:
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
        if len(options) <= 1:
            action._formatted_action_invocation = orgstr
            return orgstr

        return_list = []
        for option in options:
            meta = ""
            arg = option.split(' ')
            if len(arg) > 1:
                meta = arg[1]
            return_list.append(arg[0])
        if len(meta) > 0 and len(return_list) > 0:
            return_list[len(return_list)-1] += " "+meta
        action._formatted_action_invocation = ', '.join(return_list)
        return action._formatted_action_invocation


# ======================================================================
# Tasmota config data handling
# ======================================================================
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
    Search for version, size and settings to be used depending on given binary config data

    @param decode_cfg:
        binary config data (decrypted)

    @return:
        version, size, settings to use; None if version is invalid
    """
    version = 0x0
    size = setting = None
    version = GetField(decode_cfg,  'version', Setting_6_2_1['version'], raw=True)
    # search setting definition top-down
    for cfg in sorted(Settings, key=lambda s: s[0], reverse=True):
        if version >= cfg[0]:
            size = cfg[1]
            setting = cfg[2]
            break

    return version, size, setting


def GetGroupList(setting):
    """
    Get all avilable group definition from setting
    
    @return:
        configargparse.parse_args() result
    """
    groups = set()

    for name in setting:
        dev = setting[name]
        format, group = GetFieldDef(dev, fields="format, group")
        if group is not None and len(group) > 0:
            groups.add(group)
        if isinstance(format, dict):
            subgroups = GetGroupList(format)
            if subgroups is not None and len(subgroups) > 0:
                for group in subgroups:
                    groups.add(group)

    groups=list(groups)
    groups.sort()
    return groups
    

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
            with open(filename, "r") as f:
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
                        with open(filename, "rb") as inputfile:
                            inputbin = inputfile.read()
                        if struct.unpack_from('<L', inputbin, 0)[0] == BINARYFILE_MAGIC:
                            filetype = FileType.BIN
                        else:
                            filetype = FileType.INVALID_BIN
        except:
            filetype = FileType.FILE_NOT_FOUND
            pass
    except:
        filetype = FileType.FILE_NOT_FOUND
        pass

    return filetype


def GetVersionStr(version):
    """
    Create human readable version string

    @param version:
        version integer

    @return:
        version string
    """
    if isinstance(version, (unicode,str)):
        version = int(version, 0)
    major = ((version>>24) & 0xff)
    minor = ((version>>16) & 0xff)
    release = ((version>> 8) & 0xff)
    subrelease = (version & 0xff)
    if major >= 6:
        if subrelease > 0:
            subreleasestr = str(subrelease)
        else:
            subreleasestr = ''
    else:
        if subrelease > 0:
            subreleasestr = str(chr(subrelease+ord('a')-1))
        else:
            subreleasestr = ''
    return "{:d}.{:d}.{:d}{}{}".format( major, minor, release, '.' if (major >= 6 and subreleasestr != '') else '', subreleasestr)


def MakeFilename(filename, filetype, configmapping):
    """
    Replace variables within a filename

    @param filename:
        original filename possible containing replacements:
        @v:
            Tasmota version from config data
        @f:
            friendlyname from config data
        @h:
            hostname from config data
        @H:
            hostname from device (-d arg only)
    @param filetype:
        FileType.x object - creates extension if not None
    @param configmapping:
        binary config data (decrypted)

    @return:
        New filename with replacements
    """
    config_version = config_friendlyname = config_hostname = device_hostname = ''

    if 'version' in configmapping:
        config_version = GetVersionStr( int(str(configmapping['version']), 0) )
    if 'friendlyname' in configmapping:
        config_friendlyname = configmapping['friendlyname'][0]
    if 'hostname' in configmapping:
        if configmapping['hostname'].find('%') < 0:
            config_hostname = configmapping['hostname']
    if filename.find('@H') >= 0 and args.device is not None:
        device_hostname = GetTasmotaHostname(args.device, args.port, username=args.username, password=args.password)
        if device_hostname is None:
            device_hostname = ''

    dirname = basename = ext = ''

    # split file parts
    dirname = os.path.normpath(os.path.dirname(filename))
    basename = os.path.basename(filename)
    name, ext = os.path.splitext(basename)

    # make a valid filename
    try:
        name = name.decode('unicode-escape').translate(dict((ord(char), None) for char in '\/*?:"<>|'))
    except:
        pass
    name = str(name.replace(' ','_'))

    # append extension based on filetype if not given
    if len(ext) and ext[0]=='.':
        ext = ext[1:]
    if filetype is not None and args.extension and (len(ext)<2 or all(c.isdigit() for c in ext)):
        ext = filetype.lower()

    # join filename + extension
    if len(ext):
        name_ext = name+'.'+ext
    else:
        name_ext = name

    # join path and filename
    try:
        filename = os.path.join(dirname, name_ext)
    except:
        pass

    filename = filename.replace('@v', config_version)
    filename = filename.replace('@f', config_friendlyname )
    filename = filename.replace('@h', config_hostname )
    filename = filename.replace('@H', device_hostname )

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


def LoadTasmotaConfig(filename):
    """
    Load config from Tasmota file
    
    @param filename:
        filename to load

    @return:
        binary config data (encrypted) or None on error
    """

    encode_cfg = None

    # read config from a file
    if not os.path.isfile(filename):    # check file exists
        exit(ExitCode.FILE_NOT_FOUND, "File '{}' not found".format(filename),line=inspect.getlineno(inspect.currentframe()))
    try:
        with open(filename, "rb") as tasmotafile:
            encode_cfg = tasmotafile.read()
    except Exception, e:
        exit(e[0], "'{}' {}".format(filename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    return encode_cfg


def TasmotaGet(cmnd, host, port, username=DEFAULTS['source']['username'], password=None, contenttype = None):
    """
    Tasmota http request

    @param host:
        hostname or IP of Tasmota device
    @param port:
        http port of Tasmota device
    @param username:
        optional username for Tasmota web login
    @param password
        optional password for Tasmota web login

    @return:
        binary config data (encrypted) or None on error
    """
    body = None

    # read config direct from device via http
    c = pycurl.Curl()
    buffer = io.BytesIO()
    c.setopt(c.WRITEDATA, buffer)
    header = HTTPHeader()
    c.setopt(c.HEADERFUNCTION, header.store)
    c.setopt(c.FOLLOWLOCATION, True)
    c.setopt(c.URL, MakeUrl(host, port, cmnd))
    if username is not None and password is not None:
        c.setopt(c.HTTPAUTH, c.HTTPAUTH_BASIC)
        c.setopt(c.USERPWD, username + ':' + password)
    c.setopt(c.HTTPGET, True)
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

    if responsecode >= 400:
        exit(responsecode, 'HTTP result: {}'.format(header.response()),line=inspect.getlineno(inspect.currentframe()))
    elif contenttype is not None and header.contenttype()!=contenttype:
        exit(ExitCode.DOWNLOAD_CONFIG_ERROR, "Device did not response properly, may be Tasmota webserver admin mode is disabled (WebServer 2)",line=inspect.getlineno(inspect.currentframe()))

    try:
        body = buffer.getvalue()
    except:
        pass
    
    return responsecode, body


def GetTasmotaHostname(host, port, username=DEFAULTS['source']['username'], password=None):
    """
    Get Tasmota hostname from device

    @param host:
        hostname or IP of Tasmota device
    @param port:
        http port of Tasmota device
    @param username:
        optional username for Tasmota web login
    @param password
        optional password for Tasmota web login

    @return:
        Tasmota real hostname or None on error
    """
    hostname = None

    loginstr = ""
    if password is not None:
        loginstr = "user={}&password={}&".format(urllib2.quote(username), urllib2.quote(password))
    # get hostname
    responsecode, body = TasmotaGet("cm?{}cmnd=status%205".format(loginstr), host, port, username=username, password=password)
    if body is not None:
        jsonbody = json.loads(body)
        if "StatusNET" in jsonbody and "Hostname" in jsonbody["StatusNET"]:
            hostname = jsonbody["StatusNET"]["Hostname"]
            if args.verbose:
                message("Hostname for '{}' retrieved: '{}'".format(host, hostname), typ=LogType.INFO)

    return hostname


def PullTasmotaConfig(host, port, username=DEFAULTS['source']['username'], password=None):
    """
    Pull config from Tasmota device

    @param host:
        hostname or IP of Tasmota device
    @param port:
        http port of Tasmota device
    @param username:
        optional username for Tasmota web login
    @param password
        optional password for Tasmota web login

    @return:
        binary config data (encrypted) or None on error
    """
    responsecode, body = TasmotaGet('dl', host, port, username, password, contenttype='application/octet-stream')

    return body


def PushTasmotaConfig(encode_cfg, host, port, username=DEFAULTS['source']['username'], password=None):
    """
    Upload binary data to a Tasmota host using http

    @param encode_cfg:
        encrypted binary data or filename containing Tasmota encrypted binary config
    @param host:
        hostname or IP of Tasmota device
    @param port:
        http port of Tasmota device
    @param username:
        optional username for Tasmota web login
    @param password
        optional password for Tasmota web login

    @return
        errorcode, errorstring
        errorcode=0 if success, otherwise http response or exception code
    """
    if isinstance(encode_cfg, bytearray):
        encode_cfg = str(encode_cfg)

    # get restore config page first to set internal Tasmota vars
    responsecode, body = TasmotaGet('rs?', host, port, username, password, contenttype='text/html')
    if body is None:
        return responsecode, "ERROR"

    # post data
    c = pycurl.Curl()
    header = HTTPHeader()
    c.setopt(c.HEADERFUNCTION, header.store)
    c.setopt(c.WRITEFUNCTION, lambda x: None)
    c.setopt(c.POST, 1)
    c.setopt(c.URL, MakeUrl(host, port, 'u2'))
    if username is not None and password is not None:
        c.setopt(c.HTTPAUTH, c.HTTPAUTH_BASIC)
        c.setopt(c.USERPWD, username + ':' + password)
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

    if responsecode >= 400:
        return responsecode, header.response()
    elif header.contenttype() != 'text/html':
        return ExitCode.UPLOAD_CONFIG_ERROR, "Device did not response properly, may be Tasmota webserver admin mode is disabled (WebServer 2)"

    return 0, 'OK'


def DecryptEncrypt(obj):
    """
    Decrpt/Encrypt binary config data

    @param obj:
        binary config data

    @return:
        decrypted configuration (if obj contains encrypted data)
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


def GetFieldDef(fielddef, fields="format, addrdef, baseaddr, bits, bitshift, datadef, arraydef, validate, cmd, group, tasmotacmnd, converter, readconverter, writeconverter"):
    
    """
    Get field definition items

    @param fielddef:
        field format - see "Settings dictionary" above
    @param fields:
        comma separated string list of values to be returned
        possible values see fields default

    @return:
        set of values defined in <fields>
    """
    format = addrdef = baseaddr = datadef = arraydef = validate = cmd = group = tasmotacmnd = converter = readconverter = writeconverter = None
    bits = bitshift = 0

    # calling with nothing is wrong
    if fielddef is None:
        print >> sys.stderr, '<fielddef> is None'
        raise SyntaxError('<fielddef> error')

    # get top level items
    if len(fielddef) == 3:
        # converter not present
        format, addrdef, datadef = fielddef
    elif len(fielddef) == 4:
        # converter present
        format, addrdef, datadef, converter = fielddef
    else:
        print >> sys.stderr, 'wrong <fielddef> {} length ({}) in setting'.format(fielddef, len(fielddef))
        raise SyntaxError('<fielddef> error')

    # ignore calls with 'root' setting
    if isinstance(format, dict) and baseaddr is None and datadef is None:
        return eval(fields)

    if not isinstance(format, (unicode,str,dict)):
        print >> sys.stderr, 'wrong <format> {} type {} in <fielddef> {}'.format(format, type(format), fielddef)
        raise SyntaxError('<fielddef> error')

    # extract addrdef items
    baseaddr = addrdef
    if isinstance(baseaddr, (list,tuple)):
        if len(baseaddr) == 3:
            # baseaddr bit definition
            baseaddr, bits, bitshift = baseaddr
            if not isinstance(bits, int):
                print >> sys.stderr, '<bits> must be a integer in <fielddef> {}'.format(bits, fielddef)
                raise SyntaxError('<fielddef> error')
            if not isinstance(bitshift, int):
                print >> sys.stderr, '<bitshift> must be a integer in <fielddef> {}'.format(bitshift, fielddef)
                raise SyntaxError('<fielddef> error')
        else:
            print >> sys.stderr, 'wrong <addrdef> {} length ({}) in <fielddef> {}'.format(addrdef, len(addrdef), fielddef)
            raise SyntaxError('<fielddef> error')
    if not isinstance(baseaddr, int):
        print >> sys.stderr, '<baseaddr> must be a integer in <fielddef> {}'.format(baseaddr, fielddef)
        raise SyntaxError('<fielddef> error')

    # extract datadef items
    arraydef = datadef
    if isinstance(datadef, (tuple)):
        if len(datadef) == 2:
            # datadef has a validator
            arraydef, validate = datadef
        elif len(datadef) == 3:
            # datadef has a validator and cmd set
            arraydef, validate, cmd = datadef
            # cmd must be a tuple with 2 objects
            if isinstance(cmd, (tuple)) and len(cmd) == 2:
                group, tasmotacmnd = cmd
                if group is not None and not isinstance(group, (str, unicode)):
                    print >> sys.stderr, 'wrong <group> {} in <fielddef> {}'.format(group, fielddef)
                    raise SyntaxError('<fielddef> error')
                if tasmotacmnd is not None and not callable(tasmotacmnd) and not isinstance(tasmotacmnd, (str, unicode)):
                    print >> sys.stderr, 'wrong <tasmotacmnd> {} in <fielddef> {}'.format(tasmotacmnd, fielddef)
                    raise SyntaxError('<fielddef> error')
            else:
                print >> sys.stderr, 'wrong <cmd> {} length ({}) in <fielddef> {}'.format(cmd, len(cmd), fielddef)
                raise SyntaxError('<fielddef> error')
        else:
            print >> sys.stderr, 'wrong <datadef> {} length ({}) in <fielddef> {}'.format(datadef, len(datadef), fielddef)
            raise SyntaxError('<fielddef> error')

        if validate is not None and (not isinstance(validate, (unicode,str)) and not callable(validate)):
            print >> sys.stderr, 'wrong <validate> {} type {} in <fielddef> {}'.format(validate, type(validate), fielddef)
            raise SyntaxError('<fielddef> error')

    # convert single int into one-dimensional list
    if isinstance(arraydef, int):
        arraydef = [arraydef]

    if arraydef is not None and not isinstance(arraydef, (list)):
        print >> sys.stderr, 'wrong <arraydef> {} type {} in <fielddef> {}'.format(arraydef, type(arraydef), fielddef)
        raise SyntaxError('<fielddef> error')

    # get read/write converter items
    readconverter = converter
    if isinstance(converter, (tuple)):
        if len(converter) == 2:
            # converter has read/write converter
            readconverter, writeconverter = converter
            if readconverter is not None  and not isinstance(readconverter, (str,unicode)) and not callable(readconverter):
                print >> sys.stderr, 'wrong <readconverter> {} type {} in <fielddef> {}'.format(readconverter, type(readconverter), fielddef)
                raise SyntaxError('<fielddef> error')
            if writeconverter is not None and (not isinstance(writeconverter, (bool,str,unicode)) and not callable(writeconverter)):
                print >> sys.stderr, 'wrong <writeconverter> {} type {} in <fielddef> {}'.format(writeconverter, type(writeconverter), fielddef)
                raise SyntaxError('<fielddef> error')
        else:
            print >> sys.stderr, 'wrong <converter> {} length ({}) in <fielddef> {}'.format(converter, len(converter), fielddef)
            raise SyntaxError('<fielddef> error')
    

    return eval(fields)


def ReadWriteConverter(value, fielddef, read=True, raw=False):
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
    converter, readconverter, writeconverter = GetFieldDef(fielddef, fields='converter, readconverter, writeconverter')

    # call password functions even if raw value should be processed
    if read and callable(readconverter) and readconverter == passwordread:
        raw = False
    if not read and callable(writeconverter) and writeconverter == passwordwrite:
        raw = False

    if not raw and converter is not None:
        conv = readconverter if read else writeconverter
        try:
            if isinstance(conv, str): # evaluate strings
                return eval(conv.replace('$','value'))
            elif callable(conv):     # use as format function
                return conv(value)
        except Exception, e:
            exit(e[0], e[1], typ=LogType.WARNING, line=inspect.getlineno(inspect.currentframe()))

    return value


def CmndConverter(valuemapping, value, idx, fielddef):
    """
    Convert field value into Tasmota command if available

    @param valuemapping:
        data mapping
    @param value:
        original value
    @param fielddef
        field definition - see "Settings dictionary" above

    @return:
        converted value or None if unable to convert
    """
    converter, readconverter, writeconverter, group, tasmotacmnd = GetFieldDef(fielddef, fields='converter, readconverter, writeconverter, group, tasmotacmnd')

    result = None

    if (callable(readconverter) and readconverter == passwordread) or (callable(writeconverter) and writeconverter == passwordwrite):
        if value == HIDDEN_PASSWORD:
            return None
        else:
            result = value

    if tasmotacmnd is not None and (callable(tasmotacmnd) or len(tasmotacmnd) > 0):
        if idx is not None:
            idx += 1
        if isinstance(tasmotacmnd, str): # evaluate strings
            if idx is not None:
                evalstr = tasmotacmnd.replace('$','value').replace('#','idx').replace('@','valuemapping')
            else:
                evalstr = tasmotacmnd.replace('$','value').replace('@','valuemapping')
            # ~ try:
            result = eval(evalstr)
            # ~ except:
                # ~ print evalstr
                # ~ print value
                
        elif callable(tasmotacmnd):      # use as format function
            if idx is not None:
                result = tasmotacmnd(value, idx)
            else:
                result = tasmotacmnd(value)
    
    return result


def ValidateValue(value, fielddef):
    """
    Validate a value if validator is defined in fielddef

    @param value:
        original value
    @param fielddef
        field definition - see "Settings dictionary" above

    @return:
        True if value is valid, False if invalid
    """
    validate = GetFieldDef(fielddef, fields='validate')
    
    if value == 0:
        # can not complete all validate condition
        # some Tasmota values are not allowed to be 0 on input
        # even though these values are set to 0 on Tasmota initial.
        # so we can't validate 0 values
        return True;

    valid = True
    try:
        if isinstance(validate, str): # evaluate strings
            valid = eval(validate.replace('$','value'))
        elif callable(validate):     # use as format function
            valid = validate(value)
    except:
        valid = False

    return valid


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
    format = GetFieldDef(fielddef, fields='format')
    _min = 0
    _max = 0
    
    if format[-1:] in minmax:
        _min, _max = minmax[format[-1:]]
    elif format[-1:] in ['s','p']:
        # s and p may have a prefix as length
        match = re.search("\s*(\d+)", format)
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
    format, addrdef, arraydef = GetFieldDef(fielddef, fields='format, addrdef, arraydef')

    # <arraydef> contains a integer list
    if isinstance(arraydef, list) and len(arraydef) > 0:
        # arraydef contains a list
        # calc size recursive by sum of all elements
        for i in range(0, arraydef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            if len(arraydef) > 1:
                length += GetFieldLength( (format, addrdef, subfielddef) )
            # single array
            else:
                length += GetFieldLength( (format, addrdef, None) )

    elif isinstance(format, dict):
            # -> iterate through format
            addr = None
            setting = format
            for name in setting:
                baseaddr, bits, bitshift = GetFieldDef(setting[name], fields='baseaddr, bits, bitshift')
                _len = GetFieldLength(setting[name])
                if addr != baseaddr:
                    addr = baseaddr
                    length += _len

    # a simple value
    elif isinstance(format, str):
        if format[-1:] in ['b','B','c','?']:
            length=1
        elif format[-1:] in ['h','H']:
            length=2
        elif format[-1:] in ['i','I','l','L','f']:
            length=4
        elif format[-1:] in ['q','Q','d']:
            length=8
        elif format[-1:] in ['s','p']:
            # s and p may have a prefix as length
            match = re.search("\s*(\d+)", format)
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

    format, addrdef, datadef, arraydef, validate, cmd, converter = GetFieldDef(fielddef, fields='format, addrdef, datadef, arraydef, validate, cmd, converter')

    # create new arraydef
    if len(arraydef) > 1:
        arraydef = arraydef[1:]
    else:
        arraydef = None

    # create new datadef
    if isinstance(datadef, tuple):
        if cmd is not None:
            datadef = (arraydef, validate, cmd)
        else:
            datadef = (arraydef, validate)
    else:
        datadef = arraydef

    # set new field def
    subfielddef = None
    if converter is not None:
        subfielddef = (format, addrdef, datadef, converter)
    else:
        subfielddef = (format, addrdef, datadef)

    return subfielddef


def IsFilterGroup(group):
    """
    Check if group is valid on filter

    @param grooup:
        group name to check

    @return:
        True if group is in filter, otherwise False
    """
    if args.filter is not None:
        if group is None:
            return False
        if group != INTERNAL and group != '*' and group not in args.filter:
            return False
    return True


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
        field mapping
    """

    if isinstance(dobj, bytearray):
        dobj = str(dobj)

    valuemapping = None

    # get field definition
    format, baseaddr, bits, bitshift, arraydef, group, tasmotacmnd = GetFieldDef(fielddef, fields='format, baseaddr, bits, bitshift, arraydef, group, tasmotacmnd')

    # filter groups
    if not IsFilterGroup(group):
        return valuemapping

    # <arraydef> contains a integer list
    if isinstance(arraydef, list) and len(arraydef) > 0:
        valuemapping = []
        offset = 0
        for i in range(0, arraydef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            length = GetFieldLength(subfielddef)
            if length != 0:
                value = GetField(dobj, fieldname, subfielddef, raw=raw, addroffset=addroffset+offset)
                valuemapping.append(value)
            offset += length
        
    # <format> contains a dict
    elif isinstance(format, dict):
        mapping_value = {}
        # -> iterate through format
        for name in format:
            value = None
            value = GetField(dobj, name, format[name], raw=raw, addroffset=addroffset)
            if value is not None:
                mapping_value[name] = value
        # copy complete returned mapping
        valuemapping = copy.deepcopy(mapping_value)

    # a simple value
    elif isinstance(format, (str, bool, int, float, long)):
        if GetFieldLength(fielddef) != 0:
            valuemapping = struct.unpack_from(format, dobj, baseaddr+addroffset)[0]

            if not format[-1:].lower() in ['s','p']:
                valuemapping = bitsRead(valuemapping, bitshift, bits)

            # additional processing for strings
            if format[-1:].lower() in ['s','p']:
                # use left string until \0
                s = str(valuemapping).split('\0')[0]
                # remove character > 127
                valuemapping = unicode(s, errors='ignore')

            valuemapping = ReadWriteConverter(valuemapping, fielddef, read=True, raw=raw)

    else:
        exit(ExitCode.INTERNAL_ERROR, "Wrong mapping format definition: '{}'".format(format), typ=LogType.WARNING, doexit=not args.ignorewarning, line=inspect.getlineno(inspect.currentframe()))

    return valuemapping


def SetField(dobj, fieldname, fielddef, restore, addroffset=0, filename=""):
    """
    Get field value from definition

    @param dobj:
        decrypted binary config data
    @param fieldname:
        name of the field
    @param fielddef:
        see Settings desc above
    @param restore
        restore mapping with the new value(s)
    @param addroffset
        use offset for baseaddr (used for recursive calls)
    @param filename
        related filename (for messages only)

    @return:
        new decrypted binary config data
    """
    format, baseaddr, bits, bitshift, arraydef, group, writeconverter = GetFieldDef(fielddef, fields='format, baseaddr, bits, bitshift, arraydef, group, writeconverter')
    # cast unicode
    fieldname = str(fieldname)

    # filter groups
    if not IsFilterGroup(group):
        return dobj

    # do not write readonly values
    if writeconverter is False:
        if args.debug:
            print >> sys.stderr, "SetField(): Readonly '{}' using '{}'/{}{} @{} skipped".format(fieldname, format, arraydef, bits, hex(baseaddr+addroffset))
        return dobj

    # <arraydef> contains a list
    if isinstance(arraydef, list) and len(arraydef) > 0:
        offset = 0
        if len(restore) > arraydef[0]:
            exit(ExitCode.RESTORE_DATA_ERROR, "file '{sfile}', array '{sname}[{selem}]' exceeds max number of elements [{smax}]".format(sfile=filename, sname=fieldname, selem=len(restore), smax=arraydef[0]), typ=LogType.WARNING, doexit=not args.ignorewarning, line=inspect.getlineno(inspect.currentframe()))
        for i in range(0, arraydef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            length = GetFieldLength(subfielddef)
            if length != 0:
                if i >= len(restore): # restore data list may be shorter than definition
                    break
                subrestore = restore[i]
                dobj = SetField(dobj, fieldname, subfielddef, subrestore, addroffset=addroffset+offset, filename=filename)
            offset += length

    # <format> contains a dict
    elif isinstance(format, dict):
        for name in format:    # -> iterate through format
            if name in restore:
                dobj = SetField(dobj, name, format[name], restore[name], addroffset=addroffset, filename=filename)

    # a simple value
    elif isinstance(format, (str, bool, int, float, long)):
        valid = True
        err = ""
        errformat = ""

        _min, _max = GetFieldMinMax(fielddef)
        value = _value = None
        skip = False

        # simple char value
        if format[-1:] in ['c']:
            try:
                value = ReadWriteConverter(restore.encode(STR_ENCODING)[0], fielddef, read=False)
            except Exception, e:
                exit(e[0], e[1], typ=LogType.WARNING, line=inspect.getlineno(inspect.currentframe()))
                valid = False

        # bool
        elif format[-1:] in ['?']:
            try:
                value = ReadWriteConverter(bool(restore), fielddef, read=False)
            except Exception, e:
                exit(e[0], e[1], typ=LogType.WARNING, line=inspect.getlineno(inspect.currentframe()))
                valid = False

        # integer
        elif format[-1:] in ['b','B','h','H','i','I','l','L','q','Q','P']:
            value = ReadWriteConverter(restore, fielddef, read=False)
            if isinstance(value, (str, unicode)):
                value = int(value, 0)
            else:
                value = int(value)
            # bits
            if bits != 0:
                bitvalue = value
                value = struct.unpack_from(format, dobj, baseaddr+addroffset)[0]
                # validate restore value
                valid = ValidateValue(bitvalue, fielddef)
                if not valid:
                    err = "valid bit range exceeding"
                else:
                    mask = (1<<bits)-1
                    if bitvalue > mask:
                        _min = 0
                        _max = mask
                        _value = bitvalue
                        valid = False
                    else:
                        if bitshift >= 0:
                            bitvalue <<= bitshift
                            mask <<= bitshift
                        else:
                            bitvalue >>= abs(bitshift)
                            mask >>= abs(bitshift)
                        v=value
                        value &= (0xffffffff ^ mask)
                        value |= bitvalue

            # full size values
            else:
                # validate restore function
                valid = ValidateValue(value, fielddef)
                if not valid:
                    err = "valid range exceeding"
                _value = value

        # float
        elif format[-1:] in ['f','d']:
            try:
                value = ReadWriteConverter(float(restore), fielddef, read=False)
            except:
                valid = False

        # string
        elif format[-1:] in ['s','p']:
            value = ReadWriteConverter(restore.encode(STR_ENCODING), fielddef, read=False)
            err = "string length exceeding"
            if value is not None:
                _max -= 1
                valid = _min <= len(value) <= _max
            else:
                skip = True
                valid = True

        if value is None and not skip:
            # None is an invalid value
            valid = False

        if valid is None and not skip:
            # validate against object type size
            valid = _min <= value <= _max
            if not valid:
                err = "type range exceeding"
                errformat = " [{smin},{smax}]"

        if _value is None:
            # copy value before possible change below
            _value = value

        if isinstance(_value, (str, unicode)):
            _value = "'{}'".format(_value)

        if valid:
            if not skip:
                if args.debug:
                    if bits:
                        sbits=" {} bits shift {}".format(bits, bitshift)
                    else:
                        sbits = ""
                    print >> sys.stderr, "SetField(): Set '{}' using '{}'/{}{} @{} to {}".format(fieldname, format, arraydef, sbits, hex(baseaddr+addroffset), _value)
                if fieldname != 'cfg_crc':
                    prevvalue = struct.unpack_from(format, dobj, baseaddr+addroffset)[0]
                    struct.pack_into(format, dobj, baseaddr+addroffset, value)
                    curvalue = struct.unpack_from(format, dobj, baseaddr+addroffset)[0]
                    if prevvalue != curvalue and args.verbose:
                        message("Value for '{}' changed from {} to {}".format(fieldname, prevvalue, curvalue), typ=LogType.INFO)
        else:
            sformat = "file '{sfile}' - {{'{sname}': {svalue}}} ({serror})"+errformat
            exit(ExitCode.RESTORE_DATA_ERROR, sformat.format(sfile=filename, sname=fieldname, serror=err, svalue=_value, smin=_min, smax=_max), typ=LogType.WARNING, doexit=not args.ignorewarning)

    return dobj


def SetCmnd(cmnds, fieldname, fielddef, valuemapping, mappedvalue, addroffset=0, idx=None):
    """
    Get field value from definition

    @param cmnds:
        Tasmota command mapping: { 'group': ['cmnd' <,'cmnd'...>] ... }
    @param fieldname:
        name of the field
    @param fielddef:
        see Settings desc above
    @param valuemapping:
        data mapping
    @param mappedvalue
        mappedvalue mapping with the new value(s)
    @param addroffset
        use offset for baseaddr (used for recursive calls)
    @param idx
        optional array index

    @return:
        new Tasmota command mapping
    """
    format, baseaddr, bits, bitshift, arraydef, group, tasmotacmnd, writeconverter = GetFieldDef(fielddef, fields='format, baseaddr, bits, bitshift, arraydef, group, tasmotacmnd, writeconverter')

    # cast unicode
    fieldname = str(fieldname)

    # filter groups
    if not IsFilterGroup(group):
        return cmnds

    # <arraydef> contains a list
    if isinstance(arraydef, list) and len(arraydef) > 0:
        offset = 0
        if len(mappedvalue) > arraydef[0]:
            exit(ExitCode.RESTORE_DATA_ERROR, "array '{sname}[{selem}]' exceeds max number of elements [{smax}]".format(sname=fieldname, selem=len(mappedvalue), smax=arraydef[0]), typ=LogType.WARNING, doexit=not args.ignorewarning, line=inspect.getlineno(inspect.currentframe()))
        for i in range(0, arraydef[0]):
            subfielddef = GetSubfieldDef(fielddef)
            length = GetFieldLength(subfielddef)
            if length != 0:
                if i >= len(mappedvalue): # mappedvalue data list may be shorter than definition
                    break
                subrestore = mappedvalue[i]
                cmnds = SetCmnd(cmnds, fieldname, subfielddef, valuemapping, subrestore, addroffset=addroffset+offset, idx=i)
            offset += length

    # <format> contains a dict
    elif isinstance(format, dict):
        for name in format:    # -> iterate through format
            if name in mappedvalue:
                cmnds = SetCmnd(cmnds, name, format[name], valuemapping, mappedvalue[name], addroffset=addroffset, idx=idx)

    # a simple value
    elif isinstance(format, (str, bool, int, float, long)):
        cmnd = CmndConverter(valuemapping, mappedvalue, idx, fielddef)

        if group is not None and cmnd is not None:
            if group not in cmnds:
                cmnds[group] = []
            cmnds[group].append(cmnd)

    return cmnds


def Bin2Mapping(decode_cfg):
    """
    Decodes binary data stream into pyhton mappings dict

    @param decode_cfg:
        binary config data (decrypted)

    @return:
        valuemapping data as mapping dictionary
    """
    if isinstance(decode_cfg, bytearray):
        decode_cfg = str(decode_cfg)

    # get binary header and template to use
    version, size, setting = GetTemplateSetting(decode_cfg)

    # if we did not found a mathching setting
    if setting is None:
        exit(ExitCode.UNSUPPORTED_VERSION, "Tasmota configuration version 0x{:x} not supported".format(version),line=inspect.getlineno(inspect.currentframe()))

    if 'version' in setting:
        cfg_version = GetField(decode_cfg, 'version', setting['version'], raw=True)

    # check size if exists
    if 'cfg_size' in setting:
        cfg_size = GetField(decode_cfg, 'cfg_size', setting['cfg_size'], raw=True)
        # read size should be same as definied in setting
        if cfg_size > size:
            # may be processed
            exit(ExitCode.DATA_SIZE_MISMATCH, "Number of bytes read does ot match - read {}, expected {} byte".format(cfg_size, size), typ=LogType.ERROR,line=inspect.getlineno(inspect.currentframe()))
        elif cfg_size < size:
            # less number of bytes can not be processed
            exit(ExitCode.DATA_SIZE_MISMATCH, "Number of bytes read to small to process - read {}, expected {} byte".format(cfg_size, size), typ=LogType.ERROR,line=inspect.getlineno(inspect.currentframe()))

    # check crc if exists
    if 'cfg_crc' in setting:
        cfg_crc = GetField(decode_cfg, 'cfg_crc', setting['cfg_crc'], raw=True)
    else:
        cfg_crc = GetSettingsCrc(decode_cfg)
    if cfg_crc != GetSettingsCrc(decode_cfg):
        exit(ExitCode.DATA_CRC_ERROR, 'Data CRC error, read 0x{:x} should be 0x{:x}'.format(cfg_crc, GetSettingsCrc(decode_cfg)), typ=LogType.WARNING, doexit=not args.ignorewarning,line=inspect.getlineno(inspect.currentframe()))

    # get valuemapping
    valuemapping = GetField(decode_cfg, None, (setting,0,(None, None, (INTERNAL, None))))

    # add header info
    timestamp = datetime.now()
    valuemapping['header'] = {  'timestamp':timestamp.strftime("%Y-%m-%d %H:%M:%S"),
                                'format':   {
                                            'jsonindent':   args.jsonindent,
                                            'jsoncompact':  args.jsoncompact,
                                            'jsonsort':     args.jsonsort,
                                            'jsonhidepw':   args.jsonhidepw,
                                            },
                                'template': {
                                            'version':  hex(version),
                                            'crc':      hex(cfg_crc),
                                            },
                                'data':     {
                                            'crc':      hex(GetSettingsCrc(decode_cfg)),
                                            'size':     len(decode_cfg),
                                            },
                                'script':   {
                                            'name':     os.path.basename(__file__),
                                            'version':  VER,
                                            },
                                'os':       (platform.machine(), platform.system(), platform.release(), platform.version(), platform.platform()),
                                'python':   platform.python_version(),
                             }
    if 'cfg_crc' in setting:
        valuemapping['header']['template'].update({'size': cfg_size})
    if 'version' in setting:
        valuemapping['header']['data'].update({'version': hex(cfg_version)})

    return valuemapping


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
        changed binary config data (decrypted) or None on error
    """
    if isinstance(decode_cfg, str):
        decode_cfg = bytearray(decode_cfg)

    
    # get binary header data to use the correct version template from device
    version, size, setting = GetTemplateSetting(decode_cfg)

    # make empty binarray array
    _buffer = bytearray()
    # add data
    _buffer.extend(decode_cfg)

    if setting is not None:
        # iterate through restore data mapping
        for name in jsonconfig:
            # key must exist in both dict
            if name in setting:
                SetField(_buffer, name, setting[name], jsonconfig[name], addroffset=0, filename=filename)
            else:
                if name != 'header':
                    exit(ExitCode.RESTORE_DATA_ERROR, "Restore file '{}' contains obsolete name '{}', skipped".format(filename, name), typ=LogType.WARNING, doexit=not args.ignorewarning)

        if 'cfg_crc' in setting:
            crc = GetSettingsCrc(_buffer)
            struct.pack_into(setting['cfg_crc'][0], _buffer, setting['cfg_crc'][1], crc)
        return _buffer

    else:
        exit(ExitCode.UNSUPPORTED_VERSION,"File '{}', Tasmota configuration version 0x{:x} not supported".format(filename, version), typ=LogType.WARNING, doexit=not args.ignorewarning)

    return None


def Mapping2Cmnd(decode_cfg, valuemapping, filename=""):
    """
    Encodes mapping data into Tasmota command mapping

    @param decode_cfg:
        binary config data (decrypted)
    @param valuemapping:
        data mapping
    @param filename:
        name of the restore file (for error output only)
        
    @return:
        Tasmota command mapping {group: [cmnd <,cmnd <,...>>]}
    """
    if isinstance(decode_cfg, str):
        decode_cfg = bytearray(decode_cfg)
    
    # get binary header data to use the correct version template from device
    version, size, setting = GetTemplateSetting(decode_cfg)

    cmnds = {}

    if setting is not None:
        # iterate through restore data mapping
        for name in valuemapping:
            # key must exist in both dict
            if name in setting:
                cmnds = SetCmnd(cmnds, name, setting[name], valuemapping, valuemapping[name], addroffset=0)
            else:
                if name != 'header':
                    exit(ExitCode.RESTORE_DATA_ERROR, "Restore file '{}' contains obsolete name '{}', skipped".format(filename, name), typ=LogType.WARNING, doexit=not args.ignorewarning)

        return cmnds

    else:
        exit(ExitCode.UNSUPPORTED_VERSION,"File '{}', Tasmota configuration version 0x{:x} not supported".format(filename, version), typ=LogType.WARNING, doexit=not args.ignorewarning)

    return None


def Backup(backupfile, backupfileformat, encode_cfg, decode_cfg, configmapping):
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
    @param configmapping:
        config data mapppings
    """

    name, ext = os.path.splitext(backupfile)
    if ext.lower() == '.'+FileType.BIN.lower():
        backupfileformat = FileType.BIN
    elif ext.lower() == '.'+FileType.DMP.lower():
        backupfileformat = FileType.DMP
    elif ext.lower() == '.'+FileType.JSON.lower():
        backupfileformat = FileType.JSON

    fileformat = ""
    # Tasmota format
    if backupfileformat.lower() == FileType.DMP.lower():
        fileformat = "Tasmota"
        backup_filename = MakeFilename(backupfile, FileType.DMP, configmapping)
        if args.verbose:
            message("Writing backup file '{}' ({} format)".format(backup_filename, fileformat), typ=LogType.INFO)
        try:
            with open(backup_filename, "wb") as backupfp:
                backupfp.write(encode_cfg)
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    # binary format
    elif backupfileformat.lower() == FileType.BIN.lower():
        fileformat = "binary"
        backup_filename = MakeFilename(backupfile, FileType.BIN, configmapping)
        if args.verbose:
            message("Writing backup file '{}' ({} format)".format(backup_filename, fileformat), typ=LogType.INFO)
        try:
            with open(backup_filename, "wb") as backupfp:
                backupfp.write(struct.pack('<L',BINARYFILE_MAGIC))
                backupfp.write(decode_cfg)
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    # JSON format
    elif backupfileformat.lower() == FileType.JSON.lower():
        fileformat = "JSON"
        backup_filename = MakeFilename(backupfile, FileType.JSON, configmapping)
        if args.verbose:
            message("Writing backup file '{}' ({} format)".format(backup_filename, fileformat), typ=LogType.INFO)
        try:
            with open(backup_filename, "w") as backupfp:
                json.dump(configmapping, backupfp, sort_keys=args.jsonsort, indent=None if args.jsonindent < 0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )
        except Exception, e:
            exit(e[0], "'{}' {}".format(backup_filename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    if args.verbose:
        srctype = 'device'
        src = args.device
        if args.tasmotafile is not None:
            srctype = 'file'
            src = args.tasmotafile
        message("Backup successful from {} '{}' to file '{}' ({} format)".format(srctype, src, backup_filename, fileformat), typ=LogType.INFO)


def Restore(restorefile, backupfileformat, encode_cfg, decode_cfg, configmapping):
    """
    Restore from file

    @param encode_cfg:
        binary config data (encrypted)
    @param backupfileformat:
        Backup file format
    @param decode_cfg:
        binary config data (decrypted)
    @param configmapping:
        config data mapppings
    """

    new_encode_cfg = None

    restorefileformat = None
    if backupfileformat.lower() == 'bin':
        restorefileformat = FileType.BIN
    elif backupfileformat.lower() == 'dmp':
        restorefileformat = FileType.DMP
    elif backupfileformat.lower() == 'json':
        restorefileformat = FileType.JSON
    restorefilename = MakeFilename(restorefile, restorefileformat, configmapping)
    filetype = GetFileType(restorefilename)

    if filetype == FileType.DMP:
        if args.verbose:
            message("Reading restore file '{}' (Tasmota format)".format(restorefilename), typ=LogType.INFO)
        try:
            with open(restorefilename, "rb") as restorefp:
                new_encode_cfg = restorefp.read()
        except Exception, e:
            exit(e[0], "'{}' {}".format(restorefilename, e[1]),line=inspect.getlineno(inspect.currentframe()))

    elif filetype == FileType.BIN:
        if args.verbose:
            message("Reading restore file '{}' (binary format)".format(restorefilename), typ=LogType.INFO)
        try:
            with open(restorefilename, "rb") as restorefp:
                restorebin = restorefp.read()
        except Exception, e:
            exit(e[0], "'{}' {}".format(restorefilename, e[1]),line=inspect.getlineno(inspect.currentframe()))
        header = struct.unpack_from('<L', restorebin, 0)[0]
        if header == BINARYFILE_MAGIC:
            decode_cfg = restorebin[4:]                     # remove header from encrypted config file
            new_encode_cfg = DecryptEncrypt(decode_cfg)     # process binary to binary config

    elif filetype == FileType.JSON or filetype == FileType.INVALID_JSON:
        if args.verbose:
            message("Reading restore file '{}' (JSON format)".format(restorefilename), typ=LogType.INFO)
        try:
            with open(restorefilename, "r") as restorefp:
                jsonconfig = json.load(restorefp)
        except ValueError as e:
            exit(ExitCode.JSON_READ_ERROR, "File '{}' invalid JSON: {}".format(restorefilename, e), line=inspect.getlineno(inspect.currentframe()))
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
        if args.forcerestore or new_encode_cfg != encode_cfg:
            # write config direct to device via http
            if args.device is not None:
                if args.verbose:
                    message("Push new data to '{}' using restore file '{}'".format(args.device, restorefilename), typ=LogType.INFO)
                error_code, error_str = PushTasmotaConfig(new_encode_cfg, args.device, args.port, args.username, args.password)
                if error_code:
                    exit(ExitCode.UPLOAD_CONFIG_ERROR, "Config data upload failed - {}".format(error_str),line=inspect.getlineno(inspect.currentframe()))
                else:
                    if args.verbose:
                        message("Restore successful to device '{}' using restore file '{}'".format(args.device, restorefilename), typ=LogType.INFO)

            # write config from a file
            elif args.tasmotafile is not None:
                if args.verbose:
                    message("Write new data to file '{}' using restore file '{}'".format(args.tasmotafile, restorefilename), typ=LogType.INFO)
                try:
                    with open(args.tasmotafile, "wb") as outputfile:
                        outputfile.write(new_encode_cfg)
                except Exception, e:
                    exit(e[0], "'{}' {}".format(args.tasmotafile, e[1]),line=inspect.getlineno(inspect.currentframe()))
                if args.verbose:
                    message("Restore successful to file '{}' using restore file '{}'".format(args.tasmotafile, restorefilename), typ=LogType.INFO)

        else:
            global exitcode
            exitcode = ExitCode.RESTORE_SKIPPED
            if args.verbose:
                message("Configuration data leaving unchanged", typ=LogType.INFO)


def OutputTasmotaCmnds(tasmotacmnds):
    """
    Print Tasmota command mapping

    @param tasmotacmnds:
        Tasmota command mapping {group: [cmnd <,cmnd <,...>>]}
    """
    def OutputTasmotaSubCmnds(cmnds):
        if args.cmndsort:
            for cmnd in sorted(cmnds, key = lambda cmnd:[int(c) if c.isdigit() else c for c in re.split('(\d+)', cmnd)]):
                print "{}{}".format(" "*args.cmndindent, cmnd)
        else:
            for cmnd in cmnds:
                print "{}{}".format(" "*args.cmndindent, cmnd)

    if args.cmndgroup:
        for group in Groups:
            if group in tasmotacmnds:
                cmnds = tasmotacmnds[group]
                print
                print "# {}:".format(group)
                OutputTasmotaSubCmnds(cmnds)
                    
    else:
        cmnds = []
        for group in Groups:
            if group in tasmotacmnds:
                cmnds.extend(tasmotacmnds[group])
        OutputTasmotaSubCmnds(cmnds)

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

    backup = parser.add_argument_group('Backup/Restore', 'Backup & restore specification')
    backup.add_argument('-i', '--restore-file',
                            metavar='<filename>',
                            dest='restorefile',
                            default=DEFAULTS['backup']['backupfile'],
                            help="file to restore configuration from (default: {}). Replacements: @v=firmware version from config, @f=device friendly name from config, @h=device hostname from config, @H=device hostname from device (-d arg only)".format(DEFAULTS['backup']['restorefile']))
    backup.add_argument('-o', '--backup-file',
                            metavar='<filename>',
                            dest='backupfile',
                            default=DEFAULTS['backup']['backupfile'],
                            help="file to backup configuration to (default: {}). Replacements: @v=firmware version from config, @f=device friendly name from config, @h=device hostname from config, @H=device hostname from device (-d arg only)".format(DEFAULTS['backup']['backupfile']))
    backup_file_formats = ['json', 'bin', 'dmp']
    backup.add_argument('-t', '--backup-type',
                            metavar='|'.join(backup_file_formats),
                            dest='backupfileformat',
                            choices=backup_file_formats,
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
    backup.add_argument('-F', '--force-restore',
                            dest='forcerestore',
                            action='store_true',
                            default=DEFAULTS['backup']['forcerestore'],
                            help="force restore even configuration is identical{}".format(' (default)' if DEFAULTS['backup']['forcerestore'] else '') )

    jsonformat = parser.add_argument_group('JSON output', 'JSON format specification')
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

    jsonformat.add_argument('--json-hide-pw',
                            dest='jsonhidepw',
                            action='store_true',
                            default=DEFAULTS['jsonformat']['jsonhidepw'],
                            help="hide passwords{}".format(' (default)' if DEFAULTS['jsonformat']['jsonhidepw'] else '') )
    jsonformat.add_argument('--json-show-pw', '--json-unhide-pw',
                            dest='jsonhidepw',
                            action='store_false',
                            default=DEFAULTS['jsonformat']['jsonhidepw'],
                            help="unhide passwords{}".format(' (default)' if not DEFAULTS['jsonformat']['jsonhidepw'] else '') )

    cmndformat = parser.add_argument_group('Tasmota command output', 'Tasmota command output format specification')
    cmndformat.add_argument('--cmnd-indent',
                            metavar='<indent>',
                            dest='cmndindent',
                            type=int,
                            default=DEFAULTS['cmndformat']['cmndindent'],
                            help="Tasmota command grouping indent level (default: '{}'). 0 disables indent".format(DEFAULTS['cmndformat']['cmndindent']) )
    cmndformat.add_argument('--cmnd-groups',
                            dest='cmndgroup',
                            action='store_true',
                            default=DEFAULTS['cmndformat']['cmndgroup'],
                            help="group Tasmota commands{}".format(' (default)' if DEFAULTS['cmndformat']['cmndgroup'] else '') )
    cmndformat.add_argument('--cmnd-nogroups',
                            dest='cmndgroup',
                            action='store_false',
                            default=DEFAULTS['cmndformat']['cmndgroup'],
                            help="leave Tasmota commands ungrouped{}".format(' (default)' if not DEFAULTS['cmndformat']['cmndgroup'] else '') )
    cmndformat.add_argument('--cmnd-sort',
                            dest='cmndsort',
                            action='store_true',
                            default=DEFAULTS['cmndformat']['cmndsort'],
                            help="sort Tasmota commands{}".format(' (default)' if DEFAULTS['cmndformat']['cmndsort'] else '') )
    cmndformat.add_argument('--cmnd-unsort',
                            dest='cmndsort',
                            action='store_false',
                            default=DEFAULTS['cmndformat']['cmndsort'],
                            help="leave Tasmota commands unsorted{}".format(' (default)' if not DEFAULTS['cmndformat']['cmndsort'] else '') )

    common = parser.add_argument_group('Common', 'Optional arguments')
    common.add_argument('-c', '--config',
                            metavar='<filename>',
                            dest='configfile',
                            default=DEFAULTS['common']['configfile'],
                            is_config_file=True,
                            help="program config file - can be used to set default command args (default: {})".format(DEFAULTS['common']['configfile']) )

    common.add_argument('-S', '--output',
                            dest='output',
                            action='store_true',
                            default=DEFAULTS['common']['output'],
                            help="display output regardsless of backup/restore usage{}".format(" (default)" if DEFAULTS['common']['output'] else " (default do not output on backup or restore usage)") )
    output_formats = ['json', 'cmnd','command']
    common.add_argument('-T', '--output-format',
                            metavar='|'.join(output_formats),
                            dest='outputformat',
                            choices=output_formats,
                            default=DEFAULTS['common']['outputformat'],
                            help="display output format (default: '{}')".format(DEFAULTS['common']['outputformat']) )
    groups = GetGroupList(Settings[0][2])
    if '*' in groups:
        groups.remove('*')
    common.add_argument('-g', '--group',
                            dest='filter',
                            choices=groups,
                            nargs='+',
                            default=DEFAULTS['common']['filter'],
                            help="limit data processing to command groups (default {})".format("no filter" if DEFAULTS['common']['filter'] == None else DEFAULTS['common']['filter']) )
    common.add_argument('--ignore-warnings',
                            dest='ignorewarning',
                            action='store_true',
                            default=DEFAULTS['common']['ignorewarning'],
                            help="do not exit on warnings{}. Not recommended, used by your own responsibility!".format(' (default)' if DEFAULTS['common']['ignorewarning'] else '') )


    info = parser.add_argument_group('Info','Extra information')
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

    # check source args
    if args.device is not None and args.tasmotafile is not None:
        exit(ExitCode.ARGUMENT_ERROR, "Unable to select source, do not use -d and -f together",line=inspect.getlineno(inspect.currentframe()))

    # default no configuration available
    encode_cfg = None

    # pull config from Tasmota device
    if args.tasmotafile is not None:
        if args.verbose:
            message("Load data from file '{}'".format(args.tasmotafile), typ=LogType.INFO)
        encode_cfg = LoadTasmotaConfig(args.tasmotafile)

    # load config from Tasmota file
    if args.device is not None:
        if args.verbose:
            message("Load data from device '{}'".format(args.device), typ=LogType.INFO)
        encode_cfg = PullTasmotaConfig(args.device, args.port, username=args.username, password=args.password)

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
    configmapping = Bin2Mapping(decode_cfg)
    if args.verbose and 'version' in configmapping:
        message("{} '{}' is using version {}".format('File' if args.tasmotafile is not None else 'Device',
                                                     args.tasmotafile if args.tasmotafile is not None else args.device,
                                                     GetVersionStr(configmapping['version'])),
                                                     typ=LogType.INFO)

    # backup to file
    if args.backupfile is not None:
        Backup(args.backupfile, args.backupfileformat, encode_cfg, decode_cfg, configmapping)

    # restore from file
    if args.restorefile is not None:
        Restore(args.restorefile, args.backupfileformat, encode_cfg, decode_cfg, configmapping)

    # json screen output
    if (args.backupfile is None and args.restorefile is None) or args.output:
        if args.outputformat == 'json':
            print json.dumps(configmapping, sort_keys=args.jsonsort, indent=None if args.jsonindent<0 else args.jsonindent, separators=(',', ':') if args.jsoncompact else (', ', ': ') )

        if args.outputformat == 'cmnd' or args.outputformat == 'command':
            tasmotacmnds = Mapping2Cmnd(decode_cfg, configmapping)
            OutputTasmotaCmnds(tasmotacmnds)
            
    sys.exit(exitcode)
