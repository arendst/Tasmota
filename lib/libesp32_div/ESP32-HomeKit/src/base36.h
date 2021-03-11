/*
 * Base36 PostgreSQL input/output function for bigint
 *
 * Author: Dimitri Fontaine <dimitri@2ndQuadrant.fr>
 *
 * Taken from https://github.com/dimitri/base36/blob/master/base36.c
 */

#include <stdio.h>
#include <stdlib.h>

#define BASE36_LENGTH      13

typedef long long int base36;

static int base36_digits[36] =
  {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
   'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
   'U', 'V', 'W', 'X', 'Y', 'Z'
  };

static base36 base36_powers[BASE36_LENGTH] =
  {
1ULL,
    36ULL,
    1296ULL,
    46656ULL,
    1679616ULL,
    60466176ULL,
    2176782336ULL,
	78364164096ULL,
	2821109907456ULL,
	101559956668416ULL,
	3656158440062976ULL,
	131621703842267136ULL,
	4738381338321616896ULL
  };

static inline
char *base36_to_str(base36 c)
{
  int i, d, p = 0;
  base36 m = c;
  bool discard = true;
  char *str = calloc((BASE36_LENGTH + 1),  sizeof(char));

  for(i=BASE36_LENGTH-1; i>=0; i--)
  {
    d = m / base36_powers[i];
    m = m - base36_powers[i] * d;

    discard = discard && (d == 0 && i >0);

    if( !discard )
       str[p++] = base36_digits[d];
  }

  return str;
}
