#include "Time.h"

WORD Time::getHours() { return _hours; }
WORD Time::getMinutes() { return _minutes; }
void Time::setHours(WORD hour) { _hours = hour; }
void Time::setMinutes(WORD minute) { _minutes = minute; }
bool Time::areMinConfirmed() { return _confirmedMinutes; }
bool Time::areHoursConfirmed() { return _confirmedHours; }
void Time::confirmHours() { _confirmedHours = true; }
void Time::confirmMinutes() { _confirmedMinutes = true; }
void Time::confirmTime() { _confirmedTime = true; }
bool Time::isTimeConfirmed() { return _confirmedTime; }