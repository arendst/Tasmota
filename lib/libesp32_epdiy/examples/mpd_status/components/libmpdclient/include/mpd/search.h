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
 * Search songs in the database or the queue.
 *
 * Do not include this header directly.  Use mpd/client.h instead.
 */

#ifndef MPD_DB_H
#define MPD_DB_H

#include "connection.h"
#include "tag.h"
#include "compiler.h"

#include <stdbool.h>
#include <time.h>

/**
 * This type is not yet used, it is reserved for a future protocol
 * extension which will allow us to specify a comparison operator for
 * constraints.
 */
enum mpd_operator {
	/**
	 * The default search operator.  If "exact" was passed as
	 * "true", then it means "full string comparison"; if false,
	 * then it means "search for substring".
	 */
	MPD_OPERATOR_DEFAULT,
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Search for songs in the database.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the search command with mpd_search_commit(), and read the
 * response items with mpd_recv_song().
 *
 * @param connection the connection to MPD
 * @param exact if to match exact
 * @return true on success, false on error
 */
bool
mpd_search_db_songs(struct mpd_connection *connection, bool exact);

/**
 * Search for songs in the database and adds the result to the queue.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the search command with mpd_search_commit().
 *
 * @param connection the connection to MPD
 * @param exact if to match exact (only "true" supported by MPD 0.16)
 * @return true on success, false on error
 */
bool
mpd_search_add_db_songs(struct mpd_connection *connection, bool exact);

/**
 * Search for songs in the database and adds the result to a playlist.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the search command with mpd_search_commit().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 *
 * @since libmpdclient 2.17.
 */
bool
mpd_search_add_db_songs_to_playlist(struct mpd_connection *connection,
				    const char *playlist_name);

/**
 * Search for songs in the queue.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the search command with mpd_search_commit(), and read the
 * response items with mpd_recv_song().
 *
 * @param connection the connection to MPD
 * @param exact if to match exact
 * @return true on success, false on error
 */
bool
mpd_search_queue_songs(struct mpd_connection *connection, bool exact);

/**
 * Obtains a list of unique tag values from the database.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the search command with mpd_search_commit(), and read the
 * response items with mpd_recv_pair_tag().
 *
 * @param connection the connection to MPD
 * @param type The type of the tags to search for
 * @return true on success, false on error
 */
bool
mpd_search_db_tags(struct mpd_connection *connection, enum mpd_tag_type type);

/**
 * Gathers statistics on a set of songs in the database.
 * Constraints may be specified with mpd_search_add_tag_constraint().
 * Send the command with mpd_search_commit(), and read the response
 * with mpd_recv_stats().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool mpd_count_db_songs(struct mpd_connection *connection);

/**
 * Limit the search to a certain directory.
 *
 * @param connection a #mpd_connection
 * @param oper reserved, pass #MPD_OPERATOR_DEFAULT
 * @param value the URI relative to the music directory
 * @return true on success, false on error
 *
 * @since libmpdclient 2.9
 */
bool
mpd_search_add_base_constraint(struct mpd_connection *connection,
			       enum mpd_operator oper,
			       const char *value);

/**
 * Add a constraint on the song's URI.
 *
 * @param connection a #mpd_connection
 * @param oper reserved, pass #MPD_OPERATOR_DEFAULT
 * @param value The value of the constraint
 * @return true on success, false on error
 */
bool
mpd_search_add_uri_constraint(struct mpd_connection *connection,
			      enum mpd_operator oper,
			      const char *value);

/**
 * Add a constraint to a search limiting the value of a tag.
 *
 * @param connection a #mpd_connection
 * @param oper reserved, pass #MPD_OPERATOR_DEFAULT
 * @param type The tag type of the constraint
 * @param value The value of the constraint
 * @return true on success, false on error
 */
bool
mpd_search_add_tag_constraint(struct mpd_connection *connection,
			      enum mpd_operator oper,
			      enum mpd_tag_type type,
			      const char *value);

/**
 * Add a constraint to a search, search for a value in any tag.
 *
 * @param connection a #mpd_connection
 * @param oper reserved, pass #MPD_OPERATOR_DEFAULT
 * @param value The value of the constraint
 * @return true on success, false on error
 */
bool
mpd_search_add_any_tag_constraint(struct mpd_connection *connection,
				  enum mpd_operator oper,
				  const char *value);

/**
 * Limit the search to files modified after the given time stamp.
 *
 * @param connection a #mpd_connection
 * @param oper reserved, pass #MPD_OPERATOR_DEFAULT
 * @param value the reference time stamp
 * @return true on success, false on error
 *
 * @since libmpdclient 2.10
 */
bool
mpd_search_add_modified_since_constraint(struct mpd_connection *connection,
					 enum mpd_operator oper,
					 time_t value);

/**
 * Add an expression string.
 *
 * @param connection a #mpd_connection
 * @param expression the expression string; must be enclosed in
 * parentheses
 * @return true on success, false on error
 *
 * @since libmpdclient 2.15, MPD 0.21
 */
bool
mpd_search_add_expression(struct mpd_connection *connection,
			  const char *expression);

/**
 * Group the results by the specified tag.
 *
 * @param connection a #mpd_connection
 * @param type the tag type to group by
 * @return true on success, false on error
 *
 * @since libmpdclient 2.12, MPD 0.19
 */
bool
mpd_search_add_group_tag(struct mpd_connection *connection,
			 enum mpd_tag_type type);

/**
 * Sort the results by the specified named attribute.
 *
 * @param connection a #mpd_connection
 * @param name the attribute name to sort with; can be a tag name or
 * "Last-Modified"
 * @param descending sort in reverse order?
 * @return true on success, false on error
 *
 * @since MPD 0.21, libmpdclient 2.15
 */
bool
mpd_search_add_sort_name(struct mpd_connection *connection,
			 const char *name, bool descending);

/**
 * Sort the results by the specified tag.
 *
 * @param connection a #mpd_connection
 * @param type the tag type to sort with
 * @param descending sort in reverse order?
 * @return true on success, false on error
 *
 * @since MPD 0.21, libmpdclient 2.11; #descending since libmpdclient
 * 2.15
 */
bool
mpd_search_add_sort_tag(struct mpd_connection *connection,
			enum mpd_tag_type type, bool descending);

/**
 * Request only a portion of the result set.
 *
 * @param connection a #mpd_connection
 * @param start the start offset (including)
 * @param end the end offset (not including)
 * @return true on success, false on error
 *
 * @since libmpdclient 2.10
 */
bool
mpd_search_add_window(struct mpd_connection *connection,
		      unsigned start, unsigned end);

/**
 * Starts the real search with constraints added with
 * mpd_search_add_constraint().
 *
 * @param connection the connection to MPD
 * @return true on success, false on error
 */
bool
mpd_search_commit(struct mpd_connection *connection);

/**
 * Cancels the search request before you have called
 * mpd_search_commit().  Call this to clear the current search
 * request.
 *
 * @param connection the connection to MPD
 */
void
mpd_search_cancel(struct mpd_connection *connection);

/**
 * Same as mpd_recv_pair_named(), but the pair name is specified as
 * #mpd_tag_type.
 *
 * @param connection the connection to MPD
 * @param type the tag type you are looking for
 * @return a pair, or NULL on error or if there are no more matching
 * pairs in this response
 */
mpd_malloc
struct mpd_pair *
mpd_recv_pair_tag(struct mpd_connection *connection, enum mpd_tag_type type);

#ifdef __cplusplus
}
#endif

#endif
