## Sonoff-Tasmota template information
Sonoff-Tasmota uses Device or Module information to control peripherals connected to GPIOs. This information is stored in the ``sonoff_template.h`` file as a device specific template. The template contains information about what GPIO should be connected to what peripheral and what GPIO may be configured online using the ``GPIO`` command or GUI Configure Module menu. In addition a device may need specific coding to process the data from these peripherals. The module number as provided by the ``Modules`` command is used to select this coding.

Starting with version 6.4.1.16 Sonoff-Tasmota Modules can be extended by users online using a template. To provide easy processing by Sonoff-Tasmota a user template is written as JSON text and could look like this:

{"NAME":"UserModule1","GPIO":[17,148,29,149,7,255,255,255,138,255,139,255,255],"FLAG":0,"BASE":18}

The four properties with UPPERCASE property names have the following functionality:

Property name | Property value description
--------------|-------------------------------------------------------------------------------------------------------------------
NAME          | Up to 14 characters for the Module name
GPIO          | Up to 13 decimal numbers from 0 to 255 representing GPIO0 to GPIO5, GPIO09, GPIO10 and GPIO12 to GPIO16
FLAG          | 8 bit mask flag register
BASE          | Module number of a hard-coded device to be used when device specific functionality is needed

The above example, based on the Generic Module does not allow ADC0 input.

## GPIO functionality
The GPIO functionality numbers are the same is shown by command ``GPIOs``. In addition code 255 is added to select a GPIO as user configurable via the GUI Configure Module menu.

## FLAG functionality
The FLAG value is an 8-bit mask where each bit controls a features. Add FLAG values to set multiple bits.

FLAG | Mask     | Feature description
-----|----------|------------------------------
   1 | xxxxxxx1 | Allowing to use Analog0 (ADC0) as input if define USE_ADC_VCC in ``my_user_config.h`` is disabled
   2 | xxxxxx1x | Enable GUI pull-up control message
   4 | xxxxx1xx | Not used
   8 | xxxx1xxx | Not used
  16 | xxx1xxxx | Not used
  32 | xx1xxxxx | Not used
  64 | x1xxxxxx | Not used
 128 | 1xxxxxxx | Not used

## BASE functionality
The following table lists hard-coded device specific functionality. Notice that not all device modules need special handling.

BASE | Module         | Description
-----|----------------|----------------------------------------------
   4 | Sonoff Dual    | Process relay and button via hardware serial interface using GPIO01 and GPIO03. Change baudrate to 19200 bps. Process buttons as single press only
   9 | Sonoff Touch   | Invert ledstate 1 functionality
  10 | Sonoff LED     | Set light type to 2 PWM channels disregarding SetOption15. Fix device specific LED instabilities by disabling GPIO04, GPIO5 and GPIO14
  12 | 4 Channel      | See 4
  13 | Motor C/AC     | Force all relays ON at Power On and disable command ``PowerOnState``
  15 | EXS Relay(s)   | Enable pulse latching using even/odd numbered relay pairs
  18 | Generic        | Show Wemos specific pin information in GUI
  19 | H801           | Change hardware UART Tx from GPIO01 to GPIO02
  20 | Sonoff SC      | Enable and Process data via hardware serial interface using GPIO01 and GPIO03. Change baudrate to 19200 bps
  21 | Sonoff BN-SZ   | Set light type to 1 PWM channel disregarding SetOption15
  22 | Sonoff 4CH Pro | Button handling disregarding SetOption13 only allowing single press to enable RF learning while holding the button
  24 | Sonoff Bridge  | Enable and Process data via hardware serial interface using GPIO01 and GPIO03. Change baudrate to 19200 bps. Process 16 buttons in web GUI. Enable EFM8BB1 firmware upload
  25 | Sonoff B1      | Set light type to RGBWC using MY92x1
  26 | AiLight        | Set light type to RGBW using MY92x1
  27 | Sonoff T1 1CH  | See 9
  28 | Sonoff T1 2CH  | See 9
  29 | Sonoff T1 3CH  | See 9
  38 | Sonoff Dual R2 | Process buttons as single press only
  43 | Sonoff iFan02  | Enable command ``Fanspeed``. Disable Interlock and PulseTime. Tune status information, MQTT data and GUI. Sync with microcontroller. Process Domoticz Fan state
  47 | Xiaomi Philips | Process Color Temperature using PWM2 and Intensity using PWM1
  53 | Tuya Dimmer    | Enable and Process data via software or hardware serial interface using GPIO 148 and 149 or forced GPIO01 and GPIO03. Change baudrate to 9600 bps. Process all Buttons
  55 | ARMTR Dimmer   | Enable and Process data via software or hardware serial interface using GPIO 148 and 149. Change baudrate to 115200 bps.
  57 | PS-16-DZ       | Enable and Process data via software or hardware serial interface using GPIO 148 and 149. Change baudrate to 19200 bps.
  61 | YTF IR Bridge  | Disable serial interface to stop loopback
  65 | Mi Desk Lamp   | Process rotary and Button1 data specific to this device

## Usage
A user provided template can be stored in Sonoff-Tasmota using the ``Template`` command. It has the following options.

Command  | Payload  | Description
---------|----------|---------------------------------------
Template |          | Show current user template
Template | 0        | Copy active module template to user template
Template | 1 .. 69  | Copy hard-coded module template to user template

The following command will store a complete template based on the Generic module
``Template {"NAME":"UserModule1","GPIO":[17,148,29,149,7,255,255,255,138,255,139,255,255],"FLAG":0,"BASE":18}``

The following command will update the name of a stored template
``Template {"NAME":"UserModule2"}``

The following command will update the flag of a stored template
``Template {"FLAG":1}``

The following command will update the base of a stored template to Generic
``Template {"BASE":0}``