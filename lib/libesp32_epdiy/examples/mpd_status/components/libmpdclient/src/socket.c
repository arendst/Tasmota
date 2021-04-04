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

#include "socket.h"
#include "fd_util.h"
#include "resolver.h"
#include "ierror.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <sys/un.h>
#  include <errno.h>
#  include <unistd.h>
#endif

#ifndef MSG_DONTWAIT
#  define MSG_DONTWAIT 0
#endif

#ifdef _WIN32

bool
mpd_socket_global_init(struct mpd_error_info *error)
{
	WSADATA wsaData;

	if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0 ||
			LOBYTE(wsaData.wVersion) != 2 ||
			HIBYTE(wsaData.wVersion) != 2 ) {
		mpd_error_system(error, 0);
		mpd_error_message(error,
				  "Could not find usable WinSock DLL");
		return false;
	}

	return true;
}

#endif

/**
 * Wait for the socket to become writable.
 */
static int
mpd_socket_wait_writable(unsigned fd, struct timeval *tv)
{
	fd_set fds;
	int ret;

	while (1) {
		FD_ZERO(&fds);
		FD_SET(fd, &fds);

		ret = select(fd + 1, NULL, &fds, NULL, tv);
		if (ret > 0)
			return 0;

		if (ret == 0 || !mpd_socket_ignore_errno(mpd_socket_errno()))
			return -1;
	}
}

/**
 * Wait until the socket is connected and check its result.  Returns 1
 * on success, 0 on timeout, -errno on error.
 */
static int
mpd_socket_wait_connected(mpd_socket_t fd, struct timeval *tv)
{
	int ret;
	int s_err = 0;
	socklen_t s_err_size = sizeof(s_err);

	ret = mpd_socket_wait_writable(fd, tv);
	if (ret < 0)
		return 0;

	ret = getsockopt(fd, SOL_SOCKET, SO_ERROR,
			 (char*)&s_err, &s_err_size);
	if (ret < 0)
		return -mpd_socket_errno();

	if (s_err != 0)
		return -s_err;

	return 1;
}

mpd_socket_t
mpd_socket_connect(const char *host, unsigned port, const struct timeval *tv0,
		   struct mpd_error_info *error)
{
	struct timeval tv = *tv0;
	struct resolver *resolver;
	const struct resolver_address *address;
	int ret;

	resolver = resolver_new(host, port);
	if (resolver == NULL) {
		mpd_error_code(error, MPD_ERROR_RESOLVER);
		mpd_error_message(error, "Failed to resolve host name");
		return -1;
	}

	assert(!mpd_error_is_defined(error));

	while ((address = resolver_next(resolver)) != NULL) {
		mpd_socket_t fd = socket_cloexec_nonblock(address->family,
							  SOCK_STREAM,
							  address->protocol);
		if (fd == MPD_INVALID_SOCKET) {
			mpd_error_clear(error);
			mpd_error_errno(error);
			continue;
		}

		ret = connect(fd, address->addr, address->addrlen);
		if (ret == 0) {
			resolver_free(resolver);
			mpd_error_clear(error);
			return fd;
		}

		if (!mpd_socket_ignore_errno(mpd_socket_errno())) {
			mpd_error_clear(error);
			mpd_error_errno(error);

			mpd_socket_close(fd);
			continue;
		}

		ret = mpd_socket_wait_connected(fd, &tv);
		if (ret > 0) {
			resolver_free(resolver);
			mpd_error_clear(error);
			return fd;
		}

		if (ret == 0) {
			mpd_error_clear(error);
			mpd_error_code(error, MPD_ERROR_TIMEOUT);
			mpd_error_message(error, "Timeout while connecting");
		} else if (ret < 0) {
			mpd_error_clear(error);
			mpd_error_system_message(error, -ret);
		}

		mpd_socket_close(fd);
	}

	resolver_free(resolver);
	return -1;
}

int
mpd_socket_close(mpd_socket_t fd)
{
#ifndef _WIN32
	return close(fd);
#else
	return closesocket(fd);
#endif
}

int
mpd_socket_keepalive(mpd_socket_t fd, bool keepalive)
{
	int keepalive_i = keepalive;


	return setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE,
			  (const char *) &keepalive_i, sizeof keepalive_i);
}
