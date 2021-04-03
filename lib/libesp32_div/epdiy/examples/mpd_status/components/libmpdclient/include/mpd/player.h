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

/*! \file
 * \brief MPD client library
 *
 * Controlling playback.
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_PLAYER_H
#define MPD_PLAYER_H

#include "compiler.h"
#include "status.h"

#include <stdbool.h>

struct mpd_connection;
struct mpd_song;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * There are two ways to address songs within the queue: by their position and
 * by their id.
 * The position is a 0-based index. It is unstable by design: if you move,
 * delete or insert songs, all following indices will change, and a client can
 * never be sure what song is behind a given index/position.
 *
 * Song ids on the other hand are stable: an id is assigned to a song when it
 * is added, and will stay the same, no matter how much it is moved around.
 * Adding the same song twice will assign different ids to them, and a
 * deleted-and-readded song will have a new id. This way, a client can always
 * be sure the correct song is being used.
 *
 * Many commands come in two flavors, one for each address type. Whenever
 * possible, ids should be used.
 */

/**
 * Fetches the currently selected song (the song referenced by
 * mpd_status_get_song_id()).
 * Call mpd_recv_song() to receive the response.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_current_song(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_current_song() and mpd_recv_song().
 *
 * @param connection the connection to MPD
 * @return the current song, or NULL on error or if there is no
 * current song
 */
mpd_malloc
struct mpd_song *
mpd_run_current_song(struct mpd_connection *connection);

/**
 * Starts playing the current song from the beginning.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_play(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_play() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_run_play(struct mpd_connection *connection);

/**
 * Starts playing the specified song from the beginning.
 *
 * @param connection the connection to MPD
 * @param song_pos the position of the song in the queue
 * @return true on success, false on error
 */
bool
mpd_send_play_pos(struct mpd_connection *connection, unsigned song_pos);

/**
 * Shortcut for mpd_send_play() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param song_pos the position of the song in the queue
 * @return true on success, false on error
 */
bool
mpd_run_play_pos(struct mpd_connection *connection, unsigned song_pos);

/**
 * Starts playing the specified song from the beginning.
 *
 * @param connection the connection to MPD
 * @param id the id of the song
 * @return true on success, false on error
 */
bool
mpd_send_play_id(struct mpd_connection *connection, unsigned song_id);

/**
 * Shortcut for mpd_send_play_id() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param song_id the id of the song
 * @return true on success, false on error
 */
bool
mpd_run_play_id(struct mpd_connection *connection, unsigned song_id);

/**
 * Stops playing the current song.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_stop(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_stop() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_run_stop(struct mpd_connection *connection);

/**
 * This function uses a deprecated feature of MPD, you should avoid it.
 *
 * Toggles the pause mode by sending "pause" without arguments.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_toggle_pause(struct mpd_connection *connection);

/**
 * This function uses a deprecated feature of MPD, you should avoid it.
 * Shortcut for mpd_send_toggle_pause() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_run_toggle_pause(struct mpd_connection *connection);

/**
 * Pauses/Resumes playing the current song.
 * If mode is true, MPD pauses the song; otherwise, MPD resumes the song.
 *
 * @param connection the connection to MPD
 * @param mode if true: pause, if false: resume
 * @return true on success, false on error
 */
bool
mpd_send_pause(struct mpd_connection *connection, bool mode);

/**
 * Shortcut for mpd_send_pause() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param mode if true: pause, if false: resume
 * @return true on success, false on error
 */
bool
mpd_run_pause(struct mpd_connection *connection, bool mode);

/**
 * Play the next song in the playlist.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_next(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_next() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_run_next(struct mpd_connection *connection);

/**
 * Play the previous song in the playlist.
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_send_previous(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_previous() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_run_previous(struct mpd_connection *connection);

/**
 * Seeks to the position t (in seconds) of position song_pos in the playlist.
 *
 * @param connection the connection to MPD
 * @param song_pos the position of the song in the queue
 * @param t the position within the song, in seconds
 * @return true on success, false on error
 */
bool
mpd_send_seek_pos(struct mpd_connection *connection,
		  unsigned song_pos, unsigned t);

/**
 * Shortcut for mpd_send_seek_pos() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param song_pos the position of the song in the queue
 * @param t the position within the song, in seconds
 * @return true on success, false on error
 */
