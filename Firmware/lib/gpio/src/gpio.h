#pragma once
#include <Arduino.h>

const uint8_t LED = 4;
const uint8_t ERASE_FLASH = 15;
const uint8_t RELOAD_TIME = 2;  // sec
extern bool erase_flag;

void setGpios();
void IRAM_ATTR onTimer();
void IRAM_ATTR buttonInterrupt();
void IRAM_ATTR eraseInterrupt();
