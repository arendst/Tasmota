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

#ifndef MPD_CONNECTION_H
#define MPD_CONNECTION_H

#include "protocol.h"
#include "error.h"
#include "compiler.h"

#include <stdbool.h>

struct mpd_async;

/**
 * \struct mpd_connection
 *
 * This opaque object represents a connection to a MPD server.  Call
 * mpd_connection_new() to create a new instance.  To free an
 * instance, call mpd_connection_free().
 *
 * Error handling: most functions return a "bool" indicating success
 * or failure.  In this case, you may query the nature of the error
 * with the functions mpd_connection_get_error(),
 * mpd_connection_get_error_message(),
 * mpd_connection_get_server_error().
 *
 * Some errors can be cleared by calling mpd_connection_clear_error(),
 * like #MPD_ERROR_SERVER, #MPD_ERROR_ARGUMENT.  Most others are
 * fatal, and cannot be recovered, like #MPD_ERROR_CLOSED -
 * mpd_connection_clear_error() returns false.
 *
 * Some functions like mpd_recv_pair() cannot differentiate between
 * "end of response" and "error".  If this function returns NULL, you
 * have to check mpd_connection_get_error().
 */
struct mpd_connection;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opens a new connection to a MPD server.  Both the name server
 * lookup and the connect() call are done synchronously.  After this
 * function has returned, you should check if the connection was
 * successful with mpd_connection_get_error().
 *
 * @param host the server's host name, IP address or Unix socket path.
 * If the resolver returns more than one IP address for a host name,
 * this functions tries all of them until one accepts the connection.
 * NULL is allowed here, which will connect to the default host
 * (using the MPD_HOST environment variable if present).
 * @param port the TCP port to connect to, 0 for default port (using
 * the MPD_PORT environment variable if present).  If "host" is a Unix
 * socket path, this parameter is ignored.
 * @param timeout_ms the timeout in milliseconds, 0 for the default
 * timeout (the environment variable MPD_TIMEOUT may specify a timeout
 * in seconds); you may modify it later with
 * mpd_connection_set_timeout()
 * @return a mpd_connection object (which may have failed to connect),
 * or NULL on out-of-memory
 *
 * @since libmpdclient 2.3 added support for #MPD_HOST, #MPD_PORT and
 * #MPD_TIMEOUT.
 */
mpd_malloc
struct mpd_connection *
mpd_connection_new(const char *host, unsigned port, unsigned timeout_ms);

/**
 * Creates a #mpd_connection object based on an existing asynchronous
 * MPD connection.  You should not continue to use the #mpd_async
 * object.  Note that mpd_connection_free() also frees your #mpd_async
 * object!
 *
 * This function does not block at all, which is why you have to pass
 * the welcome message to it.
 *
 * @param async a #mpd_async instance
 * @param welcome the first line sent by MPD (the welcome message)
 * @return a mpd_connection object, or NULL on out-of-memory
 */
mpd_malloc
struct mpd_connection *
mpd_connection_new_async(struct mpd_async *async, const char *welcome);

/**
 * Close the connection and free all memory.
 *
 * @param connection the connection to MPD
 */
void mpd_connection_free(struct mpd_connection *connection);

/**
 * Returns the settings which were used to connect to the server.  May
 * be NULL if the settings are not known.
 *
 * @since libmpdclient 2.4
 */
const struct mpd_settings *
mpd_connection_get_settings(const struct mpd_connection *connection);

/**
 * Enables (or disables) TCP keepalives.
 *
 * Keepalives are enabled using the SO_KEEPALIVE socket option.  They may be
 * required for long-idled connections to persist on some networks that
 * would otherwise terminate inactive TCP sessions.
 *
 * The default value is false.
 *
 * @param connection the connection to MPD
 * @param keepalive whether TCP keepalives should be enabled
 * @return true on success, false if setsockopt failed
 *
 * @since libmpdclient 2.10
 */
