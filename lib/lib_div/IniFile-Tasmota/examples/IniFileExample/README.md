# IniFile example

## Instructions for use

  * Copy the `net.ini` file to the root directory of your (micro)SD card.
  * Modify the `IniFileExample.ino` file so that `SD_SELECT` defines
  the correct pin number.
  * Compile and upload the sketch. 


## Expected output

It may take a few seconds from the sketch starting before anything is
printed to the serial port, be patient. If the sketch runs correctly
the output should appear as below:


    Ini file exists
    section 'network' has an entry 'mac' with value 01:23:45:67:89:AB
    Could not read 'nosuchkey' from section 'network', error was key not found
    Could not read 'nosuchkey' from section 'nosuchsection', error was section not found
    The value of 'allow put' in section '/upload' is TRUE


If the SD card is missing or cannot be read the sketch will print:

    SD.begin() failed


