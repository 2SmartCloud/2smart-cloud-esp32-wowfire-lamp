#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <functional>
#include <map>
#include <string>

class MqttClient {
 public:
    MqttClient();
    ~MqttClient() { Serial.println("MqttClient destroyed"); }

    bool Init(String username, String host, String port, String password, MQTT_CALLBACK_SIGNATURE);

    void MqttLoop();
    bool Subscribe(String topic);
    bool Publish(String topic, String value, bool retained);

 private:
    const uint16_t kDelayForReconnectMQTT_ = 5 * 1000;  // 5 sec

    bool need_reconnect_ = false;
    bool mqtt_reconnected_ = false;
    uint32_t reconnect_mqtt_time_ = 0;

    String username_;
    String host_;
    String port_;
    String password_;
    std::string session_id_ = "session_";

    WiFiClient *wifi_client_;
    PubSubClient *client_;

    bool Reconnect();

    void CreateClient();
};
