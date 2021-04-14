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
 * Do not include this header directly.  Use mpd/client.h or
 * mpd/parser.h instead.
 */

#ifndef MPD_PROTOCOL_H
#define MPD_PROTOCOL_H

enum mpd_server_error {
	MPD_SERVER_ERROR_UNK = -1,

	MPD_SERVER_ERROR_NOT_LIST = 1,
	MPD_SERVER_ERROR_ARG = 2,
	MPD_SERVER_ERROR_PASSWORD = 3,
	MPD_SERVER_ERROR_PERMISSION = 4,
	MPD_SERVER_ERROR_UNKNOWN_CMD = 5,

	MPD_SERVER_ERROR_NO_EXIST = 50,
	MPD_SERVER_ERROR_PLAYLIST_MAX = 51,
	MPD_SERVER_ERROR_SYSTEM = 52,
	MPD_SERVER_ERROR_PLAYLIST_LOAD = 53,
	MPD_SERVER_ERROR_UPDATE_ALREADY = 54,
	MPD_SERVER_ERROR_PLAYER_SYNC = 55,
	MPD_SERVER_ERROR_EXIST = 56,
};

#endif
