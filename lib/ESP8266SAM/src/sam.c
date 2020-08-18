#include <stdio.h> // define printf()
#include <string.h> // strlen()
//#include <stdlib.h>
#include <stddef.h> // define NULL
#include "debug.h"
#include "sam.h"
#include "render.h"
#include "SamTabs.h"
#include "SamData.h"

//standard sam sound
unsigned char speed = 72;
unsigned char pitch = 64;
static unsigned char mouth = 128;
static unsigned char throat = 128;
int singmode = 0;

//extern int debug;

unsigned char mem39;
unsigned char mem44;
unsigned char mem47;
unsigned char mem49;
unsigned char mem50;
unsigned char mem51;
unsigned char mem53;
unsigned char mem56;

unsigned char mem59=0;

static unsigned char A, X, Y;

#define input (samdata->sam.input)
#define stress (samdata->sam.stress)
#define phonemeLength (samdata->sam.phonemeLength)
#define phonemeindex (samdata->sam.phonemeindex)
#define phonemeIndexOutput (samdata->sam.phonemeIndexOutput)
#define stressOutput (samdata->sam.stressOutput)
#define phonemeLengthOutput (samdata->sam.phonemeLengthOutput)



// contains the final soundbuffer
int bufferpos=0;
//char *buffer = NULL;


void SetInput(char *_input)
{
	int i, l;
	l = strlen(_input);
	if (l > 254) l = 254;
	for(i=0; i<l; i++)
		input[i] = _input[i];
	input[l] = 0;
}

void SetSpeed(unsigned char _speed) {speed = _speed;};
void SetPitch(unsigned char _pitch) {pitch = _pitch;};
void SetMouth(unsigned char _mouth) {mouth = _mouth;};
void SetThroat(unsigned char _throat) {throat = _throat;};
void EnableSingmode(int x) {singmode = x;};
//char* GetBuffer(){return buffer;};
int GetBufferLength(){return bufferpos;};

void Init();
int Parser1();
void Parser2();
int SAMMain();
void CopyStress();
void SetPhonemeLength();
void AdjustLengths();
void Code41240();
void Insert(unsigned char position, unsigned char mem60, unsigned char mem59, unsigned char mem58);
void InsertBreath();
void PrepareOutput();
void SetMouthThroat(unsigned char mouth, unsigned char throat);

// 168=pitches
// 169=frequency1
// 170=frequency2
// 171=frequency3
// 172=amplitude1
// 173=amplitude2
// 174=amplitude3


void Init()
{
	int i;
	SetMouthThroat( mouth, throat);

	bufferpos = 0;
	// TODO, check for free the memory, 10 seconds of output should be more than enough
//	buffer = malloc(22050*10);

	/*
	freq2data = &mem[45136];
	freq1data = &mem[45056];
	freq3data = &mem[45216];
	*/
	//pitches = &mem[43008];
	/*
	frequency1 = &mem[43264];
	frequency2 = &mem[43520];
	frequency3 = &mem[43776];
	*/
	/*
	amplitude1 = &mem[44032];
	amplitude2 = &mem[44288];
	amplitude3 = &mem[44544];
	*/
	//phoneme = &mem[39904];
	/*
	ampl1data = &mem[45296];
	ampl2data = &mem[45376];
	ampl3data = &mem[45456];
	*/

	for(i=0; i<256; i++)
	{
		stress[i] = 0;
		phonemeLength[i] = 0;
	}

	for(i=0; i<60; i++)
	{
		phonemeIndexOutput[i] = 0;
		stressOutput[i] = 0;
		phonemeLengthOutput[i] = 0;
	}
	phonemeindex[255] = 255; //to prevent buffer overflow // ML : changed from 32 to 255 to stop freezing with long inputs

}


void (*outcb)(void *, unsigned char) = NULL;
void *outcbdata = NULL;

//int Code39771()
int SAMMain( void (*cb)(void *, unsigned char), void *cbd )
{
  outcb = cb;
  outcbdata = cbd;
	Init();
	phonemeindex[255] = 32; //to prevent buffer overflow

	if (!Parser1()) return 0;
	if (sam_debug)
		PrintPhonemes(phonemeindex, phonemeLength, stress);
	Parser2();
	CopyStress();
	SetPhonemeLength();
	AdjustLengths();
	Code41240();
	do
	{
		A = phonemeindex[X];
		if (A > 80)
		{
			phonemeindex[X] = 255;
			break; // error: delete all behind it
		}
		X++;
	} while (X != 0);

	//pos39848:
	InsertBreath();

	//mem[40158] = 255;
	if (sam_debug)
	{
		PrintPhonemes(phonemeindex, phonemeLength, stress);
	}

	PrepareOutput();

	return 1;
}

int SAMPrepare()
{
  Init();
  phonemeindex[255] = 32; //to prevent buffer overflow

  if (!Parser1()) return 0;
  Parser2();
  CopyStress();
  SetPhonemeLength();
  AdjustLengths();
  Code41240();
  do
  {
    A = phonemeindex[X];
    if (A > 80)
    {
      phonemeindex[X] = 255;
      break; // error: delete all behind it
    }
    X++;
  } while (X != 0);

  InsertBreath();
  return 1;
}



