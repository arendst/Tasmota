#include<stdio.h>

extern unsigned char signInputTable1[];
extern unsigned char signInputTable2[];

void PrintPhonemes(unsigned char *phonemeindex, unsigned char *phonemeLength, unsigned char *stress)
{
	int i = 0;
	printf("===========================================\n");

	printf("Internal Phoneme presentation:\n\n");
	printf(" idx    phoneme  length  stress\n");
	printf("------------------------------\n");

	while((phonemeindex[i] != 255) && (i < 255))
	{
		if (phonemeindex[i] < 81)
		{
			printf(" %3i      %c%c      %3i       %i\n",
			phonemeindex[i],
			signInputTable1[phonemeindex[i]],
			signInputTable2[phonemeindex[i]],
			phonemeLength[i],
			stress[i]
			);
		} else
		{
			printf(" %3i      ??      %3i       %i\n", phonemeindex[i], phonemeLength[i], stress[i]);
		}
		i++;
	}
	printf("===========================================\n");
	printf("\n");
}

void PrintOutput(
	unsigned char *flag,
	unsigned char *f1,
	unsigned char *f2,
	unsigned char *f3,
	unsigned char *a1,
	unsigned char *a2,
	unsigned char *a3,
	unsigned char *p)
{
	printf("===========================================\n");
	printf("Final data for speech output:\n\n");
	int i = 0;
	printf(" flags ampl1 freq1 ampl2 freq2 ampl3 freq3 pitch\n");
	printf("------------------------------------------------\n");
	while(i < 255)
	{
		printf("%5i %5i %5i %5i %5i %5i %5i %5i\n", flag[i], a1[i], f1[i], a2[i], f2[i], a3[i], f3[i], p[i]);
		i++;
	}
	printf("===========================================\n");

}

extern unsigned char GetRuleByte(unsigned short mem62, unsigned char Y);

void PrintRule(int offset)
{
	int i = 1;
	unsigned char A = 0;
	printf("Applying rule: ");
	do
	{
		A = GetRuleByte(offset, i);
		if ((A&127) == '=') printf(" -> "); else printf("%c", A&127);
		i++;
	} while ((A&128)==0);
	printf("\n");
}
