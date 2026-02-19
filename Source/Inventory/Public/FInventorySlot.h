#pragma once

#include "CoreMinimal.h"
#include "FItemData.h"
#include "FInventorySlot.generated.h"

/*
* We have this struct to represent an item slot, intead of just directly
*	using the ItemData struct, because there is a lot of information there
*	we don't need to store in the inventory.
*/
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
	float itemCurrentDurability = DEFAULT_DURABILITY;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float itemWeight = 0.0f;

	FInventorySlot() : 
		itemId{ NAME_None },
		itemQuantity{ 0 }, 
		itemCurrentDurability{ DEFAULT_DURABILITY },
		itemWeight{ 0.0f }
	{
	}

	// Put an item in this slot.
	void setItemData(const FName id, const int quantity, const float durability, const float weight)
	{
		itemId = id;
		itemQuantity = quantity;
		itemCurrentDurability = durability;
		itemWeight = weight;
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
		itemCurrentDurability = DEFAULT_DURABILITY;
		itemWeight = 0.0f;
	}
};
