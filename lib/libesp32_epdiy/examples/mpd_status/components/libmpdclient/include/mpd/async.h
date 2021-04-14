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
 * \brief Asynchronous MPD connections
 *
 * This class provides a very basic interface to MPD connections.  It
 * does not know much about the MPD protocol, it does not know any
 * specific MPD command.
 *
 * The constructor expects a socket descriptor which is already
 * connected to MPD.  The first thing it does is read the server's
 * handshake code ("OK MPD 0.15.0").
 */

#ifndef MPD_ASYNC_H
#define MPD_ASYNC_H

#include "error.h"
#include "compiler.h"

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

/**
 * Event bit mask for polling.
 */
enum mpd_async_event {
	/** ready to read from the file descriptor */
	MPD_ASYNC_EVENT_READ = 1,

	/** ready to write to the file descriptor */
	MPD_ASYNC_EVENT_WRITE = 2,

	/** hangup detected */
	MPD_ASYNC_EVENT_HUP = 4,

	/** I/O error */
	MPD_ASYNC_EVENT_ERROR = 8,
};

/**
 * \struct mpd_async
 *
 * This opaque object represents an asynchronous connection to a MPD
 * server.  Call mpd_async_new() to create a new instance.
 */
struct mpd_async;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a new asynchronous MPD connection, based on a stream socket
 * connected with MPD.
 *
 * @param fd the socket file descriptor of the stream connection to MPD
 * @return a mpd_async object, or NULL on out of memory
 */
mpd_malloc
struct mpd_async *
mpd_async_new(int fd);

/**
 * Closes the socket and frees memory.
 */
void
mpd_async_free(struct mpd_async *async);

/**
 * After an error has occurred, this function returns the error code.
 * If no error has occurred, it returns #MPD_ERROR_SUCCESS.
 */
mpd_pure
enum mpd_error
mpd_async_get_error(const struct mpd_async *async);

/**
 * If mpd_async_get_error() returns an error code other than #MPD_ERROR_SUCCESS,
 * this function returns the human readable error message which caused this.
 * This message is optional, and may be NULL.  The pointer is invalidated by
 * mpd_async_free().
 *
 * For #MPD_ERROR_SERVER, the error message is encoded in UTF-8.
 * #MPD_ERROR_SYSTEM obtains its error message from the operating
 * system, and thus the locale's character set (and probably language)
 * is used.  Keep that in mind when you print error messages.
 */
mpd_pure
const char *
mpd_async_get_error_message(const struct mpd_async *async);

/**
 * Returns the error code from the operating system; on most operating
 * systems, this is the errno value.  Calling this function is only
 * valid if mpd_async_get_error() returned #MPD_ERROR_SYSTEM.
 *
 * May be 0 if the operating system did not specify an error code.
 */
mpd_pure
int
mpd_async_get_system_error(const struct mpd_async *async);

/**
 * Returns the file descriptor which should be polled by the caller.
 * Do not use the file descriptor for anything except polling!  The
 * file descriptor never changes during the lifetime of this
 * #mpd_async object.
 */
mpd_pure
int
mpd_async_get_fd(const struct mpd_async *async);

/**
 * Enables (or disables) TCP keepalives.
 *
 * Keepalives are enabled using the SO_KEEPALIVE socket option.  They may be
 * required for long-idled connections to persist on some networks that
 * would otherwise terminate inactive TCP sessions.
 *
 * The default value is false.
 *
 * @param async the #mpd_async object
 * @param keepalive whether TCP keepalives should be enabled
 * @return true on success, false if setsockopt failed
 *
 * @since libmpdclient 2.10
 */
bool
mpd_async_set_keepalive(struct mpd_async *async,
			bool keepalive);

/**
 * Returns a bit mask of events which should be polled for.
 */
mpd_pure
enum mpd_async_event
mpd_async_events(const struct mpd_async *async);

/**
 * Call this function when poll() has returned events for this
 * object's file descriptor.  libmpdclient will attempt to perform I/O
 * operations.
 *
 * @return false if the connection was closed due to an error
 */
bool
mpd_async_io(struct mpd_async *async, enum mpd_async_event events);

/**
 * Appends a command to the output buffer.
 *
 * @param async the connection
 * @param command the command name, followed by arguments, terminated by
 * NULL
 * @param args the list of 'const char *' arguments
 * @return true on success, false if the buffer is full or an error has
 * previously occurred
 */
bool
mpd_async_send_command_v(struct mpd_async *async, const char *command,
			 va_list args);

/**
 * Appends a command to the output buffer.
 *
 * @param async the connection
 * @param command the command name, followed by arguments, terminated by
 * NULL. The arguments should be of type 'const char *'
 * @return true on success, false if the buffer is full or an error has
 * previously occurred
 */
mpd_sentinel
bool
mpd_async_send_command(struct mpd_async *async, const char *command, ...);

/**
 * Receives a line from the input buffer.  The result will be
 * null-terminated, without the newline character.  The pointer is
 * only valid until the next async function is called.
 *
 * You can use mpd_parser_new() and mpd_parser_feed() for parsing the line.
 *
 * @param async the connection
 * @return a line on success, NULL otherwise
 */
mpd_malloc
char *
mpd_async_recv_line(struct mpd_async *async);

/**
 * Copy raw data from the input buffer.  This can be used to receive
 * binary data from MPD, such as album art.
 *
 * @param async the connection
 * @param dest a buffer where this function will copy the data
 * @param length of bytes to consume
 * @return the number of bytes copied to the destination buffer (may
 * be 0 if the input buffer was empty)
 *
 * @since libmpdclient 2.17
 */
size_t
mpd_async_recv_raw(struct mpd_async *async, void *dest, size_t length);

#ifdef __cplusplus
}
#endif

#endif
