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
#include <mpd/pair.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct mpd_neighbor {
	char *uri;
	char *display_name;
};

struct mpd_neighbor *
mpd_neighbor_begin(const struct mpd_pair *pair)
{
	assert(pair != NULL);

	if (strcmp(pair->name, "neighbor") != 0)
		return NULL;

	struct mpd_neighbor *neighbor = malloc(sizeof(*neighbor));
	if (neighbor == NULL)
		return NULL;

	neighbor->uri = strdup(pair->value);
	if (neighbor->uri == NULL) {
		free(neighbor);
		return NULL;
	}

	neighbor->display_name = NULL;
	return neighbor;
}

bool
mpd_neighbor_feed(struct mpd_neighbor *neighbor, const struct mpd_pair *pair)
{
	if (strcmp(pair->name, "neighbor") == 0)
		return false;

	if (strcmp(pair->name, "name") == 0) {
		free(neighbor->display_name);
		neighbor->display_name = strdup(pair->value);
	}

	return true;
}

void
mpd_neighbor_free(struct mpd_neighbor *neighbor)
{
	assert(neighbor != NULL);

	free(neighbor->uri);
	free(neighbor->display_name);
	free(neighbor);
}

const char *
mpd_neighbor_get_uri(const struct mpd_neighbor *neighbor)
{
	assert(neighbor != NULL);

	return neighbor->uri;
}

const char *
mpd_neighbor_get_display_name(const struct mpd_neighbor *neighbor)
{
	assert(neighbor != NULL);

	return neighbor->display_name;
}
