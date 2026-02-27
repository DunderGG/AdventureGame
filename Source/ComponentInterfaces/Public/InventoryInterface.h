

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FItemData.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	The functions in the interface have slightly different names than the functions in the implementation class, 
 *      so that function names don't overlap.
 * 
 *	Each _Impl function will return the worst-case here.
 */
class COMPONENTINTERFACES_API IInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	int addItem(const FItemData& itemData, const int quantity, const float durability);
	virtual int addItem_Impl(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY)
	{
		return quantity;
	};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	bool removeItem(const FName itemId, const int quantity);
	virtual bool removeItem_Impl(const FName itemId, const int quantity)
	{
		return false;
	};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	void transferItemFromInv();
	virtual void transferItemFromInv_Impl() {};

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	bool setNumberOfSlots(const int newNumber);
	virtual bool setNumberOfSlots_Impl(const int newNumber)
	{
		return false;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	float getCarryCapacityPercentile() const;
	virtual float getCarryCapacityPercentile_Impl() const 
	{
		return 0.0f;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory")
	int addItemAtSlot(const FItemData& itemData, const int quantity, const float durability, const int index);
	virtual int addItemAtSlot_Impl(const FItemData& itemData, const int quantity, const float durability = DEFAULT_DURABILITY, const int index = -1)
	{
		return quantity;
	}
};
