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

/*
  The MIDI processing engine is a heavily modified version of MIDITONES,
  by Len Shustek, https://github.com/LenShustek/miditones .
  Whereas MIDITONES original simply parsed a file beforehand to a byte
  stream to be played by another program, this does the parsing and
  playback in real-time.

   Here's his original header/readme w/MIT license, which is subsumed by the
   GPL license of the ESP8266Audio project.
*/

/***************************************************************************

   MIDITONES: Convert a MIDI file into a simple bytestream of notes

  -------------------------------------------------------------------------
  The MIT License (MIT)
  Copyright (c) 2011,2013,2015,2016, Len Shustek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF, OR
  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************/


#include "AudioGeneratorMIDI.h"

#pragma GCC optimize ("O3")

#define TSF_NO_STDIO
#define TSF_IMPLEMENTATION
#include "libtinysoundfont/tsf.h"

/****************  utility routines  **********************/

/* announce a fatal MIDI file format error */

void AudioGeneratorMIDI::midi_error(const char *msg, int curpos)
{
  cb.st(curpos, msg);
#if 0
  int ptr;
  audioLogger->printf("---> MIDI file error at position %04X (%d): %s\n", (uint16_t) curpos, (uint16_t) curpos, msg);
  /* print some bytes surrounding the error */
  ptr = curpos - 16;
  if (ptr < 0) ptr = 0;
  buffer.seek( buffer.data, ptr );
  for (int i = 0; i < 32; i++) {
    char c;
    buffer.read (buffer.data, &c, 1);
    audioLogger->printf((ptr + i) == curpos ? " [%02X]  " : "%02X ", (int) c & 0xff);
  }
  audioLogger->printf("\n");
#endif
  running = false;
}

/* check that we have a specified number of bytes left in the buffer */

void AudioGeneratorMIDI::chk_bufdata (int ptr, unsigned long int len) {
  if ((unsigned) (ptr + len) > buflen)
    midi_error ("data missing", ptr);
}

/* fetch big-endian numbers */

uint16_t AudioGeneratorMIDI::rev_short (uint16_t val) {
  return ((val & 0xff) << 8) | ((val >> 8) & 0xff);
}

uint32_t AudioGeneratorMIDI::rev_long (uint32_t val) {
  return (((rev_short ((uint16_t) val) & 0xffff) << 16) |
          (rev_short ((uint16_t) (val >> 16)) & 0xffff));
}

/**************  process the MIDI file header  *****************/

void AudioGeneratorMIDI::process_header (void) {
  struct midi_header hdr;
  unsigned int time_division;

  chk_bufdata (hdrptr, sizeof (struct midi_header));
  buffer.seek (buffer.data, hdrptr);
  buffer.read (buffer.data, &hdr, sizeof (hdr));
  if (!charcmp ((char *) hdr.MThd, "MThd"))
    midi_error ("Missing 'MThd'", hdrptr);
  num_tracks = rev_short (hdr.number_of_tracks);
  time_division = rev_short (hdr.time_division);
  if (time_division < 0x8000)
    ticks_per_beat = time_division;
  else
    ticks_per_beat = ((time_division >> 8) & 0x7f) /* SMTE frames/sec */ *(time_division & 0xff);     /* ticks/SMTE frame */
  hdrptr += rev_long (hdr.header_size) + 8;    /* point past header to track header, presumably. */
  return;
}


/****************  Process a MIDI track header *******************/

void AudioGeneratorMIDI::start_track (int tracknum) {
  struct track_header hdr;
  unsigned long tracklen;

  chk_bufdata (hdrptr, sizeof (struct track_header));
  buffer.seek (buffer.data, hdrptr);
  buffer.read (buffer.data, &hdr, sizeof (hdr));
  if (!charcmp ((char *) (hdr.MTrk), "MTrk"))
    midi_error ("Missing 'MTrk'", hdrptr);
  tracklen = rev_long (hdr.track_size);
  hdrptr += sizeof (struct track_header);      /* point past header */
  chk_bufdata (hdrptr, tracklen);
  track[tracknum].trkptr = hdrptr;
  hdrptr += tracklen;          /* point to the start of the next track */
  track[tracknum].trkend = hdrptr;     /* the point past the end of the track */
}

