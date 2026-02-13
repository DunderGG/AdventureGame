// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDefinition.h"
#include "EquippableToolDefinition.generated.h"

// The Actor representing the equippables in-game
class AEquippableToolBase;
// Each equippable should hold a reference to an IMC to apply to the character holding it
class UInputMappingContext;

UCLASS(BlueprintType, Blueprintable)
class GAMEONE_API UEquippableToolDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	//	TSubclassOf<AEquippableToolBase> is a template wrapper around UClass that enables you
	//	to reference Blueprint subclasses of AEquippableToolBase while ensuring type safety.
	//		It’s useful in gameplay scenarios where you want to spawn different types of actors dynamically.
	// The tool actor associated with this item, which we'll use to dynamically spawn a tool when it gets equipped.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEquippableToolBase> toolAsset;
	
	// Create and return a copy of this item, overrides the ItemDefinition.h
	UFUNCTION()
	virtual UEquippableToolDefinition* createItemCopy() const override;
};
