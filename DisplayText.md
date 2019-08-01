**DisplayText commands:**

(You must set DisplayMode to zero to use DisplayText, or disable other
modes before compilation with \#undef USE\_DISPLAY\_MODES1TO5), then
select your display with DisplayModel (see list below) dont forget to
select the gpio pins you need.  

DisplayText not only draws Text but via escapes also line primitives  

commands are embedded within brackets \[\]  

p stands for parameter and may be a number from 1 to n digits  

on monochrome graphics displays things are drawn into a local frame  
buffer and send to the display either via the „d" command or
automatically at the end of the command  

**command list:**  

*positioning:*  

>l*p* = sets a character line to print at  
c*p* = sets a character column to print at  
x*p* = sets the x position for consecutive prints  
y*p* = sets the y position for consecutive prints  

*line primitives:*   
>h*p* = draws a horizontal line with lenght p (x is advanced)  
v*p* = draws a vertical line with lenght p (y is advanced)  
*Lp:p* = draws a line to p(x) and p(y) (x,y are advanced)  
k*p* = draws a circle with radius p  
K*p* = draws a filled circle with radius p  
r*w:h* = draws a rectangle with width and height  
R*w:h* = draws a filled rectangle with width and height  
u*w:h:r* = draws a rounded rectangle width, height and radius  
U*w:h:r* = draws a filled rounded rectangle width, height and radius  

*miscelanious:*

>z = clears the display  
i = (re)init the display (in e-paper mode with partial update)  
I = (re)init the display (in e-paper mode with full update)  
d = draw actually updates the display  
D*p* = switch display auto updates on/off (1), when off must be updated via
cmd d  (this is only used on Displays with a RAM framebuffer => black and white displays)  
bit 1 (2) sets the Text print to transparent mode else opaque mode  
o = switch display off  
O = switch display on  
ax = x 0..3 set rotation angle
t = display Tasmota time in HH:MM  
ts = display Tasmota time in HH:MM:SS  
T = display Tasmota date in DD.MM.YY  
p*p =* pad text with spaces, positiv values align left, negativ values
align right  
s*p* = set text size (scaling factor 1\...4) (only for classic GFX + RA8876 internal font)  
f*p* = set font (1=12, 2=24,(opt 3=8)) if font==0 the classic GFX is used if font==7 the RA8876 internal font is used  
C*p* = set foreground color (0,1) for black or white or 16 bit color
code  
B*p* = set background color (0,1) for black or white or 16 bit color
code  
Ci*p* or Bi*p* = set colors from index currently 0-18 see table below  
w*p* = draws an analog watch with radius p  (#define USE_AWATCH)
Pfilename: = display an rgb 16bit color image when sdcard file system is present

*Buttons:*

>Draw GFX Buttons (up to 16)
buttons switch real Tasmota devices like relais
when button number + 256 a virtual touch toggle button is created (MQTT => TBT)
when button number + 512 a virtual touch push button is created (MQTT => PBT)
(#define USE_TOUCH_BUTTONS)  

>b \...  n=button number 0-15  
xp = x position  
yp = y position  
xs = x size  
ys = y size  
oc = outline color  
fc = fill color  
tc = text color  
ts = text size  
bt = button text (must end with a colon :) (max 9 chars)  


*Line chart:*

> Up to 4 line charts may be defined. Ticks may be defined by adding
> tick numbers to the 1. parameter n like this =\> n = graph number
> (0-3) + x ticks (16\*number of x ticks) + y ticks (1024\*number of y
> ticks)  
(#define USE_GRAPH #define NUM_GRAPHS 4 (16 max))

>*Gn:xp:yp:xs:ys:t:fmax:fmin* = defines a line chart.  
n = number up to 4 charts (0-3) + optional ticks  
xp = x position  
yp = y position  
xs = x size  (if xs<0) graph is not reinitialized on second call (e.g. restart of scripter)
ys = y size  
t = time in minutes for total chart  
ymin = float chart minimum y  
ymax = float chart maximum y  
icol = line indexcolor (only for color graphs)  
gn:v Adds a value to the chart buffer  
n = number up to 4 charts (0-3)  
v = float value to add  
*Gdn:m* = set graph n draw mode 0=off, 1=on when on, redraw graph


**remarks on e paper displays:**

epaper displays have 2 operating modes: full update and partial update.
While full update delivers a clean and sharp picture it has the
disadvantage of taking several seconds for the screen update and a
severe flickering during update.
Partial update is quite fast (300 ms) with no flickering but there is
the possiblity that erased content is still slightly visible .
To „whiten" the display it is therefore usefull to „full update" the
display in regular intervals (e.g each hour)

2 types are supported:  
the 2.9 inch partial update display (296x128)  
and the 4.2inch full update display (400x300)  

Defines =\> USE\_SPI , USE\_DISPLAY\_EPAPER29, USE\_DISPLAY\_EPAPER42

To unify the color interface on epaper 1 (white) is actually black and 0
(black) is white.
updating the display does not wait for completion, so after updating you
must wait about 300 ms before the next update cmd.  


**Hardware connections:**  

I2C displays are connected in the usual manner and defined by tasmota
pin selection  
The I2C Adress must be given by DisplayAdress XX e.g. 60 and the Model
set with DisplayModel e.g. 2 (for SSD1306) to permanently turn the
display on set DisplayDimmer 100. Display rotation can be permantly set
by DisplayRotate X (0-3)  
E-paper displays are connected via 3 wire SPI (CS,SCLK,MOSI) the other 3
Interface lines of the display (DC,Reset,busy) may be left unconnected.
The jumper on the circuit board of the display should be set to 3 wire
SPI. You can NOT use GPIO16 for software spi!

**ILI9488 driver**  

This color LCD display (480x320) uses a 3 wire hardware SPI interface. (MOSI=GPIO13, SCLK=GPIO14,CS=GPIO15) the backlight on/off pin must be selected too.
Defines =\> USE\_SPI , USE\_DISPLAY\_ILI9488  

The capacitive touch panel is connected via I2C.  
if you want to use virtual buttons define USE_TOUCH_BUTTONS  

**RA8876 driver**  

This color LCD display (1024x600) uses normal hardware SPI (with MISO) (backlight pin not needed)  
Defines =\> USE\_SPI , USE\_DISPLAY\_RA8876  
font 7 is the build in font and is extremely fast. (size from 1..4 supported)
over all this display is extremely fast because of hardware acceleration.
rotation not yet supported.  
The capacitive touch panel is connected via I2C.  
if you want to use virtual buttons define USE_TOUCH_BUTTONS  
(dimmer supported)  

**SSD1351 driver**  

This OLED color display (128x128) also uses a 3 wire SPI interface. This driver uses hardware SPI (MOSI=GPIO13, SCLK=GPIO14,CS=GPIO15)
(dimmer supported)  

Defines =\> USE\_SPI , USE\_DISPLAY\_SSD1351  

**remarks on OLED displays:**  

oled display modules have an expected lifetime of about 10000 hours (416 days) which means they should not be on permanently. you may use a proximity sensor to turn the display on and off.
dimming the display will also expand the lifetime. (dimming to 50% expands the lifetime to about 25000 hours)  

**remarks on burnin:**  

OLED displays as well as LCD displays are subject to burn in when a static  
content is displayed over a long period of time. therefore they should not be on  
permanently. you may use a proximity sensor to turn the display on and off.  


**remarks on fonts:**  

The EPD fonts contains 95 chars starting from code 32 while the classic
GFX font containts 256 chars ranging from 0 to 255.  
To display chars above code 127 you must specify an escape sequence
(standard octal escapes dont work here) the \~character followed by a
hex byte can define any character code. So custom characters above 127
can be displayed  

**remarks on heap usage:**

the epaper and b/w oled displays use a frame buffer within heap memory which consumes x*y/8 bytes. (e.g. 15 kb on the 400x600 epaper)
the color lcd and the color OLED do not hold frame buffers in heap  

**examples:**

// print Text at line  
// size 1 on line 1, column 1  
DisplayText \[s1l1c1\]Hallo how are you?  

// draw a rectangle and draw text inside with size2 and 7 chars padded
with spaces  
DisplayText \[x85y95h130v30h-130v-30s2p7x90y100\]37.25 C

// clear screen  
DisplayText \[z\]

// draw rectangle from x,y with width and height  
DisplayText \[x50y50r200:100\]  

you can display local sensors via rules or scripts =\>  

// show sensor values and time and separation line, whiten display every
60 minutes

// on 296x128

rule1 on tele-SHT3X-0x44\#Temperature do DisplayText \[f1p7x0y5\]%value%
C endon on tele-SHT3X-0x44\#Humidity do DisplayText
\[f1p10x70y5\]%value% % \[x0y20h296x250y5t\] endon on
tele-BMP280\#Pressure do DisplayText \[f1p10x140y5\]%value% hPa endon on
Time\#Minute\|60 do DisplayText \[Ii\] endon

// show 4 analog channels (on128x64)

rule1 on tele-ADS1115\#A0 do DisplayText \[s1p21c1l01\]Analog1: %value%
adc endon on tele-ADS1115\#A1 do DisplayText \[s1p21c1l3\]Analog2:
%value% adc endon on tele-ADS1115\#A2 do DisplayText
\[s1p21c1l5\]Analog3: %value% adc endon on tele-ADS1115\#A3 do
DisplayText \[s1p21c1l7\]Analog4: %value% adc endon

// show BME280 + SGP30 (on 128x64)

rule1 on tele-BME280\#Temperature do DisplayText \[s1p21x0y0\]Temp:
%value% C endon on tele-BME280\#Humidity do DisplayText
\[s1p21x0y10\]Hum : %value% % endon on BME280\#Pressure do DisplayText
\[s1p21x0y20\]Prss: %value% hPa endon on tele-SGP30\#TVOC do DisplayText
\[s1p21x0y30\]TVOC: %value% ppb endon on tele-SGP30\#eCO2 do DisplayText
\[s1p21x0y40\]eCO2: %value% ppm \[s1p0x0y50\]Time: \[x35y50t\] endon

// show graphs etc on 400 x 300 epaper % sign must be %% in core 2.42
!!!

// must use core 2.42 due to ram size restrictions !!!

rule1 on tele-SHT3X-0x44\#Temperature do DisplayText \[f1p7x0y5\]%value%
C endon on tele-SHT3X-0x44\#Humidity do DisplayText
\[x0y20h400x250y5T\]\[x350t\]\[f1p10x70y5\]%value% %% endon on
tele-BMP280\#Pressure do DisplayText \[f1p10x140y5\]%value% hPa endon on
Time\#Minute\|60 do DisplayText \[Ii\] endon

rule2 on System\#Boot do DisplayText
\[zG2656:5:40:300:80:1440:-5000:5000f3x310y40\]+5000 W\[x310y115\]-5000
W endon on System\#Boot do DisplayText \[f1x60y25\]Zweirichtungszaehler
- 24 Stunden\[f1x330y75\] 3500 W endon on System\#Boot do DisplayText
\[G2657:5:140:300:80:1440:0:5000f3x310y140\]+5000 W\[x310y215\]0 W endon
on System\#Boot do DisplayText \[f1x70y125\]Volleinspeisung - 24
Stunden\[f1x330y180\] 3500 W endon

// oled 128x64 show 24 hour graph of range 0-5000 watts house power

rule1 on System\#Boot do DisplayText \[zG1064:0:0:128:40:1440:0:5000d\]
endon on tele-OBIS\#Power\_curr do DisplayText
\[g0:%value%ds1f0p8x25y45\]power: %value% W endon

or distant sensors via a broker script or on the distant device via
rules and WebSend

**remarks to display drivers:**

Waveshare has 2 kinds of display controllers: with partial update and
without partial update. The 2.9 inch driver is for partial update and
should support also other waveshare partial update models with modified
WIDTH and HEIGHT parameters

The 4.2 inch driver is a hack which makes the full update display behave
like a partial update and should probably work with other full update
displays.

the drivers are subclasses of GFX library  
the class hirarchie is LOWLEVEL :: Paint :: Renderer :: GFX  
modified (some more functions declared virtual for acceleration) GFX library    
Renderer: the interface for Tasmota  
Paint: the modified pixel driver for epaper  
there are several virtual functions that can be subclassed down to
LOWLEVEL.

The display dispatcher only does the class init call  
All other calls go to the Renderer class  
In black and white displays a local ram buffer must be allocated before
calling the driver  
This must be set to zero on character or TFT color displays.  

The GFX proportional fonts can alternativly be used instead of the EPD
fonts by selecting the fonts with \#define in the renderer driver file.

only 1 SPI Display driver \#define is allowed at a time  


**Remark for the 400x300 epaper:**

This display requires 15k RAM which only fits when using the 2.42
Arduino library because this lib leaves much more free ram than previous
versions. epd fonts use about 9k flash space. Can be ifdefd  

SSD1306 = 1,15 k  
SH1106 = 1,18 k  
EPD42 = 2,57 k  
EPD29 = 2,1 k  
ILI9488 = 7,3 k  
SSD1351 = 1,7 k  

Display and Renderer class about 12 k  

**List of TASMOTA display codes (displaymodel)**  

>1 = LCD  character LCD  
2 = SSD1306   128x64 b/w oled 0.96 inch  
3 = MATRIX  
4 = ILI9341 TFT  
5 = Waveshare 296x128 E-PAPER 2.9 inch  
6 = Waveshare 400x300 E-PAPER 4.2 inch  
7 = SH1106  128x64 b/w oled 1.5 inch  
8 = ILI9488 480x320 TFT =\> ER-TFTM035-6 3.5 inch  
9 = SSD1351 128x128 color oled  1.5 inch  
10 = RA8876 1024x600 TFT =\> ER-TFTM070-6 7 inch

Index colors selected in the color panels ILI9488,SSD1351,RA8876 with Ci and Bi

0=ILI9488\_BLACK  
1=ILI9488\_WHITE  
2=ILI9488\_RED  
3=ILI9488\_GREEN  
4=ILI9488\_BLUE  
5=ILI9488\_CYAN  
6=ILI9488\_MAGENTA  
7=ILI9488\_YELLOW  
8=ILI9488\_NAVY  
9=ILI9488\_DARKGREEN  
10=ILI9488\_DARKCYAN  
11=ILI9488\_MAROON  
12=ILI9488\_PURPLE  
13=ILI9488\_OLIVE  
14=ILI9488\_LIGHTGREY  
15=ILI9488\_DARKGREY  
16=ILI9488\_ORANGE  
17=ILI9488\_GREENYELLOW  
18=ILI9488\_PINK  