unsigned char AudioGeneratorMIDI::buffer_byte (int offset) {
  unsigned char c;
  buffer.seek (buffer.data, offset);
  buffer.read (buffer.data, &c, 1);
  return c;
}

unsigned short AudioGeneratorMIDI::buffer_short (int offset) {
  unsigned short s;
  buffer.seek (buffer.data, offset);
  buffer.read (buffer.data, &s, sizeof (short));
  return s;
}

unsigned int AudioGeneratorMIDI::buffer_int32 (int offset) {
  uint32_t i;
  buffer.seek (buffer.data, offset);
  buffer.read (buffer.data, &i, sizeof (i));
  return i;
}

/* Get a MIDI-style variable-length integer */

unsigned long AudioGeneratorMIDI::get_varlen (int *ptr) {
  /* Get a 1-4 byte variable-length value and adjust the pointer past it.
    These are a succession of 7-bit values with a MSB bit of zero marking the end */

  unsigned long val;
  int i, byte;

  val = 0;
  for (i = 0; i < 4; ++i) {
    byte = buffer_byte ((*ptr)++);
    val = (val << 7) | (byte & 0x7f);
    if (!(byte & 0x80))
      return val;
  }
  return val;
}


/***************  Process the MIDI track data  ***************************/

/* Skip in the track for the next "note on", "note off" or "set tempo" command,
  then record that information in the track status block and return. */

void AudioGeneratorMIDI::find_note (int tracknum) {
  unsigned long int delta_time;
  int event, chan;
  int note, velocity, controller, pressure, pitchbend, instrument;
  int meta_cmd, meta_length;
  unsigned long int sysex_length;
  struct track_status *t;
  const char *tag;

  /* process events */

  t = &track[tracknum];        /* our track status structure */
  while (t->trkptr < t->trkend) {

    delta_time = get_varlen (&t->trkptr);
    t->time += delta_time;
    if (buffer_byte (t->trkptr) < 0x80)
      event = t->last_event; /* using "running status": same event as before */
    else {                    /* otherwise get new "status" (event type) */
      event = buffer_byte (t->trkptr++);
    }
    if (event == 0xff) {      /* meta-event */
      meta_cmd = buffer_byte (t->trkptr++);
      meta_length = get_varlen(&t->trkptr);
      switch (meta_cmd) {
        case 0x00:
          break;
        case 0x01:
          tag = "description";
          goto show_text;
        case 0x02:
          tag = "copyright";
          goto show_text;
        case 0x03:
          tag = "track name";
          goto show_text;
        case 0x04:
          tag = "instrument name";
          goto show_text;
        case 0x05:
          tag = "lyric";
          goto show_text;
        case 0x06:
          tag = "marked point";
          goto show_text;
        case 0x07:
          tag = "cue point";
show_text:
          break;
        case 0x20:
          break;
        case 0x2f:
          break;
        case 0x51:            /* tempo: 3 byte big-endian integer! */
          t->cmd = CMD_TEMPO;
          t->tempo = rev_long (buffer_int32 (t->trkptr - 1)) & 0xffffffL;
          t->trkptr += meta_length;
          return;
        case 0x54:
          break;
        case 0x58:
          break;
        case 0x59:
          break;
        case 0x7f:
          tag = "sequencer data";
          goto show_hex;
        default:              /* unknown meta command */
          tag = "???";
show_hex:
          break;
      }
      t->trkptr += meta_length;
    }

    else if (event < 0x80)
      midi_error ("Unknown MIDI event type", t->trkptr);

    else {
      if (event < 0xf0)
        t->last_event = event;      // remember "running status" if not meta or sysex event
      chan = event & 0xf;
      t->chan = chan;
      switch (event >> 4) {
        case 0x8:
          t->note = buffer_byte (t->trkptr++);
          velocity = buffer_byte (t->trkptr++);
note_off:
          t->cmd = CMD_STOPNOTE;
          return;             /* stop processing and return */
        case 0x9:
          t->note = buffer_byte (t->trkptr++);
          velocity = buffer_byte (t->trkptr++);
          if (velocity == 0)  /* some scores use note-on with zero velocity for off! */
            goto note_off;
          t->velocity = velocity;
          t->cmd = CMD_PLAYNOTE;
          return;             /* stop processing and return */
        case 0xa:
          note = buffer_byte (t->trkptr++);
          velocity = buffer_byte (t->trkptr++);
          break;
        case 0xb:
          controller = buffer_byte (t->trkptr++);
          velocity = buffer_byte (t->trkptr++);
          break;
        case 0xc:
          instrument = buffer_byte (t->trkptr++);
          midi_chan_instrument[chan] = instrument;    // record new instrument for this channel
          break;
        case 0xd:
          pressure = buffer_byte (t->trkptr++);
          break;
        case 0xe:
          pitchbend = buffer_byte (t->trkptr) | (buffer_byte (t->trkptr + 1) << 7);
          t->trkptr += 2;
          break;
        case 0xf:
          sysex_length = get_varlen (&t->trkptr);
          t->trkptr += sysex_length;
          break;
        default:
          midi_error ("Unknown MIDI command", t->trkptr);
      }
    }
  }
  t->cmd = CMD_TRACKDONE;      /* no more notes to process */
  ++tracks_done;

  // Remove unused warnings..maybe some day we'll look at these
  (void)note;
  (void)controller;
  (void)pressure;
  (void)pitchbend;
  (void)tag;
}


