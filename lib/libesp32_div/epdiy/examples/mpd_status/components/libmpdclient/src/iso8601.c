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

#include "iso8601.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
static inline struct tm *
gmtime_r(const time_t *timep, struct tm *result)
{
	(void)result;

	/* Windows does not have thread-safe functions.  That
	   sucks. */
	return gmtime(timep);
}
#endif /* _WIN32 */

/**
 * @return the current time zone offset in seconds
 */
static time_t
timezone_offset(void)
{
	const time_t t0 = 1234567890;
	time_t t = t0;
	struct tm tm_buffer, *tm;

	tm = gmtime_r(&t, &tm_buffer);
	if (tm == NULL)
		return 0;

	/* force the daylight saving time to be off; gmtime_r() should
	   have set this already */
	tm->tm_isdst = 0;

	t = mktime(tm);
	if (t == -1)
		return 0;

	return t0 - t;
}

/**
 * Unfortunately, the useful timegm() function is a GNU extension, and
 * mktime() returns a stamp relative to the current time zone.  This
 * function emulates timegm() by subtracting the time zone offset, see
 * timezone_offset().
 */
static time_t
timegm_emulation(struct tm *tm)
{
	time_t t = mktime(tm);
	if (t == -1)
		return 0;

	return t + timezone_offset();
}

time_t
iso8601_datetime_parse(const char *input)
{
	char *endptr;
	unsigned year, month, day, hour, minute, second;
	struct tm tm;

	year = strtoul(input, &endptr, 10);
	if (year < 1970 || year >= 3000 || *endptr != '-')
		/* beware of the Y3K problem! */
		return 0;

	input = endptr + 1;
	month = strtoul(input, &endptr, 10);
	if (month < 1 || month > 12 || *endptr != '-')
		return 0;

	input = endptr + 1;
	day = strtoul(input, &endptr, 10);
	if (day < 1 || day > 31 || *endptr != 'T')
		return 0;

	input = endptr + 1;
	hour = strtoul(input, &endptr, 10);
	if (endptr == input || hour >= 24 || *endptr != ':')
		return 0;

	input = endptr + 1;
	minute = strtoul(input, &endptr, 10);
	if (endptr == input || minute >= 60 || *endptr != ':')
		return 0;

	input = endptr + 1;
	second = strtoul(input, &endptr, 10);
	if (endptr == input || second >= 60 ||
	    (*endptr != 0 && *endptr != 'Z'))
		return 0;

	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;

	/* force the daylight saving time to be off, same as in
	   timezone_offset() */
	tm.tm_isdst = 0;

	return timegm_emulation(&tm);
}

bool
iso8601_datetime_format(char *buffer, size_t size, time_t t)
{
	struct tm tm_buffer, *tm;

	tm = gmtime_r(&t, &tm_buffer);
	if (tm == NULL)
		return false;

#ifdef _WIN32
	strftime(buffer, size, "%Y-%m-%dT%H:%M:%SZ", tm);
#else
	strftime(buffer, size, "%FT%TZ", tm);
#endif
	return true;
}
