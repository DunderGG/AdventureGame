// Copyright dunder.gg. All Rights Reserved.

#include "InventoryComponent.h"

// Sets default values
UInventoryComponent::UInventoryComponent()
	: totalNumberOfSlots{10}
{

}

/*
* TODO: We don't have functionality to drop items to world yet.
*/
void UInventoryComponent::dropItemAtIndexToWorld(const int index)
{

}

void UInventoryComponent::dropItemFromSlotData(const FInventorySlot& slotData)
{
}

/*
* @return The number of items we failed to add to inventory. Should be 0 if everything was added successfully.
* 
* The logic here is:
*	1. Check if there is an existing stack of items in a slot we can add the new item to, 
		and if so, add to that stack until it is full or we have added all we need to add.

* 	2. After checking all stacks, if we still have more to add, find an empty slot to add to, 
		and add to that stack until it is full or we have added all we need to add.

* 	3. After checking all stacks and empty slots, if we still have more to add, 
		log a warning and return the number we still have to add (which should be 0 if we added everything successfully).
*/
int UInventoryComponent::addItemToInventory(const FItemData& itemToAdd, const int quantity, const float durability)
{
	// A counter to keep track of how many we still have to add to inventory
	int totalNumberToAdd = quantity;

	// 1. Check if there is an existing stack of items in a slot we can add the new item to
	for (auto& itemSlot : inventory)
	{
		if (!itemSlot.isValid() ||			// Check that the item slot is valid
			itemToAdd.id != itemSlot.itemId ||	// Check if the item slot is not holding the same item
			itemSlot.itemQuantity >= itemToAdd.maxStackSize)	// Check if the item slot is already at max stack size
		{
			// Could not add to this item slot, move to the next one.
			continue;
		}

		// We found a stack to add to, how many can we add to that stack?
		int spaceLeftInSlot = itemToAdd.maxStackSize - itemSlot.itemQuantity;
		int numberToAddToSlot = FMath::Min(spaceLeftInSlot, totalNumberToAdd);
		// Add that amount to the item slot, and if that was all we're adding, break.
		itemSlot.itemQuantity += numberToAddToSlot;
		totalNumberToAdd -= numberToAddToSlot;
		if (totalNumberToAdd <= 0)
		{
			updateWeight();
			return totalNumberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
		}
	}
	
	// 2. After checking all stacks, if we still have more to add, find an empty slot to add to.
	for (auto& itemSlot : inventory)
	{
		// This means the item slot is already holding something, i.e. not empty, so move on.
		if (itemSlot.isValid())
		{
			continue;
		}

		// We found an empty slot, how many can we add to that stack?
		int numberToAddToSlot = FMath::Min(itemToAdd.maxStackSize, totalNumberToAdd);
		// Add that amount to the item slot, and if that was all we're adding, break.
		itemSlot.setItemData(itemToAdd.id, numberToAddToSlot, durability, itemToAdd.weight);
		totalNumberToAdd -= numberToAddToSlot;
		if (totalNumberToAdd <= 0)
		{
			updateWeight();
			return totalNumberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
		}
	}

	// 3. After checking all stacks and empty slots, if we still have more to add, log a warning and return the number we still have to add.
	UE_LOG(LogTemp, Warning, TEXT("UInventory::addItemToInventory(): We still have %d items to add."), totalNumberToAdd);
	updateWeight();
	return totalNumberToAdd;
}

