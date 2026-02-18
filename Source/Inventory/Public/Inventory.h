// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FInventorySlot.h"
#include "Inventory.generated.h"

UCLASS()
class INVENTORY_API AInventory : public AActor
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int totalNumberOfSlots = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInventorySlot> inventory;

protected:
	virtual void resizeInventory();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AInventory();

	bool setNumberOfSlots(const int newNumber);
};
