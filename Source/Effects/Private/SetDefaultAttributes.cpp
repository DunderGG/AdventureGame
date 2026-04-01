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

	FGameplayModifierInfo maxHealthMod;
	maxHealthMod.Attribute = UPlayerAttributeSet::GetMaxHealthAttribute();
	maxHealthMod.ModifierOp = EGameplayModOp::Override;
	maxHealthMod.ModifierMagnitude = FScalableFloat(defaultHealth);
	Modifiers.Add(maxHealthMod);

	FGameplayModifierInfo healthMod;
	healthMod.Attribute = UPlayerAttributeSet::GetHealthAttribute();
	healthMod.ModifierOp = EGameplayModOp::Override;
	healthMod.ModifierMagnitude = FScalableFloat(defaultHealth/2);
	Modifiers.Add(healthMod);

	FGameplayModifierInfo maxStaminaMod;
	maxStaminaMod.Attribute = UPlayerAttributeSet::GetMaxStaminaAttribute();
	maxStaminaMod.ModifierOp = EGameplayModOp::Override;
	maxStaminaMod.ModifierMagnitude = FScalableFloat(defaultStamina);
	Modifiers.Add(maxStaminaMod);

	FGameplayModifierInfo staminaMod;
	staminaMod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	staminaMod.ModifierOp = EGameplayModOp::Override;
	staminaMod.ModifierMagnitude = FScalableFloat(defaultStamina/2);
	Modifiers.Add(staminaMod);

	FGameplayModifierInfo maxStrengthMod;
	maxStrengthMod.Attribute = UPlayerAttributeSet::GetMaxStrengthAttribute();
	maxStrengthMod.ModifierOp = EGameplayModOp::Override;
	maxStrengthMod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(maxStrengthMod);

	FGameplayModifierInfo strengthMod;
	strengthMod.Attribute = UPlayerAttributeSet::GetStrengthAttribute();
	strengthMod.ModifierOp = EGameplayModOp::Override;
	strengthMod.ModifierMagnitude = FScalableFloat(defaultStrength);
	Modifiers.Add(strengthMod);

	FGameplayModifierInfo levelMod;
	levelMod.Attribute = UPlayerAttributeSet::GetCharacterLevelAttribute();
	levelMod.ModifierOp = EGameplayModOp::Override;
	levelMod.ModifierMagnitude = FScalableFloat(defaultLevel);
	Modifiers.Add(levelMod);

	FGameplayModifierInfo moveSpeedMod;
	moveSpeedMod.Attribute = UPlayerAttributeSet::GetMoveSpeedAttribute();
	moveSpeedMod.ModifierOp = EGameplayModOp::Override;
	moveSpeedMod.ModifierMagnitude = FScalableFloat(defaultMoveSpeed);
	Modifiers.Add(moveSpeedMod);

	FGameplayModifierInfo sprintSpeedMod;
	sprintSpeedMod.Attribute = UPlayerAttributeSet::GetSprintSpeedAttribute();
	sprintSpeedMod.ModifierOp = EGameplayModOp::Override;
	sprintSpeedMod.ModifierMagnitude = FScalableFloat(defaultSprintSpeed);
	Modifiers.Add(sprintSpeedMod);

	FGameplayModifierInfo sneakSpeedMod;
	sneakSpeedMod.Attribute = UPlayerAttributeSet::GetSneakSpeedAttribute();
	sneakSpeedMod.ModifierOp = EGameplayModOp::Override;
	sneakSpeedMod.ModifierMagnitude = FScalableFloat(defaultSneakSpeed);
	Modifiers.Add(sneakSpeedMod);

	FGameplayModifierInfo noiseMod;
	noiseMod.Attribute = UPlayerAttributeSet::GetNoiseAttribute();
	noiseMod.ModifierOp = EGameplayModOp::Override;
	noiseMod.ModifierMagnitude = FScalableFloat(defaultNoise);
	Modifiers.Add(noiseMod);
}


