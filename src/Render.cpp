//
// Created by Jeka8833 on 22.01.2023.
//

#include "Render.h"
#include "RTClib.h"
#include "Time.h"

LiquidCrystal_I2C lcd(DISPLAY_ADDRESS, DISPLAY_COLUMN, DISPLAY_ROW);
uint8_t lastPage = 0;
uint8_t timeUpdate = 0;

void Render::init() {
    lcd.init();
    lcd.backlight();

    // Пины D9 и D10 - 7.8 кГц
    TCCR1A = 0b00000001;  // 8bit
    TCCR1B = 0b00001010;  // x8 fast pwm
}

void Render::render() {
    uint16_t button = analogRead(BUTTONS_PIN);
    if (button < BUTTON_RIGHT / 2) {
        // Not pressed button

    } else if (BUTTON_RIGHT / 2 <= button && button < BUTTON_CENTER - (BUTTON_CENTER - BUTTON_RIGHT) / 2) {
        // Right button
        Serial.println("Right");

    } else if (BUTTON_CENTER - (BUTTON_CENTER - BUTTON_RIGHT) / 2 <= button &&
               button < BUTTON_LEFT - (BUTTON_LEFT - BUTTON_CENTER) / 2) {
        // Center button
        Serial.println("Center");

    } else {
        // Left button
        Serial.println("Left");

    }

    uint16_t light = max(min(analogRead(LIGHT_SENSOR), 1023), 0);
    analogWrite(DISPLAY_BRIGHTNESS_PWM, map(light, 0, 1023, 254, 0));

    uint8_t time = millis() >> 10; // +- 1 second

    if (timeUpdate == time) return;
    timeUpdate = time;

    lcd.setCursor(0, 0);
    uint8_t pages = time / 5 % 3;
    if (lastPage != pages) {
        lcd.clear();
        lastPage = pages;
    }

    switch (pages) {
        case 1:
            drawPage2();
            break;
        case 2:
            drawPage3();
            break;
        default:
            drawPage1();
            break;
    }
}

void Render::drawPage1() {
    DateTime now = Time::now();
    lcd.print(add0(now.day()));
    lcd.print('.');
    lcd.print(add0(now.month()));
    lcd.print('.');
    lcd.print(now.year() - 2000, DEC);
    lcd.print(' ');
    lcd.print(Time::rtc.getTemperature(), 2);
    lcd.print("*C");

    lcd.setCursor(0, 1);
    lcd.print(add0(now.hour()));
    lcd.print(':');
    lcd.print(add0(now.minute()));
    lcd.print(':');
    lcd.print(add0(now.second()));
}

void Render::drawPage2() {
    TimeSpan onTime = Time::getEnabledTime();

    lcd.print("On  :");
    lcd.print(add0(onTime.days()));
    lcd.print(' ');
    lcd.print(add0(onTime.hours()));
    lcd.print(':');
    lcd.print(add0(onTime.minutes()));
    lcd.print(':');
    lcd.print(add0(onTime.seconds()));

    lcd.setCursor(0, 1);
    lcd.print("Off :");
    lcd.print(add0(Time::disabledTime.days()));
    lcd.print(' ');
    lcd.print(add0(Time::disabledTime.hours()));
    lcd.print(':');
    lcd.print(add0(Time::disabledTime.minutes()));
    lcd.print(':');
    lcd.print(add0(Time::disabledTime.seconds()));
}

void Render::drawPage3() {
    lcd.print("The old on time:");
    lcd.setCursor(0, 1);
    lcd.print(add0(Time::lastEnabledTime.days()));
    lcd.print(' ');
    lcd.print(add0(Time::lastEnabledTime.hours()));
    lcd.print(':');
    lcd.print(add0(Time::lastEnabledTime.minutes()));
    lcd.print(':');
    lcd.print(add0(Time::lastEnabledTime.seconds()));
}

char *Render::add0(uint8_t value) {
    static char buffer[3];
    buffer[0] = value / 10 + '0';
    buffer[1] = value % 10 + '0';
    buffer[2] = '\0';
    return buffer;
}
