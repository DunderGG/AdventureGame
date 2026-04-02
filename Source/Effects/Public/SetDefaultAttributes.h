// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SetDefaultAttributes.generated.h"

/**
 * 
 */
UCLASS()
class EFFECTS_API USetDefaultAttributes : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	float defaultHealth = 100.0f;
	float defaultStamina = 100.0f;
	float defaultStrength = 10.0f;
	float defaultLevel = 1.0f;
	float defaultMoveSpeed = 400.0f;
	//TODO Try using multipliers instead of flat values. Not sure how this combines with other effects.
	float defaultSprintSpeed = 2.0f;
	float defaultSneakSpeed = 0.5f;
	//TODO Not sure how "Noise" is gonna be used, maybe as a percentage?
	float defaultNoise = 100.0f;
public:
	void SetDefaultHealth(float newDefaultHealth) { defaultHealth = newDefaultHealth; }
	void SetDefaultStamina(float newDefaultStamina) { defaultStamina = newDefaultStamina; }
	void SetDefaultStrength(float newDefaultStrength) { defaultStrength = newDefaultStrength; }
	void SetDefaultLevel(float newDefaultLevel) { defaultLevel = newDefaultLevel; }
	void SetDefaultMoveSpeed(float newDefaultMoveSpeed) { defaultMoveSpeed = newDefaultMoveSpeed; }
	void SetDefaultSprintSpeed(float newDefaultSprintSpeed) { defaultSprintSpeed = newDefaultSprintSpeed; }
	void SetDefaultSneakSpeed(float newDefaultSneakSpeed) { defaultSneakSpeed = newDefaultSneakSpeed; }
	void SetDefaultNoise(float newDefaultNoise) { defaultNoise = newDefaultNoise; }
};
