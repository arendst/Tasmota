/* reservoir.c
 * Layer3 bit reservoir: Described in C.1.5.4.2.2 of the IS
 */

#include "types.h"
#include "layer3.h"
#include "l3loop.h"
#include "huffman.h"
#include "bitstream.h"
#include "l3bitstream.h"
#include "reservoir.h"

/*
 * shine_max_reservoir_bits:
 * ------------
 * Called at the beginning of each granule to get the max bit
 * allowance for the current granule based on reservoir size
 * and perceptual entropy.
 */
int shine_max_reservoir_bits (double *pe, shine_global_config *config ) {
  int more_bits, max_bits, add_bits, over_bits;
  int mean_bits = config->mean_bits;

  mean_bits /= config->wave.channels;
  max_bits = mean_bits;

  if(max_bits>4095)
    max_bits = 4095;
  if(!config->ResvMax)
    return max_bits;

  more_bits = *pe * 3.1 - mean_bits;
  add_bits = 0;
  if(more_bits>100)
  {
    int frac = (config->ResvSize * 6) / 10;

    if(frac<more_bits)
      add_bits = frac;
    else
      add_bits = more_bits;
  }
  over_bits = config->ResvSize - ((config->ResvMax <<3) / 10) - add_bits;
  if (over_bits>0)
    add_bits += over_bits;

  max_bits += add_bits;
  if(max_bits>4095)
    max_bits = 4095;
  return max_bits;
}

/*
 * shine_ResvAdjust:
 * -----------
 * Called after a granule's bit allocation. Readjusts the size of
 * the reservoir to reflect the granule's usage.
 */
void shine_ResvAdjust(gr_info *gi, shine_global_config *config ) {
  config->ResvSize += (config->mean_bits / config->wave.channels) - gi->part2_3_length;
}

/*
 * shine_ResvFrameEnd:
 * -------------
 * Called after all granules in a frame have been allocated. Makes sure
 * that the reservoir size is within limits, possibly by adding stuffing
 * bits. Note that stuffing bits are added by increasing a granule's
 * part2_3_length. The bitstream formatter will detect this and write the
 * appropriate stuffing bits to the bitstream.
 */
void shine_ResvFrameEnd(shine_global_config *config ) {
  gr_info *gi;
  int gr, ch, ancillary_pad, stuffingBits;
  int over_bits;
  shine_side_info_t *l3_side = &config->side_info;

  ancillary_pad = 0;

  /* just in case mean_bits is odd, this is necessary... */
  if((config->wave.channels==2) && (config->mean_bits & 1))
    config->ResvSize += 1;

  over_bits = config->ResvSize - config->ResvMax;
  if(over_bits<0)
    over_bits = 0;

  config->ResvSize -= over_bits;
  stuffingBits = over_bits + ancillary_pad;

  /* we must be byte aligned */
  if((over_bits = config->ResvSize % 8))
  {
    stuffingBits += over_bits;
    config->ResvSize -= over_bits;
  }

  if(stuffingBits)
  {
    /*
     * plan a: put all into the first granule
     * This was preferred by someone designing a
     * real-time decoder...
     */
    gi = (gr_info *) &(l3_side->gr[0].ch[0]);

    if ( gi->part2_3_length + stuffingBits < 4095 )
      gi->part2_3_length += stuffingBits;
    else
    {
      /* plan b: distribute throughout the granules */
      for (gr = 0; gr < config->mpeg.granules_per_frame; gr++ )
        for (ch = 0; ch < config->wave.channels; ch++ )
        {
          int extraBits, bitsThisGr;
          gr_info *gi = (gr_info *) &(l3_side->gr[gr].ch[ch]);
          if (!stuffingBits)
            break;
          extraBits = 4095 - gi->part2_3_length;
          bitsThisGr = extraBits < stuffingBits ? extraBits : stuffingBits;
          gi->part2_3_length += bitsThisGr;
          stuffingBits -= bitsThisGr;
        }
      /*
       * If any stuffing bits remain, we elect to spill them
       * into ancillary data. The bitstream formatter will do this if
       * l3side->resvDrain is set
       */
      l3_side->resvDrain = stuffingBits;
    }
  }
}
