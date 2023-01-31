//
// Created by Jeka8833 on 28.01.2023.
//

#include "Config.h"

uint8_t EEMEM minLightBrightnessPointer;
uint8_t EEMEM shiftLightBrightnessPointer;
uint8_t EEMEM maxLightBrightnessPointer;

uint8_t Config::minLightBrightness = 1;
uint8_t Config::shiftLightBrightness = 0;
uint8_t Config::maxLightBrightness = 255;

void Config::read() {
    minLightBrightness = eeprom_read_byte(&minLightBrightnessPointer);
    shiftLightBrightness = eeprom_read_byte(&shiftLightBrightnessPointer);
    maxLightBrightness = eeprom_read_byte(&maxLightBrightnessPointer);
}

void Config::write() {
    eeprom_update_byte(&minLightBrightnessPointer, minLightBrightness);
    eeprom_update_byte(&shiftLightBrightnessPointer, shiftLightBrightness);
    eeprom_update_byte(&maxLightBrightnessPointer, maxLightBrightness);
}