bool
mpd_run_seek_pos(struct mpd_connection *connection,
		 unsigned song_pos, unsigned t);

/**
 * Seeks to the position t (in seconds) of song id song_id.
 *
 * @param connection the connection to MPD
 * @param id the id of the song
 * @param t the position within the song, in seconds
 * @return true on success, false on error
 */
bool
mpd_send_seek_id(struct mpd_connection *connection,
		 unsigned song_id, unsigned t);

/**
 * Shortcut for mpd_send_seek_id() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param song_id the id of the song
 * @param t the position within the song, in seconds
 * @return true on success, false on error
 */
bool
mpd_run_seek_id(struct mpd_connection *connection,
		unsigned song_id, unsigned t);

/**
 * Seeks to the position t (in seconds; fractions allowed) of song id song_id.
 *
 * @param connection the connection to MPD
 * @param song_id the id of the song
 * @param t the position within the song, in seconds (fractions allowed)
 * @return true on success, false on error
 */
bool
mpd_send_seek_id_float(struct mpd_connection *connection,
		       unsigned song_id, float t);

/**
 * Shortcut for mpd_send_seek_id_float() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param song_id the id of the song
 * @param t the position within the song, in seconds (fractions allowed)
 * @return true on success, false on error
 */
bool
mpd_run_seek_id_float(struct mpd_connection *connection,
		      unsigned song_id, float t);

/**
 * Seeks the current song.
 *
 * @param connection the connection to MPD
 * @param t the position within the song, in seconds
 * @param relative true makes #t a relative to the current position
 * @return true on success, false on error
 *
 * @since MPD 0.17, libmpdclient 2.15
 */
bool
mpd_send_seek_current(struct mpd_connection *connection,
		      float t, bool relative);

/**
 * Shortcut for mpd_send_seek_current() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 *
 * @since MPD 0.17, libmpdclient 2.15
 */
bool
mpd_run_seek_current(struct mpd_connection *connection,
		     float t, bool relative);

/**
 * Sets repeat on/off for the current song.
 * If mode is true, MPD repeats the song; otherwise, MPD does not repeat the
 * song.
 *
 * @param connection the connection to MPD
 * @param mode if true: repeat, if false: do not repeat
 * @return true on success, false on error
 */
bool
mpd_send_repeat(struct mpd_connection *connection, bool mode);

/**
 * Shortcut for mpd_send_repeat() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param mode if true: pause, if false: resume
 * @return true on success, false on error
 */
bool
mpd_run_repeat(struct mpd_connection *connection, bool mode);

/**
 * Sets random mode on/off for the queue.
 * If mode is true, MPD enables random mode; otherwise, MPD disables random
 * mode.
 *
 * @param connection the connection to MPD
 * @param mode if true: enable random mode, if false: disable random mode
 * @return true on success, false on error
 */
bool
mpd_send_random(struct mpd_connection *connection, bool mode);

/**
 * Shortcut for mpd_send_random() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param mode if true: enable random mode, if false: disable random mode
 * @return true on success, false on error
 */
bool
mpd_run_random(struct mpd_connection *connection, bool mode);

/**
 * Sets single state for the playlist.
 * If state is #MPD_SINGLE_ON, MPD enables single mode: playback is stopped
 * after current song, or song is repeated if the repeat mode is enabled.
 *
 * If state is #MPD_SINGLE_OFF, MPD disables single mode: if random mode is
 * enabled, the playlist order is shuffled after it is played completely.
 *
 * If state is #MPD_SINGLE_ONESHOT, MPD enables single mode temporarily: single
 * mode is disabled (#MPD_SINGLE_OFF) after a song has been played and there is
 * another song in the current playlist.
 *
 * @param connection the connection to MPD
 * @param state the desired single mode state
 * @return true on success, false on error or state is #MPD_SINGLE_UNKNOWN
 *
 * @since MPD 0.21, libmpdclient 2.18.
 */
bool
mpd_send_single_state(struct mpd_connection *connection,
		      enum mpd_single_state state);

/**
 * Shortcut for mpd_send_single_state() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param state the desired single mode state
 * @return true on success, false on error or state is #MPD_SINGLE_UNKNOWN
 *
 * @since MPD 0.21, libmpdclient 2.18.
 */
