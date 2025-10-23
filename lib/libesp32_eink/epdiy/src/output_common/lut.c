#include "lut.h"

#include "epdiy.h"
#include "esp_attr.h"
#include "render_context.h"
#include "render_method.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "esp_timer.h"

/*
 * Build Lookup tables and translate via LUTs.
 * WARNING: These functions must only ever write to internal memory,
 *          Since we disable the PSRAM workaround here for performance reasons.
 */

/* Python script for generating the 8ppB, starting at white lookup table:
 for i in range(256):
     number = 0;
     for b in range(8):
         if not (i & (1 << b)):
             number |= 1 << (2*b)
     print ('0x%04x,'%number)
 */
const uint32_t lut_8ppB_start_at_white[256] = {
    0x5555, 0x5554, 0x5551, 0x5550, 0x5545, 0x5544, 0x5541, 0x5540, 0x5515, 0x5514, 0x5511, 0x5510,
    0x5505, 0x5504, 0x5501, 0x5500, 0x5455, 0x5454, 0x5451, 0x5450, 0x5445, 0x5444, 0x5441, 0x5440,
    0x5415, 0x5414, 0x5411, 0x5410, 0x5405, 0x5404, 0x5401, 0x5400, 0x5155, 0x5154, 0x5151, 0x5150,
    0x5145, 0x5144, 0x5141, 0x5140, 0x5115, 0x5114, 0x5111, 0x5110, 0x5105, 0x5104, 0x5101, 0x5100,
    0x5055, 0x5054, 0x5051, 0x5050, 0x5045, 0x5044, 0x5041, 0x5040, 0x5015, 0x5014, 0x5011, 0x5010,
    0x5005, 0x5004, 0x5001, 0x5000, 0x4555, 0x4554, 0x4551, 0x4550, 0x4545, 0x4544, 0x4541, 0x4540,
    0x4515, 0x4514, 0x4511, 0x4510, 0x4505, 0x4504, 0x4501, 0x4500, 0x4455, 0x4454, 0x4451, 0x4450,
    0x4445, 0x4444, 0x4441, 0x4440, 0x4415, 0x4414, 0x4411, 0x4410, 0x4405, 0x4404, 0x4401, 0x4400,
    0x4155, 0x4154, 0x4151, 0x4150, 0x4145, 0x4144, 0x4141, 0x4140, 0x4115, 0x4114, 0x4111, 0x4110,
    0x4105, 0x4104, 0x4101, 0x4100, 0x4055, 0x4054, 0x4051, 0x4050, 0x4045, 0x4044, 0x4041, 0x4040,
    0x4015, 0x4014, 0x4011, 0x4010, 0x4005, 0x4004, 0x4001, 0x4000, 0x1555, 0x1554, 0x1551, 0x1550,
    0x1545, 0x1544, 0x1541, 0x1540, 0x1515, 0x1514, 0x1511, 0x1510, 0x1505, 0x1504, 0x1501, 0x1500,
    0x1455, 0x1454, 0x1451, 0x1450, 0x1445, 0x1444, 0x1441, 0x1440, 0x1415, 0x1414, 0x1411, 0x1410,
    0x1405, 0x1404, 0x1401, 0x1400, 0x1155, 0x1154, 0x1151, 0x1150, 0x1145, 0x1144, 0x1141, 0x1140,
    0x1115, 0x1114, 0x1111, 0x1110, 0x1105, 0x1104, 0x1101, 0x1100, 0x1055, 0x1054, 0x1051, 0x1050,
    0x1045, 0x1044, 0x1041, 0x1040, 0x1015, 0x1014, 0x1011, 0x1010, 0x1005, 0x1004, 0x1001, 0x1000,
    0x0555, 0x0554, 0x0551, 0x0550, 0x0545, 0x0544, 0x0541, 0x0540, 0x0515, 0x0514, 0x0511, 0x0510,
    0x0505, 0x0504, 0x0501, 0x0500, 0x0455, 0x0454, 0x0451, 0x0450, 0x0445, 0x0444, 0x0441, 0x0440,
    0x0415, 0x0414, 0x0411, 0x0410, 0x0405, 0x0404, 0x0401, 0x0400, 0x0155, 0x0154, 0x0151, 0x0150,
    0x0145, 0x0144, 0x0141, 0x0140, 0x0115, 0x0114, 0x0111, 0x0110, 0x0105, 0x0104, 0x0101, 0x0100,
    0x0055, 0x0054, 0x0051, 0x0050, 0x0045, 0x0044, 0x0041, 0x0040, 0x0015, 0x0014, 0x0011, 0x0010,
    0x0005, 0x0004, 0x0001, 0x0000
};

