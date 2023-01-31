//
// Created by Jeka8833 on 28.01.2023.
//

#ifndef POWERWRITER_CONFIG_H
#define POWERWRITER_CONFIG_H

#include "Arduino.h"

class Config {
public:
    static uint8_t minLightBrightness;
    static uint8_t shiftLightBrightness;
    static uint8_t maxLightBrightness;

    static void read();

    static void write();
};


#endif //POWERWRITER_CONFIG_H
