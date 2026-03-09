//
// Created by V on 09.07.2024.
//

#ifndef UNTITLED4_LIGHTCONTROL_H
#define UNTITLED4_LIGHTCONTROL_H
#include <WString.h>
#include <HardwareSerial.h>
#include <Arduino.h>

class LightControl {
private:
    bool isLightOn;
private:
    byte pin;
private:
    String name;


public:
    LightControl(int pin, const String &name) : isLightOn(false), pin(pin), name(name) {
        this->pin = pin;
        this->name = name;
    }

    LightControl() {}

    void setPin(byte pinNum) {
        pin = pinNum;
    }

    void setName(const String &n) {
        name = n;
    }

public:
    bool isOn() const {
        return isLightOn;
    }

public:
    bool isOff() const {
        return !isLightOn;
    }

public:
    void turnOn() {
        if (!isLightOn) Serial.println(name + " On");
        isLightOn = true;
        digitalWrite(pin, LOW);
    }

public:
    void turnOff() {
        if (isLightOn) {
            Serial.println(name + " Off");
            digitalWrite(pin, HIGH);
        }
        isLightOn = false;
    }

public:
    void changeState() {
        if (isLightOn)
            turnOff();
        else
            turnOn();
    }
};

#endif //UNTITLED4_LIGHTCONTROL_H
