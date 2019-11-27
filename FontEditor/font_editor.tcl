// this programm can edit bitmap fonts from epd and gfx_classic in c source code 
// and convert jpeg pictures to GFX 16 bit c color arrays
SET SCREEN 1024 768  10 10 "EPD Bitmap Font Editor" 
NEW SCREEN
CLR SCREEN C_WH6
DEFINE PATH "home"
INCLUDE "paket/glob_macros.tcl"
/* start history
v 0.1 = speed optimizations
v 0.2 = gfx classic fonts
v 0.3 = character import (mac only)
v 0.4 = character shift
v 0.5 = convert jpeg pictures to c array, character import Windows too
v 0.6 = import pictogramm jpegs into edit window
*/

fontpath=""
fontpath = SELECT 0 "select epd font c file"
if fontpath==""
	END

FLUSHSCRN

// when choosing other fonts:
// on windows MUST choose fonts that are really loaded! else crashes 
//FONT="Monaco"
//FONT="Courier"
//FONT="Helvetica"

:restart
TIMEOUT=100
b1=CONTROLS

s1=STRTOK(fontpath "." 99)
if s1=="jpg"
or s1=="JPG"
then
	goto :convertpicture
endif


SkipLastPathElement fontpath
temppath=""
temppath =path + "/tempfile.c"
numlines=0

// read asci file into stringtable
STRINGTABLE  1 fontpath ""  1 2 1
numlines=b1

fontx=0
fonty=0
copied=0
line=0
blinecount=0
gfx_classic=0
char_offset=32
for line 1 numlines 1
	STRINGTABLE  1 "" ""  line 1 1
	b1= INSTR(s1 "sFONT")
	if b1>=0
	then
		STRINGTABLE  1 "" ""  line+2 1 2
		STRINGTABLE  1 "" ""  line+3 1 3
		fontx=STRNUM(s2)
		fonty=STRNUM(s3)
		break
	endif
	
	b1= INSTR(s1 "0x")
	if b1>=0
	then
		blinecount+=1
	endif
	
	b1= INSTR(s1 "FONT5X7_H")
	if b1>=0
	then
		gfx_classic=1
		fontx=5
		fonty=7
		char_offset=0
	endif
	
next

fontbytes=0
fontbytes= FLOOR(fontx/8)
b1=fontx%8
if b1>0
	fontbytes+=1

numchars =0
// number of defined chars
if gfx_classic>0
then
	numchars= blinecount
else
	numchars = blinecount/fonty
endif

//print fontx TAB fonty TAB blinecount TAB fontbytes TAB numchars LF

//print "characters found: " numchars LF
//print "of size: " fonty " x " fontx LF
//print "bytes per line: " fontbytes LF


// copy all chars to array 3
SET ALEN a3 fontbytes*fonty*numchars
SET ALEN a1 numchars
// build font index
blinecount=0
b10=0
for line 1 numlines 1
	STRINGTABLE  1 "" ""  line 1 1
	b1= INSTR(s1 "0x")
	if b1>=0
	then
		if gfx_classic==0
		then
			for b1 1 fontbytes 1
				a3=HSTRNUM(STRTOK(s1 "," b1))
			next
			b1=blinecount%fonty
			if b1==0
			then
				a1=line
			endif
			blinecount+=1
		else
			for b1 1 5 1
				a3=HSTRNUM(STRTOK(s1 "," b1))
			next
		endif
	endif
next

gosub :printset

cxs=100
cys=30
cxp=SCRNXS-cxs-10
cyp=SCRNYS-cys-10
ccs=14
xButton "Exit" 1 0

cxs=100
cxp=10
cdx=10
xButton "load a font" 4 0
xButton "add a char" 2 0
xButton "save font as" 3 0
val_num=1
cxs=90
Value "char: %02.0f" -80 
tlxp=cxp
tlyp=cyp
tlxs=200
tlys=cys
tlcs=ccs
s1=NUMSTR("Size: %2.0f" fonty) + NUMSTR("x%2.0f" fontx) + NUMSTR(" : %2.0f chrs" numchars)
Tlog s1
cxp+=215

