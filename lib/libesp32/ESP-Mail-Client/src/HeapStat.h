/** Memory usage info for debugging purpose

#include "HeapStat.h"

HeapStat heapInfo;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // Other codes here

    // Collect memory info
    heapInfo.collect();

    // Print memory info
    heapInfo.print();

}

*/

#ifndef HEAP_STAT_H
#define HEAP_STAT_H

#if defined(ESP8266) || defined(ESP32)
#ifndef MB_ARDUINO_ESP
#define MB_ARDUINO_ESP
#endif
#endif

#if defined(ARDUINO_ARCH_RP2040)

#if defined(ARDUINO_NANO_RP2040_CONNECT)
#ifndef MB_ARDUINO_NANO_RP2040_CONNECT
#define MB_ARDUINO_NANO_RP2040_CONNECT
#endif
#else
#ifndef MB_ARDUINO_PICO
#define MB_ARDUINO_PICO
#endif
#endif

#endif

#include "ESP_Mail_Client.h"

class HeapStat
{
private:
    int current_heap = 0;
    int first_round_heap = 0;
    int min_heap = 0;
    int max_heap = 0;
    int diff_heap_from_first_round = 0;
    int diff_heap_from_current_round = 0;
    int _count = 0;

    int getFreeHeap()
    {
#if defined(MB_ARDUINO_ESP)
        return ESP.getFreeHeap();
#elif defined(MB_ARDUINO_PICO)
        return rp2040.getFreeHeap();
#else
        return 0;
#endif
    }

public:
    HeapStat() {}
    ~HeapStat() {}

    void reset()
    {
        current_heap = 0;
        first_round_heap = 0;
        min_heap = 0;
        max_heap = 0;
        diff_heap_from_first_round = 0;
        diff_heap_from_current_round = 0;
        _count = 0;
    }

    void collect()
    {
        _count++;

        if (max_heap < getFreeHeap())
            max_heap = getFreeHeap();

        if (min_heap == 0 || min_heap > getFreeHeap())
            min_heap = getFreeHeap();

        if (first_round_heap == 0)
            first_round_heap = getFreeHeap();

        if (current_heap > 0)
        {
            diff_heap_from_first_round = getFreeHeap() - first_round_heap;
            diff_heap_from_current_round = getFreeHeap() - current_heap;
        }

        current_heap = getFreeHeap();
    }

    int current()
    {
        return current_heap;
    }

    int min()
    {
        return min_heap;
    }

    int max()
    {
        return max_heap;
    }

    int diff1()
    {
        return diff_heap_from_first_round;
    }

    int diffN()
    {
        return diff_heap_from_current_round;
    }

    int count()
    {
        return _count;
    }

    void print()
    {
        MailClient.printf("#### Heap Info\n#### Current: %d, Min: %d, Max: %d, Diff_1: %d, Diff_%d: %d\n", current(), min(), max(), diff1(), count(), diffN());
    }
};

#endif