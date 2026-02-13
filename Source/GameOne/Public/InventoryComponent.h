// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UEquippableToolDefinition;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEONE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	* This inventory only stores tools, but you can expand this to include any type of item you want. 
		A more generic implementation would store only UItemDefinition or TSubclassOf<UItemDefinition> values 
		to build a more complex inventory with UI, icons, sound effects, cost, and other item properties. 
	*/
	// The array of tools stored in inventory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
	TArray<UEquippableToolDefinition*> toolInventory;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

		
};
