/* layer3.c */

#ifdef ESP32

#include "types.h"
#include "tables.h"
#include "layer3.h"
#include "l3subband.h"
#include "l3mdct.h"
#include "l3loop.h"
#include "bitstream.h"
#include "l3bitstream.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"

static uint32_t counter[5] = {0};

static int granules_per_frame[4] = {
    1,  /* MPEG 2.5 */
   -1,  /* Reserved */
    1,  /* MPEG II */
    2  /* MPEG I */
};

/* Set default values for important vars */
void shine_set_config_mpeg_defaults(shine_mpeg_t *mpeg) {
  mpeg->bitr = 128;
  mpeg->emph = NONE;
  mpeg->copyright = 0;
  mpeg->original  = 1;
}

int shine_mpeg_version(int samplerate_index) {
  /* Pick mpeg version according to samplerate index. */
  if (samplerate_index < 3) {
    /* First 3 samplerates are for MPEG-I */
    return MPEG_I;
  } else if (samplerate_index < 6) {
    /* Then it's MPEG-II */
    return MPEG_II;
  } else {
    /* Finally, MPEG-2.5 */
    return MPEG_25;
  }
}

int shine_find_samplerate_index(int freq) {
  int i;

  for(i=0;i<9;i++) {
    if(freq==samplerates[i]) return i;
  }
  return -1; /* error - not a valid samplerate for encoder */
}

int shine_find_bitrate_index(int bitr, int mpeg_version) {
  int i;

  for(i=0;i<16;i++) {
    if(bitr==bitrates[i][mpeg_version]) return i;
  }
  return -1; /* error - not a valid samplerate for encoder */
}

int shine_check_config(int freq, int bitr) {
  int samplerate_index, bitrate_index, mpeg_version;

  samplerate_index = shine_find_samplerate_index(freq);
  if (samplerate_index < 0) {
    return -1;
  }
  mpeg_version = shine_mpeg_version(samplerate_index);

  bitrate_index = shine_find_bitrate_index(bitr, mpeg_version);
  if (bitrate_index < 0) {
    return -1;
  }
  return mpeg_version;
}

int shine_samples_per_pass(shine_t s) {
  return s->mpeg.granules_per_frame * GRANULE_SIZE;
}

