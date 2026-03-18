// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "EnvironmentManager.h"
#include "Logger.h"
#include "Misc/DateTime.h"

void UEnvironmentManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	TObjectPtr<UWorld> world = GetWorld();

	// TODO: Not sure about IsGameWorld().
	if (IsValid(world) && world->IsGameWorld())
	{
		// Cache the messaging subsystem from the world game instance.
		// TODO: Should we do this here, or in OnWorldBeginPlay()?
		messageManager = UMessagingSubsystem::Get(this);

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
	messageManager = nullptr;
	Logger::addMessage(TEXT("Environment Manager Deinitialized"), SEVERITY::Debug);
}

bool UEnvironmentManager::IsTickable() const
{
	// Only tick instances created for a world, not the class template itself.
	return !IsTemplate() && shouldTick;
}

TStatId UEnvironmentManager::GetStatId() const
{
	// This identifies the class in the 'stat cycle' profiler (F9 in-game)
	// TODO: Not sure how this works, investigate?
	RETURN_QUICK_DECLARE_CYCLE_STAT(UEnvironmentManager, STATGROUP_Tickables);
}

void UEnvironmentManager::Tick(float DeltaTime)
{
	// Send a precise time update every tick.
	// TODO: Maybe this is too resource intensive, sending updates every tick.
	// This tickCounter is just a cheap way of "fixing" this... Must investigate better solutions.
	++tickCounter;
	if (tickCounter == preciseTimeUpdateFrequency)
	{
		tickCounter = 0;
		updateTimeOfDayRef();
		if (messageManager)
		{
			//Logger::addMessage(TEXT("Environment Manager: Sending precise time update"), SEVERITY::Debug);
			messageManager->updatePreciseTime(timeOfDayRef);
		}
	}

	updateTime(DeltaTime);
	if (timeWasUpdated && messageManager)
	{
		messageManager->updateTimeOfDay(currentTime);
		timeWasUpdated = false;
	}
}

/*
* Called from Tick() every frame
* The timeDecay prevents time updates from happening too frequently.
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

#pragma region Advance Time Functions
/*
* A reason for breaking all of these up in to separate functions is to easily be able to advance one of them.
* For example, if the player sleeps for 8 hours, we can just call advanceHour() 8 times, 
*		and it will handle all the necessary rollovers.
*	Look at many RPGs; You go to sleep, the hours start ticking forward one at a time.
*/
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
	Logger::addMessage(FString::Printf(TEXT("UEnvironmentManager::advanceHour: Advancing hour to %d"), currentTime.hour), SEVERITY::Debug);
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
	currentTime.dayOfMonth++;
	currentTime.dayOfYear++;

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
		messageManager->updateMonth(currentTime.month);
	}
}

void UEnvironmentManager::advanceYear()
{
	timeWasUpdated = true;
	currentTime.year++;
	currentTime.dayOfYear = 1; // Reset day of year to 1 at the start of a new year.

	if (messageManager)
	{
		messageManager->updateYear(currentTime.year);
	}
}
#pragma endregion

/*
* The "overall" day of the year.
*/
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

/*
* A reference point for the current time of day, in minutes. 
* This can be used for lighting and other time-of-day related calculations.
*/
void UEnvironmentManager::updateTimeOfDayRef()
{
	// Calculate precise time: (Hours * 60) + Minutes + Fraction of current minute
	// timeDecay counts down from minuteLength to 0
	float minuteProgress = FMath::Clamp(1.0f - (timeDecay / minuteLength), 0.0f, 1.0f);
	timeOfDayRef = (currentTime.hour * 60) + currentTime.minute + minuteProgress;
}

void UEnvironmentManager::updateLighting()
{
}

void UEnvironmentManager::updateLightRotation()
{
}

/* 
* Used for dynamic day length, i.e.the player decides how long the day should be.
* 1440 minutes in a day, so we divide the total real-world seconds in a day by 1440 to get the length of each minute.
*/
void UEnvironmentManager::calculateDayLength()
{
	minuteLength = (gameDayLengthInRealMinutes * 60) / 1440;

	timeDecay = minuteLength;
}

void UEnvironmentManager::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	calculateDayLength();

	Logger::addMessage(FString::Printf(TEXT(
		"UEnvironmentManager::OnWorldBeginPlay: Minute length calculated to %f seconds per minute"), 
		minuteLength), SEVERITY::Debug);
}
