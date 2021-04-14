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

/*
 * This code is copied from MPD.  It is a subset of the original
 * library (we don't need pipes and regular files in libmpdclient).
 *
 */

#include "fd_util.h"

#include <assert.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#ifndef _WIN32

static int
fd_mask_flags(mpd_socket_t fd, int and_mask, int xor_mask)
{
	int ret;

	assert(fd != MPD_INVALID_SOCKET);

	ret = fcntl(fd, F_GETFD, 0);
	if (ret < 0)
		return ret;

	return fcntl(fd, F_SETFD, (ret & and_mask) ^ xor_mask);
}

#endif /* !_WIN32 */

static int
fd_set_cloexec(mpd_socket_t fd, bool enable)
{
#ifndef _WIN32
	return fd_mask_flags(fd, ~FD_CLOEXEC, enable ? FD_CLOEXEC : 0);
#else
	(void)fd;
	(void)enable;
	return 0;
#endif
}

/**
 * Enables non-blocking mode for the specified file descriptor.  On
 * WIN32, this function only works for sockets.
 */
static int
fd_set_nonblock(mpd_socket_t fd)
{
#ifdef _WIN32
	u_long val = 1;
	return ioctlsocket(fd, FIONBIO, &val);
#else
	int flags;

	assert(fd != MPD_INVALID_SOCKET);

	flags = fcntl(fd, F_GETFL);
	if (flags < 0)
		return flags;

	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
}

mpd_socket_t
socket_cloexec_nonblock(int domain, int type, int protocol)
{
	mpd_socket_t fd;

#if defined(SOCK_CLOEXEC) && defined(SOCK_NONBLOCK)
	fd = socket(domain, type | SOCK_CLOEXEC | SOCK_NONBLOCK, protocol);
	if (fd != MPD_INVALID_SOCKET || errno != EINVAL)
		return fd;
#endif

	fd = socket(domain, type, protocol);
	if (fd != MPD_INVALID_SOCKET) {
		fd_set_cloexec(fd, true);
		fd_set_nonblock(fd);
	}

	return fd;
}
