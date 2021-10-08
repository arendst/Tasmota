/*
  AudioOutput
  Base class of an audio output player

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

#ifndef _AUDIOOUTPUTNULLSLOW_H
#define _AUDIOOUTPUTNULLSLOW_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>

#include "AudioOutput.h"

class AudioOutputNullSlow : public AudioOutput
{
  public:
    AudioOutputNullSlow() { };
    ~AudioOutputNullSlow() {};
    virtual bool begin() { samples = 0; startms = millis(); return true; }

    virtual bool ConsumeSample(int16_t sample[2]) {

      if (fd < 0) {
        fd = open("/dev/dsp", O_RDWR);
        if (fd < 0) {
          perror("open of /dev/dsp failed (Try with 'padsp this-exec')");
          exit(1);
        }
      }

      if (channels && lastchannels != channels) {
        Serial.printf("CHANNELS=%d\n", channels);
        int arg = channels;  /* mono or stereo */
        int status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
        if (status == -1) {
          perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
          exit(1);
        } else if (arg != channels) {
          perror("unable to set number of channels");
          exit(1);
        }
        lastchannels = channels;
      }

      if (lastchannels > 0 && hertz && lasthertz != hertz) {
        Serial.printf("FREQ=%d\n", hertz);
        int arg = hertz*4/lastchannels;      /* sampling rate */
        int status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
        if (status == -1) {
          perror("SOUND_PCM_WRITE_RATE ioctl failed");
          exit(1);
        }
        lasthertz = hertz;
      }

      if (bps && lastbps != bps) {
        Serial.printf("BPS=%d\n", bps);
        int arg = bps;      /* sample size */
        int status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
        if (status == -1) {
          perror("SOUND_PCM_WRITE_BITS ioctl failed");
          exit(1);
        } else if (arg != bps) {
          perror("unable to set sample size");
          exit(1);
        }
        lastbps = bps;
      }

      if ((++samples & ((1<<9)-1)) == 0) {
        // let the main loop a chance to run
        return false;
      }

      if (write(fd, sample, sizeof(sample)) != sizeof(sample)) {
        perror("doing sound");
        exit(1);
      }

      return true;
    }

    virtual bool stop() { endms = millis(); return true; };
    unsigned long GetMilliseconds() { return endms - startms; }
    int GetSamples() { return samples; }
    int GetFrequency() { return hertz; }

  protected:
    unsigned long startms;
    unsigned long endms;
    int samples;
    int lastchannels = -1;
    int lasthertz = -1;
    int lastbps = -1;
    int fd = -1;
};

#endif
