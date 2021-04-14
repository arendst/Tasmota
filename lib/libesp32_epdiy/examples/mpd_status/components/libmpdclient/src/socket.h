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

#ifndef MPD_SOCKET_H
#define MPD_SOCKET_H

#include <mpd/socket.h>

#include <stdbool.h>

#ifdef _WIN32
#  include <winsock2.h>
#else
#  include <errno.h>
#endif

struct timeval;
struct mpd_error_info;

#ifdef _WIN32
bool
mpd_socket_global_init(struct mpd_error_info *error);
#else
static inline bool
mpd_socket_global_init(struct mpd_error_info *error)
{
	(void)error;
	return true;
}
#endif

static inline int
mpd_socket_errno(void)
{
#ifdef _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

/**
 * Can this error code be ignored?
 */
static inline bool
mpd_socket_ignore_errno(int e)
{
#ifdef _WIN32
	return e == WSAEINTR || e == WSAEINPROGRESS || e == WSAEWOULDBLOCK;
#else
	return e == EINTR || e == EINPROGRESS || e == EAGAIN;
#endif
}

/**
 * Connects the socket to the specified host and port.
 *
 * @return the socket file descriptor, or -1 on failure
 */
mpd_socket_t
mpd_socket_connect(const char *host, unsigned port, const struct timeval *tv,
		   struct mpd_error_info *error);

/**
 * Closes a socket descriptor.  This is a wrapper for close() or
 * closesocket(), depending on the OS.
 */
int
mpd_socket_close(mpd_socket_t fd);

/**
 * Sets (or unsets) keepalive on a socket descriptor.
 */
int
mpd_socket_keepalive(mpd_socket_t fd, bool keepalive);

#endif
