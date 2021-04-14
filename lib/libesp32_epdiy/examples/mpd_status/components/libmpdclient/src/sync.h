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
 * \brief Synchronous MPD connections
 *
 * This library provides synchronous access to a mpd_async object.
 * For all operations, you may provide a timeout.
 */

#ifndef MPD_SYNC_H
#define MPD_SYNC_H

#include <mpd/compiler.h>

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

struct timeval;
struct mpd_async;

/**
 * Synchronous wrapper for mpd_async_send_command_v().
 */
bool
mpd_sync_send_command_v(struct mpd_async *async, const struct timeval *tv,
			const char *command, va_list args);

/**
 * Synchronous wrapper for mpd_async_send_command().
 */
mpd_sentinel
bool
mpd_sync_send_command(struct mpd_async *async, const struct timeval *tv,
		      const char *command, ...);

/**
 * Sends all pending data from the output buffer to MPD.
 */
bool
mpd_sync_flush(struct mpd_async *async, const struct timeval *tv);

/**
 * Synchronous wrapper for mpd_async_recv_line().
 */
char *
mpd_sync_recv_line(struct mpd_async *async, const struct timeval *tv);

/**
 * Synchronous wrapper for mpd_async_recv_raw() which waits until at
 * least one byte was received (or an error has occurred).
 *
 * @return the number of bytes copied to the destination buffer or 0
 * on error
 */
size_t
mpd_sync_recv_raw(struct mpd_async *async, const struct timeval *tv,
		  void *dest, size_t length);

#endif