/*
*   Three scenarios:
*	   1. We are trying to add the item to an out of bounds slot, so we try to add it to the inventory normally.
* 	   2. We are trying to add the item to an empty slot, so we add it to that slot and if we still have more to add, we add the rest to the inventory normally.
*	   3a. We are trying to add the item to a slot that is already holding something, so we move the pre-existing item to an empty slot.
*      3b. If we fail to find an empty slot to move the pre-existing item to, we drop it instead.
*/
int UInventoryComponent::addItemToSlot(const FItemData& itemData, const int quantity, const float durability, const int index)
{
	// 1. If the index is out of bounds, try adding the item to the inventory normally.
	if (index < 0 || index >= inventory.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::addItemToSlot(): Cannot add item to slot %d because it is out of bounds. Will try to add to other slots."), index);
		return addItemToInventory(itemData, quantity, durability);
	}

	int totalNumberToAdd = quantity;

	// 2. Are we trying to add the item to an empty slot?
	if (!inventory[index].isValid())
	{
		addItemToEmptySlot(itemData, quantity, index, durability);
		
		updateWeight();
		return totalNumberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
	}

	//	 TODO: This logic seems weird. Usually when you add an item to a non-empty slot, you would swap places of the items.
	// 3a. We are trying to add the item to a slot that is already holding something, so we need to move the pre-existing item to an empty slot.
	for (auto& itemSlot : inventory)
	{
		// Slot not empty, move to the next.
		if (itemSlot.isValid())
		{
			continue;
		}

		itemSlot = inventory[index];	    // The empty slot now holds the pre-existing item.
		inventory[index].resetItemData();	// Clear the item data from the original slot, which is now empty.
			
		// Now we can add the new item to the original slot, which is now empty.
		// TODO: Make sure the logic is right here after we refactored it into a separate function.
		totalNumberToAdd = addItemToEmptySlot(itemData, quantity, index, durability);

		updateWeight();
		return totalNumberToAdd;		// This should never be negative, but just in case, we return it so we can log it for debugging purposes.
		
	}

	// 3b. We failed to find an empty slot to move the pre-existing item to, so we drop it instead.
	dropItemFromSlotData(inventory[index]);
	inventory[index].resetItemData();

	// TODO: Make sure the logic is right here after we refactored it into a separate function.
	totalNumberToAdd = addItemToEmptySlot(itemData, quantity, index, durability);

	updateWeight();
	return totalNumberToAdd;
}

int UInventoryComponent::addItemToEmptySlot(const FItemData& itemData, const int quantity, const int index, const float durability)
{
	int totalNumberToAdd = quantity;
	int numberToAddToSlot = FMath::Min(itemData.maxStackSize, quantity);
	inventory[index].setItemData(itemData.id, numberToAddToSlot, durability, itemData.weight);
	totalNumberToAdd -= numberToAddToSlot;

	// If we added maxStackSize to an item slot, and still have more to add, add the rest to the inventory normally.
	if (totalNumberToAdd > 0)
	{
		return addItemToInventory(itemData, totalNumberToAdd, durability);
	}
	return totalNumberToAdd;
}


/*
* We are saving the item indexes first, because we want to make sure there are actually enough items to remove before we start removing them.
*/
bool UInventoryComponent::removeItemFromInventory(const FName itemId, const int quantity)
{
	int totalNumberToRemove = quantity;
	TArray<int> itemAtIndexes;

	// Loop through the whole inventory, from back to front.
	for (int index = inventory.Num() - 1; index >= 0; index--)
	{
		// The slot holds the item we are looking for
		if (inventory[index].itemId == itemId)
		{
			// So save the index of that slot.
			itemAtIndexes.Add(index);
			// How many can we remove?
			const int removedFromThisSlot = FMath::Min(inventory[index].itemQuantity, totalNumberToRemove);
			// How many more do we need to remove?
			totalNumberToRemove -= removedFromThisSlot;

			// If we removed enough, break. Should never be negative, it's an edge case.
			//   Not really needed, I guess, but a failsafe.
			if (totalNumberToRemove <= 0)
			{
				break;
			}
		}
	}

	// We failed finding enough in the inventory, so we do not want to start removing any.
	//   Nothing removed yet, so no need to updateWeight().
	if (totalNumberToRemove > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::removeItemFromInventory(): Could not remove %d items because there were not enough in inventory."), totalNumberToRemove);
		return false;
	}

	// We found enough, so now we can start removing them.
	totalNumberToRemove = quantity;
	for (const auto index : itemAtIndexes)
	{
		// How many can we remove from this stack?
		int stackSize = inventory[index].itemQuantity;
		int toRemoveFromThisStack = FMath::Min(stackSize, totalNumberToRemove);

		// Remove that number of items from this item slot, and check if it is now empty.
		// Not sure if this will work, itemId.IsNone() will probably not be false???
		inventory[index].itemQuantity -= toRemoveFromThisStack;
		if (!inventory[index].isValid())
		{
			// Reset the item slot data.
			inventory[index].resetItemData();
		}

		totalNumberToRemove -= toRemoveFromThisStack;
		if (totalNumberToRemove <= 0)
		{
			break;
		}
	}
	// Update the weight after we have removed the items.
	updateWeight();
	return true;
}

