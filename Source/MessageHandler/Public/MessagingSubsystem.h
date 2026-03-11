// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FTimeData.h"
#include "MessagingSubsystem.generated.h"

/*
* A delegate is kind of like a mailing list,
* where you can subscribe to it and then when the event happens,
* all the subscribers will be notified.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeChangeDelegate, FTimeData, timeData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHourChangeDelegate, int, hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayOfYearChangeDelegate, int, dayOfYear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonthChangeDelegate, int, month);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FYearChangeDelegate, int, year);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTemperatureChangeDelegate, float, currentTemp);

/**
 * 
 */
UCLASS()
class MESSAGEHANDLER_API UMessagingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:

protected:

public:
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FTimeChangeDelegate onTimeChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FHourChangeDelegate onHourChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Date")
	FDayOfYearChangeDelegate onDayOfYearChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FMonthChangeDelegate onMonthChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FYearChangeDelegate onYearChange;

	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Temperature")
	FTemperatureChangeDelegate onTemperatureChange;
#pragma endregion

#pragma region TRIGGERS
	void updateTimeOfDay(FTimeData newTime);
	void updateHourOfDay(int newHour);
	void updateDayOfYear(int newDayOfYear);
	void updateMonth(int newMonth);
	void updateYear(int newYear);
	void updateTemperature(float newTemperature);
#pragma endregion
};
