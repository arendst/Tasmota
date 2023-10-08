// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

static int
recursiveloop(char *pc, const char *sp, Subject *input, const char **subp, int nsubp)
{
	const char *old;
	int off;

	re1_5_stack_chk();

	for(;;) {
		if(inst_is_consumer(*pc)) {
			// If we need to match a character, but there's none left, it's fail
			if(sp >= input->end)
				return 0;
		}
		switch(*pc++) {
		case Char:
			if(*sp != *pc++)
				return 0;
		case Any:
			sp++;
			continue;
		case Class:
		case ClassNot:
			if (!_re1_5_classmatch(pc, sp))
				return 0;
			pc += *(unsigned char*)pc * 2 + 1;
			sp++;
			continue;
                case NamedClass:
			if (!_re1_5_namedclassmatch(pc, sp))
				return 0;
			pc++;
			sp++;
			continue;
		case Match:
			return 1;
		case Jmp:
			off = (signed char)*pc++;
			pc = pc + off;
			continue;
		case Split:
			off = (signed char)*pc++;
			if(recursiveloop(pc, sp, input, subp, nsubp))
				return 1;
			pc = pc + off;
			continue;
		case RSplit:
			off = (signed char)*pc++;
			if(recursiveloop(pc + off, sp, input, subp, nsubp))
				return 1;
			continue;
		case Save:
			off = (unsigned char)*pc++;
			if(off >= nsubp) {
				continue;
			}
			old = subp[off];
			subp[off] = sp;
			if(recursiveloop(pc, sp, input, subp, nsubp))
				return 1;
			subp[off] = old;
			return 0;
		case Bol:
			if(sp != input->begin)
				return 0;
			continue;
		case Eol:
			if(sp != input->end)
				return 0;
			continue;
		}
		re1_5_fatal("recursiveloop");
	}
}

int
re1_5_recursiveloopprog(ByteProg *prog, Subject *input, const char **subp, int nsubp, int is_anchored)
{
	return recursiveloop(HANDLE_ANCHORED(prog->insts, is_anchored), input->begin, input, subp, nsubp);
}
