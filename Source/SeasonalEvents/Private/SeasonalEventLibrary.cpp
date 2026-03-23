

#include "SeasonalEventLibrary.h"

ESeasonalEventType USeasonalEventLibrary::convertDayOfYearToSeasonalEvent(const FDateTime& dateTime) const
{
	const int32 dayOfYear = dateTime.GetDayOfYear();
	const bool isLeapYear = dateTime.IsLeapYear(dateTime.GetYear());

	//Christmas: December 25th (Day 359 or 360 in leap years)
	if (dayOfYear >= 344 && dayOfYear < 365)
	{
		return ESeasonalEventType::SE_CHRISTMAS;
	}

	//Halloween: October 31st (Day 304 or 305 in leap years)
	if (dayOfYear == 304 || (dayOfYear == 305 && isLeapYear))
	{
		return ESeasonalEventType::SE_HALLOWEEN;
	}

	//Valentine's Day: February 14th (Day 45 or 46 in leap years)
	if (dayOfYear == 45 || (dayOfYear == 46 && isLeapYear))
	{
		return ESeasonalEventType::SE_VALENTINES;
	}

	return ESeasonalEventType::SE_NONE;
}

/*
*	Source: https://cplusplus.com/forum/beginner/173917/
* 
*    Using the Meeus/Jones/Butcher algorithm.
*/
bool USeasonalEventLibrary::isEasterSunday(const FDateTime& dateTime) const
{
	const int32 year = dateTime.GetYear();
	int a = year % 19;
	int b = year / 100;
	int c = year % 100;
	int d = b / 4;
	int e = b % 4;
	int f = (b + 8) / 25;
	int g = (b - f + 1) / 3;
	int h = (19 * a + b - d - g + 15) % 30;
	int i = c / 4;
	int k = c % 4;
	int l = (32 + 2 * e + 2 * i - h - k) % 7;
	int m = (a + 11 * h + 22 * l) / 451;
	int month = (h + l - 7 * m + 114) / 31; // 3 for March, 4 for April
	int day = ((h + l - 7 * m + 114) % 31) + 1;

	if (dateTime.GetMonth() == month && dateTime.GetDay() == day)
	{
		return true;
	}

	return false;
}

/*
*	Convert a given DateTime to a Seasonal Event, if any.
*
* Logic here is:
*	Go from the least specific, to the most. I.e, biggest span of days, to smallest.
*		Checking for easter is complicated, so check that last.
*/
ESeasonalEventType USeasonalEventLibrary::convertDateTimeToSeasonalEvent(const FDateTime& dateTime)
{
	// Try to find one of the fixed date seasonal events first.
	ESeasonalEventType seasonalEvent = convertDayOfYearToSeasonalEvent(dateTime);
	if (seasonalEvent != ESeasonalEventType::SE_NONE)
	{
		return seasonalEvent;
	}

	// If no fixed, check if it's easter, which is a bit more complicated to calculate.
	if (isEasterSunday(dateTime))
	{
		return ESeasonalEventType::SE_EASTER;
	}

	return seasonalEvent;
}
