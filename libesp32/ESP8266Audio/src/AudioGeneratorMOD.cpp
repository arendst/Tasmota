/*
  AudioGeneratorMOD
  Audio output generator that plays Amiga MOD tracker files
    
  Copyright (C) 2017  Earle F. Philhower, III

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
#define PGM_READ_UNALIGNED 0

#include "AudioGeneratorMOD.h"

/* 
   Ported/hacked out from STELLARPLAYER by Ronen K.
   http://mobile4dev.blogspot.com/2012/11/stellaris-launchpad-mod-player.html
   A version exists in GitHub at https://github.com/steveway/stellarplayer
   and also at https://github.com/MikesModz/StellarPlayer
   Both which were themselves a port of the PIC32 MOD player
   https://www.youtube.com/watch?v=i3Yl0TISQBE (seems to no longer be available.)

   Most changes involved reducing memory usage by changing data structures,
   moving constants to PROGMEM and minor tweaks to allow non pow2 buffer sizes.
*/

#pragma GCC optimize ("O3")

#define NOTE(r, c) (Player.currentPattern.note8[r][c]==NONOTE8?NONOTE:8*Player.currentPattern.note8[r][c])

#ifndef min
#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#endif

AudioGeneratorMOD::AudioGeneratorMOD()
{
  sampleRate = 44100;
  fatBufferSize = 6 * 1024;
  stereoSeparation = 32;
  mixerTick = 0;
  usePAL = false;
  UpdateAmiga();
  running = false;
  file = NULL;
  output = NULL;
}

AudioGeneratorMOD::~AudioGeneratorMOD()
{
  // Free any remaining buffers
  for (int i = 0; i < CHANNELS; i++) {
    FatBuffer.channels[i] = NULL;
  }
}

bool AudioGeneratorMOD::stop()
{
  // We may be stopping because of allocation failures, so always deallocate
  for (int i = 0; i < CHANNELS; i++) {
    free(FatBuffer.channels[i]);
    FatBuffer.channels[i] = NULL;
  }
  if (file) file->close();
  running = false;
  output->stop();
  return true;
}

bool AudioGeneratorMOD::loop()
{
  if (!running) goto done; // Easy-peasy

  // First, try and push in the stored sample.  If we can't, then punt and try later
  if (!output->ConsumeSample(lastSample)) goto done; // FIFO full, wait...

  // Now advance enough times to fill the i2s buffer
  do {
    if (mixerTick == 0) {
      running = RunPlayer();
      if (!running) {
        stop();
        goto done;
      }
      mixerTick = Player.samplesPerTick;
    }
    GetSample( lastSample );
    mixerTick--;
  } while (output->ConsumeSample(lastSample));

done:
  file->loop();
  output->loop();

  // We'll be left with one sample still in our buffer because it couldn't fit in the FIFO
  return running;
}

bool AudioGeneratorMOD::begin(AudioFileSource *source, AudioOutput *out)
{
  if (running) stop();
  
  if (!source) return false;
  file = source;
  if (!out) return false;
  output = out;
  
  if (!file->isOpen()) return false; // Can't read the file!

  // Set the output values properly
  if (!output->SetRate(sampleRate)) return false;
  if (!output->SetBitsPerSample(16)) return false;
  if (!output->SetChannels(2)) return false;
  if (!output->begin()) return false;

  UpdateAmiga();

  for (int i = 0; i < CHANNELS; i++) {
    FatBuffer.channels[i] = reinterpret_cast<uint8_t*>(malloc(fatBufferSize));
    if (!FatBuffer.channels[i]) {
      stop();
      return false;
    }
  }
  if (!LoadMOD()) {
    stop();
    return false;
  }
  running = true;
  return true;
}

