Fonts, Images, Waveforms
========================

The ESP32 is, although fairly capable, still a microcontroller.
Thus, with memory and computational resources limited, it is useful to do as much of the processing
for displaying fonts and images on a computer.

Epdiy comes with scripts that convert fonts, images and waveforms to C headers,
that you can then simply `#include` in your project.

Generating Font Files
---------------------

Fonts can only be used by the driver in  a special header format 
(inspired by the Adafruit GFX library), which need to be generated from TTF fonts.
For this purpose, the :code:`scripts/fontconvert.py` utility is provided.
::

    fontconvert.py [-h] [--compress] [--additional-intervals ADDITIONAL_INTERVALS] name size fontstack [fontstack ...]

The following example generates a header file for Fira Code at size 10, where glyphs that are not found in Fira Code will be taken from Symbola:
::

    ./fontconvert.py FiraCode 10 /usr/share/fonts/TTF/FiraCode-Regular.ttf /usr/share/fonts/TTF/Symbola.ttf > ../examples/terminal/main/firacode.h

You can change which unicode character codes are to be exported by specifying additional
ranges of unicode code points with :code:`--additional-intervals`.
Intervals are written as :code:`min,max`. 
To add multiple intervals, you can specify the :code:`--additional-intervals` option multiple times.
::

    ./fontconvert.py ... --additional-intervals 0xE0A0,0xE0A2 --additional-intervals 0xE0B0,0xE0B3 ...

The above command would add two addtitional ranges.

You can enable compression with :code:`--compress`, which reduces the size of the generated font but comes at a performance cost.

If the generated font files with the default characters are too large for your application,
you can modify :code:`intervals` in :code:`fontconvert.py`.

Generating Images
-----------------

The process for converting images is very similar to converting fonts.
Run the :code:`scripts/imgconvert.py` script with an input image, an image name and an output image.
::

    imgconvert.py [-h] -i INPUTFILE -n NAME -o OUTPUTFILE [-maxw MAX_WIDTH] [-maxh MAX_HEIGHT]

The image is converted to grayscale scaled down to match fit into :code:`MAX_WIDTH` and :code:`MAX_HEIGHT` (1200x825 by default). 
For accurate grayscale it is advisable to color-grade and scale the image with a dedicated tool before converting it.

:code:`OUTPUTFILE` will be a C header with the following constants defined: 

- :code:`{NAME}_width` is the width of the image
- :code:`{NAME}_height` is the height of the image
- :code:`{NAME}_data` is the image data in 4 bit-per-pixel grayscale format.

Converting Waveforms
--------------------

In comercial applications, displays are driven with information in so-called `Waveform Files`.
These specify how which pulses to apply to the pixel to transition from one gray tone to another.
Unfortunately, they are display-specific and proprietary.
However, while they are not freely available, they can be obtained through a number of ways:

- Being a large customer of E-Ink. Unfortunately not doable for mere mortals.
- Finding them scattered around the internet. Examples include the `MobileRead forums <https://www.mobileread.com/>`_ or the `NXP Support forum <https://community.nxp.com/t5/i-MX-Processors/How-to-convert-wbf-waveform-file-to-wf-file/m-p/467926/highlight/true>`_.
- Extracting from e-Reader firmware.
- Extracting from a flash chip that comes with some displays. More on this can be found `here <https://hackaday.io/project/21168-fpga-eink-controller/log/57822-waveforms-binary-extract>`_.

Waveforms usually come with a :code:`*.wbf` file extension.


If you have a matching waveform file for your display, it can be converted to a waveform header that's usable by epdiy.
The advantage of using vendor waveforms include the availability of all implemented modes in the waveform file, support of a wide range of temperatures and more accurate grayscale-to-grayscale transitions.

As a first step, the waveform data is extracted from the original waveform file and stored in JSON format.
This can be done using a `modified version <https://github.com/vroland/inkwave>`_ of `inkwave <https://github.com/fread-ink/inkwave>`_ by Marc Juul.

Once a matching JSON file is obtained, the :code:`scripts/waveform_hdrgen.py` utility can be used to generate a waveform header, which can be included in your project.
::

    waveform_hdrgen.py [-h] [--list-modes] [--temperature-range TEMPERATURE_RANGE] [--export-modes EXPORT_MODES] name

With the :code:`--list-modes` option, a list of all included modes is printed.
:code:`name` specifies a name for the generated :code:`EpdWaveform` object.
Additionally, the temperature range and modes to export can be limited in order to reduce file size.
An example for the usage of this script can be found in the top-level :code:`Makefile` of the epdiy repository.
