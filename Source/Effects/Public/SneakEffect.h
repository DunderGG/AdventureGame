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
	float sneakSpeedMultiplier = 200; // Set speed to 200
	//TODO Add a "noise" attribute which is modified by sneak and speed.
public:
	
};