// Called when the game starts or when spawned
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Fill our inventory array with inventory slots
	resizeInventoryArray();
	updateWeight();
}

/*
* 
*/
bool UInventoryComponent::setNumberOfInventorySlots(const int newNumber)
{
	if (newNumber < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::setNumberOfInventorySlots(): Cannot set number of slots to a negative value."));
		return false;
	}
	totalNumberOfSlots = newNumber;
	resizeInventoryArray();
	updateWeight();
	return true;
}

/*
* The logic here is:
*	1. Find the index in the array where the item we are transferring is located. If we can't find it, return false.
*	2. Remove the items from the source inventory.
*	3. Add the items to the target inventory.
*	4. If we fail to add all the items to the target inventory, add the remaining items back to the source inventory and (MAYBE?) return false.
*/
bool UInventoryComponent::transferItemFromInventory(UInventoryComponent* targetInventory, const FName itemId, const int quantity, const int targetIndex)
{
	// Is the target inventory valid?
	if (!IsValid(targetInventory))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::transferItemFromInventory(): Target inventory is not valid."));
		return false;
	}

	int sourceIndex = DEFAULT_INDEX;
	// Where is the item we are transferring?
	// TODO: break out into helper function.
	for (int i = 0; i < inventory.Num(); i++)
	{
		if (inventory[i].itemId == itemId)
		{
			sourceIndex = i;
			break;
		}
	}

	// Did we find the item?
	if (sourceIndex == DEFAULT_INDEX)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::transferItemFromInventory(): Could not find item in inventory."));
		return false;
	}

	// Store the item slot we are transferring.
	const auto& tempItem = inventory[sourceIndex];

	// Remove items from inventory, return false if there were not enough items to remove.
	if (!removeItemFromInventory(itemId, quantity))
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::transferItemFromInventory(): Failed to remove item from source inventory."));
		return false;
	}

	// TODO: fix when have a "data manager".
	FItemData itemToTransfer;

	// How many items are we moving?
	int numberRemaining = quantity;

	// Now add the item to the target inventory, return false if we failed to add the item to the target inventory.
	//   If targetIndex is out of bounds, addItemToSlot will call addItemToInventory, and add items normally.
	numberRemaining = targetInventory->addItemToSlot(itemToTransfer, quantity, tempItem.itemCurrentDurability, targetIndex);

	// If we failed to add all the items to the target inventory, we need to add the remaining items back to the source inventory slot, and return false.
	if (numberRemaining > 0)
	{
		addItemToSlot(itemToTransfer, numberRemaining, tempItem.itemCurrentDurability, sourceIndex);
		// TODO: I'm not sure about returning false here, check later.
		return false;
	}

	return true;
}


/*
* Resizes the inventory array to match the total number of slots. 
	If the inventory is too small, it fills it with empty slots. 
	If the inventory is too large, it drops items until it matches the new size.
*/
void UInventoryComponent::resizeInventoryArray()
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
		UE_LOG(LogTemp, Warning, TEXT("UInventory::resizeInventory(): Inventory resized to %d slots. Added empty slots."), totalNumberOfSlots);
		return;
	}
	
	// If we are over the size limit... drop stuff
	if (inventory.Num() > totalNumberOfSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventory::resizeInventory(): Inventory resized to %d slots. Dropping extra items."), totalNumberOfSlots);
		while (inventory.Num() > totalNumberOfSlots)
		{
			// TODO: Drop extra items on to the ground somehow instead of just deleting them... 
			//	Maybe add a "drop item" function that can be called here?
			FInventorySlot droppedItem = inventory.Pop();
			UE_LOG(LogTemp, Warning, TEXT("UInventory::resizeInventory(): Dropping item %s "), *droppedItem.itemId.ToString());
			dropItemFromSlotData(droppedItem);
		}
		updateWeight();
		return;
	}
}

/* 
*	Loop through the inventory slots array and add up all the item weights
*	Important to do this after every add or remove of item!
*/
void UInventoryComponent::updateWeight()
{
	// This is not really needed, but good for debugging later.
	float tempCurrentCarryWeight = 0.0f;
	for (const auto& slot : inventory)
	{
		if (slot.isValid())
		{
			tempCurrentCarryWeight += slot.itemWeight * slot.itemQuantity;
		}
	}
	currentCarryWeight = tempCurrentCarryWeight;
}

