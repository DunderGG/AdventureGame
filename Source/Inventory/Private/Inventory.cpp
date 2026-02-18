// Copyright dunder.gg. All Rights Reserved.

#include "Inventory.h"

// Sets default values
AInventory::AInventory()
	: totalNumberOfSlots{10}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	// Fill our inventory array with inventory slots
	resizeInventory();
}

bool AInventory::setNumberOfSlots(const int newNumber)
{
	if (newNumber < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot set number of slots to a negative value."));
		return false;
	}
	totalNumberOfSlots = newNumber;
	resizeInventory();
	return true;
}

/*
* Resizes the inventory array to match the total number of slots. 
	If the inventory is too small, it fills it with empty slots. 
	If the inventory is too large, it drops items until it matches the new size.
*/
void AInventory::resizeInventory()
{
	// Inventory is full
	if (inventory.Num() == totalNumberOfSlots)
	{
		return;
	}

	// If we are under the size limit, fill our inventory array with inventory slots
	if (inventory.Num() < totalNumberOfSlots)
	{
		while (inventory.Num() < totalNumberOfSlots)
		{
			inventory.Add(FInventorySlot());
		}
		UE_LOG(LogTemp, Warning, TEXT("Inventory::resizeInventory(): Inventory resized to %d slots. Added empty slots."), totalNumberOfSlots);
		return;
	}
	
	// If we are over the size limit... drop stuff
	if (inventory.Num() > totalNumberOfSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory::resizeInventory(): Inventory resized to %d slots. Dropping extra items."), totalNumberOfSlots);
		while (inventory.Num() > totalNumberOfSlots)
		{
			// TODO: Drop extra items on to the ground somehow instead of just deleting them... 
			//	Maybe add a "drop item" function that can be called here?
			FInventorySlot droppedItem = inventory.Pop();
			UE_LOG(LogTemp, Warning, TEXT("Inventory::resizeInventory(): Dropping item %s "), *droppedItem.itemId.ToString());
		}
		return;
	}
}
