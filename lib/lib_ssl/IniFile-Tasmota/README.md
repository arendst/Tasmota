# IniFile

IniFile is an Arduino library for reading ini files. The format is
similar to that seen in Microsoft `.ini` files but the implementation
is completely independent. IniFile is designed to use minimal memory
requirements, and the only buffer used is one supplied by the user,
thus the user remains in charge of memory usage.

The ini file is separated into sections, where the section names are
written inside square brackets. If you don't wish to use sections then
pass a `NULL` pointer for the section name. Under each section are a
set of key-value pairs, separated by an equals sign (`=`). Spaces
around keys and values are ignored, but extra spaces inside the key
are significant. Whitespace inside the value string is preserved; if
leading or trailing whitespace is important you must quote the value
string inside the ini file, and you must strip out the quotes
yourself. If multiple entries for the same key exist inside the
selected section (if using) then only the first value is returned. If
a section is defined more than once only the first is used. The .ini
file can contain comments, which begin with a semicolon (`;`) or hash
(`#`). The user-supplied buffer must be large enough to accomodate the
longest line in the file.

## Example file format

    ; Semi-colon comment
    [network]
    mac = 01:23:45:67:89:AB
    
    # hash comment, leading spaces below
     gateway = 192.168.1.1
    
    # extraneous spaces before and after key and value
     ip   =   192.168.1.2             
    
    hosts allow = example.com
    
    # A similarly-named section
    [network2]
    mac = ee:ee:ee:ee:ee:ee
    subnet mask=255.255.255.0
    
    ; Extra whitespace around the key and value is permitted
    ; (and ignored)
      hosts allow =    sloppy.example.com  
    
    [misc]
    
    string = 123456789012345678901234567890123456789001234567890
    string2 = a string with spaces in it
    
    ; This section is a repeat of en existing section and will be ignored.
    [network]
    mac = 01:23:45:67:89:ab
    ip = 192.168.1.2
    gateway = 192.168.1.1
    

## Write support

Write support is a feature that has been requested on several
occasions but as I am no longer using the IniFile library I will not
add this feature. For anyone planning to add such support the
information below may be useful.

One goal of the `IniFile` implementation was to limit the amount of
memory required. For use in embedded systems `malloc` and `new` are
deliberately not used. Another goal was that tasks which take a longer
duration were broken down into smaller chunks of work, eg
`IniFile::getValue(const char* section, const char* key, char* buffer,
int len, IniFileState &state)`. This was because I wanted my `WwwServer`
library, which uses `IniFile`, to avoid interfering with time-critical
code.

I don't think that write support can meet the time-critical goal but
that doesn't prevent its inclusion. I think the way I would choose to
implement write support is to use `IniFile::findKey()` to find where the
desired key is located in the file. I'd then copy everything up to
that point to a temporary file, insert a line for the value and new
key, skip the current line in the existing file (using
`IniFile::readline()`) and then write out the reminder of the existing
file into the temporary file. I'd like to move or rename the temporary
file over the existing file but the Arduino SD library doesn't provide
this functionality; I'd probably just copy the temporary file over the
old one and then delete the temporary one.

The code has been written under a standard Linux environment, using a
compatibility header file to mimic the SD library. This was far more
convenient than doing everything on the Arduino and enabled me to use
`gdb` to debug the code and core dumps. You'll find `arduino_compat.h`
inside the test directory, along with a `Makefile` which can be used for
regression testing. Any proposed changes must pass the regression
tests.

## Contributors

  * [Steve Marple](https://github.com/stevemarple)
  * [per1234](https://github.com/per1234)
  * [OscarVanL](https://github.com/OscarVanL)
  * [MikuX2](https://github.com/toybox01)
  * [kaixxx](https://github.com/kaixxx)
