// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
//#include "FTimeData.h"
#include "MessagingSubsystem.generated.h"

class UGameInstance;

/*
* A delegate is kind of like a mailing list,
* where you can subscribe to it and then when the event happens,
* all the subscribers will be notified.
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPreciseTimeChangeDelegate, float, preciseTime);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeChangeDelegate, const FTimeData&, timeData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHourChangeDelegate, const int, hour);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDayOfYearChangeDelegate, const int, dayOfYear);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMonthChangeDelegate, const int, month);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FYearChangeDelegate, const int, year);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTemperatureChangeDelegate, const float, currentTemp);
/**
 * 
 */
UCLASS()
class MESSAGEHANDLER_API UMessagingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:

protected:

public:
#pragma region DELEGATES
	UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	FPreciseTimeChangeDelegate onPreciseTimeChange;

	//UPROPERTY(BlueprintAssignable, Category = "MessageManager | Time")
	//FTimeChangeDelegate onTimeChange;

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
	void updatePreciseTime(float preciseTime);
	//void updateTimeOfDay(const FTimeData& newTime);
	void updateHourOfDay(const int newHour);
	void updateDayOfYear(const int newDayOfYear);
	void updateMonth(const int newMonth);
	void updateYear(const int newYear);
	void updateTemperature(const float newTemperature);
#pragma endregion

	static UMessagingSubsystem* Get(const UObject* worldContextObject);
};
