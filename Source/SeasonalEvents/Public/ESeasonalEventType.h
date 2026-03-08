#pragma once

#include "CoreMinimal.h"
#include "ESeasonalEventType.generated.h"

UENUM(BlueprintType)
enum class ESeasonalEventType : uint8
{
	SE_CHRISTMAS UMETA(DisplayName = "Christmas"),
	SE_HALLOWEEN UMETA(DisplayName = "Halloween"),
	SE_EASTER UMETA(DisplayName = "Easter"),
	SE_VALENTINES UMETA(DisplayName = "Valentine's Day"),
	SE_NONE UMETA(DisplayName = "None")
};
