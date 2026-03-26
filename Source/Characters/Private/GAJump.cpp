// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "GAJump.h"
#include "CharacterBase.h"
#include "AdventureGameplayTags.h"
#include "Logger.h"

UGAJump::UGAJump()
{
	abilityInputId = EAbilityInputId::Jump;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// Accessing AbilityTags directly is deprecated, use SetAssetTags instead.
	FGameplayTagContainer tags;
	tags.AddTag(AdventureGameplayTags::Gameplay_Ability_Jump);
	SetAssetTags(tags);
}

void UGAJump::ActivateAbility(const FGameplayAbilitySpecHandle handle, 
							  const FGameplayAbilityActorInfo* actorInfo, 
							  const FGameplayAbilityActivationInfo activationInfo, 
							  const FGameplayEventData* triggerEventData)
{
	if (HasAuthorityOrPredictionKey(actorInfo, &activationInfo))
	{
		if (!CommitAbility(handle, actorInfo, activationInfo))
		{
			EndAbility(handle, actorInfo, activationInfo, true, true);
			return;
		}

		Logger::addMessage(TEXT("UGAJump::ActivateAbility(): Jumping"), SEVERITY::Info);
		ACharacter* character = CastChecked<ACharacter>(actorInfo->AvatarActor.Get());
		character->Jump();
	}
}

bool UGAJump::CanActivateAbility(const FGameplayAbilitySpecHandle handle, 
								 const FGameplayAbilityActorInfo* actorInfo, 
								 const FGameplayTagContainer* sourceTags, 
								 const FGameplayTagContainer* targetTags, 
								 OUT FGameplayTagContainer* optionalRelevantTags) const
{
	if (!Super::CanActivateAbility(handle, actorInfo, sourceTags, targetTags, optionalRelevantTags))
	{
		return false;
	}

	const ACharacterBase* character = CastChecked<ACharacterBase>(actorInfo->AvatarActor.Get(), ECastCheckedType::NullAllowed);
	
	return character && character->CanJump();
}

/*
* Destroys instanced-per-execution abilities. 
* Instance-per-actor abilities should 'reset'. 
* Any active ability state tasks receive the 'OnAbilityStateInterrupted' event.
*/
void UGAJump::CancelAbility(const FGameplayAbilitySpecHandle handle, 
							const FGameplayAbilityActorInfo* actorInfo, 
							const FGameplayAbilityActivationInfo activationInfo, 
							bool replicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGAJump::CancelAbility, handle, actorInfo, activationInfo, replicateCancelAbility));
		return;
	}

	Super::CancelAbility(handle, actorInfo, activationInfo, replicateCancelAbility);

	ACharacter* character = CastChecked<ACharacter>(actorInfo->AvatarActor.Get());
	character->StopJumping();
}

void UGAJump::InputReleased(const FGameplayAbilitySpecHandle handle, 
							const FGameplayAbilityActorInfo* actorInfo, 
							const FGameplayAbilityActivationInfo activationInfo)
{
	if (actorInfo && actorInfo->AvatarActor.IsValid())
	{
		CancelAbility(handle, actorInfo, activationInfo, true);
	}
}
