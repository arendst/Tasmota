#include "unity_config.h"
#include <stdio.h>

void unityOutputStart() {}

void unityOutputChar(char c) {
    putchar(c);
}

void unityOutputFlush() {
    fflush(stdout);
}

void unityOutputComplete() {}