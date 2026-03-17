// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FTimeData.h"
#include "SunMoonController.generated.h"

class UMessagingSubsystem;
class UCurveLinearColor;

UCLASS()
class ENVIRONMENT_API ASunMoonController : public AActor
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	void updateSunLightPrecise(float preciseTime);
	UFUNCTION()
	void updateTimeOfDay(const FTimeData& newTime);
	void updateSunLight();
	void updateSkyLight();
	void updateMoonLight();

	TObjectPtr<UMessagingSubsystem> messageManager;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess="true"))
	FTimeData currentTime;

	// These two show how far through the day we are, in minutes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float timeOfDayRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float currentTimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	bool hasDayNightCycle = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ADirectionalLight* sunLight;

	/*
	* Curves are made in the editor and added to the blueprint we have there.
	* TODO: The values in the curve needs investigating.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveLinearColor* dailySunRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveLinearColor* annualSunRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASkyLight* skyLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveLinearColor* skyLightDailyColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float maxSunIntensity = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	ASunMoonController();

	// Without the UFUNCTION() macro, the reflection system doesn't "see" the function, 
	// causing AddDynamic to fail silently at runtime.
	UFUNCTION()
	void timeChangeUpdate(const FTimeData& timeData);
	UFUNCTION()
	void preciseTimeChangeUpdate(float preciseTime);
};
