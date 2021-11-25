// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

void
re1_5_fatal(char *msg)
{
	fprintf(stderr, "fatal error: %s\n", msg);
	exit(2);
}

void*
mal(int n)
{
	void *v;
	
	v = malloc(n);
	if(v == nil)
		re1_5_fatal("out of memory");
	memset(v, 0, n);
	return v;
}	
