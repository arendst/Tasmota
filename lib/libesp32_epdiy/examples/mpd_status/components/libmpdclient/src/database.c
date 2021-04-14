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

#include <mpd/database.h>
#include <mpd/send.h>
#include <mpd/recv.h>
#include <mpd/pair.h>
#include <mpd/response.h>
#include "run.h"

#include <stddef.h>
#include <stdlib.h>

bool
mpd_send_list_all(struct mpd_connection *connection, const char *dir)
{
	return mpd_send_command(connection, "listall", dir, NULL);
}

bool
mpd_send_list_all_meta(struct mpd_connection *connection, const char *dir)
{
	return mpd_send_command(connection, "listallinfo", dir, NULL);
}

bool
mpd_send_list_meta(struct mpd_connection *connection, const char *dir)
{
	return mpd_send_command(connection, "lsinfo", dir, NULL);
}

bool
mpd_send_list_files(struct mpd_connection *connection, const char *uri)
{
	return mpd_send_command(connection, "listfiles", uri, NULL);
}

bool
mpd_send_read_comments(struct mpd_connection *connection, const char *path)
{
	return mpd_send_command(connection, "readcomments", path, NULL);
}

bool
mpd_send_update(struct mpd_connection *connection, const char *path)
{
	return mpd_send_command(connection, "update", path, NULL);
}

bool
mpd_send_rescan(struct mpd_connection *connection, const char *path)
{
	return mpd_send_command(connection, "rescan", path, NULL);
}

unsigned
mpd_recv_update_id(struct mpd_connection *connection)
{
	struct mpd_pair *pair;
	int ret = 0;

	pair = mpd_recv_pair_named(connection, "updating_db");
	if (pair != NULL) {
		ret = atoi(pair->value);
		mpd_return_pair(connection, pair);
	}

	return ret;
}

unsigned
mpd_run_update(struct mpd_connection *connection, const char *path)
{
	unsigned id;

	if (!mpd_run_check(connection) || !mpd_send_update(connection, path))
		return 0;

	id = mpd_recv_update_id(connection);
	return id != 0 && mpd_response_finish(connection)
		? id : 0;
}

unsigned
mpd_run_rescan(struct mpd_connection *connection, const char *path)
{
	unsigned id;

	if (!mpd_run_check(connection) || !mpd_send_rescan(connection, path))
		return 0;

	id = mpd_recv_update_id(connection);
	return id != 0 && mpd_response_finish(connection)
		? id : 0;
}
