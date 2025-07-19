// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

typedef struct Thread Thread;
struct Thread
{
	char *pc;
	const char *sp;
	Sub *sub;
};

static Thread
thread(char *pc, const char *sp, Sub *sub)
{
	Thread t = {pc, sp, sub};
	return t;
}

int
re1_5_backtrack(ByteProg *prog, Subject *input, const char **subp, int nsubp, int is_anchored)
{
	enum { MAX = 1000 };
	Thread ready[MAX];
	int i, nready;
	char *pc;
	const char *sp;
	Sub *sub;
	int off;

	/* queue initial thread */
	sub = newsub(nsubp);
	for(i=0; i<nsubp; i++)
		sub->sub[i] = nil;
	ready[0] = thread(HANDLE_ANCHORED(prog->insts, is_anchored), input->begin, sub);
	nready = 1;

	/* run threads in stack order */
	while(nready > 0) {
		--nready;	/* pop state for next thread to run */
		pc = ready[nready].pc;
		sp = ready[nready].sp;
		sub = ready[nready].sub;
		assert(sub->ref > 0);
		for(;;) {
			if(inst_is_consumer(*pc)) {
				// If we need to match a character, but there's none left, it's fail
				if(sp >= input->end)
					goto Dead;
			}
			switch(*pc++) {
			case Char:
				if(*sp != *pc++)
					goto Dead;
			case Any:
				sp++;
				continue;
			case Class:
			case ClassNot:
				if (!_re1_5_classmatch(pc, sp))
					goto Dead;
				pc += *(unsigned char*)pc * 2 + 1;
				sp++;
				continue;
			case NamedClass:
				if (!_re1_5_namedclassmatch(pc, sp))
					goto Dead;
				pc++;
				sp++;
				continue;
			case Match:
				for(i=0; i<nsubp; i++)
					subp[i] = sub->sub[i];
				decref(sub);
				return 1;
			case Jmp:
				off = (signed char)*pc++;
				pc = pc + off;
				continue;
			case Split:
				if(nready >= MAX)
					re1_5_fatal("backtrack overflow");
				off = (signed char)*pc++;
				ready[nready++] = thread(pc + off, sp, incref(sub));
//				pc = pc->x;	/* continue current thread */
				continue;
			case RSplit:
				if(nready >= MAX)
					re1_5_fatal("backtrack overflow");
				off = (signed char)*pc++;
				ready[nready++] = thread(pc, sp, incref(sub));
				pc = pc + off;
				continue;
			case Save:
				off = (unsigned char)*pc++;
				sub = update(sub, off, sp);
				continue;
			case Bol:
				if(sp != input->begin)
					goto Dead;
				continue;
			case Eol:
				if(sp != input->end)
					goto Dead;
				continue;
			default:
				re1_5_fatal("backtrack");
			}
		}
	Dead:
		decref(sub);
	}
	return 0;
}

