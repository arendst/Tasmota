
libmad=../../src/libmad/decoder.c ../../src/libmad/frame.c ../../src/libmad/bit.c ../../src/libmad/stream.c ../../src/libmad/fixed.c \
../../src/libmad/timer.c ../../src/libmad/layer3.c ../../src/libmad/synth.c ../../src/libmad/huffman.c ../../src/libmad/version.c
 

libhelix_mp3=../../src/libhelix-mp3/dequant.c ../../src/libhelix-mp3/mp3dec.c ../../src/libhelix-mp3/stproc.c \
../../src/libhelix-mp3/hufftabs.c ../../src/libhelix-mp3/dct32.c ../../src/libhelix-mp3/trigtabs.c \
../../src/libhelix-mp3/dqchan.c ../../src/libhelix-mp3/scalfact.c ../../src/libhelix-mp3/polyphase.c ../../src/libhelix-mp3/buffers.c \
../../src/libhelix-mp3/bitstream.c ../../src/libhelix-mp3/imdct.c ../../src/libhelix-mp3/subband.c ../../src/libhelix-mp3/huffman.c \
../../src/libhelix-mp3/mp3tabs.c

audiolib=../../src/AudioGeneratorWAV.cpp ../../src/AudioGeneratorMIDI.cpp ../../src/AudioFileSourceSTDIO.cpp ../../src/AudioOutputSTDIO.cpp \
../../src/AudioFileSourceID3.cpp ../../src/AudioGeneratorAAC.cpp ../../src/AudioGeneratorMP3.cpp ../../src/AudioOutputFilterDecimate.cpp \
../../src/AudioGeneratorFLAC.cpp ../../src/AudioGeneratorMOD.cpp ../../src/AudioFileSourceBuffer.cpp ../../src/AudioGeneratorMP3a.cpp \
Serial.cpp

libhelix_aac=../../src/libhelix-aac/decelmnt.c ../../src/libhelix-aac/dct4.c ../../src/libhelix-aac/dequant.c ../../src/libhelix-aac/sbrhuff.c \
../../src/libhelix-aac/sbrmath.c ../../src/libhelix-aac/aactabs.c ../../src/libhelix-aac/stproc.c ../../src/libhelix-aac/hufftabs.c \
../../src/libhelix-aac/sbrtabs.c ../../src/libhelix-aac/sbrfft.c ../../src/libhelix-aac/filefmt.c ../../src/libhelix-aac/tns.c \
../../src/libhelix-aac/trigtabs.c ../../src/libhelix-aac/fft.c ../../src/libhelix-aac/pns.c ../../src/libhelix-aac/sbrfreq.c \
../../src/libhelix-aac/sbrside.c ../../src/libhelix-aac/sbrhfadj.c ../../src/libhelix-aac/buffers.c ../../src/libhelix-aac/bitstream.c \
../../src/libhelix-aac/noiseless.c ../../src/libhelix-aac/imdct.c ../../src/libhelix-aac/aacdec.c ../../src/libhelix-aac/sbrhfgen.c \
../../src/libhelix-aac/sbrqmf.c ../../src/libhelix-aac/huffman.c ../../src/libhelix-aac/sbr.c ../../src/libhelix-aac/sbrimdct.c

libflac=../../src/libflac/md5.c ../../src/libflac/window.c ../../src/libflac/memory.c ../../src/libflac/cpu.c ../../src/libflac/fixed.c \
../../src/libflac/format.c ../../src/libflac/lpc.c ../../src/libflac/crc.c ../../src/libflac/bitreader.c ../../src/libflac/bitmath.c \
../../src/libflac/stream_decoder.c ../../src/libflac/float.c


CCOPTS=-g -Wunused-parameter -Wall -m32 -include Arduino.h
CPPOPTS=-g -Wunused-parameter -Wall -std=c++11 -m32 -include Arduino.h

.phony: all

all: mp3 aac wav midi

mp3: FORCE
	rm -f *.o
	gcc $(CCOPTS) -c $(libmad) -I ../../src/ -I.
	g++ $(CPPOPTS) -o mp3 mp3.cpp Serial.cpp *.o ../../src/AudioFileSourceSTDIO.cpp ../../src/AudioOutputSTDIO.cpp ../../src/AudioFileSourceID3.cpp ../../src/AudioFileSourceBuffer.cpp ../../src/AudioGeneratorMP3.cpp ../../src/AudioOutputMixer.cpp ../../src/AudioLogger.cpp  -I ../../src/ -I.
	rm -f *.o
	echo valgrind --leak-check=full --track-origins=yes -v --error-limit=no --show-leak-kinds=all ./mp3

aac: FORCE
	rm -f *.o
	gcc $(CCOPTS) -DUSE_DEFAULT_STDLIB -c $(libhelix_aac) -I ../../src/ -I.
	g++ $(CPPOPTS) -o aac aac.cpp Serial.cpp *.o ../../src/AudioFileSourceSTDIO.cpp ../../src/AudioOutputSTDIO.cpp ../../src/AudioFileSourceID3.cpp ../../src/AudioGeneratorAAC.cpp  ../../src/AudioLogger.cpp -I ../../src/ -I.
	rm -f *.o
	echo valgrind --leak-check=full --track-origins=yes -v --error-limit=no --show-leak-kinds=all ./aac

wav: FORCE
	rm -f *.o
	g++ $(CPPOPTS) -o wav wav.cpp Serial.cpp  ../../src/AudioFileSourceSTDIO.cpp ../../src/AudioOutputSTDIO.cpp ../../src/AudioGeneratorWAV.cpp   ../../src/AudioLogger.cpp -I ../../src/ -I.
	rm -f *.o
	echo valgrind --leak-check=full --track-origins=yes -v --error-limit=no --show-leak-kinds=all ./wav

midi: FORCE
	rm -f *.o
	g++ $(CPPOPTS) -o midi midi.cpp Serial.cpp  ../../src/AudioFileSourceSTDIO.cpp ../../src/AudioOutputSTDIO.cpp ../../src/AudioGeneratorMIDI.cpp   ../../src/AudioLogger.cpp -I ../../src/ -I.
	rm -f *.o
	echo valgrind --leak-check=full --track-origins=yes -v --error-limit=no --show-leak-kinds=all ./midi

clean:
	rm -f mp3 aac wav midi *.o

FORCE:
