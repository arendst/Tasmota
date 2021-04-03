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

#ifndef MPD_ISEND_H
#define MPD_ISEND_H

#include <stdbool.h>

struct mpd_connection;

/**
 * Sends a command without arguments to the server, but does not
 * update the "receiving" flag nor the "listOks" counter.  This is
 * used internally by the command_list functions.
 */
bool
mpd_send_command2(struct mpd_connection *connection, const char *command);

bool
mpd_send_int_command(struct mpd_connection *connection, const char *command,
		     int arg);

bool
mpd_send_int2_command(struct mpd_connection *connection, const char *command,
		      int arg1, int arg2);

bool
mpd_send_int3_command(struct mpd_connection *connection, const char *command,
		      int arg1, int arg2, int arg3);

bool
mpd_send_float_command(struct mpd_connection *connection, const char *command,
		       float arg);

bool
mpd_send_u_f_command(struct mpd_connection *connection, const char *command,
		     unsigned arg1, float arg2);

bool
mpd_send_u_s_command(struct mpd_connection *connection, const char *command,
		     unsigned arg1, const char *arg2);

bool
mpd_send_u_s_s_command(struct mpd_connection *connection, const char *command,
		       unsigned arg1, const char *arg2, const char *arg3);

bool
mpd_send_s_u_command(struct mpd_connection *connection, const char *command,
		     const char *arg1, unsigned arg2);

bool
mpd_send_range_command(struct mpd_connection *connection, const char *command,
		       unsigned arg1, unsigned arg2);

/**
 * Send command with one string argument followed by a range argument.
 */
bool
mpd_send_s_range_command(struct mpd_connection *connection,
			 const char *command, const char *arg1,
			 unsigned start, unsigned end);

/**
 * Send command with one integer argument followed by a range argument.
 */
bool
mpd_send_i_range_command(struct mpd_connection *connection,
			 const char *command, int arg1,
			 unsigned start, unsigned end);

bool
mpd_send_u_range_command(struct mpd_connection *connection,
			 const char *command, unsigned arg1,
			 unsigned start, unsigned end);

bool
mpd_send_range_u_command(struct mpd_connection *connection,
			 const char *command,
			 unsigned start, unsigned end, unsigned arg2);

bool
mpd_send_ll_command(struct mpd_connection *connection, const char *command,
		    long long arg);

/**
 * Sends all pending data from the output buffer to MPD.
 */
bool
mpd_flush(struct mpd_connection *connection);

#endif
