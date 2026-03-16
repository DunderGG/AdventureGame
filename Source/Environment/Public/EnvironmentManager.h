// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "FTimeData.h"
#include "MessagingSubsystem.h"
#include "EnvironmentManager.generated.h"

/**
 * UTickableWorldSubsystem combines UWorldSubsystem and FTickableGameObject.
 *    Must call it's respective Initialize and Deinitialize functions to make it tick.
 */
UCLASS()
class ENVIRONMENT_API UEnvironmentManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
private:
	bool useDayNightCycle = true;
	bool timeWasUpdated = false;
	FTimeData currentTime;
	float minuteLength = 10;
	float timeDecay = 0;
	int currentTimeOfDay = 0;

	UPROPERTY()
	TObjectPtr<UMessagingSubsystem> messageManager = nullptr;

	void updateTime(const float DeltaTime);
	void advanceMinute();
	void advanceHour();
	void advanceDay();
	void advanceMonth();
	void advanceYear();
	void setDayOfYear();
	void calculateDayLength();
	void updateTimeOfDayRef();
	void updateLighting();
	void updateLightRotation();
protected:
	// Should the time advance? 
	UPROPERTY(EditAnywhere, Category = "Environment|Time")
	bool shouldTick = true;

	// Real-world minutes it takes for one full game day.
	UPROPERTY(EditAnywhere, Category = "Environment | Time")
	float gameDayLengthInRealMinutes = 10;

public:
	// UWorldSubsystem / FTickableGameObject overrides
	// There are also pre- and post-initialize functions.
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;
	virtual bool IsTickable() const override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};
