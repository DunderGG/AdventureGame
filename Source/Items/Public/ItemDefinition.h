// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FItemData.h"
#include "ItemDefinition.generated.h"

/*
* From Epic:
* Data Table rows are similar to CSV rows, meant for holding textual data and not storing full assets. 
* To optimize data management, we recommend bundling information like an item’s mesh, material, 
*  and animations in a DataAsset as this is the central place where all data about one particular item lives. 
* So, the item’s static mesh property is here in UItemDefinition instead of in the FItemData struct. 
* 
* 
* Not sure how well multiple inheritance will work here.
* Might have to just add all the FItemData properties here as was suggested.
* But that makes it kinda annoying to add new properties, not a big deal maybe.
*/
UCLASS(BlueprintType, Blueprintable)
class ITEMS_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()
	
private:
protected:
public:
	////////////////////////
	// COPIED FROM FItemData
	////////////////////////
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

	///////////////////////////////////////
	// ADDED ON TOP OF FItemData PROPERTIES
	///////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> skeletalMesh = nullptr;

	UItemDefinition() :
		id{ NAME_None },
		maxStackSize{ 0 },
		weight{ 1.0f },
		maxDurability{ DEFAULT_DURABILITY },
		quality{ EItemQuality::Common },
		useStaticMesh{ true },
		name{ FText::GetEmpty() },
		description{ FText::GetEmpty() },
		type{ EItemType::Miscellaneous },
		icon{ nullptr },
		mesh{ nullptr },
		skeletalMesh{ nullptr }
	{
	}

	FORCEINLINE bool isValid() const
	{
		//TODO Add that the mesh needs to be valid if useStaticMesh is true?
		return !id.IsNone() && maxStackSize > 0;
	}

	FORCEINLINE void clear() 
	{
		// From FItemData
		id = NAME_None;
		maxStackSize = 0;
		weight = 1.0f;
		maxDurability = DEFAULT_DURABILITY;
		quality = EItemQuality::Common;
		useStaticMesh = true;
		name = FText::GetEmpty();
		description = FText::GetEmpty();
		type = EItemType::Miscellaneous;

		// From UItemDefinition
		icon = nullptr;
		mesh = nullptr;
		skeletalMesh = nullptr;
	}
};
