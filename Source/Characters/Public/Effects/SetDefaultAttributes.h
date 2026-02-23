// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SetDefaultAttributes.generated.h"

/**
 * 
 */
UCLASS()
class CHARACTERS_API USetDefaultAttributes : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	float defaultHealth = 100.0f;
	float defaultStamina = 100.0f;
	float defaultStrength = 10.0f;
	float defaultLevel = 1.0f;
public:
	void SetDefaultHealth(float newDefaultHealth) { defaultHealth = newDefaultHealth; }
	void SetDefaultStamina(float newDefaultStamina) { defaultStamina = newDefaultStamina; }
	void SetDefaultStrength(float newDefaultStrength) { defaultStrength = newDefaultStrength; }
	void SetDefaultLevel(float newDefaultLevel) { defaultLevel = newDefaultLevel; }
};
