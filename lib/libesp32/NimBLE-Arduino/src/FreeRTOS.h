/*
 * FreeRTOS.h
 *
 *  Created on: Feb 24, 2017
 *      Author: kolban
 */

#ifndef MAIN_FREERTOS_H_
#define MAIN_FREERTOS_H_

#include <freertos/FreeRTOS.h>   // Include the base FreeRTOS definitions.
#include <freertos/task.h>       // Include the task definitions.
#include <freertos/semphr.h>     // Include the semaphore definitions.
#include <freertos/ringbuf.h>    // Include the ringbuffer definitions.

#include <stdint.h>
#include <string>
#include <pthread.h>


/**
 * @brief Interface to %FreeRTOS functions.
 */
class FreeRTOS {
public:
    static void sleep(uint32_t ms);
    static void startTask(void task(void*), std::string taskName, void* param = nullptr, uint32_t stackSize = 2048);
    static void deleteTask(TaskHandle_t pTask = nullptr);

    static uint32_t getTimeSinceStart();

/**
 * @brief A binary semaphore class that operates like a mutex, it is already given when constructed.
 */
    class Semaphore {
    public:
        Semaphore(std::string owner = "<Unknown>");
        ~Semaphore();
        void        give();
        void        give(uint32_t value);
        void        giveFromISR();
        void        setName(std::string name);
        bool        take(std::string owner = "<Unknown>");
        bool        take(uint32_t timeoutMs, std::string owner = "<Unknown>");
        std::string toString();
        bool        timedWait(std::string owner = "<Unknown>", uint32_t timeoutMs = portMAX_DELAY);
        uint32_t    wait(std::string owner = "<Unknown>");
        /**
         * @brief Get the value of the semaphore.
         * @return The value stored if the semaphore was given with give(value);
         */
        uint32_t    value(){ return m_value; };

    private:
        SemaphoreHandle_t m_semaphore;
        pthread_mutex_t   m_pthread_mutex;
        std::string       m_name;
        std::string       m_owner;
        uint32_t          m_value;
        bool              m_usePthreads;

    };
};


/**
 * @brief A wrapper class for a freeRTOS ringbuffer.
 */
class Ringbuffer {
public:
#if defined(ESP_IDF_VERSION) && !defined(ESP_IDF_VERSION_VAL) //Quick hack to detect if using IDF version that replaced ringbuf_type_t, ESP_IDF_VERSION_VAL is for IDF>4.0.0
    Ringbuffer(size_t length, RingbufferType_t type = RINGBUF_TYPE_NOSPLIT);
#else
    Ringbuffer(size_t length, ringbuf_type_t type = RINGBUF_TYPE_NOSPLIT);
#endif
    ~Ringbuffer();

    void*    receive(size_t* size, TickType_t wait = portMAX_DELAY);
    void     returnItem(void* item);
    bool     send(void* data, size_t length, TickType_t wait = portMAX_DELAY);
private:
    RingbufHandle_t m_handle;
};

#endif /* MAIN_FREERTOS_H_ */
