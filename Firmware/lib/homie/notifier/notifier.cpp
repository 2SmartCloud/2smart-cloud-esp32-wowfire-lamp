#include "notifier.h"

Notifier::Notifier(MqttClient* mqtt_client) {
    mqtt_client_ = mqtt_client;
    Serial.println("Notifier object init");
}

void Notifier::SetUserHash(String user_hash) {
    if (user_hash.length() == kUserHashLength) user_hash_ = user_hash;
}

bool Notifier::CreateNotification(String text) {
    if (user_hash_.length() == kUserHashLength) {
        Serial.print("notif ");
        String topic = user_hash_;
        topic += "/notifications/create";
        Serial.print(topic);
        Serial.print(" : ");
        Serial.println(text);
        return mqtt_client_->Publish(topic.c_str(), text.c_str(), false);
    }
    return false;
}
