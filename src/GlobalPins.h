//
// Created by V on 09.07.2024.
//

#ifndef UNTITLED4_GLOBALPINS_H
#define UNTITLED4_GLOBALPINS_H

#include <Arduino.h>


enum GlobalPins {
    inputSkull = 2,
    inputAllLightOn = 4,
    inputSwitch = 7,
    inputGab = 12,
    inputLowLight = 8,
    relayPower = A1,
    outputGab = A3,
    outputLowLight = A4,
    outputHighLight = A5,
    outputSkull = A2,
    skullLedR =3,
    skullLedG =5,
    skullLedB =6,
    lowHigLedR =9,
    lowHigLedG =10,
    lowHigLedB =11,
};


#endif //UNTITLED4_GLOBALPINS_H
