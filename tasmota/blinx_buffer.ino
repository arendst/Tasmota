#ifdef BLINX

bufferSensor* initBufferSensor(int size){
  bufferSensor* temp = new bufferSensor(size);
  int index = 0;
  if (size == 6){
    temp->activate(index, SIZE_BUFFER_50MS, 0);
    index ++;
  }
  
  temp->activate(index, SIZE_BUFFER_1S, 20);
  index ++;
  
  temp->activate(index, SIZE_BUFFER_10S, 10);
  index ++;
  
  temp->activate(index, SIZE_BUFFER_1M, 6);
  index ++;
  
  temp->activate(index, SIZE_BUFFER_10M, 10);
  index ++;
  
  temp->activate(index, SIZE_BUFFER_1H, 6);

  return temp;
}

#endif