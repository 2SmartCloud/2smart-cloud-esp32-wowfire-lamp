#include "gpio.h"

uint64_t lastClickButton = 0;
hw_timer_t* timer = nullptr;

void IRAM_ATTR onTimer() {
    if (!digitalRead(ERASE_FLASH)) {
        erase_flag = true;
    } else {
        timerAlarmDisable(timer);
    }
}

void IRAM_ATTR eraseInterrupt() {
    timer = timerBegin(0, 80, true);  // 80=prescaller 80Mhz/prescaller = 1 000 000 counts in a sec
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, RELOAD_TIME * 1000000, false);  // timer overflow in (reload time) seconds
    timerAlarmEnable(timer);
}

void setGpios() {
    pinMode(ERASE_FLASH, INPUT_PULLUP);
    attachInterrupt(ERASE_FLASH, eraseInterrupt, FALLING);
}
