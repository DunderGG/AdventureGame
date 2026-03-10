// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "EnvironmentManager.h"
#include "Logger.h"
#include "MessageManager.h"
#include "Misc/DateTime.h"

void UEnvironmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TObjectPtr<UWorld> world = GetWorld();

	// TODO: Not sure about IsGameWorld().
	if (IsValid(world) && world->IsGameWorld())
	{
		FString mapName = world->GetMapName();
		mapName = world->RemovePIEPrefix(mapName);

		// TODO: Remember to use this name for our main menu map.
		if (mapName.Equals("MainMenu", ESearchCase::IgnoreCase))
		{
			shouldTick = false;
			Logger::addMessage(TEXT("Environment Manager: Ticking disabled for MainMenu"), SEVERITY::Debug);
		}

		SetTickableTickType(ETickableTickType::Conditional);
	}

	Logger::addMessage(TEXT("Environment Manager Initialized"), SEVERITY::Debug);
}

void UEnvironmentManager::Deinitialize()
{
	Super::Deinitialize();
	Logger::addMessage(TEXT("Environment Manager Deinitialized"), SEVERITY::Debug);
}

bool UEnvironmentManager::IsTickable() const
{
	// Only tick instances created for a world, not the class template itself.
	return !IsTemplate() && shouldTick;
}

void UEnvironmentManager::Tick(float DeltaTime)
{
	if (logTick)
	{
		logTick = false;
		Logger::addMessage(TEXT("Environment Manager Tick"), SEVERITY::Debug);
	}
}

TStatId UEnvironmentManager::GetStatId() const
{
	// This identifies the class in the 'stat cycle' profiler (F9 in-game)
	// TODO: Not sure how this works, investigate?
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEnvironmentManager, STATGROUP_Tickables);
}

/*
* Time management functions. 
*/
void UEnvironmentManager::updateTime(const float DeltaTime)
{
	timeDecay -= DeltaTime;
	if (timeDecay <= 0)
	{
		timeDecay += minuteLength;
		advanceMinute();
	}
}

void UEnvironmentManager::advanceMinute()
{
	timeWasUpdated = true;
	currentTime.minute++;

	if (currentTime.minute > 59)
	{
		currentTime.minute = 0;
		advanceHour();
	}
}

void UEnvironmentManager::advanceHour()
{
	timeWasUpdated = true;
	currentTime.hour++;
	if (currentTime.hour > 23)
	{
		currentTime.hour = 0;
		advanceDay();
	}

	if (messageManager)
	{
		messageManager->updateHourOfDay(currentTime.hour);
	}
}

void UEnvironmentManager::advanceDay()
{
	timeWasUpdated = true;
	addDayOfYear();
	currentTime.dayOfMonth++;

	const bool isLeapYear = FDateTime::IsLeapYear(currentTime.year);

	switch (currentTime.month)
	{
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if (currentTime.dayOfMonth > 31)
			{
				currentTime.dayOfMonth = 1;
				advanceMonth();
			}
			break;
		case 4: case 6: case 9: case 11:
			if (currentTime.dayOfMonth > 30)
			{
				currentTime.dayOfMonth = 1;
				advanceMonth();
			}
			break;
		case 2:
			// February has 29 days in leap years.
			if (isLeapYear)
			{
				if (currentTime.dayOfMonth > 29)
				{
					currentTime.dayOfMonth = 1;
					advanceMonth();
				}
			}
			else
			{
				if (currentTime.dayOfMonth > 28)
				{
					currentTime.dayOfMonth = 1;
					advanceMonth();
				}
			}
			break;
		default:
			Logger::addMessage(FString::Printf(TEXT("Invalid month: %d"), currentTime.month), SEVERITY::Error);
	}

	if (messageManager)
	{
		messageManager->updateDayOfYear(currentTime.dayOfYear);
	}
}

void UEnvironmentManager::advanceMonth()
{
	timeWasUpdated = true;
	currentTime.month++;
	if (currentTime.month > 12)
	{
		currentTime.month = 1;
		advanceYear();
	}

	if (messageManager)
	{
		// TODO: messageManager->updateMonthOfYear(currentTime.month);
	}
}

void UEnvironmentManager::advanceYear()
{
	timeWasUpdated = true;
	currentTime.year++;

	if (messageManager)
	{
		// TODO: messageManager->updateYear(currentTime.year);
	}
}

void UEnvironmentManager::setDayOfYear()
{
	const bool isLeapYear = FDateTime::IsLeapYear(currentTime.year);
	const int daysInFeb = (isLeapYear ? 29 : 28);
	const int daysInMonth[] = { 31, daysInFeb, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	currentTime.dayOfYear = 0;
	
	// Add up the number of days in all previous months.
	for (int previousMonth = 0; previousMonth < currentTime.month - 1; previousMonth++)
	{
		currentTime.dayOfYear += daysInMonth[previousMonth];
	}
	// Then add the number of days into the current month.
	currentTime.dayOfYear += currentTime.dayOfMonth;
}

void UEnvironmentManager::calculateDayLength()
{
}

void UEnvironmentManager::updateTimeOfDayRef()
{
}

void UEnvironmentManager::updateLighting()
{
}

void UEnvironmentManager::updateLightRotation()
{
}

void UEnvironmentManager::addDayOfYear()
{
}
