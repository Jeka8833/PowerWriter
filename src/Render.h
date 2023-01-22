//
// Created by Jeka8833 on 22.01.2023.
//

#ifndef POWERWRITER_RENDER_H
#define POWERWRITER_RENDER_H

#include "Settings.h"
#include <LiquidCrystal_I2C.h>

class Render {
public:
    static void init();
    static void render();

private:
    static void drawPage1();
    static void drawPage2();
    static void drawPage3();

    static char *add0(uint8_t value);
};


#endif //POWERWRITER_RENDER_H