// Sorted Amiga periods
static const uint16_t amigaPeriods[296] PROGMEM = {
  907, 900, 894, 887, 881, 875, 868, 862, //  -8 to -1
  856, 850, 844, 838, 832, 826, 820, 814, // C-1 to +7
  808, 802, 796, 791, 785, 779, 774, 768, // C#1 to +7
  762, 757, 752, 746, 741, 736, 730, 725, // D-1 to +7
  720, 715, 709, 704, 699, 694, 689, 684, // D#1 to +7
  678, 675, 670, 665, 660, 655, 651, 646, // E-1 to +7
  640, 636, 632, 628, 623, 619, 614, 610, // F-1 to +7
  604, 601, 597, 592, 588, 584, 580, 575, // F#1 to +7
  570, 567, 563, 559, 555, 551, 547, 543, // G-1 to +7
  538, 535, 532, 528, 524, 520, 516, 513, // G#1 to +7
  508, 505, 502, 498, 494, 491, 487, 484, // A-1 to +7
  480, 477, 474, 470, 467, 463, 460, 457, // A#1 to +7
  453, 450, 447, 444, 441, 437, 434, 431, // B-1 to +7
  428, 425, 422, 419, 416, 413, 410, 407, // C-2 to +7
  404, 401, 398, 395, 392, 390, 387, 384, // C#2 to +7
  381, 379, 376, 373, 370, 368, 365, 363, // D-2 to +7
  360, 357, 355, 352, 350, 347, 345, 342, // D#2 to +7
  339, 337, 335, 332, 330, 328, 325, 323, // E-2 to +7
  320, 318, 316, 314, 312, 309, 307, 305, // F-2 to +7
  302, 300, 298, 296, 294, 292, 290, 288, // F#2 to +7
  285, 284, 282, 280, 278, 276, 274, 272, // G-2 to +7
  269, 268, 266, 264, 262, 260, 258, 256, // G#2 to +7
  254, 253, 251, 249, 247, 245, 244, 242, // A-2 to +7
  240, 238, 237, 235, 233, 232, 230, 228, // A#2 to +7
  226, 225, 223, 222, 220, 219, 217, 216, // B-2 to +7
  214, 212, 211, 209, 208, 206, 205, 203, // C-3 to +7
  202, 200, 199, 198, 196, 195, 193, 192, // C#3 to +7
  190, 189, 188, 187, 185, 184, 183, 181, // D-3 to +7
  180, 179, 177, 176, 175, 174, 172, 171, // D#3 to +7
  170, 169, 167, 166, 165, 164, 163, 161, // E-3 to +7
  160, 159, 158, 157, 156, 155, 154, 152, // F-3 to +7
  151, 150, 149, 148, 147, 146, 145, 144, // F#3 to +7
  143, 142, 141, 140, 139, 138, 137, 136, // G-3 to +7
  135, 134, 133, 132, 131, 130, 129, 128, // G#3 to +7
  127, 126, 125, 125, 123, 123, 122, 121, // A-3 to +7
  120, 119, 118, 118, 117, 116, 115, 114, // A#3 to +7
  113, 113, 112, 111, 110, 109, 109, 108  // B-3 to +7
};
#define ReadAmigaPeriods(a) (uint16_t)pgm_read_word(amigaPeriods + (a))

static const uint8_t sine[64] PROGMEM = {
  0,  24,  49,  74,  97, 120, 141, 161,
  180, 197, 212, 224, 235, 244, 250, 253,
  255, 253, 250, 244, 235, 224, 212, 197,
  180, 161, 141, 120,  97,  74,  49,  24
};
#define ReadSine(a) pgm_read_byte(sine + (a))


static inline uint16_t MakeWord(uint8_t h, uint8_t l) { return h << 8 | l; }

