// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#include "SprintStaminaExpense.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"

void USprintStaminaExpense::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = expensePeriod;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(expenseMagnitude);
	Modifiers.Add(mod);

	// Add the isSprinting tag to identify this state
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsSprinting);
	component.SetAndApplyTargetTagChanges(tagContainer);
}