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
	void updateSunLightPrecise();
	UFUNCTION()
	void updateTimeOfDay(const FTimeData& newTime);
	//void updateSunLight();
	UFUNCTION()
	void updateSkyLight();
	UFUNCTION()
	void updateMoonLight();

	TObjectPtr<UMessagingSubsystem> messageManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	FTimeData currentTime;

	// These two show how far through the day we are, in minutes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	float timeOfDayRef;
	// TODO: We probably don't need this if we stick to the more precise time reporting instead.
	//		On the other hand, if we go with a much longer day, the more precise reporting might not be needed.
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time", meta = (AllowPrivateAccess = "true"))
	//float currentTimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time", meta=(AllowPrivateAccess="true"))
	bool hasDayNightCycle = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ADirectionalLight* sunLight;

	TObjectPtr<ULightComponent> sunComponent;

	/*
	* Curves are made in the editor and added to the blueprint we have there.
	* TODO: The values in this curve need investigating, they come from the tutorial.
		Time (X)	Clock	G (Pitch)	B (Yaw)		A (Intensity)	Description
		0			00:00	60.0		180.0		0.0				Midnight
		360			06:00	0.0			270.0		0.1				Sunrise
		540			09:00	-45.0		315.0		0.75			Morning
		720			12:00	-60.0		360.0		1.0				Noon
		900			15:00	-45.0		405.0		0.85			Afternoon
		1080		18:00	0.0			450.0		0.2				Sunset
		1440		24:00	60.0		540.0		0.0				Midnight Loop
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveLinearColor> dailySunRotation;

	/* 
	*	The values currently used:
		Day (X)		Season		G (Pitch Offset)	B (Yaw Offset)	A (Intensity Offset)	Description
		0			Jan 1st		20.0				-30.0			-0.3					Winter (Short/Dim days)
		90			Spring		0.0					0.0				0.0						Equinox (Neutral)
		172			Summer		-25.0				30.0			0.5						Solstice (Long/Hot days)
		265			Autumn		0.0					0.0				0.0						Equinox (Neutral)
		365			Dec 31st	20.0				-30.0			-0.3					Return to Winter
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveLinearColor> annualSunRotation;

	/*
	*	The values currently used:
		Time (X)	Clock	Color (R, G, B)		A (Intensity)	Description
		0			00:00	0.05, 0.05, 0.1		0.1				Dark Night Blue
		360			06:00	1.0, 0.4, 0.2		0.5				Firey Orange Sunrise
		540			09:00	0.8, 0.9, 1.0		1.0				Bright Morning Blue
		720			12:00	1.0, 1.0, 1.0		1.0				Clean White Noon
		1080		18:00	1.0, 0.2, 0.1		0.6				Deep Red Sunset
		1200		20:00	0.1, 0.1, 0.2		0.2				Twilight
		1440		24:00	0.05, 0.05, 0.1		0.1				Night Loop
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveLinearColor> skyLightDailyColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ASkyLight* skyLight;

	TObjectPtr<USkyLightComponent> skyLightComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float maxSunIntensity = 20.0f;

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
