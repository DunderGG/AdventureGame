// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SneakEffect.generated.h"

/**
 * 
 */
UCLASS()
class EFFECTS_API USneakEffect : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float noiseMultiplier = 0.5f; // Half the amount of noise 
public:
	
};
