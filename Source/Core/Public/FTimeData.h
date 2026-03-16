#pragma once

#include "CoreMinimal.h"
#include "FTimeData.generated.h"

USTRUCT(BlueprintType)
struct FTimeData
{
	//GENERATED_USTRUCT_BODY() No longer needed apparently.
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int year = 2026;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int dayOfYear = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int month = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int dayOfMonth = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int hour = 7;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int minute = 30;

	float getTimeOfDay() const {return (hour * 60) + minute; };
};