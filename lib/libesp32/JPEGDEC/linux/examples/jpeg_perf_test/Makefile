CFLAGS=-ggdb -D__LINUX__ -Wall -O2 -I../../
LIBS=-g -lJPEGDEC 

all: jpeg_perf_test

jpeg_perf_test: main.o
	$(CXX) main.o $(LIBS) -o jpeg_perf_test

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o jpeg_perf_test
