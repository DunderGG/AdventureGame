// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SneakEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"

/*
* Practically identical to SprintEffect, so look there for more documentation.
*/
void USneakEffect::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;

	// The Modifier for Movement Speed
	FGameplayModifierInfo speedMod;
	speedMod.Attribute = UPlayerAttributeSet::GetMoveSpeedAttribute();
	speedMod.ModifierOp = EGameplayModOp::Override;
	FAttributeBasedFloat AttributeBasedFloat;
	AttributeBasedFloat.BackingAttribute.AttributeToCapture = UPlayerAttributeSet::GetSneakSpeedAttribute();
	AttributeBasedFloat.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	speedMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBasedFloat);
	Modifiers.Add(speedMod);

	// The Modifier for Noise
	FGameplayModifierInfo noiseMod;
	noiseMod.Attribute = UPlayerAttributeSet::GetNoiseAttribute();
	noiseMod.ModifierOp = EGameplayModOp::MultiplyCompound;
	noiseMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(noiseMultiplier));
	Modifiers.Add(noiseMod);

	// Add the isSneaking tag to identify this state
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsSneaking);
	component.SetAndApplyTargetTagChanges(tagContainer);
}
