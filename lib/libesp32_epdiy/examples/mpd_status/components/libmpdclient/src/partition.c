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

#include <mpd/pair.h>
#include <mpd/partition.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct mpd_partition {
	char *name;
};

struct mpd_partition *
mpd_partition_new(const struct mpd_pair *pair)
{
	assert(pair != NULL);

	if (strcmp(pair->name, "partition") != 0)
		return NULL;

	struct mpd_partition *partition = malloc(sizeof(*partition));
	if (partition == NULL)
		return NULL;

	partition->name = strdup(pair->value);
	if (partition->name == NULL) {
		free(partition);
		return NULL;
	}

	return partition;
}

void
mpd_partition_free(struct mpd_partition *partition)
{
	assert(partition != NULL);

	free(partition->name);
	free(partition);
}

mpd_pure
const char *
mpd_partition_get_name(const struct mpd_partition *partition)
{
	assert(partition != NULL);

	return partition->name;
}
