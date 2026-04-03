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
	int month = 6;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int dayOfMonth = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int hour = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame)
	int minute = 0;

	float getTimeOfDay() const {return (hour * 60) + minute; };
};