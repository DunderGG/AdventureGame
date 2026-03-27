// Copyright (C) 2026 dunder.gg [GNU GPLv3]


#include "GAJump.h"
#include "CharacterBase.h"
#include "AdventureGameplayTags.h"
#include "Logger.h"
#include "JumpCost.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

/*
* A lot of code to basically just implement jumping,
* but it serves as a good example of how to create your own Gameplay Abilities for GAS.
* 
* 1. It sets the AbilityTags as well as the ActivationOwnedTags (which we see in the editor).
* 2. It makes sure we are allowed to trigger the jump ability.
* 3. It calls the character to jump, and when we land again there is a callback to end the ability.
*/
UGAJump::UGAJump()
{
	// A gameplay effect that defines the cost of the ability.
	// This is recommended over manually checking the AttributeSet if stamina < 10.
	CostGameplayEffectClass = UJumpCost::StaticClass();

	abilityInputId = EAbilityInputId::Jump;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	// Accessing AbilityTags directly is deprecated, use SetAssetTags instead.
	FGameplayTagContainer tags;
	tags.AddTag(AdventureGameplayTags::Gameplay_Ability_Jump);
	SetAssetTags(tags);

	ActivationOwnedTags.AddTag(AdventureGameplayTags::Gameplay_Ability_Jump);
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


		// Create a task to wait until the movement mode changes 
		// (e.g., from Falling back to Walking)
		UAbilityTask_WaitMovementModeChange* waitLandingTask = 
			UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, MOVE_Walking);

		// Bind a lambda to end the ability when the task completes
		waitLandingTask->OnChange.AddDynamic(this, &UGAJump::onLandedCallback);
		waitLandingTask->ReadyForActivation();
	}
}

void UGAJump::onLandedCallback(EMovementMode newMovementMode)
{
	// Now that we've landed, we can end the ability. Otherwise we can't jump again.
	// This will also automatically remove the 'Gameplay_Ability_Jump' tag.
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


bool UGAJump::CanActivateAbility(const FGameplayAbilitySpecHandle handle, 
								 const FGameplayAbilityActorInfo* actorInfo, 
								 const FGameplayTagContainer* sourceTags, 
								 const FGameplayTagContainer* targetTags, 
								 OUT FGameplayTagContainer* optionalRelevantTags) const
{
	// 1. Check base requirements (tags, etc.)
	if (!Super::CanActivateAbility(handle, actorInfo, sourceTags, targetTags, optionalRelevantTags))
	{
		return false;
	}

	// 2. Built-in GAS Check: Verify we can afford the cost assigned to CostGameplayEffectClass
	// While CommitAbility handles the check during activation, 
	// updating CanActivateAbility allows the UI or other systems 
	// to know the ability is "unavailable" (greyed out) before
	// the player even tries to press the button.
	if (!CheckCost(handle, actorInfo, optionalRelevantTags))
	{
		return false;
	}

	// 3. Custom check: Can the character jump?
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
