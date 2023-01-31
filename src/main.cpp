#include "Settings.h"
#include "Time.h"
#include "Render.h"
#include "Config.h"
#include <SD.h>
#include <RTClib.h>

void setup() {
    Serial.begin(9600);
    Time::init();

    bool SDReady = SD.begin(CS_PIN_SD);
    if (SDReady) {
        File myFile = SD.open("time.csv", FILE_WRITE);
        if (myFile) {
            DateTime timeAlarm1 = Time::getAlarm1(Time::timeEnable);

            myFile.print(Time::getAlarm2(timeAlarm1).timestamp());
            myFile.print(',');
            myFile.print(timeAlarm1.timestamp());
            myFile.print(',');
            myFile.print(Time::timeEnable.timestamp());
            myFile.println();
            myFile.close();
        }
    }
    Time::updateAlarms();

    Config::read();
    Render::init();
}

void loop() {
    Render::render();
}