/* Python script for generating the 8ppB, starting at black lookup table:
 for i in range(256):
      number = 0;
      for b in range(8):
          if (i & (1 << b)):
              number |= 2 << (2*b)
      print ('0x%04x,'%number)
 */
const uint32_t lut_8ppB_start_at_black[256] = {
    0x0000, 0x0002, 0x0008, 0x000a, 0x0020, 0x0022, 0x0028, 0x002a, 0x0080, 0x0082, 0x0088, 0x008a,
    0x00a0, 0x00a2, 0x00a8, 0x00aa, 0x0200, 0x0202, 0x0208, 0x020a, 0x0220, 0x0222, 0x0228, 0x022a,
    0x0280, 0x0282, 0x0288, 0x028a, 0x02a0, 0x02a2, 0x02a8, 0x02aa, 0x0800, 0x0802, 0x0808, 0x080a,
    0x0820, 0x0822, 0x0828, 0x082a, 0x0880, 0x0882, 0x0888, 0x088a, 0x08a0, 0x08a2, 0x08a8, 0x08aa,
    0x0a00, 0x0a02, 0x0a08, 0x0a0a, 0x0a20, 0x0a22, 0x0a28, 0x0a2a, 0x0a80, 0x0a82, 0x0a88, 0x0a8a,
    0x0aa0, 0x0aa2, 0x0aa8, 0x0aaa, 0x2000, 0x2002, 0x2008, 0x200a, 0x2020, 0x2022, 0x2028, 0x202a,
    0x2080, 0x2082, 0x2088, 0x208a, 0x20a0, 0x20a2, 0x20a8, 0x20aa, 0x2200, 0x2202, 0x2208, 0x220a,
    0x2220, 0x2222, 0x2228, 0x222a, 0x2280, 0x2282, 0x2288, 0x228a, 0x22a0, 0x22a2, 0x22a8, 0x22aa,
    0x2800, 0x2802, 0x2808, 0x280a, 0x2820, 0x2822, 0x2828, 0x282a, 0x2880, 0x2882, 0x2888, 0x288a,
    0x28a0, 0x28a2, 0x28a8, 0x28aa, 0x2a00, 0x2a02, 0x2a08, 0x2a0a, 0x2a20, 0x2a22, 0x2a28, 0x2a2a,
    0x2a80, 0x2a82, 0x2a88, 0x2a8a, 0x2aa0, 0x2aa2, 0x2aa8, 0x2aaa, 0x8000, 0x8002, 0x8008, 0x800a,
    0x8020, 0x8022, 0x8028, 0x802a, 0x8080, 0x8082, 0x8088, 0x808a, 0x80a0, 0x80a2, 0x80a8, 0x80aa,
    0x8200, 0x8202, 0x8208, 0x820a, 0x8220, 0x8222, 0x8228, 0x822a, 0x8280, 0x8282, 0x8288, 0x828a,
    0x82a0, 0x82a2, 0x82a8, 0x82aa, 0x8800, 0x8802, 0x8808, 0x880a, 0x8820, 0x8822, 0x8828, 0x882a,
    0x8880, 0x8882, 0x8888, 0x888a, 0x88a0, 0x88a2, 0x88a8, 0x88aa, 0x8a00, 0x8a02, 0x8a08, 0x8a0a,
    0x8a20, 0x8a22, 0x8a28, 0x8a2a, 0x8a80, 0x8a82, 0x8a88, 0x8a8a, 0x8aa0, 0x8aa2, 0x8aa8, 0x8aaa,
    0xa000, 0xa002, 0xa008, 0xa00a, 0xa020, 0xa022, 0xa028, 0xa02a, 0xa080, 0xa082, 0xa088, 0xa08a,
    0xa0a0, 0xa0a2, 0xa0a8, 0xa0aa, 0xa200, 0xa202, 0xa208, 0xa20a, 0xa220, 0xa222, 0xa228, 0xa22a,
    0xa280, 0xa282, 0xa288, 0xa28a, 0xa2a0, 0xa2a2, 0xa2a8, 0xa2aa, 0xa800, 0xa802, 0xa808, 0xa80a,
    0xa820, 0xa822, 0xa828, 0xa82a, 0xa880, 0xa882, 0xa888, 0xa88a, 0xa8a0, 0xa8a2, 0xa8a8, 0xa8aa,
    0xaa00, 0xaa02, 0xaa08, 0xaa0a, 0xaa20, 0xaa22, 0xaa28, 0xaa2a, 0xaa80, 0xaa82, 0xaa88, 0xaa8a,
    0xaaa0, 0xaaa2, 0xaaa8, 0xaaaa,
};

