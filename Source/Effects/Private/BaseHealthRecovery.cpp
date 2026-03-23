// Copyright dunder.gg. All Rights Reserved.


#include "BaseHealthRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"

void UBaseHealthRecovery::PostInitProperties()
{
	Super::PostInitProperties();

	// TODO: Don't think our tags are working, look into this.
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
	mod.Attribute = UPlayerAttributeSet::GetHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryRate);
	Modifiers.Add(mod);
}

