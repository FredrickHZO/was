#pragma once
#include <Windows.h>

class Time{
private:
	WORD _hours;
	WORD _minutes;
	bool _confirmedHours;
	bool _confirmedMinutes;
	bool _confirmedTime;

public:
	Time() : _hours(0), _minutes(0),
		_confirmedHours(false),
		_confirmedMinutes(false),
		_confirmedTime(false) {}

	WORD getHours();
	WORD getMinutes();
	void setHours(WORD hour);
	void setMinutes(WORD minute);
	void confirmHours();
	void confirmMinutes();
	void confirmTime();
	bool areMinConfirmed();
	bool areHoursConfirmed();
	bool isTimeConfirmed();
};

