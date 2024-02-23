#pragma once

#include <forward_list>
#include "advertisement.h"

// this is left outside the class because it doesn't work
// code-generation bug?!
const int MAX_SIZE = 10;

class AdvertisementList {
  private:
    int currentSize_;
    std::forward_list<Advertisement> list_;
    bool containsDevice(const Advertisement& advertisement) const;
  
  public:
    // Constructor
    AdvertisementList();

    // Methods
    bool tryAdd(const Advertisement& advertisement);
    bool tryUpdateData(const Advertisement& advertisement);
    void printList() const;
};
