/*
  support_float.ino - Small floating point support for Tasmota

  Copyright (C) 2020  Theo Arends and Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//#ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// Functions not available in 2.3.0

float fmodf(float x, float y)
{
  // https://github.com/micropython/micropython/blob/master/lib/libm/fmodf.c
  union {float f; uint32_t i;} ux = {x}, uy = {y};
  int ex = ux.i>>23 & 0xff;
  int ey = uy.i>>23 & 0xff;
  uint32_t sx = ux.i & 0x80000000;
  uint32_t i;
  uint32_t uxi = ux.i;

  if (uy.i<<1 == 0 || isnan(y) || ex == 0xff)
    return (x*y)/(x*y);
  if (uxi<<1 <= uy.i<<1) {
    if (uxi<<1 == uy.i<<1)
      return 0*x;
    return x;
  }

  // normalize x and y
  if (!ex) {
    for (i = uxi<<9; i>>31 == 0; ex--, i <<= 1);
    uxi <<= -ex + 1;
  } else {
    uxi &= -1U >> 9;
    uxi |= 1U << 23;
  }
  if (!ey) {
    for (i = uy.i<<9; i>>31 == 0; ey--, i <<= 1);
    uy.i <<= -ey + 1;
  } else {
    uy.i &= -1U >> 9;
    uy.i |= 1U << 23;
  }

  // x mod y
  for (; ex > ey; ex--) {
    i = uxi - uy.i;
    if (i >> 31 == 0) {
      if (i == 0)
        return 0*x;
      uxi = i;
    }
    uxi <<= 1;
  }
  i = uxi - uy.i;
  if (i >> 31 == 0) {
    if (i == 0)
      return 0*x;
    uxi = i;
  }
  for (; uxi>>23 == 0; uxi <<= 1, ex--);

  // scale result up
  if (ex > 0) {
    uxi -= 1U << 23;
    uxi |= (uint32_t)ex << 23;
  } else {
    uxi >>= -ex + 1;
  }
  uxi |= sx;
  ux.i = uxi;
  return ux.f;
}
//#endif  // ARDUINO_ESP8266_RELEASE_2_3_0

double FastPrecisePow(double a, double b)
{
  // https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
  // calculate approximation with fraction of the exponent
  int e = abs((int)b);
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }
  return r * u.d;
}

float FastPrecisePowf(const float x, const float y)
{
//  return (float)(pow((double)x, (double)y));
  return (float)FastPrecisePow(x, y);
}

double TaylorLog(double x)
{
  // https://stackoverflow.com/questions/46879166/finding-the-natural-logarithm-of-a-number-using-taylor-series-in-c

  if (x <= 0.0) { return NAN; }
  double z = (x + 1) / (x - 1);                              // We start from power -1, to make sure we get the right power in each iteration;
  double step = ((x - 1) * (x - 1)) / ((x + 1) * (x + 1));   // Store step to not have to calculate it each time
  double totalValue = 0;
  double powe = 1;
  for (uint32_t count = 0; count < 10; count++) {            // Experimental number of 10 iterations
    z *= step;
    double y = (1 / powe) * z;
    totalValue = totalValue + y;
    powe = powe + 2;
  }
  totalValue *= 2;
/*
  char logxs[33];
  dtostrfd(x, 8, logxs);
  double log1 = log(x);
  char log1s[33];
  dtostrfd(log1, 8, log1s);
  char log2s[33];
  dtostrfd(totalValue, 8, log2s);
  AddLog_P2(LOG_LEVEL_DEBUG, PSTR("input %s, log %s, taylor %s"), logxs, log1s, log2s);
*/
  return totalValue;
}

// Following code adapted from: http://www.ganssle.com/approx.htm
// ==============================================================
// The following code implements approximations to various trig functions.
//
// This is demo code to guide developers in implementing their own approximation
// software. This code is merely meant to illustrate algorithms.

inline float sinf(float x) { return sin_52(x); }
inline float cosf(float x) { return cos_52(x); }
inline float tanf(float x) { return tan_56(x); }
inline float atanf(float x) { return atan_66(x); }
inline float asinf(float x) { return asinf1(x); }
inline float acosf(float x) { return acosf1(x); }
inline float sqrtf(float x) { return sqrt1(x); }
inline float powf(float x, float y) { return FastPrecisePow(x, y); }