cxs=70
xButton "imp =>" 5 0
s33="A"
ccs=18
impxs=0
impys=0
impxp=0
impyp=0
if WIN32==0
then
	FONT="Monaco"
	ccs=54
else
	//FONT="Arial"
	ccs=48
endif

impxp=cxp
impyp=cyp-20
impxs=40
impys=60
CONTROL "" cxp  cyp-20 impxs impys ccs C_BK C_WH7 33 5 3 0 0 0

ccs=18
cxs=30
cxp+=impxs+10
xButton "?" 6 0

cxp+=impxs+5
mousx=0
mousy=0

:mloop
TIMEOUT = 10
b1=CONTROLS
ongoto b1 1 :exit 2 :addchar 3 :save 4 :load 5 :import 6 :help
gosub :checkhit
loop :mloop

:exit
END

:help
HELP "!shift click on char => copy that char\rctrl click on char => paste char\rtype any char into import box and then paste that char with ctrl click\rin edit window you may import any jpeg picture with 'import'\r\rselecting a picture in initial selction box converts that picture to c color image array (eg for ILI9488)" 50 50 800 400 16
loop :mloop

// create new offscreen buffer number 1
MACRO "New_OB1"
// delete buffer if already there
DRAW PICT "" 0 0 0 0 0 -1
SCREEN 1
ENDM


// import from text input field
// can only read pixels from offscreen buffer
:import
// create offscreen
New_OB1
// draw main screen to offscreen
DRAW SCREEN -1 -1 0 SCRNXS SCRNYS
ixs=0
iys=0
ixs=impxs-3
iys=impys-4
SET ALEN a20 ixs*iys*3
// read pixels from offscreen buffer
GPIX -1 20 impxp+1 impyp+3 ixs iys
// switch back to main screen
SCREEN -1

cval=0
index=0
xindex=0
yindex=0
// transpose to destination pattern
SET ALEN a5 fontx*fonty
for b1 0 iys-1 1
	for b2 0 ixs-1 1
		index=(b1*ixs*3)+(b2*3)
		cval=a20[index+1]+a20[index+2]+a20[index+3]
		xindex=FLOOR(fontx/ixs*b2)
		yindex=FLOOR(fonty/iys*b1)
		b3=yindex*fontx+xindex+1
		if cval<1.0
		then
			a5[b3]=1
		endif
	next
next

// set bitfield pattern
// for pasting into char
scale=4
SET ALEN a2 fontbytes*fonty
for b1 0 fonty-1 1
	for b2 0 fontx-1 1
		b3=(b1*fontx)+b2+1
		cval=a5[b3]
		if cval>0
		then
			// set bit
			byte=FLOOR(b2/8)
			bit=0
			bit=FLOOR(b2%8)
			val=0
			b4=(b1*fontbytes)+byte+1
			val=a2[b4]
			bit=7-bit
			b5=1<<bit
			val=val|b5
			a2[b4]=val
		endif
	next
next
copied=1
loop :mloop

:setscale
if fontx>24
then
	scale=SCRNXS/(10*fontx)
else
	scale=SCRNXS/(30*fontx)
endif

scale=FLOOR(scale)
return
/*
switch fontx
	case 5
		scale=8
		break
	case 7
		scale=6
		break
	case 11
		scale=4
		break
	case 14
		scale=3
		break
	case 17
		scale=2
		break
	default
		scale=5
ends
if gfx_classic>0
	scale=6
return
*/

:printset
// now print chars
// this takes a while especially on windows
cxp=0
cyp=0
gosub :setscale

cxs=fontx*scale+2
cys=fonty*scale+2

charcnt=0
screen_maxx=0
screen_maxy=0
chars_xmax=0
chars_y=0
chars_x=0
chars_x=cxs
chars_y=cys+20
for charcnt 0 numchars-1 1
	tprt=1
	gosub :copychar
	gosub :printchar
	cxp+=cxs
	if cxp+cxs>=SCRNXS
	then
		cxp=0
		cyp+=cys+20
	endif
	b1=cxp+cxs
	if b1>screen_maxx
	then
		screen_maxx=b1
		chars_xmax=FLOOR(screen_maxx/cxs)
	endif
