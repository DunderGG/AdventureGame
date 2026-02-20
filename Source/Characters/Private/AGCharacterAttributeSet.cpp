// Copyright dunder.gg. All Rights Reserved.

#include "AGCharacterAttributeSet.h"

UAGCharacterAttributeSet::UAGCharacterAttributeSet() :
	MaxHealth(100.f),
	Health(100.f)
{
	InitHealth(GetMaxHealth());
}

void UAGCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& attribute, float& newValue)
{
	if (attribute == GetMaxHealthAttribute())
	{
		// Max health is often used as a divisor for calculating health percentages (in the UI for example), so we want to make sure it is never 0 or negative.
		newValue = FMath::Max<float>(1.0f, newValue);
	}
	else if (attribute == GetHealthAttribute())
	{
		// Make sure new current health is between 0 and max health.
		newValue = FMath::Clamp<float>(newValue, 0.0f, GetMaxHealth());
	}
}
