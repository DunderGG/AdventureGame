// Copyright dunder.gg. All Rights Reserved.

#include "Inventory.h"

// Sets default values
AInventory::AInventory()
	: totalNumberOfSlots{10}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

/*
* The logic here is:
*	1. Check if there is an existing stack of items in a slot we can add the new item to, 
		and if so, add to that stack until it is full or we have added all we need to add.

* 	2. After checking all stacks, if we still have more to add, find an empty slot to add to, 
		and add to that stack until it is full or we have added all we need to add.

* 	3. After checking all stacks and empty slots, if we still have more to add, 
		log a warning and return the number we still have to add (which should be 0 if we added everything successfully).
*/
int AInventory::addItemToInventory(const FItemData& itemToAdd, const int quantity, const float durability)
{
	// A counter to keep track of how many we still have to add to inventory
	int numberToAdd = quantity;

	// 1. Check if there is an existing stack of items in a slot we can add the new item to
	for (auto itemSlot : inventory)
	{
		if (!itemSlot.isValid() ||			// Check that the item slot is valid
			itemToAdd.id != itemSlot.itemId ||	// Check if the item slot is holding the same item
			itemSlot.itemQuantity >= itemToAdd.maxStackSize)	// Check if the item slot is already at max stack size
		{
			continue;
		}

		// We found a stack to add to, how many can we add to that stack?
		int spaceLeftInSlot = itemToAdd.maxStackSize - itemSlot.itemQuantity;
		int numberToAddToSlot = FMath::Min(spaceLeftInSlot, numberToAdd);
		// Add that amount to the item slot, and if that was all we're adding, break.
		itemSlot.itemQuantity += numberToAddToSlot;
		numberToAdd -= numberToAddToSlot;
		if (numberToAdd <= 0)
		{
			return numberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
		}
	}
	
	// 2. After checking all stacks, if we still have more to add, find an empty slot to add to.
	for (auto itemSlot : inventory)
	{
		// This means the item slot is already holding something, i.e. not empty.
		if (itemSlot.isValid())
		{
			continue;
		}

		// We found an empty slot, how many can we add to that stack?
		int numberToAddToSlot = FMath::Min(itemToAdd.maxStackSize, numberToAdd);
		// Add that amount to the item slot, and if that was all we're adding, break.
		itemSlot.setItemData(itemToAdd.id, numberToAddToSlot, durability, itemToAdd.weight);
		numberToAdd -= numberToAddToSlot;
		if (numberToAdd <= 0)
		{
			return numberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
		}
	}

	// 3. After checking all stacks and empty slots, if we still have more to add, log a warning and return the number we still have to add.
	UE_LOG(LogTemp, Warning, TEXT("We still have %d items to add."), numberToAdd);
	return numberToAdd;
}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
	// Fill our inventory array with inventory slots
	resizeInventoryArray();
	updateWeight();
}

bool AInventory::setNumberOfSlots(const int newNumber)
{
	if (newNumber < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot set number of slots to a negative value."));
		return false;
	}
	totalNumberOfSlots = newNumber;
	resizeInventoryArray();
	updateWeight();
	return true;
}

/*
* Resizes the inventory array to match the total number of slots. 
	If the inventory is too small, it fills it with empty slots. 
	If the inventory is too large, it drops items until it matches the new size.
*/
void AInventory::resizeInventoryArray()
{
	// Inventory is correct size already
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

// Loop through the inventory slots array and add up all the item weights
void AInventory::updateWeight()
{
	// This is not really needed, but good for debugging later.
	float tempCurrentCarryWeight = 0.0f;
	for (const FInventorySlot& slot : inventory)
	{
		if (slot.isValid())
		{
			tempCurrentCarryWeight += slot.itemWeight * slot.itemQuantity;
		}
	}
	currentCarryWeight = tempCurrentCarryWeight;
}