next
FLUSHSCRN
screen_maxy=cyp+cys+20
return

:checkhit
mousx=MOUSX
mousy=MOUSY
if mousx>0
and mousx<screen_maxx
and mousy>0
and mousy<screen_maxy
then
	b1=FLOOR(mousx/chars_x)
	//b1=mousx/chars_x
	b2=FLOOR(mousy/chars_y)
	//b2=mousy/chars_y
	b80=char_offset+(b2* chars_xmax)+b1
	b1=MOUSB
	if b1>0
	then
		b1=MKEY
		switch b1
			case 256
				// copy char
				charcnt=b80-char_offset
				gosub :copychar
				copied=1
				TICKS 200
				break
			case 512
				// paste char
				if copied>0
				then
					charcnt=b80-char_offset
					if gfx_classic==0
					then
						ACOPY  2 0 3 charcnt*(fontbytes*fonty)  fontbytes*fonty
					else
						ACOPY 2 0 3  charcnt*fontx  fontx
					endif	
					// redraw char
					gosub :redraw_char
					FLUSHSCRN
				endif
				
				break
			case 2048
				// paste char
				if copied>0
				then
					charcnt=b80-char_offset
					if gfx_classic==0
					then
						ACOPY  2 0 3 charcnt*(fontbytes*fonty)  fontbytes*fonty
					else
						ACOPY 2 0 3  charcnt*fontx  fontx
					endif	
					// redraw char
					gosub :redraw_char
					FLUSHSCRN
				endif
				break
			default
				charcnt=b80-char_offset
				if charcnt<numchars
					gosub :edit
			break
		ends
	endif
endif

return

:load
fontpath = SELECT 0 "select epd font c file"
if fontpath==""
	loop :mloop
KILL CONTROLS
STOP GRAPH
CLR SCREEN C_WH6
goto :restart

// must insert a new char at end of table
:addchar
if numchars>=256
then
	MESSAGE "too many chars!"
	loop :restart
endif
// copy space char
// copy 
OUTPUT temppath
endline=0
endline=a1[numchars]

defdelta=0
// lengh of one defintion
defdelta=a1[2]-a1[1]

for line 1 numlines 1
	if line==endline+defdelta-1
	then
		break
	endif
	STRINGTABLE  1 "" ""  line 1 1
	SAVE s1 LF
next

// now add space char
for line a1[1]-1 a1[1]+defdelta-2 1
	STRINGTABLE  1 "" ""  line 1 1
	SAVE s1 LF
next

// copy rest of file
for line endline+defdelta-1 numlines 1
	STRINGTABLE  1 "" ""  line 1 1
	SAVE s1 LF
next

CLOSE OUTPUT

FDELETE fontpath
FCOPY temppath fontpath
FDELETE temppath
KILL CONTROLS
STOP GRAPH
CLR SCREEN C_WH6
loop :restart

:save
// to save back to c source
save_as_path=""
if gfx_classic==0
then
	save_as_path=SELECT 1 "new_epd_font.c"
else
	save_as_path=SELECT 1 "new_gfx_classic_font.c"
endif

if save_as_path==""
	loop :mloop

OUTPUT save_as_path
blinecount=0
lastline=0
if gfx_classic==0
then
	lastline=numchars*fonty-1
else
	lastline=numchars-1
endif

