// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SneakEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"

void USneakEffect::PostInitProperties()
{
	Super::PostInitProperties();

	// Periodic deduction for stamina
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	// The Modifier for Movement Speed
	FGameplayModifierInfo speedMod;
	speedMod.Attribute = UPlayerAttributeSet::GetMoveSpeedAttribute();
	speedMod.ModifierOp = EGameplayModOp::Override;

	// 1. Create and configure the AttributeBasedFloat struct
	FAttributeBasedFloat AttributeBasedFloat;

	// Define which attribute to capture
	AttributeBasedFloat.BackingAttribute.AttributeToCapture = UPlayerAttributeSet::GetSneakSpeedAttribute();

	// Define where to capture it from (Target is the character this effect is on)
	AttributeBasedFloat.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Use the actual value of the attribute (rather than a change or delta)
	AttributeBasedFloat.AttributeCalculationType = EAttributeBasedFloatCalculationType::AttributeMagnitude;

	// In C++, the coefficient defaults to 0.0f, so we MUST set it to 1.0f 
	// or the resulting value will always be zero (800 * 0 = 0).
	AttributeBasedFloat.Coefficient = 1.0f;

	// 2. Wrap the struct into the ModifierMagnitude using the appropriate constructor
	speedMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBasedFloat);
	
	Modifiers.Add(speedMod);

	// Add the isSprinting tag to identify this state
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsSneaking);
	component.SetAndApplyTargetTagChanges(tagContainer);
}