// Math constants we'll use
double const f_pi           = 3.1415926535897932384626433;  // f_pi
double const f_twopi        = 2.0 * f_pi;                   // f_pi times 2
double const f_two_over_pi  = 2.0 / f_pi;                   // 2/f_pi
double const f_halfpi       = f_pi / 2.0;                   // f_pi divided by 2
double const f_threehalfpi  = 3.0 * f_pi / 2.0;             // f_pi times 3/2, used in tan routines
double const f_four_over_pi = 4.0 / f_pi;                   // 4/f_pi, used in tan routines
double const f_qtrpi        = f_pi / 4.0;                   // f_pi/4.0, used in tan routines
double const f_sixthpi      = f_pi / 6.0;                   // f_pi/6.0, used in atan routines
double const f_tansixthpi   = tan(f_sixthpi);               // tan(f_pi/6), used in atan routines
double const f_twelfthpi    = f_pi / 12.0;                  // f_pi/12.0, used in atan routines
double const f_tantwelfthpi = tan(f_twelfthpi);             // tan(f_pi/12), used in atan routines
float  const f_180pi        = 180 / f_pi;                   // 180 / pi for angles in degrees

// *******************************************************************
// ***
// ***  Routines to compute sine and cosine to 5.2 digits of accuracy.
// ***
// *******************************************************************
//
//		cos_52s computes cosine (x)
//
//  Accurate to about 5.2 decimal digits over the range [0, f_pi/2].
//  The input argument is in radians.
//
//  Algorithm:
//		cos(x)= c1 + c2*x**2 + c3*x**4 + c4*x**6
//   which is the same as:
//		cos(x)= c1 + x**2(c2 + c3*x**2 + c4*x**4)
//		cos(x)= c1 + x**2(c2 + x**2(c3 + c4*x**2))
//
float cos_52s(float x)
{
  const float c1 =  0.9999932946;
  const float c2 = -0.4999124376;
  const float c3 =  0.0414877472;
  const float c4 = -0.0012712095;

  float x2 = x * x;  // The input argument squared
  return (c1 + x2 * (c2 + x2 * (c3 + c4 * x2)));
}
//
// This is the main cosine approximation "driver"
// It reduces the input argument's range to [0, f_pi/2],
// and then calls the approximator.
// See the notes for an explanation of the range reduction.
//
float cos_52(float x)
{
  x = fmodf(x, f_twopi);                     // Get rid of values > 2* f_pi
  if (x < 0) { x = -x; }                     // cos(-x) = cos(x)
  int quad = int(x * (float)f_two_over_pi);  // Get quadrant # (0 to 3) we're in
  switch (quad) {
    case 0: return  cos_52s(x);
    case 1: return -cos_52s((float)f_pi - x);
    case 2: return -cos_52s(x-(float)f_pi);
    case 3: return  cos_52s((float)f_twopi - x);
  }
}
//
// The sine is just cosine shifted a half-f_pi, so
// we'll adjust the argument and call the cosine approximation.
//
float sin_52(float x)
{
  return cos_52((float)f_halfpi - x);
}

// *******************************************************************
// ***
// ***  Routines to compute tangent to 5.6 digits of accuracy.
// ***
// *******************************************************************
//
//		tan_56s computes tan(f_pi*x/4)
//
//  Accurate to about 5.6 decimal digits over the range [0, f_pi/4].
//  The input argument is in radians. Note that the function
//  computes tan(f_pi*x/4), NOT tan(x); it's up to the range
//  reduction algorithm that calls this to scale things properly.
//
//  Algorithm:
//		tan(x)= x(c1 + c2*x**2)/(c3 + x**2)
//
float tan_56s(float x)
{
  const float c1 = -3.16783027;
  const float c2 =  0.134516124;
  const float c3 = -4.033321984;

  float x2 = x * x;	 // The input argument squared
  return (x * (c1 + c2 * x2) / (c3 + x2));
}
//
// This is the main tangent approximation "driver"
// It reduces the input argument's range to [0, f_pi/4],
// and then calls the approximator.
// See the notes for an explanation of the range reduction.
// Enter with positive angles only.
//
// WARNING: We do not test for the tangent approaching infinity,
// which it will at x=f_pi/2 and x=3*f_pi/2. If this is a problem
// in your application, take appropriate action.
//
float tan_56(float x)
{
  x = fmodf(x, (float)f_twopi);                 // Get rid of values >2 *f_pi
  int octant = int(x * (float)f_four_over_pi);  // Get octant # (0 to 7)
  switch (octant){
    case 0: return         tan_56s(x                          * (float)f_four_over_pi);
    case 1: return  1.0f / tan_56s(((float)f_halfpi - x)      * (float)f_four_over_pi);
    case 2: return -1.0f / tan_56s((x-(float)f_halfpi)        * (float)f_four_over_pi);
    case 3: return -       tan_56s(((float)f_pi - x)          * (float)f_four_over_pi);
    case 4: return         tan_56s((x-(float)f_pi)            * (float)f_four_over_pi);
    case 5: return  1.0f / tan_56s(((float)f_threehalfpi - x) * (float)f_four_over_pi);
    case 6: return -1.0f / tan_56s((x-(float)f_threehalfpi)   * (float)f_four_over_pi);
    case 7: return -       tan_56s(((float)f_twopi - x)       * (float)f_four_over_pi);
  }
}

