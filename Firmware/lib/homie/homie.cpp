#include "homie.h"

#include "device/device.h"
#include "node/node.h"
#include "property/property.h"

Homie::Homie(MqttClient* mqtt_client) {
    mqtt_client_ = mqtt_client;
    Serial.println("Homie object init");
}

bool Homie::Init(String user_hash, String host, String broker_port, String token, MQTT_CALLBACK_SIGNATURE) {
    if (millis() - reinit_homie_time_ < kDelayForReconnectHomie) return false;
    bool status = true;
    this->user_hash_ = user_hash;
    if (!mqtt_client_->Init(user_hash_, host, broker_port, token, callback)) status = false;
    if (device_ == nullptr) {
        Serial.println("Device not setted in homie");
        status = false;
    }
    if (status) device_->Init();
    Serial.printf("Homie init %s \r\n", (status) ? "success" : "failed");
    reinit_homie_time_ = millis();

    return status;
}

void Homie::SetDevice(Device* device) { this->device_ = device; }

bool Homie::Publish(const Property& property, String attribute, String value, bool retained) {
    String topic = this->GetTopicForEntity(property);
    if (attribute != "") {
        topic += "/$";
    }

    return this->mqtt_client_->Publish(topic + attribute, value, retained);
}

bool Homie::Publish(const Node& node, String attribute, String value, bool retained) {
    String topic = this->GetTopicForEntity(node, attribute);

    return this->mqtt_client_->Publish(topic, value, retained);
}

bool Homie::Publish(const Device& device, String attribute, String value, bool retained) {
    String topic = this->GetTopicForEntity(device, attribute);

    return mqtt_client_->Publish(topic, value, retained);
}

bool Homie::SubscribeTopic(const Property& property) {
    String topic = this->GetTopicForEntity(property);
    return this->mqtt_client_->Subscribe(topic + "/set");
}

String Homie::GetTopicForEntity(const Device& device, String attribute) {
    String topic = this->user_hash_;
    topic += "/sweet-home/";
    topic += device.GetId() + '/';
    if (attribute != "") {
        topic += "$";
    }
    topic += attribute;
    return topic;
}

String Homie::GetTopicForEntity(const Node& node, String attribute) {
    Device* device = node.GetDevice();

    String topic = this->user_hash_;
    topic += "/sweet-home/";
    topic += device->GetId() + "/";
    topic += node.GetId() + '/';
    if (attribute != "") {
        topic += "$";
    }
    topic += attribute;
    return topic;
}

String Homie::GetTopicForEntity(const Property& property) {
    Node* node = property.GetNode();

    Device* device = node ? node->GetDevice() : property.GetDevice();

    String topic = user_hash_ + "/sweet-home/";
    topic += device->GetId() + "/";

    if (node) {
        topic += node->GetId() + '/';
    }

    switch (property.GetType()) {
        case OPTION:
            topic += "$option/";
            topic += property.GetId();
            break;
        case TELEMETRY:
            topic += "$telemetry/";
            topic += property.GetId();
            break;
        case SENSOR:
            topic += property.GetId();
            break;
    }

    return topic;
}

void Homie::HandleMessage(String topic, byte* payload, unsigned int length) {
    Serial.print("Message handled by homie : ");
    topic.replace(user_hash_ + "/sweet-home/", "");
    topic.replace(device_->GetId() + "/", "");

    // found node
    uint8_t index_slash = topic.indexOf("/");
    String node_id = topic.substring(0, index_slash);
    Node* node = device_->GetNode(node_id);
    Serial.printf("node is: %s\r\n", node->GetId().c_str());

    // found property
    uint8_t index_slash2 = topic.indexOf("/", ++index_slash);
    String property_id = topic.substring(index_slash, index_slash2);
    Property* property = node->GetProperty(property_id);
    Serial.printf("property is: %s\r\n", property->GetId().c_str());

    String value;
    for (int i = 0; i < length; i++) {
        value += static_cast<char>(payload[i]);  // ------------ write payload
    }

    property->SetValue(value);
}

void Homie::HandleCurrentState() {
    mqtt_client_->MqttLoop();
    device_->HandleCurrentState();
}
