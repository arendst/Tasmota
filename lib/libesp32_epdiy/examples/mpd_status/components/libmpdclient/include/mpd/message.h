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
 * \brief MPD client library
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_MESSAGE_H
#define MPD_MESSAGE_H

#include "recv.h"
#include "compiler.h"

#include <stdbool.h>

struct mpd_pair;
/**
 * \struct mpd_message
 */
struct mpd_message;
struct mpd_connection;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Begins parsing a new message.
 *
 * @param pair the first pair in this message (name must be "channel")
 * @return the new #mpd_entity object, or NULL on error (out of
 * memory, or pair name is not "channel")
 *
 * @since libmpdclient 2.5
 */
mpd_malloc
struct mpd_message *
mpd_message_begin(const struct mpd_pair *pair);

/**
 * Parses the pair, adding its information to the specified
 * #mpd_message object.
 *
 * @return true if the pair was parsed and added to the message (or if
 * the pair was not understood and ignored), false if this pair is the
 * beginning of the next message
 *
 * @since libmpdclient 2.5
 */
bool
mpd_message_feed(struct mpd_message *output, const struct mpd_pair *pair);

/**
 * Frees a #mpd_message object.
 *
 * @since libmpdclient 2.5
 */
void
mpd_message_free(struct mpd_message *message);

/**
 * Returns the channel name.
 *
 * @since libmpdclient 2.5
 */
mpd_pure
const char *
mpd_message_get_channel(const struct mpd_message *message);

/**
 * Returns the message text.
 *
 * @since libmpdclient 2.5
 */
mpd_pure
const char *
mpd_message_get_text(const struct mpd_message *message);

/**
 * Sends the "subscribe" command: subscribe to a message channel.
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_send_subscribe(struct mpd_connection *connection, const char *channel);

/**
 * Shortcut for mpd_send_subscribe() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_run_subscribe(struct mpd_connection *connection, const char *channel);

/**
 * Sends the "unsubscribe" command: unsubscribe from a message
 * channel.
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_send_unsubscribe(struct mpd_connection *connection, const char *channel);

/**
 * Shortcut for mpd_send_unsubscribe() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_run_unsubscribe(struct mpd_connection *connection, const char *channel);

/**
 * Sends the "sendmessage" command: send a message to a channel.
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @param text the message text
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_send_send_message(struct mpd_connection *connection,
		      const char *channel, const char *text);

/**
 * Shortcut for mpd_send_send_message() and mpd_response_finish().
 *
 * @param connection the connection to MPD
 * @param channel the channel name
 * @param text the message text
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_run_send_message(struct mpd_connection *connection,
		     const char *channel, const char *text);

/**
 * Sends the "readmessages" command: send a message to a channel.
 *
 * @param connection the connection to MPD
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_send_read_messages(struct mpd_connection *connection);

/**
 * Reads the next mpd_message from the MPD response.  Free the return
 * value with mpd_message_free().
 *
 * @return a mpd_message object on success, NULL on error or
 * end-of-response
 *
 * @since libmpdclient 2.5
 */
mpd_malloc
struct mpd_message *
mpd_recv_message(struct mpd_connection *connection);

/**
 * Sends the "channels" command: get a list of all channels.
 *
 * @param connection the connection to MPD
 * @return true on success
 *
 * @since libmpdclient 2.5
 */
bool
mpd_send_channels(struct mpd_connection *connection);

/**
 * Receives the next channel name.  Call this in a loop after
 * mpd_send_channels().
 *
 * Free the return value with mpd_return_pair().
 *
 * @param connection a #mpd_connection
 * @returns a "channel" pair, or NULL on error or if the end of the
 * response is reached
 *
 * @since libmpdclient 2.5
 */
mpd_malloc
static inline struct mpd_pair *
mpd_recv_channel_pair(struct mpd_connection *connection)
{
	return mpd_recv_pair_named(connection, "channel");
}

#ifdef __cplusplus
}
#endif

#endif
