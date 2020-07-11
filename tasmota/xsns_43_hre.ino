/*
  xsns_43_hre.ino - Badger HR-E Water Meter Encoder interface

  Copyright (C) 2020  Jon Little

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

#ifdef USE_HRE
/*********************************************************************************************\
 * HR-E LCD Water meter register interface
 *
 * https://www.badgermeter.com/business-lines/utility/high-resolution-lcd-encoders-hr-e-lcd/
 * Source: Jon Little, https://github.com/burundiocibu/particle/blob/master/water_meter/src/HRE_Reader.cpp
 *
 * This code marches the bits out the data line as ASCII characters with the form
 *    KG44?Q45484=0444444V;RB000000022;IB018435683
 * where the RB...; is the miligalons used
 *
 * Note that this sensor takes a _long_ time to read. 62 bits * 4 ms/bit for the
 * sync sequence plus 46 bytes * 40 ms/byte = 2088 ms minimum. If we aren't alligned
 * to the sync sequence, it could be almost twice that.
 * To keep from bogging the kernel down, we read 8 bits at a time on the 50 ms callback.
 * It will take seconds to discover if the device is there.
 *
 * In lieu of an actual schematic to describe the electrical interface, here is a description:
 *
 * hre_clock_pin: drives the power/clock for the water meter through a 1k resister to
 *   the base of a pnp transistor
 * hre_data_pin: is the data and has a 1 k pulldown
 *
 * The pnp transitor has the collector connected to the power/clock and is pulled up
 * to +5 via a 1 k resistor.
 * The emitter is connected to ground
 *
\*********************************************************************************************/

#define XSNS_43             43

enum hre_states {
   hre_idle,    // Initial state,
   hre_sync,    // Start search for sync sequence
   hre_syncing, // Searching for sync sequence
   hre_read,    // Start reading data block
   hre_reading, // Reading data
   hre_sleep,   // Start sleeping
   hre_sleeping // pausing before reading again
};

hre_states hre_state = hre_idle;

float hre_usage = 0; // total water usage, in gal
float hre_rate = 0;  // flow rate, in gal/min
uint32_t hre_usage_time = 0; // uptime associated with hre_usage and hre_rate

int hre_read_errors = 0; // total number of read errors since boot
bool hre_good =  false;


// The settling times here were determined using a single unit hooked to a scope
int hreReadBit()
{
   digitalWrite(Pin(GPIO_HRE_CLOCK), HIGH);
   delay(1);
   int bit = digitalRead(Pin(GPIO_HRE_DATA));
   digitalWrite(Pin(GPIO_HRE_CLOCK), LOW);
   delay(1);
   return bit;
}

// With the times in the HreReadBit routine, a characer will take
// 20 ms plus io time.
char hreReadChar(int &parity_errors)
{
   // start bit
   hreReadBit();

   unsigned ch=0;
   int sum=0;
   for (uint32_t i=0; i<7; i++)
   {
      int b = hreReadBit();
      ch |= b << i;
      sum += b;
   }

   // parity
   if ( (sum & 0x1) != hreReadBit())
      parity_errors++;

   // stop bit
   hreReadBit();

   return ch;
}

void hreInit(void)
{
   hre_read_errors = 0;
   hre_good = false;

   pinMode(Pin(GPIO_HRE_CLOCK), OUTPUT);
   pinMode(Pin(GPIO_HRE_DATA), INPUT);

   // Note that the level shifter inverts this line and we want to leave it
   // high when not being read.
   digitalWrite(Pin(GPIO_HRE_CLOCK), LOW);

   hre_state = hre_sync;
}


