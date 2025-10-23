## Scripts in this folder are for adding addtional capabilities to epdiy.



## imgconvert.py

#### usage:

python3 imgconvert.py [-h] -i INPUTFILE -n NAME -o OUTPUTFILE [-maxw MAX_WIDTH]
                     [-maxh MAX_HEIGHT]

**optional arguments:**

  * **-h, --help**        show this help message and exit

  * **-i INPUTFILE**

  * **-n NAME**

  * **-o OUTPUTFILE**

  * **-maxw MAX_WIDTH**

  * **-maxh MAX_HEIGHT**


==========================================================

## fontconvert.py

#### usage:

python3 fontconvert.py [-h] [--compress] [--additional-intervals ADDITIONAL_INTERVALS]
                      [--string STRING]
                      name size fontstack [fontstack ...]

Generate a header file from a font to be used with epdiy.

**positional arguments:**

  * **name**                  name of the font to be used in epdiy.
  * **size**                  font size to use.
  * **fontstack**             list of font files, ordered by descending priority. This is not actually implemented as yet. Please just use one file for now.

**optional arguments:**

  * **-h**, --help            show this help message and exit

  * **--compress**            compress glyph bitmaps.

  * **--additional-intervals** ADDITIONAL_INTERVALS

                        Additional code point intervals to export as min,max. This argument
                        can be repeated.

  * **--string STRING**       A quoted string of all required characters. The intervals are will be made from these characters if they exist in the ttf file. Missing characters will warn about their abscence. 



####example:
 1. Download a ttf from where you like to a directory. As in: "~/Downloads/any_old_ttf.ttf"
in the download directory 

 2. Run 

   `python3 fontconvert.py my_font 30 ~/Downloads/any_old_ttf.ttf --string '/0123456789:;@ABCDEFGH[\]^_`abcdefgh\{|}~¡¢£¤¥¦§¨©ª' > fonts.h`

  * you will need to use special escapes for characters like ' or " This is system dependant though.

 3. copy fonts.h into your app folder or where ever your app can find it.
 4.  include it into your project with 
`#include fonts.h`
Then use it just like any other font file in epdiy.

**To run this script the freetype module needs to be installed. This can be done with `pip install freetype-py` You will be warned if it is not accessible by the script.**

==========================================================

##waveform_hdrgen.py

####usage:

waveform_hdrgen.py [-h] [--list-modes] [--temperature-range TEMPERATURE_RANGE]
                          [--export-modes EXPORT_MODES]
                          name

**positional arguments:**
  name                  name of the waveform object.

**optional arguments:**

  * **-h, --help**            show this help message and exit

  * **--list-modes**          list the available modes for tis file.

  * **--temperature-range TEMPERATURE_RANGE**
                        only export waveforms in the temperature range of min,max °C.

  * **--export-modes EXPORT_MODES**
                        comma-separated list of waveform mode IDs to export.

