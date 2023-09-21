/* L3SubBand */

#include "types.h"
#include "tables.h"
#include "l3subband.h"

/*
 * shine_subband_initialise:
 * ----------------------
 * Calculates the analysis filterbank coefficients and rounds to the
 * 9th decimal place accuracy of the filterbank tables in the ISO
 * document.  The coefficients are stored in #filter#
 */
void shine_subband_initialise(shine_global_config *config) {
  int i,j;
  double filter;

  for(i=MAX_CHANNELS; i-- ; ) {
    config->subband.off[i] = 0;
    memset(config->subband.x[i], 0, sizeof(config->subband.x[i]));
  }

  for (i=SBLIMIT; i--; )
    for (j=64; j--; )
    {
      if ((filter = 1e9*cos((double)((2*i+1)*(16-j)*PI64))) >= 0)
        modf(filter+0.5, &filter);
      else
        modf(filter-0.5, &filter);
      /* scale and convert to fixed point before storing */
      config->subband.fl[i][j] = (int)(filter * (0x7fffffff * 1e-9));
    }
}

/*
 * shine_window_filter_subband:
 * -------------------------
 * Overlapping window on PCM samples
 * 32 16-bit pcm samples are scaled to fractional 2's complement and
 * concatenated to the end of the window buffer #x#. The updated window
 * buffer #x# is then windowed by the analysis window #shine_enwindow# to produce
 * the windowed sample #z#
 * Calculates the analysis filter bank coefficients
 * The windowed samples #z# is filtered by the digital filter matrix #filter#
 * to produce the subband samples #s#. This done by first selectively
 * picking out values from the windowed samples, and then multiplying
 * them by the filter matrix, producing 32 subband samples.
 */
void shine_window_filter_subband(int16_t **buffer, int s[SBLIMIT], int ch, shine_global_config *config, int stride) {
  int y[64];
  int i,j;
  int16_t *ptr = *buffer;

  /* replace 32 oldest samples with 32 new samples */
  for (i=32;i--;) {
    config->subband.x[ch][i+config->subband.off[ch]] = ((int)*ptr) << 16;
    ptr += stride;
  }
  *buffer = ptr;

  for (i=64; i--; ) {
	int s_value;
#ifdef __BORLANDC__
	uint32_t s_value_lo;
#else
	 uint32_t s_value_lo __attribute__((unused));
#endif

    asm_mul0  (s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (0<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (0<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (1<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (1<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (2<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (2<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (3<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (3<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (4<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (4<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (5<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (5<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (6<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (6<<6)]);
    asm_muladd(s_value, s_value_lo, config->subband.x[ch][(config->subband.off[ch] + i + (7<<6)) & (HAN_SIZE-1)], shine_enwindow[i + (7<<6)]);
    asm_mulz  (s_value, s_value_lo);
    y[i] = s_value;
  }

  config->subband.off[ch] = (config->subband.off[ch] + 480) & (HAN_SIZE-1); /* offset is modulo (HAN_SIZE)*/

  for (i=SBLIMIT; i--; ) {
	int s_value;
#ifdef __BORLANDC__
	uint32_t s_value_lo;
#else
	uint32_t s_value_lo __attribute__((unused));
#endif

    asm_mul0(s_value, s_value_lo, config->subband.fl[i][63], y[63]);
    for (j=63; j; j-=7) {
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-1], y[j-1]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-2], y[j-2]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-3], y[j-3]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-4], y[j-4]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-5], y[j-5]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-6], y[j-6]);
      asm_muladd(s_value, s_value_lo, config->subband.fl[i][j-7], y[j-7]);
    }
    asm_mulz(s_value, s_value_lo);
    s[i] = s_value;
  }
}
