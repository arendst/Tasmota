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

#include <mpd/song.h>
#include <mpd/audio_format.h>
#include <mpd/pair.h>
#include <mpd/recv.h>
#include "internal.h"
#include "iso8601.h"
#include "uri.h"
#include "iaf.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct mpd_tag_value {
	struct mpd_tag_value *next;

	char *value;
};

struct mpd_song {
	char *uri;

	struct mpd_tag_value tags[MPD_TAG_COUNT];

	/**
	 * Duration of the song in seconds, or 0 for unknown.
	 */
	unsigned duration;

	/**
	 * Duration of the song in milliseconds, or 0 for unknown.
	 */
	unsigned duration_ms;

	/**
	 * Start of the virtual song within the physical file in
	 * seconds.
	 */
	unsigned start;

	/**
	 * End of the virtual song within the physical file in
	 * seconds.  Zero means that the physical song file is
	 * played to the end.
	 */
	unsigned end;

	/**
	 * The POSIX UTC time stamp of the last modification, or 0 if
	 * that is unknown.
	 */
	time_t last_modified;

	/**
	 * The position of this song within the queue.
	 */
	unsigned pos;

	/**
	 * The id of this song within the queue.
	 */
	unsigned id;

	/**
	 * The priority of this song within the queue.
	 */
	unsigned prio;

#ifndef NDEBUG
	/**
	 * This flag is used in an assertion: when it is set, you must
	 * not call mpd_song_feed() again.  It is a safeguard for
	 * buggy callers.
	 */
	bool finished;
#endif

	/**
	 * The audio format as reported by MPD's decoder plugin.
	 */
	struct mpd_audio_format audio_format;
};

static struct mpd_song *
mpd_song_new(const char *uri)
{
	struct mpd_song *song;

	assert(uri != NULL);
	assert(mpd_verify_uri(uri));

	song = malloc(sizeof(*song));
	if (song == NULL)
		/* out of memory */
		return NULL;

	song->uri = strdup(uri);
	if (song->uri == NULL) {
		free(song);
		return NULL;
	}

	for (unsigned i = 0; i < MPD_TAG_COUNT; ++i)
		song->tags[i].value = NULL;

	song->duration = 0;
	song->duration_ms = 0;
	song->start = 0;
	song->end = 0;
	song->last_modified = 0;
	song->pos = 0;
	song->id = 0;
	song->prio = 0;

	memset(&song->audio_format, 0, sizeof(song->audio_format));

#ifndef NDEBUG
	song->finished = false;
#endif

	return song;
}

void mpd_song_free(struct mpd_song *song) {
	assert(song != NULL);

	free(song->uri);

	for (unsigned i = 0; i < MPD_TAG_COUNT; ++i) {
		struct mpd_tag_value *tag = &song->tags[i], *next;

		if (tag->value == NULL)
			continue;

		free(tag->value);

		tag = tag->next;

		while (tag != NULL) {
			assert(tag->value != NULL);
			free(tag->value);

			next = tag->next;
			free(tag);
			tag = next;
		}
	}

	free(song);
}

static bool
mpd_song_add_tag(struct mpd_song *song,
		 enum mpd_tag_type type, const char *value);

struct mpd_song *
mpd_song_dup(const struct mpd_song *song)
{
	struct mpd_song *ret;
	bool success;

	assert(song != NULL);

	ret = mpd_song_new(song->uri);
	if (ret == NULL)
		/* out of memory */
		return NULL;

	for (unsigned i = 0; i < MPD_TAG_COUNT; ++i) {
		const struct mpd_tag_value *src_tag = &song->tags[i];

		if (src_tag->value == NULL)
			continue;

		do {
			success = mpd_song_add_tag(ret, i, src_tag->value);
			if (!success) {
				mpd_song_free(ret);
				return NULL;
			}

			src_tag = src_tag->next;
		} while (src_tag != NULL);
	}

	ret->duration = song->duration;
	ret->duration_ms = song->duration_ms;
	ret->start = song->start;
	ret->end = song->end;
	ret->last_modified = song->last_modified;
	ret->pos = song->pos;
	ret->id = song->id;
	ret->prio = song->prio;

#ifndef NDEBUG
	ret->finished = true;
#endif

	return ret;
}

