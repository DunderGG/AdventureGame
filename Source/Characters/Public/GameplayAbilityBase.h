// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityTypes.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
private:

protected:

public:
	UGameplayAbilityBase();

	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilitySpec& spec) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	bool activateAbilityWhenGranted = false;

	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EAbilityInputId abilityInputId = EAbilityInputId::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	EAbilityInputId abilityId = EAbilityInputId::None;
};
