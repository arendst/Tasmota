CFLAGS=-c -Wall -O2 -I../src -D__LINUX__
LIBS = -lm -lpthread

all: jpegdec

jpegdec: main.o
	$(CC) main.o $(LIBS) -o jpegdec

main.o: main.c ../src/JPEGDEC.h makefile
	$(CC) $(CFLAGS) main.c

clean:
	rm *.o jpegdec
