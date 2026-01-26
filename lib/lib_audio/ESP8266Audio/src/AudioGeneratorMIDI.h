/*
  AudioGeneratorMIDI
  Audio output generator that plays MIDI files using a SF2 SoundFont
    
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

#ifndef _AUDIOGENERATORMIDI_H
#define _AUDIOGENERATORMIDI_H

#if defined(ESP32)
// Do not build, Espressif's GCC8+ has a compiler bug
#else // __GNUC__ == 8

#include "AudioGenerator.h"

#define TSF_NO_STDIO
#include "libtinysoundfont/tsf.h"

class AudioGeneratorMIDI : public AudioGenerator
{
  public:
    AudioGeneratorMIDI() { freq=44100; running = false; };
    virtual ~AudioGeneratorMIDI() override {};
    bool SetSoundfont(AudioFileSource *newsf2) {
      if (isRunning()) return false;
      sf2 = newsf2;
      MakeStreamFromAFS(sf2, &afsSF2);
      return true;
    }
    bool SetSampleRate(int newfreq) {
      if (isRunning()) return false;
      freq = newfreq;
      return true;
    }
    virtual bool begin(AudioFileSource *mid, AudioOutput *output) override;
    virtual bool loop() override;
    virtual bool stop() override;
    virtual bool isRunning() override { return running; };

  private:
    int freq;
    tsf *g_tsf;
    struct tsf_stream buffer;
    struct tsf_stream afsMIDI;
    struct tsf_stream afsSF2;
    AudioFileSource *sf2;
    AudioFileSource *midi;

  protected:
    struct midi_header {
      int8_t MThd[4];
      uint32_t header_size;
      uint16_t format_type;
      uint16_t number_of_tracks;
      uint16_t time_division;
    };

    struct track_header {
      int8_t MTrk[4];
      uint32_t track_size;
    };

    enum { MAX_TONEGENS = 32,         /* max tone generators: tones we can play simultaneously */
           MAX_TRACKS = 24
         };         /* max number of MIDI tracks we will process */

    int hdrptr;
    unsigned long buflen;
    int num_tracks;
    int tracks_done = 0;
    int num_tonegens = MAX_TONEGENS;
    int num_tonegens_used = 0;
    unsigned int ticks_per_beat = 240;
    unsigned long timenow = 0;
    unsigned long tempo;            /* current tempo in usec/qnote */
    // State needed for PlayMID()
    int notes_skipped = 0;
    int tracknum = 0;
    int earliest_tracknum = 0;
    unsigned long earliest_time = 0;

    struct tonegen_status {         /* current status of a tone generator */
      bool playing;                 /* is it playing? */
      char track;                   /* if so, which track is the note from? */
      char note;                    /* what note is playing? */
      char instrument;              /* what instrument? */
    } tonegen[MAX_TONEGENS];

    struct track_status {           /* current processing point of a MIDI track */
      int trkptr;                  /* ptr to the next note change */
      int trkend;                  /* ptr past the end of the track */
      unsigned long time;          /* what time we're at in the score */
      unsigned long tempo;         /* the tempo last set, in usec per qnote */
      unsigned int preferred_tonegen;      /* for strategy2, try to use this generator */
      unsigned char cmd;           /* CMD_xxxx next to do */
      unsigned char note;          /* for which note */
      unsigned char chan;          /* from which channel it was */
      unsigned char velocity;      /* the current volume */
      unsigned char last_event;    /* the last event, for MIDI's "running status" */
      bool tonegens[MAX_TONEGENS]; /* which tone generators our notes are playing on */
    } track[MAX_TRACKS];

    int midi_chan_instrument[16];   /* which instrument is currently being played on each channel */

    /* output bytestream commands, which are also stored in track_status.cmd */
    enum { CMD_PLAYNOTE   = 0x90,    /* play a note: low nibble is generator #, note is next byte */
           CMD_STOPNOTE   = 0x80,    /* stop a note: low nibble is generator # */
           CMD_INSTRUMENT = 0xc0,    /* change instrument; low nibble is generator #, instrument is next byte */
           CMD_RESTART    = 0xe0,    /* restart the score from the beginning */
           CMD_STOP       = 0xf0,    /* stop playing */
           CMD_TEMPO      = 0xFE,    /* tempo in usec per quarter note ("beat") */
           CMD_TRACKDONE  = 0xFF
         };   /* no more data left in this track */



    /* portable string length */
    int strlength (const char *str) {
      int i;
      for (i = 0; str[i] != '\0'; ++i);
      return i;
    }


    /* match a constant character sequence */

    int charcmp (const char *buf, const char *match) {
      int len, i;
      len = strlength (match);
      for (i = 0; i < len; ++i)
        if (buf[i] != match[i])
          return 0;
      return 1;
    }

    unsigned char buffer_byte (int offset);
    unsigned short buffer_short (int offset);
    unsigned int buffer_int32 (int offset);

    void midi_error (const char *msg, int curpos);
    void chk_bufdata (int ptr, unsigned long int len);
    uint16_t rev_short (uint16_t val);
    uint32_t rev_long (uint32_t val);
    void process_header (void);
    void start_track (int tracknum);

    unsigned long get_varlen (int *ptr);
    void find_note (int tracknum);
    void PrepareMIDI(AudioFileSource *src);
    int PlayMIDI();
    void StopMIDI();

    // tsf_stream <-> AudioFileSource
    static int afs_read(void *data, void *ptr, unsigned int size);
    static int afs_tell(void *data);
    static int afs_skip(void *data, unsigned int count);
    static int afs_seek(void *data, unsigned int pos);
    static int afs_close(void *data);
    static int afs_size(void *data);
    void MakeStreamFromAFS(AudioFileSource *src, tsf_stream *afs);

    int samplesToPlay;
    bool sawEOF;
    int numSamplesRendered;
    int sentSamplesRendered ;
    short samplesRendered[256];
};

#endif //__GNUC__ == 8

#endif

