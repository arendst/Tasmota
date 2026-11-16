CXX=c++
LIBS=$(shell pkg-config sdl2 --libs) -lJPEGDEC
CXXFLAGS= -D__LINUX__ -Wall $(shell pkg-config sdl2 --cflags)

all: sdl2_viewer

sdl2_viewer: main.o
	${CXX} main.o $(LIBS) -o sdl2_viewer

main: main.cpp
	${CXX} ${CXXFLAGS} main.cpp

clean:
	rm -f main *.o sdl2_viewer
