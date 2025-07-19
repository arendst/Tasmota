CFLAGS=-D__LINUX__ -c -Wall -O2 
LIBS = 
CXX = g++

all: demo 

demo: main.o JPEGDEC.o
	$(CXX) main.o JPEGDEC.o $(LIBS) -o demo 

main.o: main.cpp
	$(CXX) $(CFLAGS) main.cpp

JPEGDEC.o: JPEGDEC.cpp
	$(CXX) $(CFLAGS) JPEGDEC.cpp

clean:
	rm -rf *.o demo
