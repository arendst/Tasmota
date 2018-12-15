# decode-config.py
_decode-config.py_ is able to backup and restore Sonoff-Tasmota configuration.

In contrast to the Tasmota build-in "Backup/Restore Configuration" function,
* _decode-config.py_ uses human readable and editable [JSON](http://www.json.org/)-format for backup/restore,
* _decode-config.py_ can restore previous backuped and changed [JSON](http://www.json.org/)-format files,
* _decode-config.py_ is able to create Tasmota commands based on given configuration

Comparing backup files created by *decode-config.py* and *.dmp files created by Tasmota "Backup/Restore Configuration":  

| &nbsp;                  | decode-config.py<br />*.json file | Sonoff-Tasmota<br />*.dmp file |
|-------------------------|:-------------------------------:|:-----------------------------------:|
| Encrypted               |                No               |                 Yes                 |
| Readable                |               Yes               |                  No                 |
| Simply editable         |               Yes               |                  No                 |
| Simply batch processing |               Yes               |                  No                 |

_decode-config.py_ is able to handle Tasmota configurations for release version starting from 5.10.0 up to now.

# Content
* [Prerequisite](decode-config.md#prerequisite)
* [File Types](decode-config.md#file-types)
  * [.dmp File Format](decode-config.md#-dmp-format)
  * [.json File Format](decode-config.md#-json-format)
  * [.bin File Format](decode-config.md#-bin-format)
    * [File extensions](decode-config.md#file-extensions)
* [Usage](decode-config.md#usage)
  * [Basics](decode-config.md#basics)
  * [Save backup file](decode-config.md#save-backup-file)
  * [Restore backup file](decode-config.md#restore-backup-file)
  * [Output to screen](decode-config.md#output-to-screen)
    * [JSON output](decode-config.md#json-output)
    * [Tasmota command output](decode-config.md#tasmota-command-output)
  * [Filter data](decode-config.md#filter-data)
  * [Configuration file](decode-config.md#configuration-file)
  * [More program arguments](decode-config.md#more-program-arguments)
  * [Examples](decode-config.md#examples)
    * [Config file](decode-config.md#config-file)
    * [Using Tasmota binary configuration files](decode-config.md#using-tasmota-binary-configuration-files)
    * [Use batch processing](decode-config.md#use-batch-processing)
  * [Notes](decode-config.md#notes)

## Prerequisite
* [Python](https://en.wikipedia.org/wiki/Python_(programming_language))  
  This program is written in [Python](https://en.wikipedia.org/wiki/Python_(programming_language)) so you need to install a python environment (for details see [Python Setup and Usage](https://docs.python.org/2.7/using/index.html))

* [Sonoff-Tasmota](https://github.com/arendst/Sonoff-Tasmota) [Firmware](https://github.com/arendst/Sonoff-Tasmota/releases) with Web-Server enabled:
  * To backup or restore configurations from/to a Sonoff-Tasmota device you need a firmare with enabled web-server in admin mode (command [WebServer 2](https://github.com/arendst/Sonoff-Tasmota/wiki/Commands#wifi)).
  * If using your own compiled firmware be aware to enable the web-server (`#define USE_WEBSERVER` and `#define WEB_SERVER 2`).

## File Types
_decode-config.py_ can handle the following backup file types:  
### .dmp Format
Configuration data as used by Tasmota "Backup/Restore Configuration" web interface.  
This format is binary and encrypted.
### .json Format
Configuration data in [JSON](http://www.json.org/)-format.  
This format is decrypted, human readable and editable and can also be used for the `--restore-file` command.  
This file will becreated by _decode-config.py_ using `--backup-file` with `--backup-type json` parameter (default).
### .bin Format
Configuration data in binary format.  
This format is binary decryptet, editable (e.g. using a hex editor) and can also be used for `--restore-file` command.  
It will be created by _decode-config.py_ using `--backup-file` with `--backup-type bin`.  
Note:  
This file is 4 byte longer than an original .dmp file due to an prefix header at the beginning. The file data starting at address position 4 are containing the same as the **struct SYSCFG** from Tasmota [settings.h](https://github.com/arendst/Sonoff-Tasmota/blob/master/sonoff/settings.h) in decrypted format.

#### File extensions
_decode-config.py_ uses auto extension as default for backup filenames; you don't need to append extensions to your backup file, it will be selected based on `--backup-type` argument.  
If you want using your own extension use the `--no-extension` argument.

## Usage
After download don't forget to set exec flag under linux with `chmod +x decode-config.py` or call the program using `python decode-config.py...`.

### Basics
At least pass a source where you want to read the configuration data from using `-f <filename>` or `-d <host>`:

The source can be either 
* a Tasmota device hostname or IP by passing it using the `-d <host>` arg
* or a previously stored Tasmota `*.dmp` configuration file by passing the filename using `-f <filename>` arg

Example:  

    decode-config.py -d sonoff-4281

will output a human readable configuration in [JSON](http://www.json.org/)-format:

    {
      "altitude": 112, 
      "baudrate": 115200, 
      "blinkcount": 10, 
      "blinktime": 10, 
    ...
      "ws_width": [
        1, 
        3, 
        5
      ]
    }


### Save backup file
To save the output as backup file `--backup-file <filename>`, you can use placeholder for Version, Friendlyname and Hostname:  

    decode-config.py -d sonoff-4281 --backup-file Config_@f_@v
    
If you have setup a WebPassword within Tasmota, use

    decode-config.py -d sonoff-4281 -p <yourpassword> --backup-file Config_@f_@v

will create a file like `Config_Sonoff_x.x.x.json`. Because it is in JSON format, you can read and edit the file with any raw text editor.

### Restore backup file
Reading back a saved (and possible changed) backup file use the `--restore-file <filename>` arg. This will read the (changed) configuration data from this file and send it back to the source device or filename.

To restore the previously save backup file `Config_Sonoff_6.2.1.json` to device `sonoff-4281` use:  

    decode-config.py -d sonoff-4281 --restore-file Config_Sonoff_6.2.1.json
    
with password set by WebPassword:

    decode-config.py -d sonoff-4281 -p <yourpassword> --restore-file Config_Sonoff_6.2.1.json

### Output to screen
Output to screen is default enabled when calling the program with a source arg but without a backup or restore arg.

`--output` arg will force screen output even if you use backup or restore arg.

#### JSON output
The default output format is JSON. You can force JSON output with `--output-format json` arg.

Example:

    decode-config.py -d sonoff-4281 -c my.conf -x Wifi --output-format json

    {
      ...
      "hostname": "%s-%04d", 
      "ip_address": [
        "0.0.0.0", 
        "192.168.12.1", 
        "255.255.255.0", 
        "192.168.12.1"
      ], 
      "ntp_server": [
        "ntp.localnet.home", 
        "ntp2.localnet.home", 
        "192.168.12.1"
      ], 
      "sta_active": 0, 
      "sta_config": 5, 
      "sta_pwd": [
        "myWlAnPaszxwo!z", 
        "myWlAnPaszxwo!z2"
      ], 
      "sta_ssid": [
        "wlan.1", 
        "my-wlan"
      ], 
      "web_password": "myPaszxwo!z", 
      "webserver": 2
      ...
    }

Note: JSON output always contains all configuration data like the backup file except you are using `--group` arg.


#### Tasmota command output
_decode-config.py_ is able to translate the configuration data to (most all) Tasmota commands. To output your configuration as Tasmota commands use `--output-format cmnd` or `--output-format command`.

Example:

    decode-config.py -d sonoff-4281 -c my.conf -g Wifi --output-format cmnd

    # Wifi:
      AP 0
      Hostname %s-%04d
      IPAddress1 0.0.0.0
      IPAddress2 192.168.12.1
      IPAddress3 255.255.255.0
      IPAddress4 192.168.12.1
      NtpServer1 ntp.localnet.home
      NtpServer2 ntp2.localnet.home
      NtpServer3 192.168.12.1
      Password1 myWlAnPaszxwo!z
      Password2 myWlAnPaszxwo!z2
      SSId1 wlan.1
      SSId2 wlan.1
      WebPassword myPaszxwo!z
      WebServer 2
      WifiConfig 5

Note: A few very specific module commands like MPC230xx, KNX and some Display commands are not supported. These are still available by JSON output.

### Filter data
The huge number of Tasmota configuration data can be overstrained and confusing, so the most of the configuration data are grouped into categories. 

With _decode-config.py_ the following categories are available:   `Display`, `Domoticz`, `Internal`, `KNX`, `Led`, `Logging`, `MCP230xx`, `MQTT`, `Main`, `Management`, `Pow`, `Sensor`, `Serial`, `SetOption`, `SonoffRF`, `System`, `Timers`, `Wifi`

These are similary to the categories on [https://github.com/arendst/Sonoff-Tasmota/wiki/Commands](Tasmota Command Wiki).

To filter outputs to a subset of groups use the `-g` or `--group` arg concatenating the grooup you want, e. g.

    decode-config.py -d sonoff-4281 -c my.conf --output-format cmnd --group Main MQTT Management Wifi


### Configuration file
Each argument that start with `--` (eg. `--file`) can also be set in a config file (specified via -c). Config file syntax allows: key=value, flag=true, stuff=[a,b,c] (for details, see syntax at [https://pypi.org/project/ConfigArgParse](https://pypi.org/project/ConfigArgParse/)).

If an argument is specified in more than one place, then commandline values override config file values which override defaults. This is usefull if you always use the same argument or a basic set of arguments.

The http authentication credentials `--username` and `--password` is predestinated to store it in a file instead using it on your command line as argument:

e.g. my.conf:

    [source]
    username = admin
    password = myPaszxwo!z

To make a backup file from example above you can now pass the config file instead using the password on command line:

    decode-config.py -d sonoff-4281 -c my.conf --backup-file Config_@f_@v



### More program arguments
For better reading each short written arg (minus sign `-`) has a corresponding long version (two minus signs `--`), eg. `--device` for `-d` or `--file` for `-f` (note: not even all `--` arg has a corresponding `-` one).

A short list of possible program args is displayed using `-h` or `--help`.

For advanced help use `-H` or `--full-help`:

    usage: decode-config.py [-f <filename>] [-d <host>] [-P <port>]
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

### Program parameter notes

_decode-config.py_


### Examples
The most of the examples are for linux command line. Under Windows call the program using `python decode-config.py ...`.

#### Config file
Note: The example contains .ini style sections `[...]`. Sections are always treated as comment and serves as clarity only.
For further details of config file syntax see [https://pypi.org/project/ConfigArgParse](https://pypi.org/project/ConfigArgParse/).

*my.conf*

    [Source]
    username = admin
    password = myPaszxwo!z

    [JSON]
    json-indent 2

#### Using Tasmota binary configuration files

1. Restore a Tasmota configuration file

    `decode-config.py -c my.conf -d sonoff --restore-file Config_Sonoff_6.2.1.dmp`

2. Backup device using Tasmota configuration compatible format

   a) use file extension to choice the file format

    `decode-config.py -c my.conf -d sonoff --backup-file Config_@f_@v.dmp`

   b) use args to choice the file format

    `decode-config.py -c my.conf -d sonoff --backup-type dmp --backup-file Config_@f_@v`

#### Use batch processing

    for device in sonoff1 sonoff2 sonoff3; do ./decode-config.py -c my.conf -d $device -o Config_@f_@v

or under windows

    for device in (sonoff1 sonoff2 sonoff3) do python decode-config.py -c my.conf -d %device -o Config_@f_@v

will produce JSON configuration files for host sonoff1, sonoff2 and sonoff3 using friendly name and Tasmota firmware version for backup filenames.

## Notes
Some general notes:
* Filename replacement macros **@h** and **@H**:
  * **@h**  
The **@h** replacement macro uses the hostname configured with the Tasomta Wifi `Hostname <host>` command (defaults to `%s-%04d`). It will not use the network hostname of your device because this is not available when working with files only (e.g. `--file <filename>` as source).  
To prevent having a useless % in your filename, **@h** will not replaced by configuration data hostname if this contains '%' characters.
  * **@H**  
If you want to use the network hostname within your filename, use the **@H** replacement macro instead - but be aware this will only replaced if you are using a network device as source (`-d`, `--device`, `--host`); it will not work when using a file as source (`-f`, `--file`)