//void Code48547()
void PrepareOutput()
{
	A = 0;
	X = 0;
	Y = 0;

	//pos48551:
	while(1)
	{
		A = phonemeindex[X];
		if (A == 255)
		{
			A = 255;
			phonemeIndexOutput[Y] = 255;
			Render();
			return;
		}
		if (A == 254)
		{
			X++;
			int temp = X;
			//mem[48546] = X;
			phonemeIndexOutput[Y] = 255;
			Render();
			//X = mem[48546];
			X=temp;
			Y = 0;
			continue;
		}

		if (A == 0)
		{
			X++;
			continue;
		}

		phonemeIndexOutput[Y] = A;
		phonemeLengthOutput[Y] = phonemeLength[X];
		stressOutput[Y] = stress[X];
		X++;
		Y++;
	}
}

//void Code48431()
void InsertBreath()
{
	unsigned char mem54;
	unsigned char mem55;
	unsigned char index; //variable Y
	mem54 = 255;
	X++;
	mem55 = 0;
	unsigned char mem66 = 0;
	while(1)
	{
		//pos48440:
		X = mem66;
		index = phonemeindex[X];
		if (index == 255) return;
		mem55 += phonemeLength[X];

		if (mem55 < 232)
		{
			if (index != 254) // ML : Prevents an index out of bounds problem
			{
				A = flags2[index]&1;
				if(A != 0)
				{
					X++;
					mem55 = 0;
					Insert(X, 254, mem59, 0);
					mem66++;
					mem66++;
					continue;
				}
			}
			if (index == 0) mem54 = X;
			mem66++;
			continue;
		}
		X = mem54;
		phonemeindex[X] = 31;   // 'Q*' glottal stop
		phonemeLength[X] = 4;
		stress[X] = 0;
		X++;
		mem55 = 0;
		Insert(X, 254, mem59, 0);
		X++;
		mem66 = X;
	}

}

// Iterates through the phoneme buffer, copying the stress value from
// the following phoneme under the following circumstance:

//     1. The current phoneme is voiced, excluding plosives and fricatives
//     2. The following phoneme is voiced, excluding plosives and fricatives, and
//     3. The following phoneme is stressed
//
//  In those cases, the stress value+1 from the following phoneme is copied.
//
// For example, the word LOITER is represented as LOY5TER, with as stress
// of 5 on the diphtong OY. This routine will copy the stress value of 6 (5+1)
// to the L that precedes it.


//void Code41883()
void CopyStress()
{
    // loop thought all the phonemes to be output
	unsigned char pos=0; //mem66
	while(1)
	{
        // get the phomene
		Y = phonemeindex[pos];

	    // exit at end of buffer
		if (Y == 255) return;

		// if CONSONANT_FLAG set, skip - only vowels get stress
		if ((flags[Y] & 64) == 0) {pos++; continue;}
		// get the next phoneme
		Y = phonemeindex[pos+1];
		if (Y == 255) //prevent buffer overflow
		{
			pos++; continue;
		} else
		// if the following phoneme is a vowel, skip
		if ((flags[Y] & 128) == 0)  {pos++; continue;}

        // get the stress value at the next position
		Y = stress[pos+1];

		// if next phoneme is not stressed, skip
		if (Y == 0)  {pos++; continue;}

		// if next phoneme is not a VOWEL OR ER, skip
		if ((Y & 128) != 0)  {pos++; continue;}

		// copy stress from prior phoneme to this one
		stress[pos] = Y+1;

		// advance pointer
		pos++;
	}

}


//void Code41014()
void Insert(unsigned char position/*var57*/, unsigned char mem60, unsigned char mem59, unsigned char mem58)
{
	int i;
	for(i=253; i >= position; i--) // ML : always keep last safe-guarding 255
	{
		phonemeindex[i+1] = phonemeindex[i];
		phonemeLength[i+1] = phonemeLength[i];
		stress[i+1] = stress[i];
	}

	phonemeindex[position] = mem60;
	phonemeLength[position] = mem59;
	stress[position] = mem58;
	return;
}

// The input[] buffer contains a string of phonemes and stress markers along
// the lines of:
//
//     DHAX KAET IHZ AH5GLIY. <0x9B>
//
// The byte 0x9B marks the end of the buffer. Some phonemes are 2 bytes
// long, such as "DH" and "AX". Others are 1 byte long, such as "T" and "Z".
// There are also stress markers, such as "5" and ".".
//
// The first character of the phonemes are stored in the table signInputTable1[].
// The second character of the phonemes are stored in the table signInputTable2[].
// The stress characters are arranged in low to high stress order in stressInputTable[].
//
// The following process is used to parse the input[] buffer:
//
// Repeat until the <0x9B> character is reached:
//
//        First, a search is made for a 2 character match for phonemes that do not
//        end with the '*' (wildcard) character. On a match, the index of the phoneme
//        is added to phonemeIndex[] and the buffer position is advanced 2 bytes.
//
//        If this fails, a search is made for a 1 character match against all
//        phoneme names ending with a '*' (wildcard). If this succeeds, the
//        phoneme is added to phonemeIndex[] and the buffer position is advanced
//        1 byte.
//
//        If this fails, search for a 1 character match in the stressInputTable[].
//        If this succeeds, the stress value is placed in the last stress[] table
//        at the same index of the last added phoneme, and the buffer position is
//        advanced by 1 byte.
//
//        If this fails, return a 0.
//
// On success:
//
//    1. phonemeIndex[] will contain the index of all the phonemes.
//    2. The last index in phonemeIndex[] will be 255.
//    3. stress[] will contain the stress value for each phoneme

