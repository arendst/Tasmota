// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

typedef struct Thread Thread;
struct Thread
{
	char *pc;
	Sub *sub;
};

typedef struct ThreadList ThreadList;
struct ThreadList
{
	int n;
	Thread t[1];
};

static Thread
thread(char *pc, Sub *sub)
{
	Thread t = {pc, sub};
	return t;
}

static ThreadList*
threadlist(int n)
{
	return mal(sizeof(ThreadList)+n*sizeof(Thread));
}

static void
addthread(ThreadList *l, Thread t, Subject *input, const char *sp)
{
	int off;
	if(*t.pc & 0x80) {
		decref(t.sub);
		return;	// already on list
	}
	*t.pc |= 0x80;
	
	switch(*t.pc & 0x7f) {
	default:
		l->t[l->n] = t;
		l->n++;
		break;
	case Jmp:
		off = (signed char)t.pc[1];
		t.pc += 2;
		addthread(l, thread(t.pc + off, t.sub), input, sp);
		break;
	case Split:
		off = (signed char)t.pc[1];
		t.pc += 2;
		addthread(l, thread(t.pc, incref(t.sub)), input, sp);
		addthread(l, thread(t.pc + off, t.sub), input, sp);
		break;
	case RSplit:
		off = (signed char)t.pc[1];
		t.pc += 2;
		addthread(l, thread(t.pc + off, incref(t.sub)), input, sp);
		addthread(l, thread(t.pc, t.sub), input, sp);
		break;
	case Save:
		off = (unsigned char)t.pc[1];
		t.pc += 2;
		addthread(l, thread(t.pc, update(t.sub, off, sp)), input, sp);
		break;
	case Bol:
		if(sp == input->begin)
			addthread(l, thread(t.pc + 1, t.sub), input, sp);
		break;
	case Eol:
		if(sp == input->end)
			addthread(l, thread(t.pc + 1, t.sub), input, sp);
		break;
	}
}

int
re1_5_pikevm(ByteProg *prog, Subject *input, const char **subp, int nsubp, int is_anchored)
{
	int i, len;
	ThreadList *clist, *nlist, *tmp;
	char *pc;
	const char *sp;
	Sub *sub, *matched;

	matched = nil;	
	for(i=0; i<nsubp; i++)
		subp[i] = nil;
	sub = newsub(nsubp);
	for(i=0; i<nsubp; i++)
		sub->sub[i] = nil;

	len = prog->len;
	clist = threadlist(len);
	nlist = threadlist(len);
	
	cleanmarks(prog);
	addthread(clist, thread(HANDLE_ANCHORED(prog->insts, is_anchored), sub), input, input->begin);
	matched = 0;
	for(sp=input->begin;; sp++) {
		if(clist->n == 0)
			break;
		// printf("%d(%02x).", (int)(sp - input->begin), *sp & 0xFF);
		cleanmarks(prog);
		for(i=0; i<clist->n; i++) {
			pc = clist->t[i].pc;
			sub = clist->t[i].sub;
			// printf(" %d", (int)(pc - prog->insts));
			if (inst_is_consumer(*pc & 0x7f)) {
				// If we need to match a character, but there's none left,
				// it's fail (we don't schedule current thread for continuation)
				if(sp >= input->end) {
					decref(sub);
					continue;
				}
			}
			switch(*pc++ & 0x7f) {
			case Char:
				if(*sp != *pc++) {
					decref(sub);
					break;
				}
			case Any:
			addthread:
				addthread(nlist, thread(pc, sub), input, sp+1);
				break;
			case Class:
			case ClassNot:
				if (!_re1_5_classmatch(pc, sp)) {
					decref(sub);
					break;
				}
				pc += *(unsigned char*)pc * 2 + 1;
				goto addthread;
			case NamedClass:
				if (!_re1_5_namedclassmatch(pc, sp)) {
					decref(sub);
					break;
				}
				pc++;
				goto addthread;
			case Match:
				if(matched)
					decref(matched);
				matched = sub;
				for(i++; i < clist->n; i++)
					decref(clist->t[i].sub);
				goto BreakFor;
			// Jmp, Split, Save handled in addthread, so that
			// machine execution matches what a backtracker would do.
			// This is discussed (but not shown as code) in
			// Regular Expression Matching: the Virtual Machine Approach.
			}
		}
	BreakFor:
		// printf("\n");
		tmp = clist;
		clist = nlist;
		nlist = tmp;
		nlist->n = 0;
		//if(*sp == '\0')
		//	break;
	}
	if(matched) {
		for(i=0; i<nsubp; i++)
			subp[i] = matched->sub[i];
		decref(matched);
		return 1;
	}
	return 0;
}
