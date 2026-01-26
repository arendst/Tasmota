# IniFile browsing example

Shows how to browse through an ini file with an unknown number of sections
which all contain the same elements. This can be useful to store different 
profiles which are selectable on your device.


## Instructions for use

  * Copy the `lunch.ini` file to the root directory of your (micro)SD card.
  * Modify the `IniBrowseExample.ino` file so that `SD_SELECT` defines
  the correct pin number.
  * Compile and upload the sketch. 


## Expected output

It may take a few seconds from the sketch starting before anything is
printed to the serial port, be patient. If the sketch runs correctly
the output should appear as below:

    Ini file exists

    > Karen eats burger, drinks beer and has chocolate for dessert.
    > Peter eats falafel, drinks tea without milk and has vegan icecream for dessert.
    > Noel eats sushi, drinks water and has no dessert.
    > Jessica eats sandwich, drinks nothing and has muffin for dessert.

    end of file

If the SD card is missing or cannot be read the sketch will print:

    SD.begin() failed


