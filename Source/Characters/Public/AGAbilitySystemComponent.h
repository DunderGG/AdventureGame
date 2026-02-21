// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UAGAbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
};
