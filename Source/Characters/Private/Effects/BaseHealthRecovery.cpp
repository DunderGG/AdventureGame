// Copyright dunder.gg. All Rights Reserved.


#include "Effects/BaseHealthRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "AGCharacterAttributeSet.h"

void UBaseHealthRecovery::PostInitProperties()
{
	Super::PostInitProperties();
	const FName healthRecoveryTag = TEXT("Gameplay.State.IsBaseRecoveringHealth");

	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	const auto& tag = FGameplayTag::RequestGameplayTag(healthRecoveryTag);
	TagContainer.Added.AddTag(tag);
	component.SetAndApplyTargetTagChanges(TagContainer);

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1.0f;
	bExecutePeriodicEffectOnApplication = true;

	FGameplayModifierInfo mod;
	mod.Attribute = UAGCharacterAttributeSet::GetHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryRate);
	Modifiers.Add(mod);

	UE_LOG(LogTemp, Display, TEXT("********* Created a health regen startup effect"));
}

