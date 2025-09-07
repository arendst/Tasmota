CFLAGS=-c -Wall -O2 -ggdb -D__LINUX__ -I../src
LIBS = -lm -lpthread

all: libJPEGDEC.a

libJPEGDEC.a: JPEGDEC.o
	ar -rc libJPEGDEC.a JPEGDEC.o ;\
	sudo cp libJPEGDEC.a /usr/local/lib ;\
	sudo cp ../src/JPEGDEC.h /usr/local/include

JPEGDEC.o: ../src/JPEGDEC.cpp ../src/JPEGDEC.h ../src/jpeg.inl
	$(CXX) $(CFLAGS) ../src/JPEGDEC.cpp

clean:
	rm *.o libJPEGDEC.a
