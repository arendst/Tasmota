# Copyright 2007-2009 Russ Cox.  All Rights Reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

CC=gcc

CFLAGS=-g -Wall -Os
# Comment out when developing/testing
#CFLAGS=-DDEBUG -g -Wall -O0

TARGET=re
OFILES=\
	backtrack.o\
	compile.o\
	main.o\
	pike.o\
	recursive.o\
	recursiveloop.o\
	sub.o\
	thompson.o\
	compilecode.o\
	dumpcode.o\
	charclass.o\
	cleanmarks.o\
	util.o\
	y.tab.o\

HFILES=\
	re1.5.h\

$(TARGET): $(OFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OFILES)

%.o: %.c $(HFILES)
	$(CC) -c $(CFLAGS) $*.c

y.tab.h y.tab.c: parse.y
	bison -v -y parse.y

test: $(TARGET)
	./run-tests $(TFLAGS)

clean:
	rm -f *.o core $(TARGET) y.tab.[ch] y.output