bool AudioGeneratorMOD::LoadHeader()
{
  uint8_t i;
  uint8_t temp[4];
  uint8_t junk[22];

  if (20 != file->read(/*Mod.name*/junk, 20)) return false; // Skip MOD name
  for (i = 0; i < SAMPLES; i++) {
    if (22 != file->read(junk /*Mod.samples[i].name*/, 22)) return false; // Skip sample name
    if (2 != file->read(temp, 2)) return false;
    Mod.samples[i].length = MakeWord(temp[0], temp[1]) * 2;
    if (1 != file->read(reinterpret_cast<uint8_t*>(&Mod.samples[i].fineTune), 1)) return false;
    if (Mod.samples[i].fineTune > 7) Mod.samples[i].fineTune -= 16;
    if (1 != file->read(&Mod.samples[i].volume, 1)) return false;
    if (2 != file->read(temp, 2)) return false;
    Mod.samples[i].loopBegin = MakeWord(temp[0], temp[1]) * 2;
    if (2 != file->read(temp, 2)) return false;
    Mod.samples[i].loopLength = MakeWord(temp[0], temp[1]) * 2;
    if (Mod.samples[i].loopBegin + Mod.samples[i].loopLength > Mod.samples[i].length)
      Mod.samples[i].loopLength = Mod.samples[i].length - Mod.samples[i].loopBegin;
  }

  if (1 != file->read(&Mod.songLength, 1)) return false;
  if (1 != file->read(temp, 1)) return false; // Discard this byte

  Mod.numberOfPatterns = 0;
  for (i = 0; i < 128; i++) {
    if (1 != file->read(&Mod.order[i], 1)) return false;
    if (Mod.order[i] > Mod.numberOfPatterns)
      Mod.numberOfPatterns = Mod.order[i];
  }
  Mod.numberOfPatterns++;

  // Offset 1080
  if (4 != file->read(temp, 4)) return false;;
  if (!strncmp(reinterpret_cast<const char*>(temp + 1), "CHN", 3))
    Mod.numberOfChannels = temp[0] - '0';
  else if (!strncmp(reinterpret_cast<const char*>(temp + 2), "CH", 2))
    Mod.numberOfChannels = (temp[0] - '0') * 10 + temp[1] - '0';
  else
    Mod.numberOfChannels = 4;

  return true;
}

void AudioGeneratorMOD::LoadSamples()
{
  uint8_t i;
  uint32_t fileOffset = 1084 + Mod.numberOfPatterns * ROWS * Mod.numberOfChannels * 4 - 1;

  for (i = 0; i < SAMPLES; i++) {

    if (Mod.samples[i].length) {
      Mixer.sampleBegin[i] = fileOffset;
      Mixer.sampleEnd[i] = fileOffset + Mod.samples[i].length;
      if (Mod.samples[i].loopLength > 2) {
        Mixer.sampleloopBegin[i] = fileOffset + Mod.samples[i].loopBegin;
        Mixer.sampleLoopLength[i] = Mod.samples[i].loopLength;
        Mixer.sampleLoopEnd[i] = Mixer.sampleloopBegin[i] + Mixer.sampleLoopLength[i];
      } else {
        Mixer.sampleloopBegin[i] = 0;
        Mixer.sampleLoopLength[i] = 0;
        Mixer.sampleLoopEnd[i] = 0;
      }
      fileOffset += Mod.samples[i].length;
    }

  }

}

bool AudioGeneratorMOD::LoadPattern(uint8_t pattern)
{
  uint8_t row;
  uint8_t channel;
  uint8_t i;
  uint8_t temp[4];
  uint16_t amigaPeriod;

  if (!file->seek(1084 + pattern * ROWS * Mod.numberOfChannels * 4, SEEK_SET)) return false;

  for (row = 0; row < ROWS; row++) {
    for (channel = 0; channel < Mod.numberOfChannels; channel++) {

      if (4 != file->read(temp, 4)) return false;

      Player.currentPattern.sampleNumber[row][channel] = (temp[0] & 0xF0) + (temp[2] >> 4);

      amigaPeriod = ((temp[0] & 0xF) << 8) + temp[1];
      //   Player.currentPattern.note[row][channel] = NONOTE;
      Player.currentPattern.note8[row][channel] = NONOTE8;
      for (i = 1; i < 37; i++)
        if (amigaPeriod > ReadAmigaPeriods(i * 8) - 3 &&
            amigaPeriod < ReadAmigaPeriods(i * 8) + 3)
          Player.currentPattern.note8[row][channel] = i;

      Player.currentPattern.effectNumber[row][channel] = temp[2] & 0xF;
      Player.currentPattern.effectParameter[row][channel] = temp[3];
    }
  }

  return true;
}