// input[] holds the string of phonemes, each two bytes wide
// signInputTable1[] holds the first character of each phoneme
// signInputTable2[] holds te second character of each phoneme
// phonemeIndex[] holds the indexes of the phonemes after parsing input[]
//
// The parser scans through the input[], finding the names of the phonemes
// by searching signInputTable1[] and signInputTable2[]. On a match, it
// copies the index of the phoneme into the phonemeIndexTable[].
//
// The character <0x9B> marks the end of text in input[]. When it is reached,
// the index 255 is placed at the end of the phonemeIndexTable[], and the
// function returns with a 1 indicating success.
int Parser1()
{
	int i;
	unsigned char sign1;
	unsigned char sign2;
	unsigned char position = 0;
	X = 0;
	A = 0;
	Y = 0;

	// CLEAR THE STRESS TABLE
	for(i=0; i<256; i++)
		stress[i] = 0;

  // THIS CODE MATCHES THE PHONEME LETTERS TO THE TABLE
	// pos41078:
	while(1)
	{
        // GET THE FIRST CHARACTER FROM THE PHONEME BUFFER
		sign1 = input[X];
		// TEST FOR 155 (�) END OF LINE MARKER
		if (sign1 == 155)
		{
           // MARK ENDPOINT AND RETURN
			phonemeindex[position] = 255;      //mark endpoint
			// REACHED END OF PHONEMES, SO EXIT
			return 1;       //all ok
		}

		// GET THE NEXT CHARACTER FROM THE BUFFER
		X++;
		sign2 = input[X];

		// NOW sign1 = FIRST CHARACTER OF PHONEME, AND sign2 = SECOND CHARACTER OF PHONEME

       // TRY TO MATCH PHONEMES ON TWO TWO-CHARACTER NAME
       // IGNORE PHONEMES IN TABLE ENDING WITH WILDCARDS

       // SET INDEX TO 0
		Y = 0;
pos41095:

         // GET FIRST CHARACTER AT POSITION Y IN signInputTable
         // --> should change name to PhonemeNameTable1
		A = pgm_read_byte(signInputTable1+Y);//signInputTable1[Y];

		// FIRST CHARACTER MATCHES?
		if (A == sign1)
		{
           // GET THE CHARACTER FROM THE PhonemeSecondLetterTable
			A = pgm_read_byte(signInputTable2+Y);//signInputTable2[Y];
			// NOT A SPECIAL AND MATCHES SECOND CHARACTER?
			if ((A != '*') && (A == sign2))
			{
               // STORE THE INDEX OF THE PHONEME INTO THE phomeneIndexTable
				phonemeindex[position] = Y;

				// ADVANCE THE POINTER TO THE phonemeIndexTable
				position++;
				// ADVANCE THE POINTER TO THE phonemeInputBuffer
				X++;

				// CONTINUE PARSING
				continue;
			}
		}

		// NO MATCH, TRY TO MATCH ON FIRST CHARACTER TO WILDCARD NAMES (ENDING WITH '*')

		// ADVANCE TO THE NEXT POSITION
		Y++;
		// IF NOT END OF TABLE, CONTINUE
		if (Y != 81) goto pos41095;

// REACHED END OF TABLE WITHOUT AN EXACT (2 CHARACTER) MATCH.
// THIS TIME, SEARCH FOR A 1 CHARACTER MATCH AGAINST THE WILDCARDS

// RESET THE INDEX TO POINT TO THE START OF THE PHONEME NAME TABLE
		Y = 0;
pos41134:
// DOES THE PHONEME IN THE TABLE END WITH '*'?
		if (pgm_read_byte(signInputTable2+Y)/*signInputTable2[Y]*/ == '*')
		{
// DOES THE FIRST CHARACTER MATCH THE FIRST LETTER OF THE PHONEME
			if (pgm_read_byte(signInputTable1+Y)/*]signInputTable1[Y]*/ == sign1)
			{
                // SAVE THE POSITION AND MOVE AHEAD
				phonemeindex[position] = Y;

				// ADVANCE THE POINTER
				position++;

				// CONTINUE THROUGH THE LOOP
				continue;
			}
		}
		Y++;
		if (Y != 81) goto pos41134; //81 is size of PHONEME NAME table

// FAILED TO MATCH WITH A WILDCARD. ASSUME THIS IS A STRESS
// CHARACTER. SEARCH THROUGH THE STRESS TABLE

        // SET INDEX TO POSITION 8 (END OF STRESS TABLE)
		Y = 8;

       // WALK BACK THROUGH TABLE LOOKING FOR A MATCH
		while( (sign1 != pgm_read_byte(stressInputTable+Y)/*stressInputTable[Y]*/) && (Y>0))
		{
  // DECREMENT INDEX
			Y--;
		}

        // REACHED THE END OF THE SEARCH WITHOUT BREAKING OUT OF LOOP?
		if (Y == 0)
		{
			//mem[39444] = X;
			//41181: JSR 42043 //Error
           // FAILED TO MATCH ANYTHING, RETURN 0 ON FAILURE
			return 0;
		}
// SET THE STRESS FOR THE PRIOR PHONEME
		stress[position-1] = Y;
	} //while
}




