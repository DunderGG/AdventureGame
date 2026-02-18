#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FItemData.generated.h"

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
	float maxDurability = -1.0f; // 0-1 as percentile, <0 does not use durability

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USkeletalMesh> skeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool useStaticMesh = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText name = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText description = FText::GetEmpty();

	FItemData() :
		id{NAME_None},
		maxStackSize{0},
		weight{1.0f},
		maxDurability{-1.0f},
		quality{0},
		icon{nullptr},
		mesh{nullptr},
		skeletalMesh{nullptr},
		useStaticMesh{true},
		name{FText::GetEmpty()},
		description{FText::GetEmpty()}
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
		maxDurability = 0;
		icon = nullptr;
		mesh = nullptr;
		skeletalMesh = nullptr;
		name = FText::GetEmpty();
		FText::GetEmpty();
		weight = 1.0f;
	}
};
