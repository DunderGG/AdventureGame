// Copyright dunder.gg. All Rights Reserved.


#include "BaseStaminaRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectComponents/TargetTagRequirementsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"

void UBaseStaminaRecovery::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = recoveryPeriod;
	bExecutePeriodicEffectOnApplication = true;

	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;

	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryMagnitude);
	Modifiers.Add(mod);

	// Add the isRecoveringStamina tag to identify this state
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsBaseRecoveringStamina);
	component.SetAndApplyTargetTagChanges(tagContainer);

	UTargetTagRequirementsGameplayEffectComponent& requirements =
		this->FindOrAddComponent<UTargetTagRequirementsGameplayEffectComponent>();

	// This effect will be inhibited (paused) if the character has the "IsSprinting" tag
	requirements.OngoingTagRequirements.IgnoreTags.AddTag(AdventureGameplayTags::Gameplay_State_IsSprinting);
}

