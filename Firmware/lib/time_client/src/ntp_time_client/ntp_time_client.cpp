#include "ntp_time_client.h"

NtpTimeClient::NtpTimeClient() { CreateClients(); }

void NtpTimeClient::Init() {
    ntp_client_->begin();
    ntp_client_->setUpdateInterval(kUpdateInterval);
    ntp_client_->setTimeOffset(kTimeZone * 60 * 60);  // time zone * 60min * 60 sec
}

uint8_t NtpTimeClient::GetHours() {
    ntp_client_->update();
    return ntp_client_->getHours();
}

uint8_t NtpTimeClient::GetMinutes() {
    ntp_client_->update();
    return ntp_client_->getMinutes();
}

void NtpTimeClient::CreateClients() {
    this->udp_client_ = new WiFiUDP;
    this->ntp_client_ = new NTPClient(*udp_client_);
}
