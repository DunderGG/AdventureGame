

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ESeasonalEventType.h"
#include "SeasonalEventLibrary.generated.h"

/**
 *
 */
UCLASS()
class SEASONALEVENTS_API USeasonalEventLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

private:
	ESeasonalEventType convertDayOfYearToSeasonalEvent(const FDateTime& dateTime) const;

	bool isEasterSunday(const FDateTime& dateTime) const;

protected:

public:
	UFUNCTION(BlueprintCallable, Category = "Seasonal Events", meta = (ToolTip = "Convert Date to a Seasonal Event"))
	ESeasonalEventType convertDateTimeToSeasonalEvent(const FDateTime& dateTime);
};
