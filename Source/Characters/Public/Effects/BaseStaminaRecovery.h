// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "BaseStaminaRecovery.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API UBaseStaminaRecovery : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	// TODO: Not sure what UPROPERTY specifiers I need here, or if I need any at all.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Recovery")
	float recoveryRate = 5.0f;
public:
	
};
