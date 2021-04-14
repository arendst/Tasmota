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

/*! \file
 * \brief MPD client library
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_STATS_H
#define MPD_STATS_H

#include "compiler.h"

#include <stdbool.h>

struct mpd_connection;
struct mpd_pair;

/**
 * \struct mpd_stats
 *
 * An opaque object representing MPD's response to the "stats"
 * command.  To release this object, call mpd_stats_free().
 */
struct mpd_stats;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Send the "stats" command to MPD. Call mpd_recv_stats() to read the response.
 *
 * @return true on success
 */
bool
mpd_send_stats(struct mpd_connection *connection);

/**
 * Begins parsing server stats: creates a new empty #mpd_stats object.
 * Free it with mpd_stats_free().
 *
 * @return the newly allocated #mpd_stats object, or NULL if out of
 * memory
 */
mpd_malloc
struct mpd_stats *
mpd_stats_begin(void);

/**
 * Parses the pair, adding its information to the specified #mpd_stats
 * object.
 */
void
mpd_stats_feed(struct mpd_stats *status, const struct mpd_pair *pair);

/**
 * Reads the "stats" response from MPD.
 *
 * @return a #mpd_stats object, or NULL on error
 */
mpd_malloc
struct mpd_stats *
mpd_recv_stats(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_stats() and mpd_recv_stats().
 */
mpd_malloc
struct mpd_stats *
mpd_run_stats(struct mpd_connection *connection);

/**
 * Frees a #mpd_stats object.
 */
void mpd_stats_free(struct mpd_stats *stats);

/**
 * @return the number of distinct artists in MPD's database, or 0 if
 * unknown
 */
mpd_pure
unsigned
mpd_stats_get_number_of_artists(const struct mpd_stats *stats);

/**
 * @return the number of distinct album names in MPD's database, or 0
 * if unknown
 */
mpd_pure
unsigned
mpd_stats_get_number_of_albums(const struct mpd_stats *stats);

/**
 * @return the total number of song files in MPD's database, or 0 if
 * unknown
 */
mpd_pure
unsigned
mpd_stats_get_number_of_songs(const struct mpd_stats *stats);

/**
 * @return the uptime of MPD in seconds, or 0 if unknown
 */
mpd_pure
unsigned long mpd_stats_get_uptime(const struct mpd_stats *stats);

/**
 * @return the UNIX time stamp of the last database update, or 0 if
 * unknown
 */
mpd_pure
unsigned long mpd_stats_get_db_update_time(const struct mpd_stats *stats);

/**
 * @return the accumulated time MPD was playing music since the
 * process was started, or 0 if unknown
 */
mpd_pure
unsigned long mpd_stats_get_play_time(const struct mpd_stats *stats);

/**
 * @return the accumulated duration of all songs in the database, or 0
 * if unknown
 */
mpd_pure
unsigned long mpd_stats_get_db_play_time(const struct mpd_stats *stats);

#ifdef __cplusplus
}
#endif

#endif
