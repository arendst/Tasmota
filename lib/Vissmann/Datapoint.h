#pragma once
#include <Arduino.h>

typedef void (*GlobalCallbackFunction)(const char*, const char*, const char*);
typedef void (*TempCallbackFunction)(const char*, const char*, float);
typedef void (*StatCallbackFunction)(const char*, const char*, bool);
typedef void (*CountLCallbackFunction)(const char*, const char*, uint32_t);
//typedef void (*ModeCallbackFunction)(const char*, const char*, uint8_t);
//typedef void (*HoursCallbackFunction)(const char*, const char*, uint32_t);
//typedef void (*HoursLCallbackFunction)(const char*, const char*, uint32_t);

enum DPType {
  TEMP,
  STAT,
  COUNTL
  //MODE
  //HOURS
  //HOURSL
  //COUNT
};

class Datapoint {
  public:
    Datapoint(const char* name, const char* group, const uint16_t address, bool isWriteable);
    ~Datapoint();
    const char* getName() const;
    const char* getGroup() const;
    const uint16_t getAddress() const;
    const bool isWriteable() const;
    Datapoint& setWriteable();
    void setGlobalCallback(GlobalCallbackFunction globalCallback);

    //virtual methods, see inherited classes for implementation
    virtual const uint8_t getLength() const = 0;
    virtual void callback(uint8_t value[]) = 0;
    virtual void setCallback(GlobalCallbackFunction globalCallback) {};
    virtual Datapoint& setCallback(TempCallbackFunction callback) {};
    virtual Datapoint& setCallback(StatCallbackFunction callback) {};
    virtual Datapoint& setCallback(CountLCallbackFunction callback) {};
    virtual void transform(uint8_t transformedValue[], float value) {};

  protected:  //all properties are protected for ease of use in inherited classes
    static GlobalCallbackFunction _globalCallback;
    const char* _name;
    const char* _group;
    const uint16_t _address;
    bool _writeable;
};

class TempDP : public Datapoint {
  public:
    TempDP(const char* name, const char* group, const uint16_t address, bool isWriteable);
    virtual Datapoint& setCallback(TempCallbackFunction callback);
    virtual const uint8_t getLength() const { return 2; }
    virtual void callback(uint8_t value[]);
    virtual void transform(uint8_t transformedValue[], float value);
  private:
    TempCallbackFunction _callback;
};

class StatDP : public Datapoint {
  public:
    StatDP(const char* name, const char* group, const uint16_t address, bool isWriteable);
    virtual Datapoint& setCallback(StatCallbackFunction callback);
    virtual const uint8_t getLength() const { return 1; }
    virtual void callback(uint8_t value[]);
    virtual void transform(uint8_t transformedValue[], float value);
  private:
    StatCallbackFunction _callback;
};

class CountLDP : public Datapoint {
  public:
    CountLDP(const char* name, const char* group, const uint16_t address, bool isWriteable = false);
    virtual Datapoint& setCallback(CountLCallbackFunction callback);
    virtual const uint8_t getLength() const { return 4; }
    virtual void callback(uint8_t value[]);
    virtual void transform(uint8_t transformedValue[], float value);
  private:
    CountLCallbackFunction _callback;
};

/*
class ModeDP : public Datapoint {
  public:
    ModeDP(const char* name, const char* group, const uint16_t address, bool isWriteable);
    Datapoint& setCallback(ModeCallbackFunction callback);
    virtual void callback(uint8_t value[]);
    virtual const uint8_t getLength() const { return 1; }
  private:
    ModeCallbackFunction _callback;
};

class HoursDP : public Datapoint {
  public:
    HoursDP(const char* name, const char* group, const uint16_t address, bool isWriteable);
    Datapoint& setCallback(HoursCallbackFunction callback);
    virtual void callback(uint8_t value[]);
    virtual const uint8_t getLength() const { return 2; }
  private:
    HoursCallbackFunction _callback;
};

class HoursLDP : public Datapoint {
  public:
    HoursLDP(const char* name, const char* group, const uint16_t address, bool isWriteable);
    Datapoint& setCallback(HoursLCallbackFunction callback);
    virtual void callback(uint8_t value[]);
    virtual const uint8_t getLength() const { return 4; }
  private:
    HoursLCallbackFunction _callback;
};

*/
