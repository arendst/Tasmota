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

#ifndef MPD_ENTITY_H
#define MPD_ENTITY_H

#include "song.h"
#include "directory.h"
#include "client.h"
#include "compiler.h"

struct mpd_pair;

/**
 * The type of a #mpd_entity object.
 */
enum mpd_entity_type {
	/**
	 * The type of the entity received from MPD is not implemented
	 * in this version of libmpdclient.
	 */
	MPD_ENTITY_TYPE_UNKNOWN,

	/**
	 * A directory (#mpd_directory) containing more entities.
	 */
	MPD_ENTITY_TYPE_DIRECTORY,

	/**
	 * A song file (#mpd_song) which can be added to the playlist.
	 */
	MPD_ENTITY_TYPE_SONG,

	/**
	 * A stored playlist (#mpd_playlist).
	 */
	MPD_ENTITY_TYPE_PLAYLIST,
};

/**
 * \struct mpd_entity
 *
 * An "entity" is an object returned by commands like "lsinfo".  It is
 * an object wrapping all possible entity types.
 */
struct mpd_entity;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Releases an entity.  This also frees the wrapped object.
 */
void
mpd_entity_free(struct mpd_entity *entity);

/**
 * @return the type of this entity.
 */
mpd_pure
enum mpd_entity_type
mpd_entity_get_type(const struct mpd_entity *entity);

/**
 * Obtains a pointer to the #mpd_directory object enclosed by this
 * #mpd_entity.  Calling this function is only allowed if
 * mpd_entity_get_type() has returned #MPD_ENTITY_TYPE_DIRECTORY.
 *
 * @return the directory object
 */
mpd_pure
const struct mpd_directory *
mpd_entity_get_directory(const struct mpd_entity *entity);

/**
 * Obtains a pointer to the #mpd_song object enclosed by this
 * #mpd_entity.  Calling this function is only allowed if
 * mpd_entity_get_type() has returned #MPD_ENTITY_TYPE_SONG.
 *
 * @return the song object
 */
mpd_pure
const struct mpd_song *
mpd_entity_get_song(const struct mpd_entity *entity);

/**
 * Obtains a pointer to the #mpd_playlist object enclosed by
 * this #mpd_entity.  Calling this function is only allowed if
 * mpd_entity_get_type() has returned #MPD_ENTITY_TYPE_PLAYLIST.
 *
 * @return the playlist object
 */
mpd_pure
const struct mpd_playlist *
mpd_entity_get_playlist(const struct mpd_entity *entity);

/**
 * Begins parsing a new entity.
 *
 * @param pair the first pair in this entity
 * @return the new #mpd_entity object, or NULL on error (out of memory)
 */
mpd_malloc
struct mpd_entity *
mpd_entity_begin(const struct mpd_pair *pair);

/**
 * Parses the pair, adding its information to the specified
 * #mpd_entity object.
 *
 * @return true if the pair was parsed and added to the entity (or if
 * the pair was not understood and ignored), false if this pair is the
 * beginning of the next entity
 */
bool
mpd_entity_feed(struct mpd_entity *entity, const struct mpd_pair *pair);

/**
 * Receives the next entity from the MPD server.
 *
 * @return an entity object, or NULL on error or if the entity list is
 * finished
 */
mpd_malloc
struct mpd_entity *
mpd_recv_entity(struct mpd_connection *connection);

#ifdef __cplusplus
}
#endif

#endif
