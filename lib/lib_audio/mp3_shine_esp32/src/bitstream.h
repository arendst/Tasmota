#ifndef BITSTREAM_H
#define BITSTREAM_H

typedef struct  bit_stream_struc {
    unsigned char *data;        /* Processed data */
    int         data_size;      /* Total data size */
    int         data_position;  /* Data position */
    unsigned int cache;			/* bit stream cache */
    int         cache_bits;     /* free bits in cache */
} bitstream_t;

/* "bit_stream.h" Definitions */

#define         MINIMUM         4    /* Minimum size of the buffer in bytes */
#define         MAX_LENGTH      32   /* Maximum length of word written or
                                        read from bit stream */

#define         BUFFER_SIZE     4096

//#define         MIN(A, B)       ((A) < (B) ? (A) : (B))
//#define         MAX(A, B)       ((A) > (B) ? (A) : (B))

void shine_open_bit_stream(bitstream_t *bs,const int size);
void shine_close_bit_stream(bitstream_t *bs);
void shine_putbits(bitstream_t *bs,unsigned int val, unsigned int N);
int  shine_get_bits_count(bitstream_t *bs);

#endif