const char *
mpd_song_get_uri(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->uri;
}


/**
 * Adds a tag value to the song.
 *
 * @return true on success, false if the tag is not supported or if no
 * memory could be allocated
 */
static bool
mpd_song_add_tag(struct mpd_song *song,
		 enum mpd_tag_type type, const char *value)
{
	struct mpd_tag_value *tag = &song->tags[type], *prev;

	if ((int)type < 0 || type >= MPD_TAG_COUNT)
		return false;

	if (tag->value == NULL) {
		tag->next = NULL;
		tag->value = strdup(value);
		if (tag->value == NULL)
			return false;
	} else {
		while (tag->next != NULL)
			tag = tag->next;

		prev = tag;
		tag = malloc(sizeof(*tag));
		if (tag == NULL)
			return NULL;

		tag->value = strdup(value);
		if (tag->value == NULL) {
			free(tag);
			return false;
		}

		tag->next = NULL;
		prev->next = tag;
	}

	return true;
}

#ifdef UNUSED_CODE
/**
 * Removes all values of the specified tag.
 */
static void
mpd_song_clear_tag(struct mpd_song *song, enum mpd_tag_type type)
{
	struct mpd_tag_value *tag = &song->tags[type];

	if ((unsigned)type >= MPD_TAG_COUNT)
		return;

	if (tag->value == NULL)
		/* this tag type is empty */
		return;

	/* free and clear the first value */
	free(tag->value);
	tag->value = NULL;

	/* free all other values; no need to clear the "next" pointer,
	   because it is "undefined" as long as value==NULL */
	while ((tag = tag->next) != NULL)
		free(tag->value);
}
#endif

const char *
mpd_song_get_tag(const struct mpd_song *song,
		 enum mpd_tag_type type, unsigned idx)
{
	const struct mpd_tag_value *tag = &song->tags[type];

	if ((int)type < 0)
		return NULL;

	if (tag->value == NULL)
		return NULL;

	while (idx-- > 0) {
		tag = tag->next;
		if (tag == NULL)
			return NULL;
	}

	return tag->value;
}

static void
mpd_song_set_duration(struct mpd_song *song, unsigned duration)
{
	song->duration = duration;
}

unsigned
mpd_song_get_duration(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->duration > 0
		? song->duration
		: (song->duration_ms + 500u) / 1000u;
}

static void
mpd_song_set_duration_ms(struct mpd_song *song, unsigned duration_ms)
{
	song->duration_ms = duration_ms;
}

unsigned
mpd_song_get_duration_ms(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->duration_ms > 0
		? song->duration_ms
		: (song->duration * 1000u);
}

unsigned
mpd_song_get_start(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->start;
}

unsigned
mpd_song_get_end(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->end;
}

static void
mpd_song_set_last_modified(struct mpd_song *song, time_t mtime)
{
	song->last_modified = mtime;
}

time_t
mpd_song_get_last_modified(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->last_modified;
}

void
mpd_song_set_pos(struct mpd_song *song, unsigned pos)
{
	assert(song != NULL);

	song->pos = pos;
}

unsigned
mpd_song_get_pos(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->pos;
}

static void
mpd_song_set_id(struct mpd_song *song, unsigned id)
{
	song->id = id;
}

unsigned
mpd_song_get_id(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->id;
}

static void
mpd_song_set_prio(struct mpd_song *song, unsigned prio)
{
	song->prio = prio;
}

unsigned
mpd_song_get_prio(const struct mpd_song *song)
{
	assert(song != NULL);

	return song->prio;
}