for line 1 numlines 1
	STRINGTABLE  1 "" ""  line 1 1
	b1= INSTR(s1 "0x")
	if b1>=0
	then
		if gfx_classic==0
		then
			// update bytes
			//	0x01, 0x80, 0x00, //        ##
			b1=FLOOR(blinecount*fontbytes)
			//print b1 LF
			s1=TAB
			for b2 1 fontbytes 1
				b3=b1+b2
				if blinecount==lastline
				and b2==fontbytes
				then
					s1=s1+NUMSTRX("0x%02x" a3[b3])
				else
					s1=s1+NUMSTRX("0x%02x," a3[b3])
				endif
			next
			// draw pattern to line comment with # char
			s1=s1+" //"
			for b2 1 fontbytes 1
				b3=b1+b2
				b4=a3[b3]
				shift=0x80
				for b5 1 8 1
					b6=b4&shift
					if b6>0
					then
						s1=s1+"#"
					else
						s1=s1+" "
					endif
					shift=shift>>1
				next
			next
			SAVE s1 LF
			blinecount+=1
		else
			b1=FLOOR(blinecount*fontx)
			s1=TAB
			for b2 1 fontx 1
				b3=b1+b2
				if blinecount==lastline
				and b2==fontx
				then
					s1=s1+NUMSTRX("0x%02x" a3[b3])
				else
					s1=s1+NUMSTRX("0x%02x," a3[b3])
				endif
			next
			SAVE s1 LF
			blinecount+=1
		endif
	else
		// copy other lines
		SAVE s1 LF
	endif
next
CLOSE OUTPUT

loop :mloop

// copy char into array 2
:copychar
if gfx_classic==0
then
	SET ALEN a2 fontbytes*fonty
	ACOPY 3  charcnt*(fontbytes*fonty)  2 0 fontbytes*fonty
else
	SET ALEN a2 fontx
	ACOPY 3  charcnt*fontx  2 0 fontx
endif
return

:redraw_char
gosub :setscale
cxs=fontx*scale+2
cys=fonty*scale+2
b1=FLOOR(charcnt/chars_xmax)
b2=charcnt%chars_xmax
cyp=b1*((scale*fonty)+22)
cxp=b2*((fontx*scale)+2)

// char in a2 
:printchar
byte=0
FRAME cxp cyp cxs cys C_BK C_WH8 0
s1=NUMSTRX("0x%02.0x" charcnt+char_offset)
if tprt>0
	TEXT  s1 cxp cyp+cys 10 C_BK C_WH8 cxs 20 C_BK 0

xcnt=0
ycnt=0
col=0
if gfx_classic==0
then
	// epd fonts
	for ycnt 0 fonty-1 1
		bindex=1
		b1=(ycnt*fontbytes)+bindex
		byte=a2[b1]
		shift=0x80
		for xcnt 0 fontx-1 1
			b1=byte&shift
			if b1>0
			then
				col=C_BK
			else
				col=C_WH8
			endif
			FRAME cxp+1+(xcnt*scale) cyp+1+(ycnt*scale) scale scale col col 0
			shift/=2
			shift=FLOOR(shift)
			if shift==0
			and xcnt<fontx-2
			then
				shift=0x80
				bindex+=1
				b1=(ycnt*fontbytes)+bindex
				byte=a2[b1]
			endif
		next
	next
else
	// x and y reversed
	for xcnt 0 fontx-1 1
		byte=a2[xcnt+1]
		shift=0x80
		for ycnt 0 fonty 1
			b1=byte&shift
			if b1>0
			then
				col=C_BK
			else
				col=C_WH8
			endif
			if ycnt>0
				FRAME cxp+1+(xcnt*scale) cyp+1+(7-ycnt*scale) scale scale col col 0
			shift/=2
		next
	next
endif	
return


// print grid
:grid
b2=cyp
for b1 1 fonty+1 1
	MOVE cxp b2+1
	LINE cxp+(scale*fontx) b2+1 C_BK
	b2+=scale
next

b2=cxp
for b1 1 fontx+1 1
	MOVE b2+1 cyp
	LINE b2+1 cyp+(scale*fonty) C_BK
	b2+=scale
next
return

// edit char
:edit
scale=400/fontx
cxs=scale*fontx
cys=scale*fonty
s1=NUMSTRX("Edit char: 0x%02x" b80)
NEW SCREEN 32768+2+4+8  100 100 cxs cys+50 s1
SCREEN -2

cxs=30
cys=20
cxp=10
cdx=10
ccs=12
cyp=SCRNYS2-cys-10
xButton "<" 3 0
xButton ">" 4 0
xButton "^" 5 0
xButton "v" 6 0

