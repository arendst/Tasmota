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

#include "../ESP_SSLClient_FS.h"
#if defined(USE_LIB_SSL_ENGINE)

#define BR_ENABLE_INTRINSICS   1
#include "inner.h"

#if BR_USE_GETENTROPY
#include <unistd.h>
#endif

#if BR_USE_URANDOM
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#endif

#if BR_USE_WIN32_RAND
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "advapi32")
#endif

/*
 * Seeder that uses the RDRAND opcodes (on x86 CPU).
 */
#if BR_RDRAND
BR_TARGETS_X86_UP
BR_TARGET("rdrnd")
static int
seeder_rdrand(const br_prng_class **ctx)
{
	unsigned char tmp[32];
	size_t u;

	for (u = 0; u < sizeof tmp; u += sizeof(uint32_t)) {
		int j;
		uint32_t x;

		/*
		 * We use the 32-bit intrinsic so that code is compatible
		 * with both 32-bit and 64-bit architectures.
		 *
		 * Intel recommends trying at least 10 times in case of
		 * failure.
		 *
		 * AMD bug: there are reports that some AMD processors
		 * have a bug that makes them fail silently after a
		 * suspend/resume cycle, in which case RDRAND will report
		 * a success but always return 0xFFFFFFFF.
		 * see: https://bugzilla.kernel.org/show_bug.cgi?id=85911
		 *
		 * As a mitigation, if the 32-bit value is 0 or -1, then
		 * it is considered a failure and tried again. This should
		 * reliably detect the buggy case, at least. This also
		 * implies that the selected seed values can never be
		 * 0x00000000 or 0xFFFFFFFF, which is not a problem since
		 * we are generating a seed for a PRNG, and we overdo it
		 * a bit (we generate 32 bytes of randomness, and 256 bits
		 * of entropy are really overkill).
		 */
		for (j = 0; j < 10; j ++) {
			if (_rdrand32_step(&x) && x != 0 && x != (uint32_t)-1) {
				goto next_word;
			}
		}
		return 0;
	next_word:
		br_enc32le(tmp + u, x);
	}
	(*ctx)->update(ctx, tmp, sizeof tmp);
	return 1;
}
BR_TARGETS_X86_DOWN

static int
rdrand_supported(void)
{
	/*
	 * The RDRND support is bit 30 of ECX, as returned by CPUID.
	 */
	return br_cpuid(0, 0, 0x40000000, 0);
}
#endif

/*
 * Seeder that uses /dev/urandom (on Unix-like systems).
 */
#if BR_USE_URANDOM
static int
seeder_urandom(const br_prng_class **ctx)
{
	int f;

	f = open("/dev/urandom", O_RDONLY);
	if (f >= 0) {
		unsigned char tmp[32];
		size_t u;

		for (u = 0; u < sizeof tmp;) {
			ssize_t len;

			len = read(f, tmp + u, (sizeof tmp) - u);
			if (len < 0) {
				if (errno == EINTR) {
					continue;
				}
				break;
			}
			u += (size_t)len;
		}
		close(f);
		if (u == sizeof tmp) {
			(*ctx)->update(ctx, tmp, sizeof tmp);
			return 1;
		}
	}
	return 0;
}
#endif

/*
 * Seeder that uses getentropy() (backed by getrandom() on some systems,
 * e.g. Linux). On failure, it will use the /dev/urandom seeder (if
 * enabled).
 */
#if BR_USE_GETENTROPY
static int
seeder_getentropy(const br_prng_class **ctx)
{
	unsigned char tmp[32];

	if (getentropy(tmp, sizeof tmp) == 0) {
		(*ctx)->update(ctx, tmp, sizeof tmp);
		return 1;
	}
#if BR_USE_URANDOM
	return seeder_urandom(ctx);
#else
	return 0;
#endif
}
#endif

/*
 * Seeder that uses CryptGenRandom() (on Windows).
 */
#if BR_USE_WIN32_RAND
static int
seeder_win32(const br_prng_class **ctx)
{
	HCRYPTPROV hp;

	if (CryptAcquireContext(&hp, 0, 0, PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
	{
		BYTE buf[32];
		BOOL r;

		r = CryptGenRandom(hp, sizeof buf, buf);
		CryptReleaseContext(hp, 0);
		if (r) {
			(*ctx)->update(ctx, buf, sizeof buf);
			return 1;
		}
	}
	return 0;
}
#endif

/*
 * An aggregate seeder that uses RDRAND, and falls back to an OS-provided
 * source if RDRAND fails.
 */
#if BR_RDRAND && (BR_USE_GETENTROPY || BR_USE_URANDOM || BR_USE_WIN32_RAND)
static int
seeder_rdrand_with_fallback(const br_prng_class **ctx)
{
	if (!seeder_rdrand(ctx)) {
#if BR_USE_GETENTROPY
		return seeder_getentropy(ctx);
#elif BR_USE_URANDOM
		return seeder_urandom(ctx);
#elif BR_USE_WIN32_RAND
		return seeder_win32(ctx);
#else
#error "macro selection has gone wrong"
#endif
	}
	return 1;
}
#endif

/* see bearssl_rand.h */
br_prng_seeder
br_prng_seeder_system(const char **name)
{
#if BR_RDRAND
	if (rdrand_supported()) {
		if (name != NULL) {
			*name = "rdrand";
		}
#if BR_USE_GETENTROPY || BR_USE_URANDOM || BR_USE_WIN32_RAND
		return &seeder_rdrand_with_fallback;
#else
		return &seeder_rdrand;
#endif
	}
#endif
#if BR_USE_GETENTROPY
	if (name != NULL) {
		*name = "getentropy";
	}
	return &seeder_getentropy;
#elif BR_USE_URANDOM
	if (name != NULL) {
		*name = "urandom";
	}
	return &seeder_urandom;
#elif BR_USE_WIN32_RAND
	if (name != NULL) {
		*name = "win32";
	}
	return &seeder_win32;
#else
	if (name != NULL) {
		*name = "none";
	}
	return 0;
#endif
}

#endif