bool
mpd_run_single_state(struct mpd_connection *connection,
		      enum mpd_single_state state);

/**
 * Sets single mode on/off for the playlist.
 * This function does not support the 'oneshot' state for single mode: use
 * mpd_send_single_state() instead.
 *
 * If mode is true, MPD enables single mode: playback is stopped after current
 * song, or song is repeated if the repeat mode is enabled.
 *
 * If mode is false, MPD disables single mode: if random mode is enabled, the
 * playlist order is shuffled after it is played completely.
 *
 * @param connection the connection to MPD
 * @param mode if true: enable single mode, if false: disable single mode
 * @return true on success, false on error
 *
 * @since MPD 0.15
 */
bool
mpd_send_single(struct mpd_connection *connection, bool mode);

/**
 * Shortcut for mpd_send_single() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param mode if true: enable single mode, if false: disable single mode
 * @return true on success, false on error
 *
 * @since MPD 0.15
 */
bool
mpd_run_single(struct mpd_connection *connection, bool mode);

/**
 * Sets consume mode on/off for the playlist.
 * If mode is true, MPD enables consume mode: each song played is removed from
 * the playlist.
 *
 * If mode is false, MPD disables consume mode.
 *
 * @param connection the connection to MPD
 * @param mode if true: enable consume mode, if false: disable consume mode
 * @return true on success, false on error
 *
 * @since MPD 0.15
 */
bool
mpd_send_consume(struct mpd_connection *connection, bool mode);

/**
 * Shortcut for mpd_send_consume() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param mode if true: enable consume mode, if false: disable consume mode
 * @return true on success, false on error
 *
 * @since MPD 0.15
 */
bool
mpd_run_consume(struct mpd_connection *connection, bool mode);

/**
 * Sets crossfading of seconds between songs on for the playlist.
 * Crossfading only happens when the songs audio format are the same.
 *
 * @param connection the connection to MPD
 * @param seconds seconds of crossfading between songs
 * @return true on success, false on error
 */
bool
mpd_send_crossfade(struct mpd_connection *connection, unsigned seconds);

/**
 * Shortcut for mpd_send_crossfade() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param seconds seconds of crossfading between songs
 * @return true on success, false on error
 */
bool
mpd_run_crossfade(struct mpd_connection *connection, unsigned seconds);

/**
 * Sets the threshold at which songs will be overlapped. Like crossfading but
 * doesn't fade the track volume, just overlaps.
 *
 * The songs need to have MixRamp tags added by an external tool. 0dB is the
 * normalized maximum volume; so use negative values (I prefer -17dB). In the
 * absence of MixRamp tags, crossfading will be used.
 *
 * @param connection the connection to MPD
 * @param db decibels of volume for overlapping songs
 * @return true on success, false on error
 *
 * @since libmpdclient 2.2
 */
bool
mpd_send_mixrampdb(struct mpd_connection *connection, float db);

/**
 * Shortcut for mpd_send_mixrampdb() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param db decibels of volume for overlapping songs
 * @return true on success, false on error
 *
 * @since libmpdclient 2.2
 */
bool
mpd_run_mixrampdb(struct mpd_connection *connection, float db);

/**
 * Sets additional time subtracted from the overlap calculated by mixrampdb.
 * A value of NaN disables MixRamp overlapping and falls back to crossfading.
 *
 * @param connection the connection to MPD
 * @param seconds seconds subtracted from the overlap calculated by mixrampdb
 * @return true on success, false on error
 *
 * @since libmpdclient 2.2
 */
bool
mpd_send_mixrampdelay(struct mpd_connection *connection, float seconds);

/**
 * Shortcut for mpd_send_mixrampdelay() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param seconds seconds subtracted from the overlap calculated by mixrampdb
 * @return true on success, false on error
 *
 * @since libmpdclient 2.2
 */
bool
mpd_run_mixrampdelay(struct mpd_connection *connection, float seconds);

/**
 * Clears the current error message in MPD's status (this is also accomplished
 * by any command that starts playback).
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 *
 * @since libmpdclient 2.4
 */
bool
mpd_send_clearerror(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_clearerror() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 *
 * @since libmpdclient 2.4
 */
bool
mpd_run_clearerror(struct mpd_connection *connection);

#ifdef __cplusplus
}
#endif

#endif
