#include <xtensa/config/core-isa.h>
#include <xtensa/config/core-matmap.h>
#include "sdkconfig.h"

#ifdef CONFIG_IDF_TARGET_ESP32S3

.text
.align  4
.global calc_epd_input_1ppB_1k_S3_VE_aligned
.type   calc_epd_input_1ppB_1k_S3_VE_aligned,@function

//        // CRASH AND BURN for debugging
//        EE.MOVI.32.A q3, a2, 0
//        EE.MOVI.32.A q3, a3, 1
//        EE.MOVI.32.A q3, a4, 2
//        EE.MOVI.32.A q3, a5, 3
//        l8ui a10, a10, 0

// void calc_epd_input_1ppB_1k_S3_VE_aligned(
//    const uint32_t *ld,
//    uint8_t *epd_input,
//    const uint8_t *conversion_lut,
//    uint32_t epd_width
//);
calc_epd_input_1ppB_1k_S3_VE_aligned:
// input   - a2
// output   - a3
// lut   - a4
// len      - a5

    entry	a1, 32

    // divide by 16 and do one loop lesss,
    // because the last loop is special
    srli a5, a5, 4
    addi.n a5, a5, -1


    // bitmasks for bit shift by multiplication
    movi.n a10, 0x40001000
    EE.MOVI.32.Q q4,a10,0
    movi.n a10, 0x04000100
    EE.MOVI.32.Q q4,a10,1
    movi.n a10, 0x00400010
    EE.MOVI.32.Q q4,a10,2
    movi a10, 0x00040001
    EE.MOVI.32.Q q4,a10,3

    EE.ZERO.Q q0

    EE.VLD.128.IP q1, a2, 16

    // Instructions sometimes are in an unexpected order
    // for best pipeline utilization
    loopnez a5, .loop_end_lut_lookup

        // q1, q0 contain the input bytes, zero-extended to bits bytes
        EE.VZIP.8 q1, q0

        // load 32-bit LUT results
        EE.LDXQ.32 q2, q0, a4, 0, 6
        EE.LDXQ.32 q2, q0, a4, 1, 7
        EE.LDXQ.32 q2, q0, a4, 2, 4
        EE.LDXQ.32 q2, q0, a4, 3, 5
        EE.LDXQ.32 q3, q0, a4, 0, 2
        EE.LDXQ.32 q3, q0, a4, 1, 3
        EE.LDXQ.32 q3, q0, a4, 2, 0
        EE.LDXQ.32 q3, q0, a4, 3, 1

        EE.ZERO.ACCX
        
        // zip to have 16bit LUT results in q2, q3 zeroes
        EE.VUNZIP.16 q2, q3

        // combine results with using multiply-add as shift-or
        EE.VMULAS.U16.ACCX q2,q4

        // load 32-bit LUT results
        EE.LDXQ.32 q2, q1, a4, 0, 6
        EE.LDXQ.32 q2, q1, a4, 1, 7
        EE.LDXQ.32 q2, q1, a4, 2, 4
        EE.LDXQ.32 q2, q1, a4, 3, 5
        EE.LDXQ.32 q0, q1, a4, 0, 2
        EE.LDXQ.32 q0, q1, a4, 1, 3
        EE.LDXQ.32 q0, q1, a4, 2, 0
        EE.LDXQ.32 q0, q1, a4, 3, 1

        // store multiplication result in a6
        RUR.ACCX_0 a6
        s16i a6, a3, 2

        EE.ZERO.ACCX
 
        // zip to have 16bit LUT results in q2, q0 zeroes
        EE.VUNZIP.16 q2, q0

        // Combine second set of results and load the next data
        EE.VMULAS.U16.ACCX.LD.IP q1, a2, 16, q2, q4

        // store result in a6
        RUR.ACCX_0 a6
        s16i a6, a3, 0

        addi.n a3, a3, 4
.loop_end_lut_lookup:

    // Same as above, but in the last iteration
    // we do not load to not access out of bounds.
    EE.VZIP.8 q1, q0

    EE.LDXQ.32 q2, q0, a4, 0, 6
    EE.LDXQ.32 q2, q0, a4, 1, 7
    EE.LDXQ.32 q2, q0, a4, 2, 4
    EE.LDXQ.32 q2, q0, a4, 3, 5
    EE.LDXQ.32 q3, q0, a4, 0, 2
    EE.LDXQ.32 q3, q0, a4, 1, 3
    EE.LDXQ.32 q3, q0, a4, 2, 0
    EE.LDXQ.32 q3, q0, a4, 3, 1

    EE.ZERO.ACCX    
    EE.VUNZIP.16 q2, q3
    EE.VMULAS.U16.ACCX q2,q4

    EE.LDXQ.32 q2, q1, a4, 0, 6
    EE.LDXQ.32 q2, q1, a4, 1, 7
    EE.LDXQ.32 q2, q1, a4, 2, 4
    EE.LDXQ.32 q2, q1, a4, 3, 5
    EE.LDXQ.32 q0, q1, a4, 0, 2
    EE.LDXQ.32 q0, q1, a4, 1, 3
    EE.LDXQ.32 q0, q1, a4, 2, 0
    EE.LDXQ.32 q0, q1, a4, 3, 1

    RUR.ACCX_0 a6
    s16i a6, a3, 2
    EE.ZERO.ACCX
    
    EE.VUNZIP.16 q2, q0
    EE.VMULAS.U16.ACCX q2, q4
    RUR.ACCX_0 a6
    s16i a6, a3, 0

    movi.n	a2, 0 // return status ESP_OK
    retw.n

#endif