// Open file, parse headers, get ready tio process MIDI
void AudioGeneratorMIDI::PrepareMIDI(AudioFileSource *src)
{
  MakeStreamFromAFS(src, &afsMIDI);
  tsf_stream_wrap_cached(&afsMIDI, 32, 64, &buffer);
  buflen = buffer.size (buffer.data);

  /* process the MIDI file header */

  hdrptr = buffer.tell (buffer.data);  /* pointer to file and track headers */
  process_header ();
  printf ("  Processing %d tracks.\n", num_tracks);
  if (num_tracks > MAX_TRACKS)
    midi_error ("Too many tracks", buffer.tell (buffer.data));

  /* initialize processing of all the tracks */

  for (tracknum = 0; tracknum < num_tracks; ++tracknum) {
    start_track (tracknum);   /* process the track header */
    find_note (tracknum);     /* position to the first note on/off */
  }

  notes_skipped = 0;
  tracknum = 0;
  earliest_tracknum = 0;
  earliest_time = 0;
}

// Parses the note on/offs until we are ready to render some more samples.  Then return the
// total number of samples to render before we need to be called again
int AudioGeneratorMIDI::PlayMIDI()
{
  /* Continue processing all tracks, in an order based on the simulated time.
    This is not unlike multiway merging used for tape sorting algoritms in the 50's! */

  do {                         /* while there are still track notes to process */
    static struct track_status *trk;
    static struct tonegen_status *tg;
    static int tgnum;
    static int count_tracks;
    static unsigned long delta_time, delta_msec;

    /* Find the track with the earliest event time,
       and output a delay command if time has advanced.

       A potential improvement: If there are multiple tracks with the same time,
       first do the ones with STOPNOTE as the next command, if any.  That would
       help avoid running out of tone generators.  In practice, though, most MIDI
       files do all the STOPNOTEs first anyway, so it won't have much effect.
    */

    earliest_time = 0x7fffffff;

    /* Usually we start with the track after the one we did last time (tracknum),
       so that if we run out of tone generators, we have been fair to all the tracks.
       The alternate "strategy1" says we always start with track 0, which means
       that we favor early tracks over later ones when there aren't enough tone generators.
    */

    count_tracks = num_tracks;
    do {
      if (++tracknum >= num_tracks)
        tracknum = 0;
      trk = &track[tracknum];
      if (trk->cmd != CMD_TRACKDONE && trk->time < earliest_time) {
        earliest_time = trk->time;
        earliest_tracknum = tracknum;
      }
    } while (--count_tracks);

    tracknum = earliest_tracknum;     /* the track we picked */
    trk = &track[tracknum];
    if (earliest_time < timenow)
      midi_error ("INTERNAL: time went backwards", trk->trkptr);

    /* If time has advanced, output a "delay" command */

    delta_time = earliest_time - timenow;
    if (delta_time) {
      /* Convert ticks to milliseconds based on the current tempo */
      unsigned long long temp;
      temp = ((unsigned long long) delta_time * tempo) / ticks_per_beat;
      delta_msec = temp / 1000;      // get around LCC compiler bug
      if (delta_msec > 0x7fff)
        midi_error ("INTERNAL: time delta too big", trk->trkptr);
      int samples = (((int) delta_msec) * freq) / 1000;
      timenow = earliest_time;
      return samples;
    }
    timenow = earliest_time;

    /*  If this track event is "set tempo", just change the global tempo.
       That affects how we generate "delay" commands. */

    if (trk->cmd == CMD_TEMPO) {
      tempo = trk->tempo;
      find_note (tracknum);
    }

    /*  If this track event is "stop note", process it and all subsequent "stop notes" for this track
       that are happening at the same time. Doing so frees up as many tone generators as possible.  */

    else if (trk->cmd == CMD_STOPNOTE)
      do {
        // stop a note
        for (tgnum = 0; tgnum < num_tonegens; ++tgnum) {    /* find which generator is playing it */
          tg = &tonegen[tgnum];
          if (tg->playing && tg->track == tracknum && tg->note == trk->note) {
            tsf_note_off (g_tsf, tg->instrument, tg->note);
            tg->playing = false;
            trk->tonegens[tgnum] = false;
          }
        }
        find_note (tracknum);       // use up the note
      } while (trk->cmd == CMD_STOPNOTE && trk->time == timenow);

    /*  If this track event is "start note", process only it.
       Don't do more than one, so we allow other tracks their chance at grabbing tone generators. */

    else if (trk->cmd == CMD_PLAYNOTE) {
      bool foundgen = false;
      /* if not, then try for any free tone generator */
      if (!foundgen)
        for (tgnum = 0; tgnum < num_tonegens; ++tgnum) {
          tg = &tonegen[tgnum];
          if (!tg->playing) {
            foundgen = true;
            break;
          }
        }
      if (foundgen) {
        if (tgnum + 1 > num_tonegens_used)
          num_tonegens_used = tgnum + 1;
        tg->playing = true;
        tg->track = tracknum;
        tg->note = trk->note;
        trk->tonegens[tgnum] = true;
        trk->preferred_tonegen = tgnum;
        if (tg->instrument != midi_chan_instrument[trk->chan]) {    /* new instrument for this generator */
          tg->instrument = midi_chan_instrument[trk->chan];
        }
        tsf_note_on (g_tsf, tg->instrument, tg->note, trk->velocity / 127.0); // velocity = 0...127
      } else {
        ++notes_skipped;
      }
      find_note (tracknum);     // use up the note
    }
  }
  while (tracks_done < num_tracks);
  return -1; // EOF
}


