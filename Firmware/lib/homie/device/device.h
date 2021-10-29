#pragma once
#include <Arduino.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "homie.h"
#include "node/node.h"
#include "notifier/notifier.h"
#include "property/property.h"

typedef struct {
    const char* name;
    const char* fw_ver;
    const char* fw_name;
    const char* ip;
    const char* impl;
    const char* mac;
    const char* state;
    const char* id;
} DeviceData;

class Device {
 public:
    explicit Device(Homie* homie);
    ~Device() { Serial.println("Device destroyed"); }

    void SetCredentials(DeviceData device_data);
    bool Init();

    void AddNode(Node* node);
    void AddProperty(Property* property);

    String GetId() const;
    String GetFirmwareVersion() const;
    Node* GetNode(String id);

    void SetNotifier(Notifier* notifier);

    void HandleCurrentState();

    bool SendNotification(String text);

    bool IsSysNotifyEnabled();
    bool IsFwNotifyEnabled();

 private:
    bool InitNodes();
    bool InitProperties();

    void SendHeartbeat();

    bool need_reinit_ = false;

    uint32_t last_millis_heartbeat = 0;

    String name_;
    String fw_ver_;
    String fw_name_;
    String ip_;
    String impl_;
    String mac_;
    String state_;
    String id_;

    Homie* homie_;
    Notifier* notifier_ = nullptr;

    std::map<String, Node*> nodes_ = {};
    std::map<String, Property*> properties_ = {};
};
