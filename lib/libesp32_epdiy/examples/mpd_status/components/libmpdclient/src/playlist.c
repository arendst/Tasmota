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

#include <mpd/playlist.h>
#include <mpd/pair.h>
#include "iso8601.h"
#include "uri.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct mpd_playlist {
	char *path;

	/**
	 * The POSIX UTC time stamp of the last modification, or 0 if
	 * that is unknown.
	 */
	time_t last_modified;
};

static struct mpd_playlist *
mpd_playlist_new(const char *path)
{
	struct mpd_playlist *playlist;

	assert(path != NULL);
	assert(mpd_verify_local_uri(path));

	playlist = malloc(sizeof(*playlist));
	if (playlist == NULL)
		/* out of memory */
		return NULL;

	playlist->path = strdup(path);
	if (playlist->path == NULL) {
		/* out of memory */
		free(playlist);
		return NULL;
	}

	playlist->last_modified = 0;

	return playlist;
}

void
mpd_playlist_free(struct mpd_playlist *playlist)
{
	assert(playlist != NULL);
	assert(playlist->path != NULL);

	free(playlist->path);
	free(playlist);
}

struct mpd_playlist *
mpd_playlist_dup(const struct mpd_playlist *playlist)
{
	assert(playlist != NULL);
	assert(playlist->path != NULL);

	struct mpd_playlist *copy = mpd_playlist_new(playlist->path);
	copy->last_modified = playlist->last_modified;
	return copy;
}

const char *
mpd_playlist_get_path(const struct mpd_playlist *playlist)
{
	assert(playlist != NULL);

	return playlist->path;
}

time_t
mpd_playlist_get_last_modified(const struct mpd_playlist *playlist)
{
	return playlist->last_modified;
}

struct mpd_playlist *
mpd_playlist_begin(const struct mpd_pair *pair)
{
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "playlist") != 0 ||
	    !mpd_verify_local_uri(pair->value)) {
		errno = EINVAL;
		return NULL;
	}

	return mpd_playlist_new(pair->value);
}

bool
mpd_playlist_feed(struct mpd_playlist *playlist, const struct mpd_pair *pair)
{
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "playlist") == 0)
		return false;

	if (strcmp(pair->name, "Last-Modified") == 0)
		playlist->last_modified =
			iso8601_datetime_parse(pair->value);

	return true;
}
