// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SprintEffect.generated.h"

/**
 * 
 */
UCLASS()
class EFFECTS_API USprintEffect : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float noiseMultiplier = 2.0f; // Double the amount of noise
public:
	
};