void hreEvery50ms(void)
{
   static int sync_counter = 0;   // Number of sync bit reads
   static int sync_run = 0; // Number of consecutive '1's read

   static uint32_t curr_start = 0; // uptime when entered hre_reading for current read
   static int read_counter = 0; // number of bytes in the current read
   static int parity_errors = 0; // Number of parity errors in current read
   static char buff[46];  // 8 char and a term

   static char ch;
   static size_t i;

   switch (hre_state)
   {
      case hre_sync:
         if (uptime < 10)
            break;
         sync_run = 0;
         sync_counter = 0;
         hre_state = hre_syncing;
         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE "hre_state:hre_syncing"));
         break;

      case hre_syncing:
         // Find the header, a string of 62 '1's
         // Since each bit taks 2 ms, we just read 20 bits at a time
         for (uint32_t i=0; i<20; i++)
         {
            if (hreReadBit())
               sync_run++;
            else
               sync_run = 0;
            if (sync_run == 62)
            {
               hre_state = hre_read;
               break;
            }
            sync_counter++;
         }
         // If the meter doesn't get in sync within 1000 bits, give up for now
         if (sync_counter > 1000)
         {
            hre_state = hre_sleep;
            AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE D_ERROR));
         }
         break;

         // Start reading the data block
      case hre_read:
         AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE "sync_run:%d, sync_counter:%d"), sync_run, sync_counter);
         read_counter = 0;
         parity_errors = 0;
         curr_start = uptime;
         memset(buff, 0, sizeof(buff));
         hre_state = hre_reading;
         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE "hre_state:hre_reading"));
         // So this is intended to fall through to the hre_reading section.
         // it seems that if there is much of a delay between getting the sync
         // bits and starting the read, the HRE won't output the message we
         // are looking for...

      case hre_reading:
         // Read two characters at a time...
         buff[read_counter++] = hreReadChar(parity_errors);
         buff[read_counter++] = hreReadChar(parity_errors);

         if (read_counter == 46)
         {
            AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE "pe:%d, re:%d, buff:%s"),
                      parity_errors, hre_read_errors, buff);
            if (parity_errors == 0)
            {
               float curr_usage;
               curr_usage = 0.01 * atol(buff+24); // useage in gal
               if (hre_usage_time)
               {
                  double dt = 1.666e-2 * (curr_start - hre_usage_time); // dt in minutes
                  hre_rate = (curr_usage - hre_usage)/dt; // gallons/min
               }
               hre_usage = curr_usage;
               hre_usage_time = curr_start;
               hre_good = true;

               hre_state = hre_sleep;
            }
            else
            {
               hre_read_errors++;
               hre_state = hre_sleep;
            }
         }
         break;

      case hre_sleep:
         hre_usage_time = curr_start;
         hre_state = hre_sleeping;
         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HRE "hre_state:hre_sleeping"));

      case hre_sleeping:
         // If there isn't some delay between readings, rate calculations
         // aren't as accurate. 27 seconds will give about a 30 second refresh rate
         if (uptime - hre_usage_time >= 27)
            hre_state = hre_sync;
   }
}

void hreShow(boolean json)
{
   if (!hre_good)
      return;

   const char *id = "HRE";

   char usage[16];
   char rate[16];
   dtostrfd(hre_usage, 2, usage);
   dtostrfd(hre_rate, 3, rate);

   if (json)
   {
      ResponseAppend_P(JSON_SNS_GNGPM, id, usage, rate);
#ifdef USE_WEBSERVER
   }
   else
   {
      WSContentSend_PD(HTTP_SNS_GALLONS, id, usage);
      WSContentSend_PD(HTTP_SNS_GPM, id, rate);
#endif  // USE_WEBSERVER
   }
}


/*********************************************************************************************\
 * Interface
\*********************************************************************************************/
bool Xsns43(byte function)
{
   // If we don't have pins assigned give up quickly.
   if (!PinUsed(GPIO_HRE_CLOCK) || !PinUsed(GPIO_HRE_DATA)) { return false; }

   switch (function)
   {
      case FUNC_INIT:
         hreInit();
         break;
      case FUNC_EVERY_50_MSECOND:
         hreEvery50ms();
         break;
      case FUNC_EVERY_SECOND:
         break;
      case FUNC_JSON_APPEND:
         hreShow(1);
         break;
#ifdef USE_WEBSERVER
      case FUNC_WEB_SENSOR:
         hreShow(0);
         break;
#endif  // USE_WEBSERVER
   }
   return false;
}

#endif  // USE_HRE
