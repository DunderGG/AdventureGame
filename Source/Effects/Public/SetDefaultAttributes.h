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
	float defaultSprintSpeed = 800.0f;
	float defaultSneakSpeed = 200.0f;
public:
	void SetDefaultHealth(float newDefaultHealth) { defaultHealth = newDefaultHealth; }
	void SetDefaultStamina(float newDefaultStamina) { defaultStamina = newDefaultStamina; }
	void SetDefaultStrength(float newDefaultStrength) { defaultStrength = newDefaultStrength; }
	void SetDefaultLevel(float newDefaultLevel) { defaultLevel = newDefaultLevel; }
	void SetDefaultMoveSpeed(float newDefaultMoveSpeed) { defaultMoveSpeed = newDefaultMoveSpeed; }
	void SetDefaultSprintSpeed(float newDefaultSprintSpeed) { defaultSprintSpeed = newDefaultSprintSpeed; }
	void SetDefaultSneakSpeed(float newDefaultSneakSpeed) { defaultSneakSpeed = newDefaultSneakSpeed; }
};
