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

#include "secp256k1.h"

const ecdsa_curve secp256k1 = {
	/* .prime */ {
		/*.val =*/ {0x3ffffc2f, 0x3ffffffb, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0xffff}
	},

	/* G */ {
		/*.x =*/{/*.val =*/{0x16f81798, 0x27ca056c, 0x1ce28d95, 0x26ff36cb, 0x70b0702, 0x18a573a, 0xbbac55a, 0x199fbe77, 0x79be}},
		/*.y =*/{/*.val =*/{0x3b10d4b8, 0x311f423f, 0x28554199, 0x5ed1229, 0x1108a8fd, 0x13eff038, 0x3c4655da, 0x369dc9a8, 0x483a}}
	},

	/* order */ {
		/*.val =*/{0x10364141, 0x3f497a33, 0x348a03bb, 0x2bb739ab, 0x3ffffeba, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0xffff}
	},

	/* order_half */ {
		/*.val =*/{0x281b20a0, 0x3fa4bd19, 0x3a4501dd, 0x15db9cd5, 0x3fffff5d, 0x3fffffff, 0x3fffffff, 0x3fffffff, 0x7fff}
	},

	/* a */	0,

	/* b */ {
		/*.val =*/{7}
	}

#if USE_PRECOMPUTED_CP
	,
	/* cp */ {
#include "secp256k1.table"
	}
#endif
};

const curve_info secp256k1_info = {
	.bip32_name = "Bitcoin seed",
	.params = &secp256k1,
	.hasher_type = HASHER_SHA2,
};

const curve_info secp256k1_decred_info = {
	.bip32_name = "Decred seed",
	.params = &secp256k1,
	.hasher_type = HASHER_BLAKE,
};
