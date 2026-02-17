#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quantity = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxDurability = 100;

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

	FORCEINLINE bool isValid() const
	{
		return !id.IsNone() && quantity > 0;
	}

	FORCEINLINE void clear()
	{
		id = NAME_None;
		quantity = 0;
		maxDurability = 100;
		icon = nullptr;
		mesh = nullptr;
		skeletalMesh = nullptr;
		name = FText::GetEmpty();
		FText::GetEmpty();
	}
};
