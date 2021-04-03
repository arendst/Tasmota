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

#include <mpd/directory.h>
#include <mpd/pair.h>
#include "uri.h"
#include "iso8601.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct mpd_directory {
	/**
	 * The full path of this directory.  It does not begin with a
	 * slash.
	 */
	char *path;

	/**
	 * The POSIX UTC time stamp of the last modification, or 0 if
	 * that is unknown.
	 */
	time_t last_modified;
};

mpd_malloc
static struct mpd_directory *
mpd_directory_new(const char *path)
{
	struct mpd_directory *directory;

	assert(path != NULL);
	assert(mpd_verify_local_uri(path));

	directory = malloc(sizeof(*directory));
	if (directory == NULL)
		/* out of memory */
		return NULL;

	directory->path = strdup(path);
	if (directory->path == NULL) {
		/* out of memory */
		free(directory);
		return NULL;
	}

	directory->last_modified = 0;

	return directory;
}

void mpd_directory_free(struct mpd_directory *directory)
{
	assert(directory != NULL);
	assert(directory->path != NULL);

	free(directory->path);
	free(directory);
}

struct mpd_directory *
mpd_directory_dup(const struct mpd_directory *directory)
{
	assert(directory != NULL);
	assert(directory->path != NULL);

	struct mpd_directory *copy = mpd_directory_new(directory->path);
	copy->last_modified = directory->last_modified;
	return copy;
}

const char *
mpd_directory_get_path(const struct mpd_directory *directory)
{
	assert(directory != NULL);
	assert(directory->path != NULL);

	return directory->path;
}

time_t
mpd_directory_get_last_modified(const struct mpd_directory *directory)
{
	assert(directory != NULL);

	return directory->last_modified;
}

struct mpd_directory *
mpd_directory_begin(const struct mpd_pair *pair)
{
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "directory") != 0 ||
	    !mpd_verify_local_uri(pair->value)) {
		errno = EINVAL;
		return NULL;
	}

	return mpd_directory_new(pair->value);
}

bool
mpd_directory_feed(struct mpd_directory *directory,
		   const struct mpd_pair *pair)
{
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "directory") == 0)
		return false;

	if (strcmp(pair->name, "Last-Modified") == 0)
		directory->last_modified =
			iso8601_datetime_parse(pair->value);

	return true;
}
