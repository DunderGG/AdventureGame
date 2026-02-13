// Copyright dunder.gg. All Rights Reserved.

#include "Data/EquippableToolDefinition.h"

// This should be just like the overridden function in ItemDefinition,
//	but we will also copy the ToolAsset
UEquippableToolDefinition* UEquippableToolDefinition::createItemCopy() const
{
	UEquippableToolDefinition* item = NewObject<UEquippableToolDefinition>(StaticClass());

	item->id = this->id;
	item->itemType = this->itemType;
	item->itemText = this->itemText;
	item->worldMesh = this->worldMesh;
	item->toolAsset = this->toolAsset;

	return item;
}