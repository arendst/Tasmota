#include <stdio.h>
#include <string.h>
#include "reciter.h"
#include "ReciterTabs.h"
#include "esp8266sam_debug.h"
#include "SamData.h"

unsigned char A, X, Y;
//extern int debug;

#define inputtemp (samdata->reciter.inputtemp)

void Code37055(unsigned char mem59)
{
	X = mem59;
	X--;
	A = inputtemp[X];
	Y = A;
	A = pgm_read_byte(tab36376+Y); //tab36376[Y];
	return;
}

void Code37066(unsigned char mem58)
{
	X = mem58;
	X++;
	A = inputtemp[X];
	Y = A;
	A = pgm_read_byte(tab36376+Y); //tab36376[Y];
}

unsigned char GetRuleByte(unsigned short mem62, unsigned char Y)
{
	unsigned int address = mem62;

	if (mem62 >= 37541)
	{
		address -= 37541;
		return pgm_read_byte(rules2+address+Y); //rules2[address+Y];
	}
	address -= 32000;
	return pgm_read_byte(rules+address+Y); //rules[address+Y];
}

int TextToPhonemes(char *input) // Code36484
{
	//unsigned char *tab39445 = &mem[39445];   //input and output
	//unsigned char mem29;
	unsigned char mem56;      //output position for phonemes
	unsigned char mem57;
	unsigned char mem58;
	unsigned char mem59;
	unsigned char mem60;
	unsigned char mem61;
	unsigned short mem62;     // memory position of current rule

	unsigned char mem64;      // position of '=' or current character
	unsigned char mem65;     // position of ')'
	unsigned char mem66;     // position of '('
	unsigned char mem36653;

	inputtemp[0] = 32;

	// secure copy of input
	// because input will be overwritten by phonemes
	X = 1;
	Y = 0;
	do
	{
		//pos36499:
		A = input[Y] & 127;
		if ( A >= 112) A = A & 95;
		else if ( A >= 96) A = A & 79;

		inputtemp[X] = A;
		X++;
		Y++;
	} while (Y != 255);


	X = 255;
	inputtemp[X] = 27;
	mem61 = 255;


pos36550:
	A = 255;
	mem56 = 255;


pos36554:
	while(1)
	{
		mem61++;
		X = mem61;
		A = inputtemp[X];
		mem64 = A;
		if (A == '[')
		{
			mem56++;
			X = mem56;
			A = 155;
			input[X] = 155;
			//goto pos36542;
			//			Code39771(); 	//Code39777();
			return 1;
		}

		//pos36579:
		if (A != '.') break;
		X++;
		Y = inputtemp[X];
		A = pgm_read_byte(tab36376+Y)/*tab36376[Y]*/ & 1;
		if(A != 0) break;
		mem56++;
		X = mem56;
		A = '.';
		input[X] = '.';
	} //while


	//pos36607:
	A = mem64;
	Y = A;
	A = pgm_read_byte(tab36376+A); //tab36376[A];
	mem57 = A;
	if((A&2) != 0)
	{
		mem62 = 37541;
		goto pos36700;
	}

	//pos36630:
	A = mem57;
	if(A != 0) goto pos36677;
	A = 32;
	inputtemp[X] = ' ';
	mem56++;
	X = mem56;
	if (X > 120) goto pos36654;
	input[X] = A;
	goto pos36554;

	// -----

	//36653 is unknown. Contains position

pos36654:
	input[X] = 155;
	A = mem61;
	mem36653 = A;
	//	mem29 = A; // not used
	//	Code36538(); das ist eigentlich
	return 1;
	//Code39771();
	//go on if there is more input ???
	mem61 = mem36653;
	goto pos36550;

pos36677:
	A = mem57 & 128;
	if(A == 0)
	{
		//36683: BRK
		return 0;
	}

	// go to the right rules for this character.
	X = mem64 - 'A';
	mem62 = pgm_read_byte(&tab37489[X]) | (pgm_read_byte(&tab37515[X])<<8);

	// -------------------------------------
	// go to next rule
	// -------------------------------------

pos36700:

	// find next rule
	Y = 0;
	do
	{
		mem62 += 1;
		A = GetRuleByte(mem62, Y);
	} while ((A & 128) == 0);
	Y++;

	//pos36720:
	// find '('
	while(1)
	{
		A = GetRuleByte(mem62, Y);
		if (A == '(') break;
		Y++;
	}
	mem66 = Y;

	//pos36732:
	// find ')'
	do
	{
		Y++;
		A = GetRuleByte(mem62, Y);
	} while(A != ')');
	mem65 = Y;

	//pos36741:
	// find '='
	do
	{
		Y++;
		A = GetRuleByte(mem62, Y);
		A = A & 127;
	} while (A != '=');
	mem64 = Y;

	X = mem61;
	mem60 = X;

	// compare the string within the bracket
	Y = mem66;
	Y++;
	//pos36759:
	while(1)
	{
		mem57 = inputtemp[X];
		A = GetRuleByte(mem62, Y);
		if (A != mem57) goto pos36700;
		Y++;
		if(Y == mem65) break;
		X++;
		mem60 = X;
	}

// the string in the bracket is correct

//pos36787:
	A = mem61;
	mem59 = mem61;

pos36791:
	while(1)
	{
		mem66--;
		Y = mem66;
		A = GetRuleByte(mem62, Y);
		mem57 = A;
		//36800: BPL 36805
		if ((A & 128) != 0) goto pos37180;
		X = A & 127;
		A = pgm_read_byte(tab36376+X)/*tab36376[X]*/ & 128;
		if (A == 0) break;
		X = mem59-1;
		A = inputtemp[X];
		if (A != mem57) goto pos36700;
		mem59 = X;
	}

//pos36833:
	A = mem57;
	if (A == ' ') goto pos36895;
	if (A == '#') goto pos36910;
	if (A == '.') goto pos36920;
	if (A == '&') goto pos36935;
	if (A == '@') goto pos36967;
	if (A == '^') goto pos37004;
	if (A == '+') goto pos37019;
	if (A == ':') goto pos37040;
	//	Code42041();    //Error
	//36894: BRK
	return 0;

	// --------------

pos36895:
	Code37055(mem59);
	A = A & 128;
	if(A != 0) goto pos36700;
pos36905:
	mem59 = X;
	goto pos36791;

	// --------------

pos36910:
	Code37055(mem59);
	A = A & 64;
	if(A != 0) goto pos36905;
	goto pos36700;

	// --------------


pos36920:
	Code37055(mem59);
	A = A & 8;
	if(A == 0) goto pos36700;
pos36930:
	mem59 = X;
	goto pos36791;

	// --------------

pos36935:
	Code37055(mem59);
	A = A & 16;
	if(A != 0) goto pos36930;
	A = inputtemp[X];
	if (A != 72) goto pos36700;
	X--;
	A = inputtemp[X];
	if ((A == 67) || (A == 83)) goto pos36930;
	goto pos36700;

	// --------------

pos36967:
	Code37055(mem59);
	A = A & 4;
	if(A != 0) goto pos36930;
	A = inputtemp[X];
	if (A != 72) goto pos36700;
	if ((A != 84) && (A != 67) && (A != 83)) goto pos36700;
	mem59 = X;
	goto pos36791;

	// --------------


pos37004:
	Code37055(mem59);
	A = A & 32;
	if(A == 0) goto pos36700;

pos37014:
	mem59 = X;
	goto pos36791;

	// --------------

pos37019:
	X = mem59;
	X--;
	A = inputtemp[X];
	if ((A == 'E') || (A == 'I') || (A == 'Y')) goto pos37014;
	goto pos36700;
	// --------------

pos37040:
	Code37055(mem59);
	A = A & 32;
	if(A == 0) goto pos36791;
	mem59 = X;
	goto pos37040;

//---------------------------------------


pos37077:
	X = mem58+1;
	A = inputtemp[X];
	if (A != 'E') goto pos37157;
	X++;
	Y = inputtemp[X];
	X--;
	A = pgm_read_byte(tab36376+Y)/*tab36376[Y]*/ & 128;
	if(A == 0) goto pos37108;
	X++;
	A = inputtemp[X];
	if (A != 'R') goto pos37113;
pos37108:
	mem58 = X;
	goto pos37184;
pos37113:
	if ((A == 83) || (A == 68)) goto pos37108;  // 'S' 'D'
	if (A != 76) goto pos37135; // 'L'
	X++;
	A = inputtemp[X];
	if (A != 89) goto pos36700;
	goto pos37108;

pos37135:
	if (A != 70) goto pos36700;
	X++;
	A = inputtemp[X];
	if (A != 85) goto pos36700;
	X++;
	A = inputtemp[X];
	if (A == 76) goto pos37108;
	goto pos36700;

pos37157:
	if (A != 73) goto pos36700;
	X++;
	A = inputtemp[X];
	if (A != 78) goto pos36700;
	X++;
	A = inputtemp[X];
	if (A == 71) goto pos37108;
	//pos37177:
	goto pos36700;

	// -----------------------------------------

pos37180:

	A = mem60;
	mem58 = A;

pos37184:
	Y = mem65 + 1;

	//37187: CPY 64
	//	if(? != 0) goto pos37194;
	if(Y == mem64) goto pos37455;
	mem65 = Y;
	//37196: LDA (62),y
	A = GetRuleByte(mem62, Y);
	mem57 = A;
	X = A;
	A = pgm_read_byte(tab36376+X)/*tab36376[X]*/ & 128;
	if(A == 0) goto pos37226;
	X = mem58+1;
	A = inputtemp[X];
	if (A != mem57) goto pos36700;
	mem58 = X;
	goto pos37184;
pos37226:
	A = mem57;
	if (A == 32) goto pos37295;   // ' '
	if (A == 35) goto pos37310;   // '#'
	if (A == 46) goto pos37320;   // '.'
	if (A == 38) goto pos37335;   // '&'
	if (A == 64) goto pos37367;   // ''
	if (A == 94) goto pos37404;   // ''
	if (A == 43) goto pos37419;   // '+'
	if (A == 58) goto pos37440;   // ':'
	if (A == 37) goto pos37077;   // '%'
	//pos37291:
	//	Code42041(); //Error
	//37294: BRK
	return 0;

	// --------------
pos37295:
	Code37066(mem58);
	A = A & 128;
	if(A != 0) goto pos36700;
pos37305:
	mem58 = X;
	goto pos37184;

	// --------------

pos37310:
	Code37066(mem58);
	A = A & 64;
	if(A != 0) goto pos37305;
	goto pos36700;

	// --------------


pos37320:
	Code37066(mem58);
	A = A & 8;
	if(A == 0) goto pos36700;

pos37330:
	mem58 = X;
	goto pos37184;

	// --------------

pos37335:
	Code37066(mem58);
	A = A & 16;
	if(A != 0) goto pos37330;
	A = inputtemp[X];
	if (A != 72) goto pos36700;
	X++;
	A = inputtemp[X];
	if ((A == 67) || (A == 83)) goto pos37330;
	goto pos36700;

	// --------------


pos37367:
	Code37066(mem58);
	A = A & 4;
	if(A != 0) goto pos37330;
	A = inputtemp[X];
	if (A != 72) goto pos36700;
	if ((A != 84) && (A != 67) && (A != 83)) goto pos36700;
	mem58 = X;
	goto pos37184;

	// --------------

pos37404:
	Code37066(mem58);
	A = A & 32;
	if(A == 0) goto pos36700;
pos37414:
	mem58 = X;
	goto pos37184;

	// --------------

pos37419:
	X = mem58;
	X++;
	A = inputtemp[X];
	if ((A == 69) || (A == 73) || (A == 89)) goto pos37414;
	goto pos36700;

// ----------------------

pos37440:

	Code37066(mem58);
	A = A & 32;
	if(A == 0) goto pos37184;
	mem58 = X;
	goto pos37440;
pos37455:
	Y = mem64;
	mem61 = mem60;

	if (DEBUG_ESP8266SAM_LIB)
		PrintRule(mem62);

pos37461:
	//37461: LDA (62),y
	A = GetRuleByte(mem62, Y);
	mem57 = A;
	A = A & 127;
	if (A != '=')
	{
		mem56++;
		X = mem56;
		input[X] = A;
	}

	//37478: BIT 57
	//37480: BPL 37485  //not negative flag
	if ((mem57 & 128) == 0) goto pos37485; //???
	goto pos36554;
pos37485:
	Y++;
	goto pos37461;
}
