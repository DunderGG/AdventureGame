// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityBase.h"
#include "GAJump.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UGAJump : public UGameplayAbilityBase
{
	GENERATED_BODY()
	
private:

protected:

public:
	UGAJump();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, const FGameplayEventData* triggerEventData) override;

	UFUNCTION()
	void onLandedCallback(EMovementMode newMovementMode);

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayTagContainer* sourceTags, const FGameplayTagContainer* targetTags, OUT FGameplayTagContainer* optionalRelevantTags) const override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo, bool bReplicateCancelAbility) override;

	virtual void InputReleased(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo) override;
};
