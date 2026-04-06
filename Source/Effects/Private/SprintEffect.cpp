// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SprintEffect.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "GameplayEffectComponents/RemoveOtherGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"

/****************
* SUPER IMPORTANT to remember when creating effects like this:
* 	1. Set the DurationPolicy to Infinite if you want the effect to last until it's removed.
*   2. DO NOT SET A PERIOD. When an effect has a Period > 0, GAS treats all modifiers as INSTANT,
*      which means they will change the attributes BASE VALUE, not the CURRENT VALUE.
*   3. This means that for sprinting with a stamina cost, we need a separate SprintCost effect that is periodic.
*
* Understanding the difference between BASE and CURRENT values, and what affects what, is CRUCIAL.
*
* In other words:
*	1.	When you apply the effect, it immediately performs an Instant override of your MoveSpeed base value to 800.
*	2.	Every X seconds, it does it again.
*	3.	Because these are "Instant" modifications to the Base Value, they are permanent changes.
*	4.	When you call RemoveActiveGameplayEffectBySourceEffect, GAS stops the periodic timer, 
*			but it does not undo the changes made to the base value because. 
*			From GAS's perspective, those were "Instant" permanent updates.
*
* Two ways to create modifiers are shown here:
*    1. speedMod uses an AttributeBasedFloat, taking the value to set MoveSpeed to from another attribute, SprintSpeed.
*    2. noiseMod uses a simple ScalableFloat, multiplying the Noise attribute by a constant value.
*
* TODO: Investigate how the AttributeBasedFloat works when other movement speed effects are applied.
*		It will probably need something else than a simple Override? Trying MultiplyCompound for now, 
*			but it may need a custom calculation class to properly combine with other speed modifiers.
*    Example: MultiplyCompound: 2.0x base speed. If a potion gives +10%, you get 400 * 2.0 * 1.1 = 880.
*/
void USprintEffect::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;

	// Only allow one instance of the effect, so that we don't move faster and faster.
	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;

	// The Modifier for Movement Speed   (AttributeBasedFloat)
	FGameplayModifierInfo speedMod;
	speedMod.Attribute = UPlayerAttributeSet::GetMoveSpeedAttribute();
	speedMod.ModifierOp = EGameplayModOp::MultiplyCompound;
	// Create and configure the AttributeBasedFloat struct
	FAttributeBasedFloat AttributeBasedFloat;
	// Define which attribute to capture
	AttributeBasedFloat.BackingAttribute.AttributeToCapture = UPlayerAttributeSet::GetSprintSpeedAttribute();
	// Define where to capture it from (Target is the character this effect is on)
	AttributeBasedFloat.BackingAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// Wrap the struct into the ModifierMagnitude using the appropriate constructor
	speedMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(AttributeBasedFloat);
	Modifiers.Add(speedMod);

	// The Modifier for Noise   (ScalableFloat)
	FGameplayModifierInfo noiseMod;
	noiseMod.Attribute = UPlayerAttributeSet::GetNoiseAttribute();
	noiseMod.ModifierOp = EGameplayModOp::MultiplyCompound;
	noiseMod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(noiseMultiplier));
	Modifiers.Add(noiseMod);

	// Add the isSprinting tag to identify this state, 
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsSprinting);
	component.SetAndApplyTargetTagChanges(tagContainer);

	// Create a query that finds any active effect providing the "IsSneaking" tag
	// so that it can be removed when sprinting, to make sure they are mutually exclusive.
	URemoveOtherGameplayEffectComponent& removalComponent = this->FindOrAddComponent<URemoveOtherGameplayEffectComponent>();
	FGameplayEffectQuery sneakQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
		FGameplayTagContainer(AdventureGameplayTags::Gameplay_State_IsSneaking)
	);
	removalComponent.RemoveGameplayEffectQueries.Add(sneakQuery);

	// The IsSprinting tag should only be active on the character whenever the character IsMoving.
	UTargetTagRequirementsGameplayEffectComponent& requirements =
		this->FindOrAddComponent<UTargetTagRequirementsGameplayEffectComponent>();
	requirements.OngoingTagRequirements.RequireTags.AddTag(AdventureGameplayTags::Gameplay_State_IsMoving);
}
