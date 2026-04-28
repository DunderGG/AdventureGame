// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "MessagingSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"

void UMessagingSubsystem::updatePreciseTime(float preciseTime)
{
	onPreciseTimeChange.Broadcast(preciseTime);
}

/*
* These are basically passthrough functions
*	When the time changes, we want to trigger the delegate, which will notify all subscribers that the time has changed.
*/
/*
void UMessagingSubsystem::updateTimeOfDay(const FTimeData& newTime)
{
	onTimeChange.Broadcast(newTime);
}
*/
void UMessagingSubsystem::updateHourOfDay(const int newHour)
{
	onHourChange.Broadcast(newHour);
}

void UMessagingSubsystem::updateDayOfYear(const int newDayOfYear)
{
	onDayOfYearChange.Broadcast(newDayOfYear);
}

void UMessagingSubsystem::updateMonth(const int newMonth)
{
	onMonthChange.Broadcast(newMonth);
}

void UMessagingSubsystem::updateYear(const int newYear)
{
	onYearChange.Broadcast(newYear);
}

void UMessagingSubsystem::updateTemperature(const float newTemperature)
{
	onTemperatureChange.Broadcast(newTemperature);
}

UMessagingSubsystem* UMessagingSubsystem::Get(const UObject* worldContextObject)
{
	UWorld* world = GEngine->GetWorldFromContextObject(worldContextObject, 
													   EGetWorldErrorMode::LogAndReturnNull);
	if (world)
	{
		UGameInstance* gameInstance = world->GetGameInstance();
		if (gameInstance)
		{
			return gameInstance->GetSubsystem<UMessagingSubsystem>();
		}
	}
	return nullptr;
}
