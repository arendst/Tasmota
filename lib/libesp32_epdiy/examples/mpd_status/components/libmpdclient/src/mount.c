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

#include <mpd/mount.h>
#include <mpd/pair.h>

#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct mpd_mount {
	char *uri;
	char *storage;
};

struct mpd_mount *
mpd_mount_begin(const struct mpd_pair *pair)
{
	assert(pair != NULL);

	if (strcmp(pair->name, "mount") != 0)
		return NULL;

	struct mpd_mount *mount = malloc(sizeof(*mount));
	if (mount == NULL)
		return NULL;

	mount->uri = strdup(pair->value);
	if (mount->uri == NULL) {
		free(mount);
		return NULL;
	}

	mount->storage = NULL;
	return mount;
}

bool
mpd_mount_feed(struct mpd_mount *mount, const struct mpd_pair *pair)
{
	if (strcmp(pair->name, "mount") == 0)
		return false;

	if (strcmp(pair->name, "storage") == 0) {
		free(mount->storage);
		mount->storage = strdup(pair->value);
	}

	return true;
}

void
mpd_mount_free(struct mpd_mount *mount)
{
	assert(mount != NULL);

	free(mount->uri);
	free(mount->storage);
	free(mount);
}

const char *
mpd_mount_get_uri(const struct mpd_mount *mount)
{
	assert(mount != NULL);

	return mount->uri;
}

const char *
mpd_mount_get_storage(const struct mpd_mount *mount)
{
	assert(mount != NULL);

	return mount->storage;
}
