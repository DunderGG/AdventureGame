// Copyright dunder.gg. All Rights Reserved.


#include "BaseHealthRecovery.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "PlayerAttributeSet.h"
#include "AdventureGameplayTags.h"	

void UBaseHealthRecovery::PostInitProperties()
{
	Super::PostInitProperties();

	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = recoveryPeriod;
	bExecutePeriodicEffectOnApplication = true;

	StackingType = EGameplayEffectStackingType::AggregateByTarget;
	StackLimitCount = 1;

	FGameplayModifierInfo mod;
	mod.Attribute = UPlayerAttributeSet::GetHealthAttribute();
	mod.ModifierOp = EGameplayModOp::Additive;
	mod.ModifierMagnitude = FScalableFloat(recoveryMagnitude);
	Modifiers.Add(mod);

	// Add the isRecoveringHealth tag to identify this state
	FInheritedTagContainer tagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& component = this->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	tagContainer.Added.AddTag(AdventureGameplayTags::Gameplay_State_IsBaseRecoveringHealth);
	component.SetAndApplyTargetTagChanges(tagContainer);
}