cxs=50
xButton "Import" 7 0
cxp=SCRNXS2-(2*cxs)-20
xButton "Cancel" 1 0
xButton "Save" 2 0

FLUSHSCRN 4
cxp=0
cyp=0
charcnt=b80-char_offset
cxs=scale*fontx
cys=scale*fonty
tprt=0
gosub :copychar
gosub :printchar
gosub :grid
FLUSHSCRN
TICKS 500

// save current char
SET ALEN a5 a2[-1]
a5=a2

:editloop
TIMEOUT=1000
ongoto CONTROLS 1 :cancel 2 :endedit 3 :left 4 :right 5 :up 6 :down 7 :edimport
mousx=MOUSX
mousy=MOUSY
if mousx>0
and mousx<SCRNXS2
and mousy>0
and mousy<SCRNYS2-50
then
	cxp=FLOOR(mousx/scale)
	cyp=FLOOR(mousy/scale)
	b1=MOUSB
	if b1>0
	then
		if gfx_classic==0
		then
			// invert and redraw
			byte=FLOOR(cxp/8)
			bit=0
			bit=FLOOR(cxp%8)
			val=0
			b1=(cyp*fontbytes)+byte+1
			val=a2[b1]
			bit=7-bit
			b2=1<<bit
			val=val^b2
			a2[b1]=val
		else
			b1=cxp+1
			byte=a2[b1]
			bit=FLOOR(cyp%8)
			b2=1<<bit
			byte=byte^b2
			a2[b1]=byte
		endif	
		cxp=0
		cyp=0
		gosub :printchar
		gosub :grid
		FLUSHSCRN
		TICKS 200
	endif
endif

loop :editloop

// shift all bits
:left
if gfx_classic>0
	loop :editloop
for b1 0 fonty-1 1
	val=0
	for b2 0 fontbytes-1 1
		b3=(b1*fontbytes)+b2
		val=val<<8
		val=val|a2[b3+1]
	next
	val=val<<1
	for b2 fontbytes-1 0 -1
		b3=(b1*fontbytes)+b2
		b4=val&0xff
		a2[b3+1]=b4
		val=val>>8
	next
next
goto :endshift

:right
if gfx_classic>0
	loop :editloop
for b1 0 fonty-1 1
	val=0
	for b2 0 fontbytes-1 1
		b3=(b1*fontbytes)+b2
		val=val<<8
		val=val|a2[b3+1]
	next
	val=val>>1
	for b2 fontbytes-1 0 -1
		b3=(b1*fontbytes)+b2
		b4=val&0xff
		a2[b3+1]=b4
		val=val>>8
	next
next
goto :endshift

:up
if gfx_classic>0
	loop :editloop
for b1 0 fonty-2 1
	for b2 0 fontbytes-1 1
		b3=(b1*fontbytes)+b2
		b4=((b1+1)*fontbytes)+b2
		a2[b3+1]=a2[b4+1]
	next
next
for b2 0 fontbytes-1 1
	b3=(fonty-1*fontbytes)+b2
	a2[b3+1]=0
next
goto :endshift

:down
if gfx_classic>0
	loop :editloop
for b1 fonty-2 0 -1
	for b2 0 fontbytes-1 1
		b3=(b1*fontbytes)+b2
		b4=((b1+1)*fontbytes)+b2
		a2[b4+1]=a2[b3+1]
	next
next
for b2 0 fontbytes-1 1
	a2[b2+1]=0
next
goto :endshift

:endshift
cxp=0
cyp=0
gosub :printchar
gosub :grid
FLUSHSCRN
FLUSHSCRN 4
TICKS 200
loop :editloop

// import a picture
:edimport
imppath=""
imppath=SELECT 0 "select picture file"
if imppath ==""
	loop :editloop

WCOPY -2
DRAW PICT imppath 0 0 SCRNXS2 SCRNYS2-50 0 0
FLUSHSCRN 4
FLUSHSCRN
PAUSE 1

