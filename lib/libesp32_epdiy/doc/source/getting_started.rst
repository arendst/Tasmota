.. _getting_started:

Getting Started
===============


Getting your Board
------------------

At the current point in time, there is no official way to buy an epdiy board.
Fortunately, it is quite easy to order your own. There are many PCB prototype services
that will manufacture boards for a low price.

To use one of those services, upload the "Gerber files", usually provided as a zip file,
to the service.
Please consult the README of the epdiy repository for a link to the gerber files for the current revision.
Some services also offer automated assembly for all or a subset of components.
Usually, you will have to upload a bill of materials (BOM) and a placement file.
Both are provided in the `hardware/epaper-breakout` directory.

Choosing and Finding Parts
~~~~~~~~~~~~~~~~~~~~~~~~~~

The parts needed to assemble the epdiy board are listed in the `BOM.csv` file.
Commodity parts like resistors, capacitors, coils and diodes are interchangable, as long as they
fit the footprint. 
When in doubt, use the parts listed in the BOM file.

However, some parts are not as common, especially the connectors. 
Most of them can be found on sites like eBay or AliExpress. 

Tips:
    - Use the WROVER-B module instead of other WROVER variants.
      This module exhibits a low deep sleep current and is proven to work.
    - The LT1945 voltage booster seems to be out of stock with some distributors,
      but they are available cheaply from AliExpress.

Calibrate VCOM
--------------

EPaper displays use electrical fields to drive colored particles.
One of the required voltages, VCOM (Common Voltage) is display-dependent
and must be calibrated for each display individually.

Fortunately, the VCOM voltage is usually printed on the display, similar to this:

.. image:: img/vcom.jpg

The VCOM value is usually between -1V and -3V. 

To tune the controller output voltage, use the trimmer marked :code:`RV1`.
You can measure the VCOM on the VCOM test pad (if your board has one) or directly
at the amplifier:

.. image:: img/vcom_tp.jpg
.. image:: img/vcom_opamp.jpg

.. note::
    
    Although most boards do not have it yet, image quality for partial updates can be improved by adding a (at least) 4.7uF capacitor between VCOM and GND. 
    When adding this capacitor, take care with the polarity as VCOM is negative!

Flashing the demo
-----------------

First, connect you board with the computer. In the output of :code:`lsusb` you should find something like:
::

    Bus 001 Device 048: ID 1a86:7523 QinHeng Electronics HL-340 USB-Serial adapter

This means the serial adapter is working and there a serial like :code:`/dev/ttyUSB0` should appear.

Next, make sure you have the `Espressif IoT Development Framework <https://github.com/espressif/esp-idf>`_ installed. 
The current stable (and tested) version is 4.0.
For instructions on how to get started with the IDF, please refer to their `documentation <https://docs.espressif.com/projects/esp-idf/en/stable/get-started/>`_.

Then, clone the :code:`epdiy` git repository (and its submodules):
::

    git clone --recursive https://github.com/vroland/epdiy

Now, (after activating the IDF environment) you should be able to build the demo:
::

    cd examples/demo/
    idf.py build

Hold down the :code:`BOOT` button on your board, while quickly pressing the :code:`RESET` button. 
The ESP module is now in boot mode. 
Upload the demo program to the board with
::

    idf.py build && idf.py flash -b 921600 && idf.py monitor

Pressing :code:`RESET` a second time should start the demo program, which will
output some information on the serial monitor.

With the **board power turned off**, connect your display. 
Power on the board.
You should now see the demo output on your display.

Use with esp-idf
----------------

The neccessary functionality for driving an EPD display is encapsulated in the :code:`components/epd_driver` IDF component.
To use it in you own project, simply copy the :code:`epd_driver` folder to your project-local :code:`components` directory.
The component sould be automatically detected by the framework, you can now use
::

    #include "epd_driver.h"

to use the EPD driver's :ref:`pub_api`.

Selecting a Display Type
~~~~~~~~~~~~~~~~~~~~~~~~

To select the display type you want to use for the project (see :ref:`display_types`), run
::

    idf.py menuconfig

And navigate to :code:`Component config -> E-Paper driver -> Display Type`, select the appropriate option and save the configuration. You can use the defines
::

    EPD_WIDTH
    EPD_HEIGHT
    CONFIG_EPD_DISPLAY_TYPE_ED097OC4
    CONFIG_EPD_DISPLAY_TYPE_ED060SC4
    CONFIG_EPD_DISPLAY_TYPE_...

to make your code portable.

Use with Arduino
----------------

Epdiy can be used as an Arduino library. Additionally, epdiy comes with board definitions for its supported boards, which must be installed separately. 
To install epdiy to work with the Arduino IDE (>= 1.8), follow the steps below:

1. Download / Clone the epdiy repository into your arduino libraries folder.
2. Install the `arduino-esp32 core <https://github.com/espressif/arduino-esp32#installation-instructions>`_. 
3. Due to a limitation in the arduino-esp32 core, the epdiy board definitions must be installed manually. To do that, copy the :code:`boards.local.txt` file that comes with epdiy to the directory you installed the esp32 core in.

    * On Linux, this will be somewhere in :code:`~/Arduino/hardware/esp32/esp32`.
    * On Windows, this will look something like :code:`C:/Users/<username>/Documents/Arduino/hardware/espressif/esp32` or a similar path in AppData.

   You can also search the Arduino logs to find the arduino-esp32 install directory.

   If you have the correct directory, there should be a :code:`boards.txt` file in it. Copy the :code:`boards.local.txt` in this directory.
4. Re-start the Arduino IDE. The Epdiy boards should now show up among the other ESP32 boards.
5. Select your epdiy-compatible board. You can now select your display from the board menu.
6. Try out an example sketch! Some of them are not very Arduino-Style yet, but they should help getting you started. 

Alternatively, it is possible to use the `Arduino APIs as an IDF component <https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md>`_,
which allows you to use the Arduino ecosystem (Except for a different build process).

