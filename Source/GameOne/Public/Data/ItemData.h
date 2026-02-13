// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

// This will be the Data Asset that you can work with in the editor.
class UItemDefinition;

// Defines the type of the item
// UENUM(BlueprintType) means declare this enum to Unreal Header Tool and expose it as data inside Blueprints, allowing it to appear in Data Tables.
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Consumable UMETA(DisplayName = "Consumable")
};

// Contains textual data about the item
USTRUCT(BlueprintType)
struct FItemText
{
	GENERATED_BODY()

	// BlueprintReadOnly exposes the property to Blueprints so it can be read and used in graphs, but prevents it from being modified at runtime.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
};

// Defines a basic item that can be used in a data table.
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	// The id name of this item for referencing in a table row
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName id;

	// The type of the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType itemType;

	// Text struct including the item name and description
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemText itemText;

	// The Data Asset item definition associated with this item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	TSoftObjectPtr<UItemDefinition> itemBase;
};