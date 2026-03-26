// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "GameplayAbilityBase.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"


UGameplayAbilityBase::UGameplayAbilityBase()
{
	// Keep abilities instanced per actor by default.
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Tags that can block an ability from activating.
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.State.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Gameplay.State.Debuff.Stun")));
}

/* 
*	According to Epic: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
*/
void UGameplayAbilityBase::OnAvatarSet(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec)
{
	Super::OnAvatarSet(actorInfo, spec);

	if (activateAbilityWhenGranted)
	{
		actorInfo->AbilitySystemComponent->TryActivateAbility(spec.Handle, false);
	}
}