void AudioGeneratorMOD::Portamento(uint8_t channel)
{
  if (Player.lastAmigaPeriod[channel] < Player.portamentoNote[channel]) {
    Player.lastAmigaPeriod[channel] += Player.portamentoSpeed[channel];
    if (Player.lastAmigaPeriod[channel] > Player.portamentoNote[channel])
      Player.lastAmigaPeriod[channel] = Player.portamentoNote[channel];
  }
  if (Player.lastAmigaPeriod[channel] > Player.portamentoNote[channel]) {
    Player.lastAmigaPeriod[channel] -= Player.portamentoSpeed[channel];
    if (Player.lastAmigaPeriod[channel] < Player.portamentoNote[channel])
      Player.lastAmigaPeriod[channel] = Player.portamentoNote[channel];
  }
  Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];
}

void AudioGeneratorMOD::Vibrato(uint8_t channel)
{
  uint16_t delta;
  uint16_t temp;

  temp = Player.vibratoPos[channel] & 31;

  switch (Player.waveControl[channel] & 3) {
    case 0:
      delta = ReadSine(temp);
      break;
    case 1:
      temp <<= 3;
      if (Player.vibratoPos[channel] < 0)
        temp = 255 - temp;
      delta = temp;
      break;
    case 2:
      delta = 255;
      break;
    case 3:
      delta = rand() & 255;
      break;
  }

  delta *= Player.vibratoDepth[channel];
  delta >>= 7;

  if (Player.vibratoPos[channel] >= 0)
    Mixer.channelFrequency[channel] = Player.amiga / (Player.lastAmigaPeriod[channel] + delta);
  else
    Mixer.channelFrequency[channel] = Player.amiga / (Player.lastAmigaPeriod[channel] - delta);

  Player.vibratoPos[channel] += Player.vibratoSpeed[channel];
  if (Player.vibratoPos[channel] > 31) Player.vibratoPos[channel] -= 64;
}

void AudioGeneratorMOD::Tremolo(uint8_t channel)
{
  uint16_t delta;
  uint16_t temp;

  temp = Player.tremoloPos[channel] & 31;

  switch (Player.waveControl[channel] & 3) {
    case 0:
      delta = ReadSine(temp);
      break;
    case 1:
      temp <<= 3;
      if (Player.tremoloPos[channel] < 0)
        temp = 255 - temp;
      delta = temp;
      break;
    case 2:
      delta = 255;
      break;
    case 3:
      delta = rand() & 255;
      break;
  }

  delta *= Player.tremoloDepth[channel];
  delta >>= 6;

  if (Player.tremoloPos[channel] >= 0) {
    if (Player.volume[channel] + delta > 64) delta = 64 - Player.volume[channel];
    Mixer.channelVolume[channel] = Player.volume[channel] + delta;
  } else {
    if (Player.volume[channel] - delta < 0) delta = Player.volume[channel];
    Mixer.channelVolume[channel] = Player.volume[channel] - delta;
  }

  Player.tremoloPos[channel] += Player.tremoloSpeed[channel];
  if (Player.tremoloPos[channel] > 31) Player.tremoloPos[channel] -= 64;
}

