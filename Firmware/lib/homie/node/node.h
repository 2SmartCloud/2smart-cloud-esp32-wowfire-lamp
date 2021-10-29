#pragma once
#include <Arduino.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "device/device.h"
#include "homie.h"
#include "property/property.h"

class Node {
 public:
    Node(const char* name, const char* id, Device* device);
    ~Node() { Serial.println("Node destroyed"); }

    virtual bool Init(Homie* homie);

    void AddProperty(Property* property);

    String GetId() const;
    Device* GetDevice() const;
    Property* GetProperty(String id);

    virtual void HandleCurrentState();

 protected:
    String name_;
    String state_ = "ready";
    String id_;

    Device* device_ = nullptr;
    Homie* homie_ = nullptr;

    std::map<String, Property*> properties_ = {};
};
