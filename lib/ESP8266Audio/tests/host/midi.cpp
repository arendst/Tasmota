#include <Arduino.h>
#include "AudioFileSourceSTDIO.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorMIDI.h"

#define SF2 "../../examples/PlayMIDIFromLittleFS/data/1mgm.sf2"
#define MIDI "../../examples/PlayMIDIFromLittleFS/data/furelise.mid"


int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;
    AudioFileSourceSTDIO *midifile = new AudioFileSourceSTDIO(MIDI);
    AudioFileSourceSTDIO *sf2file = new AudioFileSourceSTDIO(SF2);
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("midi.wav");
    AudioGeneratorMIDI *midi = new AudioGeneratorMIDI();

    midi->SetSoundfont(sf2file);
    midi->SetSampleRate(22050);

    midi->begin(midifile, out);
    while (midi->loop()) { /*noop*/ }
    midi->stop();

    delete out;
    delete midi;
    delete midifile;
    delete sf2file;
}