const struct mpd_audio_format *
mpd_song_get_audio_format(const struct mpd_song *song)
{
	assert(song != NULL);

	return !mpd_audio_format_is_empty(&song->audio_format)
		? &song->audio_format
		: NULL;
}

struct mpd_song *
mpd_song_begin(const struct mpd_pair *pair)
{
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "file") != 0 || !mpd_verify_uri(pair->value)) {
		errno = EINVAL;
		return NULL;
	}

	return mpd_song_new(pair->value);
}

static void
mpd_song_parse_range(struct mpd_song *song, const char *value)
{
	assert(song != NULL);
	assert(value != NULL);

	char *endptr;
	double start, end;

	if (*value == '-') {
		start = 0.0;
		end = strtod(value + 1, NULL);
	} else {
		start = strtod(value, &endptr);
		if (*endptr != '-')
			return;

		end = strtod(endptr + 1, NULL);
	}

	song->start = start > 0.0 ? (unsigned)start : 0;

	if (end > 0.0) {
		song->end = (unsigned)end;
		if (song->end == 0)
			/* round up, because the caller must sees that
			   there's an upper limit */
			song->end = 1;
	} else
		song->end = 0;
}

static void
mpd_song_parse_audio_format(struct mpd_song *song, const char *value)
{
	assert(song != NULL);
	assert(value != NULL);

	mpd_parse_audio_format(&song->audio_format, value);
}

bool
mpd_song_feed(struct mpd_song *song, const struct mpd_pair *pair)
{
	enum mpd_tag_type tag_type;

	assert(song != NULL);
	assert(!song->finished);
	assert(pair != NULL);
	assert(pair->name != NULL);
	assert(pair->value != NULL);

	if (strcmp(pair->name, "file") == 0) {
#ifndef NDEBUG
		song->finished = true;
#endif
		return false;
	}

	if (*pair->value == 0)
		return true;

	tag_type = mpd_tag_name_parse(pair->name);
	if (tag_type != MPD_TAG_UNKNOWN) {
		mpd_song_add_tag(song, tag_type, pair->value);
		return true;
	}

	if (strcmp(pair->name, "Time") == 0)
		mpd_song_set_duration(song, atoi(pair->value));
	else if (strcmp(pair->name, "duration") == 0)
		mpd_song_set_duration_ms(song, 1000 * atof(pair->value));
	else if (strcmp(pair->name, "Range") == 0)
		mpd_song_parse_range(song, pair->value);
	else if (strcmp(pair->name, "Last-Modified") == 0)
		mpd_song_set_last_modified(song, iso8601_datetime_parse(pair->value));
	else if (strcmp(pair->name, "Pos") == 0)
		mpd_song_set_pos(song, atoi(pair->value));
	else if (strcmp(pair->name, "Id") == 0)
		mpd_song_set_id(song, atoi(pair->value));
	else if (strcmp(pair->name, "Prio") == 0)
		mpd_song_set_prio(song, atoi(pair->value));
	else if (strcmp(pair->name, "Format") == 0)
		mpd_song_parse_audio_format(song, pair->value);

	return true;
}

struct mpd_song *
mpd_recv_song(struct mpd_connection *connection)
{
	struct mpd_pair *pair;
	struct mpd_song *song;

	pair = mpd_recv_pair_named(connection, "file");
	if (pair == NULL)
		return NULL;

	song = mpd_song_begin(pair);
	mpd_return_pair(connection, pair);
	if (song == NULL) {
		mpd_error_entity(&connection->error);
		return NULL;
	}

	while ((pair = mpd_recv_pair(connection)) != NULL &&
	       mpd_song_feed(song, pair))
		mpd_return_pair(connection, pair);

	if (mpd_error_is_defined(&connection->error)) {
		mpd_song_free(song);
		return NULL;
	}

	/* unread this pair for the next mpd_recv_song() call */
	mpd_enqueue_pair(connection, pair);

	return song;
}
