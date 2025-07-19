// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

Sub *freesub;

Sub*
newsub(int n)
{
	Sub *s;
	
	s = freesub;
	if(s != nil)
		freesub = (Sub*)s->sub[0];
	else
		s = mal(sizeof *s);
	s->nsub = n;
	s->ref = 1;
	return s;
}

Sub*
incref(Sub *s)
{
	s->ref++;
	return s;
}

Sub*
update(Sub *s, int i, const char *p)
{
	Sub *s1;
	int j;

	if(s->ref > 1) {
		s1 = newsub(s->nsub);
		for(j=0; j<s->nsub; j++)
			s1->sub[j] = s->sub[j];
		s->ref--;
		s = s1;
	}
	s->sub[i] = p;
	return s;
}

void
decref(Sub *s)
{
	if(--s->ref == 0) {
		s->sub[0] = (char*)freesub;
		freesub = s;
	}
}
