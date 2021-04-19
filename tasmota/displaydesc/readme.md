Display Descriptor files for use with universal display driver.
4 (3) options to select a display driver

1. file system driven if UFILESYSTEM is in place (preferred option for normal use)
  to select a display rename the file to "dispdesc.txt" and put into flash file system.
2. scripter driven as a special section >d in scripter.
  copy the file to a script section >d and place a ->displayreinit cmd into the >B section
  (preferred for developing or modifying display driver)
3. rule buffer 3 driven
  copy descriptor to rule buffer number 3 but do not enable rule 3  
  (descriptor may not contain ANY spaces in this mode)
(4.) compile the descriptor into driver.
  convert the file to a const char array and place into source xdsp_universal.ino
  and replace the example array there + #define DSP_ROM_DESC

for further info about display descriptors read the tasmota docs display part.

the current files define the standard resolutions. if you change the resolution settings
sometimes you also have to change some register values in the init or address map section
