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

#include <mpd/playlist.h>
#include <mpd/recv.h>
#include "internal.h"

#include <errno.h>

struct mpd_playlist *
mpd_recv_playlist(struct mpd_connection *connection)
{
	struct mpd_pair *pair;
	struct mpd_playlist *playlist;

	pair = mpd_recv_pair_named(connection, "playlist");
	if (pair == NULL)
		return NULL;

	playlist = mpd_playlist_begin(pair);
	mpd_return_pair(connection, pair);
	if (playlist == NULL) {
		mpd_error_entity(&connection->error);
		return NULL;
	}

	while ((pair = mpd_recv_pair(connection)) != NULL &&
	       mpd_playlist_feed(playlist, pair))
		mpd_return_pair(connection, pair);

	if (mpd_error_is_defined(&connection->error)) {
		assert(pair == NULL);

		mpd_playlist_free(playlist);
		return NULL;
	}

	/* unread this pair for the next mpd_recv_playlist() call */
	mpd_enqueue_pair(connection, pair);

	return playlist;
}
