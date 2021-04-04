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

#include <mpd/playlist.h>
#include <mpd/send.h>
#include <mpd/response.h>
#include "isend.h"
#include "run.h"

#include <limits.h>
#include <stdio.h>

/* (bits+1)/3 (plus the sign character) */
enum {
	UNSIGNEDLEN = (sizeof(unsigned) * CHAR_BIT + 1) / 3 + 1,
};

bool
mpd_send_list_playlists(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "listplaylists", NULL);
}

bool
mpd_send_list_playlist(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "listplaylist", name, NULL);
}

bool
mpd_send_list_playlist_meta(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "listplaylistinfo", name, NULL);
}

bool
mpd_send_playlist_clear(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "playlistclear", name, NULL);
}

bool
mpd_run_playlist_clear(struct mpd_connection *connection, const char *name)
{
	return mpd_run_check(connection) &&
		mpd_send_playlist_clear(connection, name) &&
		mpd_response_finish(connection);
}

bool
mpd_send_playlist_add(struct mpd_connection *connection, const char *name,
		      const char *path)
{
	return mpd_send_command(connection, "playlistadd", name, path, NULL);
}

bool
mpd_run_playlist_add(struct mpd_connection *connection,
		     const char *name, const char *path)
{
	return mpd_run_check(connection) &&
		mpd_send_playlist_add(connection, name, path) &&
		mpd_response_finish(connection);
}

bool
mpd_send_playlist_move(struct mpd_connection *connection, const char *name,
		       unsigned from, unsigned to)
{
	char from_string[UNSIGNEDLEN], to_string[UNSIGNEDLEN];

	snprintf(from_string, sizeof(from_string), "%u", from);
	snprintf(to_string, sizeof(to_string), "%u", to);

	return mpd_send_command(connection, "playlistmove", name,
				from_string, to_string, NULL);
}

bool
mpd_send_playlist_delete(struct mpd_connection *connection, const char *name,
			 unsigned pos)
{
	char pos_string[UNSIGNEDLEN];

	snprintf(pos_string, sizeof(pos_string), "%u", pos);

	return mpd_send_command(connection, "playlistdelete", name, pos_string, NULL);
}

bool
mpd_run_playlist_delete(struct mpd_connection *connection,
			const char *name, unsigned pos)
{
	return mpd_run_check(connection) &&
		mpd_send_playlist_delete(connection, name, pos) &&
		mpd_response_finish(connection);
}

bool
mpd_send_save(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "save", name, NULL);
}

bool
mpd_run_save(struct mpd_connection *connection, const char *name)
{
	return mpd_run_check(connection) &&
		mpd_send_save(connection, name) &&
		mpd_response_finish(connection);
}

bool
mpd_send_load(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "load", name, NULL);
}

bool
mpd_run_load(struct mpd_connection *connection, const char *name)
{
	return mpd_run_check(connection) &&
		mpd_send_load(connection, name) &&
		mpd_response_finish(connection);
}

bool
mpd_send_load_range(struct mpd_connection *connection, const char *name,
		    unsigned start, unsigned end)
{
	return mpd_send_s_range_command(connection, "load", name,
					start, end);
}

bool
mpd_run_load_range(struct mpd_connection *connection, const char *name,
		   unsigned start, unsigned end)
{
	return mpd_run_check(connection) &&
		mpd_send_load_range(connection, name, start, end) &&
		mpd_response_finish(connection);
}

bool
mpd_send_rename(struct mpd_connection *connection,
		const char *from, const char *to)
{
	return mpd_send_command(connection, "rename", from, to, NULL);
}

bool
mpd_run_rename(struct mpd_connection *connection,
	       const char *from, const char *to)
{
	return mpd_run_check(connection) &&
		mpd_send_rename(connection, from, to) &&
		mpd_response_finish(connection);
}

bool
mpd_send_rm(struct mpd_connection *connection, const char *name)
{
	return mpd_send_command(connection, "rm", name, NULL);
}

bool
mpd_run_rm(struct mpd_connection *connection, const char *name)
{
	return mpd_run_check(connection) &&
		mpd_send_rm(connection, name) &&
		mpd_response_finish(connection);
}
