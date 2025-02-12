/**
  * @file RingBuffer.h
  * @author German Martin
  * @brief Library to build a gateway for EnigmaIoT system
  */

#ifndef _RINGBUFFER_h
#define _RINGBUFFER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
// Disable debug dependency if debug level is 0
#if DEBUG_LEVEL > 0
#include <QuickDebug.h>
static const char* RINGBUFFER_DEBUG_TAG = "RINGBUFFER";
#else // DEBUG_LEVEL
#define DEBUG_ERROR(...)
#define DEBUG_INFO(...)
#define DEBUG_VERBOSE(...)
#define DEBUG_WARN(...)
#define DEBUG_DBG(...)
#endif


/**
  * @brief Ring buffer class. Used to implement message buffer
  *
  */
template <typename Telement>
class RingBuffer {
protected:
    int maxSize; ///< @brief Buffer size
    int numElements = 0; ///< @brief Number of elements that buffer currently has
    int readIndex = 0; ///< @brief Pointer to next item to be read
    int writeIndex = 0; ///< @brief Pointer to next position to write onto
    Telement* buffer; ///< @brief Actual buffer

public:
    /**
      * @brief Creates a ring buffer to hold `Telement` objects
      * @param range Buffer depth
      */
    RingBuffer <Telement> (int range) : maxSize (range) {
        buffer = new Telement[maxSize];
    }

    /**
      * @brief EnigmaIOTRingBuffer destructor
      * @param range Free up buffer memory
      */
    ~RingBuffer () {
        maxSize = 0;
        delete[] (buffer);
    }

    /**
      * @brief Returns actual number of elements that buffer holds
      * @return Returns Actual number of elements that buffer holds
      */
    int size () { return numElements; }

    /**
      * @brief Checks if buffer is full
      * @return Returns `true`if buffer is full, `false` otherwise
      */
    bool isFull () { return numElements == maxSize; }

    /**
      * @brief Checks if buffer is empty
      * @return Returns `true`if buffer has no elements stored, `false` otherwise
      */
    bool empty () { return (numElements == 0); }

    /**
      * @brief Adds a new item to buffer, deleting older element if it is full
      * @param item Element to add to buffer
      * @return Returns `false` if buffer was full before inserting the new element, `true` otherwise
      */
    bool push (Telement* item) {
        bool wasFull = isFull ();
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "Add element. Buffer was %s", wasFull ? "full" : "not full");
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "Before -- > ReadIdx: %d. WriteIdx: %d. Size: %d", readIndex, writeIndex, numElements);
#ifdef ESP32
        portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;
        portENTER_CRITICAL (&myMutex);
#endif
        memcpy (&(buffer[writeIndex]), item, sizeof (Telement));
        //Serial.printf ("Copied: %d bytes\n", sizeof (Telement));
        writeIndex++;
        if (writeIndex >= maxSize) {
            writeIndex %= maxSize;
        }
        if (wasFull) { // old value is no longer valid
            readIndex++;
            if (readIndex >= maxSize) {
                readIndex %= maxSize;
            }
        } else {
            numElements++;
        }
#ifdef ESP32
        portEXIT_CRITICAL (&myMutex);
#endif
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "After -- > ReadIdx: %d. WriteIdx: %d. Size: %d", readIndex, writeIndex, numElements);
        return !wasFull;
    }

    /**
      * @brief Deletes older item from buffer, if buffer is not empty
      * @return Returns `false` if buffer was empty before trying to delete element, `true` otherwise
      */
    bool pop () {
        bool wasEmpty = empty ();
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "Remove element. Buffer was %s", wasEmpty ? "empty" : "not empty");
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "Before -- > ReadIdx: %d. WriteIdx: %d. Size: %d", readIndex, writeIndex, numElements);
        if (!wasEmpty) {
            readIndex++;
            if (readIndex >= maxSize) {
                readIndex %= maxSize;
            }
            numElements--;
        }
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "After -- > ReadIdx: %d. WriteIdx: %d. Size: %d", readIndex, writeIndex, numElements);
        return !wasEmpty;
    }

    /**
      * @brief Gets a pointer to older item in buffer, if buffer is not empty
      * @return Returns pointer to element. If buffer was empty before calling this method it returns `NULL`
      */
    Telement* front () {
        DEBUG_DBG (RINGBUFFER_DEBUG_TAG, "Read element. ReadIdx: %d. WriteIdx: %d. Size: %d", readIndex, writeIndex, numElements);
        if (!empty ()) {
            return &(buffer[readIndex]);
        } else {
            return NULL;
        }
    }
};

#endif // _RINGBUFFER_h