static inline int min(int x, int y) {
    return x < y ? x : y;
}
static inline int max(int x, int y) {
    return x > y ? x : y;
}

// status tracker for row skipping
uint32_t skipping;

__attribute__((optimize("O3"))) void IRAM_ATTR
reorder_line_buffer(uint32_t* line_data, int buf_len) {
    for (uint32_t i = 0; i < buf_len / 4; i++) {
        uint32_t val = *line_data;
        *(line_data++) = val >> 16 | ((val & 0x0000FFFF) << 16);
    }
}

__attribute__((optimize("O3"))) void IRAM_ATTR
bit_shift_buffer_right(uint8_t* buf, uint32_t len, int shift) {
    uint8_t carry = 0xFF << (8 - shift);
    for (uint32_t i = 0; i < len; i++) {
        uint8_t val = buf[i];
        buf[i] = (val >> shift) | carry;
        carry = val << (8 - shift);
    }
}

__attribute__((optimize("O3"))) void IRAM_ATTR
nibble_shift_buffer_right(uint8_t* buf, uint32_t len) {
    uint8_t carry = 0xF;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t val = buf[i];
        buf[i] = (val << 4) | carry;
        carry = (val & 0xF0) >> 4;
    }
}

__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_8ppB(
    const uint32_t* line_data, uint8_t* epd_input, const uint8_t* lut, uint32_t epd_width
) {
    uint32_t* wide_epd_input = (uint32_t*)epd_input;
    uint8_t* data_ptr = (uint8_t*)line_data;
    uint32_t* lut_32 = (uint32_t*)lut;
    // this is reversed for little-endian, but this is later compensated
    // through the output peripheral.
    for (int j = 0; j < epd_width / 16; j++) {
        uint8_t v1 = *(data_ptr++);
        uint8_t v2 = *(data_ptr++);
        wide_epd_input[j] = (lut_32[v2] << 16) | lut_32[v1];
    }

    // Account for missing line end if epd_width is not divisible by 16.
    // We assume divisibility by 4.
    for (int j = 0; j < (epd_width % 16) / 4; j++) {
        uint8_t nibble = *data_ptr;
        if (j % 2 == 1) {
            nibble = nibble >> 4;
            data_ptr++;
        } else {
            nibble = nibble & 0xF;
        }
        epd_input[(epd_width / 16) * 4 + j] = lut_32[nibble];
    }
}

/**
 * Look up 4 pixels of a differential image in a LUT constructed for use with vector extensions.
 */
__attribute__((optimize("O3"))) static inline uint8_t lookup_pixels_in_VE_LUT(
    const uint32_t in, const uint8_t* conversion_lut
) {
    uint32_t* padded_lut = (uint32_t*)conversion_lut;
    uint8_t out = padded_lut[(in >> 24) & 0xFF] << 6;
    out |= padded_lut[(in >> 16) & 0xFF] << 4;
    out |= padded_lut[(in >> 8) & 0xFF] << 2;
    out |= padded_lut[(in >> 0) & 0xFF];
    return out;
}

/**
 * Lookup accelerated by the S3 Vector Extensions.
 * Expects aligned buffers and a length that is divisible by 16.
 */
void IRAM_ATTR calc_epd_input_1ppB_1k_S3_VE_aligned(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
);

#ifdef RENDER_METHOD_I2S
void calc_epd_input_1ppB_1k_S3_VE_aligned(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    // dummy implementation, should never be called.
    abort();
}
#endif

/**
 * Lookup accelerated by the S3 Vector Extensions.
 * Uses a 1K padded LUT (each entry takes up 32 bits)
 */