void AudioGeneratorMIDI::StopMIDI()
{

  buffer.close(buffer.data);
  tsf_close(g_tsf);
  printf ("  %s %d tone generators were used.\n",
          num_tonegens_used < num_tonegens ? "Only" : "All", num_tonegens_used);
  if (notes_skipped)
    printf
    ("  %d notes were skipped because there weren't enough tone generators.\n", notes_skipped);

  printf ("  Done.\n");
}


bool AudioGeneratorMIDI::begin(AudioFileSource *src, AudioOutput *out)
{
  // Clear out status variables
  for (int i=0; i<MAX_TONEGENS; i++) memset(&tonegen[i], 0, sizeof(struct tonegen_status));
  for (int i=0; i<MAX_TRACKS; i++) memset(&track[i], 0, sizeof(struct track_status));
  memset(midi_chan_instrument, 0, sizeof(midi_chan_instrument));

  g_tsf = tsf_load(&afsSF2);
  if (!g_tsf) return false;
  tsf_set_output (g_tsf, TSF_MONO, freq, -10 /* dB gain -10 */ );

  if (!out->SetRate( freq )) return false;
  if (!out->SetBitsPerSample( 16 )) return false;
  if (!out->SetChannels( 1 )) return false;
  if (!out->begin()) return false;

  output = out;
  file = src;

  running = true;

  PrepareMIDI(src);

  samplesToPlay = 0;
  numSamplesRendered = 0;
  sentSamplesRendered = 0;
  
  sawEOF = false;
  return running;
}


