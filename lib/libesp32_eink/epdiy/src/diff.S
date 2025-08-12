#include <xtensa/config/core-isa.h>
#include <xtensa/config/core-matmap.h>
#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

.text
.align  4
.global epd_interlace_4bpp_line_VE
.type   epd_interlace_4bpp_line_VE,@function

//        // CRASH AND BURN for debugging
//        EE.MOVI.32.A q3, a2, 0
//        EE.MOVI.32.A q3, a3, 1
//        EE.MOVI.32.A q3, a4, 2
//        EE.MOVI.32.A q3, a5, 3
//        l8ui a10, a10, 0

// bool interlace_line(
//     const uint8_t *to,
//     const uint8_t *from,
//     uint8_t *col_dirtyness;
//     uint8_t *interlaced,
//     int fb_width
// )
epd_interlace_4bpp_line_VE: 
// to   - a2
// from   - a3
// interlaced   - a4
// col_dirtyness  - a5
// fb_width      - a6

    entry	a1, 32

    // divide by 32 for loop count
    srli a11, a6, 5

    movi.n a10, 0xF0F0F0F0;
    EE.MOVI.32.Q q6,a10,0
    EE.MOVI.32.Q q6,a10,1
    EE.MOVI.32.Q q6,a10,2
    EE.MOVI.32.Q q6,a10,3

    movi.n a10, 0x0F0F0F0F
    EE.MOVI.32.Q q7,a10,0
    EE.MOVI.32.Q q7,a10,1
    EE.MOVI.32.Q q7,a10,2
    EE.MOVI.32.Q q7,a10,3

    // put 4 into shift amount
    movi.n a10, 4
    WSR.SAR a10
    
    // "dirtyness" register
    EE.ZERO.Q q5

    // Instructions sometimes are in an unexpected order
    // for best pipeline utilization
    loopnez a11, .loop_end_difference

        EE.VLD.128.IP q0, a2, 16
        EE.VLD.128.IP q1, a3, 16

        // load column dirtyness
        EE.VLD.128.IP q3, a5, 0
    
        // update dirtyness
        EE.XORQ q4, q1, q0

        // line dirtyness accumulator
        EE.ORQ q5, q5, q4
        // column dirtyness 
        EE.ORQ q3, q3, q4

        // store column dirtyness 
        EE.VST.128.IP q3, a5, 16

        // mask out every second value
        EE.ANDQ q2, q0, q7
        EE.ANDQ q0, q0, q6
        EE.ANDQ q3, q1, q7
        EE.ANDQ q1, q1, q6

        // shift vectors to align
        EE.VSL.32 q2, q2
        EE.VSR.32 q1, q1

        // the right shift sign-extends, 
        // so we make sure the resulting shift is logical by masking again
        EE.ANDQ q1, q1, q7

        // Combine "from" and "to" nibble
        EE.ORQ q2, q2, q3
        EE.ORQ q0, q0, q1

        // Zip masked out values together
        EE.VZIP.8 q2, q0
        
        // store interlaced buffer data
        EE.VST.128.IP q2, a4, 16
        EE.VST.128.IP q0, a4, 16

.loop_end_difference:

    EE.MOVI.32.A q5, a2, 0
    EE.MOVI.32.A q5, a3, 1
    EE.MOVI.32.A q5, a4, 2
    EE.MOVI.32.A q5, a5, 3
    or a2, a2, a3
    or a2, a2, a4
    or a2, a2, a5

    //movi.n	a2, 1 // return "true"

    // CRASH AND BURN for debugging
    //EE.MOVI.32.A q5, a2, 0
    //EE.MOVI.32.A q5, a3, 1
    //EE.MOVI.32.A q5, a4, 2
    //EE.MOVI.32.A q5, a5, 3
    //movi.n a10, 0
    //l8ui a10, a10, 0

    retw.n


.global epd_apply_line_mask_VE
.type   epd_apply_line_mask_VE,@function

// void epd_apply_line_mask_VE(
//     uint8_t *line,
//     const uint8_t *mask,
//     int mask_len
// )
epd_apply_line_mask_VE: 
// line   - a2
// mask   - a3
// mask_len   - a4

    entry	a1, 32

    // divide by 16 for loop count
    srli a4, a4, 4

    // Instructions sometimes are in an unexpected order
    // for best pipeline utilization
    loopnez a4, .loop_end_mask

        EE.VLD.128.IP q0, a2, 0
        EE.VLD.128.IP q1, a3, 16

        EE.ANDQ q0, q0, q1
 
        EE.VST.128.IP q0, a2, 16

.loop_end_mask:

    retw.n

#endif