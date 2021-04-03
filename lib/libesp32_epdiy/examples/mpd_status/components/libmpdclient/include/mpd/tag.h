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

#ifndef LIBMPDCLIENT_TAG_H
#define LIBMPDCLIENT_TAG_H

/**
 * @since libmpdclient 2.10 added support for #MPD_TAG_MUSICBRAINZ_RELEASETRACKID.
 * @since libmpdclient 2.11 added support for #MPD_TAG_ARTIST_SORT and #MPD_TAG_ALBUM_ARTIST_SORT
 * @since libmpdclient 2.17 added support for #MPD_TAG_LABEL,
 *                                            #MPD_TAG_MUSICBRAINZ_WORKID,
 *                                            #MPD_TAG_GROUPING,
 *                                            #MPD_TAG_WORK,
 *                                            #MPD_TAG_CONDUCTOR.
 */
enum mpd_tag_type
{
	/**
	 * Special value returned by mpd_tag_name_parse() when an
	 * unknown name was passed.
	 */
	MPD_TAG_UNKNOWN = -1,

	MPD_TAG_ARTIST,
	MPD_TAG_ALBUM,
	MPD_TAG_ALBUM_ARTIST,
	MPD_TAG_TITLE,
	MPD_TAG_TRACK,
	MPD_TAG_NAME,
	MPD_TAG_GENRE,
	MPD_TAG_DATE,
	MPD_TAG_COMPOSER,
	MPD_TAG_PERFORMER,
	MPD_TAG_COMMENT,
	MPD_TAG_DISC,

	MPD_TAG_MUSICBRAINZ_ARTISTID,
	MPD_TAG_MUSICBRAINZ_ALBUMID,
	MPD_TAG_MUSICBRAINZ_ALBUMARTISTID,
	MPD_TAG_MUSICBRAINZ_TRACKID,
	MPD_TAG_MUSICBRAINZ_RELEASETRACKID,

	MPD_TAG_ORIGINAL_DATE,

	MPD_TAG_ARTIST_SORT,
	MPD_TAG_ALBUM_ARTIST_SORT,

	MPD_TAG_ALBUM_SORT,
	MPD_TAG_LABEL,
	MPD_TAG_MUSICBRAINZ_WORKID,

	MPD_TAG_GROUPING,
	MPD_TAG_WORK,
	MPD_TAG_CONDUCTOR,

	/* IMPORTANT: the ordering of tag types above must be
	   retained, or else the libmpdclient ABI breaks */

	MPD_TAG_COUNT
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Looks up the name of the specified tag.
 *
 * @return the name, or NULL if the tag type is not valid
 */
const char *
mpd_tag_name(enum mpd_tag_type type);

/**
 * Parses a tag name, and returns its #mpd_tag_type value.
 *
 * @return a #mpd_tag_type value, or MPD_TAG_UNKNOWN if the name was
 * not recognized
 */
enum mpd_tag_type
mpd_tag_name_parse(const char *name);

/**
 * Same as mpd_tag_name_parse(), but ignores case.
 *
 * @return a #mpd_tag_type value, or MPD_TAG_UNKNOWN if the name was
 * not recognized
 */
enum mpd_tag_type
mpd_tag_name_iparse(const char *name);

#ifdef __cplusplus
}
#endif

#endif
