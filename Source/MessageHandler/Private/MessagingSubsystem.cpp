// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "MessagingSubsystem.h"

/*
* These are basically passthrough functions
*	When the time changes, we want to trigger the delegate, which will notify all subscribers that the time has changed.
*/
void UMessagingSubsystem::updateTimeOfDay(FTimeData newTime)
{
	onTimeChange.Broadcast(newTime);
}

void UMessagingSubsystem::updateHourOfDay(int newHour)
{
	onHourChange.Broadcast(newHour);
}

void UMessagingSubsystem::updateDayOfYear(int newDayOfYear)
{
	onDayOfYearChange.Broadcast(newDayOfYear);
}

void UMessagingSubsystem::updateMonth(int newMonth)
{
	onMonthChange.Broadcast(newMonth);
}

void UMessagingSubsystem::updateYear(int newYear)
{
	onYearChange.Broadcast(newYear);
}

void UMessagingSubsystem::updateTemperature(float newTemperature)
{
	onTemperatureChange.Broadcast(newTemperature);
}
