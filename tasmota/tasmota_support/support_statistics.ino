/*
  support_statistics.ino - gather statistics for Tasmota

  Copyright (C) 2021  Theo Arends

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

#define USE_STATS_CODE

#ifdef USE_STATS_CODE
/*********************************************************************************************\
 * Gather statistics
\*********************************************************************************************/

String GetStatistics(void)
{
  char data[40];
  snprintf_P(data, sizeof(data), PSTR(",\"CR\":\"%d/%d\""), GetSettingsTextLen(), settings_text_size);  // Char Usage Ratio
  return String(data);
}

#else

String GetStatistics(void)
{
  return String("");
}

#endif  // USE_STATS_CODE