#pragma once
#include <Arduino.h>

class TimeClient {
 public:
    TimeClient();
    virtual void Init() = 0;

    virtual uint8_t GetHours() = 0;
    virtual uint8_t GetMinutes() = 0;

 protected:
    uint8_t kTimeZone = 3;  // Kiev +3
    const uint16_t kUpdateInterval = 55 * 1000;
};
