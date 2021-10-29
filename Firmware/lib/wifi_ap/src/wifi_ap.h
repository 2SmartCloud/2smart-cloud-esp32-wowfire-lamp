#pragma once
#include <Arduino.h>
#include <WiFi.h>

class WifiAp {
 public:
    WifiAp();
    ~WifiAp() { Serial.println("WifiAp destroyed"); }

    void Start(String ssid_name);

    void Blink();

 private:
    const uint8_t kLedStatus_ = 2;
    const uint16_t kBlinkingTimeAPMode_ = 1000;  // once in N msec
    uint32_t last_blink_AP_ = 0;                 // millis();
};
