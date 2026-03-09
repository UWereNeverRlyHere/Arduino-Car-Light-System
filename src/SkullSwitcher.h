//
// Created by V on 09.07.2024.
//

#ifndef UNTITLED4_SKULLSWITCHER_H
#define UNTITLED4_SKULLSWITCHER_H

#include "LightControl.h"
#include "GyverButton.h"
#include "GRGB.h"
#include "GlobalPins.h"
#include "EEPROM.h"
#include "HoldingTimeHelper.h"

class SkullSwitcher {

    LightControl skullLight;
    GButton skullButton;
    //HoldingTimeHelper holdingTimeHelper;
    GButton switchBtn;
public:
    void set_switch_btn(const GButton &switch_btn) {
        switchBtn = switch_btn;
    }

private:
    byte skullPressCount = 0;
    bool isReStartingSkull = false;
    bool isLedTurnedOff = true;
    bool isFirstStart = true;
    unsigned long skullRestartTime = millis();

    GRGB skullLed; // куда подключены цвета (R, G, B)
public:
    explicit SkullSwitcher(const GRGB &skullLed) : skullLed(skullLed) {
    }

public:
    void setUp() {
        skullLight.setPin(outputSkull);
        skullLight.setName("Skull");
        GButton a(inputSkull, HIGH_PULL, NORM_OPEN);
        skullButton = a;
        //skullButton.setTickMode(AUTO);
        skullButton.setDebounce(150);
        skullButton.setTimeout(1200);
        skullButton.resetStates();
        delay(350);


        skullLed.setBrightness(255);
        // skullLed.enable();
        //  skullLed.setColor(GRed);
        EEPROM.get(0, skullPressCount);
        if (skullPressCount >= 10)
            skullPressCount = 0;
        isFirstStart = true;
        isLedTurnedOff = true;
    }

private:
    void restartSkull() {
        if (isFirstStart)return;

        if (skullRestartTime + 500 > millis()) {
            return;
        }
        if (skullLight.isOn()) {
            skullLight.turnOff();
            isReStartingSkull = true;
            skullRestartTime = millis();
        } else {
            skullLight.turnOn();
            isReStartingSkull = false;
        }
    }

private:
    void turnOffLed() {
        skullLight.turnOff();
        skullLed.disable();
        turnOnOffFadeMode(false);
        isReStartingSkull = false;

        //  skullRestartTime = millis();
        isLedTurnedOff = true;
        Serial.println("Turning off");
    }

private:
    void turnOnRedLight() {
        if (isFirstStart)return;

        EEPROM.get(0, skullPressCount);
        if (skullPressCount == 2) {
            changeLedColors();
            return;
        }
        //  skullLight.turnOff();

        while (true) {
            if (isReStartingSkull) {
                restartSkull();
                delay(350);
                continue;
            }
            restartSkull();
            if (skullPressCount + 1 >= 10)
                skullPressCount = 1;
            else skullPressCount++;
           long color =  random(10, 256);
            Serial.print("Color is: ");
            Serial.println(color);
            skullLed.setWheel8(color);
            delay(350);
            if (skullPressCount == 2)break;
        }

        EEPROM.put(0, skullPressCount);
        isLedTurnedOff = false;
    }

public:
    void controlTheSkull() {

        skullButton.tick();
        skullLed.tick();

        if (isReStartingSkull) {
            restartSkull();
            return;
        }
        if (skullButton.isHold()) {
            // holdingTimeHelper.setHoldingStartTime();
            if (digitalRead(inputAllLightOn) == 0) {
                turnOnRedLight();
            } else {
                turnOffLed();
                if (switchBtn.isClick() || (switchBtn.isPress() || switchBtn.isRelease())) {
                    skullLight.changeState();
                    delay(500);
                }

            }
            return;
        }
        if (skullButton.isClick() ) {

            EEPROM.get(0, skullPressCount);

            if (skullPressCount + 1 >= 10)
                skullPressCount = 0;
            else {
                isFirstStart = false;
                skullPressCount++;
                restartSkull();
            }
            EEPROM.put(0, skullPressCount);

            isLedTurnedOff = false;
            Serial.println(skullPressCount);
            //  holdingTimeHelper.resetHolding();
        }

        if (!isLedTurnedOff) {
            changeLedColors();
        }
    }

private:
    void changeLedColors() {
        if (isFirstStart)return;
        skullLed.tick();
        switch (skullPressCount) {
            case 0:
                turnOffLed();
                break;
            case 1:
                skullLed.setColor(GNavy);
                break;
            case 2:
                skullLed.setColor(GRed);
                break;
            case 3:
                skullLed.setColor(GGreen);
                break;
            case 4:
                skullLed.setColor(GPurple);
                break;
            case 5:
                skullLed.setColor(GAqua);
                break;
            case 6:
                changeColorsMode(900);
                break;
            case 7:
                changeColorsMode(300);
                break;
            case 8:
                changeColorsMode(100);
                break;
            case 9:
                turnOnOffFadeMode(true);
                skullLed.setFadePeriod(1700);
                changeColorsMode(2000);
                break;
            default:
                break;
        }
    }

private:
    void turnOnOffFadeMode(boolean mode) {
        skullLed.fadeMode(mode);
        skullLed.setCRT(mode);
    }

private:
    void changeColorsMode(int period) {
        skullLed.tick();
        static uint32_t tmr = 0;
        static byte mode = 0;

        if (millis() - tmr >= period) {
            tmr = millis();
            if (++mode >= 6) mode = 0;
            switch (mode) {
                case 0:
                    skullLed.setColor(GMagenta);
                    break;
                case 1:
                    skullLed.setColor(GYellow);
                    break;
                case 2:
                    skullLed.setColor(GMaroon);
                    break;
                case 3:
                    skullLed.setColor(GOlive);
                    break;
                case 4:
                    skullLed.setColor(GPurple);
                    break;
                case 5:
                    skullLed.setColor(GAqua);
                    break;
                default:
                    break;
            }
        }
    }
};

#endif //UNTITLED4_SKULLSWITCHER_H
