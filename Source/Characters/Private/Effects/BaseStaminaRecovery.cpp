// Copyright dunder.gg. All Rights Reserved.


#include "Effects/BaseStaminaRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "AGCharacterAttributeSet.h"

void UBaseStaminaRecovery::PostInitProperties()
{
	Super::PostInitProperties();

	const FName staminaRecoveryTag = TEXT("Gameplay.State.IsBaseRecoveringStamina");
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	const auto& tag = FGameplayTag::RequestGameplayTag(staminaRecoveryTag);
	TagContainer.Added.AddTag(tag);
	component.SetAndApplyTargetTagChanges(TagContainer);

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.0f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo mod;
	mod.Attribute = UAGCharacterAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryRate);
	Modifiers.Add(mod);
}

