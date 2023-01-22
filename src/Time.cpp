//
// Created by Jeka8833 on 22.01.2023.
//

#include "Time.h"

RTC_DS3231 Time::rtc;

TimeSpan Time::lastEnabledTime;
TimeSpan Time::disabledTime;
DateTime Time::timeEnable;

bool Time::init() {
    if (!rtc.begin()) return false;

    if (rtc.lostPower()) {
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    timeEnable = rtc.now();
    DateTime timeAlarm1 = getAlarm1(timeEnable);
    DateTime timeAlarm2 = getAlarm2(timeAlarm1);
    lastEnabledTime = timeAlarm1 - timeAlarm2;
    disabledTime = timeEnable - timeAlarm1;
}

TimeSpan Time::getEnabledTime() {
    return now() - timeEnable;
}

void Time::updateAlarms() {
    DateTime now = rtc.now();
    DateTime timeAlarm1 = getAlarm1(now);
    rtc.setAlarm1(now, Ds3231Alarm1Mode::DS3231_A1_Date);
    rtc.setAlarm2(timeAlarm1, Ds3231Alarm2Mode::DS3231_A2_Date);
}

DateTime Time::now() {
    DateTime now = rtc.now();
    rtc.setAlarm1(now, Ds3231Alarm1Mode::DS3231_A1_Date);
    return now;
}

DateTime Time::getAlarm1(const DateTime &now) {
    DateTime timeAlarm1 = rtc.getAlarm1();

    DateTime modTime =
            timeAlarm1.day() > now.day() ?
            now -
            TimeSpan(getNumberOfDays(now.month(), now.year()), 0, 0, 0) :
            now;
    return {modTime.year(), modTime.month(), timeAlarm1.day(),
            timeAlarm1.hour(), timeAlarm1.minute(), timeAlarm1.second()};
}

DateTime Time::getAlarm2(const DateTime &alarm1) {
    DateTime timeAlarm2 = rtc.getAlarm2();

    DateTime modTime =
            timeAlarm2.day() > alarm1.day() ?
            alarm1 -
            TimeSpan(getNumberOfDays(alarm1.month(), alarm1.year()), 0, 0, 0) :
            alarm1;
    return {modTime.year(), modTime.month(), timeAlarm2.day(),
            timeAlarm2.hour(), timeAlarm2.minute(), alarm1.second()};
}

int Time::getNumberOfDays(uint8_t month, uint16_t year) {
    //leap year condition, if month is 2
    if (month == 2) {
        if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
        //months which has 31 days
    else if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8
             || month == 10 || month == 12)
        return 31;
    else
        return 30;
}
