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
 * \brief MPD client library
 *
 * Receiving response lines from MPD.
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_RECV_H
#define MPD_RECV_H

#include "compiler.h"

#include <stdbool.h>
#include <stddef.h>

struct mpd_pair;
struct mpd_connection;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reads the binary data response from the server.
 * The size and binary pair must be already read from the input buffer.
 *
 * The caller must allocate length bytes of memory for data.
 *
 * @return true on success
 *
 * @since libmpdclient 2.17
 */
bool
mpd_recv_binary(struct mpd_connection *connection, void *data, size_t length);

/**
 * Reads the next #mpd_pair from the server.  Returns NULL if there
 * are no more pairs.
 *
 * The caller must dispose the pair with either mpd_return_pair() or
 * mpd_enqueue_pair().
 */
mpd_malloc
struct mpd_pair *
mpd_recv_pair(struct mpd_connection *connection);

/**
 * Same as mpd_recv_pair(), but discards all pairs not matching the
 * specified name.
 */
mpd_malloc
struct mpd_pair *
mpd_recv_pair_named(struct mpd_connection *connection, const char *name);

/**
 * Indicates that the pair object is not needed anymore, and can be
 * freed.  You must free the previous #mpd_pair object before calling
 * mpd_recv_pair() again.
 */
void
mpd_return_pair(struct mpd_connection *connection, struct mpd_pair *pair);

/**
 * Unreads a #mpd_pair.  You may unread only the one pair you just got
 * from mpd_recv_pair().  Unreading the "NULL" pair is allowed, to
 * allow you to call mpd_recv_pair() again at the end of a response.
 */
void
mpd_enqueue_pair(struct mpd_connection *connection, struct mpd_pair *pair);

#ifdef __cplusplus
}
#endif

#endif
