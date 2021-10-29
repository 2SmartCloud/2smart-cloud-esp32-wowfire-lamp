#include "device.h"

#include <ArduinoJson.h>

#include <utility>

const uint16_t kHeartbeatDelay = 9 * 1000;  // sec

Device::Device(Homie *homie) { homie_ = homie; }

void Device::SetCredentials(DeviceData device_data) {
    name_ = device_data.name;
    fw_ver_ = device_data.fw_ver;
    fw_name_ = device_data.fw_name;
    ip_ = device_data.ip;
    impl_ = device_data.impl;
    mac_ = device_data.mac;
    state_ = device_data.state;
    id_ = device_data.id;
    Serial.print("Device id ");
    Serial.println(id_);
}

bool Device::Init() {
    Serial.println("init device");
    bool status = true;

    if (!homie_->Publish(*this, "name", name_, true)) status = false;

    if (!homie_->Publish(*this, "fw/name", fw_name_, true)) status = false;

    if (!homie_->Publish(*this, "fw/version", fw_ver_, true)) status = false;

    if (!homie_->Publish(*this, "localip", ip_, true)) status = false;

    if (!homie_->Publish(*this, "implementation", impl_, true)) status = false;

    if (!homie_->Publish(*this, "mac", mac_, true)) status = false;

    if (!homie_->Publish(*this, "state", state_, true)) status = false;

    Serial.println("base published");

    if (!InitProperties()) status = false;

    if (!InitNodes()) status = false;

    Serial.printf("Device init %s \r\n", (status) ? "success" : "failed");
    need_reinit_ = !status;
    return status;
}

void Device::AddNode(Node *node) { nodes_.insert(std::pair<String, Node *>(node->GetId(), node)); }

void Device::AddProperty(Property *property) {
    properties_.insert(std::pair<String, Property *>(property->GetId(), property));
}

bool Device::InitNodes() {
    bool status = true;
    for (auto it = begin(nodes_); it != end(nodes_); it++) {
        if (!(*it->second).Init(homie_)) {
            status = false;
            Serial.println("Node init failed");
        }
    }
    Serial.println("Nodes init success");

    return status;
}

bool Device::InitProperties() {
    bool status = true;
    for (auto it = begin(properties_); it != end(properties_); it++) {
        if (!(*it->second).Init(homie_)) {
            status = false;
            Serial.println("Propertie init failed");
        }
    }
    if (status) Serial.println("Properties init success");

    return status;
}

String Device::GetId() const { return id_; }

String Device::GetFirmwareVersion() const { return fw_ver_; }
Node *Device::GetNode(String id) { return nodes_.find(id)->second; }

void Device::HandleCurrentState() {
    // device logic + call Node::Loop for each node
    if (millis() - last_millis_heartbeat > kHeartbeatDelay) {
        if (need_reinit_) Init();
        SendHeartbeat();
        last_millis_heartbeat = millis();
    }
    for (auto it = begin(nodes_); it != end(nodes_); it++) {
        (*it->second).HandleCurrentState();
    }
    for (auto it = begin(properties_); it != end(properties_); it++) {
        (*it->second).HandleCurrentState();
    }
}

void Device::SendHeartbeat() {
    Serial.print("heartbeat ");

    if (homie_->Publish(*this, "heartbeat", "", true)) {
        Serial.println("success");
    } else {
        Serial.println("failed");
        need_reinit_ = true;
    }
}

void Device::SetNotifier(Notifier *notifier) { notifier_ = notifier; }
bool Device::SendNotification(String text) {
    StaticJsonDocument<256> doc;
    doc["logLevel"] = "info";
    doc["senderId"] = id_;
    doc["message"] = text;
    doc["type"] = "text";
    doc["senderType"] = "device";
    String response;
    serializeJson(doc, response);
    return notifier_->CreateNotification(response);
}

bool Device::IsSysNotifyEnabled() {
    Property *property = nodes_.find("notifications")->second->GetProperty("system");
    return property->GetValue() == "true";
}

bool Device::IsFwNotifyEnabled() {
    Property *property = nodes_.find("notifications")->second->GetProperty("update");
    return property->GetValue() == "true";
}
