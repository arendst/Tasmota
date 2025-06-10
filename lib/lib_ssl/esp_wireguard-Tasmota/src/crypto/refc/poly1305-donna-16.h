/*
	poly1305 implementation using 16 bit * 16 bit = 32 bit multiplication and 32 bit addition
*/

#pragma GCC optimize ("Os")

#if defined(_MSC_VER)
	#define POLY1305_NOINLINE __declspec(noinline)
#elif defined(__GNUC__)
	#define POLY1305_NOINLINE __attribute__((noinline))
#else
	#define POLY1305_NOINLINE
#endif

#define poly1305_block_size 16

/* 17 + sizeof(size_t) + 18*sizeof(unsigned short) */
typedef struct poly1305_state_internal_t {
	unsigned char buffer[poly1305_block_size];
	size_t leftover;
	unsigned short r[10];
	unsigned short h[10];
	unsigned short pad[8];
	unsigned char final;
} poly1305_state_internal_t;

/* interpret two 8 bit unsigned integers as a 16 bit unsigned integer in little endian */
static unsigned short
U8TO16(const unsigned char *p) {
	return
		(((unsigned short)(p[0] & 0xff)      ) |
	     ((unsigned short)(p[1] & 0xff) <<  8));
}

/* store a 16 bit unsigned integer as two 8 bit unsigned integers in little endian */
static void
U16TO8(unsigned char *p, unsigned short v) {
	p[0] = (v      ) & 0xff;
	p[1] = (v >>  8) & 0xff;
}

void
poly1305_init(poly1305_context *ctx, const unsigned char key[32]) {
	poly1305_state_internal_t *st = (poly1305_state_internal_t *)ctx;
	unsigned short t0,t1,t2,t3,t4,t5,t6,t7;
	size_t i;

	/* r &= 0xffffffc0ffffffc0ffffffc0fffffff */
	t0 = U8TO16(&key[ 0]); st->r[0] = ( t0                    ) & 0x1fff;
	t1 = U8TO16(&key[ 2]); st->r[1] = ((t0 >> 13) | (t1 <<  3)) & 0x1fff;
	t2 = U8TO16(&key[ 4]); st->r[2] = ((t1 >> 10) | (t2 <<  6)) & 0x1f03;
	t3 = U8TO16(&key[ 6]); st->r[3] = ((t2 >>  7) | (t3 <<  9)) & 0x1fff;
	t4 = U8TO16(&key[ 8]); st->r[4] = ((t3 >>  4) | (t4 << 12)) & 0x00ff;
	                       st->r[5] = ((t4 >>  1)             ) & 0x1ffe;
	t5 = U8TO16(&key[10]); st->r[6] = ((t4 >> 14) | (t5 <<  2)) & 0x1fff;
	t6 = U8TO16(&key[12]); st->r[7] = ((t5 >> 11) | (t6 <<  5)) & 0x1f81;
	t7 = U8TO16(&key[14]); st->r[8] = ((t6 >>  8) | (t7 <<  8)) & 0x1fff;
	                       st->r[9] = ((t7 >>  5)             ) & 0x007f;

	/* h = 0 */
	for (i = 0; i < 10; i++)
		st->h[i] = 0;

	/* save pad for later */
	for (i = 0; i < 8; i++)
		st->pad[i] = U8TO16(&key[16 + (2 * i)]);

	st->leftover = 0;
	st->final = 0;
}

static void
poly1305_blocks(poly1305_state_internal_t *st, const unsigned char *m, size_t bytes) {
	const unsigned short hibit = (st->final) ? 0 : (1 << 11); /* 1 << 128 */
	unsigned short t0,t1,t2,t3,t4,t5,t6,t7;
	unsigned long d[10];
	unsigned long c;

	while (bytes >= poly1305_block_size) {
		size_t i, j;

		/* h += m[i] */
		t0 = U8TO16(&m[ 0]); st->h[0] += ( t0                    ) & 0x1fff;
		t1 = U8TO16(&m[ 2]); st->h[1] += ((t0 >> 13) | (t1 <<  3)) & 0x1fff;
		t2 = U8TO16(&m[ 4]); st->h[2] += ((t1 >> 10) | (t2 <<  6)) & 0x1fff;
		t3 = U8TO16(&m[ 6]); st->h[3] += ((t2 >>  7) | (t3 <<  9)) & 0x1fff;
		t4 = U8TO16(&m[ 8]); st->h[4] += ((t3 >>  4) | (t4 << 12)) & 0x1fff;
		                     st->h[5] += ((t4 >>  1)             ) & 0x1fff;
		t5 = U8TO16(&m[10]); st->h[6] += ((t4 >> 14) | (t5 <<  2)) & 0x1fff;
		t6 = U8TO16(&m[12]); st->h[7] += ((t5 >> 11) | (t6 <<  5)) & 0x1fff;
		t7 = U8TO16(&m[14]); st->h[8] += ((t6 >>  8) | (t7 <<  8)) & 0x1fff;
		                     st->h[9] += ((t7 >>  5)             ) | hibit;

		/* h *= r, (partial) h %= p */
		for (i = 0, c = 0; i < 10; i++) {
			d[i] = c;
			for (j = 0; j < 10; j++) {
				d[i] += (unsigned long)st->h[j] * ((j <= i) ? st->r[i - j] : (5 * st->r[i + 10 - j]));
				/* Sum(h[i] * r[i] * 5) will overflow slightly above 6 products with an unclamped r, so carry at 5 */
				if (j == 4) {
					c = (d[i] >> 13);
					d[i] &= 0x1fff;
				}
			}
			c += (d[i] >> 13);
			d[i] &= 0x1fff;
		}
		c = ((c << 2) + c); /* c *= 5 */
		c += d[0];
		d[0] = ((unsigned short)c & 0x1fff);
		c = (c >> 13);
		d[1] += c;

		for (i = 0; i < 10; i++)
			st->h[i] = (unsigned short)d[i];

		m += poly1305_block_size;
		bytes -= poly1305_block_size;
	}
}