/* Compute default encoding values. */
shine_global_config *shine_initialise(shine_config_t *pub_config) {
  double avg_slots_per_frame;
  shine_global_config *config;
  int x, y;
  if (shine_check_config(pub_config->wave.samplerate, pub_config->mpeg.bitr) < 0) {
    return NULL;
  }

  config = (shine_global_config*)heap_caps_malloc(sizeof(shine_global_config), MALLOC_CAP_SPIRAM);
  if (config == NULL) {
    return config;
  }

  memset(config, 0, sizeof(shine_global_config));

#ifdef  SHINE_DEBUG
  printf("l3_enc & mdct_freq each: %d\n", sizeof(int32_t)*GRANULE_SIZE*MAX_GRANULES*MAX_CHANNELS);
#endif

  for (x = 0; x < MAX_CHANNELS; x++) {
      for (y = 0; y < MAX_GRANULES; y++) {
        // 2 * 2 * 576 each
        config->l3_enc[x][y] = (int*)heap_caps_malloc_prefer(4*GRANULE_SIZE, MALLOC_CAP_32BIT, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //Significant performance hit in IRAM
        if (!config->l3_enc[x][y]) {
          // error should never occur because of spiram size
          //config->l3_enc[x][y] = (int*)heap_caps_malloc(sizeof(int32_t)*GRANULE_SIZE,MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT);
        }
        config->mdct_freq[x][y] = (int*)heap_caps_malloc_prefer(4*GRANULE_SIZE, MALLOC_CAP_32BIT, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //OK 1%
        if (!config->mdct_freq[x][y]) {
          // error
          //config->mdct_freq[x][y] = (int*)heap_caps_malloc(sizeof(int32_t)*GRANULE_SIZE, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT);
        }
      }
  }
#ifdef  SHINE_DEBUG
  printf("l3loop struct: %d\n", sizeof(l3loop_t));
#endif
  config->l3loop = (l3loop_t*)heap_caps_malloc(sizeof(l3loop_t), MALLOC_CAP_SPIRAM);
#ifdef  SHINE_DEBUG
  printf("xrsq & xrabs each: %d\n", sizeof(int)*GRANULE_SIZE);
#endif
  config->l3loop->xrsq = (int*)heap_caps_malloc_prefer(4*GRANULE_SIZE, MALLOC_CAP_32BIT, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //OK 0.5%
  if (!config->l3loop->xrsq) {
    // error
    //config->l3loop->xrsq = (int*)heap_caps_malloc(sizeof(int32_t)*GRANULE_SIZE, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //OK 0.5%
  }

  config->l3loop->xrabs = (int*)heap_caps_malloc_prefer(4*GRANULE_SIZE, MALLOC_CAP_32BIT, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //OK 0.5%
  if (!config->l3loop->xrabs) {
    //config->l3loop->xrabs = (int*)heap_caps_malloc(sizeof(int32_t)*GRANULE_SIZE, MALLOC_CAP_SPIRAM|MALLOC_CAP_32BIT); //OK 0.5%
  }

/*typedef struct {
  int32_t *xr;
  int32_t *xrsq[GRANULE_SIZE];
  int32_t *xrabs[GRANULE_SIZE];
  int32_t xrmax;
  int32_t en_tot[MAX_GRANULES];
  int32_t en[MAX_GRANULES][21];
  int32_t xm[MAX_GRANULES][21];
  int32_t xrmaxl[MAX_GRANULES];
  double steptab[128];
  int32_t steptabi[128];
  int16_t int2idx[10000];
} l3loop_t;*/

  shine_subband_initialise(config);
  shine_mdct_initialise(config);
  shine_loop_initialise(config);

  /* Copy public config. */
  config->wave.channels   = pub_config->wave.channels;
  config->wave.samplerate = pub_config->wave.samplerate;
  config->mpeg.mode       = pub_config->mpeg.mode;
  config->mpeg.bitr       = pub_config->mpeg.bitr;
  config->mpeg.emph       = pub_config->mpeg.emph;
  config->mpeg.copyright  = pub_config->mpeg.copyright;
  config->mpeg.original   = pub_config->mpeg.original;

  /* Set default values. */
  config->ResvMax             = 0;
  config->ResvSize            = 0;
  config->mpeg.layer          = LAYER_III;
  config->mpeg.crc            = 0;
  config->mpeg.ext            = 0;
  config->mpeg.mode_ext       = 0;
  config->mpeg.bits_per_slot  = 8;

  config->mpeg.samplerate_index   = shine_find_samplerate_index(config->wave.samplerate);
  config->mpeg.version            = shine_mpeg_version(config->mpeg.samplerate_index);
  config->mpeg.bitrate_index      = shine_find_bitrate_index(config->mpeg.bitr, config->mpeg.version);
  config->mpeg.granules_per_frame = granules_per_frame[config->mpeg.version];

  /* Figure average number of 'slots' per frame. */
  avg_slots_per_frame = ((double)config->mpeg.granules_per_frame * GRANULE_SIZE /
                        ((double)config->wave.samplerate)) *
                        (1000*(double)config->mpeg.bitr /
                         (double)config->mpeg.bits_per_slot);

  config->mpeg.whole_slots_per_frame  = (int)avg_slots_per_frame;

  config->mpeg.frac_slots_per_frame  = avg_slots_per_frame - (double)config->mpeg.whole_slots_per_frame;
  config->mpeg.slot_lag              = -config->mpeg.frac_slots_per_frame;

  if(config->mpeg.frac_slots_per_frame==0) {
    config->mpeg.padding = 0;
  }

  shine_open_bit_stream(&config->bs, BUFFER_SIZE);

  memset((char *)&config->side_info,0,sizeof(shine_side_info_t));

  /* determine the mean bitrate for main data */
  if (config->mpeg.granules_per_frame == 2) { /* MPEG 1 */
    config->sideinfo_len = 8 * ((config->wave.channels==1) ? 4 + 17 : 4 + 32);
  } else {               /* MPEG 2 */
    config->sideinfo_len = 8 * ((config->wave.channels==1) ? 4 + 9 : 4 + 17);
  }
  return config;
}



uint32_t *shine_get_counters() {
  return counter;
}

/* Counter results
Counters 1550541561 : 1550541629 : 1553135798 : 1555116724 : 1555309952
68
core 1 will do:
2594169
core 0 will do:
1980926
193228


Counters 2664123380 : 2664123448 : 2666717886 : 2668665908 : 2668859025
*/


static unsigned char *shine_encode_buffer_internal(shine_global_config *config, int *written, int stride) {
  counter[0] = portGET_RUN_TIME_COUNTER_VALUE();      // TASMOTA more portable solution
  // counter[0] = xthal_get_ccount();
  if(config->mpeg.frac_slots_per_frame) {
    config->mpeg.padding   = (config->mpeg.slot_lag <= (config->mpeg.frac_slots_per_frame - 1.0));
    config->mpeg.slot_lag += (config->mpeg.padding - config->mpeg.frac_slots_per_frame);
  }

  config->mpeg.bits_per_frame = 8*(config->mpeg.whole_slots_per_frame + config->mpeg.padding);
  config->mean_bits = (config->mpeg.bits_per_frame - config->sideinfo_len)/config->mpeg.granules_per_frame;
  counter[1] = portGET_RUN_TIME_COUNTER_VALUE();      // TASMOTA more portable solution
  // counter[1] = xthal_get_ccount();
  /* apply mdct to the polyphase output */
  // put on core 1
  shine_mdct_sub(config, stride);
  counter[2] = portGET_RUN_TIME_COUNTER_VALUE();      // TASMOTA more portable solution
  // counter[2] = xthal_get_ccount();
  /* bit and noise allocation */
  //put on core 0
  shine_iteration_loop(config);
  counter[3] = portGET_RUN_TIME_COUNTER_VALUE();      // TASMOTA more portable solution
  // counter[3] = xthal_get_ccount();
  /* write the frame to the bitstream */
  shine_format_bitstream(config);
  counter[4] = portGET_RUN_TIME_COUNTER_VALUE();      // TASMOTA more portable solution
  // counter[4] = xthal_get_ccount();
  /* Return data. */
  *written = config->bs.data_position;
  config->bs.data_position = 0;

  return config->bs.data;
}

unsigned char *shine_encode_buffer(shine_global_config *config, int16_t **data, int *written) {
  config->buffer[0] = data[0];
  if (config->wave.channels == 2) {
    config->buffer[1] = data[1];
  }
  return shine_encode_buffer_internal(config, written, 1);
}

unsigned char *shine_encode_buffer_interleaved(shine_global_config *config, int16_t *data, int *written) {
  config->buffer[0] = data;
  if (config->wave.channels == 2) {
    config->buffer[1] = data + 1;
  }
  return shine_encode_buffer_internal(config, written, config->wave.channels);
}

unsigned char *shine_flush(shine_global_config *config, int *written) {
  *written = config->bs.data_position;
  config->bs.data_position = 0;
  return config->bs.data;
}

void shine_close(shine_global_config *config) {
  shine_close_bit_stream(&config->bs);

  for (uint16_t x = 0; x < MAX_CHANNELS; x++) {
      for (uint16_t y = 0; y < MAX_GRANULES; y++) {
        if (config->l3_enc[x][y]) {
          free(config->l3_enc[x][y]);
        }
        if (config->mdct_freq[x][y]) {
          free(config->mdct_freq[x][y]);
        }
      }
  }

  if (config->l3loop) {
    free(config->l3loop);
  }
  if (config->l3loop->xrsq) {
    free(config->l3loop->xrsq);
  }
  if (config->l3loop->xrabs) {
    free(config->l3loop->xrabs);
  }

  free(config);
}

#endif // ESP32
