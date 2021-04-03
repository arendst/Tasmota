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

/*
 * mpd_connection specific functions declared in mpd/stats.h.
 *
 */

#include <mpd/stats.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include "internal.h"

#include <assert.h>

bool
mpd_send_stats(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "stats", NULL);
}

struct mpd_stats *
mpd_recv_stats(struct mpd_connection *connection)
{
	struct mpd_stats * stats;
	struct mpd_pair *pair;

	assert(connection != NULL);

	if (mpd_error_is_defined(&connection->error))
		/* refuse to receive a response if the connection's
		   state is not clean */
		return NULL;

	stats = mpd_stats_begin();
	if (stats == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return NULL;
	}

	/* read and parse all response lines */
	while ((pair = mpd_recv_pair(connection)) != NULL) {
		mpd_stats_feed(stats, pair);
		mpd_return_pair(connection, pair);
	}

	if (mpd_error_is_defined(&connection->error)) {
		/* an error has occurred; roll back */
		mpd_stats_free(stats);
		return NULL;
	}

	return stats;
}


struct mpd_stats *
mpd_run_stats(struct mpd_connection *connection)
{
	return mpd_send_stats(connection)
		? mpd_recv_stats(connection)
		: NULL;
}
