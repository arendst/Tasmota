CFLAGS=-c -Wall -O2 -ggdb -I../src -D__LINUX__
LIBS = -lJPEGDEC -lbb_spi_lcd -lgpiod -lm -lpthread

all: showimg

showimg: showimg.o
	$(CC) showimg.o $(LIBS) -o showimg

showimg.o: showimg.cpp
	$(CXX) $(CFLAGS) showimg.cpp

clean:
	rm *.o showimg