bool
mpd_connection_set_keepalive(struct mpd_connection *connection,
			     bool keepalive);

/**
 * Sets the timeout for synchronous operations.  If the MPD server
 * does not send a response during this time span, the operation is
 * aborted by libmpdclient.
 *
 * The initial value is the one passed to mpd_connection_new().  If
 * you have used mpd_connection_new_async(), then the default value is
 * 30 seconds.
 *
 * @param connection the connection to MPD
 * @param timeout_ms the desired timeout in milliseconds; must not be 0
 */
void mpd_connection_set_timeout(struct mpd_connection *connection,
				unsigned timeout_ms);

/**
 * Returns the file descriptor which should be polled by the caller.
 * Do not use the file descriptor for anything except polling!  The
 * file descriptor never changes during the lifetime of this
 * #mpd_connection object.
 */
mpd_pure
int
mpd_connection_get_fd(const struct mpd_connection *connection);

/**
 * Returns the underlying #mpd_async object.  This can be used to send
 * commands asynchronously.  During an asynchronous command, you must
 * not use synchronous #mpd_connection functions until the
 * asynchronous response has been finished.
 *
 * If an error occurs while using #mpd_async, you must close the
 * #mpd_connection.
 */
mpd_pure
struct mpd_async *
mpd_connection_get_async(struct mpd_connection *connection);

/**
 * Returns the libmpdclient error code.  MPD_ERROR_SUCCESS means no
 * error occurred.
 */
mpd_pure
enum mpd_error
mpd_connection_get_error(const struct mpd_connection *connection);

/**
 * Returns the human-readable (English) libmpdclient error message.
 * Calling this function is only valid if an error really occurred.
 * Check with mpd_connection_get_error().
 *
 * For #MPD_ERROR_SERVER, the error message is encoded in UTF-8.
 * #MPD_ERROR_SYSTEM obtains its error message from the operating
 * system, and thus the locale's character set (and probably language)
 * is used.  Keep that in mind when you print error messages.
 */
mpd_pure
const char *
mpd_connection_get_error_message(const struct mpd_connection *connection);

/**
 * Returns the error code returned from the server.  Calling this
 * function is only valid if mpd_connection_get_error() returned
 * #MPD_ERROR_SERVER.
 */
mpd_pure
enum mpd_server_error
mpd_connection_get_server_error(const struct mpd_connection *connection);

/**
 * Returns the location of the server error, i.e. an index within the
 * command list.  Calling this function is only valid in a command
 * list response, and if mpd_connection_get_error() returned
 * #MPD_ERROR_SERVER.
 *
 * @since libmpdclient 2.4
 */
mpd_pure
unsigned
mpd_connection_get_server_error_location(const struct mpd_connection *connection);

/**
 * Returns the error code from the operating system; on most operating
 * systems, this is the errno value.  Calling this function is only
 * valid if mpd_connection_get_error() returned #MPD_ERROR_SYSTEM.
 *
 * May be 0 if the operating system did not specify an error code.
 */
mpd_pure
int
mpd_connection_get_system_error(const struct mpd_connection *connection);

/**
 * Attempts to recover from an error condition.  This function must be
 * called after a non-fatal error before you can continue using this
 * object.
 *
 * @return true on success, false if the error is fatal and cannot be
 * recovered
 */
bool
mpd_connection_clear_error(struct mpd_connection *connection);

/**
 * Returns a three-tuple containing the major, minor and patch version
 * of the MPD protocol.
 */
mpd_pure
const unsigned *
mpd_connection_get_server_version(const struct mpd_connection *connection);

/**
 * Compares the MPD protocol version with the specified triple.
 *
 * @return -1 if the server is older, 1 if it is newer, 0 if it is
 * equal
 */
mpd_pure
int
mpd_connection_cmp_server_version(const struct mpd_connection *connection,
				  unsigned major, unsigned minor,
				  unsigned patch);

#ifdef __cplusplus
}
#endif

#endif