void IRAM_ATTR calc_epd_input_1ppB_1k_S3_VE(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    // alignment boundaries in pixels
    int unaligned_len_front = (16 - (uint32_t)ld % 16) % 16;
    int unaligned_len_back = ((uint32_t)ld + epd_width) % 16;
    int aligned_len = epd_width - unaligned_len_front - unaligned_len_back;

    if (unaligned_len_front) {
        for (int i = 0; i < unaligned_len_front / 4; i++) {
            (*epd_input++) = lookup_pixels_in_VE_LUT((*ld++), conversion_lut);
        }
    }
    calc_epd_input_1ppB_1k_S3_VE_aligned(ld, epd_input, conversion_lut, aligned_len);

    ld += aligned_len / 4;
    epd_input += aligned_len / 4;

    if (unaligned_len_back) {
        for (int i = 0; i < unaligned_len_back / 4; i++) {
            (*epd_input++) = lookup_pixels_in_VE_LUT((*ld++), conversion_lut);
        }
    }
}

/**
 * Calculate EPD input for a difference image with one pixel per byte.
 */
__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_1ppB_64k(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    const uint16_t* lp = (uint16_t*)ld;
    for (uint32_t j = 0; j < epd_width / 4; j++) {
        epd_input[j] = (conversion_lut[lp[2 * j + 1]] << 4) | conversion_lut[lp[2 * j]];
    }
}

__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_2ppB_lut_64k(
    const uint32_t* line_data, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    const uint16_t* line_data_16 = (const uint16_t*)line_data;

    for (uint32_t j = 0; j < epd_width / 4; j++) {
        epd_input[j] = conversion_lut[*(line_data_16++)];
    }
}

/**
 * Look up 4 pixels in a 1K LUT with fixed "from" value.
 */
__attribute__((optimize("O3"))) static uint8_t lookup_pixels_2ppB_1k(
    uint16_t in, const uint8_t* conversion_lut, uint8_t from
) {
    uint8_t v;
    uint8_t out;

    v = ((in << 4) | from);
    out = conversion_lut[v & 0xFF];
    v = ((in & 0xF0) | from);
    out |= (conversion_lut + 0x100)[v & 0xFF];
    in = in >> 8;
    v = ((in << 4) | from);
    out |= (conversion_lut + 0x200)[v & 0xFF];
    v = ((in & 0xF0) | from);
    out |= (conversion_lut + 0x300)[v];
    return out;
}

/**
 * Calculate EPD input for a 2ppB buffer, but with a difference image LUT.
 * This is used for small-LUT mode.
 */
__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_2ppB_1k_lut(
    const uint32_t* ld,
    uint8_t* epd_input,
    const uint8_t* conversion_lut,
    uint8_t from,
    uint32_t epd_width
) {
    const uint16_t* line_data_16 = (const uint16_t*)ld;

    for (uint32_t j = 0; j < epd_width / 4; j++) {
        epd_input[j] = lookup_pixels_2ppB_1k(*(line_data_16++), conversion_lut, from);
    };
}

__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_2ppB_1k_lut_white(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    calc_epd_input_2ppB_1k_lut(ld, epd_input, conversion_lut, 0xF, epd_width);
}

__attribute__((optimize("O3"))) void IRAM_ATTR calc_epd_input_2ppB_1k_lut_black(
    const uint32_t* ld, uint8_t* epd_input, const uint8_t* conversion_lut, uint32_t epd_width
) {
    calc_epd_input_2ppB_1k_lut(ld, epd_input, conversion_lut, 0x0, epd_width);
}

///////////////////////////// Calculate Lookup Tables
//////////////////////////////////

/**
 * Unpack the waveform data into a lookup table, with bit shifted copies.
 */
__attribute__((optimize("O3"))) static void IRAM_ATTR
build_2ppB_lut_1k(uint8_t* lut, const EpdWaveformPhases* phases, int frame) {
    const uint8_t* p_lut = phases->luts + (16 * 4 * frame);
    for (uint8_t to = 0; to < 16; to++) {
        for (uint8_t from_packed = 0; from_packed < 4; from_packed++) {
            uint8_t index = (to << 4) | (from_packed * 4);
            uint8_t packed = *(p_lut++);
            lut[index] = (packed >> 6) & 3;
            lut[index + 1] = (packed >> 4) & 3;
            lut[index + 2] = (packed >> 2) & 3;
            lut[index + 3] = (packed >> 0) & 3;
            // printf("%2X%2X%2X%2X (%d)", lut[index], lut[index + 1], lut[index + 2],
            // lut[index + 3], index);
        }
        // printf("\n");
    }
    uint32_t index = 0x100;
    for (uint8_t s = 2; s <= 6; s += 2) {
        for (int i = 0; i < 0x100; i++) {
            lut[index] = lut[index % 0x100] << s;
            index++;
        }
    }
}

