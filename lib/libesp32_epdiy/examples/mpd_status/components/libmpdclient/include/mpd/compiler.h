/* libmpdclient
   (c) 2003-2019 The Music Player Daemon Project
   This project's homepage is: http://www.musicpd.org

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! \file
 * \brief Compiler specific definitions
 *
 * This file is not part of the official libmpdclient2 API.  It
 * provides access to gcc specific extensions.
 *
 */

#ifndef MPD_COMPILER_H
#define MPD_COMPILER_H

#if !defined(SPARSE) && defined(__GNUC__) && __GNUC__ >= 3

/* GCC 4.x */

#define mpd_unused __attribute__((unused))
#define mpd_malloc __attribute__((malloc))
#define mpd_pure __attribute__((pure))
#define mpd_const __attribute__((const))
#define mpd_sentinel __attribute__((sentinel))
#define mpd_printf(a,b) __attribute__((format(printf, a, b)))

#else

/* generic C compiler */

#define mpd_unused
#define mpd_malloc
#define mpd_pure
#define mpd_const
#define mpd_sentinel
#define mpd_printf(a,b)

#endif

#endif
