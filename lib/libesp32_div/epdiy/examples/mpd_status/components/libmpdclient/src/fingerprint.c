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

#include <mpd/fingerprint.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/pair.h>
#include <mpd/response.h>
#include "run.h"

#include <string.h>
#include <stdio.h>

bool
mpd_send_getfingerprint(struct mpd_connection *connection, const char *uri)
{
	return mpd_send_command(connection, "getfingerprint", uri, NULL);
}

enum mpd_fingerprint_type
mpd_parse_fingerprint_type(const char *name)
{
	if (strcmp(name, "chromaprint") == 0)
		return MPD_FINGERPRINT_TYPE_CHROMAPRINT;
	else
		return MPD_FINGERPRINT_TYPE_UNKNOWN;
}

const char *
mpd_run_getfingerprint_chromaprint(struct mpd_connection *connection,
				   const char *uri,
				   char *buffer, size_t buffer_size)
{
	if (!mpd_run_check(connection) ||
	    !mpd_send_getfingerprint(connection, uri))
		return NULL;

	const char *result = NULL;

	struct mpd_pair *pair = mpd_recv_pair_named(connection, "chromaprint");
	if (pair != NULL) {
		snprintf(buffer, buffer_size, "%s", pair->value);
		result = buffer;
		mpd_return_pair(connection, pair);
	}

	if (!mpd_response_finish(connection))
		result = NULL;

	return result;
}
