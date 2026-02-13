// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemData.h"
#include "ItemDefinition.generated.h"

 // BlueprintType and Blueprintable specifiers exposes this as a base class for creating Blueprints
 // Defines a basic item with a skeletal mesh that can be built from the editor.
UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// Creates and returns a copy of the item
	virtual UItemDefinition* createItemCopy() const;

	// The three things we defined in ItemData.h
	// Your item definition gets the same data as the FItemData struct 
	// so you don’t need to reference the original table when you want to get information about the item.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName id;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemText itemText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType itemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	TSoftObjectPtr<USkeletalMesh> worldMesh;
};