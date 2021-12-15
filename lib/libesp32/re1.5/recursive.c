// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

static int
recursive(char *pc, const char *sp, Subject *input, const char **subp, int nsubp)
{
	const char *old;
	int off;

	if(inst_is_consumer(*pc)) {
		// If we need to match a character, but there's none left, it's fail
		if(sp >= input->end)
			return 0;
	}

	re1_5_stack_chk();

	switch(*pc++) {
	case Char:
		if(*sp != *pc++)
			return 0;
	case Any:
		return recursive(pc, sp+1, input, subp, nsubp);
	case Class:
	case ClassNot:
		if (!_re1_5_classmatch(pc, sp))
			return 0;
		pc += *(unsigned char*)pc * 2 + 1;
		return recursive(pc, sp+1, input, subp, nsubp);
	case NamedClass:
		if (!_re1_5_namedclassmatch(pc, sp))
			return 0;
		return recursive(pc+1, sp+1, input, subp, nsubp);
	case Match:
		return 1;
	case Jmp:
		off = (signed char)*pc++;
		return recursive(pc + off, sp, input, subp, nsubp);
	case Split:
		off = (signed char)*pc++;
		if(recursive(pc, sp, input, subp, nsubp))
			return 1;
		return recursive(pc + off, sp, input, subp, nsubp);
	case RSplit:
		off = (signed char)*pc++;
		if(recursive(pc + off, sp, input, subp, nsubp))
			return 1;
		return recursive(pc, sp, input, subp, nsubp);
	case Save:
		off = (unsigned char)*pc++;
		if(off >= nsubp)
			return recursive(pc, sp, input, subp, nsubp);
		old = subp[off];
		subp[off] = sp;
		if(recursive(pc, sp, input, subp, nsubp))
			return 1;
		subp[off] = old;
		return 0;
	case Bol:
		if(sp != input->begin)
			return 0;
		return recursive(pc, sp, input, subp, nsubp);
	case Eol:
		if(sp != input->end)
			return 0;
		return recursive(pc, sp, input, subp, nsubp);
	}
	re1_5_fatal("recursive");
	return -1;
}

int
re1_5_recursiveprog(ByteProg *prog, Subject *input, const char **subp, int nsubp, int is_anchored)
{
	return recursive(HANDLE_ANCHORED(prog->insts, is_anchored), input->begin, input, subp, nsubp);
}
