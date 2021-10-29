#pragma once
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "time_client.h"

class NtpTimeClient : public TimeClient {
 public:
    NtpTimeClient();
    void Init();

    uint8_t GetHours();
    uint8_t GetMinutes();

 private:
    WiFiUDP* udp_client_ = nullptr;
    NTPClient* ntp_client_ = nullptr;
    void CreateClients();
};
