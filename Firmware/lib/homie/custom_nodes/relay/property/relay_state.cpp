#include "relay_state.h"

RelayState::RelayState(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained,
                       const char* data_type)
    : Property(name, id, node, type, settable, retained, data_type) {
    pinMode(kRelayPin, OUTPUT);
}

bool RelayState::Init(Homie* homie) {  // initialize toggles for notification
    bool status = true;
    if (!Property::Init(homie)) status = false;
    return status;
}

void RelayState::HandleCurrentState() {
    button_.tick();
    if (button_.isPress()) SetValue(value_ == "true" ? "false" : "true");
}

void RelayState::HandleSettingNewValue() {
    Serial.println(value_ == "true" ? "Relay on" : "Relay off");
    digitalWrite(kRelayPin, value_ == "true");

    Device* device = node_->GetDevice();
    bool is_sys_notif_enabled = device->IsSysNotifyEnabled();
    if (is_sys_notif_enabled) device->SendNotification(value_ == "true" ? "Relay on" : "Relay off");
}
