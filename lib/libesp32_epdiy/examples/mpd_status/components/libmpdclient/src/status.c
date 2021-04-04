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

#include <mpd/status.h>
#include <mpd/pair.h>
#include <mpd/audio_format.h>
#include "iaf.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 * Information about MPD's current status.
 */
struct mpd_status {
	/** 0-100, or MPD_STATUS_NO_VOLUME when there is no volume support */
	int volume;

	/** Queue repeat mode enabled? */
	bool repeat;

	/** Random mode enabled? */
	bool random;

	/** Single song mode enabled? */
	enum mpd_single_state single;

	/** Song consume mode enabled? */
	bool consume;

	/** Number of songs in the queue */
	unsigned queue_length;

	/**
	 * Queue version, use this to determine when the playlist has
	 * changed.
	 */
	unsigned queue_version;

	/** MPD's current playback state */
	enum mpd_state state;

	/** crossfade setting in seconds */
	unsigned crossfade;

	/** Mixramp threshold in dB */
	float mixrampdb;

	/** Mixramp extra delay in seconds */
	float mixrampdelay;

	/**
	 * If a song is currently selected (always the case when state
	 * is PLAY or PAUSE), this is the position of the currently
	 * playing song in the queue, beginning with 0.
	 */
	int song_pos;

	/** Song ID of the currently selected song */
	int song_id;

	/** The same as song_pos, but for the next song to be played */
	int next_song_pos;

	/** Song ID of the next song to be played */
	int next_song_id;

	/**
	 * Time in seconds that have elapsed in the currently
	 * playing/paused song.
	 */
	unsigned elapsed_time;

	/**
	 * Time in milliseconds that have elapsed in the currently
	 * playing/paused song.
	 */
	unsigned elapsed_ms;

	/** length in seconds of the currently playing/paused song */
	unsigned total_time;

	/** current bit rate in kbps */
	unsigned kbit_rate;

	/** the current audio format */
	struct mpd_audio_format audio_format;

	/** non-zero if MPD is updating, 0 otherwise */
	unsigned update_id;

	/** the name of the current partition */
	char *partition;

	/** error message */
	char *error;
};

struct mpd_status *
mpd_status_begin(void)
{
	struct mpd_status *status = malloc(sizeof(*status));
	if (status == NULL)
		return NULL;

	status->volume = -1;
	status->repeat = false;
	status->random = false;
	status->single = MPD_SINGLE_OFF;
	status->consume = false;
	status->queue_version = 0;
	status->queue_length = 0;
	status->state = MPD_STATE_UNKNOWN;
	status->song_pos = -1;
	status->song_id = -1;
	status->next_song_pos = -1;
	status->next_song_id = -1;
	status->elapsed_time = 0;
	status->elapsed_ms = 0;
	status->total_time = 0;
	status->kbit_rate = 0;
	memset(&status->audio_format, 0, sizeof(status->audio_format));
	status->crossfade = 0;
	status->mixrampdb = 100.0;
	status->mixrampdelay = -1.0;
	status->partition = NULL;
	status->error = NULL;
	status->update_id = 0;

	return status;
}

/**
 * Parses the fractional part of the "elapsed" response line.  Up to
 * three digits are parsed.
 */
static unsigned
parse_ms(const char *p)
{
	unsigned ms;

	if (*p >= '0' && *p <= '9')
		ms = 100 * (*p++ - '0');
	else
		return 0;

	if (*p >= '0' && *p <= '9')
		ms += 10 * (*p - '0');
	else
		return ms;

	if (*p >= '0' && *p <= '9')
		ms += *p - '0';

	return ms;
}

static enum mpd_state
parse_mpd_state(const char *p)
{
	if (strcmp(p, "play") == 0)
		return MPD_STATE_PLAY;
	else if (strcmp(p, "stop") == 0)
		return MPD_STATE_STOP;
	else if (strcmp(p, "pause") == 0)
		return MPD_STATE_PAUSE;
	else
		return MPD_STATE_UNKNOWN;
}

static enum mpd_single_state
parse_mpd_single_state(const char *p)
{
	if (strcmp(p, "0") == 0)
		return MPD_SINGLE_OFF;
	else if (strcmp(p, "1") == 0)
		return MPD_SINGLE_ON;
	else if (strcmp(p, "oneshot") == 0)
		return MPD_SINGLE_ONESHOT;
	else
		return MPD_SINGLE_UNKNOWN;
}

