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
 * \brief Database
 *
 * This file declares functions which query or update MPD's music
 * database.
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_DATABASE_H
#define MPD_DATABASE_H

#include <stdbool.h>

struct mpd_connection;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get a recursive list of all directories, songs and playlist from
 * MPD.  They are returned without metadata.  This is a rather
 * expensive operation, because the response may be large.
 *
 * To read the response, you may use mpd_recv_entity().
 *
 * @param connection the connection to MPD
 * @param path an optional base path for the query
 * @return true on success, false on error
 */
bool
mpd_send_list_all(struct mpd_connection *connection, const char *path);

/**
 * Like #mpd_send_list_all(), but return metadata.  This operation is
 * even more expensive, because the response is larger.  If it is
 * larger than a configurable server-side limit, MPD may disconnect
 * you.
 *
 * To read the response, you may use mpd_recv_entity().
 *
 * @param connection the connection to MPD
 * @param path an optional base path for the query
 * @return true on success, false on error
 */
bool
mpd_send_list_all_meta(struct mpd_connection *connection, const char *path);

/**
 * Get a list of all directories, songs and playlist in a directory
 * from MPD, including metadata.
 *
 * To read the response, you may use mpd_recv_entity().
 *
 * @param connection the connection to MPD
 * @param path an optional directory to be listed
 * @return true on success, false on error
 */
bool
mpd_send_list_meta(struct mpd_connection *connection, const char *path);

/**
 * Lists the contents of the specified directory, including files that are
 * not recognized by MPD (command "listfiles").
 *
 * To read the response, you may use mpd_recv_entity().  All regular
 * files will be reported as #MPD_ENTITY_TYPE_SONG, even if they are
 * not actually songs.
 *
 * @param connection the connection to MPD
 * @param uri the directory to be listed
 * @return true on success, false on error
 *
 * @since libmpdclient 2.12, MPD 0.19
 */
bool
mpd_send_list_files(struct mpd_connection *connection, const char *uri);

/**
 * Send "readcomments".  Read the "comments" of a song file.  This
 * returns key/value pairs which can be read using mpd_recv_pair().
 *
 * @param connection the connection to MPD
 * @param path the relative path of the song file within the music
 * directory or an arbitrary file path starting with file:///
 * @return true on success, false on error
 *
 * @since libmpdclient 2.9
 */
bool
mpd_send_read_comments(struct mpd_connection *connection, const char *path);

/**
 * Instructs MPD to update the music database: find new files, remove
 * deleted files, update modified files.
 *
 * @param connection the connection to MPD
 * @param path optional path to update; if NULL, then all of the music
 * directory is updated
 * @return true on success, false on error
 */
bool
mpd_send_update(struct mpd_connection *connection, const char *path);

/**
 * Like mpd_send_update(), but also rescans unmodified files.
 *
 * @param connection the connection to MPD
 * @param path optional path to update; if NULL, then all of the music
 * directory is updated
 * @return true on success, false on error
 */
bool
mpd_send_rescan(struct mpd_connection *connection, const char *path);

/**
 * Receives the id of the update job which was submitted by
 * mpd_send_update().
 *
 * @param connection the connection to MPD
 * @return a positive job id on success, 0 on error
 */
unsigned
mpd_recv_update_id(struct mpd_connection *connection);

/**
 * Shortcut for mpd_send_update() and mpd_recv_update_id().
 *
 * @param connection the connection to MPD
 * @param path optional path to update; if NULL, then all of the music
 * directory is updated
 * @return a positive job id on success, 0 on error
 */
unsigned
mpd_run_update(struct mpd_connection *connection, const char *path);

/**
 * Like mpd_run_update(), but also rescans unmodified files.
 *
 * @param connection the connection to MPD
 * @param path optional path to update; if NULL, then all of the music
 * directory is updated
 * @return a positive job id on success, 0 on error
 */
unsigned
mpd_run_rescan(struct mpd_connection *connection, const char *path);

#ifdef __cplusplus
}
#endif

#endif
