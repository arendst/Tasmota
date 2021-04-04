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

#include <mpd/list.h>
#include <mpd/send.h>
#include "internal.h"
#include "isend.h"

#include <assert.h>

bool
mpd_command_list_begin(struct mpd_connection *connection, bool discrete_ok)
{
	bool success;

	assert(connection != NULL);

	if (connection->sending_command_list) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "already in command list mode");
		return false;
	}

	success = mpd_send_command2(connection,
				    discrete_ok
				    ? "command_list_ok_begin"
				    : "command_list_begin");
	if (!success)
		return false;

	connection->sending_command_list = true;
	connection->sending_command_list_ok = discrete_ok;
	connection->command_list_remaining = 0;
	connection->discrete_finished = false;

	return true;
}

bool
mpd_command_list_end(struct mpd_connection *connection)
{
	bool success;

	assert(connection != NULL);

	if (!connection->sending_command_list) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "not in command list mode");
		return false;
	}

	connection->sending_command_list = false;
	success = mpd_send_command(connection, "command_list_end", NULL);
	/* sending_command_list will be cleared when the user requests the
	   command list response (a function that calls mpd_recv_pair()) */
	connection->sending_command_list = true;
	if (!success)
		return false;

	assert(connection->receiving);
	return true;
}