void
mpd_status_feed(struct mpd_status *status, const struct mpd_pair *pair)
{
	assert(status != NULL);
	assert(pair != NULL);

	if (strcmp(pair->name, "volume") == 0)
		status->volume = atoi(pair->value);
	else if (strcmp(pair->name, "repeat") == 0)
		status->repeat = !!atoi(pair->value);
	else if (strcmp(pair->name, "random") == 0)
		status->random = !!atoi(pair->value);
	else if (strcmp(pair->name, "single") == 0)
		status->single = parse_mpd_single_state(pair->value);
	else if (strcmp(pair->name, "consume") == 0)
		status->consume = !!atoi(pair->value);
	else if (strcmp(pair->name, "playlist") == 0)
		status->queue_version = strtoul(pair->value, NULL, 10);
	else if (strcmp(pair->name, "playlistlength") == 0)
		status->queue_length = atoi(pair->value);
	else if (strcmp(pair->name, "bitrate") == 0)
		status->kbit_rate = atoi(pair->value);
	else if (strcmp(pair->name, "state") == 0)
		status->state = parse_mpd_state(pair->value);
	else if (strcmp(pair->name, "song") == 0)
		status->song_pos = atoi(pair->value);
	else if (strcmp(pair->name, "songid") == 0)
		status->song_id = atoi(pair->value);
	else if (strcmp(pair->name, "nextsong") == 0)
		status->next_song_pos = atoi(pair->value);
	else if (strcmp(pair->name, "nextsongid") == 0)
		status->next_song_id = atoi(pair->value);
	else if (strcmp(pair->name, "time") == 0) {
		char *endptr;

		status->elapsed_time = strtoul(pair->value, &endptr, 10);
		if (*endptr == ':')
			status->total_time = strtoul(endptr + 1, NULL, 10);

		if (status->elapsed_ms == 0)
			status->elapsed_ms = status->elapsed_time * 1000;
	} else if (strcmp(pair->name, "elapsed") == 0) {
		char *endptr;

		status->elapsed_ms = strtoul(pair->value, &endptr, 10) * 1000;
		if (*endptr == '.')
			status->elapsed_ms += parse_ms(endptr + 1);

		if (status->elapsed_time == 0)
			status->elapsed_time = status->elapsed_ms / 1000;
	} else if (strcmp(pair->name, "partition") == 0) {
		free(status->partition);
		status->partition = strdup(pair->value);
	} else if (strcmp(pair->name, "error") == 0) {
		free(status->error);
		status->error = strdup(pair->value);
	} else if (strcmp(pair->name, "xfade") == 0)
		status->crossfade = atoi(pair->value);
	else if (strcmp(pair->name, "mixrampdb") == 0)
		status->mixrampdb = atof(pair->value);
	else if (strcmp(pair->name, "mixrampdelay") == 0)
		status->mixrampdelay = atof(pair->value);
	else if (strcmp(pair->name, "updating_db") == 0)
		status->update_id = atoi(pair->value);
	else if (strcmp(pair->name, "audio") == 0)
		mpd_parse_audio_format(&status->audio_format, pair->value);
}

void mpd_status_free(struct mpd_status * status)
{
	assert(status != NULL);

	free(status->partition);
	free(status->error);
	free(status);
}

int mpd_status_get_volume(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->volume;
}

bool
mpd_status_get_repeat(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->repeat;
}

bool
mpd_status_get_random(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->random;
}

enum mpd_single_state
mpd_status_get_single_state(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->single;
}

bool
mpd_status_get_single(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->single == MPD_SINGLE_ONESHOT ||
	       status->single == MPD_SINGLE_ON;
}

bool
mpd_status_get_consume(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->consume;
}

unsigned
mpd_status_get_queue_length(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->queue_length;
}

unsigned
mpd_status_get_queue_version(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->queue_version;
}

enum mpd_state
mpd_status_get_state(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->state;
}

unsigned
mpd_status_get_crossfade(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->crossfade;
}

float
mpd_status_get_mixrampdb(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->mixrampdb;
}

float
mpd_status_get_mixrampdelay(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->mixrampdelay;
}

int
mpd_status_get_song_pos(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->song_pos;
}

int
mpd_status_get_song_id(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->song_id;
}

int
mpd_status_get_next_song_pos(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->next_song_pos;
}

int
mpd_status_get_next_song_id(const struct mpd_status *status)
{
	return status->next_song_id;
}

unsigned
mpd_status_get_elapsed_time(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->elapsed_time;
}

unsigned
mpd_status_get_elapsed_ms(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->elapsed_ms;
}

unsigned
mpd_status_get_total_time(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->total_time;
}

unsigned
mpd_status_get_kbit_rate(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->kbit_rate;
}

const struct mpd_audio_format *
mpd_status_get_audio_format(const struct mpd_status *status)
{
	assert(status != NULL);

	return !mpd_audio_format_is_empty(&status->audio_format)
		? &status->audio_format
		: NULL;
}

unsigned
mpd_status_get_update_id(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->update_id;
}

const char *
mpd_status_get_partition(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->partition;
}

const char *
mpd_status_get_error(const struct mpd_status *status)
{
	assert(status != NULL);

	return status->error;
}
