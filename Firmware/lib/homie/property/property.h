#pragma once
#include <Arduino.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "device/device.h"
#include "homie.h"
#include "node/node.h"

enum PROPERTY_TYPE { OPTION, TELEMETRY, SENSOR };

class Property {
 public:
    // ----------------------------Constructor for Node`s properties
    Property(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained,
             const char* data_type, String format = "", String unit = "");

    // ----------------------------Constructor for Device`s properties
    Property(const char* name, const char* id, Device* device, PROPERTY_TYPE type, bool settable, bool retained,
             const char* data_type, String format = "", String unit = "");

    ~Property() { Serial.printf("Property %s destroyed\r\n", name_.c_str()); }

    virtual bool Init(Homie* homie);

    String GetValue() const;
    String GetId() const;
    PROPERTY_TYPE GetType() const;
    Node* GetNode() const;
    Device* GetDevice() const;

    bool HasNewValue();

    void SetValue(String value);
    void SetHasNewValue(bool has_new_value);

    virtual void HandleCurrentState();

    virtual void HandleSettingNewValue();

 protected:
    PROPERTY_TYPE type_;

    bool has_new_value_ = false;
    bool settable_;
    bool retained_;

    String id_;
    String name_;
    String data_type_;
    String format_;
    String unit_;
    String value_;

    Node* node_ = nullptr;
    Device* device_ = nullptr;
    Homie* homie_ = nullptr;
};
