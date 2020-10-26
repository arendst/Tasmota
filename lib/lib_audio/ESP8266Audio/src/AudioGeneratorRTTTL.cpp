/*
  AudioGeneratorRTTTL
  Audio output generator that plays RTTTL (Nokia ringtone)

  Based on the Rtttl Arduino library by James BM, https://github.com/spicajames/Rtttl
  Based on the gist from Daniel Hall https://gist.github.com/smarthall/1618800
  
  Copyright (C) 2018  Earle F. Philhower, III

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "AudioGeneratorRTTTL.h"

AudioGeneratorRTTTL::AudioGeneratorRTTTL()
{
  running = false;
  file = NULL;
  output = NULL;
  rate = 22050;
  buff = nullptr;
  ptr = 0;
}

AudioGeneratorRTTTL::~AudioGeneratorRTTTL()
{
  free(buff);
}

bool AudioGeneratorRTTTL::stop()
{
  if (!running) return true;
  running = false;
  output->stop();
  return file->close();
}

bool AudioGeneratorRTTTL::isRunning()
{
  return running;
}

bool AudioGeneratorRTTTL::loop()
{
  if (!running) goto done; // Nothing to do here!

  // Load the next note, if we've hit the end of the last one
  if (samplesSent == ttlSamples) {
    if (!GetNextNote()) {
      running = false;
      goto done;
    }
    samplesSent = 0;
  }
  
  // Try and send out the remainder of the existing note, one per loop()
  if (ttlSamplesPerWaveFP10 == 0) { // Mute
    int16_t mute[2] = {0, 0};
    while ((samplesSent < ttlSamples) && output->ConsumeSample(mute)) {
      samplesSent++;
    }
  } else {
    while (samplesSent < ttlSamples) {
      int samplesSentFP10 = samplesSent << 10;
      int rem = samplesSentFP10 % ttlSamplesPerWaveFP10;
      int16_t val = (rem > ttlSamplesPerWaveFP10/2) ? 8192:-8192;
      int16_t s[2] = { val, val };
      if (!output->ConsumeSample(s)) goto done;
      samplesSent++;
    }
  }

done:
  file->loop();
  output->loop();

  return running;
}

bool AudioGeneratorRTTTL::SkipWhitespace()
{
  while ((ptr < len) && (buff[ptr] == ' ')) ptr++;
  return ptr < len;
}

bool AudioGeneratorRTTTL::ReadInt(int *dest)
{
  if (ptr >= len) return false;

  SkipWhitespace();
  if (ptr >= len) return false;
  if ((buff[ptr] <'0') || (buff[ptr] > '9')) return false;

  int t = 0;
  while ((buff[ptr] >= '0') && (buff[ptr] <='9')) {
    t = (t * 10) + (buff[ptr] - '0');
    ptr++;
  }
  *dest = t;
  return true;
}
  

bool AudioGeneratorRTTTL::ParseHeader()
{
  // Skip the title
  while ((ptr < len) && (buff[ptr] != ':')) ptr++;
  if (ptr >= len) return false;
  if (buff[ptr++] != ':') return false;
  if (!SkipWhitespace()) return false;
  if ((buff[ptr] != 'd') && (buff[ptr] != 'D')) return false;
  ptr++;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != '=') return false;
  if (!ReadInt(&defaultDuration)) return false;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != ',') return false;

  if (!SkipWhitespace()) return false;
  if ((buff[ptr] != 'o') && (buff[ptr] != 'O')) return false;
  ptr++;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != '=') return false;
  if (!ReadInt(&defaultOctave)) return false;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != ',') return false;

  int bpm;
  if (!SkipWhitespace()) return false;
  if ((buff[ptr] != 'b') && (buff[ptr] != 'B')) return false;
  ptr++;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != '=') return false;
  if (!ReadInt(&bpm)) return false;
  if (!SkipWhitespace()) return false;
  if (buff[ptr++] != ':') return false;

  wholeNoteMS = (60 * 1000 * 4) / bpm;

  return true;
}

#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
static int notes[49] = { 0,
  NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
  NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
  NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
  NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7 };

bool AudioGeneratorRTTTL::GetNextNote()
{
  int dur, note, scale;
  if (ptr >= len) return false;

  if (!ReadInt(&dur)) {
    dur = defaultDuration;
  }
  dur = wholeNoteMS / dur;

  if (ptr >= len) return false;
  note = 0;
  switch (buff[ptr++]) {
    case 'c': case 'C': note = 1; break;
    case 'd': case 'D': note = 3; break;
    case 'e': case 'E': note = 5; break;
    case 'f': case 'F': note = 6; break;
    case 'g': case 'G': note = 8; break;
    case 'a': case 'A': note = 10; break;
    case 'b': case 'B': note = 12; break;
    case 'p': case 'P': note = 0; break;
    default: return false;
  }
  if ((ptr < len) && (buff[ptr] == '#')) {
    ptr++;
    note++;
  }
  if ((ptr < len) && (buff[ptr] == '.')) {
    ptr++;
    dur += dur / 2;
  }
  if (!ReadInt(&scale)) {
    scale = defaultOctave;
  }
  // Eat any trailing whitespace and comma
  SkipWhitespace();
  if ((ptr < len) && (buff[ptr]==',')) {
    ptr++;
  }

  if (scale < 4) scale = 4;
  if (scale > 7) scale = 7;
  if (note) {
    int freq = notes[(scale - 4) * 12 + note];
    // Convert from frequency in Hz to high and low samples in fixed point
    ttlSamplesPerWaveFP10 = (rate << 10) / freq;
  } else {
    ttlSamplesPerWaveFP10 = 0;
  }
  ttlSamples = (rate * dur ) / 1000;

  //audioLogger->printf("%d %d %d %d %d\n", dur, note, scale, ttlSamplesPerWaveFP10, ttlSamples );

  return true;
}

bool AudioGeneratorRTTTL::begin(AudioFileSource *source, AudioOutput *output)
{
  if (!source) return false;
  file = source;
  if (!output) return false;
  this->output = output;
  if (!file->isOpen()) return false; // Error
  
  len = file->getSize();
  buff = (char *)malloc(len);
  if (!buff) return false;
  if (file->read(buff, len) != (uint32_t)len) return false;

  ptr = 0;
  samplesSent = 0;
  ttlSamples = 0;

  if (!ParseHeader()) return false;

  if (!output->SetRate( rate )) return false;
  if (!output->SetBitsPerSample( 16 )) return false;
  if (!output->SetChannels( 2 )) return false;
  if (!output->begin()) return false;

  running = true;
  
  return true;
}
