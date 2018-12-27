/*
  SensorFilter.h
  
  Copyright (C) 2018 Andre Thomas and Theo Arends
  
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

#ifndef __SensorFilter_h__
#define __SensorFilter_h__

class SensorFilter_Median {
  public:
    SensorFilter_Median(uint16_t elements, uint16_t median_idx);
    double NewMedian(double newvalue);
  private:
    void MedianDataShift(void);
    void MedianDataSort(void);
    void MedianSetAll(double newvalue);
    boolean firstcall = true;
    uint16_t MedianElements;
    uint16_t MedianIndex;
    double *MedianData;
    double *MedianData_tmp;
};

class SensorFilter_Average {
  public:
    SensorFilter_Average(uint16_t elements);
    double NewAverage(double newvalue);
  private:
    double AverageShiftAndAdd(double newvalue);
    void AverageSetAll(double newvalue);
    boolean firstcall = true;
    uint16_t AverageElements;
    double *AverageData;
};

#endif // __SensorFilter_h__

