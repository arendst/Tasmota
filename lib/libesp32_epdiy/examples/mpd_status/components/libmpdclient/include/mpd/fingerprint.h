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

   - Neither the name of the Music Player Daemon nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

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
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_FINGERPRINT_H
#define MPD_FINGERPRINT_H

#include "compiler.h"

#include <stdbool.h>
#include <stddef.h>

struct mpd_connection;
struct mpd_pair;

enum mpd_fingerprint_type {
	MPD_FINGERPRINT_TYPE_UNKNOWN,
	MPD_FINGERPRINT_TYPE_CHROMAPRINT,
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parse a #mpd_pair name to check which fingerprint type it contains.
 */
mpd_pure
enum mpd_fingerprint_type
mpd_parse_fingerprint_type(const char *name);

/**
 * Sends the "getfingerprint" command to MPD.  Call mpd_recv_pair() to
 * read response lines.  Use mpd_parse_fingerprint_type() to check
 * each pair's name; the pair's value then contains the actual
 * fingerprint.
 *
 * @param connection a valid and connected #mpd_connection
 * @param uri the song URI
 * @return true on success
 *
 * @since libmpdclient 2.17, MPD 0.22
 */
bool
mpd_send_getfingerprint(struct mpd_connection *connection, const char *uri);

/**
 * Shortcut for mpd_send_getfingerprint(), mpd_recv_pair_named() and
 * mpd_response_finish().
 *
 * @param connection a valid and connected #mpd_connection
 * @param uri the song URI
 * @param buffer a buffer for the fingerprint string
 * @param buffer_size the size of the buffer (with enough room for a
 * trailing null byte); if the buffer is too small, behavior is
 * undefined; the library may truncate the string or fail
 * @return a pointer to the buffer on success or NULL on error (or if
 * there was no chromaprint in MPD's response)
 *
 * @since libmpdclient 2.17, MPD 0.22
 */
mpd_malloc
const char *
mpd_run_getfingerprint_chromaprint(struct mpd_connection *connection,
				   const char *uri,
				   char *buffer, size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif
