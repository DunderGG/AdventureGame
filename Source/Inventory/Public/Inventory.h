// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FInventorySlot.h"
#include "FItemData.h"
#include "InventoryInterface.h"
#include "Inventory.generated.h"

UCLASS()
class INVENTORY_API AInventory : public AActor, 
								 public IInventoryInterface
{
	GENERATED_BODY()
	
private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float currentCarryWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float maxCarryWeight = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int totalNumberOfSlots = 10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FInventorySlot> inventory;

	// Helper function to add an item to a specific empty slot. Private because it's just code refactored from addItemToInventory.
	int addItemToEmptySlot(const FItemData& itemData, const int quantity, const int index, const float durability = DEFAULT_DURABILITY);
	void dropItemAtIndexToWorld(const int index);
	void dropItemFromSlotData(const FInventorySlot& slotData);

protected:
	virtual void BeginPlay() override;
	virtual void resizeInventoryArray();
	virtual void updateWeight();

	// We removed "UFUNCTION(BlueprintCallable)" because it wouldn't work with the global DEFAULT_DURABILITY... 
	//   Will have to find solution if it needs to be BlueprintCallable.
	int addItemToInventory(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY);
	
	int addItemToSlot(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY, const int index = -1);

	// Can be used to show if we are over weight limit or not, and if so by how much.
	//   If above 1, divide player speed with this number for example.
	// BlueprintPure means it can be used in Blueprints without modifying the state of the object.
	float getCarryWeightPercentile() const { return currentCarryWeight / maxCarryWeight; }

	bool removeItemFromInventory(const FName itemId, const int quantity);

	bool setNumberOfInventorySlots(const int newNumber);

	void transferItemFromInventory() {};

public:	
	// Sets default values for this actor's properties
	AInventory();

	// Override all the Inventory Interface functions, with the internal functionality.
	int addItem_Impl(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY) override
	{
		return addItemToInventory(itemData, quantity, durability);
	}
	int addItemAtSlot_Impl(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY, const int index = -1) override
	{
		return addItemToSlot(itemData, quantity, durability, index);
	}
	float getCarryCapacityPercentile_Impl() const override
	{
		return getCarryWeightPercentile();
	}
	bool removeItem_Impl(const FName itemId, const int quantity) override
	{
		return removeItemFromInventory(itemId, quantity);
	}
	bool setNumberOfSlots_Impl(const int newNumber) override
	{
		return setNumberOfInventorySlots(newNumber);
	}

};
