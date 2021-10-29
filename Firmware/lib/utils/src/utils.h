#pragma once
#include <string>

#include "Arduino.h"

std::string RandomString(uint8_t seed, uint8_t length);

String TimeToStr(uint8_t hours, uint8_t minutes);

String Sha256(String data);
