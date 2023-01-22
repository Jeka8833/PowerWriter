//
// Created by Jeka8833 on 22.01.2023.
//

#ifndef POWERWRITER_TIME_H
#define POWERWRITER_TIME_H

#include <RTClib.h>

class Time {
public:
    static RTC_DS3231 rtc;

    static bool init();
    static void updateAlarms();

    static DateTime timeEnable;

    static TimeSpan lastEnabledTime;
    static TimeSpan disabledTime;

    static TimeSpan getEnabledTime();

    static DateTime now();
    static DateTime getAlarm1(const DateTime& now);
    static DateTime getAlarm2(const DateTime& alarm1);
private:
    static int getNumberOfDays(uint8_t month, uint16_t year);
};


#endif //POWERWRITER_TIME_H
