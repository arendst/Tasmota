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

#include <mpd/partition.h>
#include <mpd/send.h>
#include <mpd/response.h>
#include "internal.h"
#include "run.h"

#include <assert.h>
#include <stddef.h>

bool
mpd_send_newpartition(struct mpd_connection *connection, const char *partition)
{
	return mpd_send_command(connection, "newpartition", partition, NULL);
}

bool
mpd_run_newpartition(struct mpd_connection *connection, const char *partition)
{
	return mpd_run_check(connection) &&
		mpd_send_newpartition(connection, partition) &&
		mpd_response_finish(connection);
}

bool
mpd_send_delete_partition(struct mpd_connection *connection,
			  const char *partition)
{
	return mpd_send_command(connection, "delpartition", partition, NULL);
}

bool
mpd_run_delete_partition(struct mpd_connection *connection,
			 const char *partition)
{
	return mpd_run_check(connection) &&
		mpd_send_delete_partition(connection, partition) &&
		mpd_response_finish(connection);
}

bool
mpd_send_switch_partition(struct mpd_connection *connection,
			  const char *partition)
{
	return mpd_send_command(connection, "partition", partition, NULL);
}

bool
mpd_run_switch_partition(struct mpd_connection *connection,
			 const char *partition)
{
	return mpd_run_check(connection) &&
		mpd_send_switch_partition(connection, partition) &&
		mpd_response_finish(connection);
}

bool
mpd_send_listpartitions(struct mpd_connection *connection)
{
	return mpd_send_command(connection, "listpartitions", NULL);
}

struct mpd_partition *
mpd_recv_partition(struct mpd_connection *connection)
{
	struct mpd_pair *pair = mpd_recv_partition_pair(connection);
	if (pair == NULL)
		return NULL;

	struct mpd_partition *partition = mpd_partition_new(pair);
	mpd_return_pair(connection, pair);
	return partition;
}
