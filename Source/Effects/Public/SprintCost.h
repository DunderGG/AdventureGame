// Copyright dunder.gg. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "SprintCost.generated.h"

/**
 *
 */
UCLASS()
class EFFECTS_API USprintCost : public UGameplayEffect
{
	GENERATED_BODY()

private:
	virtual void PostInitProperties() override;
protected:
	// TODO: Not sure what UPROPERTY specifiers I need here, or if I need any at all.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Expense")
	float costMagnitude = -0.5f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS | Expense")
	float costPeriod = 0.1f;
public:

};
