// Copyright dunder.gg. All Rights Reserved.


#include "BaseStaminaRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"

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
	mod.Attribute = UPlayerAttributeSet::GetStaminaAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryRate);
	Modifiers.Add(mod);
}

