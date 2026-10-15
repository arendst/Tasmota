#ifndef RESERVOIR_H
#define RESERVOIR_H

void shine_ResvFrameBegin(int frameLength, shine_global_config *config);
int  shine_max_reservoir_bits   (double *pe, shine_global_config *config);
void shine_ResvAdjust    (gr_info *gi, shine_global_config *config );
void shine_ResvFrameEnd  (shine_global_config *config );

#endif
