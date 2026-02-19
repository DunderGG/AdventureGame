// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FInventorySlot.h"
#include "FItemData.h"
#include "Inventory.generated.h"

UCLASS()
class INVENTORY_API AInventory : public AActor
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int totalNumberOfSlots = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float maxCarryWeight = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float currentCarryWeight = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInventorySlot> inventory;

protected:
	virtual void resizeInventoryArray();
	virtual void updateWeight();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AInventory();

	// We removed "UFUNCTION(BlueprintCallable)" because it wouldn't work with the global DEFAULT_DURABILITY... 
	//   Will have to find solution if it needs to be BlueprintCallable.
	int addItemToInventory(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY);

	UFUNCTION(BlueprintCallable)
	bool setNumberOfSlots(const int newNumber);

	// Can be used to show if we are over weight limit or not, and if so by how much.
	//   If above 1, divide player speed with this number for example.
	// BlueprintPure means i
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float getCarryWeightPercentile() const { return currentCarryWeight / maxCarryWeight; }
};
