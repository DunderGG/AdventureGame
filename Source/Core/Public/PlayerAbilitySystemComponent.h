// Copyright (C) 2026 dunder.gg [GNU GPLv3]

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PlayerAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class COREDATA_API UPlayerAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UPlayerAbilitySystemComponent();

	bool areStartupEffectsApplied = false;
	bool areDefaultAbilitiesGiven = false;

protected:
	virtual void BeginPlay() override;
};
