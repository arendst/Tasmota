#include <Arduino.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioOutputSTDIO.h"
#include "AudioGeneratorMOD.h"

#include "../../examples/PlayMODFromPROGMEMToDAC/enigma.h"

int main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    AudioFileSourcePROGMEM *file = new AudioFileSourcePROGMEM(enigma_mod, sizeof(enigma_mod));
    AudioOutputSTDIO *out = new AudioOutputSTDIO();
    out->SetFilename("mod.wav");
    AudioGeneratorMOD *mod = new AudioGeneratorMOD();

    mod->begin(file, out);
    // The MOD plays forever, so only run for ~30 seconds worth
    for (int i=0; i<10000; i++) mod->loop();
    mod->stop();

    delete out;
    delete mod;
    delete file;
}
