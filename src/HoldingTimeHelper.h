//
// Created by V on 10.07.2024.
//
#ifndef UNTITLED4_HOLDINGTIMEHELPER_H
#define UNTITLED4_HOLDINGTIMEHELPER_H
#include <Arduino.h>

class HoldingTimeHelper {
    bool isHoldingStarted = false;
    unsigned holdingStartTime;

public:
    void setHoldingStartTime() {
        if (isHoldingStarted)return;
        holdingStartTime = millis();
        isHoldingStarted = true;
    }

public:
    void resetHolding() {
        isHoldingStarted = false;
    }

public:
    bool isHoldingForTime(long time) {
        bool res = holdingStartTime + time <= millis() && isHoldingStarted;
        return res;
    }
};

#endif //UNTITLED4_HOLDINGTIMEHELPER_H