//change phonemelength depedendent on stress
//void Code41203()
void SetPhonemeLength()
{
	unsigned char A;
	int position = 0;
	while(phonemeindex[position] != 255 )
	{
		A = stress[position];
		//41218: BMI 41229
		if ((A == 0) || ((A&128) != 0))
		{
			phonemeLength[position] = pgm_read_byte(&phonemeLengthTable[phonemeindex[position]]);
		} else
		{
			phonemeLength[position] = pgm_read_byte(&phonemeStressedLengthTable[phonemeindex[position]]);
		}
		position++;
	}
}


void Code41240()
{
	unsigned char pos=0;

	while(phonemeindex[pos] != 255)
	{
		unsigned char index; //register AC
		X = pos;
		index = phonemeindex[pos];
		if ((flags[index]&2) == 0)
		{
			pos++;
			continue;
		} else
		if ((flags[index]&1) == 0)
		{
			Insert(pos+1, index+1, pgm_read_byte(&phonemeLengthTable[index+1]), stress[pos]);
			Insert(pos+2, index+2, pgm_read_byte(&phonemeLengthTable[index+2]), stress[pos]);
			pos += 3;
			continue;
		}

		do
		{
			X++;
			A = phonemeindex[X];
		} while(A==0);

		if (A != 255)
		{
			if ((flags[A] & 8) != 0)  {pos++; continue;}
			if ((A == 36) || (A == 37)) {pos++; continue;} // '/H' '/X'
		}

		Insert(pos+1, index+1, pgm_read_byte(&phonemeLengthTable[index+1]), stress[pos]);
		Insert(pos+2, index+2, pgm_read_byte(&phonemeLengthTable[index+2]), stress[pos]);
		pos += 3;
	};

}

// Rewrites the phonemes using the following rules:
//
//       <DIPHTONG ENDING WITH WX> -> <DIPHTONG ENDING WITH WX> WX
//       <DIPHTONG NOT ENDING WITH WX> -> <DIPHTONG NOT ENDING WITH WX> YX
//       UL -> AX L
//       UM -> AX M
//       <STRESSED VOWEL> <SILENCE> <STRESSED VOWEL> -> <STRESSED VOWEL> <SILENCE> Q <VOWEL>
//       T R -> CH R
//       D R -> J R
//       <VOWEL> R -> <VOWEL> RX
//       <VOWEL> L -> <VOWEL> LX
//       G S -> G Z
//       K <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> KX <VOWEL OR DIPHTONG NOT ENDING WITH IY>
//       G <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> GX <VOWEL OR DIPHTONG NOT ENDING WITH IY>
//       S P -> S B
//       S T -> S D
//       S K -> S G
//       S KX -> S GX
//       <ALVEOLAR> UW -> <ALVEOLAR> UX
//       CH -> CH CH' (CH requires two phonemes to represent it)
//       J -> J J' (J requires two phonemes to represent it)
//       <UNSTRESSED VOWEL> T <PAUSE> -> <UNSTRESSED VOWEL> DX <PAUSE>
//       <UNSTRESSED VOWEL> D <PAUSE>  -> <UNSTRESSED VOWEL> DX <PAUSE>