bool AudioGeneratorMOD::ProcessRow()
{
  bool jumpFlag;
  bool breakFlag;
  uint8_t channel;
  uint8_t sampleNumber;
  uint16_t note;
  uint8_t effectNumber;
  uint8_t effectParameter;
  uint8_t effectParameterX;
  uint8_t effectParameterY;
  uint16_t sampleOffset;

  if (!running) return false;

  Player.lastRow = Player.row++;
  jumpFlag = false;
  breakFlag = false;
  for (channel = 0; channel < Mod.numberOfChannels; channel++) {

    sampleNumber = Player.currentPattern.sampleNumber[Player.lastRow][channel];
    note = NOTE(Player.lastRow, channel);
    effectNumber = Player.currentPattern.effectNumber[Player.lastRow][channel];
    effectParameter = Player.currentPattern.effectParameter[Player.lastRow][channel];
    effectParameterX = effectParameter >> 4;
    effectParameterY = effectParameter & 0xF;
    sampleOffset = 0;

    if (sampleNumber) {
      Player.lastSampleNumber[channel] = sampleNumber - 1;
      if (!(effectParameter == 0xE && effectParameterX == NOTEDELAY))
        Player.volume[channel] = Mod.samples[Player.lastSampleNumber[channel]].volume;
    }

    if (note != NONOTE) {
      Player.lastNote[channel] = note;
      Player.amigaPeriod[channel] = ReadAmigaPeriods(note + Mod.samples[Player.lastSampleNumber[channel]].fineTune);

      if (effectNumber != TONEPORTAMENTO && effectNumber != PORTAMENTOVOLUMESLIDE)
        Player.lastAmigaPeriod[channel] = Player.amigaPeriod[channel];

      if (!(Player.waveControl[channel] & 0x80)) Player.vibratoPos[channel] = 0;
      if (!(Player.waveControl[channel] & 0x08)) Player.tremoloPos[channel] = 0;
    }

    switch (effectNumber) {
      case TONEPORTAMENTO:
        if (effectParameter) Player.portamentoSpeed[channel] = effectParameter;
        Player.portamentoNote[channel] = Player.amigaPeriod[channel];
        note = NONOTE;
        break;

      case VIBRATO:
        if (effectParameterX) Player.vibratoSpeed[channel] = effectParameterX;
        if (effectParameterY) Player.vibratoDepth[channel] = effectParameterY;
        break;

      case PORTAMENTOVOLUMESLIDE:
        Player.portamentoNote[channel] = Player.amigaPeriod[channel];
        note = NONOTE;
        break;

      case TREMOLO:
        if (effectParameterX) Player.tremoloSpeed[channel] = effectParameterX;
        if (effectParameterY) Player.tremoloDepth[channel] = effectParameterY;
        break;

      case SETCHANNELPANNING:
        Mixer.channelPanning[channel] = effectParameter >> 1;
        break;

      case SETSAMPLEOFFSET:
        sampleOffset = effectParameter << 8;
        if (sampleOffset > Mod.samples[Player.lastSampleNumber[channel]].length)
          sampleOffset = Mod.samples[Player.lastSampleNumber[channel]].length;
        break;

      case JUMPTOORDER:
        Player.orderIndex = effectParameter;
        if (Player.orderIndex >= Mod.songLength)
          Player.orderIndex = 0;
        Player.row = 0;
        jumpFlag = true;
        break;

      case SETVOLUME:
        if (effectParameter > 64) Player.volume[channel] = 64;
        else Player.volume[channel] = effectParameter;
        break;

      case BREAKPATTERNTOROW:
        Player.row = effectParameterX * 10 + effectParameterY;
        if (Player.row >= ROWS)
          Player.row = 0;
        if (!jumpFlag && !breakFlag) {
          Player.orderIndex++;
          if (Player.orderIndex >= Mod.songLength)
            Player.orderIndex = 0;
        }
        breakFlag = true;
        break;

      case 0xE:
        switch (effectParameterX) {
          case FINEPORTAMENTOUP:
            Player.lastAmigaPeriod[channel] -= effectParameterY;
            break;

          case FINEPORTAMENTODOWN:
            Player.lastAmigaPeriod[channel] += effectParameterY;
            break;

          case SETVIBRATOWAVEFORM:
            Player.waveControl[channel] &= 0xF0;
            Player.waveControl[channel] |= effectParameterY;
            break;

          case SETFINETUNE:
            Mod.samples[Player.lastSampleNumber[channel]].fineTune = effectParameterY;
            if (Mod.samples[Player.lastSampleNumber[channel]].fineTune > 7)
              Mod.samples[Player.lastSampleNumber[channel]].fineTune -= 16;
            break;

          case PATTERNLOOP:
            if (effectParameterY) {
              if (Player.patternLoopCount[channel])
                Player.patternLoopCount[channel]--;
              else
                Player.patternLoopCount[channel] = effectParameterY;
              if (Player.patternLoopCount[channel])
                Player.row = Player.patternLoopRow[channel] - 1;
            } else
              Player.patternLoopRow[channel] = Player.row;
            break;

          case SETTREMOLOWAVEFORM:
            Player.waveControl[channel] &= 0xF;
            Player.waveControl[channel] |= effectParameterY << 4;
            break;

          case FINEVOLUMESLIDEUP:
            Player.volume[channel] += effectParameterY;
            if (Player.volume[channel] > 64) Player.volume[channel] = 64;
            break;

          case FINEVOLUMESLIDEDOWN:
            Player.volume[channel] -= effectParameterY;
            if (Player.volume[channel] < 0) Player.volume[channel] = 0;
            break;

          case NOTECUT:
            note = NONOTE;
            break;

          case PATTERNDELAY:
            Player.patternDelay = effectParameterY;
            break;

          case INVERTLOOP:

            break;
        }
        break;

      case SETSPEED:
        if (effectParameter < 0x20)
          Player.speed = effectParameter;
        else
          Player.samplesPerTick = sampleRate / (2 * effectParameter / 5);
        break;
    }

    if (note != NONOTE || (Player.lastAmigaPeriod[channel] &&
        effectNumber != VIBRATO && effectNumber != VIBRATOVOLUMESLIDE &&
        !(effectNumber == 0xE && effectParameterX == NOTEDELAY)))
      Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];

    if (note != NONOTE)
      Mixer.channelSampleOffset[channel] = sampleOffset << DIVIDER;

    if (sampleNumber)
      Mixer.channelSampleNumber[channel] = Player.lastSampleNumber[channel];

    if (effectNumber != TREMOLO)
      Mixer.channelVolume[channel] = Player.volume[channel];

  }
  return true;
}

