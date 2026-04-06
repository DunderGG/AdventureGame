// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SprintCost.h"
#include "PlayerAttributeSet.h"
#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "AdventureGameplayTags.h"

/*
* TODO: Stamina drains even if standing still, 
* because the effect is applied and active, 
* but it should only drain when the character is actually sprinting.
*/
void USprintCost::PostInitProperties()
{
	Super::PostInitProperties();

	// The effect should run "Infinite" (until removed), and happen every <costPeriod> seconds, starting on application.
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = costPeriod;
	bExecutePeriodicEffectOnApplication = true;

	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;

	// The Modifier for Stamina cost
	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(costMagnitude);
	Modifiers.Add(mod);

	// Add a requirement that the target must have the isSprinting tag to be affected by this effect.
	UTargetTagRequirementsGameplayEffectComponent& requirements =
		this->FindOrAddComponent<UTargetTagRequirementsGameplayEffectComponent>();

	// The effect only "ticks" if both tags are present.
	requirements.OngoingTagRequirements.RequireTags.AddTag(AdventureGameplayTags::Gameplay_State_IsSprinting);
	requirements.OngoingTagRequirements.RequireTags.AddTag(AdventureGameplayTags::Gameplay_State_IsMoving);

	requirements.OngoingTagRequirements.IgnoreTags.AddTag(AdventureGameplayTags::Gameplay_State_IsAirborne);
}
