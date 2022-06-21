#ifndef RFC2047_CPP
#define RFC2047_CPP

#include "RFC2047.h"

RFC2047_Decoder::RFC2047_Decoder() {}
RFC2047_Decoder::~RFC2047_Decoder() {}

void RFC2047_Decoder::rfc2047Decode(char *d, const char *s, size_t dlen)
{

  const char *p, *q;
  size_t n;
  int found_encoded = 0;

  dlen--; /* save room for the terminal nul */

  while (*s && dlen > 0)
  {
    if ((p = strstr (s, "=?")) == NULL ||
	(q = strchr (p + 2, '?')) == NULL ||
	(q = strchr (q + 1, '?')) == NULL ||
	(q = strstr (q + 1, "?=")) == NULL)
    {
      /* no encoded words */
      if (d != s)
	strfcpy (d, s, dlen + 1);
      return;
    }

    if (p != s)
    {
      n = (size_t) (p - s);
      /* ignore spaces between encoded words */
      if (!found_encoded || strspn (s, " \t\r\n") != n)
      {
	if (n > dlen)
	  n = dlen;
	if (d != s)
	  memcpy (d, s, n);
	d += n;
	dlen -= n;
      }
    }

    rfc2047DecodeWord (d, p, dlen);
    found_encoded = 1;
    s = q + 2;
    n = strlen (d);
    dlen -= n;
    d += n;
  }
  *d = 0;
}

void RFC2047_Decoder::rfc2047DecodeWord(char *d, const char *s, size_t dlen)
{

  char *p = safe_strdup (s);
  char *pp = p;
  char *pd = d;
  size_t len = dlen;
  int enc = 0, filter = 0, count = 0, c1, c2, c3, c4;

  while ((pp = strtok (pp, "?")) != NULL)
  {
    count++;
    switch (count)
    {
      case 2:
	if (strcasecmp (pp, Charset) != 0)
	{
	  filter = 1;
	}
	break;
      case 3:
	if (toupper (*pp) == 'Q')
	  enc = ENCQUOTEDPRINTABLE;
	else if (toupper (*pp) == 'B')
	  enc = ENCBASE64;
	else
	  return;
	break;
      case 4:
	if (enc == ENCQUOTEDPRINTABLE)
	{
	  while (*pp && len > 0)
	  {
	    if (*pp == '_')
	    {
	      *pd++ = ' ';
	      len--;
	    }
	    else if (*pp == '=')
	    {
	      *pd++ = (hexval(pp[1]) << 4) | hexval(pp[2]);
	      len--;
	      pp += 2;
	    }
	    else
	    {
	      *pd++ = *pp;
	      len--;
	    }
	    pp++;
	  }
	  *pd = 0;
	}
	else if (enc == ENCBASE64)
	{
	  while (*pp && len > 0)
	  {
	    c1 = base64val(pp[0]);
	    c2 = base64val(pp[1]);
	    *pd++ = (c1 << 2) | ((c2 >> 4) & 0x3);
	    if (--len == 0) break;
	    
	    if (pp[2] == '=') break;

	    c3 = base64val(pp[2]);
	    *pd++ = ((c2 & 0xf) << 4) | ((c3 >> 2) & 0xf);
	    if (--len == 0)
	      break;

	    if (pp[3] == '=')
	      break;

	    c4 = base64val(pp[3]);
	    *pd++ = ((c3 & 0x3) << 6) | c4;
	    if (--len == 0)
	      break;

	    pp += 4;
	  }
	  *pd = 0;
	}
	break;
    }
    pp = 0;
  }
  safe_free (&p);
  if (filter)
  {

      pd = d;
      while (*pd)
      {
	if (!IsPrint (*pd))
	  *pd = '?';
	pd++;
      }
  }
  return;
}

void *RFC2047_Decoder::safe_calloc(size_t nmemb, size_t size)
{
  void *p;

  if (!nmemb || !size)
    return NULL;
  if (!(p = calloc (nmemb, size)))
  {
    //out of memory
    return NULL;
  }
  return p;
}

void *RFC2047_Decoder::safe_malloc(unsigned int siz)
{
  void *p;

  if (siz == 0)
    return 0;
  if ((p = (void *) malloc (siz)) == 0)
  {
    //out of memory
    return NULL;
  }
  return (p);
}

void RFC2047_Decoder::safe_realloc(void **p, size_t siz)
{
  void *r;

  if (siz == 0)
  {
    if (*p)
    {
      free (*p);
      *p = NULL;
    }
    return;
  }

  if (*p)
    r = (void *) realloc (*p, siz);
  else
  {
    r = (void *) malloc (siz);
  }

  if (!r)
  {
    //out of memory
    return;
  }

  *p = r;
}

void RFC2047_Decoder::safe_free(void *ptr)
{ 
  void **p = (void **)ptr;
  if (*p)
  { 
    free (*p);
    *p = 0;
  }
}

char *RFC2047_Decoder::safe_strdup(const char *s)
{
  char *p;
  size_t l;

  if (!s || !*s) return 0;
  l = strlen (s) + 1;
  p = (char *)safe_malloc (l);
  memcpy (p, s, l);
  return (p);
}


#endif //RFC2047_CPP