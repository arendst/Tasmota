#include <Arduino.h>
#include "AudioFileSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorWAV.h"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioFileSourceSTDIO *in = new AudioFileSourceSTDIO("test_8u_16.wav");
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("pcm.wav");
    AudioGeneratorWAV *wav = new AudioGeneratorWAV();

    wav->begin(in, out);
    while (wav->loop()) { /*noop*/ }
    wav->stop();

    delete wav;
    delete out;
    delete in;
}
