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

#include <mpd/player.h>
#include <mpd/send.h>
#include <mpd/song.h>
#include <mpd/response.h>
#include "isend.h"
#include "run.h"

#include <limits.h>
#include <stdio.h>

bool
mpd_send_current_song(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "currentsong", NULL);
}

struct mpd_song *
mpd_run_current_song(struct mpd_connection *connection)
{
	struct mpd_song *song;

	if (!mpd_run_check(connection) || !mpd_send_current_song(connection))
		return NULL;

	song = mpd_recv_song(connection);
	if (song == NULL)
		return NULL;

	if (!mpd_response_finish(connection)) {
		mpd_song_free(song);
		return NULL;
	}

	return song;
}

bool
mpd_send_play(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "play", NULL);
}

bool
mpd_run_play(struct mpd_connection *connection)
{
	return mpd_run_check(connection) && mpd_send_play(connection) &&
		mpd_response_finish(connection);
}

bool
mpd_send_play_pos(struct mpd_connection *connection, unsigned song_pos)
{
	return mpd_send_int_command(connection, "play", song_pos);
}

bool
mpd_run_play_pos(struct mpd_connection *connection, unsigned song_pos)
{
	return mpd_run_check(connection) &&
		mpd_send_play_pos(connection, song_pos) &&
		mpd_response_finish(connection);
}

bool
mpd_send_play_id(struct mpd_connection *connection, unsigned song_id)
{
	return mpd_send_int_command(connection, "playid", song_id);
}

bool
mpd_run_play_id(struct mpd_connection *connection, unsigned song_id)
{
	return mpd_run_check(connection) &&
		mpd_send_play_id(connection, song_id) &&
		mpd_response_finish(connection);
}

bool
mpd_send_stop(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "stop", NULL);
}

bool
mpd_run_stop(struct mpd_connection *connection)
{
	return mpd_run_check(connection) && mpd_send_stop(connection) &&
		mpd_response_finish(connection);
}

bool
mpd_send_toggle_pause(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "pause", NULL);
}

bool
mpd_run_toggle_pause(struct mpd_connection *connection)
{
	return mpd_run_check(connection) &&
		mpd_send_toggle_pause(connection) &&
		mpd_response_finish(connection);
}

bool
mpd_send_pause(struct mpd_connection *connection, bool mode)
{
	return mpd_send_int_command(connection, "pause", mode);
}

bool
mpd_run_pause(struct mpd_connection *connection, bool mode)
{
	return mpd_run_check(connection) && mpd_send_pause(connection, mode) &&
		mpd_response_finish(connection);
}

bool
mpd_send_next(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "next", NULL);
}

bool
mpd_run_next(struct mpd_connection *connection)
{
	return mpd_run_check(connection) && mpd_send_next(connection) &&
		mpd_response_finish(connection);
}

bool
mpd_send_previous(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "previous", NULL);
}

bool
mpd_run_previous(struct mpd_connection *connection)
{
	return mpd_run_check(connection) && mpd_send_previous(connection) &&
		mpd_response_finish(connection);
}

bool
mpd_send_seek_pos(struct mpd_connection *connection,
		 unsigned song_pos, unsigned t)
{
	return mpd_send_int2_command(connection, "seek", song_pos, t);
}

bool
mpd_run_seek_pos(struct mpd_connection *connection,
		unsigned song_pos, unsigned t)
{
	return mpd_run_check(connection) &&
		mpd_send_seek_pos(connection, song_pos, t) &&
		mpd_response_finish(connection);
}

bool
mpd_send_seek_id(struct mpd_connection *connection,
		 unsigned song_id, unsigned t)
{
	return mpd_send_int2_command(connection, "seekid", song_id, t);
}

bool
mpd_run_seek_id(struct mpd_connection *connection,
	       unsigned song_id, unsigned t)
{
	return mpd_run_check(connection) &&
		mpd_send_seek_id(connection, song_id, t) &&
		mpd_response_finish(connection);
}

bool
mpd_send_seek_id_float(struct mpd_connection *connection,
		       unsigned song_id, float t)
{
	return mpd_send_u_f_command(connection, "seekid", song_id, t);
}

bool
mpd_run_seek_id_float(struct mpd_connection *connection,
		      unsigned song_id, float t)
{
	return mpd_run_check(connection) &&
		mpd_send_seek_id_float(connection, song_id, t) &&
		mpd_response_finish(connection);
}

bool
mpd_send_seek_current(struct mpd_connection *connection,
		      float t, bool relative)
{
	char ts[32];
	if (relative)
		snprintf(ts, sizeof(ts), "%+.3f", t);
	else
		snprintf(ts, sizeof(ts), "%.3f", t);

	return mpd_send_command(connection, "seekcur", ts, NULL);
}

bool
mpd_run_seek_current(struct mpd_connection *connection,
		     float t, bool relative)
{
	return mpd_run_check(connection) &&
		mpd_send_seek_current(connection, t, relative) &&
		mpd_response_finish(connection);
}

bool
mpd_send_repeat(struct mpd_connection *connection, bool mode)
{
	return mpd_send_int_command(connection, "repeat", mode);
}

bool
mpd_run_repeat(struct mpd_connection *connection, bool mode)
{
	return mpd_run_check(connection) &&
		mpd_send_repeat(connection, mode) &&
		mpd_response_finish(connection);
}

bool
mpd_send_random(struct mpd_connection *connection, bool mode)
{
	return mpd_send_int_command(connection, "random", mode);
}

bool
mpd_run_random(struct mpd_connection *connection, bool mode)
{
	return mpd_run_check(connection) &&
		mpd_send_random(connection, mode) &&
		mpd_response_finish(connection);
}

static const char *
single_state_to_string(enum mpd_single_state state)
{
	switch (state) {
	case MPD_SINGLE_OFF:
		return "0";
	case MPD_SINGLE_ON:
		return "1";
	case MPD_SINGLE_ONESHOT:
		return "oneshot";
	case MPD_SINGLE_UNKNOWN:
		return NULL;
	}
	return NULL;
}
bool
mpd_send_single_state(struct mpd_connection *connection,
		      enum mpd_single_state state)
{
	const char *state_str = single_state_to_string(state);
	if (state_str == NULL)
		return false;

	return mpd_send_command(connection, "single", state_str, NULL);
}

bool
mpd_run_single_state(struct mpd_connection *connection,
		      enum mpd_single_state state)
{
	return mpd_run_check(connection) &&
		mpd_send_single_state(connection, state) &&
		mpd_response_finish(connection);
}

bool
mpd_send_single(struct mpd_connection *connection, bool mode)
{
	return mpd_send_int_command(connection, "single", mode);
}

bool
mpd_run_single(struct mpd_connection *connection, bool mode)
{
	return mpd_run_check(connection) &&
		mpd_send_single(connection, mode) &&
		mpd_response_finish(connection);
}

bool
mpd_send_consume(struct mpd_connection *connection, bool mode)
{
	return mpd_send_int_command(connection, "consume", mode);
}

bool
mpd_run_consume(struct mpd_connection *connection, bool mode)
{
	return mpd_run_check(connection) &&
		mpd_send_consume(connection, mode) &&
		mpd_response_finish(connection);
}

bool
mpd_send_crossfade(struct mpd_connection *connection, unsigned seconds)
{
	return mpd_send_int_command(connection, "crossfade", seconds);
}

bool
mpd_run_crossfade(struct mpd_connection *connection, unsigned seconds)
{
	return mpd_run_check(connection) &&
		mpd_send_crossfade(connection, seconds) &&
		mpd_response_finish(connection);
}

bool
mpd_send_mixrampdb(struct mpd_connection *connection, float db)
{
	return mpd_send_float_command(connection, "mixrampdb", db);
}

bool
mpd_run_mixrampdb(struct mpd_connection *connection, float db)
{
	return mpd_run_check(connection) &&
		mpd_send_mixrampdb(connection, db) &&
		mpd_response_finish(connection);
}

bool
mpd_send_mixrampdelay(struct mpd_connection *connection, float seconds)
{
	return mpd_send_float_command(connection, "mixrampdelay", seconds);
}

bool
mpd_run_mixrampdelay(struct mpd_connection *connection, float seconds)
{
	return mpd_run_check(connection) &&
		mpd_send_mixrampdelay(connection, seconds) &&
		mpd_response_finish(connection);
}

bool
mpd_send_clearerror(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "clearerror", NULL);
}

bool
mpd_run_clearerror(struct mpd_connection *connection)
{
	return mpd_run_check(connection) && mpd_send_clearerror(connection) &&
		mpd_response_finish(connection);
}
