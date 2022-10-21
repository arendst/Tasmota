// Lazy "include all the things" header for simplicity.
// In general a user should only include the specific headers they need
// to miniimize build times.

// Input stage
#include "AudioFileSourceBuffer.h"
#include "AudioFileSourceFATFS.h"
#include "AudioFileSourceFS.h"
#include "AudioFileSource.h"
#include "AudioFileSourceHTTPStream.h"
#include "AudioFileSourceICYStream.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourceLittleFS.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceSD.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceSPIRAMBuffer.h"
#include "AudioFileSourceSTDIO.h"

// Misc. plumbing
#include "AudioFileStream.h"
#include "AudioLogger.h"
#include "AudioStatus.h"

// Actual decode/audio generation logic
#include "AudioGeneratorAAC.h"
#include "AudioGeneratorFLAC.h"
#include "AudioGenerator.h"
#include "AudioGeneratorMIDI.h"
#include "AudioGeneratorMOD.h"
#include "AudioGeneratorMP3a.h"
#include "AudioGeneratorMP3.h"
#include "AudioGeneratorOpus.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioGeneratorTalkie.h"
#include "AudioGeneratorWAV.h"

// Render(output) sounds
#include "AudioOutputBuffer.h"
#include "AudioOutputFilterDecimate.h"
#include "AudioOutput.h"
#include "AudioOutputI2S.h"
#include "AudioOutputI2SNoDAC.h"
#include "AudioOutputMixer.h"
#include "AudioOutputNull.h"
#include "AudioOutputSerialWAV.h"
#include "AudioOutputSPDIF.h"
#include "AudioOutputSPIFFSWAV.h"
#include "AudioOutputSTDIO.h"
#include "AudioOutputULP.h"
