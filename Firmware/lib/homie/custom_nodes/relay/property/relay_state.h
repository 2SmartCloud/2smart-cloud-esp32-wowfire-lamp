#pragma once

#include <Arduino.h>
#include <EncButton.h>

#include <map>
#include <string>

#include "property/property.h"

class RelayState : public Property {
 public:
    RelayState(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained,
               const char* data_type);

    bool Init(Homie* homie);

    void HandleCurrentState();

    void HandleSettingNewValue();

 private:
    EncButton<EB_TICK, -1> button_;

    const uint8_t kRelayPin = 4;

    uint32_t period_loop_ = millis();
};
