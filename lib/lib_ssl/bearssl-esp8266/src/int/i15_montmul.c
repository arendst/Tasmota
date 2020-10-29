/*
 * Copyright (c) 2017 Thomas Pornin <pornin@bolet.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "t_inner.h"

/* see inner.h */
void
br_i15_montymul(uint16_t *d, const uint16_t *x, const uint16_t *y,
	const uint16_t *m, uint16_t m0i)
{
	size_t len, len4, u, v;
	uint32_t dh;

	len = (pgm_read_word(&m[0]) + 15) >> 4;
	len4 = len & ~(size_t)3;
	br_i15_zero(d, pgm_read_word(&m[0]));
	dh = 0;
	for (u = 0; u < len; u ++) {
		uint32_t f, xu, r, zh;

		xu = pgm_read_word(&x[u + 1]);
		f = MUL15((d[1] + MUL15(pgm_read_word(&x[u + 1]), pgm_read_word(&y[1]))) & 0x7FFF, m0i)
			& 0x7FFF;
#if BR_ARMEL_CORTEXM_GCC
		if (len4 != 0) {
			uint16_t *limit;

			limit = d + len4;
			asm volatile (
"\n\
	@ carry: r=r2                                              \n\
	@ multipliers: xu=r3 f=r4                                  \n\
	@ base registers: d+v=r5 y+v=r6 m+v=r7                     \n\
	@ r8 contains 0x7FFF                                       \n\
	@ r9 contains d+len4                                       \n\
	ldr	r0, %[limit]                                       \n\
	ldr	r3, %[xu]                                          \n\
	mov	r9, r0                                             \n\
	ldr	r4, %[f]                                           \n\
	eor	r2, r2                                             \n\
	ldr	r5, %[d]                                           \n\
	sub	r1, r2, #1                                         \n\
	ldr	r6, %[y]                                           \n\
	lsr	r1, r1, #17                                        \n\
	ldr	r7, %[m]                                           \n\
	mov	r8, r1                                             \n\
loop%=:                                                            \n\
	ldrh	r0, [r6, #2]                                       \n\
	ldrh	r1, [r7, #2]                                       \n\
	mul	r0, r3                                             \n\
	mul	r1, r4                                             \n\
	add	r2, r0, r2                                         \n\
	ldrh	r0, [r5, #2]                                       \n\
	add	r2, r1, r2                                         \n\
	mov	r1, r8                                             \n\
	add	r2, r0, r2                                         \n\
	and	r1, r2                                             \n\
	lsr	r2, r2, #15                                        \n\
	strh	r1, [r5, #0]                                       \n\
		                                                   \n\
	ldrh	r0, [r6, #4]                                       \n\
	ldrh	r1, [r7, #4]                                       \n\
	mul	r0, r3                                             \n\
	mul	r1, r4                                             \n\
	add	r2, r0, r2                                         \n\
	ldrh	r0, [r5, #4]                                       \n\
	add	r2, r1, r2                                         \n\
	mov	r1, r8                                             \n\
	add	r2, r0, r2                                         \n\
	and	r1, r2                                             \n\
	lsr	r2, r2, #15                                        \n\
	strh	r1, [r5, #2]                                       \n\
		                                                   \n\
	ldrh	r0, [r6, #6]                                       \n\
	ldrh	r1, [r7, #6]                                       \n\
	mul	r0, r3                                             \n\
	mul	r1, r4                                             \n\
	add	r2, r0, r2                                         \n\
	ldrh	r0, [r5, #6]                                       \n\
	add	r2, r1, r2                                         \n\
	mov	r1, r8                                             \n\
	add	r2, r0, r2                                         \n\
	and	r1, r2                                             \n\
	lsr	r2, r2, #15                                        \n\
	strh	r1, [r5, #4]                                       \n\
		                                                   \n\
	ldrh	r0, [r6, #8]                                       \n\
	ldrh	r1, [r7, #8]                                       \n\
	mul	r0, r3                                             \n\
	mul	r1, r4                                             \n\
	add	r2, r0, r2                                         \n\
	ldrh	r0, [r5, #8]                                       \n\
	add	r2, r1, r2                                         \n\
	mov	r1, r8                                             \n\
	add	r2, r0, r2                                         \n\
	and	r1, r2                                             \n\
	lsr	r2, r2, #15                                        \n\
	strh	r1, [r5, #6]                                       \n\
		                                                   \n\
	add	r5, r5, #8                                         \n\
	add	r6, r6, #8                                         \n\
	add	r7, r7, #8                                         \n\
	cmp	r5, r9                                             \n\
	bne	loop%=                                             \n\
		                                                   \n\
	str	r2, %[carry]                                       \n\
"
: [carry] "=m" (r)
: [xu] "m" (xu), [f] "m" (f), [d] "m" (d), [y] "m" (y),
	[m] "m" (m), [limit] "m" (limit)
: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9" );
		} else {
			r = 0;
		}
		v = len4;
#else
		r = 0;
		const uint16_t *py;
		const uint16_t *pm;

		py = &y[0];				// addresses of both arrays that will be scanned as uint16_t
		pm = &m[0];
		int py_unaligned = (((intptr_t)py) & 2) != 0;
		int pm_unaligned = (((intptr_t)pm) & 2) != 0;
		uint32_t ty, tm;	// 32 bits buffers
		if (!py_unaligned && !pm_unaligned) {
			// both are aligned to 32 bits, we always skip the first 16 bits
			ty = *(uint32_t*)py;		// pre-load with 32 bits value, next value will be loaded at end of loop iteration
			tm = *(uint32_t*)pm;
			for (v = 0; v < len4; v += 4) {
				uint16_t y1, y2, y3, y4;		// we define 4 variables for easy reading
				uint16_t m1, m2, m3, m4;		// but optimizer will collapse them into 1

				uint32_t z;

				y1 = ty >> 16;		// v+1, get upper 16 bits current 32 bits
				m1 = tm >> 16;
				z = d[v + 1] + MUL15(xu, y1) + MUL15(f, m1) + r;
				r = z >> 15;
				d[v + 0] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y2 = ty;											// get lower 16 bits
				tm = *(uint32_t*)(pm = pm + 2);
				m2 = tm;
				z = d[v + 2] + MUL15(xu, y2) + MUL15(f, m2) + r;
				r = z >> 15;
				d[v + 1] = z & 0x7FFF;
				//
				y3 = ty >> 16;
				m3 = tm >> 16;
				z = d[v + 3] + MUL15(xu, y3) + MUL15(f, m3) + r;
				r = z >> 15;
				d[v + 2] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y4 = ty;											// get lower 16 bits
				tm = *(uint32_t*)(pm = pm + 2);
				m4 = tm;
				z = d[v + 4] + MUL15(xu, y4) + MUL15(f, m4) + r;
				r = z >> 15;
				d[v + 3] = z & 0x7FFF;
			}
		} else if (!py_unaligned && pm_unaligned) {
			pm--;		// we decrement by 1 because will increment by 2 at beginning of loop
			ty = *(uint32_t*)py;		// pre-load with 32 bits value
			for (v = 0; v < len4; v += 4) {
				uint16_t y1, y2, y3, y4;
				uint16_t m1, m2, m3, m4;
				uint32_t z;

				y1 = ty >> 16;		// +1
				tm = *(uint32_t*)(pm = pm + 2);
				m1 = tm;
				z = d[v + 1] + MUL15(xu, y1) + MUL15(f, m1) + r;
				r = z >> 15;
				d[v + 0] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y2 = ty;
				m2 = tm >> 16;
				z = d[v + 2] + MUL15(xu, y2) + MUL15(f, m2) + r;
				r = z >> 15;
				d[v + 1] = z & 0x7FFF;
				//
				y3 = ty >> 16;
				tm = *(uint32_t*)(pm = pm + 2);
				m3 = tm;
				z = d[v + 3] + MUL15(xu, y3) + MUL15(f, m3) + r;
				r = z >> 15;
				d[v + 2] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y4 = ty;
				m4 = tm >> 16;
				z = d[v + 4] + MUL15(xu, y4) + MUL15(f, m4) + r;
				r = z >> 15;
				d[v + 3] = z & 0x7FFF;
			}
		} else if (py_unaligned && !pm_unaligned) {		// buggy
			// py unaligned, pm aligned
			py--;
			tm = *(uint32_t*)pm;
			for (v = 0; v < len4; v += 4) {
				uint16_t y1, y2, y3, y4;
				uint16_t m1, m2, m3, m4;
				uint32_t z;

				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y1 = ty;
				m1 = tm >> 16;
				z = d[v + 1] + MUL15(xu, y1) + MUL15(f, m1) + r;
				r = z >> 15;
				d[v + 0] = z & 0x7FFF;
				//
				y2 = ty >> 16;
				tm = *(uint32_t*)(pm = pm + 2);
				m2 = tm;
				z = d[v + 2] + MUL15(xu, y2) + MUL15(f, m2) + r;
				r = z >> 15;
				d[v + 1] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y3 = ty;
				m3 = tm >> 16;
				z = d[v + 3] + MUL15(xu, y3) + MUL15(f, m3) + r;
				r = z >> 15;
				d[v + 2] = z & 0x7FFF;
				//
				y4 = ty >> 16;
				tm = *(uint32_t*)(pm = pm + 2);
				m4 = tm;
				z = d[v + 4] + MUL15(xu, y4) + MUL15(f, m4) + r;
				r = z >> 15;
				d[v + 3] = z & 0x7FFF;
			}
		} else { // if (py_unaligned && pm_unaligned) {
			// py unaligned, pm unaligned
			py--;
			pm--;
			for (v = 0; v < len4; v += 4) {
				uint16_t y1, y2, y3, y4;
				uint16_t m1, m2, m3, m4;
				uint32_t z;

				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y1 = ty;		// +1
				tm = *(uint32_t*)(pm = pm + 2);
				m1 = tm;
				z = d[v + 1] + MUL15(xu, y1) + MUL15(f, m1) + r;
				r = z >> 15;
				d[v + 0] = z & 0x7FFF;
				//
				y2 = ty >> 16;
				m2 = tm >> 16;
				z = d[v + 2] + MUL15(xu, y2) + MUL15(f, m2) + r;
				r = z >> 15;
				d[v + 1] = z & 0x7FFF;
				//
				ty = *(uint32_t*)(py = py + 2);		// next 32 bits
				y3 = ty;
				tm = *(uint32_t*)(pm = pm + 2);
				m3 = tm;
				z = d[v + 3] + MUL15(xu, y3) + MUL15(f, m3) + r;
				r = z >> 15;
				d[v + 2] = z & 0x7FFF;
				//
				y4 = ty >> 16;
				m4 = tm >> 16;
				z = d[v + 4] + MUL15(xu, y4) + MUL15(f, m4) + r;
				r = z >> 15;
				d[v + 3] = z & 0x7FFF;
			}
		}
#endif  // BR_ARMEL_CORTEXM_GCC
		for (; v < len; v ++) {
			uint32_t z;

			z = d[v + 1] + MUL15(xu, pgm_read_word(&y[v + 1]))
				+ MUL15(f, pgm_read_word(&m[v + 1])) + r;
			r = z >> 15;
			d[v + 0] = z & 0x7FFF;
		}

		zh = dh + r;
		d[len] = zh & 0x7FFF;
		dh = zh >> 15;
	}

	/*
	 * Restore the bit length (it was overwritten in the loop above).
	 */
	d[0] = pgm_read_word(&m[0]);

	/*
	 * d[] may be greater than m[], but it is still lower than twice
	 * the modulus.
	 */
	br_i15_sub(d, m, NEQ(dh, 0) | NOT(br_i15_sub(d, m, 0)));
}
