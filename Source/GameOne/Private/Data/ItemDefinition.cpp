// Copyright dunder.gg. All Rights Reserved.

#include "Data/ItemDefinition.h"

/**
*	Creates and returns a copy of this Item Definition.
*	@return	a copy of this item.
*/
UItemDefinition* UItemDefinition::createItemCopy() const
{
	// In Unreal Engine, NewObject<T>() is a templated function for dynamically creating UObject-derived instances at runtime.
	//	UItemDefinition::StaticClass() is the class of the created object.
	//	The class argument is often T::StaticClass(), which gives a UClass pointer representing T's class type.
	//		You can often omit both arguments as UE assumes first argument is the current class and uses T to infer the UClass.
	UItemDefinition* item = NewObject<UItemDefinition>(StaticClass());

	item->id = this->id;
	item->itemType = this->itemType;
	item->itemText = this->itemText;
	item->worldMesh = this->worldMesh;

	return item;
}