bool AudioGeneratorMOD::ProcessTick()
{
  uint8_t channel;
  uint8_t sampleNumber;
  uint16_t note;
  uint8_t effectNumber;
  uint8_t effectParameter;
  uint8_t effectParameterX;
  uint8_t effectParameterY;
  uint16_t tempNote;

  if (!running) return false;

  for (channel = 0; channel < Mod.numberOfChannels; channel++) {

    if (Player.lastAmigaPeriod[channel]) {

      sampleNumber = Player.currentPattern.sampleNumber[Player.lastRow][channel];
      //   note = Player.currentPattern.note[Player.lastRow][channel];
      note = NOTE(Player.lastRow, channel);
      effectNumber = Player.currentPattern.effectNumber[Player.lastRow][channel];
      effectParameter = Player.currentPattern.effectParameter[Player.lastRow][channel];
      effectParameterX = effectParameter >> 4;
      effectParameterY = effectParameter & 0xF;

      switch (effectNumber) {
        case ARPEGGIO:
          if (effectParameter)
            switch (Player.tick % 3) {
              case 0:
                Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];
                break;
              case 1:
                tempNote = Player.lastNote[channel] + effectParameterX * 8 + Mod.samples[Player.lastSampleNumber[channel]].fineTune;
                if (tempNote < 296) Mixer.channelFrequency[channel] = Player.amiga / ReadAmigaPeriods(tempNote);
                break;
              case 2:
                tempNote = Player.lastNote[channel] + effectParameterY * 8 + Mod.samples[Player.lastSampleNumber[channel]].fineTune;
                if (tempNote < 296) Mixer.channelFrequency[channel] = Player.amiga / ReadAmigaPeriods(tempNote);
                break;
            }
          break;

        case PORTAMENTOUP:
          Player.lastAmigaPeriod[channel] -= effectParameter;
          if (Player.lastAmigaPeriod[channel] < 113) Player.lastAmigaPeriod[channel] = 113;
          Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];
          break;

        case PORTAMENTODOWN:
          Player.lastAmigaPeriod[channel] += effectParameter;
          if (Player.lastAmigaPeriod[channel] > 856) Player.lastAmigaPeriod[channel] = 856;
          Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];
          break;

        case TONEPORTAMENTO:
          Portamento(channel);
          break;

        case VIBRATO:
          Vibrato(channel);
          break;

        case PORTAMENTOVOLUMESLIDE:
          Portamento(channel);
          Player.volume[channel] += effectParameterX - effectParameterY;
          if (Player.volume[channel] < 0) Player.volume[channel] = 0;
          else if (Player.volume[channel] > 64) Player.volume[channel] = 64;
          Mixer.channelVolume[channel] = Player.volume[channel];
          break;

        case VIBRATOVOLUMESLIDE:
          Vibrato(channel);
          Player.volume[channel] += effectParameterX - effectParameterY;
          if (Player.volume[channel] < 0) Player.volume[channel] = 0;
          else if (Player.volume[channel] > 64) Player.volume[channel] = 64;
          Mixer.channelVolume[channel] = Player.volume[channel];
          break;

        case TREMOLO:
          Tremolo(channel);
          break;

        case VOLUMESLIDE:
          Player.volume[channel] += effectParameterX - effectParameterY;
          if (Player.volume[channel] < 0) Player.volume[channel] = 0;
          else if (Player.volume[channel] > 64) Player.volume[channel] = 64;
          Mixer.channelVolume[channel] = Player.volume[channel];
          break;

        case 0xE:
          switch (effectParameterX) {
            case RETRIGGERNOTE:
              if (!effectParameterY) break;
              if (!(Player.tick % effectParameterY)) {
                Mixer.channelSampleOffset[channel] = 0;
              }
              break;

            case NOTECUT:
              if (Player.tick == effectParameterY)
                Mixer.channelVolume[channel] = Player.volume[channel] = 0;
              break;

            case NOTEDELAY:
              if (Player.tick == effectParameterY) {
                if (sampleNumber) Player.volume[channel] = Mod.samples[Player.lastSampleNumber[channel]].volume;
                if (note != NONOTE) Mixer.channelSampleOffset[channel] = 0;
                Mixer.channelFrequency[channel] = Player.amiga / Player.lastAmigaPeriod[channel];
                Mixer.channelVolume[channel] = Player.volume[channel];
              }
              break;
          }
          break;
      }

    }

  }
  return true;
}

