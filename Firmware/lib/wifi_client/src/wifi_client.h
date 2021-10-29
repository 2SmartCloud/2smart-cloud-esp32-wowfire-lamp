#pragma once
#include <Arduino.h>
#include <WiFi.h>

class WifiClient {
 public:
    WifiClient() {}
    ~WifiClient() { Serial.println("WifiClient destroyed"); }

    void SetCredentials(String ssid_name, String ssid_password);

    bool Connect();

 private:
    const uint8_t kLedStatus_ = 2;
    const uint8_t kBlinkingPairingDelay_ = 250;  // ms

    const uint16_t kWifiReconnectDelay_ = 10 * 1000;  // sec

    uint32_t wifi_reconnect_time_ = 0;
    uint32_t last_blink_time_ = 0;

    String ssid_name_;
    String ssid_password_;
};
