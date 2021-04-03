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

#ifndef MPD_VERSION_H
#define MPD_VERSION_H

#define LIBMPDCLIENT_MAJOR_VERSION @MAJOR_VERSION@
#define LIBMPDCLIENT_MINOR_VERSION @MINOR_VERSION@
#define LIBMPDCLIENT_PATCH_VERSION @PATCH_VERSION@

/**
 * Preprocessor macro which allows you to check which version of
 * libmpdclient you are compiling with.  It can be used in
 * preprocessor directives.
 *
 * @return true if this libmpdclient version equals or is newer than
 * the specified version number
 * @since libmpdclient 2.1
 */
#define LIBMPDCLIENT_CHECK_VERSION(major, minor, patch) \
	((major) < LIBMPDCLIENT_MAJOR_VERSION || \
	 ((major) == LIBMPDCLIENT_MAJOR_VERSION && \
	  ((minor) < LIBMPDCLIENT_MINOR_VERSION || \
	   ((minor) == LIBMPDCLIENT_MINOR_VERSION && \
	    (patch) <= LIBMPDCLIENT_PATCH_VERSION))))

#endif
