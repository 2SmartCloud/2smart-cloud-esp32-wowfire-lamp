#pragma once

#include <Arduino.h>

#include <map>
#include <string>

#include "node/node.h"

class Notifications : public Node {
 public:
    Notifications(const char* name, const char* id, Device* device);

    bool Init(Homie* homie);

    void HandleCurrentState();

    bool LoadNotifySettings();
    bool SaveNotifySettings();

 private:
    typedef struct {
        bool system_;
        bool update_;
    } NotificationSettings;

    uint32_t period_loop_ = millis();

    NotificationSettings notifySettings = {true, true};
};
