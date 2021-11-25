// Copyright 2007-2009 Russ Cox.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "re1.5.h"

struct {
	char *name;
	int (*fn)(ByteProg*, Subject*, const char**, int, int);
} tab[] = {
	{"recursive", re1_5_recursiveprog},
	{"recursiveloop", re1_5_recursiveloopprog},
	{"backtrack", re1_5_backtrack},
	{"thompson", re1_5_thompsonvm},
	{"pike", re1_5_pikevm},
};

#ifdef DEBUG
int debug;
#endif
const char *re_engine;

void
usage(void)
{
	fprintf(stderr, "Usage: re [-hmd] [-e ENGINE] <regexp> <string>...\n"
	        "-h:        Print help message and exit\n"
	        "-m:        String is anchored\n"
	        "-e ENGINE: Specify one of: recursive recursiveloop backtrack thompson pike\n");
#ifdef DEBUG
	fprintf(stderr,
		"-d:        Print debug messages\n");
#endif
	exit(2);
}

int
main(int argc, char **argv)
{
	int i, j, k, l;
	int is_anchored = 0;

	argv++;
	argc--;
	while (argc > 0 && argv[0][0] == '-') {
		char *arg;
		for (arg = &argv[0][1]; *arg; arg++) {
			switch (*arg) {
				case 'h':
					usage();
					break;
				case 'm':
					is_anchored = 1;
					break;
#ifdef DEBUG
				case 'd':
					debug = 1;
					break;
#endif
				case 'e':
					if (argv[1] == NULL)
						re1_5_fatal("-e: Missing Regex engine argument");
					if (re_engine)
						re1_5_fatal("-e: Regex engine already specified");
					re_engine = argv[1];
					argv++;
					argc--;
					break;
				default:
					re1_5_fatal("Unknown flag");
			}
		}
		argv++;
		argc--;
	}

	if(argc < 2)
		usage();

#ifdef ODEBUG
	// Old and unmaintained code
	Regexp *re = parse(argv[0]);
	printre(re);
	printf("\n");

	Prog *prog = compile(re);
	printprog(prog);
	printf("=============\n");
#endif
	int sz = re1_5_sizecode(argv[0]);
#ifdef DEBUG
	if (debug) printf("Precalculated size: %d\n", sz);
#endif
	if (sz == -1) {
		re1_5_fatal("Error in regexp");
	}

	ByteProg *code = malloc(sizeof(ByteProg) + sz);
	int ret = re1_5_compilecode(code, argv[0]);
        if (ret != 0) {
		re1_5_fatal("Error in regexp");
	}

	int sub_els = (code->sub + 1) * 2;
#ifdef DEBUG
	if (debug) re1_5_dumpcode(code);
#endif
	const char *sub[sub_els];
	int engine_found = 0;
	for(i=1; i<argc; i++) {
		printf("#%d %s\n", i, argv[i]);
		for(j=0; j<nelem(tab); j++) {
			Subject subj = {argv[i], argv[i] + strlen(argv[i])};
			if (re_engine) {
				if (0 != strcmp(re_engine, tab[j].name))
					continue;
				engine_found = 1;
			}
			printf("%s ", tab[j].name);
			memset(sub, 0, sub_els * sizeof sub[0]);
			if(!tab[j].fn(code, &subj, sub, sub_els, is_anchored)) {
				printf("-no match-\n");
				continue;
			}
			printf("match");
			for(k=sub_els; k>0; k--)
				if(sub[k-1])
					break;
			for(l=0; l<k; l+=2) {
				printf(" (");
				if(sub[l] == nil)
					printf("?");
				else
					printf("%d", (int)(sub[l] - argv[i]));
				printf(",");
				if(sub[l+1] == nil)
					printf("?");
				else
					printf("%d", (int)(sub[l+1] - argv[i]));
				printf(")");
			}
			printf("\n");
		}
		if (re_engine && !engine_found)
			re1_5_fatal("-e: Unknown engine name");
	}

	free(code);
	return 0;
}