// convert picture
// create offscreen
New_OB1
// draw main screen to offscreen
DRAW SCREEN -2 -1 0 SCRNXS2 SCRNYS2
ixs=SCRNXS2
iys=SCRNYS2-50
SET ALEN a20 ixs*iys*3
// read pixels from offscreen buffer
GPIX -1 20 0 0 ixs iys
// switch back to main screen
SCREEN -2
FLUSHSCRN


cval=0
index=0
xindex=0
yindex=0
// transpose to destination pattern
SET ALEN a5 fontx*fonty
for b1 0 iys-1 1
	for b2 0 ixs-2 1
		index=(b1*ixs*3)+(b2*3)
		cval=a20[index+1]+a20[index+2]+a20[index+3]
		xindex=FLOOR(fontx/ixs*b2)
		yindex=FLOOR(fonty/iys*b1)
		b3=yindex*fontx+xindex+1
		if cval<1.0
		then
			a5[b3]=1
		endif
	next
next

// set bitfield pattern
// for pasting into char
SET ALEN a2 fontbytes*fonty
for b1 0 fonty-1 1
	for b2 0 fontx-1 1
		b3=(b1*fontx)+b2+1
		cval=a5[b3]
		if cval>0
		then
			// set bit
			byte=FLOOR(b2/8)
			bit=0
			bit=FLOOR(b2%8)
			val=0
			b4=(b1*fontbytes)+byte+1
			val=a2[b4]
			bit=7-bit
			b5=1<<bit
			val=val|b5
			a2[b4]=val
		endif
	next
next
loop :endshift

:endedit
// copy back
if gfx_classic==0
then
	ACOPY 2 0 3 charcnt*(fontbytes*fonty)  fontbytes*fonty
else
	ACOPY  2 0 3  charcnt*fontx fontx
endif

:cancel
// remove 7 controls
COPT 1 3
COPT 1 3
COPT 1 3
COPT 1 3
COPT 1 3
COPT 1 3
COPT 1 3
SCREEN -1
CLOSE SCREEN 2
gosub :copychar
gosub :redraw_char
return

// convert RGB to 16 bit GFX color
MACRO "RGB2COL16" b10 b11 b12
b10=(b10&0xf8)<<8
b11=(b11&0xf8)<<3
b12=b12>>3
ui16col=0
ui16col=b10|b11|b12
  //return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
ENDM

// convert a picture to 16 bit color c array
:convertpicture
// create offscreen
New_OB1
ixs=40
iys=60
impxp=1
impyp=1
s1=STRTOK(fontpath "." 1) + ".c"
OUTPUT s1

// approximate x size
desired_x= 50
desired_x =FNUM "desired x size?" 100 100 desired_x

PICSIZ fontpath  1 2
scale=FLOOR(b1/desired_x)
if scale<1
	scale=1
ixs=FLOOR(b1/scale)
iys=FLOOR(b2/scale)

// draw the picture
DRAW PICT fontpath impxp impyp ixs iys 0 0
SET ALEN a20 ixs*iys*3
// read pixels from offscreen buffer, value is from 0-1
GPIX -1 20 impxp impyp ixs iys
// switch back to main screen
SCREEN -1

// save c array
s1= NUMSTR("// picture with %02.0f x " ixs) + NUMSTR("%02.0f pixels" iys) +  LF
SAVE s1
s1= NUMSTR("// table size %02.0f bytes " ixs*iys*2) + LF
SAVE s1
SAVE "const uint16_t picture[] = {" LF

for ycnt 0 iys-1 1
	for xcnt 0 ixs-1 1
		b1=(ycnt*ixs*3)+(xcnt*3)
		RGB2COL16 a20[b1+1]*255 a20[b1+2]*255 a20[b1+3]*255
		s1=NUMSTRX("0x%04x" ui16col)
		DEFINE COLOR 100 a20[b1+1]*1000 a20[b1+2]*1000 a20[b1+3]*1000
		CIRCLE xcnt*2 ycnt*2 2 100 100 0 
		SAVE s1 ","
	next
	SAVE LF
next
SAVE "0x0000\n};"
CLOSE OUTPUT

// draw test picture
FLUSHSCRN

//DRAW SCREEN 1
PAUSE 3

END
