#include "advertisement_list.h"

AdvertisementList::AdvertisementList()
  : currentSize_(0)
{
  list_.resize(MAX_SIZE);
}

bool AdvertisementList::tryAdd(const Advertisement& advertisement)
{
  if (currentSize_ < MAX_SIZE && !containsDevice(advertisement)) {
    list_.push_front(advertisement);
    currentSize_ += 1;
    return true;
  } else {
    return false;
  }
}

bool AdvertisementList::containsDevice(const Advertisement& advertisement) const
{
  for (auto it = list_.begin(); it != list_.end(); ++it) {
    if (it->device_detected(advertisement)) {
      return true;
    }
  }
  return false;
}

bool AdvertisementList::tryUpdateData(const Advertisement& advertisement)
{
  for (auto it = list_.begin(); it != list_.end(); ++it) {
    if (*it != advertisement) {
      list_.remove(*it);
      list_.push_front(advertisement);
      return true;
    }
  }
  return false;
}

void AdvertisementList::printList() const
{
  printf("--------------------------LIST-------------------------\r\n\r\n");
  for (auto it = list_.begin(); it != list_.end(); ++it) {
    it->print();
  }
  printf("--------------------------END---------------------------\r\n\r\n");
}
