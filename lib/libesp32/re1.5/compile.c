// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
#ifdef DEBUG

#include "re1.5.h"

static Inst *pc;
static int count(Regexp*);
static void emit(Regexp*);

Prog*
compile(Regexp *r)
{
	int n;
	Prog *p;

	n = count(r) + 1;
	p = mal(sizeof *p + n*sizeof p->start[0]);
	p->start = (Inst*)(p+1);
	pc = p->start;
	emit(r);
	pc->opcode = Match;
	pc++;
	p->len = pc - p->start;
	return p;
}

// how many instructions does r need?
static int
count(Regexp *r)
{
	switch(r->type) {
	default:
		re1_5_fatal("bad count");
	case Alt:
		return 2 + count(r->left) + count(r->right);
	case Cat:
		return count(r->left) + count(r->right);
	case Lit:
	case Dot:
		return 1;
	case Paren:
		return 2 + count(r->left);
	case Quest:
		return 1 + count(r->left);
	case Star:
		return 2 + count(r->left);
	case Plus:
		return 1 +  count(r->left);
	}
}

static void
emit(Regexp *r)
{
	Inst *p1, *p2, *t;

	switch(r->type) {
	default:
		re1_5_fatal("bad emit");

	case Alt:
		pc->opcode = Split;
		p1 = pc++;
		p1->x = pc;
		emit(r->left);
		pc->opcode = Jmp;
		p2 = pc++;
		p1->y = pc;
		emit(r->right);
		p2->x = pc;
		break;

	case Cat:
		emit(r->left);
		emit(r->right);
		break;
	
	case Lit:
		pc->opcode = Char;
		pc->c = r->ch;
		pc++;
		break;
	
	case Dot:
		pc++->opcode = Any;
		break;

	case Paren:
		pc->opcode = Save;
		pc->n = 2*r->n;
		pc++;
		emit(r->left);
		pc->opcode = Save;
		pc->n = 2*r->n + 1;
		pc++;
		break;
	
	case Quest:
		pc->opcode = Split;
		p1 = pc++;
		p1->x = pc;
		emit(r->left);
		p1->y = pc;
		if(r->n) {	// non-greedy
			t = p1->x;
			p1->x = p1->y;
			p1->y = t;
		}
		break;

	case Star:
		pc->opcode = Split;
		p1 = pc++;
		p1->x = pc;
		emit(r->left);
		pc->opcode = Jmp;
		pc->x = p1;
		pc++;
		p1->y = pc;
		if(r->n) {	// non-greedy
			t = p1->x;
			p1->x = p1->y;
			p1->y = t;
		}
		break;

	case Plus:
		p1 = pc;
		emit(r->left);
		pc->opcode = Split;
		pc->x = p1;
		p2 = pc;
		pc++;
		p2->y = pc;
		if(r->n) {	// non-greedy
			t = p2->x;
			p2->x = p2->y;
			p2->y = t;
		}
		break;
	}
}

void
printprog(Prog *p)
{
	Inst *pc, *e;
	
	pc = p->start;
	e = p->start + p->len;
	
	for(; pc < e; pc++) {
		switch(pc->opcode) {
		default:
			re1_5_fatal("printprog");
		case Split:
			printf("%2d. split %d, %d\n", (int)(pc-p->start), (int)(pc->x-p->start), (int)(pc->y-p->start));
			break;
		case Jmp:
			printf("%2d. jmp %d\n", (int)(pc-p->start), (int)(pc->x-p->start));
			break;
		case Char:
			printf("%2d. char %c\n", (int)(pc-p->start), pc->c);
			break;
		case Any:
			printf("%2d. any\n", (int)(pc-p->start));
			break;
		case Match:
			printf("%2d. match\n", (int)(pc-p->start));
			break;
		case Save:
			printf("%2d. save %d\n", (int)(pc-p->start), pc->n);
		}
	}
}

#endif //DEBUG
