/*
  AudioOutputULP
  Outputs to ESP32 DAC through the ULP, freeing I2S for other uses
  
  Copyright (C) 2020  Martin Laclaustra, based on bitluni's code

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

#ifdef ESP32

#include "AudioOutputULP.h"
#include <esp32/ulp.h>
#include <driver/rtc_io.h>
#include <driver/dac.h>
#include <soc/rtc.h>
#include <math.h>

uint32_t create_I_WR_REG(uint32_t reg, uint32_t low_bit, uint32_t high_bit, uint32_t val){
    typedef union {ulp_insn_t ulp_ins; uint32_t ulp_bin;} ulp_union;
    const ulp_insn_t singleinstruction[] = {I_WR_REG(reg, low_bit, high_bit, val)};
    ulp_union recover_ins;
    recover_ins.ulp_ins=singleinstruction[0];
    return (uint32_t)(recover_ins.ulp_bin);
}

uint32_t create_I_BXI(uint32_t imm_pc){
    typedef union {ulp_insn_t ulp_ins; uint32_t ulp_bin;} ulp_union;
    const ulp_insn_t singleinstruction[] = {I_BXI(imm_pc)};
    ulp_union recover_ins;
    recover_ins.ulp_ins=singleinstruction[0];
    return (uint32_t)(recover_ins.ulp_bin);
}

bool AudioOutputULP::begin()
{
  if(!stereoOutput){
    waitingOddSample = false;
    //totalSampleWords += 512;
    //dacTableStart2 = dacTableStart1;
  }
  
  //calculate the actual ULP clock
  unsigned long rtc_8md256_period = rtc_clk_cal(RTC_CAL_8MD256, 1000);
  unsigned long rtc_fast_freq_hz = 1000000ULL * (1 << RTC_CLK_CAL_FRACT) * 256 / rtc_8md256_period;

  //initialize DACs
  if(activeDACs & 1){
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_voltage(DAC_CHANNEL_1, 128);
  }
  if(activeDACs & 2){
    dac_output_enable(DAC_CHANNEL_2);
    dac_output_voltage(DAC_CHANNEL_2, 128);
  }

  int retAddress1 = 9;
  int retAddress2 = 14;

  int loopCycles = 134;
  int loopHalfCycles1 = 90;
  int loopHalfCycles2 = 44;
  
  Serial.print("Real RTC clock: ");
  Serial.println(rtc_fast_freq_hz);

  uint32_t dt = (rtc_fast_freq_hz / hertz) - loopCycles;
  uint32_t dt2 = 0;
  if(!stereoOutput){
    dt = (rtc_fast_freq_hz / hertz) - loopHalfCycles1;
    dt2 = (rtc_fast_freq_hz / hertz) - loopHalfCycles2;
  }
  
  Serial.print("dt: ");
  Serial.println(dt);
  
  Serial.print("dt2: ");
  Serial.println(dt2);

  const ulp_insn_t stereo[] = {
    //reset offset register
    I_MOVI(R3, 0),
    //delay to get the right sampling rate
    I_DELAY(dt), // 6 + dt
    //reset sample index
    I_MOVI(R0, 0), // 6
    //write the index back to memory for the main cpu
    I_ST(R0, R3, indexAddress), // 8
    //load the samples
    I_LD(R1, R0, bufferStart), // 8
    //mask the lower 8 bits
    I_ANDI(R2, R1, 0x00ff), // 6
    //multiply by 2
    I_LSHI(R2, R2, 1), // 6
    //add start position
    I_ADDI(R2, R2, dacTableStart1),// 6
    //jump to the dac opcode
    I_BXR(R2), // 4
    //back from first dac
    //delay between the two samples in mono rendering
    I_DELAY(dt2), // 6 + dt2
    //mask the upper 8 bits
    I_ANDI(R2, R1, 0xff00), // 6
    //shift the upper bits to right and multiply by 2
    I_RSHI(R2, R2, 8 - 1), // 6
    //add start position of second dac table
    I_ADDI(R2, R2, dacTableStart2),// 6
    //jump to the dac opcode
    I_BXR(R2), // 4
    //here we get back from writing the second sample
    //load 0x8080 as sample
    I_MOVI(R1, 0x8080), // 6
    //write 0x8080 in the sample buffer
    I_ST(R1, R0, indexAddress), // 8
    //increment the sample index
    I_ADDI(R0, R0, 1), // 6
    //if reached end of the buffer, jump relative to index reset
    I_BGE(-16, totalSampleWords), // 4
    //wait to get the right sample rate (2 cycles more to compensate the index reset)
    I_DELAY((unsigned int)dt + 2), // 8 + dt
    //if not, jump absolute to where index is written to memory
    I_BXI(3) // 4
  };
  // write io and jump back another 12 + 4 + 12 + 4

  size_t load_addr = 0;
  size_t size = sizeof(stereo)/sizeof(ulp_insn_t);
  ulp_process_macros_and_load(load_addr, stereo, &size);
  //  this is how to get the opcodes
  //  for(int i = 0; i < size; i++)
  //    Serial.println(RTC_SLOW_MEM[i], HEX);
  
  //create DAC opcode tables
  switch(activeDACs){
    case 1:
      for(int i = 0; i < 256; i++)
      {
        RTC_SLOW_MEM[dacTableStart1 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC1_REG,19,26,i); //dac1: 0x1D4C0121 | (i << 10)
        RTC_SLOW_MEM[dacTableStart1 + 1 + i * 2] = create_I_BXI(retAddress1); // 0x80000000 + retAddress1 * 4
        RTC_SLOW_MEM[dacTableStart2 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC1_REG,19,26,i); //dac2: 0x1D4C0122 | (i << 10)
        RTC_SLOW_MEM[dacTableStart2 + 1 + i * 2] = create_I_BXI(retAddress2); // 0x80000000 + retAddress2 * 4
      }
      break;
    case 2:
      for(int i = 0; i < 256; i++)
      {
        RTC_SLOW_MEM[dacTableStart1 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC2_REG,19,26,i); //dac1: 0x1D4C0121 | (i << 10)
        RTC_SLOW_MEM[dacTableStart1 + 1 + i * 2] = create_I_BXI(retAddress1); // 0x80000000 + retAddress1 * 4
        RTC_SLOW_MEM[dacTableStart2 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC2_REG,19,26,i); //dac2: 0x1D4C0122 | (i << 10)
        RTC_SLOW_MEM[dacTableStart2 + 1 + i * 2] = create_I_BXI(retAddress2); // 0x80000000 + retAddress2 * 4
      }
      break;
    case 3:
      for(int i = 0; i < 256; i++)
      {
        RTC_SLOW_MEM[dacTableStart1 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC1_REG,19,26,i); //dac1: 0x1D4C0121 | (i << 10)
        RTC_SLOW_MEM[dacTableStart1 + 1 + i * 2] = create_I_BXI(retAddress1); // 0x80000000 + retAddress1 * 4
        RTC_SLOW_MEM[dacTableStart2 + i * 2] = create_I_WR_REG(RTC_IO_PAD_DAC1_REG,19,26,i); //dac2: 0x1D4C0122 | (i << 10)
        RTC_SLOW_MEM[dacTableStart2 + 1 + i * 2] = create_I_BXI(retAddress2); // 0x80000000 + retAddress2 * 4
      }
      break;
  }

  //set all samples to 128 (silence)
  for(int i = 0; i < totalSampleWords; i++)
    RTC_SLOW_MEM[bufferStart + i] = 0x8080;
    
  //start
  RTC_SLOW_MEM[indexAddress] = 0;
  ulp_run(0);

  //wait until ULP starts using samples and the index of output sample advances
  while(RTC_SLOW_MEM[indexAddress] == 0)
    delay(1);

  return true;
}

bool AudioOutputULP::ConsumeSample(int16_t sample[2])
{
  int16_t ms[2];
  ms[0] = sample[0];
  ms[1] = sample[1];
  MakeSampleStereo16( ms );

  // TODO: needs improvement (counting is different here with respect to ULP code)
  int currentSample = RTC_SLOW_MEM[indexAddress] & 0xffff;
  int currentWord = currentSample >> 1;

  for (int i=0; i<2; i++) {
    ms[i] = ((ms[i] >> 8) + 128) & 0xff;
  }
  if(!stereoOutput) // mix both channels
    ms[0] = (uint16_t)(( (uint32_t)((int32_t)(ms[0]) + (int32_t)(ms[1])) >> 1 ) & 0xff);

  if(waitingOddSample){ // always true for stereo because samples are consumed in pairs
    if(lastFilledWord != currentWord) // accept sample if writing index lastFilledWord has not reached index of output sample
    {
      unsigned int w;
      if(stereoOutput){
        w = ms[0];
        w |= ms[1] << 8;
      } else {
        w = bufferedOddSample;
        w |= ms[0] << 8;
        bufferedOddSample = 128;
        waitingOddSample = false;
      }
      RTC_SLOW_MEM[bufferStart + lastFilledWord] = w;
      lastFilledWord++;
      if(lastFilledWord == totalSampleWords)
        lastFilledWord = 0;
      return true;
    } else {
      return false;
    }
  } else {
    bufferedOddSample = ms[0];
    waitingOddSample = true;
    return true;
  }
}


bool AudioOutputULP::stop()
{
  audioLogger->printf_P(PSTR("\n\n\nstop\n\n\n"));
  const ulp_insn_t stopulp[] = {
    //stop the timer
    I_END(),
    //end the program
    I_HALT()};

  size_t load_addr = 0;
  size_t size = sizeof(stopulp)/sizeof(ulp_insn_t);
  ulp_process_macros_and_load(load_addr, stopulp, &size);
  
  //start
  ulp_run(0);

  if(activeDACs & 1){
    dac_output_voltage(DAC_CHANNEL_1, 128);
  }
  if(activeDACs & 2){
    dac_output_voltage(DAC_CHANNEL_2, 128);
  }

  return true;
}

#endif
