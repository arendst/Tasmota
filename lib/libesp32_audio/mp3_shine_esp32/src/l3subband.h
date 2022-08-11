#ifndef L3SUBBAND_H
#define L3SUBBAND_H

#include <stdint.h>

void shine_subband_initialise( shine_global_config *config );
void shine_window_filter_subband(int16_t **buffer, int32_t s[SBLIMIT], int k, shine_global_config *config, int stride);

#endif
