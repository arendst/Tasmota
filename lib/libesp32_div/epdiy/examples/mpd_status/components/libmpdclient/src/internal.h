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

#ifndef MPD_INTERNAL_H
#define MPD_INTERNAL_H

#include <mpd/pair.h>

#include "ierror.h"

/* for struct timeval */
#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/time.h>
#endif

/**
 * This opaque object represents a connection to a MPD server.  Call
 * mpd_connection_new() to create a new instance.
 */
struct mpd_connection {
	/**
	 * The connection settings.
	 */
	struct mpd_settings *settings;

	/**
	 * The version number received by the MPD server.
	 */
	unsigned version[3];

	/**
	 * The last error which occurred.  This attribute must be
	 * cleared with mpd_connection_clear_error() before another
	 * command may be executed.
	 */
	struct mpd_error_info error;

	/**
	 * The backend of the MPD connection.
	 */
	struct mpd_async *async;

	/**
	 * The timeout for all commands.  If the MPD server does not
	 * respond within this time span, the connection is assumed to
	 * be dead.
	 */
	struct timeval timeout;

	/**
	 * The parser object used to parse response lines received
	 * from the MPD server.
	 */
	struct mpd_parser *parser;

	/**
	 * Are we currently receiving the response of a command?
	 */
	bool receiving;

	/**
	 * Sending a command list right now?
	 */
	bool sending_command_list;

	/**
	 * Sending a command list with "command_list_ok"?
	 */
	bool sending_command_list_ok;

	/**
	 * Did the caller finish reading one sub response?
	 * (i.e. list_OK was received, and mpd_recv_pair() has
	 * returned NULL)
	 */
	bool discrete_finished;

	/**
	 * The number of list_OK responses remaining in the command
	 * list response.
	 */
	int command_list_remaining;

	/**
	 * Declare the validity of the #pair attribute.
	 */
	enum {
		/**
		 * There is no pair currently.
		 */
		PAIR_STATE_NONE,

		/**
		 * The NULL pair has been enqueued with
		 * mpd_enqueue_pair().
		 */
		PAIR_STATE_NULL,

		/**
		 * A pair has been enqueued with mpd_enqueue_pair().
		 */
		PAIR_STATE_QUEUED,

		/**
		 * There is a pair, and it has been delivered to the
		 * caller via mpd_recv_pair().  We're waiting for him
		 * to call mpd_return_pair().
		 */
		PAIR_STATE_FLOATING,
	} pair_state;

	/**
	 * The name-value pair which was "unread" with
	 * mpd_enqueue_pair().  The special value #PAIR_NONE denotes
	 * that this value is empty, while NULL means that somebody
	 * "unread" the NULL pointer.
	 */
	struct mpd_pair pair;

	/**
	 * The search request which is being built, committed by
	 * mpd_search_commit().
	 */
	char *request;
};

/**
 * Copies the error state from connection->sync to connection->error.
 */
void
mpd_connection_sync_error(struct mpd_connection *connection);

static inline const struct timeval *
mpd_connection_timeout(const struct mpd_connection *connection)
{
	return connection->timeout.tv_sec != 0 ||
		connection->timeout.tv_usec != 0
		? &connection->timeout
		: NULL;
}

#endif