//void Code41397()
void Parser2()
{
	if (sam_debug) printf("Parser2\n");
	unsigned char pos = 0; //mem66;
	unsigned char mem58 = 0;


  // Loop through phonemes
	while(1)
	{
// SET X TO THE CURRENT POSITION
		X = pos;
// GET THE PHONEME AT THE CURRENT POSITION
		A = phonemeindex[pos];

// DEBUG: Print phoneme and index
		if (sam_debug && A != 255) printf("%d: %c%c\n", X, signInputTable1[A], signInputTable2[A]);

// Is phoneme pause?
		if (A == 0)
		{
// Move ahead to the
			pos++;
			continue;
		}

// If end of phonemes flag reached, exit routine
		if (A == 255) return;

// Copy the current phoneme index to Y
		Y = A;

// RULE:
//       <DIPHTONG ENDING WITH WX> -> <DIPHTONG ENDING WITH WX> WX
//       <DIPHTONG NOT ENDING WITH WX> -> <DIPHTONG NOT ENDING WITH WX> YX
// Example: OIL, COW


// Check for DIPHTONG
		if ((flags[A] & 16) == 0) goto pos41457;

// Not a diphthong. Get the stress
		mem58 = stress[pos];

// End in IY sound?
		A = flags[Y] & 32;

// If ends with IY, use YX, else use WX
		if (A == 0) A = 20; else A = 21;    // 'WX' = 20 'YX' = 21
		//pos41443:
// Insert at WX or YX following, copying the stress

		if (sam_debug) if (A==20) printf("RULE: insert WX following diphtong NOT ending in IY sound\n");
		if (sam_debug) if (A==21) printf("RULE: insert YX following diphtong ending in IY sound\n");
		Insert(pos+1, A, mem59, mem58);
		X = pos;
// Jump to ???
		goto pos41749;



pos41457:

// RULE:
//       UL -> AX L
// Example: MEDDLE

// Get phoneme
		A = phonemeindex[X];
// Skip this rule if phoneme is not UL
		if (A != 78) goto pos41487;  // 'UL'
		A = 24;         // 'L'                 //change 'UL' to 'AX L'

		if (sam_debug) printf("RULE: UL -> AX L\n");

pos41466:
// Get current phoneme stress
		mem58 = stress[X];

// Change UL to AX
		phonemeindex[X] = 13;  // 'AX'
// Perform insert. Note code below may jump up here with different values
		Insert(X+1, A, mem59, mem58);
		pos++;
// Move to next phoneme
		continue;

pos41487:

// RULE:
//       UM -> AX M
// Example: ASTRONOMY

// Skip rule if phoneme != UM
		if (A != 79) goto pos41495;   // 'UM'
		// Jump up to branch - replaces current phoneme with AX and continues
		A = 27; // 'M'  //change 'UM' to  'AX M'
		if (sam_debug) printf("RULE: UM -> AX M\n");
		goto pos41466;
pos41495:

// RULE:
//       UN -> AX N
// Example: FUNCTION


// Skip rule if phoneme != UN
		if (A != 80) goto pos41503; // 'UN'

		// Jump up to branch - replaces current phoneme with AX and continues
		A = 28;         // 'N' //change UN to 'AX N'
		if (sam_debug) printf("RULE: UN -> AX N\n");
		goto pos41466;
pos41503:

// RULE:
//       <STRESSED VOWEL> <SILENCE> <STRESSED VOWEL> -> <STRESSED VOWEL> <SILENCE> Q <VOWEL>
// EXAMPLE: AWAY EIGHT

		Y = A;
// VOWEL set?
		A = flags[A] & 128;

// Skip if not a vowel
		if (A != 0)
		{
// Get the stress
			A = stress[X];

// If stressed...
			if (A != 0)
			{
// Get the following phoneme
				X++;
				A = phonemeindex[X];
// If following phoneme is a pause

				if (A == 0)
				{
// Get the phoneme following pause
					X++;
					Y = phonemeindex[X];

// Check for end of buffer flag
					if (Y == 255) //buffer overflow
// ??? Not sure about these flags
     					A = 65&128;
					else
// And VOWEL flag to current phoneme's flags
     					A = flags[Y] & 128;

// If following phonemes is not a pause
					if (A != 0)
					{
// If the following phoneme is not stressed
						A = stress[X];
						if (A != 0)
						{
// Insert a glottal stop and move forward
							if (sam_debug) printf("RULE: Insert glottal stop between two stressed vowels with space between them\n");
							// 31 = 'Q'
							Insert(X, 31, mem59, 0);
							pos++;
							continue;
						}
					}
				}
			}
		}


// RULES FOR PHONEMES BEFORE R
//        T R -> CH R
// Example: TRACK


// Get current position and phoneme
		X = pos;
		A = phonemeindex[pos];
		if (A != 23) goto pos41611;     // 'R'

// Look at prior phoneme
		X--;
		A = phonemeindex[pos-1];
		//pos41567:
		if (A == 69)                    // 'T'
		{
// Change T to CH
			if (sam_debug) printf("RULE: T R -> CH R\n");
			phonemeindex[pos-1] = 42;
			goto pos41779;
		}


// RULES FOR PHONEMES BEFORE R
//        D R -> J R
// Example: DRY

// Prior phonemes D?
		if (A == 57)                    // 'D'
		{
// Change D to J
			phonemeindex[pos-1] = 44;
			if (sam_debug) printf("RULE: D R -> J R\n");
			goto pos41788;
		}

// RULES FOR PHONEMES BEFORE R
//        <VOWEL> R -> <VOWEL> RX
// Example: ART


// If vowel flag is set change R to RX
		A = flags[A] & 128;
		if (sam_debug) printf("RULE: R -> RX\n");
		if (A != 0) phonemeindex[pos] = 18;  // 'RX'

// continue to next phoneme
		pos++;
		continue;

pos41611:

// RULE:
//       <VOWEL> L -> <VOWEL> LX
// Example: ALL

// Is phoneme L?
		if (A == 24)    // 'L'
		{
// If prior phoneme does not have VOWEL flag set, move to next phoneme
			if ((flags[phonemeindex[pos-1]] & 128) == 0) {pos++; continue;}
// Prior phoneme has VOWEL flag set, so change L to LX and move to next phoneme
			if (sam_debug) printf("RULE: <VOWEL> L -> <VOWEL> LX\n");
			phonemeindex[X] = 19;     // 'LX'
			pos++;
			continue;
		}

// RULE:
//       G S -> G Z
//
// Can't get to fire -
//       1. The G -> GX rule intervenes
//       2. Reciter already replaces GS -> GZ

// Is current phoneme S?
		if (A == 32)    // 'S'
		{
// If prior phoneme is not G, move to next phoneme
			if (phonemeindex[pos-1] != 60) {pos++; continue;}
// Replace S with Z and move on
			if (sam_debug) printf("RULE: G S -> G Z\n");
			phonemeindex[pos] = 38;    // 'Z'
			pos++;
			continue;
		}

// RULE:
//             K <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> KX <VOWEL OR DIPHTONG NOT ENDING WITH IY>
// Example: COW

// Is current phoneme K?
		if (A == 72)    // 'K'
		{
// Get next phoneme
			Y = phonemeindex[pos+1];
// If at end, replace current phoneme with KX
			if (Y == 255) phonemeindex[pos] = 75; // ML : prevents an index out of bounds problem
			else
			{
// VOWELS AND DIPHTONGS ENDING WITH IY SOUND flag set?
				A = flags[Y] & 32;
				if (sam_debug) if (A==0) printf("RULE: K <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> KX <VOWEL OR DIPHTONG NOT ENDING WITH IY>\n");
// Replace with KX
				if (A == 0) phonemeindex[pos] = 75;  // 'KX'
			}
		}
		else

// RULE:
//             G <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> GX <VOWEL OR DIPHTONG NOT ENDING WITH IY>
// Example: GO


// Is character a G?
		if (A == 60)   // 'G'
		{
// Get the following character
			unsigned char index = phonemeindex[pos+1];

// At end of buffer?
			if (index == 255) //prevent buffer overflow
			{
				pos++; continue;
			}
			else
// If diphtong ending with YX, move continue processing next phoneme
			if ((flags[index] & 32) != 0) {pos++; continue;}
// replace G with GX and continue processing next phoneme
			if (sam_debug) printf("RULE: G <VOWEL OR DIPHTONG NOT ENDING WITH IY> -> GX <VOWEL OR DIPHTONG NOT ENDING WITH IY>\n");
			phonemeindex[pos] = 63; // 'GX'
			pos++;
			continue;
		}

// RULE:
//      S P -> S B
//      S T -> S D
//      S K -> S G
//      S KX -> S GX
// Examples: SPY, STY, SKY, SCOWL

		Y = phonemeindex[pos];
		//pos41719:
// Replace with softer version?
		A = flags[Y] & 1;
		if (A == 0) goto pos41749;
		A = phonemeindex[pos-1];
		if (A != 32)    // 'S'
		{
			A = Y;
			goto pos41812;
		}
		// Replace with softer version
		if (sam_debug) printf("RULE: S* %c%c -> S* %c%c\n", signInputTable1[Y], signInputTable2[Y],signInputTable1[Y-12], signInputTable2[Y-12]);
		phonemeindex[pos] = Y-12;
		pos++;
		continue;


pos41749:

// RULE:
//      <ALVEOLAR> UW -> <ALVEOLAR> UX
//
// Example: NEW, DEW, SUE, ZOO, THOO, TOO

//       UW -> UX

		A = phonemeindex[X];
		if (A == 53)    // 'UW'
		{
// ALVEOLAR flag set?
			Y = phonemeindex[X-1];
			A = flags2[Y] & 4;
// If not set, continue processing next phoneme
			if (A == 0) {pos++; continue;}
			if (sam_debug) printf("RULE: <ALVEOLAR> UW -> <ALVEOLAR> UX\n");
			phonemeindex[X] = 16;
			pos++;
			continue;
		}
pos41779:

// RULE:
//       CH -> CH CH' (CH requires two phonemes to represent it)
// Example: CHEW

		if (A == 42)    // 'CH'
		{
			//        pos41783:
			if (sam_debug) printf("CH -> CH CH+1\n");
			Insert(X+1, A+1, mem59, stress[X]);
			pos++;
			continue;
		}

pos41788:

// RULE:
//       J -> J J' (J requires two phonemes to represent it)
// Example: JAY


		if (A == 44) // 'J'
		{
			if (sam_debug) printf("J -> J J+1\n");
			Insert(X+1, A+1, mem59, stress[X]);
			pos++;
			continue;
		}

// Jump here to continue
pos41812:

// RULE: Soften T following vowel
// NOTE: This rule fails for cases such as "ODD"
//       <UNSTRESSED VOWEL> T <PAUSE> -> <UNSTRESSED VOWEL> DX <PAUSE>
//       <UNSTRESSED VOWEL> D <PAUSE>  -> <UNSTRESSED VOWEL> DX <PAUSE>
// Example: PARTY, TARDY


// Past this point, only process if phoneme is T or D

		if (A != 69)    // 'T'
		if (A != 57) {pos++; continue;}       // 'D'
		//pos41825:


// If prior phoneme is not a vowel, continue processing phonemes
		if ((flags[phonemeindex[X-1]] & 128) == 0) {pos++; continue;}

// Get next phoneme
		X++;
		A = phonemeindex[X];
		//pos41841
// Is the next phoneme a pause?
		if (A != 0)
		{
// If next phoneme is not a pause, continue processing phonemes
			if ((flags[A] & 128) == 0) {pos++; continue;}
// If next phoneme is stressed, continue processing phonemes
// FIXME: How does a pause get stressed?
			if (stress[X] != 0) {pos++; continue;}
//pos41856:
// Set phonemes to DX
		if (sam_debug) printf("RULE: Soften T or D following vowel or ER and preceding a pause -> DX\n");
		phonemeindex[pos] = 30;       // 'DX'
		} else
		{
			A = phonemeindex[X+1];
			if (A == 255) //prevent buffer overflow
				A = 65 & 128;
			else
// Is next phoneme a vowel or ER?
				A = flags[A] & 128;
			if (sam_debug) if (A != 0) printf("RULE: Soften T or D following vowel or ER and preceding a pause -> DX\n");
			if (A != 0) phonemeindex[pos] = 30;  // 'DX'
		}

		pos++;

	} // while
}


