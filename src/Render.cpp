//
// Created by Jeka8833 on 22.01.2023.
//

#include "Render.h"
#include "RTClib.h"
#include "Time.h"
#include "Config.h"
#include "EncButton.h"

LiquidCrystal_I2C lcd(DISPLAY_ADDRESS, DISPLAY_COLUMN, DISPLAY_ROW);
uint32_t nextPageTime = 0;
uint8_t mainScreenPage = 0;
uint8_t menuItem = 0;

EncButton<EB_TICK, VIRT_BTN> selectBtn;
EncButton<EB_TICK, VIRT_BTN> minusBtn;
EncButton<EB_TICK, VIRT_BTN> plusBtn;

void Render::init() {
    lcd.init();
    lcd.backlight();

    // Пины D9 и D10 - 7.8 кГц
    TCCR1A = 0b00000001;  // 8bit
    TCCR1B = 0b00001010;  // x8 fast pwm

    selectBtn.setHoldTimeout(1500);
    selectBtn.setStepTimeout(200);

    minusBtn.setHoldTimeout(1500);
    minusBtn.setStepTimeout(200);

    plusBtn.setHoldTimeout(1500);
    plusBtn.setStepTimeout(200);
}

void Render::render() {
    uint16_t button = analogRead(BUTTONS_PIN);

    uint8_t buttonPressed = 0;
    if (button < BUTTON_RIGHT / 2) {
    } else if (BUTTON_RIGHT / 2 <= button && button < BUTTON_CENTER - (BUTTON_CENTER - BUTTON_RIGHT) / 2) {
        buttonPressed = 3;
    } else if (BUTTON_CENTER - (BUTTON_CENTER - BUTTON_RIGHT) / 2 <= button &&
               button < BUTTON_LEFT - (BUTTON_LEFT - BUTTON_CENTER) / 2) {
        buttonPressed = 2;
    } else {
        buttonPressed = 1;
    }
    selectBtn.tick(buttonPressed == 1);
    minusBtn.tick(buttonPressed == 2);
    plusBtn.tick(buttonPressed == 3);

    if (menuItem) {
        if (selectBtn.hold()) {
            Config::write();
            menuItem = 0;
        }

    } else {
        if (selectBtn.press()) {
            menuItem = 1;
        }

        if (minusBtn.press() || minusBtn.step()) {
            nextPageTime = millis() + 15000;

            if (mainScreenPage == 0)
                changeScreen(mainScreenPage + 2);
            else
                changeScreen(mainScreenPage - 1);
        }

        if (plusBtn.press() || plusBtn.step()) {
            nextPageTime = millis() + 15000;

            changeScreen(mainScreenPage + 1);
            if (mainScreenPage >= 3) mainScreenPage = 0;
        }
    }

    uint16_t light = max(min(analogRead(LIGHT_SENSOR), 1023), 0);
    analogWrite(DISPLAY_BRIGHTNESS_PWM, map(light, 0, 1023, 254, 0));

    if (!menuItem && nextPageTime < millis()) {
        changeScreen(mainScreenPage + 1);
        if (mainScreenPage >= 3) mainScreenPage = 0;
        nextPageTime = millis() + 5000;
    }

    lcd.setCursor(0, 0);
    switch (mainScreenPage) {
        case 0:
            drawPage1();
            break;
        case 1:
            drawPage2();
            break;
        case 2:
            drawPage3();
            break;
        default:
            drawPageMenu();
            break;
    }
}

void Render::changeScreen(uint8_t screen) {
    lcd.clear();
    mainScreenPage = screen;
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

void Render::drawPageMenu() {

}

char *Render::add0(uint8_t value) {
    static char buffer[3];
    buffer[0] = value / 10 + '0';
    buffer[1] = value % 10 + '0';
    buffer[2] = '\0';
    return buffer;
}