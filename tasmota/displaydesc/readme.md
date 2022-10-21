Display Descriptor files for use with universal display driver. 

Options to select a display driver:

1. file system driven if UFILESYSTEM is in place (preferred option for normal use)
  to select a display rename the file to "display.ini" and put into flash file system.
2. scripter driven as a special section >d in scripter.
  copy the file to a script section >d and place a ->displayreinit cmd into the >B section
  (preferred for developing or modifying display driver)
3. rule buffer 3 driven
  copy descriptor to rule buffer number 3 but do not enable rule 3
  (descriptor may not contain ANY spaces in this mode)
4. compile the descriptor into driver.
  
  convert the file to a string and place it into your `user_config_override.h` file

  Example:

```
#define DSP_ROM_DESC \
":H,SH1106,128,64,1,I2C,3c,*,*,*\n" \
":S,0,1,1,0,40,20\n" \
":I\n" \
"AE\n" \
"D5,80\n" \
"A8,3f\n" \
"D3,00\n" \
"40\n" \
"8D,14\n" \
"20,00\n" \
"A1\n" \
"C8\n" \
"DA,12\n" \
"81,CF\n" \
"D9F1\n" \
"DB,40\n" \
"A4\n" \
"A6\n" \
"AF\n" \
":o,AE\n" \
":O,AF\n" \
":A,00,10,40,00,02\n" \
":i,A6,A7\n" \
"#\n"
```


for further info about display descriptors read the tasmota docs display part.

the current files define the standard resolutions. if you change the resolution settings
sometimes you also have to change some register values in the init or address map section
