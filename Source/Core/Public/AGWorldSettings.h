// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "AGWorldSettings.generated.h"

class UCurveFloat;

/**
 * 
 */
UCLASS()
class COREDATA_API AAGWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
private:

protected:

public:
	/*
	* The values currently used:
		Time ($X$)	Real Time	Temp ($Y$)	Description
		0			Midnight	15.0		Stable night temperature.
		300			05:00 AM	12.0		Minimum: Just before sunrise is usually the coolest.
		720			12:00 PM	22.0		Rapidly warming as the sun is highest.
		960			04:00 PM	28.0		Maximum: Peak heat due to ground radiation.
		1200		08:00 PM	20.0		Cooling down after sunset.
		1440		Midnight	15.0		Loops back to match start.
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ToolTip = "Daily Temperature Curve"), Category = "Temperature")
	TSoftObjectPtr<UCurveFloat> dailyTemperatureCurve;

	/*
	*	The values currently used:
		Day		Season ($X$)	Offset ($Y$)	Description
		0		Jan 1st			-12.0			Cold winter baseline.
		45		Mid-Feb			-15.0			Minimum: Usually the coldest part of the year.
		105		Mid-Apr			0.0				Neutral: Baseline spring temperature.
		200		Late-July		+15.0			Maximum: Peak summer heat.
		290		Mid-Oct			0.0				Neutral: Baseline autumn temperature.
		365		Dec 31st		-12.0			Returns to start for a perfect loop.

		In the code where we sample these curves, 
		remember to add the values together: 
		currentTemp = dailyCurve->GetFloatValue(TimeMinutes) + annualCurve->GetFloatValue(DayOfYear);
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ToolTip = "Annual Temperature Curve, an offset to the daily curve"), Category = "Temperature")
	TSoftObjectPtr<UCurveFloat> annualTemperatureCurve;
};
