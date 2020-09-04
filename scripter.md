**Script Language for Tasmota**  

As an alternative to rules. (about 17k flash size, variable ram size)  

In submenu Configuration =\> edit script  
1535 bytes max script size (uses rules buffer)

to enable:  
\#define USE_SCRIPT  
\#undef USE_RULES  


Up to 50 variables (45 numeric and 5 strings, maybe changed by #define)  
Freely definable variable names (all names are intentionally case sensitive)  
Nested if,then,else up to a level of 8  
Math operators  **+,-,\*,/,%,&,|,^**  
all operators may be used in the op= form e.g. **+=**  
Left right evaluation with optional brackets  
all numbers are float  
e.g. temp=hum\*(100/37.5)+temp-(timer\*hum%10)  
no spaces allowed between math operations
Comparison operators **==,!=,\>,\>=,<,<=**  
**and** , **or** support  

strings support **+** and **+=** operators  
string comparison **==,!=**  
max string size = 19 chars (default, can be increased or decreased by optional >D parameter)  

**Comments** start with **;**  

**Sections** defined:  

>**\>D ssize**  
ssize = optional max stringsize (default=19)  
define and init variables here, must be the first section, no other code allowed  
**p:**vname specifies permanent vars (the number of permanent vars is limited by tasmota rules space (50 bytes)
numeric var=4 bytes, string var=lenght of string+1)  
**t:**vname specifies countdown timers, if >0 they are decremented in seconds  until zero is reached. see example below  
**i:**vname specifies auto increment counters if >=0 (in seconds)  
**m:**vname specifies a median filter variable with 5 entries (for elimination of outliers)  
**M:**vname specifies a moving average filter variable with 8 entries (for smoothing data)  
(max 5 filters in total m+M) optional another filter lenght (1..127) can be given after the definition.  
filter vars can be accessed also in indexed mode vname[x] (index = 1...N, index 0 returns current array index pointer)  
by this filter vars can be used as arrays  

>all variable names length taken together may not exceed 256 characters, so keep variable names as short as possible.  
memory is dynamically allocated as a result of the D section.  
copying a string to a number or reverse is supported   

>**\>B**  
executed on BOOT time  

>**\>T**  
executed on teleperiod time (**SENSOR** and **STATE**), get tele vars only in this section  

>**\>S**  
executed every second  

>**\>E**  
executed e.g. on power change  and mqtt **RESULT**

>**\>R**  
executed on restart, p vars are saved automatically after this call  


special variables (read only):  

>**upsecs** = seconds since start  
**uptime** = minutes since start  
**time** = minutes since midnight  
**sunrise** = sunrise minutes since midnight  
**sunset** = sunset minutes since midnight  
**tper** = teleperiod (may be set also)   
**tstamp** = timestamp (local date and time)  
**topic** = mqtt topic  
**gtopic** = mqtt group topic  
**prefixn** = prefix n = 1-3    
**pwr[x]** = tasmota power state  (x = 1-N)  
**sw[x]** = tasmota switch state  (x = 1-N)  
>**pin[x]** = gpio pin level (x = 0-16)  
**pn[x]** = pin number for sensor code x, 99 if none  
**pd[x]** = defined sensor for gpio pin nr x none=999  
**gtmp** = global temperature  
**ghum** = global humidity  
**gprs** = global pressure  
**pow(x y)** = calculates the power of x^y  
**med(n x)** = calculates a 5 value median filter of x (2 filters possible n=0,1)  
**int(x)** = gets the integer part of x (like floor)  
**hn(x)** = converts x (0..255) to a hex nibble string  
**st(svar c n)** = stringtoken gets the n th substring of svar separated by c  
**s(x)** = explicit conversion from number x to string  
**mqtts** = state of mqtt disconnected=0, connected>0  
**wifis** = state of wifi disconnected=0, connected>0  

>**hours** = hours  
**mins** = mins  
**secs** = seconds  
**day** = day of month  
**wday** = day of week  
**month** = month  
**year** = year  

these variables are cleared after reading true  
>**chg[var]** = true if a variables value was changed (numeric vars only)  
**upd[var]** = true if a variable was updated  
**boot** = true on BOOT  
**tinit** = true on time init  
**tset** = true on time set  
**mqttc** = true on mqtt connect  
**mqttd** = true on mqtt disconnect  
**wific** = true on wifi connect  
**wifid** = true on wifi disconnect  

system vars (for debugging)
>**stack** = stack size  
**heap** = heap size  
**ram** = used ram size  
**slen** = script length  
**micros** = running microseconds  
**millis** = running milliseconds  
**loglvl** = loglevel of script cmds, may be set also  

remarks:
if you define a variable with the same name as a special
variable that special variable is discarded  


**Tasmota** cmds start with **=\>**  
within cmds you can replace text with variables with **%varname%**  
a single percent sign must be given as **%%**  

**special** cmds:

>**=\> print** prints to info log for debugging  

to save code space nearly no error messages are provided. However it is taken care of that at least it should not crash on syntax errors.  
if a variable does not exist a **???** is given on commands  
if a **SENSOR** or **STATUS** or **RESULT** message or a var does not exist the destination variable is NOT updated.  

2 possibilities for conditionals:  
>**if** a==b  
**and** x==y  
**or** k==i  
**then** => do this  
**else** => do that  
**endif**  

OR  

>**if** a==b  
**and** x==y  
**or** k==i **{**  
  => do this  
**} else {**  
  => do that  
**}**  

you may NOT mix both methods  

also possible e.g.  

>if var1-var2==var3*var4  
then  

remarks:  
the last closing bracket must be on a single line  
the condition may not be enclosed in brackets  

>**break** exits a section or terminates a for next loop  
**dpx** sets decimal precision to x (0-9)  
**svars** save permanent vars  
**delay(x)** pauses x milliseconds (should be as short as possible)  
**spin(x m)** set gpio pin x (0-16) to value m (0,1) only the last bit is used, so even values set the pin to zero and uneven values set the pin to 1  
**spinm(x m)** set pin mode gpio pin x (0-16) to mode m (input=0,output=1)  

>**#name** names a subroutine, subroutines are called with **=#name**  
**#name(param)** names a subroutines with a parameter is called with **=#name(param)**  
subroutines end with the next '#' or '>' line or break, may be nested  
params can be numbers or strings and on mismatch are converted  

>**for var from to inc**  
**next**  
specifies a for next loop, (loop count must not be less then 1)

>**switch x**    
**case a**  
**case b**  
**ends**  
specifies a switch case selector  

**sd card support**  
enable by CARD_CS = gpio pin of card chip select (+ 10k flash)  
\#define USE_SCRIPT_FATFS CARD_CS   
sd card uses standard hardware spi gpios: mosi,miso,sclk  
max 4 files open at a time  
allows for e.g. logging sensors to a tab delimited file and then download (see example below)  
the download of files may be executed in a kind of "multitasking" when bit 7 of loglvl is set (128+loglevel)  
without multitasking 150kb/s (all processes are stopped during download), with multitasking 50kb/s (other tasmota processes are running)  
script itself is also stored on sdcard with a default size of 4096 chars  


enable sd card directory support (+ 1,2k flash)  
\#define SDCARD_DIR  
shows a web sdcard directory (submeu of scripter) where you can
upload and download files to/from sd card  


>**fr=fo("fname" m)** open file fname, mode 0=read, 1=write (returns file reference (0-3) or -1 for error)  
**res=fw("text" fr)** writes text to (the end of) file fr, returns number of bytes written  
**res=fr(svar fr)** reads a string into svar, returns bytes read (string is read until delimiter \t \n \r or eof)  
**fc(fr)** close file  
**ff(fr)** flush file, writes cached data and updates directory     
**fd("fname")** delete file fname   
**flx(fname)** create download link for file (x=1 or 2) fname = file name of file to download   
**fsm** return 1 if filesystem is mounted, (valid sd card found)  

extended commands   (+0,9k flash)
\#define USE_SCRIPT_FATFS_EXT  
>**fmd("fname")** make directory fname  
>**frd("fname")** remove directory fname  
>**fx("fname")** check if file fname exists  
>**fe("fname")** execute script fname  (max 2048 bytes, script file must start with '>' char on the 1. line)  


**konsole script cmds**  
>**script 1 or 0** switch script on or off  
**script >cmdline** executes the script cmdline  
can be used e.g. to set variables  e.g. **script >mintmp=15**  
more then one line may be executed seperated by a semicolon e.g. **script >mintmp=15;maxtemp=40**   
script itself cant be set because the size would not fit the mqtt buffers

***example script***  
meant to show some of the possibilities    
(actually this code ist too large)  

**\>D**  
; define all vars here  
p:mintmp=10  (p:means permanent)  
p:maxtmp=30  
t:timer1=30  (t:means countdown timer)  
t:mt=0  
i:count=0  (i:means auto counter)  
hello=&quot;hello world&quot;  
string=&quot;xxx&quot;  
url=&quot;[192.168.178.86]&quot;  
hum=0  
temp=0  
timer=0  
dimmer=0  
sw=0  
rssi=0  
param=0  

col=&quot;&quot;  
ocol=&quot;&quot;  
chan1=0  
chan2=0  
chan3=0  

ahum=0  
atemp=0  
tcnt=0  
hour=0
state=1  
m:med5=0  
M:movav=0  
; define array with 10 entries  
m:array=0 10  

**\>B**  

string=hello+"how are you?"  
=\>print BOOT executed  
=\>print %hello%  
=\>mp3track 1  

; list gpio pin definitions  
for cnt 0 16 1  
tmp=pd[cnt]  
=>print %cnt% = %tmp%  
next  

; get gpio pin for relais 1  
tmp=pn[21]  
=>print relais 1 is on pin %tmp%  

; pulse relais over raw gpio  
spin(tmp 1)  
delay(100)  
spin(tmp 0)  

; raw pin level  
=>print level of gpio1 %pin[1]%  

; pulse over tasmota cmd  
=>power 1  
delay(100)  
=>power 0  

**\>T**  
hum=BME280#Humidity  
temp=BME280#Temperature  
rssi=Wifi#RSSI  
string=SleepMode  

; add to median filter  
median=temp  
; add to moving average filter  
movav=hum  

; show filtered results  
=>print %median% %movav%  

if chg[rssi]>0  
then =>print rssi changed to %rssi%  
endif  

if temp\>30  
and hum\>70  
then =\>print damn hot!  
endif  

**\>S**  

; every second but not completely reliable time here  
; use upsecs and uptime or best t: for reliable timers  

; arrays  
array[1]=4  
array[2]=5  
tmp=array[1]+array[2]  

; call subrountines with parameters   
=#sub1("hallo")  
=#sub2(999)  

; stop timer after expired  
if timer1==0  
then timer1=-1  
=>print timer1 expired  
endif  

; auto counter with restart  
if count>=10  
then =>print 10 seconds over  
count=0  
endif  

if upsecs%5==0  
then =\>print %upsecs%  (every 5 seconds)  
endif  

; not recommended for reliable timers  
timer+=1  
if timer\>=5  
then =\>print 5 seconds over (may be)  
timer=0  
endif  

dimmer+=1  
if dimmer\>100  
then dimmer=0  
endif  

=\>dimmer %dimmer%  
=\>WebSend %url% dimmer %dimmer%  

; show on display  
dprec0  
=\>displaytext [c1l1f1s2p20] dimmer=%dimmer%  

=\>print %upsecs% %uptime% %time% %sunrise% %sunset% %tstamp%  

if time\>sunset  
and time< sunrise  
then  
; night time  
if pwr[1]==0  
then =\>power1 1  
endif  
else  
; day time  
if pwr[1]\>0  
then =\>power1 0  
endif  
endif  

; clr display on boot  
if boot\>0  
then =\>displaytext [z]  
endif  

; frost warning  
if temp<0  
and mt<=0  
then =#sendmail("frost alert")  
; alarm only every 5 minutes  
mt=300  
=>mp3track 2  
endif  

; var has been updated  
if upd[hello]>0  
then =>print %hello%  
endif  

; send to Thingspeak every 60 seconds  
; average data in between  
if upsecs%60==0   
then  
ahum/=tcnt  
atemp/=tcnt  
=>Websend [184.106.153.149:80]/update?key=PYUZMVWCICBW492&field1=%atemp%&field2=%ahum%  
tcnt=0  
atemp=0  
ahum=0   
else  
ahum+=hum  
atemp+=temp  
tcnt+=1  
endif  

hour=int(time/60)  
if chg[hour]>0  
then  
; exactly every hour  
=>print full hour reached  
endif  

if time>5 {  
=>print more then 5 minutes after midnight   
} else {  
=>print less then 5 minutes after midnight  
}  


; publish abs hum every teleperiod time  
if mqtts>0  
and upsecs%tper==0  
then  
; calc abs humidity  
tmp=pow(2.718281828 (17.67\*temp)/(temp+243.5))  
tmp=(6.112\*tmp\*hum\*18.01534)/((273.15+temp)\*8.31447215)  
; publish median filtered value  
=>Publish tele/%topic%/SENSOR {"Script":{"abshum":%med(0 tmp)%}}  
endif  

;switch case state machine   
switch state  
case 1  
=>print state=%state% , start  
state+=1  
case 2  
=>print state=%state%  
state+=1  
case 3  
=>print state=%state%  , reset  
state=1  
ends  


; subroutines  
\#sub1(string)  
=>print sub1: %string%  
\#sub2(param)  
=>print sub2: %param%  

\#sendmail(string)  
=>sendmail [smtp.gmail.com:465:user:passwd:<sender@gmail.de>:<rec@gmail.de>:alarm] %string%  

**\>E**  
=\>print event executed!  

; get HSBColor 1. component  
tmp=st(HSBColor , 1)  

; check if switch changed state  
sw=sw[1]  
if chg[sw]>0  
then =\>power1 %sw%  
endif  

hello=&quot;event occured&quot;  

; check for Color change (Color is a string)  
col=Color  
; color change needs 2 string vars  
if col!=ocol  
then ocol=col  
=>print color changed  %col%  
endif  

; or check change of color channels  
chan1=Channel[1]  
chan2=Channel[2]  
chan3=Channel[3]  

if chg[chan1]>0  
or chg[chan2]>0  
or chg[chan3]>0  
then => color has changed  
endif  

; compose color string for red  
col=hn(255)+hn(0)+hn(0)  
=>color %col%  

**\>R**  
=\>print restarting now  

**a log sensor example**  
; define all vars here  
; reserve large strings  
**\>D** 48  
hum=0  
temp=0  
fr=0  
res=0  
; moving average for 60 seconds  
M:mhum=0 60  
M:mtemp=0 60  
str=""  

**\>B**  
; set sensor file download link   
fl1("slog.txt")  
; delete file in case we want to start fresh
;fd("slog.txt")  


; list all files in root directory  
fr=fo("/" 0)  
for cnt 1 20 1  
res=fr(str fr)  
if res>0  
then  
=>print %cnt% : %str%  
else  
break  
endif  
next  
fc(fr)  


**\>T**  
; get sensor values  
temp=BME280#Temperature  
hum=BME280#Humidity  

**\>S**
; average sensor values every second  
mhum=hum  
mtemp=temp  

; write average to sensor log every minute  
if upsecs%60==0  
then  
; open file for write  
fr=fo("slog.txt" 1)  
; compose string for tab delimited file entry  
str=s(upsecs)+"\t"+s(mhum)+"\t"+s(mtemp)+"\n"  
; write string to log file  
res=fw(str fr)  
; close file  
fc(fr)  
endif  

**\>R**  



**a real example**  
epaper 29 with sgp30 and bme280  
some vars are set from iobroker  
DisplayText substituted to save script space  
**\>D**  
hum=0  
temp=0  
press=0  
ahum=0  
tvoc=0  
eco2=0  
zwz=0  
wr1=0  
wr2=0  
wr3=0  
otmp=0  
pwl=0  
tmp=0  
DT="DisplayText"  
; preset units in case they are not available   
punit="hPa"  
tunit="C"  

**\>B**  
;reset auto draw  
=>%DT% [zD0]  
;clr display and draw a frame  
=>%DT% [x0y20h296x0y40h296]  

**\>T**  
; get tele vars  
temp=BME280#Temperature  
hum=BME280#Humidity  
press=BME280#Pressure  
tvoc=SGP30#TVOC  
eco2=SGP30#eCO2  
ahum=SGP30#aHumidity  
tunit=TempUnit  
punit=PressureUnit  

**\>S**  
// update display every teleperiod time  
if upsecs%tper==0  
then  
dprec2  
=>%DT% [f1p7x0y5]%temp% %tunit%  
=>%DT% [p5x70y5]%hum% %%[x250y5t]   
=>%DT% [p11x140y5]%press% %punit%  
=>%DT% [p10x30y25]TVOC: %tvoc% ppb  
=>%DT% [p10x160y25]eCO2: %eco2% ppm  
=>%DT% [p10c26l5]ahum: %ahum% g^m3  

dprec0  
=>%DT% [p25c1l5]WR 1 (Dach)  : %wr1% W  
=>%DT% [p25c1l6]WR 2 (Garage): %-wr3% W  
=>%DT% [p25c1l7]WR 3 (Garten): %-wr2% W  
=>%DT% [p25c1l8]Aussentemperatur: %otmp% C  
=>%DT% [x170y95r120:30f2p6x185y100] %pwl% %%  
; now update screen  
=>%DT% [d]  
endif  


**\>E**  

**\>R**  

**another real example**  
ILI 9488 color LCD Display shows various energy graphs  
display switches on and off with proximity sensor  
BMP280 and vl5310x
some vars are set from iobroker  

**>D**  
temp=0  
press=0  
zwz=0  
wr1=0  
wr2=0  
wr3=0  
otmp=0  
pwl=0  
tmp=0  
dist=0  
DT="DisplayText"  
punit="hPa"  
tunit="C"  
hour=0  

**>B**  
=>%DT% [z]  

// define 2 graphs, 2. has 3 tracks  
=>%DT% [zCi1G2656:5:20:400:80:1440:-5000:5000:3Ci7f3x410y20]+5000 W[x410y95]-5000 W [Ci7f1x70y3] Zweirichtungsz~80hler - 24 Stunden  
=>%DT%  [Ci1G2657:5:120:400:80:1440:0:5000:3Ci7f3x410y120]+5000 W[x410y195]0 W [Ci7f1x70y103] Wechselrichter 1-3 - 24 Stunden  
=>%DT% [Ci1G2658:5:120:400:80:1440:0:5000:16][Ci1G2659:5:120:400:80:1440:0:5000:5]  
=>%DT% [f1s1b0:260:260:100:50:2:11:4:2:Rel 1:b1:370:260:100:50:2:11:4:2:Dsp off:]  
=>mp3volume 100  
=>mp3track 4  

**>T**  
; get some tele vars  
temp=BMP280#Temperature  
press=BMP280#Pressure  
tunit=TempUnit  
punit=PressureUnit  
dist=VL53L0X#Distance  

; check proximity sensor to switch display on/off  
; to prevent burn in  
if dist>300  
then  
if pwr[2]>0  
then  
=>power2 0  
endif  
else  
if pwr[2]==0  
then  
=>power2 1  
endif  
endif  


**>S**  
; update graph every teleperiod  
if upsecs%tper==0  
then  
dprec2  
=>%DT% [f1Ci3x40y260w30Ci1]  
=>%DT% [Ci7x120y220t]  
=>%DT% [Ci7x180y220T]  
=>%DT% [Ci7p8x120y240]%temp% %tunit%   
=>%DT% [Ci7x120y260]%press% %punit%  
=>%DT% [Ci7x120y280]%dist% mm  
dprec0  
=>%DT% [g0:%zwz%g1:%wr1%g2:%-wr2%g3:%-wr3%]  
if zwz>0  
then
=>%DT% [p-8x410y55Ci2Bi0]%zwz% W  
else
=>%DT% [p-8x410y55Ci3Bi0]%zwz% W  
endif  
=>%DT% [p-8x410y140Ci3Bi0]%wr1% W  
=>%DT% [p-8x410y155Ci16Bi0]%-wr2% W  
=>%DT% [p-8x410y170Ci5Bi0]%-wr3% W  
endif  

; chime every full hour  
hour=int(time/60)  
if chg[hour]>0  
then =>mp3track 4  
endif  

**>E**

**>R**