bool AudioGeneratorMOD::RunPlayer()
{
  if (!running) return false;

  if (Player.tick == Player.speed) {
    Player.tick = 0;

    if (Player.row == ROWS) {
      Player.orderIndex++;
      if (Player.orderIndex == Mod.songLength)
      {
        //Player.orderIndex = 0;
        // No loop, just say we're done!
        return false;
      }
      Player.row = 0;
    }

    if (Player.patternDelay) {
      Player.patternDelay--;
    } else {
      if (Player.orderIndex != Player.oldOrderIndex)
        if (!LoadPattern(Mod.order[Player.orderIndex])) return false;
      Player.oldOrderIndex = Player.orderIndex;
      if (!ProcessRow()) return false;
    }

  } else {
    if (!ProcessTick()) return false;
  }
  Player.tick++;
  return true;
}

void AudioGeneratorMOD::GetSample(int16_t sample[2])
{
  int16_t sumL;
  int16_t sumR;
  uint8_t channel;
  uint32_t samplePointer;
  int8_t current;
  int8_t next;
  int16_t out;

  if (!running) return;

  sumL = 0;
  sumR = 0;
  for (channel = 0; channel < Mod.numberOfChannels; channel++) {

    if (!Mixer.channelFrequency[channel] ||
        !Mod.samples[Mixer.channelSampleNumber[channel]].length) continue;

    Mixer.channelSampleOffset[channel] += Mixer.channelFrequency[channel];

    if (!Mixer.channelVolume[channel]) continue;

    samplePointer = Mixer.sampleBegin[Mixer.channelSampleNumber[channel]] +
                    (Mixer.channelSampleOffset[channel] >> DIVIDER);

    if (Mixer.sampleLoopLength[Mixer.channelSampleNumber[channel]]) {

      if (samplePointer >= Mixer.sampleLoopEnd[Mixer.channelSampleNumber[channel]]) {
        Mixer.channelSampleOffset[channel] -= Mixer.sampleLoopLength[Mixer.channelSampleNumber[channel]] << DIVIDER;
        samplePointer -= Mixer.sampleLoopLength[Mixer.channelSampleNumber[channel]];
      }

    } else {

      if (samplePointer >= Mixer.sampleEnd[Mixer.channelSampleNumber[channel]]) {
        Mixer.channelFrequency[channel] = 0;
        samplePointer = Mixer.sampleEnd[Mixer.channelSampleNumber[channel]];
      }

    }

    if (samplePointer < FatBuffer.samplePointer[channel] ||
        samplePointer >= FatBuffer.samplePointer[channel] + fatBufferSize - 1 ||
        Mixer.channelSampleNumber[channel] != FatBuffer.channelSampleNumber[channel]) {

      uint16_t toRead = Mixer.sampleEnd[Mixer.channelSampleNumber[channel]] - samplePointer + 1;
      if (toRead > fatBufferSize) toRead  = fatBufferSize;

      if (!file->seek(samplePointer, SEEK_SET)) {
        stop();
        return;
      }
      if (toRead != file->read(FatBuffer.channels[channel], toRead)) {
        stop();
        return;
      }

      FatBuffer.samplePointer[channel] = samplePointer;
      FatBuffer.channelSampleNumber[channel] = Mixer.channelSampleNumber[channel];
    }

    current = FatBuffer.channels[channel][(samplePointer - FatBuffer.samplePointer[channel]) /*& (FATBUFFERSIZE - 1)*/];
    next = FatBuffer.channels[channel][(samplePointer + 1 - FatBuffer.samplePointer[channel]) /*& (FATBUFFERSIZE - 1)*/];

    out = current;

    // Integer linear interpolation
    out += (next - current) * (Mixer.channelSampleOffset[channel] & ((1 << DIVIDER) - 1)) >> DIVIDER;

    // Upscale to BITDEPTH
    out <<= BITDEPTH - 8;

    // Channel volume
    out = out * Mixer.channelVolume[channel] >> 6;

    // Channel panning
    sumL += out * min(128 - Mixer.channelPanning[channel], 64) >> 6;
    sumR += out * min(Mixer.channelPanning[channel], 64) >> 6;
  }

  // Downscale to BITDEPTH
  sumL /= Mod.numberOfChannels;
  sumR /= Mod.numberOfChannels;

  // Fill the sound buffer with unsigned values
  sample[AudioOutput::LEFTCHANNEL] = sumL + (1 << (BITDEPTH - 1));
  sample[AudioOutput::RIGHTCHANNEL] = sumR + (1 << (BITDEPTH - 1));
}

