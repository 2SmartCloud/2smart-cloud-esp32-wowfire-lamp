#include "notifications.h"

#include "file_system.h"

Notifications::Notifications(const char* name, const char* id, Device* device) : Node(name, id, device) {}

bool Notifications::Init(Homie* homie) {  // initialize toggles for notification
    bool status;
    if (!Node::Init(homie)) status = true;
    if (!LoadNotifySettings()) status = true;
    return status;
}

void Notifications::HandleCurrentState() {
    if (millis() - period_loop_ > 10000) {
        Serial.println("notify node loop");

        if (properties_.find("system")->second->HasNewValue()) {
            notifySettings.system_ = properties_.find("system")->second->GetValue() == "true";
            properties_.find("system")->second->SetHasNewValue(false);
            SaveNotifySettings();
            // logic
        }

        if (properties_.find("update")->second->HasNewValue()) {
            notifySettings.update_ = properties_.find("update")->second->GetValue() == "true";
            properties_.find("update")->second->SetHasNewValue(false);
            SaveNotifySettings();
            // logic
        }

        period_loop_ = millis();
    }
}

bool Notifications::LoadNotifySettings() {
    ReadSettings("/notifyconf.txt", reinterpret_cast<byte*>(&notifySettings), sizeof(notifySettings));

    String state_flag = notifySettings.system_ ? "true" : "false";
    properties_.find("system")->second->SetValue(state_flag);
    properties_.find("system")->second->SetHasNewValue(false);

    state_flag = notifySettings.update_ ? "true" : "false";
    properties_.find("update")->second->SetValue(state_flag);
    properties_.find("update")->second->SetHasNewValue(false);

    return true;
}

bool Notifications::SaveNotifySettings() {
    return WriteSettings("/notifyconf.txt", reinterpret_cast<byte*>(&notifySettings), sizeof(notifySettings));
}
