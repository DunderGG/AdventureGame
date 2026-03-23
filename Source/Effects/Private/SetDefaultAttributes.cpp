// Copyright dunder.gg. All Rights Reserved.

#include "SetDefaultAttributes.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"

/*
* TODO: This works but its ugly... can it be fixed?
*    If we follow the theme of data-driven design, maybe use a struct to hold the default values somehow?
*/
void USetDefaultAttributes::PostInitProperties()
{
	Super::PostInitProperties();
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetMaxHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultHealth);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultHealth/2);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetMaxStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStamina);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStamina/2);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetMaxStrengthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetStrengthAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(mod);

	mod.Attribute = UPlayerAttributeSet::GetCharacterLevelAttribute();
	mod.ModifierOp = EGameplayModOp::Override;
	mod.ModifierMagnitude = FScalableFloat(defaultLevel);
	Modifiers.Add(mod);
}


