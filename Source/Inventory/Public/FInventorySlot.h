#pragma once
#include "CoreMinimal.h"
#include "FInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_USTRUCT_BODY()

	// The blueprints should only be able to see, not edit
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName itemId = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int itemQuantity = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float itemCurrentDurability = -1.0f;

	FInventorySlot() : 
		itemId{ NAME_None },
		itemQuantity{ 0 }, 
		itemCurrentDurability{ -1.0f }
	{
	}

	// ***** HELPERS *****
	FORCEINLINE bool isValid() const
	{
		return !itemId.IsNone() && itemQuantity > 0;
	}

	FORCEINLINE void clear()
	{
		itemId = NAME_None;
		itemQuantity = 0;
		itemCurrentDurability = -1.0f;
	}
};
