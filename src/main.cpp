#include <Arduino.h>
#include "LightSwitcher.h"
#include "SkullSwitcher.h"
#include "GRGB.h"

GRGB skullLed(COMMON_CATHODE, skullLedR, skullLedG, skullLedB);; // куда подключены цвета (R, G, B)
GRGB lightLed(COMMON_CATHODE, lowHigLedR, lowHigLedG, lowHigLedB);; // куда подключены цвета (R, G, B)
SkullSwitcher skullControl (skullLed);
LightSwitcher lightControl (lightLed);

void setup() {
    // put your setup code here, to run once:
    pinMode(inputGab, INPUT_PULLUP);  // 0 = нажато
    pinMode(inputLowLight, INPUT_PULLUP);
    pinMode(inputSwitch, INPUT_PULLUP);
    pinMode(inputAllLightOn, INPUT_PULLUP);
    pinMode(outputSkull, INPUT_PULLUP);

    pinMode(outputSkull, OUTPUT);
    pinMode(outputHighLight, OUTPUT);
    pinMode(outputLowLight, OUTPUT);
    pinMode(outputGab, OUTPUT);
    pinMode(relayPower, OUTPUT);
    digitalWrite(relayPower,HIGH);

    digitalWrite(outputGab, HIGH);
    digitalWrite(outputLowLight, HIGH);
    digitalWrite(outputHighLight, HIGH);
    digitalWrite(outputSkull, HIGH);
    //RGB
    pinMode(skullLedR, OUTPUT);
    pinMode(skullLedG, OUTPUT);
    pinMode(skullLedB, OUTPUT);

    pinMode(lowHigLedR, OUTPUT);
    pinMode(lowHigLedG, OUTPUT);
    pinMode(lowHigLedB, OUTPUT);

    // теперь любая установка цвета будет плавной
    // установить период перехода
    Serial.begin(9600);
    lightControl.setUp();
    skullControl.setUp();
    skullControl.set_switch_btn(lightControl.switch_btn());

    skullLed.setBrightness(255);
    skullLed.enable();

    lightLed.setBrightness(255);
    lightLed.enable();
}

void loop() {
    skullLed.tick();
    skullControl.controlTheSkull();
    lightControl.controlTheLight();
}
