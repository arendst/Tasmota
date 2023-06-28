/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "nist256p1.h"

const ecdsa_curve nist256p1 = {
	/* .prime */ {
		/*.val =*/ {0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3f, 0x0, 0x0, 0x1000, 0x3fffc000, 0xffff}
	},

	/* G */ {
		/*.x =*/{/*.val =*/{0x1898c296, 0x1284e517, 0x1eb33a0f, 0xdf604b, 0x2440f277, 0x339b958e, 0x4247f8b, 0x347cb84b, 0x6b17}},
		/*.y =*/{/*.val =*/{0x37bf51f5, 0x2ed901a0, 0x3315ecec, 0x338cd5da, 0xf9e162b, 0x1fad29f0, 0x27f9b8ee, 0x10b8bf86, 0x4fe3}}
	},

	/* order */ {
		/*.val =*/{0x3c632551, 0xee72b0b, 0x3179e84f, 0x39beab69, 0x3fffffbc, 0x3fffffff, 0xfff, 0x3fffc000, 0xffff}
	},

	/* order_half */ {
		/*.val =*/{0x3e3192a8, 0x27739585, 0x38bcf427, 0x1cdf55b4, 0x3fffffde, 0x3fffffff, 0x7ff, 0x3fffe000, 0x7fff}
	},

	/* a */ -3,

	/* b */ {
		/*.val =*/{0x27d2604b, 0x2f38f0f8, 0x53b0f63, 0x741ac33, 0x1886bc65, 0x2ef555da, 0x293e7b3e, 0xd762a8e, 0x5ac6}
	}

#if USE_PRECOMPUTED_CP
	,
	/* cp */ {
#include "nist256p1.table"
	}
#endif
};

const curve_info nist256p1_info = {
	.bip32_name = "Nist256p1 seed",
	.params = &nist256p1,
	.hasher_type = HASHER_SHA2,
};
