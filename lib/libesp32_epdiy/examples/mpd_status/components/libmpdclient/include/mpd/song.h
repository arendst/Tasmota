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

#ifndef MPD_SONG_H
#define MPD_SONG_H

#include "tag.h"
#include "compiler.h"

#include <stdbool.h>
#include <time.h>

struct mpd_pair;
struct mpd_connection;

/**
 * \struct mpd_song
 *
 * An opaque representation for a song in MPD's database or playlist.
 * Use the functions provided by this header to access the object's
 * attributes.
 */
struct mpd_song;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Free memory allocated by the #mpd_song object.
 */
void mpd_song_free(struct mpd_song *song);

/**
 * Duplicates the specified #mpd_song object.
 *
 * @returns the copy, or NULL if out of memory
 */
mpd_malloc
struct mpd_song *
mpd_song_dup(const struct mpd_song *song);

/**
 * Returns the URI of the song.  This is either a path relative to the
 * MPD music directory (without leading slash), or an URL with a
 * scheme, e.g. a HTTP URL for a radio stream.
 */
mpd_pure
const char *
mpd_song_get_uri(const struct mpd_song *song);

/**
 * Queries a tag value.
 *
 * @param song the song object
 * @param type the tag type
 * @param idx pass 0 to get the first value for this tag type.  This
 * argument may be used to iterate all values, until this function
 * returns NULL
 * @return the tag value, or NULL if this tag type (or this index)
 * does not exist
 */
mpd_pure
const char *
mpd_song_get_tag(const struct mpd_song *song,
		 enum mpd_tag_type type, unsigned idx);

/**
 * Returns the duration of this song in seconds.  0 means the duration
 * is unknown.
 */
mpd_pure
unsigned
mpd_song_get_duration(const struct mpd_song *song);

/**
 * Returns the duration of this song in milliseconds.  0 means the
 * duration is unknown.
 *
 * @since libmpdclient 2.10
 */
mpd_pure
unsigned
mpd_song_get_duration_ms(const struct mpd_song *song);

/**
 * Returns the start of the virtual song within the physical file in
 * seconds.
 *
 * @since libmpdclient 2.3
 */
mpd_pure
unsigned
mpd_song_get_start(const struct mpd_song *song);

/**
 * Returns the end of the virtual song within the physical file in
 * seconds. 0 means that the physical song file is played to the end.
 *
 * @since libmpdclient 2.3
 */
mpd_pure
unsigned
mpd_song_get_end(const struct mpd_song *song);

/**
 * @return the POSIX UTC time stamp of the last modification, or 0 if
 * that is unknown
 */
mpd_pure
time_t
mpd_song_get_last_modified(const struct mpd_song *song);

/**
 * Sets the position within the queue.  This value is not used for
 * songs which are not in the queue.
 *
 * This function is useful when applying the values returned by
 * mpd_recv_queue_change_brief().
 */
void
mpd_song_set_pos(struct mpd_song *song, unsigned pos);

/**
 * Returns the position of this song in the queue.  The value is
 * undefined if you did not obtain this song from the queue.
 */
mpd_pure
unsigned
mpd_song_get_pos(const struct mpd_song *song);

/**
 * Returns the id of this song in the playlist.  The value is
 * undefined if you did not obtain this song from the queue.
 */
mpd_pure
unsigned
mpd_song_get_id(const struct mpd_song *song);

/**
 * Returns the priority of this song in the playlist.  The value is
 * undefined if you did not obtain this song from the queue.
 *
 * @since libmpdclient 2.8
 */
mpd_pure
unsigned
mpd_song_get_prio(const struct mpd_song *song);

/**
 * Returns audio format as determined by MPD's decoder plugin.  May
 * return NULL if the format is not available or unknown.
 *
 * @since libmpdclient 2.15
 */
mpd_pure
const struct mpd_audio_format *
mpd_song_get_audio_format(const struct mpd_song *song);

/**
 * Begins parsing a new song.
 *
 * @param pair the first pair in this song (name must be "file")
 * @return the new #mpd_entity object, or NULL on error (out of
 * memory, or pair name is not "file")
 */
mpd_malloc
struct mpd_song *
mpd_song_begin(const struct mpd_pair *pair);

/**
 * Parses the pair, adding its information to the specified
 * #mpd_song object.
 *
 * @return true if the pair was parsed and added to the song (or if
 * the pair was not understood and ignored), false if this pair is the
 * beginning of the next song
 */
bool
mpd_song_feed(struct mpd_song *song, const struct mpd_pair *pair);

/**
 * Receives the next song from the MPD server.
 *
 * @return a #mpd_song object, or NULL on error or if the song list is
 * finished
 */
mpd_malloc
struct mpd_song *
mpd_recv_song(struct mpd_connection *connection);

#ifdef __cplusplus
}
#endif

#endif
