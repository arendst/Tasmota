The current library needs two fixes solving the deprecated ``i2s_write_bytes`` function.

In file AudioOutputI2S.cpp:

bool AudioOutputI2S::ConsumeSample(int16_t sample[2])
{

  //return if we haven't called ::begin yet
  if (!i2sOn)
    return false;

  int16_t ms[2];

  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16( ms );

  if (this->mono) {
    // Average the two samples and overwrite
    int32_t ttl = ms[LEFTCHANNEL] + ms[RIGHTCHANNEL];
    ms[LEFTCHANNEL] = ms[RIGHTCHANNEL] = (ttl>>1) & 0xffff;
  }
  #ifdef ESP32
    uint32_t s32;
    if (output_mode == INTERNAL_DAC)
    {
      int16_t l = Amplify(ms[LEFTCHANNEL]) + 0x8000;
      int16_t r = Amplify(ms[RIGHTCHANNEL]) + 0x8000;
      s32 = (r << 16) | (l & 0xffff);
    }
    else
    {
      s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
    }
// Deprecated. Use i2s_write
//    return i2s_write_bytes((i2s_port_t)portNo, (const char *)&s32, sizeof(uint32_t), 0);
    size_t bytes_written;
    i2s_write((i2s_port_t)portNo, (const char*)&s32, sizeof(uint32_t), &bytes_written, 0);
    return bytes_written;
  #elif defined(ESP8266)
    uint32_t s32 = ((Amplify(ms[RIGHTCHANNEL])) << 16) | (Amplify(ms[LEFTCHANNEL]) & 0xffff);
    return i2s_write_sample_nb(s32); // If we can't store it, return false.  OTW true
  #elif defined(ARDUINO_ARCH_RP2040)
    return !!I2S.write((void*)ms, 4);
  #endif
}

In file AudioOutputI2SNoDac.cpp:

bool AudioOutputI2SNoDAC::ConsumeSample(int16_t sample[2])
{
  int16_t ms[2];
  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16( ms );

  // Make delta-sigma filled buffer
  uint32_t dsBuff[8];
  DeltaSigma(ms, dsBuff);

  // Either send complete pulse stream or nothing
#ifdef ESP32
// Deprecated. Use i2s_write
//  if (!i2s_write_bytes((i2s_port_t)portNo, (const char *)dsBuff, sizeof(uint32_t) * (oversample/32), 0))
  size_t bytes_written;
  i2s_write((i2s_port_t)portNo, (const char *)dsBuff, sizeof(uint32_t) * (oversample/32), &bytes_written, 0);
  if (!bytes_written)
    return false;
#elif defined(ESP8266)
  if (!i2s_write_sample_nb(dsBuff[0])) return false; // No room at the inn
  // At this point we've sent in first of possibly 8 32-bits, need to send
  // remaining ones even if they block.
  for (int i = 32; i < oversample; i+=32)
    i2s_write_sample( dsBuff[i / 32]);
#elif defined(ARDUINO_ARCH_RP2040)
  int16_t *p = (int16_t *) dsBuff;
  for (int i = 0; i < oversample / 16; i++) {
    I2S.write(*(p++));
  }
#endif
  return true;
}
