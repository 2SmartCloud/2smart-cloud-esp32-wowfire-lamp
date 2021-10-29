#pragma once
#include <Arduino.h>

#include "mqtt_client.h"

class Notifier {
 public:
    explicit Notifier(MqttClient* mqtt_client);
    ~Notifier() { Serial.println("Notifier destroyed"); }

    bool CreateNotification(String text);
    void SetUserHash(String user_hash);

 private:
    const uint8_t kUserHashLength = 64;

    String user_hash_ = "";
    String device_id_;

    MqttClient* mqtt_client_ = nullptr;
};