/**
 * Unpack the waveform data into a lookup table,
 * 64k to loop up two bytes at once
 */
__attribute__((optimize("O3"))) static void IRAM_ATTR
build_1ppB_lut_64k(uint8_t* lut, const EpdWaveformPhases* phases, int frame) {
    const uint8_t* p_lut = phases->luts + (16 * 4 * frame);
    for (uint8_t to = 0; to < 16; to++) {
        for (uint8_t from_packed = 0; from_packed < 4; from_packed++) {
            uint8_t index = (to << 4) | (from_packed * 4);
            uint8_t packed = *(p_lut++);
            lut[index] = (packed >> 6) & 3;
            lut[index + 1] = (packed >> 4) & 3;
            lut[index + 2] = (packed >> 2) & 3;
            lut[index + 3] = (packed >> 0) & 3;
            // printf("%2X%2X%2X%2X (%d)", lut[index], lut[index + 1], lut[index + 2],
            // lut[index + 3], index);
        }
        // printf("\n");
    }

    for (int outer = 0xFF; outer >= 0; outer--) {
        uint32_t outer_result = lut[outer] << 2;
        outer_result |= (outer_result << 16);
        outer_result |= (outer_result << 8);
        uint32_t* lut_section = (uint32_t*)(&lut[outer << 8]);
        memcpy(lut_section, lut, 0x100);
        for (int i = 0; i < 0x100 / 4; i++) {
            lut_section[i] = lut_section[i] | outer_result;
        }
    }
}

/**
 * A 32bit aligned lookup table for lookup using the ESP32-S3 vector extensions.
 */
__attribute__((optimize("O3"))) static void IRAM_ATTR
build_1ppB_lut_S3_VE_1k(uint8_t* lut, const EpdWaveformPhases* phases, int frame) {
    uint32_t* lut32 = (uint32_t*)lut;
    const uint8_t* p_lut = phases->luts + (16 * 4 * frame);
    for (uint8_t to = 0; to < 16; to++) {
        for (uint8_t from_packed = 0; from_packed < 4; from_packed++) {
            uint8_t index = (to << 4) | (from_packed * 4);
            uint8_t packed = *(p_lut++);
            lut32[index] = (packed >> 6) & 3;
            lut32[index + 1] = (packed >> 4) & 3;
            lut32[index + 2] = (packed >> 2) & 3;
            lut32[index + 3] = (packed >> 0) & 3;
        }
    }
}

/**
 * Build a 16-bit LUT from the waveform if the previous color is
 * known, e.g. all white or all black.
 * This LUT is use to look up 4 pixels at once, as with the epdiy LUT.
 */
__attribute__((optimize("O3"))) static void build_2ppB_lut_64k_static_from(
    uint8_t* lut, const EpdWaveformPhases* phases, uint8_t from, int frame
) {
    const uint8_t* p_lut = phases->luts + (16 * 4 * frame);

    /// index into the packed "from" row
    uint8_t fi = from >> 2;
    /// bit shift amount for the packed "from" row
    uint8_t fs = 6 - 2 * (from & 3);

    // populate the first 4096 bytes
    uint8_t v1 = 0;
    uint32_t s1 = 0;
    for (uint8_t t2 = 0; t2 < 16; t2++) {
        uint8_t v2 = ((p_lut[(t2 << 2) + fi] >> fs) & 0x03) << 4;
        uint32_t s2 = t2 << 8;
        for (uint8_t t3 = 0; t3 < 16; t3++) {
            uint8_t v3 = ((p_lut[(t3 << 2) + fi] >> fs) & 0x03) << 2;
            uint32_t s3 = t3 << 4;
            for (uint8_t t4 = 0; t4 < 16; t4++) {
                uint8_t v4 = ((p_lut[(t4 << 2) + fi] >> fs) & 0x03) << 0;
                uint32_t s4 = t4;
                lut[s1 | s2 | s3 | s4] = v1 | v2 | v3 | v4;
            }
        }
    }

    // now just copy and the first 4096 bytes and add the upper two bits
    for (uint8_t t1 = 1; t1 < 16; t1++) {
        memcpy(&lut[t1 << 12], lut, 1 << 12);
    }

    for (int i = 0; i < 16; i++) {
        uint32_t v1 = ((p_lut[(i << 2) + fi] >> fs) & 0x03);
        uint32_t mask = (v1 << 30) | (v1 << 22) | (v1 << 14) | (v1 << 6);
        for (int j = 0; j < 16 * 16 * 16 / 4; j++) {
            ((uint32_t*)lut)[(i << 10) + j] |= mask;
        }
    }
}

