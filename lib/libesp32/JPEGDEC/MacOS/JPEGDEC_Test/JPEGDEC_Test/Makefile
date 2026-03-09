CFLAGS=-D__LINUX__ -Wall -O2 
LIBS = 

all: jpegtest

jpegtest: main.o
	$(CC) main.o $(LIBS) -o jpegtest 

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp

clean:
	rm -rf *.o jpegtest
