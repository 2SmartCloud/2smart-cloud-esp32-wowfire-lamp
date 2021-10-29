#include "mqtt_client.h"

#include "utils.h"

MqttClient::MqttClient() {
    CreateClient();
    session_id_.append(RandomString(millis(), 8));
}

void MqttClient::CreateClient() {
    this->wifi_client_ = new WiFiClient;
    this->client_ = new PubSubClient(*wifi_client_);
}

bool MqttClient::Init(String username, String host, String port, String password, MQTT_CALLBACK_SIGNATURE) {
    this->username_ = username;
    this->host_ = host;
    this->port_ = port;
    this->password_ = password;
    client_->setServer(this->host_.c_str(), this->port_.toInt());
    client_->setCallback(callback);
    return (this->Reconnect());
}

bool MqttClient::Reconnect() {
    if (this->username_ == "") {
        Serial.println("mqtt credentials not setted");
        return false;
    }
    if (millis() - reconnect_mqtt_time_ < kDelayForReconnectMQTT_) return false;
    if (!client_->connected()) {
        if (client_->connect(session_id_.c_str(), this->username_.c_str(), this->password_.c_str())) {
            Serial.println("MQTT connected");
            if (need_reconnect_) mqtt_reconnected_ = true;
            need_reconnect_ = false;
        } else {
            Serial.print("failed, rc=");
            Serial.print(client_->state());
            Serial.println(" try again in 5 seconds");
            need_reconnect_ = true;
            reconnect_mqtt_time_ = millis();
        }
    } else {
        if (need_reconnect_) mqtt_reconnected_ = true;
        need_reconnect_ = false;
    }
    return (!need_reconnect_);
}

bool MqttClient::Subscribe(String topic) {  // susbcribe 1 topic
    return client_->subscribe(topic.c_str());
}

bool MqttClient::Publish(String topic, String payload, bool retained = 1) {  // publish 1 topic
    if (!client_->connected()) return false;
    if (client_->publish(topic.c_str(), payload.c_str(), retained)) {
        return true;
    } else {
        Serial.printf("Err publishing %s\r\n", topic.c_str());
        return false;
    }
}

void MqttClient::MqttLoop() {
    if (!client_->connected()) Reconnect();
    client_->loop();
}