static void build_2ppB_lut_64k_from_0(uint8_t* lut, const EpdWaveformPhases* phases, int frame) {
    build_2ppB_lut_64k_static_from(lut, phases, 0, frame);
}

static void build_2ppB_lut_64k_from_15(uint8_t* lut, const EpdWaveformPhases* phases, int frame) {
    build_2ppB_lut_64k_static_from(lut, phases, 0xF, frame);
}

static void build_8ppB_lut_256b_from_white(
    uint8_t* lut, const EpdWaveformPhases* phases, int frame
) {
    memcpy(lut, lut_8ppB_start_at_white, sizeof(lut_8ppB_start_at_white));
}

static void build_8ppB_lut_256b_from_black(
    uint8_t* lut, const EpdWaveformPhases* phases, int frame
) {
    memcpy(lut, lut_8ppB_start_at_black, sizeof(lut_8ppB_start_at_black));
}

void IRAM_ATTR epd_apply_line_mask(uint8_t* buf, const uint8_t* mask, int len) {
    for (int i = 0; i < len / 4; i++) {
        ((uint32_t*)buf)[i] &= ((uint32_t*)mask)[i];
    }
}

LutFunctionPair find_lut_functions(enum EpdDrawMode mode, uint32_t lut_size) {
    LutFunctionPair pair;
    pair.build_func = NULL;
    pair.lookup_func = NULL;

    if (mode & MODE_PACKING_1PPB_DIFFERENCE) {
        if (EPD_CURRENT_RENDER_METHOD == RENDER_METHOD_LCD && !(mode & MODE_FORCE_NO_PIE)
            && lut_size >= 1024) {
            pair.build_func = &build_1ppB_lut_S3_VE_1k;
            pair.lookup_func = &calc_epd_input_1ppB_1k_S3_VE;
            return pair;
        } else if (lut_size >= 1 << 16) {
            pair.build_func = &build_1ppB_lut_64k;
            pair.lookup_func = &calc_epd_input_1ppB_64k;
            return pair;
        }
    } else if (mode & MODE_PACKING_2PPB) {
        if (lut_size >= 1 << 16) {
            if (mode & PREVIOUSLY_WHITE) {
                pair.build_func = &build_2ppB_lut_64k_from_15;
                pair.lookup_func = &calc_epd_input_2ppB_lut_64k;
                return pair;
            } else if (mode & PREVIOUSLY_BLACK) {
                pair.build_func = &build_2ppB_lut_64k_from_0;
                pair.lookup_func = &calc_epd_input_2ppB_lut_64k;
                return pair;
            }
        } else if (lut_size >= 1024) {
            if (mode & PREVIOUSLY_WHITE) {
                pair.build_func = &build_2ppB_lut_1k;
                pair.lookup_func = &calc_epd_input_2ppB_1k_lut_white;
                return pair;
            } else if (mode & PREVIOUSLY_BLACK) {
                pair.build_func = &build_2ppB_lut_1k;
                pair.lookup_func = &calc_epd_input_2ppB_1k_lut_black;
                return pair;
            }
        }
    } else if (mode & MODE_PACKING_8PPB) {
        if (lut_size < sizeof(lut_8ppB_start_at_white)) {
            return pair;
        }

        if (mode & PREVIOUSLY_WHITE) {
            pair.build_func = &build_8ppB_lut_256b_from_white;
            pair.lookup_func = &calc_epd_input_8ppB;
            return pair;
        } else if (mode & PREVIOUSLY_BLACK) {
            pair.build_func = &build_8ppB_lut_256b_from_black;
            pair.lookup_func = &calc_epd_input_8ppB;
            return pair;
        }
    }

    return pair;
}
