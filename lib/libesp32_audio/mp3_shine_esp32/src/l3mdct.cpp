/* L3mdct */

#include "types.h"
#include "l3mdct.h"
#include "l3subband.h"

/* This is table B.9: coefficients for aliasing reduction */
#define MDCT_CA(coef)	(int)(coef / sqrt(1.0 + (coef * coef)) * 0x7fffffff)
#define MDCT_CS(coef)	(int)(1.0  / sqrt(1.0 + (coef * coef)) * 0x7fffffff)

#define MDCT_CA0	MDCT_CA(-0.6)
#define MDCT_CA1	MDCT_CA(-0.535)
#define MDCT_CA2	MDCT_CA(-0.33)
#define MDCT_CA3	MDCT_CA(-0.185)
#define MDCT_CA4	MDCT_CA(-0.095)
#define MDCT_CA5	MDCT_CA(-0.041)
#define MDCT_CA6	MDCT_CA(-0.0142)
#define MDCT_CA7	MDCT_CA(-0.0037)

#define MDCT_CS0	MDCT_CS(-0.6)
#define MDCT_CS1	MDCT_CS(-0.535)
#define MDCT_CS2	MDCT_CS(-0.33)
#define MDCT_CS3	MDCT_CS(-0.185)
#define MDCT_CS4	MDCT_CS(-0.095)
#define MDCT_CS5	MDCT_CS(-0.041)
#define MDCT_CS6	MDCT_CS(-0.0142)
#define MDCT_CS7	MDCT_CS(-0.0037)

/*
 * shine_mdct_initialise:
 * -------------------
 */
void shine_mdct_initialise(shine_global_config *config) {
  int m,k;

  /* prepare the mdct coefficients */
  for(m=18; m--; )
    for(k=36; k--; )
      /* combine window and mdct coefficients into a single table */
      /* scale and convert to fixed point before storing */
      config->mdct.cos_l[m][k] = (int)(sin(PI36*(k+0.5))
                                      * cos((PI/72)*(2*k+19)*(2*m+1)) * 0x7fffffff);
}

/*
 * shine_mdct_sub:
 * ------------
 */
void shine_mdct_sub(shine_global_config *config, int stride) {
  /* note. we wish to access the array 'config->mdct_freq[2][2][576]' as
   * [2][2][32][18]. (32*18=576),
   */
  int (*mdct_enc)[18];

  int  ch,gr,band,j,k;
  int mdct_in[36];

  for(ch=config->wave.channels; ch--; )
  {
    for(gr=0; gr<config->mpeg.granules_per_frame; gr++)
    {
      /* set up pointer to the part of config->mdct_freq we're using */
      mdct_enc = (int (*)[18]) config->mdct_freq[ch][gr];

      /* polyphase filtering */
      for(k=0; k<18; k+=2)
      {
      	shine_window_filter_subband(&config->buffer[ch], &config->l3_sb_sample[ch][gr+1][k  ][0], ch, config, stride);
      	shine_window_filter_subband(&config->buffer[ch], &config->l3_sb_sample[ch][gr+1][k+1][0], ch, config, stride);
        /* Compensate for inversion in the analysis filter
         * (every odd index of band AND k)
         */
        for(band=1; band<32; band+=2)
          config->l3_sb_sample[ch][gr+1][k+1][band] *= -1;
      }

      /* Perform imdct of 18 previous subband samples + 18 current subband samples */
      for(band=0; band<32; band++)
      {
        for(k=18; k--; )
        {
          mdct_in[k   ] = config->l3_sb_sample[ch][gr  ][k][band];
          mdct_in[k+18] = config->l3_sb_sample[ch][gr+1][k][band];
        }

        /* Calculation of the MDCT
         * In the case of long blocks ( block_type 0,1,3 ) there are
         * 36 coefficients in the time domain and 18 in the frequency
         * domain.
         */

        for(k=18; k--; )
        {
		  int vm;
#ifdef __BORLANDC__
		  uint32_t vm_lo;
#else
		  uint32_t vm_lo __attribute__((unused));
#endif

          asm_mul0(vm, vm_lo, mdct_in[35], config->mdct.cos_l[k][35]);
          for(j=35; j; j-=7) {
            asm_muladd(vm, vm_lo, mdct_in[j-1], config->mdct.cos_l[k][j-1]);
            asm_muladd(vm, vm_lo, mdct_in[j-2], config->mdct.cos_l[k][j-2]);
            asm_muladd(vm, vm_lo, mdct_in[j-3], config->mdct.cos_l[k][j-3]);
            asm_muladd(vm, vm_lo, mdct_in[j-4], config->mdct.cos_l[k][j-4]);
            asm_muladd(vm, vm_lo, mdct_in[j-5], config->mdct.cos_l[k][j-5]);
            asm_muladd(vm, vm_lo, mdct_in[j-6], config->mdct.cos_l[k][j-6]);
            asm_muladd(vm, vm_lo, mdct_in[j-7], config->mdct.cos_l[k][j-7]);
          }
          asm_mulz(vm, vm_lo);
          mdct_enc[band][k] = vm;
        }

        /* Perform aliasing reduction butterfly */
        asm ("#asm_cmuls:");
        if (band != 0)
        {
          asm_cmuls(mdct_enc[band][0], mdct_enc[band-1][17-0], mdct_enc[band][0], mdct_enc[band-1][17-0], MDCT_CS0, MDCT_CA0);
          asm_cmuls(mdct_enc[band][1], mdct_enc[band-1][17-1], mdct_enc[band][1], mdct_enc[band-1][17-1], MDCT_CS1, MDCT_CA1);
          asm_cmuls(mdct_enc[band][2], mdct_enc[band-1][17-2], mdct_enc[band][2], mdct_enc[band-1][17-2], MDCT_CS2, MDCT_CA2);
          asm_cmuls(mdct_enc[band][3], mdct_enc[band-1][17-3], mdct_enc[band][3], mdct_enc[band-1][17-3], MDCT_CS3, MDCT_CA3);
          asm_cmuls(mdct_enc[band][4], mdct_enc[band-1][17-4], mdct_enc[band][4], mdct_enc[band-1][17-4], MDCT_CS4, MDCT_CA4);
          asm_cmuls(mdct_enc[band][5], mdct_enc[band-1][17-5], mdct_enc[band][5], mdct_enc[band-1][17-5], MDCT_CS5, MDCT_CA5);
          asm_cmuls(mdct_enc[band][6], mdct_enc[band-1][17-6], mdct_enc[band][6], mdct_enc[band-1][17-6], MDCT_CS6, MDCT_CA6);
          asm_cmuls(mdct_enc[band][7], mdct_enc[band-1][17-7], mdct_enc[band][7], mdct_enc[band-1][17-7], MDCT_CS7, MDCT_CA7);
        }
      }
    }

    /* Save latest granule's subband samples to be used in the next mdct call */
    memcpy(config->l3_sb_sample[ch][0], config->l3_sb_sample[ch][config->mpeg.granules_per_frame], sizeof(config->l3_sb_sample[0][0]));
  }
}