bool AudioGeneratorMOD::LoadMOD()
{
  uint8_t channel;

  if (!LoadHeader()) return false;
  LoadSamples();

  Player.amiga = AMIGA;
  Player.samplesPerTick = sampleRate / (2 * 125 / 5); // Hz = 2 * BPM / 5
  Player.speed = 6;
  Player.tick = Player.speed;
  Player.row = 0;

  Player.orderIndex = 0;
  Player.oldOrderIndex = 0xFF;
  Player.patternDelay = 0;

  for (channel = 0; channel < Mod.numberOfChannels; channel++) {
    Player.patternLoopCount[channel] = 0;
    Player.patternLoopRow[channel] = 0;

    Player.lastAmigaPeriod[channel] = 0;

    Player.waveControl[channel] = 0;

    Player.vibratoSpeed[channel] = 0;
    Player.vibratoDepth[channel] = 0;
    Player.vibratoPos[channel] = 0;

    Player.tremoloSpeed[channel] = 0;
    Player.tremoloDepth[channel] = 0;
    Player.tremoloPos[channel] = 0;

    FatBuffer.samplePointer[channel] = 0;
    FatBuffer.channelSampleNumber[channel] = 0xFF;

    Mixer.channelSampleOffset[channel] = 0;
    Mixer.channelFrequency[channel] = 0;
    Mixer.channelVolume[channel] = 0;
    switch (channel % 4) {
      case 0:
      case 3:
        Mixer.channelPanning[channel] = stereoSeparation;
        break;
      default:
        Mixer.channelPanning[channel] = 128 - stereoSeparation;
    }
  }
  return true;
}

