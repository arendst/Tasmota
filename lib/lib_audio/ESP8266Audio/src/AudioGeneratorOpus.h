/*
  AudioGeneratorOpus
  Audio output generator that plays Opus audio files
    
  Copyright (C) 2020  Earle F. Philhower, III

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

#ifndef _AUDIOGENERATOROPUS_H
#define _AUDIOGENERATOROPUS_H

#include <AudioGenerator.h>
//#include "libopus/opus.h"
#include "opusfile/opusfile.h"

class AudioGeneratorOpus : public AudioGenerator
{
  public:
    AudioGeneratorOpus();
    virtual ~AudioGeneratorOpus() override;
    virtual bool begin(AudioFileSource *source, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override;

  protected:
    // Opus callbacks, need static functions to bounce into C++ from C
    static int OPUS_read(void *_stream, unsigned char *_ptr, int _nbytes) {
      return static_cast<AudioGeneratorOpus*>(_stream)->read_cb(_ptr, _nbytes);
    }
    static int OPUS_seek(void *_stream, opus_int64 _offset, int _whence) {
      return static_cast<AudioGeneratorOpus*>(_stream)->seek_cb(_offset, _whence);
    }
    static opus_int64 OPUS_tell(void *_stream) {
      return static_cast<AudioGeneratorOpus*>(_stream)->tell_cb();
    }
    static int OPUS_close(void *_stream) {
      return static_cast<AudioGeneratorOpus*>(_stream)->close_cb();
    }

    // Actual Opus callbacks
    int read_cb(unsigned char *_ptr, int _nbytes);
    int seek_cb(opus_int64 _offset, int _whence);
    opus_int64 tell_cb();
    int close_cb();

  private:
    OpusFileCallbacks cb = {OPUS_read, OPUS_seek, OPUS_tell, OPUS_close};
    OggOpusFile *of;
    int prev_li; // To detect changes in streams

    int16_t *buff;
    uint32_t buffPtr;
    uint32_t buffLen;
};

#endif