bool AudioGeneratorMIDI::loop()
{
  static int c = 0;

  if (!running) goto done; // Nothing to do here!

  // First, try and push in the stored sample.  If we can't, then punt and try later
  if (!output->ConsumeSample(lastSample)) goto done; // Can't send, but no error detected

  // Try and stuff the buffer one sample at a time
  do {
    c++;
    if (c%44100 == 0) yield();

play:

    if (sentSamplesRendered < numSamplesRendered) {
      lastSample[AudioOutput::LEFTCHANNEL] = samplesRendered[sentSamplesRendered];
      lastSample[AudioOutput::RIGHTCHANNEL] = samplesRendered[sentSamplesRendered];
      sentSamplesRendered++;
    } else if (samplesToPlay) {
      numSamplesRendered = sizeof(samplesRendered)/sizeof(samplesRendered[0]);
      if ((int)samplesToPlay < (int)(sizeof(samplesRendered)/sizeof(samplesRendered[0]))) numSamplesRendered = samplesToPlay;
      tsf_render_short_fast(g_tsf, samplesRendered, numSamplesRendered, 0);
      lastSample[AudioOutput::LEFTCHANNEL] = samplesRendered[0];
      lastSample[AudioOutput::RIGHTCHANNEL] = samplesRendered[0];
      sentSamplesRendered = 1;
      samplesToPlay -= numSamplesRendered;
    } else {
      numSamplesRendered = 0;
      sentSamplesRendered = 0;
      if (sawEOF) {
        running = false;
      } else {
        samplesToPlay = PlayMIDI();
        if (samplesToPlay == -1) {
            sawEOF = true;
            samplesToPlay = freq / 2;
        }
        goto play;
      }
    }
  } while (running && output->ConsumeSample(lastSample));

done:
  file->loop();
  output->loop();

  return running;
}
 
bool AudioGeneratorMIDI::stop()
{
  StopMIDI();
  output->stop();
  return true;
}


int AudioGeneratorMIDI::afs_read(void *data, void *ptr, unsigned int size)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->read(ptr, size);
}

int AudioGeneratorMIDI::afs_tell(void *data)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->getPos();
}

int AudioGeneratorMIDI::afs_skip(void *data, unsigned int count)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->seek(count, SEEK_CUR);
}

int AudioGeneratorMIDI::afs_seek(void *data, unsigned int pos)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->seek(pos, SEEK_SET);
}

int AudioGeneratorMIDI::afs_close(void *data)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->close();
}

int AudioGeneratorMIDI::afs_size(void *data)
{
  AudioFileSource *s = reinterpret_cast<AudioFileSource *>(data);
  return s->getSize();
}

void AudioGeneratorMIDI::MakeStreamFromAFS(AudioFileSource *src, tsf_stream *afs)
{
  afs->data = reinterpret_cast<void*>(src);
  afs->read = &afs_read;
  afs->tell = &afs_tell;
  afs->skip = &afs_skip;
  afs->seek = &afs_seek;
  afs->close = &afs_close;
  afs->size = &afs_size;
}

