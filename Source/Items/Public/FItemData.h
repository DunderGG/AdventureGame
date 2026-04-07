#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemData.generated.h"

class UItemDefinition;

// By default, items do not use durability.
const float DEFAULT_DURABILITY = -1.0f;

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Poor UMETA(DisplayName = "Poor"),
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Equipment UMETA(DisplayName = "Equipment"),
	Material UMETA(DisplayName = "Material"),
	Miscellaneous UMETA(DisplayName = "Miscellaneous")
};

/*
* If adding new properties, make sure to update the constructor and clear() function to set default values.
*/
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 maxStackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float weight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxDurability = DEFAULT_DURABILITY; // 0-1 as percentile, <0 does not use durability

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemQuality quality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool useStaticMesh = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText name = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText description = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UItemDefinition> itemDefinition = nullptr;

	FItemData() :
		id{NAME_None},
		maxStackSize{0},
		weight{1.0f},
		maxDurability{ DEFAULT_DURABILITY },
		quality{EItemQuality::Common},
		useStaticMesh{true},
		name{FText::GetEmpty()},
		description{FText::GetEmpty()},
		type{EItemType::Miscellaneous},
		itemDefinition{nullptr}
	{
	}
	
	// ***** HELPERS *****
	FORCEINLINE bool isValid() const
	{
		return !id.IsNone() && maxStackSize > 0;
	}

	FORCEINLINE void clear()
	{
		id = NAME_None;
		maxStackSize = 0;
		weight = 1.0f;
		maxDurability = DEFAULT_DURABILITY;
		quality = EItemQuality::Common;
		useStaticMesh = true;
		name = FText::GetEmpty();
		description = FText::GetEmpty();
		type = EItemType::Miscellaneous;
		itemDefinition = nullptr;
	}
};
