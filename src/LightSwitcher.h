//
// Created by V on 09.07.2024.
//

#ifndef UNTITLED4_LIGHTSWITCHER_H
#define UNTITLED4_LIGHTSWITCHER_H


#include <Arduino.h>
#include "GyverButton.h"
#include "LightControl.h"
#include "GlobalPins.h"
#include "GRGB.h"


class LightSwitcher {


    LightControl lowLight;
    LightControl highLight;
    LightControl gabLight;
    GButton switchBtn;
    GButton lowBtn;
    GButton shineOrBlinkBtn;
    GButton gabBtn;
    GRGB lightLed; // куда подключены цвета (R, G, B)
public:


    explicit LightSwitcher(const GRGB &lightLed) : lightLed(lightLed) {}

    GButton switch_btn() const {
        return switchBtn;
    }

    void setUp() {

        lowLight.setPin(outputLowLight);
        lowLight.setName("Low Light");
        highLight.setPin(outputHighLight);
        highLight.setName("High Light");
        gabLight.setPin(outputGab);
        gabLight.setName("Gab Light");

        GButton sButton(inputSwitch, HIGH_PULL, NORM_OPEN);
        GButton lButton(inputLowLight, HIGH_PULL, NORM_OPEN);
        GButton allButton(inputAllLightOn, HIGH_PULL, NORM_OPEN);
        GButton gabButton(inputGab, HIGH_PULL, NORM_OPEN);


        switchBtn = sButton;
        lowBtn = lButton;
        shineOrBlinkBtn = allButton;
        gabBtn = gabButton;

        switchBtn.setTickMode(AUTO);
        lowBtn.setTickMode(AUTO);
        shineOrBlinkBtn.setTickMode(AUTO);
        gabBtn.setTickMode(AUTO);

        lowBtn.setTimeout(500);
        lowBtn.setDebounce(200);
        shineOrBlinkBtn.setTimeout(500);
        shineOrBlinkBtn.setDebounce(200);
        gabBtn.setTimeout(500);
        gabBtn.setDebounce(200);
        switchBtn.setDebounce(100);
        switchBtn.setTimeout(400);

        delay(1000);
        switchBtn.resetStates();
        lowBtn.resetStates();
        shineOrBlinkBtn.resetStates();
        gabBtn.resetStates();
        delay(1000);
        switchBtn.tick();
        lowBtn.tick();
        shineOrBlinkBtn.tick();
        gabBtn.tick();

        lightLed.setBrightness(255);
        lightLed.fadeMode(true);
        lightLed.setCRT(true);
        lightLed.setFadePeriod(1000);

    }


    unsigned long blinkStartTime = millis();

     byte blinkStep = 0;
private:
    bool blink(int blinkTimeout) {
        lightLed.fadeMode(false);
        lightLed.setCRT(false);
        if (digitalRead(inputSkull) == 0)return false;

        if (blinkStartTime + blinkTimeout > millis())return false;
        // static bool blinkStep = true;


        switch (blinkStep) {
            case 0:
                lowLight.turnOff();
                highLight.turnOff();
                lightLed.disable();
                break;
            case 1:
                lowLight.turnOn();
                highLight.turnOff();
                lightLed.setColor(GGreen);
                break;
            case 2:
                lowLight.turnOff();
                highLight.turnOn();
                lightLed.setColor(GBlue);
                break;
            default:;
        }

        blinkStep++;
        if (blinkStep == 3)blinkStep = 0;
        blinkStartTime = millis();
        return true;
    }

private:
    void blinkManyTimes() {
        for (int i = 0; i < 10;) {
            if (blink(90))i++;
        }
    }

private:
    void shine(int timeOut) {
        lightLed.fadeMode(false);
        lightLed.setCRT(false);
        lightLed.setColor(GBlue);
        lowLight.turnOff();
        highLight.turnOn();
        delay(timeOut);
        if (switchBtn.isHold())return;
        if (shouldTurnOnLow())lowLight.turnOn();
        else lowLight.turnOff();
        switchBtn.tick();
        if (!switchBtn.isHold())
            highLight.turnOff();
    }


private:
    bool shouldTurnOnLow() {
        return lowBtn.state();
    }

private:
    bool shouldShine() {
        return shineOrBlinkBtn.state();
    }

private:
    bool shouldBlink() {
        return !shineOrBlinkBtn.state();
    }


public:
    void controlTheLight() {
        // Input 1 == OFF 0 == ON
        // output  //HIGH == OFF LOW == ON #define HIGH 0x1 #define LOW  0x0


        lightLed.tick();
        changeColorsMode();
        if (gabBtn.state()) {
            gabLight.turnOn();
        } else {
            gabLight.turnOff();
        }


        if (shouldShine()) {
            if (switchBtn.isHold()) {
                shine(30);
                return;
            }

            if ( switchBtn.isPress() ||switchBtn.isRelease() || switchBtn.isClick()) {
                shine(150);
                return;
            }
            if (shouldTurnOnLow() && lowLight.isOff())
                lowLight.turnOn();
            else if (!shouldTurnOnLow())lowLight.turnOff();
            return;
        }

        if (shouldBlink()) {

            if (!shouldTurnOnLow()) {

                if (switchBtn.isHold()) {

                    blink(130);
                    return;
                }

                if ( switchBtn.isRelease() || switchBtn.isClick()) {
                    blinkManyTimes();
                    return;
                }

                lowLight.turnOff();
                highLight.turnOff();
            } else {
                static bool wasHold = false;
                if (switchBtn.isHold()) {

                    blink(120);
                    wasHold = true;
                    return;
                }

                if (wasHold) {
                    lowLight.turnOn();
                    highLight.turnOff();
                    wasHold = false;
                    delay(200);
                    changeColorsMode();
                    return;
                }

                if (switchBtn.isPress() || switchBtn.isRelease() || switchBtn.isClick()) {
                    lowLight.changeState();
                    highLight.changeState();
                    return;
                }
                if (shouldTurnOnLow() && lowLight.isOff() && highLight.isOff())
                    lowLight.turnOn();

            }


        }
    }


    uint32_t tmr = 0;

private:
    void changeColorsMode() {

        lightLed.tick();
        if (switchBtn.isHold()){
            lightLed.fadeMode(false);
            lightLed.setCRT(false);
            return;
        }
        if (lowLight.isOff() && highLight.isOff() && gabLight.isOff()) {
            lightLed.disable();
            return;
        }
        lightLed.fadeMode(true);
    //    lightLed.setCRT(true);
        lightLed.setFadePeriod(300);
        if (millis() - tmr >= 400) {
            tmr = millis();


            if (lowLight.isOn()) {
                lightLed.setWheel8(random(80, 120),(int) random(200,255));
                return;
            }
            if (highLight.isOn()) {
                lightLed.setWheel8(random(130, 170),(int) random(200,255));
                return;
            }
            if (gabLight.isOn() && !shouldTurnOnLow() && lowLight.isOff()) {
                lightLed.setWheel8(random(200,250),(int) random(200,255));
                return;
            }
        }
    }
};


#endif //UNTITLED4_LIGHTSWITCHER_H
