/*
  SensorFilter.cpp
  
  Copyright (C) 2018  Andre Thomas and Theo Arends
  
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
 
 Usage for SensorFilter_Median
 
 SensorFilter_Median MyMedian(x,y);  // where x = number of elements e.g. 9 and y the index which will be used e.g. 5
 newvalue = MyMedian.NewMedian(z);   // where z is the new value to be added to the sorted array to determine the median - new median is returned by the function

 Usage for SensorFilter_Average

 SensorFilter_Average myAverage(x);  // where x = number of elements over which average will be calculated
 newvalue = myAverage.NewAverage(y); // where y is the new number to be added to the array on which average will be based - new average is returned by the function

*/

#include <Arduino.h>
#include "SensorFilter.h"

SensorFilter_Median::SensorFilter_Median(uint16_t elements, uint16_t median_idx)
{
  MedianElements = elements;
  MedianIndex = median_idx;
  MedianData = (double *) malloc(sizeof(double)*elements);
  MedianData_tmp = (double *) malloc(sizeof(double)*elements);
}

double SensorFilter_Median::NewMedian(double newvalue)
{
  if (firstcall) { MedianSetAll(newvalue); }
  MedianDataShift();
  MedianData[MedianElements-1] = newvalue;
  MedianDataSort();
  return MedianData_tmp[MedianIndex];
}

void SensorFilter_Median::MedianDataShift(void)
{
  for (uint8_t i = 0;i < (MedianElements-1);i++) {
    MedianData[i]=MedianData[i+1];
  }
}

/*
 * Could probably use a more complex quicksort but given the amount of variables in the array its likely negligible compared to the re-allocation in contrast.
 */

void SensorFilter_Median::MedianDataSort(void)
{
  double tmp;
  memcpy(MedianData_tmp,MedianData,sizeof(double)*MedianElements);
  boolean sorted = false;
  while (!sorted) {
    sorted = true;
    for (uint8_t i = 0;i < (MedianElements-1);i++) {
      if (MedianData_tmp[i] > MedianData_tmp[i+1]) {
        sorted=false;
        tmp = MedianData_tmp[i];
        MedianData_tmp[i] = MedianData_tmp[i+1];
        MedianData_tmp[i+1] = tmp;
      }
    }
  }
}

void SensorFilter_Median::MedianSetAll(double newvalue)
{
  for (uint8_t i = 0;i < MedianElements;i++) {
    MedianData[i] = newvalue;
  }
  firstcall = false;
}

/*
 * SensorFilter_Average implementation
 */

SensorFilter_Average::SensorFilter_Average(uint16_t elements)
{
  AverageElements = elements;
  AverageData = (double *) malloc(sizeof(double)*elements);
}

double SensorFilter_Average::NewAverage(double newvalue)
{
  double newaverage = 0;
  if (firstcall) { AverageSetAll(newvalue); }
  for (uint16_t i = 0;i < (AverageElements-1);i++) {
    AverageData[i]=AverageData[i+1];
    newaverage = newaverage + AverageData[i];
  }
  AverageData[AverageElements-1] = newvalue;
  newaverage = newaverage + newvalue;
  if (newaverage != 0) {
    newaverage = newaverage / AverageElements;
  } else {
    newaverage = 0;
  }
  return newaverage;
}

void SensorFilter_Average::AverageSetAll(double newvalue)
{
  for (uint8_t i = 0;i < AverageElements;i++) {
    AverageData[i] = newvalue;
  }
  firstcall = false;
}