POLY1305_NOINLINE void
poly1305_finish(poly1305_context *ctx, unsigned char mac[16]) {
	poly1305_state_internal_t *st = (poly1305_state_internal_t *)ctx;
	unsigned short c;
	unsigned short g[10];
	unsigned short mask;
	unsigned long f;
	size_t i;

	/* process the remaining block */
	if (st->leftover) {
		size_t i = st->leftover;
		st->buffer[i++] = 1;
		for (; i < poly1305_block_size; i++)
			st->buffer[i] = 0;
		st->final = 1;
		poly1305_blocks(st, st->buffer, poly1305_block_size);
	}

	/* fully carry h */
	c = st->h[1] >> 13;
	st->h[1] &= 0x1fff;
	for (i = 2; i < 10; i++) {
		st->h[i] += c;
		c = st->h[i] >> 13;
		st->h[i] &= 0x1fff;
	}
	st->h[0] += (c * 5);
	c = st->h[0] >> 13;
	st->h[0] &= 0x1fff;
	st->h[1] += c;
	c = st->h[1] >> 13;
	st->h[1] &= 0x1fff;
	st->h[2] += c;

	/* compute h + -p */
	g[0] = st->h[0] + 5;
	c = g[0] >> 13;
	g[0] &= 0x1fff;
	for (i = 1; i < 10; i++) {
		g[i] = st->h[i] + c;
		c = g[i] >> 13;
		g[i] &= 0x1fff;
	}

	/* select h if h < p, or h + -p if h >= p */
	mask = (c ^ 1) - 1;
	for (i = 0; i < 10; i++)
		g[i] &= mask;
	mask = ~mask;
	for (i = 0; i < 10; i++)
		st->h[i] = (st->h[i] & mask) | g[i];

	/* h = h % (2^128) */
	st->h[0] = ((st->h[0]      ) | (st->h[1] << 13)                   ) & 0xffff;
	st->h[1] = ((st->h[1] >>  3) | (st->h[2] << 10)                   ) & 0xffff;
	st->h[2] = ((st->h[2] >>  6) | (st->h[3] <<  7)                   ) & 0xffff;
	st->h[3] = ((st->h[3] >>  9) | (st->h[4] <<  4)                   ) & 0xffff;
	st->h[4] = ((st->h[4] >> 12) | (st->h[5] <<  1) | (st->h[6] << 14)) & 0xffff;
	st->h[5] = ((st->h[6] >>  2) | (st->h[7] << 11)                   ) & 0xffff;
	st->h[6] = ((st->h[7] >>  5) | (st->h[8] <<  8)                   ) & 0xffff;
	st->h[7] = ((st->h[8] >>  8) | (st->h[9] <<  5)                   ) & 0xffff;

	/* mac = (h + pad) % (2^128) */
	f = (unsigned long)st->h[0] + st->pad[0];
	st->h[0] = (unsigned short)f;
	for (i = 1; i < 8; i++) {
		f = (unsigned long)st->h[i] + st->pad[i] + (f >> 16);
		st->h[i] = (unsigned short)f;
	}

	for (i = 0; i < 8; i++)
		U16TO8(mac + (i * 2), st->h[i]);

	/* zero out the state */
	for (i = 0; i < 10; i++)
		st->h[i] = 0;
	for (i = 0; i < 10; i++)
		st->r[i] = 0;
	for (i = 0; i < 8; i++)
		st->pad[i] = 0;
}