// *******************************************************************
// ***
// ***  Routines to compute arctangent to 6.6 digits of accuracy.
// ***
// *******************************************************************
//
//		atan_66s computes atan(x)
//
//  Accurate to about 6.6 decimal digits over the range [0, f_pi/12].
//
//  Algorithm:
//		atan(x)= x(c1 + c2*x**2)/(c3 + x**2)
//
float atan_66s(float x)
{
  const float c1 = 1.6867629106;
  const float c2 = 0.4378497304;
  const float c3 = 1.6867633134;

  float x2 = x * x;  // The input argument squared
  return (x * (c1 + x2 * c2) / (c3 + x2));
}
//
// This is the main arctangent approximation "driver"
// It reduces the input argument's range to [0, f_pi/12],
// and then calls the approximator.
//
float atan_66(float x)
{
  float y;                  // return from atan__s function
  bool complement= false;   // true if arg was >1
  bool region= false;       // true depending on region arg is in
  bool sign= false;         // true if arg was < 0

  if (x < 0) {
    x = -x;
    sign = true;            // arctan(-x)=-arctan(x)
  }
  if (x > 1.0) {
    x = 1.0 / x;            // keep arg between 0 and 1
    complement = true;
  }
  if (x > (float)f_tantwelfthpi) {
    x = (x - (float)f_tansixthpi) / (1 + (float)f_tansixthpi * x);  // reduce arg to under tan(f_pi/12)
    region = true;
  }

  y = atan_66s(x);          // run the approximation
  if (region) { y += (float)f_sixthpi; }      // correct for region we're in
  if (complement) { y = (float)f_halfpi-y; }  // correct for 1/x if we did that
  if (sign) { y = -y;	}     // correct for negative arg
  return (y);
}

float asinf1(float x)
{
  float d = 1.0f - x * x;
  if (d < 0.0f) { return NAN; }
  return 2 * atan_66(x / (1 + sqrt1(d)));
}

float acosf1(float x)
{
  float d = 1.0f - x * x;
  if (d < 0.0f) { return NAN; }
  float y = asinf1(sqrt1(d));
  if (x >= 0.0f) {
    return y;
  } else {
    return (float)f_pi - y;
  }
}

// https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
float sqrt1(const float x)
{
  union {
    int i;
    float x;
  } u;
  u.x = x;
  u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

  // Two Babylonian Steps (simplified from:)
  // u.x = 0.5f * (u.x + x/u.x);
  // u.x = 0.5f * (u.x + x/u.x);
  u.x =         u.x + x / u.x;
  u.x = 0.25f * u.x + x / u.x;

  return u.x;
}

//
// changeUIntScale
// Change a value for range a..b to c..d, using only unsigned int math
//
// New version, you don't need the "to_min < to_max" precondition anymore
//
// PRE-CONDITIONS (if not satisfied, you may 'halt and catch fire')
//    from_min < from_max  (not checked)
//    from_min <= num <= from-max  (chacked)
// POST-CONDITIONS
//    to_min <= result <= to_max
//
uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
                                       uint16_t ito_min, uint16_t ito_max) {
  // guard-rails
  if (ifrom_min >= ifrom_max) {
    if (ito_min > ito_max) {
      return ito_max;
    } else {
      return ito_min;  // invalid input, return arbitrary value
    }
  }
  // convert to uint31, it's more verbose but code is more compact
  uint32_t num = inum;
  uint32_t from_min = ifrom_min;
  uint32_t from_max = ifrom_max;
  uint32_t to_min = ito_min;
  uint32_t to_max = ito_max;

  // check source range
  num = (num > from_max ? from_max : (num < from_min ? from_min : num));

  // check to_* order
  if (to_min > to_max) {
    // reverse order
    num = (from_max - num) + from_min;
    to_min = ito_max;
    to_max = ito_min;
  }

  uint32_t numerator = (num - from_min) * (to_max - to_min);
  uint32_t result;
  if (numerator >= 0x80000000L) {
    // don't do rounding as it would create an overflow
    result = numerator / (from_max - from_min) + to_min;
  } else {
    result = (((numerator * 2) / (from_max - from_min)) + 1) / 2 + to_min;
  }
  return (uint32_t) (result > to_max ? to_max : (result < to_min ? to_min : result));
}
