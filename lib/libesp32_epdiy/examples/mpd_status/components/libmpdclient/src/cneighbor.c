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

#include <mpd/neighbor.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/response.h>
#include "internal.h"

#include <stddef.h>

bool
mpd_send_list_neighbors(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "listneighbors", NULL);
}

struct mpd_neighbor *
mpd_recv_neighbor(struct mpd_connection *connection)
{
	struct mpd_neighbor *neighbor;
	struct mpd_pair *pair;

	pair = mpd_recv_pair_named(connection, "neighbor");
	if (pair == NULL)
		return NULL;

	neighbor = mpd_neighbor_begin(pair);
	mpd_return_pair(connection, pair);
	if (neighbor == NULL) {
		mpd_error_code(&connection->error, MPD_ERROR_OOM);
		return NULL;
	}

	while ((pair = mpd_recv_pair(connection)) != NULL &&
	       mpd_neighbor_feed(neighbor, pair))
		mpd_return_pair(connection, pair);

	if (mpd_error_is_defined(&connection->error)) {
		assert(pair == NULL);

		mpd_neighbor_free(neighbor);
		return NULL;
	}

	mpd_enqueue_pair(connection, pair);
	return neighbor;
}
