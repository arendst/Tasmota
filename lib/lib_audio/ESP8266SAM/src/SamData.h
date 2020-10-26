/*
 * dataspace.h
 *
 *  Created on: Feb 24, 2019
 *      Author: chris.l
 */

#ifndef SAMDATA_H_
#define SAMDATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SAMDATA

typedef struct s_samdata
{
    struct render
    {
        unsigned char pitches[256]; // tab43008
        unsigned char frequency1[256];
        unsigned char frequency2[256];
        unsigned char frequency3[256];
        unsigned char amplitude1[256];
        unsigned char amplitude2[256];
        unsigned char amplitude3[256];
        unsigned char sampledConsonantFlag[256]; // tab44800
    } render;
    struct reciter {
        unsigned char inputtemp[256];
    } reciter;
    struct sam
    {
        char input[256]; //tab39445
        unsigned char stress[256]; //numbers from 0 to 8
        unsigned char phonemeLength[256]; //tab40160
        unsigned char phonemeindex[256];
        unsigned char phonemeIndexOutput[60]; //tab47296
        unsigned char stressOutput[60]; //tab47365
        unsigned char phonemeLengthOutput[60]; //tab47416
    } sam;
} SamData;

extern SamData* samdata;

#ifdef __cplusplus
}
#endif



#endif /* SAMDATA_H_ */