// Applies various rules that adjust the lengths of phonemes
//
//         Lengthen <FRICATIVE> or <VOICED> between <VOWEL> and <PUNCTUATION> by 1.5
//         <VOWEL> <RX | LX> <CONSONANT> - decrease <VOWEL> length by 1
//         <VOWEL> <UNVOICED PLOSIVE> - decrease vowel by 1/8th
//         <VOWEL> <UNVOICED CONSONANT> - increase vowel by 1/2 + 1
//         <NASAL> <STOP CONSONANT> - set nasal = 5, consonant = 6
//         <VOICED STOP CONSONANT> {optional silence} <STOP CONSONANT> - shorten both to 1/2 + 1
//         <LIQUID CONSONANT> <DIPHTONG> - decrease by 2


//void Code48619()
void AdjustLengths()
{

    // LENGTHEN VOWELS PRECEDING PUNCTUATION
    //
    // Search for punctuation. If found, back up to the first vowel, then
    // process all phonemes between there and up to (but not including) the punctuation.
    // If any phoneme is found that is a either a fricative or voiced, the duration is
    // increased by (length * 1.5) + 1

    // loop index
	X = 0;
	unsigned char index;

    // iterate through the phoneme list
	unsigned char loopIndex=0;
	while(1)
	{
        // get a phoneme
		index = phonemeindex[X];

		// exit loop if end on buffer token
		if (index == 255) break;

		// not punctuation?
		if((flags2[index] & 1) == 0)
		{
            // skip
			X++;
			continue;
		}

		// hold index
		loopIndex = X;

		// Loop backwards from this point
pos48644:

        // back up one phoneme
		X--;

		// stop once the beginning is reached
		if(X == 0) break;

		// get the preceding phoneme
		index = phonemeindex[X];

		if (index != 255) //inserted to prevent access overrun
		if((flags[index] & 128) == 0) goto pos48644; // if not a vowel, continue looping

		//pos48657:
		do
		{
            // test for vowel
			index = phonemeindex[X];

			if (index != 255)//inserted to prevent access overrun
			// test for fricative/unvoiced or not voiced
			if(((flags2[index] & 32) == 0) || ((flags[index] & 4) != 0))     //nochmal �berpr�fen
			{
				//A = flags[Y] & 4;
				//if(A == 0) goto pos48688;

                // get the phoneme length
				A = phonemeLength[X];

				// change phoneme length to (length * 1.5) + 1
				A = (A >> 1) + A + 1;
if (sam_debug) printf("RULE: Lengthen <FRICATIVE> or <VOICED> between <VOWEL> and <PUNCTUATION> by 1.5\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);

				phonemeLength[X] = A;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);

			}
            // keep moving forward
			X++;
		} while (X != loopIndex);
		//	if (X != loopIndex) goto pos48657;
		X++;
	}  // while

    // Similar to the above routine, but shorten vowels under some circumstances

    // Loop throught all phonemes
	loopIndex = 0;
	//pos48697

	while(1)
	{
        // get a phoneme
		X = loopIndex;
		index = phonemeindex[X];

		// exit routine at end token
		if (index == 255) return;

		// vowel?
		A = flags[index] & 128;
		if (A != 0)
		{
            // get next phoneme
			X++;
			index = phonemeindex[X];

			// get flags
			if (index == 255)
			mem56 = 65; // use if end marker
			else
			mem56 = flags[index];

            // not a consonant
			if ((flags[index] & 64) == 0)
			{
                // RX or LX?
				if ((index == 18) || (index == 19))  // 'RX' & 'LX'
				{
                    // get the next phoneme
					X++;
					index = phonemeindex[X];

					// next phoneme a consonant?
					if ((flags[index] & 64) != 0) {
                        // RULE: <VOWEL> RX | LX <CONSONANT>


if (sam_debug) printf("RULE: <VOWEL> <RX | LX> <CONSONANT> - decrease length by 1\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", loopIndex, signInputTable1[phonemeindex[loopIndex]], signInputTable2[phonemeindex[loopIndex]], phonemeLength[loopIndex]);

                        // decrease length of vowel by 1 frame
    					phonemeLength[loopIndex]--;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", loopIndex, signInputTable1[phonemeindex[loopIndex]], signInputTable2[phonemeindex[loopIndex]], phonemeLength[loopIndex]);

                    }
                    // move ahead
					loopIndex++;
					continue;
				}
				// move ahead
				loopIndex++;
				continue;
			}


			// Got here if not <VOWEL>

            // not voiced
			if ((mem56 & 4) == 0)
			{

                 // Unvoiced
                 // *, .*, ?*, ,*, -*, DX, S*, SH, F*, TH, /H, /X, CH, P*, T*, K*, KX

                // not an unvoiced plosive?
				if((mem56 & 1) == 0) {
                    // move ahead
                    loopIndex++;
                    continue;
                }

                // P*, T*, K*, KX


                // RULE: <VOWEL> <UNVOICED PLOSIVE>
                // <VOWEL> <P*, T*, K*, KX>

                // move back
				X--;

if (sam_debug) printf("RULE: <VOWEL> <UNVOICED PLOSIVE> - decrease vowel by 1/8th\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]],  phonemeLength[X]);

                // decrease length by 1/8th
				mem56 = phonemeLength[X] >> 3;
				phonemeLength[X] -= mem56;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);

                // move ahead
				loopIndex++;
				continue;
			}

            // RULE: <VOWEL> <VOICED CONSONANT>
            // <VOWEL> <WH, R*, L*, W*, Y*, M*, N*, NX, DX, Q*, Z*, ZH, V*, DH, J*, B*, D*, G*, GX>

if (sam_debug) printf("RULE: <VOWEL> <VOICED CONSONANT> - increase vowel by 1/2 + 1\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X-1, signInputTable1[phonemeindex[X-1]], signInputTable2[phonemeindex[X-1]],  phonemeLength[X-1]);

            // decrease length
			A = phonemeLength[X-1];
			phonemeLength[X-1] = (A >> 2) + A + 1;     // 5/4*A + 1

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X-1, signInputTable1[phonemeindex[X-1]], signInputTable2[phonemeindex[X-1]], phonemeLength[X-1]);

            // move ahead
			loopIndex++;
			continue;

		}


        // WH, R*, L*, W*, Y*, M*, N*, NX, Q*, Z*, ZH, V*, DH, J*, B*, D*, G*, GX

//pos48821:

        // RULE: <NASAL> <STOP CONSONANT>
        //       Set punctuation length to 6
        //       Set stop consonant length to 5

        // nasal?
        if((flags2[index] & 8) != 0)
        {

            // M*, N*, NX,

            // get the next phoneme
            X++;
            index = phonemeindex[X];

            // end of buffer?
            if (index == 255)
               A = 65&2;  //prevent buffer overflow
            else
                A = flags[index] & 2; // check for stop consonant


            // is next phoneme a stop consonant?
            if (A != 0)

               // B*, D*, G*, GX, P*, T*, K*, KX

            {
if (sam_debug) printf("RULE: <NASAL> <STOP CONSONANT> - set nasal = 5, consonant = 6\n");
if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X-1, signInputTable1[phonemeindex[X-1]], signInputTable2[phonemeindex[X-1]], phonemeLength[X-1]);

                // set stop consonant length to 6
                phonemeLength[X] = 6;

                // set nasal length to 5
                phonemeLength[X-1] = 5;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X-1, signInputTable1[phonemeindex[X-1]], signInputTable2[phonemeindex[X-1]], phonemeLength[X-1]);

            }
            // move to next phoneme
            loopIndex++;
            continue;
        }


        // WH, R*, L*, W*, Y*, Q*, Z*, ZH, V*, DH, J*, B*, D*, G*, GX

        // RULE: <VOICED STOP CONSONANT> {optional silence} <STOP CONSONANT>
        //       Shorten both to (length/2 + 1)

        // (voiced) stop consonant?
        if((flags[index] & 2) != 0)
        {
            // B*, D*, G*, GX

            // move past silence
            do
            {
                // move ahead
                X++;
                index = phonemeindex[X];
            } while(index == 0);


            // check for end of buffer
            if (index == 255) //buffer overflow
            {
                // ignore, overflow code
                if ((65 & 2) == 0) {loopIndex++; continue;}
            } else if ((flags[index] & 2) == 0) {
                // if another stop consonant, move ahead
                loopIndex++;
                continue;
            }

            // RULE: <UNVOICED STOP CONSONANT> {optional silence} <STOP CONSONANT>
if (sam_debug) printf("RULE: <UNVOICED STOP CONSONANT> {optional silence} <STOP CONSONANT> - shorten both to 1/2 + 1\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X-1, signInputTable1[phonemeindex[X-1]], signInputTable2[phonemeindex[X-1]], phonemeLength[X-1]);
// X gets overwritten, so hold prior X value for debug statement
int debugX = X;
            // shorten the prior phoneme length to (length/2 + 1)
            phonemeLength[X] = (phonemeLength[X] >> 1) + 1;
            X = loopIndex;

            // also shorten this phoneme length to (length/2 +1)
            phonemeLength[loopIndex] = (phonemeLength[loopIndex] >> 1) + 1;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", debugX, signInputTable1[phonemeindex[debugX]], signInputTable2[phonemeindex[debugX]], phonemeLength[debugX]);
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", debugX-1, signInputTable1[phonemeindex[debugX-1]], signInputTable2[phonemeindex[debugX-1]], phonemeLength[debugX-1]);


            // move ahead
            loopIndex++;
            continue;
        }


        // WH, R*, L*, W*, Y*, Q*, Z*, ZH, V*, DH, J*, **,

        // RULE: <VOICED NON-VOWEL> <DIPHTONG>
        //       Decrease <DIPHTONG> by 2

        // liquic consonant?
        if ((flags2[index] & 16) != 0)
        {
            // R*, L*, W*, Y*

            // get the prior phoneme
            index = phonemeindex[X-1];

            // prior phoneme a stop consonant>
            if((flags[index] & 2) != 0)
                             // Rule: <LIQUID CONSONANT> <DIPHTONG>

if (sam_debug) printf("RULE: <LIQUID CONSONANT> <DIPHTONG> - decrease by 2\n");
if (sam_debug) printf("PRE\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);

             // decrease the phoneme length by 2 frames (20 ms)
             phonemeLength[X] -= 2;

if (sam_debug) printf("POST\n");
if (sam_debug) printf("phoneme %d (%c%c) length %d\n", X, signInputTable1[phonemeindex[X]], signInputTable2[phonemeindex[X]], phonemeLength[X]);
         }

         // move to next phoneme
         loopIndex++;
         continue;
    }
//            goto pos48701;
}

// -------------------------------------------------------------------------
// ML : Code47503 is division with remainder, and mem50 gets the sign
void Code47503(unsigned char mem52)
{

	Y = 0;
	if ((mem53 & 128) != 0)
	{
		mem53 = -mem53;
		Y = 128;
	}
	mem50 = Y;
	A = 0;
	for(X=8; X > 0; X--)
	{
		int temp = mem53;
		mem53 = mem53 << 1;
		A = A << 1;
		if (temp >= 128) A++;
		if (A >= mem52)
		{
			A = A - mem52;
			mem53++;
		}
	}

	mem51 = A;
	if ((mem50 & 128) != 0) mem53 = -mem53;

}
