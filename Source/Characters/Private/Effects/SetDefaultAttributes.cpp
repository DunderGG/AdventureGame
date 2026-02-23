// Copyright dunder.gg. All Rights Reserved.

#include "Effects/SetDefaultAttributes.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "AGCharacterAttributeSet.h"
#include "Effects/SetDefaultAttributes.h"

/*
* TODO: This works but its ugly... can it be fixed?
*    If we follow the theme of data-driven design, maybe use a struct to hold the default values somehow?
*/
void USetDefaultAttributes::PostInitProperties()
{
	Super::PostInitProperties();
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo mod;
	mod.Attribute = UAGCharacterAttributeSet::GetMaxHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultHealth);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultHealth);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetMaxStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStamina);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStamina);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetMaxStrengthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetStrengthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(mod);

	mod.Attribute = UAGCharacterAttributeSet::GetCharacterLevelAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultLevel);
	Modifiers.Add(mod);
}


