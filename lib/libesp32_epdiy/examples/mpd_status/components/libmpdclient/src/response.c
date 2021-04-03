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

#include <mpd/response.h>
#include <mpd/recv.h>
#include "internal.h"

#include <assert.h>

bool
mpd_response_finish(struct mpd_connection *connection)
{
	struct mpd_pair *pair;

	if (mpd_error_is_defined(&connection->error))
		return false;

	if (connection->pair_state == PAIR_STATE_NULL)
		/* reset the stored NULL pair because it will conflict
		   with an assertion within the loop */
		connection->pair_state = PAIR_STATE_NONE;

	while (connection->receiving) {
		assert(!mpd_error_is_defined(&connection->error));

		connection->discrete_finished = false;

		pair = mpd_recv_pair(connection);
		assert(pair != NULL || !connection->receiving ||
		       (connection->sending_command_list &&
			connection->discrete_finished) ||
		       mpd_error_is_defined(&connection->error));

		if (pair != NULL)
			mpd_return_pair(connection, pair);
	}

	return !mpd_error_is_defined(&connection->error);
}

bool
mpd_response_next(struct mpd_connection *connection)
{
	struct mpd_pair *pair;

	if (mpd_error_is_defined(&connection->error))
		return false;

	if (!connection->receiving) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "Response is already finished");
		return false;
	}

	if (!connection->sending_command_list_ok) {
		mpd_error_code(&connection->error, MPD_ERROR_STATE);
		mpd_error_message(&connection->error,
				  "Not in command list mode");
		return false;
	}

	while (!connection->discrete_finished) {
		if (connection->command_list_remaining == 0 ||
		    !connection->receiving) {
			mpd_error_code(&connection->error,
				       MPD_ERROR_MALFORMED);
			mpd_error_message(&connection->error,
					  "No list_OK found");
			return false;
		}

		pair = mpd_recv_pair(connection);
		if (pair != NULL)
			mpd_return_pair(connection, pair);
		else if (mpd_error_is_defined(&connection->error))
			return false;
	}

	connection->discrete_finished = false;
	return true;
}
