#include "property.h"

Property::Property(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained,
                   const char* data_type, String format, String unit) {
    this->name_ = name;
    this->id_ = id;
    this->node_ = node;
    this->type_ = type;
    this->settable_ = settable;
    this->retained_ = retained;
    this->data_type_ = data_type;
    this->format_ = format;
    this->unit_ = unit;
    Serial.print("Created property: ");
    Serial.println(name_);
}

Property::Property(const char* name, const char* id, Device* device, PROPERTY_TYPE type, bool settable, bool retained,
                   const char* data_type, String format, String unit) {
    this->name_ = name;
    this->id_ = id;
    this->device_ = device;
    this->type_ = type;
    this->settable_ = settable;
    this->retained_ = retained;
    this->data_type_ = data_type;
    this->format_ = format;
    this->unit_ = unit;
    Serial.print("Created property: ");
    Serial.println(name_);
}

bool Property::Init(Homie* homie) {
    Serial.print("start init property ||");
    Serial.print(this->name_);

    bool status = true;
    homie_ = homie;
    Serial.println("||");

    if (!homie->Publish(*this, "name", name_, true)) status = false;
    if (!homie->Publish(*this, "settable", settable_ ? "true" : "false", true)) status = false;
    if (!homie->Publish(*this, "retained", retained_ ? "true" : "false", true)) status = false;
    if (!homie->Publish(*this, "datatype", data_type_, true)) status = false;
    if (format_)
        if (!homie->Publish(*this, "format", format_, true)) status = false;
    if (unit_)
        if (!homie->Publish(*this, "unit", unit_, true)) status = false;

    if (!status) Serial.printf("Init property %s failed\r\n", name_.c_str());

    if (settable_) homie->SubscribeTopic(*this);
    return status;
}

void Property::SetValue(String value) {
    Serial.print("Message handled by property: ");
    Serial.println(value);

    this->has_new_value_ = value != value_;

    if (this->has_new_value_) {
        this->value_ = value;

        HandleSettingNewValue();
    }
    if (homie_ != nullptr) homie_->Publish(*this, "", value_, retained_);
}

void Property::HandleSettingNewValue() {}
String Property::GetValue() const { return value_; }

String Property::GetId() const { return id_; }

PROPERTY_TYPE Property::GetType() const { return this->type_; }

Node* Property::GetNode() const { return node_; }
Device* Property::GetDevice() const { return device_; }

void Property::HandleCurrentState() {}

bool Property::HasNewValue() { return has_new_value_; }

void Property::SetHasNewValue(bool has_new_value) { has_new_value_ = has_new_value; }
